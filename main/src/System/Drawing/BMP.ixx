export module BMP;
import Objects;
import Image;
using Image = System::Drawing::Image;
import File;
import Math;
import Vector;
import Exception;

namespace System::Drawing {
	/// <summary>
	/// BMPファイルのファイルヘッダを表す構造体
	/// </summary>
	struct BitmapFileHeader {
		//ファイルタイプの標識。{ 'B', 'M' }固定
		uint8_t bfType[2];
		//ファイルのバイト長。デコーダは使用すべきでない
		uint32_t bfSize;
		//予約領域。0固定
		uint16_t bfReserved1;
		//予約領域。0固定
		uint16_t bfReserved2;
		//ファイルの先頭から画像データまでのオフセット(バイト)
		//0以外の場合、デコーダは使用してもよい
		uint32_t bfOffBits;
	public:
		/// <summary>
		/// ファイルヘッダを読み取る
		/// </summary>
		/// <param name="file">現在位置がBitmapFileHeaderの先頭を指しているFileオブジェクト</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bfType);
			if (bfType[0] != 'B' || (bfType[1] != 'M')) return false;
			file.ReadLE(bfSize);
			file.ReadLE(bfReserved1);
			file.ReadLE(bfReserved2);
			file.ReadLE(bfOffBits);
			return static_cast<bool>(file);
		}
		/// <summary>
		/// Fileオブジェクトの現在位置にファイルヘッダを書き込む
		/// </summary>
		/// <param name="file">書き込み先のFileオブジェクト</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bfType);
			file.WriteLE(bfSize);
			file.WriteLE(bfReserved1);
			file.WriteLE(bfReserved2);
			file.WriteLE(bfOffBits);
		}
	public:
		/// <summary>
		/// ファイルサイズと画像データまでのオフセットを指定して初期化する
		/// </summary>
		/// <param name="fileSize">ファイル全体のバイト長。0でもよい</param>
		/// <param name="pixelDataOffset">ファイルの先頭から画像データまでのオフセット(バイト)。0でもよい</param>
		void Init(uint32_t fileSize, uint32_t pixelDataOffset) noexcept {
			bfType[0] = 'B';
			bfType[1] = 'M';
			bfSize = fileSize;
			bfReserved1 = 0;
			bfReserved2 = 0;
			bfOffBits = pixelDataOffset;
		}
	};

	//OS/2で使用されていたCOREタイプの情報ヘッダを表す構造体
	struct BitmapCoreInfoHeader {
		//画像データの幅。1以上の数値が入る
		uint16_t bcWidth;
		//画像データの高さ。1以上の数値が入る
		uint16_t bcHeight;
		//チャンネル数。1固定
		uint16_t bcPlanes;
		//ピクセルごとのビット数。1, 4, 8, 24を使用する
		uint16_t bcBitCount;
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bcWidth);
			file.ReadLE(bcHeight);
			file.ReadLE(bcPlanes);
			file.ReadLE(bcBitCount);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bcWidth);
			file.WriteLE(bcHeight);
			file.WriteLE(bcPlanes);
			file.WriteLE(bcBitCount);
		}
	public:
		void Init(uint16_t width, uint16_t height, uint16_t bitCount) noexcept {
			bcWidth = width;
			bcHeight = height;
			bcPlanes = 1;
			bcBitCount = bitCount;
		}
	public:
		uint32_t GetLineSize() const noexcept {
			return static_cast<uint32_t>((static_cast<size_t>(bcWidth) * bcBitCount + 31) / 32ull * 4);
		}
	};

	//標準的な情報ヘッダを表す構造体
	struct BitmapInfoInfoHeader {
		//画像の幅(px)。0以下にはならない
		int32_t biWidth;
		//画像の高さ(px)。0以外の数値が入る。
		//正数の場合、画像データは下の行から格納され(ボトムアップ)、
		//負数の場合、画像データは上の行から格納される(トップダウン)
		int32_t biHeight;
		//チャンネル数。1固定
		uint16_t biPlanes;
		//ピクセルごとのビット数。
		//0, 1, 4, 8, 16, 24, 32のいずれかを使用する
		uint16_t biBitCount;
		//画像データの形式を表す。
		//0: 非圧縮形式。biBitCountは0以外を使用する。
		//1: 8ビットRLE形式。biBitCountは8固定。
		//2: 4ビットRLE形式。biBitCountは4固定。
		//3: ビットフィールドを持つ非圧縮形式。biBitCountは16か32を使用する。
		//4: JPEG形式。biBitCountは0固定。
		//5: PNG形式。biBitCountは0固定。
		uint32_t biCompression;
		//画像データのバイト長
		uint32_t biSizeImage;
		//横方向の1メートルごとのピクセル数。
		//不要な場合、0を格納する
		int32_t biXPixPerMeter;
		//縦方向の1メートルごとのピクセル数。
		//不要な場合、0を格納する
		int32_t biYPixPerMeter;
		//カラーパレットに格納される色数。
		//この値が0かつ、biBitCountが8以下の場合、
		//カラーパレットの色数は(2^biBitCount)個になる
		uint32_t biClrUsed;
		//カラーパレットの色のうち、正確に表示すべき色の数。
		//カラーパレットの先頭から、この数値の分の色が重要色である。
		//0の場合、すべての色が重要色となる
		uint32_t biClrImportant;
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(biWidth);
			file.ReadLE(biHeight);
			file.ReadLE(biPlanes);
			file.ReadLE(biBitCount);
			file.ReadLE(biCompression);
			file.ReadLE(biSizeImage);
			file.ReadLE(biXPixPerMeter);
			file.ReadLE(biYPixPerMeter);
			file.ReadLE(biClrUsed);
			file.ReadLE(biClrImportant);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(biWidth);
			file.WriteLE(biHeight);
			file.WriteLE(biPlanes);
			file.WriteLE(biBitCount);
			file.WriteLE(biCompression);
			file.WriteLE(biSizeImage);
			file.WriteLE(biXPixPerMeter);
			file.WriteLE(biYPixPerMeter);
			file.WriteLE(biClrUsed);
			file.WriteLE(biClrImportant);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount) noexcept {
			biWidth = static_cast<int32_t>(width);
			biHeight = height;
			biPlanes = 1;
			biBitCount = bitCount;
			biCompression = 0;
			uint32_t lineSize = GetLineSize();
			biSizeImage = static_cast<uint32_t>(lineSize * System::Math::Abs(biHeight));
			biXPixPerMeter = 0;
			biYPixPerMeter = 0;
			biClrUsed = 0;
			biClrImportant = 0;
		}
	public:
		uint32_t GetLineSize() const noexcept {
			return static_cast<uint32_t>((static_cast<size_t>(biWidth) * biBitCount + 31) / 32ull * 4);
		}
	};

	//V4タイプの情報ヘッダを表す構造体
	struct BitmapV4InfoHeader : public BitmapInfoInfoHeader {
		//赤成分のカラーマスク値。ビットフィールドを持つ非圧縮形式の場合、有効
		uint32_t bv4RedMask;
		//緑成分のカラーマスク値。ビットフィールドを持つ非圧縮形式の場合、有効
		uint32_t bv4GreenMask;
		//青成分のカラーマスク値。ビットフィールドを持つ非圧縮形式の場合、有効
		uint32_t bv4BlueMask;
		//アルファ成分のカラーマスク値。ビットフィールドを持つ非圧縮形式の場合、有効
		uint32_t bv4AlphaMask;
		//色空間タイプ。V4タイプの情報ヘッダでは0固定。
		//"BGRs"(0x73524742): sRGB空間を持つ
		//" niW"(0x57696e20): Windows標準の色空間を持つ
		//"KNIL"(0x4c494e4b): BMPファイル内に色空間プロファイルファイルへのパスが含まれる
		//"DEBM"(0x4d424544): BMPファイル内に色空間プロファイルが埋め込まれている
		uint32_t bv4CSType;
		//CIEXYZカラーモデルの赤成分の座標(x, y, z)。bv4CSTypeが0の場合、有効
		int32_t bv4RedEndPoint[3];
		//CIEXYZカラーモデルの緑成分の座標(x, y, z)。bv4CSTypeが0の場合、有効
		int32_t bv4GreenEndPoint[3];
		//CIEXYZカラーモデルの青成分の座標(x, y, z)。bv4CSTypeが0の場合、有効
		int32_t bv4BlueEndPoint[3];
		//CIEのxyY色度空間におけるYの赤成分を表す。16.16の固定小数点数
		uint32_t bv4GammaRed;
		//CIEのxyY色度空間におけるYの緑成分を表す。16.16の固定小数点数
		uint32_t bv4GammaGreen;
		//CIEのxyY色度空間におけるYの青成分を表す。16.16の固定小数点数
		uint32_t bv4GammaBlue;
	public:
		bool Load(IO::FileStream& file, uint32_t biSize) noexcept {
			if (!BitmapInfoInfoHeader::Load(file)) return false;
			if (biSize <= 40) return static_cast<bool>(file);
			file.ReadLE(bv4RedMask);
			file.ReadLE(bv4GreenMask);
			file.ReadLE(bv4BlueMask);
			if (biSize <= 52) return static_cast<bool>(file);
			file.ReadLE(bv4AlphaMask);
			if (biSize <= 56) return static_cast<bool>(file);
			file.ReadLE(bv4CSType);
			if (biSize <= 60) return static_cast<bool>(file);
			file.ReadLE(bv4RedEndPoint);
			file.ReadLE(bv4GreenEndPoint);
			file.ReadLE(bv4BlueEndPoint);
			if (biSize <= 96) return static_cast<bool>(file);
			file.ReadLE(bv4GammaRed);
			file.ReadLE(bv4GammaGreen);
			file.ReadLE(bv4GammaBlue);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file, uint32_t biSize) const noexcept {
			BitmapInfoInfoHeader::Write(file);
			if (biSize <= 40) return;
			file.WriteLE(bv4RedMask);
			file.WriteLE(bv4GreenMask);
			file.WriteLE(bv4BlueMask);
			if (biSize <= 52) return;
			file.WriteLE(bv4AlphaMask);
			if (biSize <= 56) return;
			file.WriteLE(bv4CSType);
			if (biSize <= 60) return;
			file.WriteLE(bv4RedEndPoint);
			file.WriteLE(bv4GreenEndPoint);
			file.WriteLE(bv4BlueEndPoint);
			if (biSize <= 96) return;
			file.WriteLE(bv4GammaRed);
			file.WriteLE(bv4GammaGreen);
			file.WriteLE(bv4GammaBlue);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount, uint32_t biSize) noexcept {
			BitmapInfoInfoHeader::Init(width, height, bitCount);
			if (biSize <= 40) return;
			bv4RedMask = bitCount == 16 ? 0x00007c00 : 0x00ff0000;
			bv4GreenMask = bitCount == 16 ? 0x000003e0 : 0x0000ff00;
			bv4BlueMask = bitCount == 16 ? 0x0000001f : 0x000000ff;
			if (biSize <= 52) return;
			bv4AlphaMask = bitCount == 16 ? 0 : 0xff000000;
			if (biSize <= 56) return;
			bv4CSType = 0;
			if (biSize <= 60) return;
			bv4RedEndPoint[0] = 0;
			bv4RedEndPoint[1] = 0;
			bv4RedEndPoint[2] = 0;
			bv4GreenEndPoint[0] = 0;
			bv4GreenEndPoint[1] = 0;
			bv4GreenEndPoint[2] = 0;
			bv4BlueEndPoint[0] = 0;
			bv4BlueEndPoint[1] = 0;
			bv4BlueEndPoint[2] = 0;
			if (biSize <= 96) return;
			bv4GammaRed = 0;
			bv4GammaGreen = 0;
			bv4GammaBlue = 0;
		}
	};

	//V5タイプの情報ヘッダを表す構造体
	struct BitmapV5InfoHeader : public BitmapV4InfoHeader {
		//sRGB色空間タイプ。
		//1: Saturation。図やグラフに使用。
		//2: Relative Colorimetric。ロゴやデザイン画に使用。
		//4: Perceptual。写真や風景画に使用。
		//8: Absolute Colorimetric。プレビューに使用。
		uint32_t bv5Intent;
		//情報ヘッダの先頭から色空間プロファイルデータへのオフセット(バイト)。
		//BitmapV4InfoHeader::bv4CStypeが、
		//	"KNIL"(link)の場合、プロファイルデータはファイルパスを表すヌル終端の文字列。
		//	"DEBM"(embedded)の場合、プロファイルデータはbv5ProfileSize(バイト)の埋め込みデータ。
		uint32_t bv5ProfileData;
		//プロファイルデータのバイト長
		uint32_t bv5ProfileSize;
		//予約領域。0固定
		uint32_t bv5Reserved;
	public:
		bool Load(IO::FileStream& file, uint32_t biSize) noexcept {
			if (!BitmapV4InfoHeader::Load(file, biSize)) return false;
			if (biSize <= 108) return static_cast<bool>(file);
			file.ReadLE(bv5Intent);
			if (biSize <= 112) return static_cast<bool>(file);
			file.ReadLE(bv5ProfileData);
			file.ReadLE(bv5ProfileSize);
			if (biSize <= 120) return static_cast<bool>(file);
			file.ReadLE(bv5Reserved);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file, uint32_t biSize) const noexcept {
			BitmapV4InfoHeader::Write(file, biSize);
			if (biSize <= 108) return;
			file.WriteLE(bv5Intent);
			if (biSize <= 112) return;
			file.WriteLE(bv5ProfileData);
			file.WriteLE(bv5ProfileSize);
			if (biSize <= 120) return;
			file.WriteLE(bv5Reserved);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount, uint32_t biSize) noexcept {
			BitmapV4InfoHeader::Init(width, height, bitCount, biSize);
			if (biSize <= 108) return;
			bv5Intent = 1;
			if (biSize <= 112) return;
			bv5ProfileData = 0;
			bv5ProfileSize = 0;
			if (biSize <= 120) return;
			bv5Reserved = 0;
		}
	};

	/// <summary>
	/// BMPファイルの情報ヘッダを表す構造体
	/// </summary>
	struct BitmapInfoHeader {
		//このヘッダのバイト長
		uint32_t biSize = 0;
		union {
			BitmapCoreInfoHeader core;
			BitmapInfoInfoHeader info;
			BitmapV4InfoHeader v4;
			BitmapV5InfoHeader v5;
		};
	public:
		/// <summary>
		/// 情報ヘッダを読み取る
		/// </summary>
		/// <param name="file">現在位置がBitmapInfoHeaderの先頭を指しているFileオブジェクト</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(biSize);
			if (biSize == 12) return core.Load(file);
			else if (biSize >= 40) return v5.Load(file, biSize);
			else return false;
		}
		/// <summary>
		/// Fileオブジェクトの現在位置に情報ヘッダを書き込む
		/// </summary>
		/// <param name="file">書き込み先のFileオブジェクト</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(biSize);
			if (biSize == 12) core.Write(file);
			else if (biSize >= 40) v5.Write(file, biSize);
		}
	public:
		/// <summary>
		/// 情報ヘッダサイズと画像データの幅、高さ、ピクセルごとのビット数を指定して初期化する
		/// </summary>
		/// <param name="infoHeaderSize">
		/// 情報ヘッダのサイズ。
		/// 12, 40, 52, 56, 60, 96, 108, 112, 120, 124のいずれかを指定する。
		/// </param>
		/// <param name="width">画像データの幅(px)。0以外の値を指定する</param>
 		/// <param name="height">画像データの高さ(px)。0以外の値を指定する</param>
		/// <param name="bitCount">
		/// ピクセルごとのビット数。
		/// 0, 1, 4, 8, 16, 24, 32のいずれかを指定する
		/// </param>
		void Init(uint32_t infoHeaderSize, uint32_t width, int32_t height, uint16_t bitCount) noexcept {
			biSize = infoHeaderSize;
			if (biSize == 12) core.Init(static_cast<uint16_t>(width), static_cast<uint16_t>(height), bitCount);
			else if (biSize >= 40) v5.Init(width, height, bitCount, biSize);
		}
	public:
		/// <summary>
		/// 情報ヘッダから、BMPファイルにビットフィールドが含まれるか調べる
		/// </summary>
		bool ExistsBitField() const noexcept {
			return biSize == 40
				&& (info.biBitCount == 16 || info.biBitCount == 32)
				&& info.biCompression == 3;
		}
		/// <summary>
		/// 情報ヘッダから、BMPファイルにカラーパレットが含まれるか調べる
		/// </summary>
		bool ExistsPalette() const noexcept {
			if (biSize != 12 && biSize < 40) return false;
			switch (biSize == 12 ? core.bcBitCount : info.biBitCount) {
			case 1:
			case 4:
			case 8:
				return true;
			default:
				return biSize == 12 ? false : info.biClrUsed;
			}
		}
		/// <summary>
		/// BMPファイルに含まれるカラーパレット数を取得する
		/// </summary>
		uint32_t GetPaletteCount() const noexcept {
			if (!ExistsPalette()) return 0;
			if (biSize >= 40 && info.biClrUsed) return info.biClrUsed;
			return 1ul << (biSize == 12 ? core.bcBitCount : info.biBitCount);
		}
		/// <summary>
		/// ピクセルごとのビット数を取得する
		/// </summary>
		uint16_t GetBitCount() const noexcept {
			return IsCoreHeader() ? core.bcBitCount : info.biBitCount;
		}
		/// <summary>
		/// この情報ヘッダがCOREタイプか調べる
		/// </summary>
		bool IsCoreHeader() const noexcept { return biSize == 12; }
		/// <summary>
		/// 画像データがトップダウン(上の行->下の行の順に並んでいる)か調べる
		/// </summary>
		bool HasTopDownImageData() const noexcept {
			return IsCoreHeader() ? false : info.biHeight < 0;
		}
		/// <summary>
		/// 画像データの1行ごとのバイト長(パディング含む)を取得する
		/// </summary>
		uint32_t GetLineSize() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return biSize == 12 ? core.GetLineSize() : info.GetLineSize();
		}
		/// <summary>
		/// 画像データの幅(px)を取得する
		/// </summary>
		uint32_t GetWidth() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return static_cast<uint32_t>(biSize == 12 ? core.bcWidth : info.biWidth);
		}
		/// <summary>
		/// 画像データの高さ(px)を取得する
		/// </summary>
		uint32_t GetHeight() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return static_cast<uint32_t>(biSize == 12 ? core.bcHeight : System::Math::Abs(info.biHeight));
		}
		/// <summary>
		/// 画像データのバイト長(パディング含む)を取得する
		/// </summary>
		size_t GetImageDataSize() const noexcept {
			uint32_t lineSize = GetLineSize();
			return static_cast<size_t>(lineSize) * GetHeight();
		}
	};

	/// <summary>
	/// BMPファイルのビットフィールドを表す構造体
	/// </summary>
	struct BitmapBitField {
		//赤成分のカラーマスク値
		uint32_t bfRedMask;
		//緑成分のカラーマスク値
		uint32_t bfGreenMask;
		//青成分のカラーマスク値
		uint32_t bfBlueMask;
	public:
		/// <summary>
		/// ビットフィールドを読み取る
		/// </summary>
		/// <param name="file">現在位置がBitmapBitFieldの先頭を指しているFileオブジェクト</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bfRedMask);
			file.ReadLE(bfGreenMask);
			file.ReadLE(bfBlueMask);
			return static_cast<bool>(file);
		}
		/// <summary>
		/// Fileオブジェクトの現在位置にビットフィールドを書き込む
		/// </summary>
		/// <param name="file">書き込み先のFileオブジェクト</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bfRedMask);
			file.WriteLE(bfGreenMask);
			file.WriteLE(bfBlueMask);
		}
	public:
		/// <summary>
		/// 赤、緑、青のカラーマスクを指定して初期化する
		/// </summary>
		/// <param name="red">赤成分のカラーマスク</param>
		/// <param name="green">緑成分のカラーマスク</param>
		/// <param name="blue">青成分のカラーマスク</param>
		void Init(uint32_t red, uint32_t green, uint32_t blue) noexcept {
			bfRedMask = red;
			bfGreenMask = green;
			bfBlueMask = blue;
		}
	};

	struct RGBTRIPLE {
		uint8_t rgbBlue;
		uint8_t rgbGreen;
		uint8_t rgbRed;
	public:
		static bool Load(IO::FileStream& file, RGBTRIPLE* triple, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.ReadLE(&triple[i].rgbBlue, 3);
			return static_cast<bool>(file);
		}
		static void Write(IO::FileStream& file, const RGBTRIPLE* triple, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.WriteLE(&triple[i].rgbBlue, 3);
		}
	public:
		static uint32_t GetPaletteIndex(const RGBTRIPLE* palette, uint32_t paletteCount, uint8_t red, uint8_t green, uint8_t blue) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) {
				if (palette[i].rgbBlue == blue && palette[i].rgbGreen == green && palette[i].rgbRed == red) {
					return i;
				}
			}
			return paletteCount;
		}
	public:
		constexpr operator Pixel() const noexcept { return Pixel(rgbRed, rgbGreen, rgbBlue); }
	};

	struct RGBQUAD {
		uint8_t rgbBlue;
		uint8_t rgbGreen;
		uint8_t rgbRed;
		uint8_t rgbReserved;
	public:
		static bool Load(IO::FileStream& file, RGBQUAD* quad, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.ReadLE(&quad[i].rgbBlue, 4);
			return static_cast<bool>(file);
		}
		static void Write(IO::FileStream& file, const RGBQUAD* quad, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.WriteLE(&quad[i].rgbBlue, 4);
		}
	public:
		static uint32_t GetPaletteIndex(const RGBQUAD* palette, uint32_t paletteCount, uint8_t red, uint8_t green, uint8_t blue) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) {
				if (palette[i].rgbBlue == blue && palette[i].rgbGreen == green && palette[i].rgbRed == red) {
					return i;
				}
			}
			return paletteCount;
		}
	public:
		constexpr operator Pixel() const noexcept { return Pixel(rgbRed, rgbGreen, rgbBlue, rgbReserved); }
	};
}

export namespace System::Drawing {
	class BMP : public Object {
		BitmapFileHeader header;
		BitmapInfoHeader infoHeader;
		//infoHeader.biSizeが40かつ、
		//infoHeader.info.biBitCountが16または32で、
		//かつinfoHeader.info.biCompressionが3のときのみ存在する
		BitmapBitField* bitField = nullptr;
		//infoHeader.(core.bcBitCount/info.biBitCount)が1, 4, 8または、
		//infoHeader.biClrUsedが1以上のときのみ存在する
		union {
			//infoHeader.biSizeが12(COREタイプ)のとき使用する
			RGBTRIPLE* triple = nullptr;
			//tripleでない場合、こちらを使用する
			RGBQUAD* quad;
		public:
			Pixel GetPixel(bool useTriple, uint32_t paletteIndex) const noexcept {
				if (useTriple) return static_cast<Pixel>(triple[paletteIndex]);
				else return static_cast<Pixel>(quad[paletteIndex]);
			}
		} palette;
		uint8_t* data = nullptr;
	public:
		BMP() noexcept = default;
		BMP(BMP&& arg) noexcept : header(arg.header), infoHeader(arg.infoHeader), bitField(arg.bitField), data(arg.data) {
			palette.triple = arg.palette.triple;
			arg.bitField = nullptr;
			arg.palette.triple = nullptr;
			arg.data = nullptr;
		}
		explicit BMP(const String& filename) noexcept { Load(filename); }
		explicit BMP(System::IO::FileStream&& file) noexcept { Load(file); }
		~BMP() {
			delete bitField;
			bitField = nullptr;
			if (palette.triple) {
				if (infoHeader.IsCoreHeader()) delete[] palette.triple;
				else delete[] palette.quad;
			}
			palette.triple = nullptr;
			delete[] data;
			data = nullptr;
		}
	public:
		/// <summary>
		/// このファイルがBMPファイルか調べる
		/// </summary>
		/// <param name="file">バイナリ入力で開かれたファイルオブジェクト</param>
		static bool IsBMP(System::IO::FileStream& file) noexcept {
			System::IO::StreamPos pos = file.TellPos();
			file.Seek(0, System::IO::SeekDir::Begin);
			uint8_t type[2]{};
			file.ReadLE(type);
			file.Clear();
			file.Seek(pos);
			return type[0] == 'B' && type[1] == 'M';
		}
	public:
		bool Load(const String& filePath) noexcept {
			System::IO::FileStream file{ filePath, System::IO::OpenMode::IN_BINARY };
			if (!file) return false;
			return Load(file);
		}
		bool Load(System::IO::FileStream& file) noexcept {
			Clear();
			if (!header.Load(file)) return Clear(false);
			if (!infoHeader.Load(file)) return Clear(false);
			if (infoHeader.ExistsBitField()) {
				bitField = new BitmapBitField;
				if (!bitField->Load(file)) return Clear(false);
			}
			uint32_t paletteCount = infoHeader.GetPaletteCount();
			if (paletteCount) {
				if (infoHeader.biSize == 12) {
					palette.triple = new RGBTRIPLE[paletteCount];
					if (!RGBTRIPLE::Load(file, palette.triple, paletteCount)) return Clear(false);
				}
				else {
					palette.quad = new RGBQUAD[paletteCount];
					if (!RGBQUAD::Load(file, palette.quad, paletteCount)) return Clear(false);
				}
			}
			size_t dataSize = infoHeader.GetImageDataSize();
			if (dataSize) {
				data = new uint8_t[dataSize];
				file.ReadLE(data, dataSize);
				if (!file) return Clear(false);
			}
			else return Clear(false);
			return static_cast<bool>(file);
		}
		bool Save(const String& filePath) noexcept {
			if (!data || infoHeader.GetWidth() <= 0 || infoHeader.GetHeight() == 0) return false;
			try {
				IO::FileStream file(filePath, IO::OpenMode::OUT_BINARY);
				if (!file) return false;
				header.Write(file);
				infoHeader.Write(file);
				//ビットフィールド書き込み
				if (infoHeader.ExistsBitField() && bitField) bitField->Write(file);
				//カラーパレット書き込み
				uint32_t paletteCount = infoHeader.GetPaletteCount();
				if (paletteCount && palette.triple) {
					if (infoHeader.IsCoreHeader()) RGBTRIPLE::Write(file, palette.triple, paletteCount);
					else RGBQUAD::Write(file, palette.quad, paletteCount);
				}
				size_t dataSize = infoHeader.GetImageDataSize();
				file.WriteLE(data, dataSize);
			}
			catch (std::exception&) {
				return false;
			}
			return true;
		}
		bool Clear(bool result = false) {
			delete[] data;
			data = nullptr;
			if (palette.triple) {
				if (infoHeader.biSize == 12) delete[] palette.triple;
				else delete[] palette.quad;
				palette.triple = nullptr;
			}
			delete bitField;
			bitField = nullptr;
			infoHeader.Init(40, 0, 0, 0);
			header.Init(0, 0);
			return result;
		}
	private:
		void Load1bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = (w >> 3) + (w % 8 == 0 ? 0 : 1);
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			Pixel p0 = palette.GetPixel(useTriple, 0);
			Pixel p1 = palette.GetPixel(useTriple, 1);
			for (uint32_t y = h; y-- > 0;) {
				uint32_t x = 0;
				size_t cur = y * widthLen;
				while (x < w) {
					uint8_t paletteNum = data[cur++];
					if (x + 7 < w) {
						if (paletteNum & 0x80) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x40) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x20) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x10) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x08) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x04) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x02) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x01) pixels.Add(p1);
						else pixels.Add(p0);
						x += 8;
					}
					else {
						do {
							if (paletteNum & 0x80) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x40) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x20) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x10) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x08) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x04) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x02) pixels.Add(p1);
							else pixels.Add(p0);
							++x;
						} while (false);
					}
				}
			}
		}
		void Load4bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = (w >> 1) + (w % 2 == 0 ? 0 : 1);
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			const uint32_t end = (w >> 1) << 1;
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < end; x += 2) {
					uint8_t paletteNum = data[cur++];
					pixels.Add(palette.GetPixel(useTriple, (paletteNum & 0xf0u) >> 4));
					pixels.Add(palette.GetPixel(useTriple, paletteNum & 0x0fu));
				}
				if (end != w) pixels.Add(palette.GetPixel(useTriple, (data[cur] & 0xf0u) >> 4));
			}
		}
		void Load8bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			const size_t widthLen = w + (w % 4 == 0 ? 0 : 4 - (w % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < w; ++x) {
					uint8_t paletteNum = data[cur++];
					pixels.Add(palette.GetPixel(useTriple, paletteNum));
				}
			}
		}
		void Load24bitTrueColor(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = w * 3;
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < w; ++x) {
					pixels.Add(Pixel(data[cur + 2], data[cur + 1], data[cur]));
					cur += 3;
				}
			}
		}
		void Load32bitTrueColor(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = static_cast<size_t>(y) * w * 4;
				for (uint32_t x = 0; x < w; ++x) {
					pixels.Add(Pixel(data[cur + 2], data[cur + 1], data[cur], data[cur + 3]));
					cur += 4;
				}
			}
		}
	public:
		/// <summary>
		/// Image型オブジェクトを生成する
		/// </summary>
		Image ToImage() const noexcept {
			Vector<Pixel> pixels;
			uint32_t w = infoHeader.GetWidth();
			uint32_t h = infoHeader.GetHeight();
			size_t pixelCount = static_cast<size_t>(w) * h;
			pixels.Reserve(pixelCount);
			switch (infoHeader.GetBitCount()) {
			case 1: {
				Load1bitIndexed(pixels, w, h);
				break;
			}
			case 4: {
				Load4bitIndexed(pixels, w, h);
				break;
			}
			case 8: {
				Load8bitIndexed(pixels, w, h);
				break;
			}
			case 24: {
				Load24bitTrueColor(pixels, w, h);
				break;
			}
			case 32: {
				Load32bitTrueColor(pixels, w, h);
				break;
			}
			}
			Image ret(w, h, static_cast<Vector<Pixel>&&>(pixels));
			if (!infoHeader.HasTopDownImageData()) ret.Reverse();
			return ret;
		}
		/// <summary>
		/// ImageオブジェクトからBMP型オブジェクトを生成する
		/// </summary>
		/// <param name="image">ソースとなるImage型変数へのconst参照</param>
		static BMP FromImage(const Image& image) noexcept {
			BMP ret;
			const Pixel* pixels = image.Data();
			const uint32_t width = image.Width();
			const uint32_t height = image.Height();
			ret.infoHeader.Init(40, width, height, 24);
			//ファイルヘッダ14Byteと情報ヘッダのバイト長の合計
			uint32_t headerSize = 14 + ret.infoHeader.biSize;
			//画像データのバイト長
			size_t dataSize = ret.infoHeader.GetImageDataSize();
			ret.header.Init(static_cast<uint32_t>(headerSize + dataSize), headerSize);
			ret.data = new uint8_t[dataSize];
			//1行ごとのパディングサイズ(バイト)
			const uint32_t paddingSize = ret.infoHeader.GetLineSize() - (width * 3);
			size_t i = 0;	//現在のデータ位置
			for (uint32_t y = height; y-- > 0;) {
				const Pixel* p = pixels + (y * static_cast<size_t>(width));
				for (uint32_t x = 0; x < width; ++x, i += 3) {
					ret.data[i] = p->b;
					ret.data[i + 1] = p->g;
					ret.data[i + 2] = p->r;
					++p;
				}
				//パディングサイズだけデータ位置を進める
				for (uint32_t n = 0; n < paddingSize; ++n, ++i);
			}
			return ret;
		}
	public://Objectクラスオーバーライド
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const BMP&>(obj) : false; }
		Type GetType() const noexcept override { return Type(u"System::Drawing::BMP"); }
		String ToString() const noexcept override { return String(u"Bitmap Image"); }
		uint32_t GetTypeID() const noexcept override { return GetID<BMP>(); }
	};
}