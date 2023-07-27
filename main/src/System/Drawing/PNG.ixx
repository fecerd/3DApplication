export module PNG;
import Objects;
import Image;
import File;
import Vector;
import Zlib;
import Huffman;
import Memory;
import Math;

export namespace System::Drawing {
	class PNG : public Object {
	private:
		struct IHDR {
		public:
			enum class ColourType : uint8_t {
				GrayScale = 0,
				TrueColour = 2,
				IndexedColour = 3,
				GrayScaleWithAlpha = 4,
				TrueColourWithAlpha = 6
			};
		public:
			uint32_t width = 0;
			uint32_t height = 0;
			uint8_t bitDepth = 0;
			ColourType colourType = ColourType::GrayScale;
			uint8_t compressionMethod = 0;
			uint8_t filterMethod = 0;
			uint8_t interlaceMethod = 0;
		public:
			void ByteOrder() noexcept {
				using Memory::ByteOrder;
				width = ByteOrder(width);
				height = ByteOrder(height);
				bitDepth = ByteOrder(bitDepth);
				colourType = ByteOrder(colourType);
				compressionMethod = ByteOrder(compressionMethod);
				filterMethod = ByteOrder(filterMethod);
				interlaceMethod = ByteOrder(interlaceMethod);
			}
			constexpr uint8_t GetChannelCount() const noexcept {
				if (colourType == ColourType::GrayScale) return 1;
				else if (colourType == ColourType::GrayScaleWithAlpha) return 2;
				else if (colourType == ColourType::IndexedColour) return 1;
				else if (colourType == ColourType::TrueColour) return 3;
				else return 4;
			}
		};
		struct Chunk {
		private:
			static constexpr char8_t TYPE_IHDR[4] = { u8'I', u8'H', u8'D', u8'R' };
			static constexpr char8_t TYPE_IDAT[4] = { u8'I', u8'D', u8'A', u8'T' };
			static constexpr char8_t TYPE_IEND[4] = { u8'I', u8'E', u8'N', u8'D' };
		public:
			uint32_t length = 0;
			char8_t type[4] = { 0,0,0,0 };
			union {
				uint8_t* data = nullptr;
				IHDR* ihdr;
			};
			uint32_t crc = 0;
		public:
			Chunk() noexcept = default;
			Chunk(const Chunk&) = delete;
			~Chunk() {
				delete[] data;
				data = nullptr;
				length = 0;
			}
		public:
			static Chunk* CreateChunk(IO::FileStream& file, Vector<uint8_t>& idat) noexcept {
				Chunk* ret = new Chunk();
				file.ReadBE(ret->length);
				file.ReadBE(ret->type);
				if (ret->IsIDAT()) idat.AddRange(file, ret->length);
				else {
					ret->data = new uint8_t[ret->length];
					file.Read(ret->data, ret->length);
					if (ret->IsIHDR()) ret->ihdr->ByteOrder();
				}
				file.ReadBE(ret->crc);
				return ret;
			}
		public:
			bool IsIHDR() const noexcept { return Memory::Compare(type, TYPE_IHDR, sizeof(TYPE_IHDR)); }
			bool IsIDAT() const noexcept { return Memory::Compare(type, TYPE_IDAT, sizeof(TYPE_IDAT)); }
			bool IsIEnd() const noexcept { return Memory::Compare(type, TYPE_IEND, sizeof(TYPE_IEND)); }
		};
	private:
		static constexpr uint8_t MAGIC_NUMBER[8] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
		bool isLoaded = false;
		uint8_t magicNumber[8];
		Vector<Chunk*> chunks;
		Vector<uint8_t> idatData;
	public:
		PNG() = delete;
		explicit PNG(const String& fileName) noexcept : PNG(System::IO::FileStream(fileName, System::IO::OpenMode::IN_BINARY)) {}
		explicit PNG(System::IO::FileStream&& file) noexcept {
			if (!file) return;
			file.ReadBE(magicNumber);
			if (!Memory::Compare(magicNumber, MAGIC_NUMBER, sizeof(magicNumber))) return;
			while (!file.IsEOF()) {
				Chunk* tmp = Chunk::CreateChunk(file, idatData);
				chunks.Add(tmp);
				if (tmp->IsIEnd()) {
					isLoaded = true;
					break;
				}
			}
			file.Close();
		}
		~PNG() noexcept {
			isLoaded = false;
			for (size_t i = 0, end = chunks.Count(); i < end; ++i) delete chunks[i];
			chunks.Clear();
		}
	public:
		static bool IsPNG(System::IO::FileStream& file) noexcept {
			System::IO::StreamPos pos = file.TellPos();
			uint8_t magic[8]{};
			file.ReadBE(magic);
			file.Clear();
			file.Seek(pos);
			return Memory::Compare(magic, MAGIC_NUMBER, sizeof(magic));
		}
	private:
		IHDR* GetIHDR() const noexcept {
			for (size_t i = 0, end = chunks.Count(); i < end; ++i) if (chunks[i]->IsIHDR()) return chunks[i]->ihdr;
			return nullptr;
		}
		Vector<uint8_t> GetData() noexcept {
			IHDR* ihdr = GetIHDR();
			if (!ihdr) return Vector<uint8_t>();
			const size_t widthBit = static_cast<size_t>(ihdr->width) * ihdr->bitDepth * ihdr->GetChannelCount();
			const size_t widthByte = widthBit % 8 == 0 ? widthBit / 8 : widthBit / 8 + 1;
			const size_t dataSize = (widthByte + 1) * ihdr->height;	//(一行あたりのバイト数 + フィルター1バイト) * 行数
			Compression::ZlibView zlib(idatData.Items(), idatData.Count());
			uint8_t* compressedData = zlib.GetCompressedData();
			if (!compressedData) return Vector<uint8_t>();
			return Compression::HuffmanDecoder::Decode(compressedData, zlib.GetCompressedDataSize(), dataSize);
		}
	private:
		static void RemoveFilter(Vector<uint8_t>& raw, size_t width, uint8_t bitDepth, uint8_t channelPerPixel) noexcept {
			const size_t widthBit = width * bitDepth * channelPerPixel;
			const size_t widthByte = widthBit % 8 == 0 ? widthBit / 8 : widthBit / 8 + 1;
			const size_t bytesPerPixel = bitDepth < 8 ? 1 : (bitDepth / 8) * channelPerPixel;
			uint8_t* data = raw.Items();
			const size_t dataCount = raw.Count();
			for (size_t i = 0; i < dataCount;) {
				uint8_t filter = data[i++];
				const size_t lineEnd = i + widthByte;	//次のラインのフィルターを指すインデックス
				if (lineEnd > dataCount) break;
				if (filter == 1) {
					i += bytesPerPixel;	//1ピクセル目を飛ばす
					for (; i < lineEnd; ++i) {
						data[i] += data[i - bytesPerPixel];
					}
				}
				else if (filter == 2) {
					for (; i < lineEnd; ++i) data[i] += data[i - widthByte - 1];
				}
				else if (filter == 3) {
					for (size_t j = 0; j < bytesPerPixel; ++j, ++i) data[i] += data[i - widthByte - 1] / 2;	//1ピクセル目は上段のみ参照
					for (; i < lineEnd; ++i) data[i] += (data[i - bytesPerPixel] + data[i - widthByte - 1]) / 2;
				}
				else if (filter == 4) {
					for (size_t j = 0; j < bytesPerPixel; ++j, ++i) data[i] += data[i - widthByte - 1];	//1ピクセル目は真上のバイトのみ参照
					for (; i < lineEnd; ++i) {
						uint8_t a = data[i - bytesPerPixel];	//左
						uint8_t b = data[i - widthByte - 1];	//真上
						uint8_t c = data[i - widthByte - 1 - bytesPerPixel];	//左上
						int32_t p = a + b - c;
						uint32_t pa = Math::Abs(p - a);
						uint32_t pb = Math::Abs(p - b);
						uint32_t pc = Math::Abs(p - c);
						if (pa <= pb && pa <= pc) data[i] += a;
						else if (pb <= pc) data[i] += b;
						else data[i] += c;
					}
				}
				else i += widthByte;
			}
		}
	private:
		static constexpr uint8_t ExtendBit(uint8_t bit, uint8_t bitCount) noexcept {
			switch (bitCount) {
			case 1: return bit & 1 ? 255 : 0;
			case 2: {
				uint8_t tmp = bit & 3;
				return (tmp << 6) | (tmp << 4) | (tmp << 2) | tmp;
			}
			case 4: return ((bit & 0x0f) << 4) | (bit & 0x0f);
			default: return bit;
			}
		}
		template<bool RangeCheck = false>
		inline static size_t Set1bitGrayByte(Pixel* begin, uint8_t byte, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 7, 1);
			begin[0] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte >> 6, 1);
			begin[1] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 2;
			val = ExtendBit(byte >> 5, 1);
			begin[2] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 3;
			val = ExtendBit(byte >> 4, 1);
			begin[3] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 4;
			val = ExtendBit(byte >> 3, 1);
			begin[4] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 5;
			val = ExtendBit(byte >> 2, 1);
			begin[5] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 6;
			val = ExtendBit(byte >> 1, 1);
			begin[6] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 7;
			val = ExtendBit(byte, 1);
			begin[7] = Pixel(val, val, val);
			return 8;
		}
		template<bool RangeCheck = false>
		inline static size_t Set2bitGrayByte(Pixel* begin, uint8_t byte, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 6, 2);
			begin[0] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte >> 4, 2);
			begin[1] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 2;
			val = ExtendBit(byte >> 2, 2);
			begin[2] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 3;
			val = ExtendBit(byte, 2);
			begin[3] = Pixel(val, val, val);
			return 4;
		}
		template<bool RangeCheck = false>
		inline static size_t Set4bitGrayByte(Pixel* begin, uint8_t byte, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 4, 4);
			begin[0] = Pixel(val, val, val);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte, 4);
			begin[1] = Pixel(val, val, val);
			return 2;
		}
		template<uint8_t bitDepth>
		inline static void DecodeGrayScale(Image& image, Vector<uint8_t>& raw) noexcept {
			uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			Pixel* pixels = image.Data();
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			for (size_t i = 0; i < rawDataCount; ++i) {
				if (currentX == width) {
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
					if (nextY >= height) break;
				}
				else {
					uint8_t tmp = rawData[i];
					if constexpr (bitDepth == 1) {
						if (currentX + 7 < width) currentX += Set1bitGrayByte(lineFirst + currentX, tmp);
						else currentX += Set1bitGrayByte<true>(lineFirst + currentX, tmp, width - currentX);
					}
					else if constexpr (bitDepth == 2) {
						if (currentX + 3 < width) currentX += Set2bitGrayByte(lineFirst + currentX, tmp);
						else currentX += Set2bitGrayByte<true>(lineFirst + currentX, tmp, width - currentX);
					}
					else if constexpr (bitDepth == 4) {
						if (currentX + 1 < width) currentX += Set4bitGrayByte(lineFirst + currentX, tmp);
						else currentX += Set4bitGrayByte<true>(lineFirst + currentX, tmp, width - currentX);
					}
					else if constexpr (bitDepth == 8) {
						lineFirst[currentX++] = Pixel(tmp, tmp, tmp);
					}
					else if constexpr (bitDepth == 16) {
						uint8_t tmp2 = rawData[++i];
						if (Math::Abs(tmp - tmp2) <= 128) lineFirst[currentX++] = Pixel(tmp, tmp, tmp);
						else lineFirst[currentX++] = tmp > tmp2 ? Pixel(tmp - 1, tmp - 1, tmp - 1) : Pixel(tmp + 1, tmp + 1, tmp + 1);
					}
				}
			}
		}
	private:
		template<uint8_t bitDepth>
		inline static void DecodeTrueColour(Image& image, Vector<uint8_t>& raw) noexcept {
			uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			Pixel* pixels = image.Data();
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			for (size_t i = 0; i < rawDataCount; ++i) {
				if (currentX == width) {
					if (nextY >= height) break;
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
				}
				else {
					if constexpr (bitDepth == 8) {
						lineFirst[currentX++] = Pixel(rawData[i], rawData[i + 1], rawData[i + 2]);
						i += 2;
					}
					else if constexpr (bitDepth == 16) {
						uint8_t r1 = rawData[i];
						uint8_t r2 = rawData[++i];
						if (Math::Abs(r1 - r2) > 128) r1 = r1 > r2 ? r1 - 1 : r1 + 1;
						uint8_t g1 = rawData[++i];
						uint8_t g2 = rawData[++i];
						if (Math::Abs(g1 - g2) > 128) g1 = g1 > g2 ? g1 - 1 : g1 + 1;
						uint8_t b1 = rawData[++i];
						uint8_t b2 = rawData[++i];
						if (Math::Abs(b1 - b2) > 128) b1 = b1 > b2 ? b1 - 1 : b1 + 1;
						lineFirst[currentX++] = Pixel(r1, g1, b1);
					}
				}
			}
		}
		template<uint8_t bitDepth>
		inline static void DecodeTrueColourWithAlpha(Image& image, Vector<uint8_t>& raw) noexcept {
			uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			Pixel* pixels = image.Data();
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			for (size_t i = 0; i < rawDataCount; ++i) {
				if (currentX == width) {
					if (nextY >= height) break;
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
				}
				else {
					if constexpr (bitDepth == 8) {
						lineFirst[currentX++] = Pixel(rawData[i], rawData[i + 1], rawData[i + 2], rawData[i + 3]);
						i += 3;
					}
					else if constexpr (bitDepth == 16) {
						uint8_t r1 = rawData[i];
						uint8_t r2 = rawData[++i];
						if (Math::Abs(r1 - r2) > 128) r1 = r1 > r2 ? r1 - 1 : r1 + 1;
						uint8_t g1 = rawData[++i];
						uint8_t g2 = rawData[++i];
						if (Math::Abs(g1 - g2) > 128) g1 = g1 > g2 ? g1 - 1 : g1 + 1;
						uint8_t b1 = rawData[++i];
						uint8_t b2 = rawData[++i];
						if (Math::Abs(b1 - b2) > 128) b1 = b1 > b2 ? b1 - 1 : b1 + 1;
						uint8_t a1 = rawData[++i];
						uint8_t a2 = rawData[++i];
						if (Math::Abs(a1 - a2) > 128) a1 = a1 > a2 ? a1 - 1 : a1 + 1;
						lineFirst[currentX++] = Pixel(r1, g1, b1, a1);
					}
				}
			}
		}
	public:
		Image ToImage() noexcept {
			IHDR* ihdr = GetIHDR();
			if (!ihdr) return Image();
			Vector<uint8_t> raw = GetData();
			RemoveFilter(raw, ihdr->width, ihdr->bitDepth, ihdr->GetChannelCount());
			Image ret(ihdr->width, ihdr->height);
			if (ihdr->colourType == IHDR::ColourType::GrayScale) {
				switch (ihdr->bitDepth) {
				case 1:
					DecodeGrayScale<1>(ret, raw);
					break;
				case 2:
					DecodeGrayScale<2>(ret, raw);
					break;
				case 4:
					DecodeGrayScale<4>(ret, raw);
					break;
				case 8:
					DecodeGrayScale<8>(ret, raw);
					break;
				case 16:
					DecodeGrayScale<16>(ret, raw);
					break;
				default:
					break;
				}
			}
			else if (ihdr->colourType == IHDR::ColourType::TrueColour) {
				if (ihdr->bitDepth == 8) DecodeTrueColour<8>(ret, raw);
				else if (ihdr->bitDepth == 16) DecodeTrueColour<16>(ret, raw);
			}
			else if (ihdr->colourType == IHDR::ColourType::TrueColourWithAlpha) {
				if (ihdr->bitDepth == 8) DecodeTrueColourWithAlpha<8>(ret, raw);
				else if (ihdr->bitDepth == 16) DecodeTrueColourWithAlpha<16>(ret, raw);
			}
			return ret;
		}
	public:
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const PNG&>(obj) : false; }
		Type GetType() const noexcept override { return Type(u"System::Drawing::PNG"); }
		String ToString() const noexcept override { return String(u"PNG Image"); }
		uint32_t GetTypeID() const noexcept override { return GetID<PNG>(); }
	};
}