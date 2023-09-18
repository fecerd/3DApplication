export module TGA;
import CSTDINT;
import Objects;
import Math;
import VectorBase;
import Exception;
import File;
import Path;
export import Image;

export namespace System::Drawing {
	class TGA {
		//m_signature[18]はこの文字列(ヌル終端あり)を持つ
		//"TRUEVISION-TARGA\0"(+ヌル終端)の場合もある？
		static constexpr char Signature[18] = "TRUEVISION-XFILE.";
	private:
		//ImageIDフィールドのバイト長。0のときフィールドが存在しない
		uint8_t m_idLength = 0;
		//カラーマップの有無(0:なし、1:あり)
		uint8_t m_colorMapType = 0;
		//画像形式
		//0: なし
		//1: インデックスカラー(256色)
		//2: フルカラー
		//3: 白黒
		//4ビット目(8)が1のとき(9から11)、RLE圧縮されている
		uint8_t m_imageType = 0;
		//カラーマップを読み込むときの開始インデックス
		uint16_t m_firstEntryIndex = 0;
		//カラーマップの数
		uint16_t m_colorMapLength = 0;
		//エントリごとのビット数。通常、15、16、24、32のいずれか
		uint8_t m_colorMapEntrySize = 0;
		//スクリーン上での画像の左下のX座標
		int16_t m_xOrigin = 0;
		//スクリーン上での画像の左下のY座標
		int16_t m_yOrigin = 0;
		//画像の幅
		uint16_t m_width = 0;
		//画像の高さ
		uint16_t m_height = 0;
		//ピクセルごとのビット数。通常、8、16、24、32のいずれか
		uint8_t m_pixelDepth = 0;
		//下位から、
		//0-3: 属性(アルファチャネルやオーバーレイ機能に使用)
		//4: 0のとき左から右、1のとき右から左に格納
		//5: 0のとき下から上、1のとき上から下に格納
		//6,7: 未使用領域(0固定)
		uint8_t m_imageDescriptor = 0;
		//画像識別子。[m_idLength]
		uint8_t* m_imageID = nullptr;
		//カラーマップデータ。
		//m_colorMapTypeが1のときのみ存在する。
		//[RoundUp(m_colorMapLength * m_colorMapEntrySize / 8)]
		uint8_t* m_colorMap = nullptr;
		//画像データ
		uint8_t* m_data = nullptr;
		//ファイル先頭から拡張領域へのオフセット
		//0のとき、拡張領域が存在しない
		int32_t m_extensionAreaOffset = 0;
		//ファイル先頭から開発者ディレクトリへのオフセット
		//0のとき、開発者ディレクトリが存在しない
		int32_t m_developerDirectoryOffset = 0;
		//TGAフォーマットを表す標識
		char m_signature[18] = {};
	public:
		TGA() noexcept = default;
		TGA(const String& filePath) noexcept {
			Load(filePath);
		}
		TGA(System::IO::FileStream&& file) noexcept {
			Load(file);
		}
		~TGA() noexcept {
			delete[] m_imageID;
			m_imageID = nullptr;
			delete[] m_colorMap;
			m_colorMap = nullptr;
			delete[] m_data;
			m_data = nullptr;
		}
	private:
		size_t GetColorMapByteSize() const noexcept {
			size_t colorMapBitCount = m_colorMapLength * m_colorMapEntrySize;
			return colorMapBitCount / 8 + (colorMapBitCount % 8 == 0 ? 0 : 1);
		}
		size_t GetPixelDataByteSize() const noexcept {
			size_t bit = static_cast<size_t>(m_width) * m_height * m_pixelDepth;
			return bit / 8 + (bit % 8 == 0 ? 0 : 1);
		}
		bool IsLeftToRight() const noexcept {
			return !(m_imageDescriptor & 0x10);
		}
		bool IsBottomToTop() const noexcept {
			return !(m_imageDescriptor & 0x20);
		}
	public:
		static bool IsTGA(System::IO::FileStream& file) noexcept {
			System::IO::StreamPos pos = file.TellPos();
			bool ret = true;
			file.Seek(-18, System::IO::SeekDirs::End);
			char signature[18]{};
			file.Read(signature, 18);
			const char check[] = "TRUEVISION-";
			for (size_t i = 0; i < sizeof(check) - 1; ++i) {
				if (signature[i] != check[i]) {
					ret = false;
					break;
				}
			}
			if (signature[17] != '\0') ret = false;
			file.Clear();
			file.Seek(pos);
			return ret;
		}
	public:
		bool Load(const String& filePath) noexcept {
			System::IO::FileStream file(filePath, System::IO::OpenMode::IN_BINARY);
			return Load(file);
		}
		bool Load(System::IO::FileStream& file) noexcept {
			if (!file) return false;
			file.ReadLE(m_idLength);
			file.ReadLE(m_colorMapType);
			file.ReadLE(m_imageType);
			file.ReadLE(m_firstEntryIndex);
			file.ReadLE(m_colorMapLength);
			file.ReadLE(m_colorMapEntrySize);
			file.ReadLE(m_xOrigin);
			file.ReadLE(m_yOrigin);
			file.ReadLE(m_width);
			file.ReadLE(m_height);
			file.ReadLE(m_pixelDepth);
			file.ReadLE(m_imageDescriptor);
			if (m_idLength) {
				m_imageID = new uint8_t[m_idLength];
				file.ReadLE(m_imageID, m_idLength);
			}
			if (m_colorMapType) {
				size_t colorMapByteCount = GetColorMapByteSize();
				m_colorMap = new uint8_t[colorMapByteCount];
				file.Read(m_colorMap, colorMapByteCount);
			}
			size_t pixelByteCount = GetPixelDataByteSize();
			m_data = new uint8_t[pixelByteCount];
			file.Read(m_data, pixelByteCount);
			return static_cast<bool>(file);
		}
	private:
		void LoadTrueColor(VectorBase<Pixel>& pixels) const noexcept {
			size_t pixelCount = static_cast<size_t>(m_width) * m_height;
			pixels.Reserve(pixelCount);
			const uint8_t* read = m_data;
			if (m_pixelDepth == 24) {
				for (size_t i = 0; i < pixelCount; ++i) {
					pixels.Add(Pixel(read[2], read[1], read[0]));
					read += 3;
				}
			}
			else if (m_pixelDepth == 32) {
				for (size_t i = 0; i < pixelCount; ++i) {
					pixels.Add(Pixel(read[2], read[1], read[0], read[3]));
					read += 4;
				}
			}
		}
	public:
		Image ToImage() const {
			if (!m_data) return Image();
			VectorBase<Pixel> pixels;
			if (m_colorMap) {

			}
			else {
				if (m_imageType == 2) LoadTrueColor(pixels);
			}
			if (!pixels.Count()) throw LogicException(u"このTGA形式のサポートは未実装です");
			Image ret(m_width, m_height, System::move(pixels));
			if (IsBottomToTop()) ret.Reverse();
			return ret;
		}
	};
}
