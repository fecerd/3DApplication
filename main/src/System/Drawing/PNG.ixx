export module PNG;
export import Objects;
import Vector;
import Memory;
import Math;
import File;
import Zlib;
import Huffman;
export import Image;

/*
	16bitのチャンネルはImageクラスに変換する際に、8bitにスケールダウンされる。
	スケールダウンは下位バイトの切り捨て(8bit右シフト)で実装している。
	なお、最も正確なスケーリング法は
		MaxInSample = 2^(変換前bit数) - 1;
		MaxOutSample = 2^(変換後bit数) - 1;
		output = floor((input * MaxOutSample / MaxInSample) + 0.5);
	による線形変換である。

	スケールアップは左シフト&ビットのリピートで実装している。(ExtendBit()関数)
		例: 0b101' (3bit) -> 0b101'101'10 (9bit)
	なお、最も正確なスケーリング法は
		MaxInSample = 2^(変換前bit数) - 1 = 変換前の範囲の最大値;
		MaxOutSample = 2^(変換後bit数) - 1 = 変換後の範囲の最大値;
		output = floor((input * MaxOutSample / MaxInSample) + 0.5);
	による線形変換である。
	この方法の場合、データの最大値が2の累乗-1でなくても機能する。
*/

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
		struct PLTE {
			struct PaletteData {
				uint8_t red;
				uint8_t green;
				uint8_t blue;
			};
		public:
			PaletteData first;
		public:
			const PaletteData* GetPaletteArray() const noexcept {
				return &first;
			}
		};
		struct tRNS {
			struct TrueColourAlpha {
				uint16_t red;
				uint16_t green;
				uint16_t blue;
			};
		public:
			union {
				uint8_t first;
				uint16_t alpha;
				TrueColourAlpha trueColourAlpha;
			};
		public:
			const uint8_t* GetPaletteAlphaArray() const noexcept {
				return &first;
			}
			uint16_t GetAlpha() const noexcept {
				return Memory::ByteOrder(alpha);
			}
			void GetTrueColourAlpha(uint16_t& red, uint16_t& green, uint16_t& blue) const noexcept {
				red = Memory::ByteOrder(trueColourAlpha.red);
				green = Memory::ByteOrder(trueColourAlpha.green);
				blue = Memory::ByteOrder(trueColourAlpha.blue);
			}
		};
		struct Chunk {
		private:
			static constexpr char8_t TYPE_IHDR[4] = { u8'I', u8'H', u8'D', u8'R' };
			static constexpr char8_t TYPE_IDAT[4] = { u8'I', u8'D', u8'A', u8'T' };
			static constexpr char8_t TYPE_PLTE[4] = { u8'P', u8'L', u8'T', u8'E' };
			static constexpr char8_t TYPE_tRNS[4] = { u8't', u8'R', u8'N', u8'S' };
			static constexpr char8_t TYPE_IEND[4] = { u8'I', u8'E', u8'N', u8'D' };
		public:
			uint32_t length = 0;
			char8_t type[4] = { 0,0,0,0 };
			union {
				uint8_t* data = nullptr;
				IHDR* ihdr;
				PLTE* plte;
				tRNS* trns;
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
			bool IsPLTE() const noexcept { return Memory::Compare(type, TYPE_PLTE, sizeof(TYPE_PLTE)); }
			bool IstRNS() const noexcept { return Memory::Compare(type, TYPE_tRNS, sizeof(TYPE_tRNS)); }
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
		PLTE* GetPLTE(uint32_t& paletteCount) const noexcept {
			paletteCount = 0;
			for (size_t i = 0, end = chunks.Count(); i < end; ++i) {
				if (chunks[i]->IsPLTE()) {
					paletteCount = chunks[i]->length / 3;
					return chunks[i]->plte;
				}
			}
			return nullptr;
		}
		tRNS* GettRNS(uint32_t& length) const noexcept {
			length = 0;
			for (size_t i = 0, end = chunks.Count(); i < end; ++i) {
				if (chunks[i]->IstRNS()) {
					length = chunks[i]->length;
					return chunks[i]->trns;
				}
			}
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
		inline static size_t Set1bitGrayByte(Pixel* begin, uint8_t byte, uint8_t alphaValLt4bit, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 7, 1);
			begin[0] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte >> 6, 1);
			begin[1] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 2;
			val = ExtendBit(byte >> 5, 1);
			begin[2] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 3;
			val = ExtendBit(byte >> 4, 1);
			begin[3] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 4;
			val = ExtendBit(byte >> 3, 1);
			begin[4] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 5;
			val = ExtendBit(byte >> 2, 1);
			begin[5] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 6;
			val = ExtendBit(byte >> 1, 1);
			begin[6] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 7;
			val = ExtendBit(byte, 1);
			begin[7] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			return 8;
		}
		template<bool RangeCheck = false>
		inline static size_t Set2bitGrayByte(Pixel* begin, uint8_t byte, uint8_t alphaValLt4bit, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 6, 2);
			begin[0] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte >> 4, 2);
			begin[1] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 2;
			val = ExtendBit(byte >> 2, 2);
			begin[2] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 3;
			val = ExtendBit(byte, 2);
			begin[3] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			return 4;
		}
		template<bool RangeCheck = false>
		inline static size_t Set4bitGrayByte(Pixel* begin, uint8_t byte, uint8_t alphaValLt4bit, size_t max = 0) noexcept {
			if constexpr (RangeCheck) if (!max) return 0;
			uint8_t val = ExtendBit(byte >> 4, 4);
			begin[0] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			if constexpr (RangeCheck) if (!--max) return 1;
			val = ExtendBit(byte, 4);
			begin[1] = Pixel(val, val, val, val == alphaValLt4bit ? 0 : 255);
			return 2;
		}
		template<uint8_t bitDepth>
		inline static void DecodeGrayScale(Image& image, Vector<uint8_t>& raw, const tRNS* trns, uint32_t trnsLength) noexcept {
			const bool alphaExists = trns && trnsLength;
			const uint16_t alphaVal = alphaExists ? trns->GetAlpha() : 0;
			const uint8_t alphaValLt4bit = static_cast<uint8_t>(alphaExists ? alphaVal : 0x0010);
			uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			Pixel* pixels = image.Data();
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			for (size_t i = 0; i < rawDataCount; ++i) {
				//各行1バイト目はフィルターに使用されているため飛ばす必要がある
				if (currentX == width) {
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
					if (nextY > height) break;
				}
				else {
					uint8_t tmp = rawData[i];
					if constexpr (bitDepth == 1) {
						if (currentX + 7 < width) currentX += Set1bitGrayByte(lineFirst + currentX, tmp, alphaValLt4bit);
						else currentX += Set1bitGrayByte<true>(lineFirst + currentX, tmp, alphaValLt4bit, width - currentX);
					}
					else if constexpr (bitDepth == 2) {
						if (currentX + 3 < width) currentX += Set2bitGrayByte(lineFirst + currentX, tmp, alphaValLt4bit);
						else currentX += Set2bitGrayByte<true>(lineFirst + currentX, tmp, alphaValLt4bit, width - currentX);
					}
					else if constexpr (bitDepth == 4) {
						if (currentX + 1 < width) currentX += Set4bitGrayByte(lineFirst + currentX, tmp, alphaValLt4bit);
						else currentX += Set4bitGrayByte<true>(lineFirst + currentX, tmp, alphaValLt4bit, width - currentX);
					}
					else if constexpr (bitDepth == 8) {
						lineFirst[currentX++] = Pixel(tmp, tmp, tmp, alphaExists && (tmp == alphaVal) ? 0 : 255);
					}
					else if constexpr (bitDepth == 16) {
						uint8_t tmp2 = rawData[++i];
						uint16_t val16 = Memory::ByteOrder(static_cast<uint16_t>((tmp << 8) | tmp2));
						lineFirst[currentX++] = Pixel(tmp, tmp, tmp, alphaExists && (val16 == alphaVal) ? 0 : 255);
					}
				}
			}
		}
	private:
		template<uint8_t bitDepth>
		inline static void DecodeIndexedColour(Image& image, Vector<uint8_t>& raw, const PLTE& plte, uint32_t paletteCount, const tRNS* trns, uint32_t trnsLength) noexcept {
			const uint8_t* trnsArray = trns && trnsLength ? trns->GetPaletteAlphaArray() : nullptr;
			const PLTE::PaletteData* palettes = plte.GetPaletteArray();
			if (!palettes) return;
			Pixel* pixels = image.Data();
			if (!pixels) return;
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			const uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			for (size_t i = 0; i < rawDataCount; ++i) {
				//各行1バイト目はフィルターに使用されているため飛ばす必要がある
				if (currentX == width) {
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
					if (nextY > height) {
						break;
					}
				}
				else {
					uint8_t data = rawData[i];
					if constexpr (bitDepth == 1) {
						for (int n = 8; n-- > 0;) {
							uint8_t id = static_cast<uint8_t>((data >> n) & 0x01);
							if (paletteCount > id) {
								const PLTE::PaletteData& palette = palettes[id];
								uint8_t alpha = (trnsArray && trnsLength > id) ? trnsArray[id] : 255;
								lineFirst[currentX] = Pixel(palette.red, palette.green, palette.blue, alpha);
							}
							++currentX;
							if (currentX == width) break;
						}
					}
					else if constexpr (bitDepth == 2) {
						for (int n = 4; n-- > 0;) {
							uint8_t id = static_cast<uint8_t>((data >> (n << 1)) & 0x03);
							if (paletteCount > id) {
								const PLTE::PaletteData& palette = palettes[id];
								uint8_t alpha = (trnsArray && trnsLength > id) ? trnsArray[id] : 255;
								lineFirst[currentX] = Pixel(palette.red, palette.green, palette.blue, alpha);
							}
							++currentX;
							if (currentX == width) break;
						}
					}
					else if constexpr (bitDepth == 4) {
						uint8_t ids[2] = {
							static_cast<uint8_t>((data >> 4) & 0x0f),
							static_cast<uint8_t>(data & 0x0f)
						};
						if (paletteCount > ids[0]) {
							const PLTE::PaletteData& palette = palettes[ids[0]];
							uint8_t alpha = (trnsArray && trnsLength > ids[0]) ? trnsArray[ids[0]] : 255;
							lineFirst[currentX] = Pixel(palette.red, palette.green, palette.blue, alpha);
						}
						++currentX;
						if (currentX == width) continue;
						if (paletteCount > ids[1]) {
							const PLTE::PaletteData& palette = palettes[ids[1]];
							uint8_t alpha = (trnsArray && trnsLength > ids[1]) ? trnsArray[ids[1]] : 255;
							lineFirst[currentX] = Pixel(palette.red, palette.green, palette.blue, alpha);
						}
						++currentX;
					}
					else if constexpr (bitDepth == 8) {
						if (paletteCount > data) {
							const PLTE::PaletteData& palette = palettes[data];
							uint8_t alpha = (trnsArray && trnsLength > data) ? trnsArray[data] : 255;
							lineFirst[currentX] = Pixel(palette.red, palette.green, palette.blue, alpha);
						}
						++currentX;
					}
				}

			}
		}
	private:
		template<uint8_t bitDepth>
		inline static void DecodeTrueColour(Image& image, Vector<uint8_t>& raw, const tRNS* trns, uint32_t trnsLength) noexcept {
			uint16_t aRed = 0;
			uint16_t aGreen = 0;
			uint16_t aBlue = 0;
			const bool alphaExists = trns && trnsLength >= 6;
			if (alphaExists) trns->GetTrueColourAlpha(aRed, aGreen, aBlue);
			uint8_t* rawData = raw.Items();
			const size_t rawDataCount = raw.Count();
			Pixel* pixels = image.Data();
			const size_t width = image.Width();
			const size_t height = image.Height();
			size_t currentX = width;
			size_t nextY = 0;
			Pixel* lineFirst = nullptr;
			for (size_t i = 0; i < rawDataCount; ++i) {
				//各行1バイト目はフィルターに使用されているため飛ばす必要がある
				if (currentX == width) {
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
					if (nextY > height) break;
				}
				else {
					if constexpr (bitDepth == 8) {
						uint8_t red = rawData[i];
						uint8_t green = rawData[++i];
						uint8_t blue = rawData[++i];
						lineFirst[currentX++] = Pixel(red, green, blue, (alphaExists && red == aRed && green == aGreen && blue == aBlue) ? 0 : 255);
					}
					else if constexpr (bitDepth == 16) {
						uint8_t r1 = rawData[i];
						uint8_t r2 = rawData[++i];
						uint16_t red = Memory::ByteOrder(static_cast<uint16_t>((r1 << 8) | r2));
						uint8_t g1 = rawData[++i];
						uint8_t g2 = rawData[++i];
						uint16_t green = Memory::ByteOrder(static_cast<uint16_t>((g1 << 8) | g2));
						uint8_t b1 = rawData[++i];
						uint8_t b2 = rawData[++i];
						uint16_t blue = Memory::ByteOrder(static_cast<uint16_t>((b1 << 8) | b2));
						lineFirst[currentX++] = Pixel(r1, g1, b1, (alphaExists && red == aRed && green == aGreen && blue == aBlue) ? 0 : 255);
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
				//各行1バイト目はフィルターに使用されているため飛ばす必要がある
				if (currentX == width) {
					lineFirst = pixels + nextY * width;
					currentX = 0;
					++nextY;
					if (nextY > height) break;
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
				uint32_t trnsLength = 0;
				tRNS* trns = GettRNS(trnsLength);
				switch (ihdr->bitDepth) {
				case 1:
					DecodeGrayScale<1>(ret, raw, trns, trnsLength);
					break;
				case 2:
					DecodeGrayScale<2>(ret, raw, trns, trnsLength);
					break;
				case 4:
					DecodeGrayScale<4>(ret, raw, trns, trnsLength);
					break;
				case 8:
					DecodeGrayScale<8>(ret, raw, trns, trnsLength);
					break;
				case 16:
					DecodeGrayScale<16>(ret, raw, trns, trnsLength);
					break;
				default:
					break;
				}
			}
			else if (ihdr->colourType == IHDR::ColourType::TrueColour) {
				uint32_t trnsLength = 0;
				tRNS* trns = GettRNS(trnsLength);
				if (ihdr->bitDepth == 8) DecodeTrueColour<8>(ret, raw, trns, trnsLength);
				else if (ihdr->bitDepth == 16) DecodeTrueColour<16>(ret, raw, trns, trnsLength);
			}
			else if (ihdr->colourType == IHDR::ColourType::TrueColourWithAlpha) {
				if (ihdr->bitDepth == 8) DecodeTrueColourWithAlpha<8>(ret, raw);
				else if (ihdr->bitDepth == 16) DecodeTrueColourWithAlpha<16>(ret, raw);
			}
			else if (ihdr->colourType == IHDR::ColourType::IndexedColour) {
				uint32_t paletteCount = 0;
				PLTE* plte = GetPLTE(paletteCount);
				if (!plte || !paletteCount) return ret;
				uint32_t trnsLength = 0;
				tRNS* trns = GettRNS(trnsLength);
				switch (ihdr->bitDepth) {
				case 1:
					DecodeIndexedColour<1>(ret, raw, *plte, paletteCount, trns, trnsLength);
					break;
				case 2:
					DecodeIndexedColour<2>(ret, raw, *plte, paletteCount, trns, trnsLength);
					break;
				case 4:
					DecodeIndexedColour<4>(ret, raw, *plte, paletteCount, trns, trnsLength);
					break;
				case 8:
					DecodeIndexedColour<8>(ret, raw, *plte, paletteCount, trns, trnsLength);
					break;
				default:
					break;
				}
			}
			return ret;
		}
	public:
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const PNG&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<PNG>(); }
		String ToString() const noexcept override { return String(u"PNG Image"); }
		uint32_t GetTypeID() const noexcept override { return GetID<PNG>(); }
	};
}
