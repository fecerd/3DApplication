export module OpenType:Internal;
#undef UNICODE
export import CSTDINT;
export import Traits;
export import Objects;
import TypeName;
export import Math;
export import Vector;
import IEnumerable;
import HashMap;
export import BSpline;
export import IO;
export import Image;
using namespace System;

//内部用Enum
export namespace System {
	//文字エンコード設定
	enum class EncodingType : uint8_t {
		NONE,
		UNICODE,
		SHIFT_JIS
	};

	//DICT形式で使用する演算子
	enum class DICTOperator : uint16_t {
		version = 0x00,
		Notice,
		Fullname,
		FamilyName,
		Weight,
		FontBBox,
		BlueValues,
		OtherBlues,
		FamilyBlues,
		FamilyOtherBlues,
		StdHW,
		StdVW,
		escape,
		UniqueID,
		XUID,
		charset,
		Encoding,
		CharStrings,
		Private,
		Subrs,
		defaultWidthX,
		nominalWidthX,
		shortint = 0x1c,
		longint,
		BCD,
		Copyright = 0x0c00,
		isFixedPitch,
		ItalicAngle,
		UnderlinePosition,
		UnderlineThickness,
		PaintType,
		CharstringType,
		FontMatrix,
		StrokeWidth,
		BlueScale,
		BlueShift,
		BlueFuzz,
		StemSnapH,
		StemSnapV,
		ForceBold,
		LanguageGroup = 0x0c11,
		ExpansionFactor,
		initialRandomSeed,
		SyntheticBase,
		PostScript,
		BaseFontName,
		BaseFontBlend,
		ROS = 0x0c1e,
		CIDFontVersion,
		CIDFontRevision,
		CIDFontType,
		CIDCount,
		UIDBase,
		FDArray,
		FDSelect,
		FontName,
		Undefined	//DICTKeyValue用
	};

	System::String GetDICTOperatorName(DICTOperator opr) noexcept {
		switch (opr) {
			//Valueが一つ
		case DICTOperator::version: return EnumName<DICTOperator>::GetName<DICTOperator::version>();
		case DICTOperator::Notice: return EnumName<DICTOperator>::GetName<DICTOperator::Notice>();
		case DICTOperator::Fullname: return EnumName<DICTOperator>::GetName<DICTOperator::Fullname>();
		case DICTOperator::FamilyName: return EnumName<DICTOperator>::GetName<DICTOperator::FamilyName>();
		case DICTOperator::Weight: return EnumName<DICTOperator>::GetName<DICTOperator::Weight>();
		case DICTOperator::isFixedPitch: return EnumName<DICTOperator>::GetName<DICTOperator::isFixedPitch>();
		case DICTOperator::ItalicAngle: return EnumName<DICTOperator>::GetName<DICTOperator::ItalicAngle>();
		case DICTOperator::UnderlinePosition: return EnumName<DICTOperator>::GetName<DICTOperator::UnderlinePosition>();
		case DICTOperator::UnderlineThickness: return EnumName<DICTOperator>::GetName<DICTOperator::UnderlineThickness>();
		case DICTOperator::PaintType: return EnumName<DICTOperator>::GetName<DICTOperator::PaintType>();
		case DICTOperator::CharstringType: return EnumName<DICTOperator>::GetName<DICTOperator::CharstringType>();
		case DICTOperator::UniqueID: return EnumName<DICTOperator>::GetName<DICTOperator::UniqueID>();
		case DICTOperator::StrokeWidth: return EnumName<DICTOperator>::GetName<DICTOperator::StrokeWidth>();
		case DICTOperator::charset: return EnumName<DICTOperator>::GetName<DICTOperator::charset>();
		case DICTOperator::Encoding: return EnumName<DICTOperator>::GetName<DICTOperator::Encoding>();
		case DICTOperator::CharStrings: return EnumName<DICTOperator>::GetName<DICTOperator::CharStrings>();
		case DICTOperator::SyntheticBase: return EnumName<DICTOperator>::GetName<DICTOperator::SyntheticBase>();
		case DICTOperator::PostScript: return EnumName<DICTOperator>::GetName<DICTOperator::PostScript>();
		case DICTOperator::BaseFontName: return EnumName<DICTOperator>::GetName<DICTOperator::BaseFontName>();
		case DICTOperator::BaseFontBlend: return EnumName<DICTOperator>::GetName<DICTOperator::BaseFontBlend>();
		case DICTOperator::CIDFontVersion: return EnumName<DICTOperator>::GetName<DICTOperator::CIDFontVersion>();
		case DICTOperator::CIDFontRevision: return EnumName<DICTOperator>::GetName<DICTOperator::CIDFontRevision>();
		case DICTOperator::CIDFontType: return EnumName<DICTOperator>::GetName<DICTOperator::CIDFontType>();
		case DICTOperator::CIDCount: return EnumName<DICTOperator>::GetName<DICTOperator::CIDCount>();
		case DICTOperator::UIDBase: return EnumName<DICTOperator>::GetName<DICTOperator::UIDBase>();
		case DICTOperator::FDArray: return EnumName<DICTOperator>::GetName<DICTOperator::FDArray>();
		case DICTOperator::FDSelect: return EnumName<DICTOperator>::GetName<DICTOperator::FDSelect>();
		case DICTOperator::FontName: return EnumName<DICTOperator>::GetName<DICTOperator::FontName>();
		case DICTOperator::BlueScale: return EnumName<DICTOperator>::GetName<DICTOperator::BlueScale>();
		case DICTOperator::BlueShift: return EnumName<DICTOperator>::GetName<DICTOperator::BlueShift>();
		case DICTOperator::BlueFuzz: return EnumName<DICTOperator>::GetName<DICTOperator::BlueFuzz>();
		case DICTOperator::StdHW: return EnumName<DICTOperator>::GetName<DICTOperator::StdHW>();
		case DICTOperator::StdVW: return EnumName<DICTOperator>::GetName<DICTOperator::StdVW>();
		case DICTOperator::ForceBold: return EnumName<DICTOperator>::GetName<DICTOperator::ForceBold>();
		case DICTOperator::LanguageGroup: return EnumName<DICTOperator>::GetName<DICTOperator::LanguageGroup>();
		case DICTOperator::ExpansionFactor: return EnumName<DICTOperator>::GetName<DICTOperator::ExpansionFactor>();
		case DICTOperator::initialRandomSeed: return EnumName<DICTOperator>::GetName<DICTOperator::initialRandomSeed>();
		case DICTOperator::Subrs: return EnumName<DICTOperator>::GetName<DICTOperator::Subrs>();
		case DICTOperator::defaultWidthX: return EnumName<DICTOperator>::GetName<DICTOperator::defaultWidthX>();
		case DICTOperator::nominalWidthX: return EnumName<DICTOperator>::GetName<DICTOperator::nominalWidthX>();
		case DICTOperator::Private: return EnumName<DICTOperator>::GetName<DICTOperator::Private>();
		case DICTOperator::ROS: return EnumName<DICTOperator>::GetName<DICTOperator::ROS>();
		case DICTOperator::FontMatrix: return EnumName<DICTOperator>::GetName<DICTOperator::FontMatrix>();
		case DICTOperator::FontBBox: return EnumName<DICTOperator>::GetName<DICTOperator::FontBBox>();
		case DICTOperator::XUID: return EnumName<DICTOperator>::GetName<DICTOperator::XUID>();
		case DICTOperator::BlueValues: return EnumName<DICTOperator>::GetName<DICTOperator::BlueValues>();
		case DICTOperator::OtherBlues: return EnumName<DICTOperator>::GetName<DICTOperator::OtherBlues>();
		case DICTOperator::FamilyBlues: return EnumName<DICTOperator>::GetName<DICTOperator::FamilyBlues>();
		case DICTOperator::FamilyOtherBlues: return EnumName<DICTOperator>::GetName<DICTOperator::FamilyOtherBlues>();
		case DICTOperator::StemSnapH: return EnumName<DICTOperator>::GetName<DICTOperator::StemSnapH>();
		case DICTOperator::StemSnapV: return EnumName<DICTOperator>::GetName<DICTOperator::StemSnapV>();
		default: return System::String(u"Undefined Operator");
		}
	}

	//GLYFテーブル解析用定数
	enum class SimpleGlyphFlags : uint8_t {
		ON_CURVE_POINT = 0x01,
		X_SHORT_VECTOR = 0x02,
		Y_SHORT_VECTOR = 0x04,
		REPEAT_FLAG = 0x08,
		X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10,
		Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20,
		OVERLAP_SIMPLE = 0x40,
		Reserved = 0x80
	};
	enum class CompositeGlyphFlags : uint16_t {
		ARG_1_AND_2_ARE_WORDS = 0x0001,
		ARGS_ARE_XY_VALUES = 0x0002,
		ROUND_XY_TO_GRID = 0x0004,
		WE_HAVE_A_SCALE = 0x0008,
		MORE_COMPONENTS = 0x0020,
		WE_HAVE_AN_X_AND_Y_SCALE = 0x0040,
		WE_HAVE_A_TWO_BY_TWO = 0x0080,
		WE_HAVE_INSTRUCTIONS = 0x0100,
		USE_MY_METRICS = 0x0200,
		OVERLAP_COMPOUND = 0x0400,
		SCALED_COMPONENT_OFFSET = 0x0800,
		UNSCALED_COMPONENT_OFFSET = 0x1000,
		Reserved = 0xE010
	};

	constexpr uint8_t operator&(uint8_t lhs, SimpleGlyphFlags rhs) noexcept {
		return lhs & static_cast<uint8_t>(rhs);
	}
	constexpr uint16_t operator&(uint16_t lhs, CompositeGlyphFlags rhs) noexcept {
		return lhs & static_cast<uint16_t>(rhs);
	}
	constexpr uint8_t operator~(SimpleGlyphFlags arg) noexcept { 
		return ~static_cast<uint8_t>(arg);
	}

	//Type2 CharString形式で使用する演算子
	enum class Type2Operator {
		Hstem = 0x01,
		Vstem = 0x03,
		Vmoveto,
		Rlineto,
		Hlineto,
		Vlineto,
		Rrcurveto,
		Callsubr = 0x0a,
		Return,
		Escape,
		Endchar = 0x0e,
		Hstemhm = 0x12,
		Hintmask,
		Cntrmask,
		Rmoveto,
		Hmoveto,
		Vstemhm,
		Rcurveline,
		Rlinecurve,
		Vvcurveto,
		Hhcurveto,
		Shortint,
		Callgsubr,
		Vhcurveto,
		Hvcurveto,
		And = 0x0c03,
		Or,
		Not,
		Abs = 0x0c09,
		Add,
		Sub,
		Div,
		Neg = 0x0c0e,
		Eq,
		Drop = 0x0c12,
		Put = 0x0c14,
		Get,
		Ifelse,
		Random,
		Mul,
		Sqrt = 0x0c1a,
		Dup,
		Exch,
		Index,
		Roll,
		Hflex = 0x0c22,
		Flex,
		Hflex1,
		Flex1
	};
}

//OffsetTable
export namespace System {
	/// <summary>
	/// オフセットテーブルを表す構造体。
	/// OpenTypeフォントファイルの先頭に配置され、ファイルタイプや次に配置される
	/// テーブルレコード配列の要素数などを示す。
	/// </summary>
	struct OffsetTable {
	private://アウトラインデータの形式を表す定数。sfnt_versionと比較する。
		static constexpr uint32_t TRUETYPE_VERSION = 0x00010000u;	//TrueTypeアウトライン
		static constexpr uint32_t CFF_VERSION = 0x4f54544fu;	//CFF(PostScriptアウトライン)
	public:
		uint32_t sfnt_version = 0;	//アウトライン形式
		uint16_t numTables = 0;	//テーブルレコード配列の要素数
		uint16_t searchRange = 0;
		uint16_t entrySelector = 0;
		uint16_t rangeShift = 0;
	public:
		/// <summary>
		/// ファイルからオフセットテーブルを読み取り、このインスタンスを更新する。
		/// </summary>
		/// <param name="file">
		/// OpenTypeフォントファイルを読み取り可能のバイナリデータとして開いたファイルストリーム。
		/// </param>
		/// <returns>
		/// 読み取り後のFileStreamの状態値。
		/// 先頭4バイトがOpenTypeフォントファイルのものと一致しない場合、false。
		/// </returns>
		bool Load(System::IO::FileStream& file) noexcept {
			if (!file) return false;
			file.ReadBE(sfnt_version);
			if (sfnt_version != TRUETYPE_VERSION && sfnt_version != CFF_VERSION) return false;
			file.ReadBE(numTables);
			file.ReadBE(searchRange);
			file.ReadBE(entrySelector);
			file.ReadBE(rangeShift);
			return static_cast<bool>(file);
		}
	public:
		bool HasCFF() const noexcept { return sfnt_version == CFF_VERSION; }
	};
}

//TableRecord
export namespace System {
	/// <summary>
	/// テーブルレコードを表す構造体。
	/// オフセットテーブルに続き配列として配置され、
	/// 各テーブルへのオフセットとそのテーブルのバイト長を示す。
	/// </summary>
	struct TableRecord {
		//テーブルの識別子
		char tag[4] = {};
		//テーブルの4バイトチェックサム。
		//HEAD::checkSumAdjustmentよりも先に計算する。
		//(HEADテーブル計算時にはHEAD::checkSumAdjustmentは0として扱う)
		uint32_t checkSum = 0;
		//ファイルの先頭からテーブルへのオフセット(バイト)
		uint32_t offset = 0;
		//テーブルのバイト長
		uint32_t length = 0;
	public:
		/// <summary>
		/// ファイルからテーブルレコードを読み取り、このインスタンスを更新する。
		/// </summary>
		/// <param name="file">
		/// OpenTypeフォントファイルを読み取り可能のバイナリデータとして開いたファイルストリーム。
		/// 現在のシーク位置から読み取る。
		/// </param>
		/// <returns>読み取り後のFileStreamの状態値</returns>
		bool Load(System::IO::FileStream& file) noexcept {
			if (!file) return false;
			file.ReadBE(tag);
			file.ReadBE(checkSum);
			file.ReadBE(offset);
			file.ReadBE(length);
			return static_cast<bool>(file);
		}
	};

}

//SubTables in CMAP
namespace System {
	/// <summary>
	/// CMAPクラスで使用されるサブテーブルの基底クラス
	/// </summary>
	struct SubTable {
		//サブテーブルのフォーマット番号
		uint16_t format;
	public:
		SubTable() noexcept : SubTable(0xffff) {}
		SubTable(uint16_t format) noexcept : format(format) {}
		virtual ~SubTable() noexcept = default;
	public:
		/// <summary>
		/// コードポイントに対応するグリフIDを取得する
		/// </summary>
		/// <param name="codePoint">コードポイント</param>
		/// <returns>グリフID。存在しないコードポイントの場合、0を返す。</returns>
		virtual uint16_t GetGID(uint32_t codePoint) const noexcept = 0;
		/// <summary>
		/// サブテーブルのバイト長を取得する
		/// </summary>
		virtual uint32_t Length() const noexcept = 0;
	};

	/// <summary>
	/// SubTableFormat0, 2, 4, 6の継承元
	/// </summary>
	struct SubTableBase16 : public SubTable {
		//サブテーブルのバイト長
		uint16_t length;
		//言語固有のエンコーディングを指定する。
		//このSubTableへのオフセットを持つEncodingRecordのplatformIDメンバが1以外の場合、0固定。
		//1(Macintosh)の場合、指定したい言語のMacintosh言語IDに1を加算した値を指定する。
		uint16_t language;
	public:
		SubTableBase16(System::IO::FileStream& file, uint16_t format) noexcept : SubTable(format) {
			if (format > 6) return;
			file.ReadBE(length);
			file.ReadBE(language);
		}
	public:
		uint32_t Length() const noexcept override { return length; }
	};

	/// <summary>
	/// SubTableFormat8, 10, 12, 13, 14の継承元
	/// </summary>
	struct SubTableBase32 : public SubTable {
		//サブテーブルのバイト長
		uint32_t length;
		//言語固有のエンコーディングを指定する。
		//このSubTableへのオフセットを持つEncodingRecordのplatformIDメンバが1以外の場合、0固定。
		//1(Macintosh)の場合、指定したい言語のMacintosh言語IDに1を加算した値を指定する。
		//Format14にはこのメンバが存在しない。
		uint32_t language;
	public:
		SubTableBase32(System::IO::FileStream& file, uint16_t format) noexcept : SubTable(format) {
			if (format < 8 || format > 14) return;
			if (format < 14) {
				uint16_t reserved;
				file.ReadBE(reserved);
			}
			file.ReadBE(length);
			if (format < 14) file.ReadBE(language);
		}
	public:
		uint32_t Length() const noexcept override { return length; }
	};

	/// <summary>
	/// 1バイト文字とグリフを関連付けるフォーマット。
	/// </summary>
	struct SubTableFormat0 : public SubTableBase16 {
		//グリフインデックスの配列。
		//文字コードを配列の添え字としたとき、値はグリフインデックスを表す。
		uint8_t glyphIdArray[256];
	public:
		SubTableFormat0(System::IO::FileStream& file) noexcept : SubTableBase16(file, 0) {
			file.ReadBE(glyphIdArray);
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			return static_cast<uint16_t>(codePoint <= 0xff ? glyphIdArray[static_cast<uint8_t>(codePoint)] : 0);
		}
	};

	/// <summary>
	/// 1バイト文字と2バイト文字が混合された文字コード(日本語、中国語、韓国語)とグリフを関連付けるフォーマット。
	/// </summary>
	struct SubTableFormat2 : public SubTableBase16 {
		/// <summary>
		/// 文字コードの下位バイトからGlyphIndexArrayへの対応を表す構造体
		/// </summary>
		struct SubHeader {
			//下位バイトの最初の値(最小値)
			uint16_t firstCode;
			//下位バイトの範囲([firstCode, firstCode + entryCode])
			uint16_t entryCount;
			//glyphIndexArray[]の対応する値に加算する差分値。
			//法は65536なので、グリフインデックスは(uint16_t)(glyphIndexArray[n] + idDelta)。
			int16_t idDelta;
			//このSubHeaderの次のバイトから、このSubHeaderに対応するglyphIndexArray[]までのバイト数
			uint16_t idRangeOffset;
		public:
			SubHeader() noexcept = default;
			SubHeader(System::IO::FileStream& file) noexcept {
				file.ReadBE(firstCode);
				file.ReadBE(entryCount);
				file.ReadBE(idDelta);
				file.ReadBE(idRangeOffset);
			}
		};
	public:
		//文字コードの上位バイトをインデックスとし、対応するsubHeaders[]のインデックスの8倍を値とする配列
		uint16_t subHeaderKeys[256];
		//要素数は(subHeaderKeys[]の0でない要素数 + 1)
		Vector<SubHeader> subHeaders;
		// グリフインデックスの配列。
		// subHeaders[n]にはインデックス番号
		// (subHeaders[n].idRangeOffset - (subHeaders.Count() - n - 1) * sizeof(SubHeader)) / sizeof(glyphIndexArray[0])が対応する。
		uint16_t* glyphIndexArray = nullptr;
	public:
		SubTableFormat2(System::IO::FileStream& file) noexcept : SubTableBase16(file, 2) {
			file.ReadBE(subHeaderKeys);
			uint16_t subHeaderCount = 0;
			//subHeaderKeysの中で一番大きい値を8で割った値がSubHeadersの最後のインデックス
			for (uint16_t i = 0; i < 256; ++i) {
				if (subHeaderCount < subHeaderKeys[i]) subHeaderCount = subHeaderKeys[i];
			}
			subHeaderCount /= 8;
			++subHeaderCount;
			subHeaders.Reserve(subHeaderCount);
			for (uint16_t i = 0; i < subHeaderCount; ++i) subHeaders.Add(SubHeader(file));
			//glyphIndexArray[]のバイト長は(サブテーブルのバイト長)-(ここまで読み込んだバイト数)
			uint16_t glyphIndexCount = (this->length - (6 + sizeof(subHeaderKeys) + sizeof(SubHeader) * subHeaderCount)) / sizeof(uint16_t);
			glyphIndexArray = new uint16_t[glyphIndexCount];
			file.ReadBE(glyphIndexArray, glyphIndexCount);
		}
		~SubTableFormat2() noexcept {
			delete glyphIndexArray;
			glyphIndexArray = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			if (codePoint > 0xffff) return 0;
			uint8_t high = static_cast<uint8_t>((codePoint >> 8) & 0x000000ff);
			if (high == 0) return glyphIndexArray[codePoint];
			else {
				uint16_t subHeaderIndex = subHeaderKeys[high] / 8;
				const SubHeader& subHeader = subHeaders[subHeaderIndex];
				uint8_t low = static_cast<uint8_t>(codePoint & 0x000000ff);
				if (low >= subHeader.firstCode && low < (subHeader.firstCode + subHeader.entryCount)) {
					uint16_t glyphIndexArrayIndex = static_cast<uint16_t>((subHeader.idRangeOffset - (subHeaders.Count() - subHeaderIndex - 1) * sizeof(SubHeader)) / sizeof(glyphIndexArray[0]));
					uint16_t baseGID = glyphIndexArray[glyphIndexArrayIndex];
					return static_cast<uint16_t>(baseGID ? baseGID + subHeader.idDelta + (low - subHeader.firstCode) : 0);
				}
				else return 0;	//定義上ありえない
			}
		}
	};

	/// <summary>
	/// UnicodeのU+0000からU+FFFF(基本多言語面文字)のみをグリフに関連付けるフォーマット
	/// </summary>
	struct SubTableFormat4 : public SubTableBase16 {
		//セグメントの個数を2倍した値。
		//endCode[], startCode[], idDelta[], idRangeOffset[]の要素数は(segCountX2 / 2)。
		uint16_t segCountX2;
		//2 * (2^floor(log2(segCount)))が入る
		uint16_t searchRange;
		//log2(searchRange / 2)が入る
		uint16_t entrySelector;
		//segCountX2 - searchRangeが入る
		uint16_t rangeShift;
		//対応するセグメントの最後の文字コードを示す配列。最後の要素は0xffff固定。
		uint16_t* endCode = nullptr;
		//0固定
		uint16_t reservedPad;
		//対応するセグメントの最初の文字コードを示す配列
		uint16_t* startCode = nullptr;
		//対応するセグメントに含まれるすべての文字コードに加算される差分値の配列
		int16_t* idDelta = nullptr;
		//対応するglyphIDArrayの位置までのオフセットの配列
		uint16_t* idRangeOffset = nullptr;
		//要素数は(length - (sizeof(uint16_t) * (8 + segCount * 4))) / sizeof(uint16_t)
		uint16_t* glyphIdArray = nullptr;
	public:
		SubTableFormat4(System::IO::FileStream& file) noexcept : SubTableBase16(file, 4) {
			file.ReadBE(segCountX2);
			file.ReadBE(searchRange);
			file.ReadBE(entrySelector);
			file.ReadBE(rangeShift);
			uint16_t segCount = segCountX2 / 2;
			endCode = new uint16_t[segCount];
			file.ReadBE(endCode, segCount);
			file.ReadBE(reservedPad);
			startCode = new uint16_t[segCount];
			file.ReadBE(startCode, segCount);
			idDelta = new int16_t[segCount];
			file.ReadBE(idDelta, segCount);
			idRangeOffset = new uint16_t[segCount];
			file.ReadBE(idRangeOffset, segCount);
			uint16_t glyphIdCount = (this->length - (sizeof(uint16_t) * (8 + segCount * 4))) / sizeof(uint16_t);
			glyphIdArray = new uint16_t[glyphIdCount];
			file.ReadBE(glyphIdArray, glyphIdCount);
		}
		~SubTableFormat4() noexcept {
			delete endCode;
			endCode = nullptr;
			delete startCode;
			startCode = nullptr;
			delete idDelta;
			idDelta = nullptr;
			delete idRangeOffset;
			idRangeOffset = nullptr;
			delete glyphIdArray;
			glyphIdArray = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			if (codePoint > 0xffff) return 0;
			uint16_t segCount = segCountX2 / 2;
			for (uint16_t i = 0; i < segCount; ++i) {
				if (codePoint <= endCode[i]) {
					if (codePoint >= startCode[i]) {
						if (idRangeOffset[i] == 0) return static_cast<uint16_t>((codePoint + idDelta[i]) & 0xffff);
						else {
							uint16_t index = (((idRangeOffset[i] + (codePoint - startCode[i]) * 2) - (segCount - i - 1) * sizeof(idRangeOffset[0])) & 0xffff) / sizeof(glyphIdArray[0]);
							return glyphIdArray[index];
						}
					}
					else return 0;	//セグメント範囲外
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// Format4においてフォント内の文字コードが連続した単一の範囲に存在する場合に使用されるフォーマット。
	/// 16bitUnicodeを表すサブテーブルはFormat4とFormat6の一方のみファイルに含まれる。
	/// (両方ある場合、Format4を優先する)
	/// </summary>
	struct SubTableFormat6 : public SubTableBase16 {
		//連続する文字コード範囲の最初の値
		uint16_t firstCode;
		//連続する文字コード範囲に含まれる文字コードの数
		uint16_t entryCount;
		//[entryCount]
		uint16_t* glyphIdArray = nullptr;
	public:
		SubTableFormat6(System::IO::FileStream& file) noexcept : SubTableBase16(file, 6) {
			file.ReadBE(firstCode);
			file.ReadBE(entryCount);
			glyphIdArray = new uint16_t[entryCount];
			file.ReadBE(glyphIdArray, entryCount);
		}
		~SubTableFormat6() noexcept {
			delete glyphIdArray;
			glyphIdArray = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			if (codePoint < firstCode) return 0;
			uint16_t index = codePoint - firstCode;
			return index < entryCount ? glyphIdArray[index] : 0;
		}
	};

	/// <summary>
	/// Format8, 12で使用される文字コードのグループを表す構造体
	/// </summary>
	struct SequentialMapGroup {
		//このグループの最初の文字コード。直前のグループのendCharCodeよりも大きい。
		uint32_t startCharCode;
		//このグループの最後の文字コード
		uint32_t endCharCode;
		union {
			//startCharCodeに対応するグリフID。Format8, 12で使用
			uint32_t startGlyphID;
			//このグループすべての文字コードに対応するグリフID。Format13で使用
			uint32_t glyphID;
		};
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(startCharCode);
			file.ReadBE(endCharCode);
			file.ReadBE(startGlyphID);
			return static_cast<bool>(file);
		}
	};
	/// <summary>
	/// Format13で使用される文字コードのグループを表す構造体
	/// </summary>
	using ConstantMapGroup = SequentialMapGroup;

	/// <summary>
	/// 16bitUnicode(0x0000-0xffff)と32bitUnicode(0x10000-0x10ffff)が混ざったフォーマット
	/// </summary>
	struct SubTableFormat8 : public SubTableBase32 {
		uint8_t is32[8192];
		uint32_t numGroups;
		//[numGroups]
		SequentialMapGroup* groups = nullptr;
	public:
		SubTableFormat8(System::IO::FileStream& file) noexcept : SubTableBase32(file, 8) {
			file.ReadBE(is32);
			file.ReadBE(numGroups);
			groups = new SequentialMapGroup[numGroups];
			for (uint32_t i = 0; i < numGroups; ++i) groups[i].Load(file);
		}
		~SubTableFormat8() noexcept {
			delete groups;
			groups = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			uint16_t high = static_cast<uint16_t>(codePoint >> 16);
			uint8_t check = static_cast<uint8_t>(is32[high / 8] & (1 << (7 - (high % 8))));
			for (uint32_t i = 0; i < numGroups; ++i) {
				SequentialMapGroup& group = groups[i];
				if (group.startCharCode <= codePoint && codePoint <= group.endCharCode) {
					return static_cast<uint16_t>(group.startGlyphID + (codePoint - group.startCharCode));
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// 文字コード(Unicode 0x0000-0x10ffff)が連続した単一の範囲に存在する場合に使用されるフォーマット
	/// </summary>
	struct SubTableFormat10 : public SubTableBase32 {
		//連続する文字コード範囲の最初の値
		uint32_t startCharCode;
		//連続する文字コード範囲に含まれる文字コードの数
		uint32_t numChars;
		//[numChars]
		uint16_t* glyphs = nullptr;
	public:
		SubTableFormat10(System::IO::FileStream& file) noexcept : SubTableBase32(file, 10) {
			file.ReadBE(startCharCode);
			file.ReadBE(numChars);
			glyphs = new uint16_t[numChars];
			file.ReadBE(glyphs, numChars);
		}
		~SubTableFormat10() noexcept {
			delete glyphs;
			glyphs = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			if (codePoint < startCharCode) return 0;
			uint32_t index = codePoint - startCharCode;
			return static_cast<uint16_t>(index < numChars ? glyphs[index] : 0);
		}
	};

	/// <summary>
	/// Unicode 0x0000-0x10ffffの文字コードをグループ単位でグリフIDと関連付けるフォーマット。
	/// Format8と異なり、すべての文字コードを32bitとして扱う
	/// </summary>
	struct SubTableFormat12 : public SubTableBase32 {
		uint32_t numGroups;
		SequentialMapGroup* groups = nullptr;
	public:
		SubTableFormat12(System::IO::FileStream& file) noexcept : SubTableBase32(file, 12) {
			file.ReadBE(numGroups);
			groups = new SequentialMapGroup[numGroups];
			for (uint32_t i = 0; i < numGroups; ++i) groups[i].Load(file);
		}
		~SubTableFormat12() noexcept {
			delete groups;
			groups = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			for (uint32_t i = 0; i < numGroups; ++i) {
				if (groups[i].startCharCode <= codePoint && codePoint <= groups[i].endCharCode) {
					return groups[i].startGlyphID + (codePoint - groups[i].startCharCode);
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// Format12と同じ構成のフォーマット。
	/// グループ内の文字コードすべてに同じグリフIDを対応させる点のみが異なる
	/// </summary>
	struct SubTableFormat13 : public SubTableBase32 {
		uint32_t numGroups;
		ConstantMapGroup* groups = nullptr;
	public:
		SubTableFormat13(System::IO::FileStream& file) noexcept : SubTableBase32(file, 13) {
			file.ReadBE(numGroups);
			groups = new ConstantMapGroup[numGroups];
			for (uint32_t i = 0; i < numGroups; ++i) groups[i].Load(file);
		}
		~SubTableFormat13() noexcept {
			delete groups;
			groups = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			for (uint32_t i = 0; i < numGroups; ++i) {
				if (groups[i].startCharCode <= codePoint) {
					if (groups[i].endCharCode < codePoint) return 0;
					else return groups[i].glyphID;
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// 対応する異体字セレクタ(VariationSelector::varSelector)が設定されている
	/// 文字コードを表す構造体
	/// </summary>
	struct DefaultUVS {
		/// <summary>
		/// 文字コード範囲を表す構造体
		/// </summary>
		struct UnicodeRange {
			//この範囲の最初の文字コード
			uint24_c startUnicodeValue;
			//startUnicodeValueに続くこの範囲の文字コード数。
			//startUnicodeValue + additionalCountがこの範囲の最後の文字コードとなる
			uint8_t additionalCount;
		public:
			bool Load(System::IO::FileStream& file) noexcept {
				file.ReadBE(startUnicodeValue);
				file.ReadBE(additionalCount);
				return static_cast<bool>(file);
			}
		};
	public:
		uint32_t numUnicodeValueRanges;
		//対応する異体字セレクタが設定されている文字コード範囲の配列
		//[numUnicodeValueRanges]
		UnicodeRange* ranges = nullptr;
	public:
		DefaultUVS() noexcept = default;
		DefaultUVS(DefaultUVS&& arg) noexcept : numUnicodeValueRanges(arg.numUnicodeValueRanges), ranges(arg.ranges) {
			arg.numUnicodeValueRanges = 0;
			arg.ranges = nullptr;
		}
		~DefaultUVS() {
			delete[] ranges;
			ranges = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(numUnicodeValueRanges);
			ranges = new UnicodeRange[numUnicodeValueRanges];
			for (uint32_t i = 0; i < numUnicodeValueRanges; ++i) ranges[i].Load(file);
			return static_cast<bool>(file);
		}
	public:
		DefaultUVS& operator=(DefaultUVS&& rhs) noexcept {
			if (this == &rhs) return *this;
			delete[] ranges;
			ranges = rhs.ranges;
			numUnicodeValueRanges = rhs.numUnicodeValueRanges;
			rhs.ranges = nullptr;
			rhs.numUnicodeValueRanges = 0;
			return *this;
		}
	};

	/// <summary>
	/// 対応する異体字セレクタ(VariationSelector::varSelector)が設定されている
	/// 文字コードとグリフIDの組み合わせを表す構造体
	/// </summary>
	struct NonDefaultUVS {
		/// <summary>
		/// 文字コードに対応するグリフIDを表す構造体
		/// </summary>
		struct UVSMapping {
			//文字コード
			uint24_c unicodeValue;
			//unicodeValue + 異体字セレクタに対応するグリフID
			uint16_t glyphID;
		public:
			bool Load(System::IO::FileStream& file) noexcept {
				file.ReadBE(unicodeValue);
				file.ReadBE(glyphID);
				return static_cast<bool>(file);
			}
		};
	public:
		uint32_t numUVSMappings;
		//文字コードとグリフIDの組み合わせの配列
		//[numUVSMappings]
		UVSMapping* uvsMappings = nullptr;
	public:
		NonDefaultUVS() noexcept = default;
		NonDefaultUVS(NonDefaultUVS&& arg) noexcept : numUVSMappings(arg.numUVSMappings), uvsMappings(arg.uvsMappings) {
			arg.numUVSMappings = 0;
			arg.uvsMappings = nullptr;
		}
		~NonDefaultUVS() {
			delete[] uvsMappings;
			uvsMappings = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(numUVSMappings);
			uvsMappings = new UVSMapping[numUVSMappings];
			for (uint32_t i = 0; i < numUVSMappings; ++i) uvsMappings[i].Load(file);
			return static_cast<bool>(file);
		}
	public:
		NonDefaultUVS& operator=(NonDefaultUVS&& rhs) noexcept {
			if (this == &rhs) return *this;
			delete[] uvsMappings;
			uvsMappings = rhs.uvsMappings;
			numUVSMappings = rhs.numUVSMappings;
			rhs.uvsMappings = nullptr;
			rhs.numUVSMappings = 0;
			return *this;
		}
	};

	/// <summary>
	/// Format14で使用される異体字セレクタとそのマッピングに使用される
	/// UVSテーブルへのオフセットを表す構造体
	/// </summary>
	struct VariationSelector {
		//異体字セレクタ
		uint24_c varSelector;
		//サブテーブルの先頭からdefaultUVSへのオフセット。
		//SubTableFormat14::defaultUVS読み込み後はインデックスを格納する
		uint32_t defaultUVSOffset;
		//サブテーブルの先頭からnonDefaultUVSへのオフセット
		//SubTableFormat14::nonDefaultUVS読み込み後はインデックスを格納する
		uint32_t nonDefaultUVSOffset;
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(varSelector);
			file.ReadBE(defaultUVSOffset);
			file.ReadBE(nonDefaultUVSOffset);
			return static_cast<bool>(file);
		}
	};

	/// <summary>
	/// Unicodeバリエーションシークエンス(異体字セレクタ)に対応したフォーマット。
	/// EncodingRecordがplatformID: 0かつencodingID: 5の場合のみ使用する
	/// </summary>
	struct SubTableFormat14 : public SubTableBase32 {
		uint32_t numVarSelectorRecords;
		//[numVarSelectorRecords]
		VariationSelector* varSelector = nullptr;
		//varSelector[n].defaultUVSOffsetをインデックスとする動的配列
		Vector<DefaultUVS> defaultUVS;
		//varSelector[n].nonDefaultUVSOffsetをインデックスとする動的配列
		Vector<NonDefaultUVS> nonDefaultUVS;
	public:
		SubTableFormat14(System::IO::FileStream& file) noexcept : SubTableBase32(file, 14) {
			//SubTableBase32で読み込まれるバイト数戻して、サブテーブルの先頭位置を取得する
			file.Seek(-6, System::IO::SeekDir::Current);
			System::IO::StreamPos subTableBegin = file.TellPos();
			file.Seek(6, System::IO::SeekDir::Current);
			file.ReadBE(numVarSelectorRecords);
			varSelector = new VariationSelector[numVarSelectorRecords];
			for (uint32_t i = 0; i < numVarSelectorRecords; ++i) varSelector[i].Load(file);
			defaultUVS.Reserve(numVarSelectorRecords);
			nonDefaultUVS.Reserve(numVarSelectorRecords);
			//すでに読み込んだオフセット値を記録する
			Vector<uint32_t> defaultUVSOffsets;
			Vector<uint32_t> nonDefaultUVSOffsets;
			defaultUVSOffsets.Reserve(numVarSelectorRecords);
			nonDefaultUVSOffsets.Reserve(numVarSelectorRecords);
			for (VariationSelector* selector = varSelector, *end = varSelector + numVarSelectorRecords; selector != end; ++selector) {
				size_t offsetCount = defaultUVSOffsets.Count();
				size_t offsetIndex = defaultUVSOffsets.IndexOf(selector->defaultUVSOffset);
				if (offsetCount != offsetIndex) selector->defaultUVSOffset = static_cast<uint32_t>(offsetIndex);
				else {
					file.Seek(subTableBegin);
					file.Seek(selector->defaultUVSOffset, System::IO::SeekDir::Current);
					DefaultUVS tmp;
					tmp.Load(file);
					selector->defaultUVSOffset = static_cast<uint32_t>(defaultUVS.Add(static_cast<DefaultUVS&&>(tmp)));
				}
				offsetCount = nonDefaultUVSOffsets.Count();
				offsetIndex = nonDefaultUVSOffsets.IndexOf(selector->nonDefaultUVSOffset);
				if (offsetCount != offsetIndex) selector->nonDefaultUVSOffset = static_cast<uint32_t>(offsetIndex);
				else {
					file.Seek(subTableBegin);
					file.Seek(selector->nonDefaultUVSOffset, System::IO::SeekDir::Current);
					NonDefaultUVS tmp;
					tmp.Load(file);
					selector->nonDefaultUVSOffset = static_cast<uint32_t>(nonDefaultUVS.Add(static_cast<NonDefaultUVS&&>(tmp)));
				}
			}
		}
		~SubTableFormat14() noexcept {
			delete[] varSelector;
			varSelector = nullptr;
		}
	public:
		uint16_t GetGID(uint32_t codePoint) const noexcept override {
			return 0;
		}
	};
}

//CMAP
export namespace System {
	/// <summary>
	/// cmapテーブルを表す構造体。
	/// 文字コードからグリフインデックスへのマッピングを定義している
	/// </summary>
	struct CMAP {
		/// <summary>
		/// サブテーブルのエンコーディング情報を表す構造体
		/// </summary>
		struct EncodingRecord {
			//プラットフォーム識別子
			//0 : Unicode
			//1 : Macintosh(非推奨・下位互換維持用)
			//2 : do not use
			//3 : Microsoft
			uint16_t platformID = 0;
			//プラットフォーム固有エンコード識別子
			//・platformIDが0(Unicode)の場合
			//0 : デフォルトのセマンティクス
			//1 : version 1.1のセマンティクス
			//2 : ISO 10646 1993のセマンティクス
			//3 : Unicode 2.0以降のセマンティクス(BMPのみ)
			//4 : Unicode 2.0以降のセマンティクス
			//5 : Unicode Variation Sequence
			//6 : Unicodeを完全網羅(OpenTypeの13.0cmapsを使用)
			//・platformIDが3(Microsoft)の場合
			//0 : シンボル
			//1 : Unicode BMPのみ
			//2 : Shift-JIS
			//3 : PRC
			//4 : BigFive
			//5 : Johab
			//10 : Unicode UCS-4
			uint16_t encodingID = 0;
			//CMAPテーブル先頭からサブテーブルへのオフセット(バイト)
			uint32_t offset = 0;
		};
	public:
		//バージョン数(0固定)
		uint16_t version = 0;
		//encodingRecords[]の要素数
		uint16_t numTables = 0;
		//[numTables]
		EncodingRecord* encodingRecords = nullptr;
		Vector<SubTable*> subTables;
	private:
		uint16_t m_unicodeSubtableIndex = System::MAX_VALUE<uint16_t>;
	public:
		~CMAP() {
			delete[] encodingRecords;
			encodingRecords = nullptr;
			for (SubTable* x : subTables) delete x;
			subTables.Clear();
		}
	private:
		uint16_t GetUnicodeSubTableIndex() const noexcept {
			uint16_t ret = System::MAX_VALUE<uint16_t>;
			uint16_t platformID = System::MAX_VALUE<uint16_t>;
			uint16_t encodingID = System::MAX_VALUE<uint16_t>;
			for (uint16_t i = 0; i < numTables; ++i) {
				EncodingRecord& enc = encodingRecords[i];
				if (enc.platformID == 0) {
					if (enc.encodingID == 5) continue;
					if (platformID == System::MAX_VALUE<uint16_t>) {
						platformID = enc.platformID;
						encodingID = enc.encodingID;
						ret = i;
					}
					else {
						if (platformID == 0) {
							if (encodingID < enc.encodingID) {
								encodingID = enc.encodingID;
								ret = i;
							}
						}
						else if (platformID == 3) {
							if (encodingID != 10 && enc.encodingID > 3) {
								platformID = enc.platformID;
								encodingID = enc.encodingID;
								ret = i;
							}
						}
					}
				}
				else if (enc.platformID == 3) {
					if (enc.encodingID == 1 || enc.encodingID == 10) {
						if (platformID == System::MAX_VALUE<uint16_t>) {
							platformID = enc.platformID;
							encodingID = enc.encodingID;
							ret = i;
						}
						else {
							if (platformID == 0) {
								if (encodingID < 3) {
									platformID = enc.platformID;
									encodingID = enc.encodingID;
									ret = i;
								}
							}
							else if (platformID == 3) {
								if (encodingID < enc.encodingID) {
									encodingID = enc.encodingID;
									ret = i;
								}
							}
						}
					}
				}
			}
			return ret;
		}
	public:
		/// <summary>
		/// ファイルからCMAPテーブルを読み取り、このインスタンスを更新する
		/// </summary>
		/// <param name="file">
		/// OpenTypeフォントファイルを読み取り可能のバイナリデータとして開いたファイルストリーム
		/// </param>
		/// <param name="offset">
		/// ファイル先頭からCMAPテーブルへのオフセット(バイト)。
		/// TableRecord::offsetの値を指定する。
		/// </param>
		/// <returns>読み取り後のFileStreamの状態値</returns>
		bool Load(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			System::IO::StreamPos cmapPos = file.TellPos();
			file.ReadBE(version);
			file.ReadBE(numTables);
			//エンコーディングレコード取得
			encodingRecords = new EncodingRecord[numTables];
			for (uint16_t i = 0; i < numTables; ++i) {
				file.ReadBE(encodingRecords[i].platformID);
				file.ReadBE(encodingRecords[i].encodingID);
				file.ReadBE(encodingRecords[i].offset);
			}
			//一番範囲の大きいUnicode用SubTableのインデックスを保存する
			m_unicodeSubtableIndex = GetUnicodeSubTableIndex();
			//サブテーブル取得
			subTables.Reserve(numTables);
			for (uint16_t i = 0; i < numTables; ++i) {
				file.Seek(cmapPos);
				file.Seek(encodingRecords[i].offset, System::IO::SeekDir::Current);
				uint16_t format = 0xffff;
				file.ReadBE(format);
				if (format == 0xffff) return false;
				switch (format) {
				case 0:
					subTables.Add(new SubTableFormat0(file));
					break;
				case 2:
					subTables.Add(new SubTableFormat2(file));
					break;
				case 4:
					subTables.Add(new SubTableFormat4(file));
					break;
				case 6:
					subTables.Add(new SubTableFormat6(file));
					break;
				case 8:
					subTables.Add(new SubTableFormat8(file));
					break;
				case 10:
					subTables.Add(new SubTableFormat10(file));
					break;
				case 12:
					subTables.Add(new SubTableFormat12(file));
					break;
				case 13:
					subTables.Add(new SubTableFormat13(file));
					break;
				case 14:
					subTables.Add(new SubTableFormat14(file));
					break;
				default:
					return false;
				}
			}
			return static_cast<bool>(file);
		}
		/// <summary>
		/// ファイルからCMAPバージョンとすべてのエンコーディングレコード、
		/// および一番広いUnicode範囲に対応したサブテーブルを選択して読み取り、
		/// このインスタンスを更新する
		/// </summary>
		/// <param name="file">
		/// OpenTypeフォントファイルを読み取り可能のバイナリデータとして開いたファイルストリーム
		/// </param>
		/// <param name="offset">
		/// ファイル先頭からCMAPテーブルへのオフセット(バイト)。
		/// TableRecord::offsetの値を指定する。
		/// </param>
		/// <returns>読み取り後のFileStreamの状態値</returns>
		bool QuickLoad(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			System::IO::StreamPos cmapPos = file.TellPos();
			file.ReadBE(version);
			file.ReadBE(numTables);
			//エンコーディングレコード取得
			encodingRecords = new EncodingRecord[numTables];
			for (uint16_t i = 0; i < numTables; ++i) {
				file.ReadBE(encodingRecords[i].platformID);
				file.ReadBE(encodingRecords[i].encodingID);
				file.ReadBE(encodingRecords[i].offset);
			}
			//一番範囲の大きいUnicode用SubTableのインデックスを保存する
			m_unicodeSubtableIndex = GetUnicodeSubTableIndex();
			//(m_unicodeSubtableIndex)番目のサブテーブルのみ取得
			file.Seek(cmapPos);
			file.Seek(encodingRecords[m_unicodeSubtableIndex].offset, System::IO::SeekDir::Current);
			uint16_t format = 0xffff;
			file.ReadBE(format);
			if (format == 0xffff) return false;
			switch (format) {
			case 0:
				subTables.Add(new SubTableFormat0(file));
				break;
			case 2:
				subTables.Add(new SubTableFormat2(file));
				break;
			case 4:
				subTables.Add(new SubTableFormat4(file));
				break;
			case 6:
				subTables.Add(new SubTableFormat6(file));
				break;
			case 8:
				subTables.Add(new SubTableFormat8(file));
				break;
			case 10:
				subTables.Add(new SubTableFormat10(file));
				break;
			case 12:
				subTables.Add(new SubTableFormat12(file));
				break;
			case 13:
				subTables.Add(new SubTableFormat13(file));
				break;
			case 14:
				subTables.Add(new SubTableFormat14(file));
				break;
			default:
				return false;
			}
			m_unicodeSubtableIndex = 0;
			return static_cast<bool>(file);
		}
	public:
		uint16_t GetGID(uint32_t unicodePoint, uint32_t uvs) const noexcept {
			if (m_unicodeSubtableIndex >= numTables) return 0;
			SubTable* subTable = subTables[m_unicodeSubtableIndex];
			return subTable ? subTable->GetGID(unicodePoint) : 0;
		}
	};
}

//HEAD
export namespace System {
	/// <summary>
	/// headテーブルを表す構造体。
	/// フォントに関するグローバルな情報が格納されている
	/// </summary>
	struct HEAD {
		//0x0001固定
		uint16_t majorVersion = 0;	//0x0001固定
		//0x0000固定
		uint16_t minorVersion = 0;
		//フォントメーカーが指定。16.16の固定小数点数(Fixed)。
		//フォントのバージョンはこのプロパティではなく、nameテーブルを参照する
		int32_t fontRevision = 0;
		//フォント全体の4バイトチェックサムから0xb1b0afbaを引いた値
		uint32_t checkSumAdjustment = 0;
		//0x5f0f3cf5固定
		uint32_t magicNumber = 0;
		//各ビットフラグの和
		//0: フォントのベースラインがy=0
		//1: 左サイドベアリングがx=0(TrueTypeラスタイザのみ)
		//2: ポイントサイズによって意味が異なる
		//3: 内部スケール演算で整数ppemを強制する。オフの時、小数ppemが使用可能。
		//4: 
		//5: 使用しない(一部プラットフォームで垂直レイアウトに影響)
		//6-10: 使用しない
		//11: ロスレス圧縮されている。DSIGテーブルが無効になる可能性あり
		//12: メトリクス互換
		//13: ClearTypeに最適化されている。EBDTを使用するフォントはClearTypeではない(のでオフ)。
		//14: オフの時、cmapから取得されるグリフインデックスがコードポイントに対応したグリフを表している
		//15: 0固定
		uint16_t flags = 0;
		//[16, 16384]のいずれか。TrueTypeでは2の累乗推奨
		uint16_t unitsPerEm = 0;
		//作成日時(1904/1/1 00:00からの秒数)
		int64_t created = 0;
		//修正日時(1904/1/1 00:00からの秒数)
		int64_t modified = 0;
		//すべてのグリフ境界ボックスの最小x座標
		int16_t xMin = 0;
		//すべてのグリフ境界ボックスの最小y座標
		int16_t yMin = 0;
		//すべてのグリフ境界ボックスの最大x座標
		int16_t xMax = 0;
		//すべてのグリフ境界ボックスの最大y座標
		int16_t yMax = 0;
		//各ビットフラグの和。OS_2::fsSelectionと一致する。
		//0: 太字(Bold)
		//1: イタリック体(Italic)
		//2: 下線(Underline)
		//3: 輪郭線(Outline)
		//4: 影付き(Shadow)
		//5: 横幅が細い(Condensed)
		//6: 横幅が太い(Extended)
		//7-15: 予約領域(0固定)
		uint16_t macStyle = 0;
		//読み取り可能な最小ピクセル
		uint16_t lowestRecPPEM = 0;
		//方向ヒント。非推奨なプロパティであり、2に固定される。
		int16_t fontDirectionHint = 0;
		//loca.offsets[maxp.numGlyphs + 1]の型を指定
		//0 : uint16_t(実際のオフセット値を2で割ったものが保存されている)
		//1 : uint32_t(実際のオフセット値が保存されている)
		int16_t indexToLocFormat = 0;
		//0固定
		int16_t glyphDataFormat = 0;
	public:
		/// <summary>
		/// ファイルからHEADテーブルを読み取り、このインスタンスを更新する
		/// </summary>
		/// <param name="file">
		/// OpenTypeフォントファイルを読み取り可能のバイナリデータとして開いたファイルストリーム
		/// </param>
		/// <param name="offset">
		/// ファイル先頭からHEADテーブルへのオフセット(バイト)。
		/// TableRecord::offsetの値を指定する。
		/// </param>
		/// <returns>読み取り後のFileStreamの状態値</returns>
		bool Load(System::IO::FileStream& file, uint32_t offset) noexcept {
			if (!file) return false;
			file.Seek(static_cast<System::IO::StreamOff>(offset), System::IO::SeekDir::Begin);
			file.ReadBE(majorVersion);
			file.ReadBE(minorVersion);
			file.ReadBE(fontRevision);
			file.ReadBE(checkSumAdjustment);
			file.ReadBE(magicNumber);
			file.ReadBE(flags);
			file.ReadBE(unitsPerEm);
			file.ReadBE(created);
			file.ReadBE(modified);
			file.ReadBE(xMin);
			file.ReadBE(yMin);
			file.ReadBE(xMax);
			file.ReadBE(yMax);
			file.ReadBE(macStyle);
			file.ReadBE(lowestRecPPEM);
			file.ReadBE(fontDirectionHint);
			file.ReadBE(indexToLocFormat);
			file.ReadBE(glyphDataFormat);
			return static_cast<bool>(file);
		}
	};
}

//HHEA
export namespace System {
	/// <summary>
	/// hheaテーブルを表す構造体。
	/// 水平方向レイアウト情報を含んでいる
	/// </summary>
	struct HHEA {
		//テーブルのメジャーバージョン。1固定
		uint16_t majorVersion = 0;
		//テーブルのマイナーバージョン。0固定
		uint16_t minorVersion = 0;
		//ベースラインから最も高いアセンダーラインまでの距離
		int16_t ascender = 0;
		//ベースラインから最も低いディセンダーラインまでの距離
		int16_t descender = 0;
		//行間の幅
		int16_t lineGap = 0;
		//hmtxテーブルの最大前進幅
		uint16_t advanceWidthMax = 0;
		//hmtxテーブルの最小左サイドベアリング値(輪郭線を持つグリフのみ)
		int16_t minLeftSideBearing = 0;
		//hmtxテーブルの最小右サイドベアリング値(輪郭線を持つグリフのみ)
		int16_t minRightSideBearing = 0;
		//左サイドベアリングと(xMax-XMin)の和の最大値
		int16_t xMaxExtent = 0;
		//入力カーソルの勾配の分子(垂直の場合、1)
		int16_t caretSlopeRise = 0;
		//入力カーソルの勾配の分母(垂直の場合、0)
		int16_t caretSlopeRun = 0;
		//斜体フォントの場合、入力カーソルを移動させるオフセット。その他の場合、0
		int16_t caretOffset = 0;
		//0固定
		/*int16_t reserved[4] */
		//0固定
		int16_t metricDataFormat = 0;
		//HMTX::hMetricの要素数
		uint16_t numberOfHMetrics = 0;
	public:
		//HHEAテーブルへのオフセットを指定してファイルを読み取り、構造体を初期化する
		bool Load(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			file.ReadBE(majorVersion);
			file.ReadBE(minorVersion);
			file.ReadBE(ascender);
			file.ReadBE(descender);
			file.ReadBE(lineGap);
			file.ReadBE(advanceWidthMax);
			file.ReadBE(minLeftSideBearing);
			file.ReadBE(minRightSideBearing);
			file.ReadBE(xMaxExtent);
			file.ReadBE(caretSlopeRise);
			file.ReadBE(caretSlopeRun);
			file.ReadBE(caretOffset);
			int16_t reserved[4];
			file.ReadBE(reserved);
			file.ReadBE(metricDataFormat);
			file.ReadBE(numberOfHMetrics);
			return static_cast<bool>(file);
		}
	public:
		int16_t GetAscender() const noexcept { return ascender; }
		int16_t GetDescender() const noexcept { return descender; }
		int16_t GetLineGap() const noexcept { return lineGap; }
	};
}

//MAXP
export namespace System {
	/// <summary>
	/// maxpテーブルを表す構造体。
	/// フォントの持つ様々な情報の最大値を含んでいる
	/// </summary>
	struct MAXP {
		//MAXPテーブルのバージョン。16.16の固定小数点数で表す。
		//0x00010000(1.0)の場合、すべてのプロパティを持つ
		//0x00005000(0.5)の場合、version, numGlyphsのみ持つ
		int32_t version;
		//フォントに含まれるグリフの総数
		uint16_t numGlyphs;
		//非複合グリフの最大制御点数
		uint16_t maxPoints;
		//非複合グリフの最大輪郭数
		uint16_t maxContours;
		//複合グリフの最大制御点数
		uint16_t maxCompositePoints;
		//複合グリフの最大輪郭数
		uint16_t maxCompositeContours;
		//ゾーンの最大数。1もしくは2を指定する。
		//2の場合、通常のゾーン(Z1)に加えてグリフ上に存在しない点
		//を格納するゾーン(Z0)を使用する
		uint16_t maxZones;
		//Z0に含まれる点の最大数
		uint16_t maxTwilightPoints;
		//ストレージエリアの場所の数
		uint16_t maxStorage;
		//最大関数番号+1の数
		uint16_t maxFunctionDefs;
		//IDEFsの数
		uint16_t maxInstructionDefs;
		//fpgm, prepのプログラムとglyfの命令の最大スタック数
		uint16_t maxStackElements;
		//グリフ命令の最大バイト数
		uint16_t maxSizeOfInstructions;
		//複合グリフのトップレベルで参照される最大コンポーネント数
		uint16_t maxComponentElements;
		//コンポーネントの最大再帰回数。非複合グリフは1
		uint16_t maxComponentDepth;
	public:
		bool Load(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			file.ReadBE(version);
			file.ReadBE(numGlyphs);
			if (version == 0x00005000) return true;
			file.ReadBE(maxPoints);
			file.ReadBE(maxContours);
			file.ReadBE(maxCompositePoints);
			file.ReadBE(maxCompositeContours);
			file.ReadBE(maxZones);
			file.ReadBE(maxTwilightPoints);
			file.ReadBE(maxStorage);
			file.ReadBE(maxFunctionDefs);
			file.ReadBE(maxInstructionDefs);
			file.ReadBE(maxStackElements);
			file.ReadBE(maxSizeOfInstructions);
			file.ReadBE(maxComponentElements);
			file.ReadBE(maxComponentDepth);
			return static_cast<bool>(file);
		}
	};
}

//struct in HMTX
namespace System {
	/// <summary>
	/// グリフの前進幅と左サイドベアリングの値を
	/// 表す構造体
	/// </summary>
	struct LongHorMetric {
		//前進幅
		uint16_t advanceWidth;
		//左サイドベアリング
		int16_t lsb;
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(advanceWidth);
			file.ReadBE(lsb);
			return static_cast<bool>(file);
		}
	};
}

//HMTX
export namespace System {
	/// <summary>
	/// hmtxテーブルを表す構造体。
	/// グリフの前進幅と左サイドベアリングを含んでいる。
	/// グリフアウトラインデータとともに使用される
	/// </summary>
	struct HMTX {
		//グリフIDでアクセス可能なグリフの前進幅と左サイドベアリングを表す配列。
		//要素数は[HHEA::numberOfHMetrics]
		LongHorMetric* hMetrics = nullptr;
		//HHEA::numberOfMetrics以上のグリフIDを持つグリフの左サイドベアリング値の配列
		//要素数は[MAXP::numGlyphs - HHEA::numberOfHMetrics]
		int16_t* leftSideBearings = nullptr;
	private:
		//HHEA::numberOfHMetricsを保存
		uint16_t m_numberOfHMetrics;
		//MAXP::numGlyphs - HHEA::numberOfHMetricsを保存
		uint16_t m_leftSideBearingCount;
	public:
		~HMTX() {
			delete[] hMetrics;
			hMetrics = nullptr;
			delete[] leftSideBearings;
			leftSideBearings = nullptr;
			m_numberOfHMetrics = 0;
			m_leftSideBearingCount = 0;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t offset, const HHEA& hhea, const MAXP& maxp) noexcept {
			m_numberOfHMetrics = hhea.numberOfHMetrics;
			m_leftSideBearingCount = maxp.numGlyphs - hhea.numberOfHMetrics;
			file.Seek(offset, System::IO::SeekDir::Begin);
			hMetrics = new LongHorMetric[m_numberOfHMetrics];
			for (uint16_t i = 0; i < m_numberOfHMetrics; ++i) hMetrics[i].Load(file);
			leftSideBearings = new int16_t[m_leftSideBearingCount];
			file.ReadBE(leftSideBearings, m_leftSideBearingCount);
			return static_cast<bool>(file);
		}
	public:
		/// <summary>
		/// グリフIDに対応した前進幅を取得する
		/// </summary>
		constexpr uint16_t GetAdvanceWidth(uint16_t gid) const noexcept {
			return gid < m_numberOfHMetrics ? hMetrics[gid].advanceWidth : 0;
		}
		/// <summary>
		/// グリフIDに対応した左サイドベアリングを取得する
		/// </summary>
		constexpr int16_t GetLeftSideBearing(uint16_t gid) const noexcept {
			if (gid < m_numberOfHMetrics) return hMetrics[gid].lsb;
			else {
				gid -= m_numberOfHMetrics;
				return gid < m_leftSideBearingCount ? leftSideBearings[gid] : 0;
			}
		}
	};
}

//LOCA
export namespace System {
	struct LOCA {
		//グリフIDでアクセスできるグリフ情報を含むテーブルの先頭からのオフセット。
		//HEAD::IndexToLocFormatが0の場合、uint16_tの配列になる。
		//最後の要素は最後のグリフ情報の次のバイトを指し、
		//最後のグリフ情報のバイト長を求めるために使用できる。
		//要素数は[maxp.numGlyphs + 1]
		uint32_t* offsets = nullptr;
	private:
		//offsetsの要素数。maxp.numGlyphs + 1を保存する
		uint32_t m_offsetCount;
	public:
		~LOCA() {
			delete[] offsets;
			offsets = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t offset, const HEAD& head, const MAXP& maxp) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			m_offsetCount = maxp.numGlyphs + 1;
			offsets = new uint32_t[m_offsetCount];
			if (head.indexToLocFormat == 0) {
				uint16_t* tmp = new uint16_t[m_offsetCount];
				file.ReadBE(tmp, m_offsetCount);
				for (uint32_t i = 0; i < m_offsetCount; ++i) offsets[i] = static_cast<uint32_t>(tmp[i] * 2);
				delete tmp;
			}
			else file.ReadBE(offsets, m_offsetCount);
			return static_cast<bool>(file);
		}
	public:
		/// <summary>
		/// グリフIDに対応するグリフ情報を含むテーブルの先頭からのオフセットを取得する。
		/// 存在しないグリフIDの場合、0xffffffffuを返す
		/// </summary>
		constexpr uint32_t GetOffset(uint16_t gid) const noexcept {
			return gid < m_offsetCount ? offsets[gid] : System::MAX_VALUE<uint32_t>;
		}
	};
}

namespace System {
	void PixelInverse(System::Drawing::Pixel* line0, uint32_t x, uint32_t xEnd) noexcept {
		for (uint32_t tx = x + 1; tx <= xEnd; ++tx) {
			if (line0[tx].a != 0xaau) line0[tx].a = ~line0[tx].a;
		}
	}
}

//struct in GLYF
namespace System {
	struct Glyph;

	struct SimpleGlyph {
		//各輪郭の最後の制御点のインデックス。
		//最後の要素に1を加えた数がこのグリフに含まれる制御点の総数となる。
		//要素数は[Glyph::numberOfContours]
		uint16_t* endPtsOfContours = nullptr;
		//instructionsのバイト長
		uint16_t instructionLength;
		//命令の配列。
		//要素数は[instructionLength]
		uint8_t* instructions = nullptr;
		//列挙型SimpleGlyphFlagsの和の配列。
		//ファイル上では、SimpleGlyphFlags::REPEAT_FLAGによって
		//連続する同じフラグが省略されるため、読み込むまで長さは不明。
		//読み込み時に展開するため、要素数は[endPtsOfContours[Glyph::numberOFContours - 1]]
		uint8_t* flags = nullptr;
		//ファイル上では、前の座標(始点の場合(0,0))からの相対座標がuint8_tかint16_tで保存されており、
		//型と配列サイズはflagsから計算される
		//格納方法：(xMin, yMin)を左下隅の原点(0, 0)とする絶対座標に変換して格納
		//[endPtsOfCountours[Glyph::numberOfContours - 1]]
		int16_t* xCoordinates = nullptr;
		//xCoodinates参照
		int16_t* yCoordinates = nullptr;
	private:
		//個数を保存
		uint16_t pointCount;
		uint16_t xCount;
		uint16_t yCount;
	public:
		~SimpleGlyph() {
			delete[] endPtsOfContours;
			endPtsOfContours = nullptr;
			delete[] instructions;
			instructions = nullptr;
			delete[] flags;
			flags = nullptr;
			delete[] xCoordinates;
			xCoordinates = nullptr;
			delete[] yCoordinates;
			yCoordinates = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, const Glyph& glyph) noexcept;
	public:
		System::Drawing::Image GetGlyphImage(const Glyph& glyph, float scale, const System::Drawing::Pixel& contoursPixel) const noexcept;
	};

	struct CompositeComponent {
		//列挙型CompositeGlyphFlagsの和
		uint16_t flags;
		//参照するグリフID
		uint16_t glyphIndex;
		//グリフに適用するx座標のオフセット値
		union {
			uint8_t u8;
			int8_t s8;
			uint16_t u16;
			int16_t s16;
		} argument1;
		//グリフに適用するy座標のオフセット値
		union {
			uint8_t u8;
			int8_t s8;
			uint16_t u16;
			int16_t s16;
		} argument2;
		//グリフに適用するスケール値。
		//1つの場合、単純なスケール値を表す。
		//2つの場合、それぞれX軸、Y軸のスケール値を表す。
		//4つの場合、2*2の変換行列(?)によるスケーリングを表す。
		f2dot14_c* transformation = nullptr;
	private:
		//transformationの要素数を保存する
		uint8_t m_transformationCount;
	public:
		CompositeComponent() noexcept = default;
		CompositeComponent(const CompositeComponent& component) noexcept
			: flags(component.flags), glyphIndex(component.glyphIndex),
			argument1(component.argument1), argument2(component.argument2)
		{
			if (flags & CompositeGlyphFlags::WE_HAVE_A_SCALE) {
				transformation = new f2dot14_c;
				*transformation = *component.transformation;
			}
			else if (flags & CompositeGlyphFlags::WE_HAVE_AN_X_AND_Y_SCALE) {
				transformation = new f2dot14_c[2];
				transformation[0] = component.transformation[0];
				transformation[1] = component.transformation[1];
			}
			else if (flags & CompositeGlyphFlags::WE_HAVE_A_TWO_BY_TWO) {
				transformation = new f2dot14_c[4];
				transformation[0] = component.transformation[0];
				transformation[1] = component.transformation[1];
				transformation[2] = component.transformation[2];
				transformation[3] = component.transformation[3];
			}
			else transformation = nullptr;
		}
		CompositeComponent(CompositeComponent&& component) noexcept
			: flags(component.flags), glyphIndex(component.glyphIndex),
			argument1(component.argument1), argument2(component.argument2),
			transformation(component.transformation)
		{
			component.transformation = nullptr;
			component.m_transformationCount = 0;
		}
		~CompositeComponent() {
			delete[] transformation;
			transformation = nullptr;
			m_transformationCount = 0;
		}
	public:
		bool Load(System::IO::FileStream& file) noexcept {
			file.ReadBE(flags);
			file.ReadBE(glyphIndex);
			if (flags & CompositeGlyphFlags::ARG_1_AND_2_ARE_WORDS) {
				if (flags & CompositeGlyphFlags::ARGS_ARE_XY_VALUES) {
					file.ReadBE(argument1.s16);
					file.ReadBE(argument2.s16);
				}
				else {
					file.ReadBE(argument1.u16);
					file.ReadBE(argument2.u16);
				}
			}
			else {
				if (flags & CompositeGlyphFlags::ARGS_ARE_XY_VALUES) {
					file.ReadBE(argument1.s8);
					file.ReadBE(argument2.s8);
				}
				else {
					file.ReadBE(argument1.u8);
					file.ReadBE(argument2.u8);
				}
			}
			if (flags & CompositeGlyphFlags::WE_HAVE_A_SCALE) m_transformationCount = 1;
			else if (flags & CompositeGlyphFlags::WE_HAVE_AN_X_AND_Y_SCALE) m_transformationCount = 2;
			else if (flags & CompositeGlyphFlags::WE_HAVE_A_TWO_BY_TWO) m_transformationCount = 4;
			else m_transformationCount = 0;
			if (m_transformationCount) {
				transformation = new f2dot14_c[m_transformationCount];
				file.ReadBE(transformation, m_transformationCount);
			}
			else transformation = nullptr;
			return static_cast<bool>(file);
		}
	public:
		CompositeComponent& operator=(const CompositeComponent& rhs) noexcept {
			if (this == &rhs) return *this;
			flags = rhs.flags;
			glyphIndex = rhs.glyphIndex;
			argument1 = rhs.argument1;
			argument2 = rhs.argument2;
			m_transformationCount = rhs.m_transformationCount;
			if (flags & CompositeGlyphFlags::WE_HAVE_A_SCALE) {
				transformation = new f2dot14_c;
				*transformation = *rhs.transformation;
			}
			else if (flags & CompositeGlyphFlags::WE_HAVE_AN_X_AND_Y_SCALE) {
				transformation = new f2dot14_c[2];
				transformation[0] = rhs.transformation[0];
				transformation[1] = rhs.transformation[1];
			}
			else if (flags & CompositeGlyphFlags::WE_HAVE_A_TWO_BY_TWO) {
				transformation = new f2dot14_c[4];
				transformation[0] = rhs.transformation[0];
				transformation[1] = rhs.transformation[1];
				transformation[2] = rhs.transformation[2];
				transformation[3] = rhs.transformation[3];
			}
			else transformation = nullptr;
			return *this;
		}
		CompositeComponent& operator=(CompositeComponent&& rhs) noexcept {
			if (this == &rhs) return *this;
			flags = rhs.flags;
			glyphIndex = rhs.glyphIndex;
			argument1 = rhs.argument1;
			argument2 = rhs.argument2;
			transformation = rhs.transformation;
			m_transformationCount = rhs.m_transformationCount;
			rhs.transformation = nullptr;
			rhs.m_transformationCount = 0;
			return *this;
		}
	};

	struct CompositeGlyph {
		//配列サイズはCompositeGlyphFlags::MORE_COMPONENTSから計算する
		CompositeComponent* components = nullptr;
		//instructionsのバイト長
		uint16_t instructionLength;
		//命令の配列。
		//要素数は[instructionLength]
		uint8_t* instructions = nullptr;
	private:
		//componentsの要素数を保存する
		uint16_t m_componentCount;
	public:
		~CompositeGlyph() {
			delete[] components;
			components = nullptr;
			delete[] instructions;
			instructions = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, const MAXP& maxp) noexcept {
			//コンポーネントの領域を確保(最大数はMAXP::maxComponentElements)
			CompositeComponent* tmps = new CompositeComponent[maxp.maxComponentElements];
			m_componentCount = 0;
			//CompositeComponent::flagsにMORE_COMPONENTが含まれる間、読み込み続ける
			for (; m_componentCount < maxp.maxComponentElements; ++m_componentCount) {
				tmps[m_componentCount].Load(file);
				if (!(tmps[m_componentCount].flags & CompositeGlyphFlags::MORE_COMPONENTS)) break;
			}
			++m_componentCount;	//最後のインデックスを指しているのでインクリメント
			components = new CompositeComponent[m_componentCount];
			for (uint16_t i = 0; i < m_componentCount; ++i) components[i] = static_cast<CompositeComponent&&>(tmps[i]);
			delete[] tmps;
			if (components[m_componentCount - 1].flags & CompositeGlyphFlags::WE_HAVE_INSTRUCTIONS) {
				file.ReadBE(instructionLength);
				instructions = new uint8_t[instructionLength];
				file.ReadBE(instructions, instructionLength);
			}
			else {
				instructionLength = 0;
				instructions = nullptr;
			}
			return static_cast<bool>(file);
		}
	public:
		System::Drawing::Image GetGlyphImage(const Glyph& glyph, float scale, const System::Drawing::Pixel& contoursPixel) noexcept;
	};

	struct Glyph {
		//グリフの持つ輪郭数。
		//負数(-1が多い)の場合、複合グリフを表す
		int16_t numberOfContours;
		//グリフの座標データの最小値・最大値
		//グリフは左下隅(xMin, yMin)、右上隅(xMax, yMax)の長方形に内接する
		int16_t xMin;
		int16_t yMin;
		int16_t xMax;
		int16_t yMax;
		union {
			SimpleGlyph* simple = nullptr;
			CompositeGlyph* composite;
		};
	public:
		~Glyph() {
			if (numberOfContours >= 0) delete simple;
			else delete composite;
			simple = nullptr;
		}
	public:
		void SetEmptyGlyph(const HMTX& hmtx, uint16_t index) noexcept {
			numberOfContours = 0;
			xMin = 0;
			yMin = 0;
			xMax = static_cast<int16_t>(hmtx.GetAdvanceWidth(index));
			yMax = 1;
		}
		bool Load(System::IO::FileStream& file, uint32_t tableOffset, uint32_t glyphOffset, const MAXP& maxp) noexcept {
			file.Seek(tableOffset, System::IO::SeekDir::Begin);
			file.Seek(glyphOffset, System::IO::SeekDir::Current);
			file.ReadBE(numberOfContours);
			file.ReadBE(xMin);
			file.ReadBE(yMin);
			file.ReadBE(xMax);
			file.ReadBE(yMax);
			if (numberOfContours > 0) {
				simple = new SimpleGlyph;
				return simple->Load(file, *this);
			}
			else if (numberOfContours < 0) {
				composite = new CompositeGlyph;
				return composite->Load(file, maxp);
			}
			return static_cast<bool>(file);
		}
	public:
		System::Drawing::Image GetGlyphImage(float scale, const System::Drawing::Pixel& contoursPixel) const noexcept {
			if (!simple) return System::Drawing::Image();
			if (numberOfContours > 0) return simple->GetGlyphImage(*this, scale, contoursPixel);
			else return composite->GetGlyphImage(*this, scale, contoursPixel);
		}
		int16_t GetXMin() const noexcept { return xMin; }
		int16_t GetYMax() const noexcept { return yMax; }
	};

	bool SimpleGlyph::Load(System::IO::FileStream& file, const Glyph& glyph) noexcept {
		endPtsOfContours = new uint16_t[glyph.numberOfContours];
		file.ReadBE(endPtsOfContours, glyph.numberOfContours);
		file.ReadBE(instructionLength);
		instructions = new uint8_t[instructionLength];
		file.ReadBE(instructions, instructionLength);
		//制御点数
		const uint32_t pointCount = endPtsOfContours[glyph.numberOfContours - 1] + 1;
		const uint16_t endPtsOfGlyph = endPtsOfContours[glyph.numberOfContours - 1];
		//flagsを読み込み、制御点数と等しい要素数を持つ配列に展開する
		uint16_t point = 0;
		flags = new uint8_t[pointCount];
		while (point < pointCount) {
			file.ReadBE(flags[point]);
			if (flags[point] & SimpleGlyphFlags::REPEAT_FLAG) {
				flags[point] &= ~SimpleGlyphFlags::REPEAT_FLAG;
				uint8_t flag = flags[point];
				uint8_t repeatCount;
				file.ReadBE(repeatCount);
				for (uint8_t j = 0; j < repeatCount; ++j) flags[++point] = flag;
			}
			++point;
		}
		//x座標を読み込み、制御点数と等しい要素数を持つ配列に展開する
		xCoordinates = new int16_t[pointCount];
		int16_t currentX = -glyph.xMin;	//x座標の最小値を基準値(原点)とする
		for (point = 0; point < pointCount; ++point) {
			if (flags[point] & SimpleGlyphFlags::X_SHORT_VECTOR) {
				uint8_t x8;
				file.ReadBE(x8);
				if (flags[point] & SimpleGlyphFlags::X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
					currentX += static_cast<int16_t>(x8);
				}
				else {
					currentX += -static_cast<int16_t>(x8);
				}
			}
			else {
				if (~flags[point] & SimpleGlyphFlags::X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
					int16_t x16;
					file.ReadBE(x16);
					currentX += x16;
				}
			}
			xCoordinates[point] = currentX;
		}
		//y座標を読み込み、制御点数と等しい要素数を持つ配列に展開する
		yCoordinates = new int16_t[pointCount];
		int16_t currentY = -glyph.yMin;	//y座標の最小値を基準値(原点)とする
		for (point = 0; point < pointCount; ++point) {
			if (flags[point] & SimpleGlyphFlags::Y_SHORT_VECTOR) {
				uint8_t y8;
				file.ReadBE(y8);
				if (flags[point] & SimpleGlyphFlags::Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
					currentY += static_cast<int16_t>(y8);
				}
				else {
					currentY += -static_cast<int16_t>(y8);
				}
			}
			else {
				if (~flags[point] & SimpleGlyphFlags::Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
					int16_t y16;
					file.ReadBE(y16);
					currentY += y16;
				}
			}
			yCoordinates[point] = currentY;
		}
		return static_cast<bool>(file);
	}

	System::Drawing::Image SimpleGlyph::GetGlyphImage(const Glyph& glyph, float scale, const System::Drawing::Pixel& contoursPixel) const noexcept {
		System::Drawing::Image ret{
			static_cast<uint32_t>((glyph.xMax - glyph.xMin) * scale) + 1,
			static_cast<uint32_t>((glyph.yMax - glyph.yMin) * scale) + 1
		};
		System::Drawing::Pixel* pixels = ret.Data();
		const uint32_t xMax = ret.Width() - 1;
		const uint32_t yMax = ret.Height() - 1;
		uint16_t xIndex = 0;
		uint16_t yIndex = 0;
		uint8_t* currentFlag = flags;
		//輪郭ごとに処理
		for (uint16_t i = 0; i < glyph.numberOfContours; ++i) {
			//輪郭を構成する制御点数
			const uint16_t numPoints = i == 0 ? endPtsOfContours[i] + 1 : endPtsOfContours[i] - endPtsOfContours[i - 1];
			ControlPoint* points = new ControlPoint[numPoints];
			for (uint16_t j = 0; j < numPoints; ++j) {
				points[j].OnCurve = *(currentFlag++) & SimpleGlyphFlags::ON_CURVE_POINT;
				points[j].x = static_cast<int16_t>(xCoordinates[xIndex++] * scale);
				points[j].y = static_cast<int16_t>(yCoordinates[yIndex++] * scale);
			}
			//制御点から2次Bスプライン曲線を生成
			System::BSpline bspline;
			bspline.SetFunction(points, numPoints, 2, System::Math::Max(xMax, yMax), true);
			delete[] points;
			//コルーチンによって連続した曲線上の点を取得して描画する。
			//コルーチンは最初の点と最後の点は同じ座標を返す。
			//ピクセルのアルファ値によって以下のように状態を管理する。
			//0x00: 黒塗り, 0xaa: 曲線上, 0xff: 白塗り
			bool first = true;	//輪郭の最初の点の処理時のみtrue
			Point<int32_t> prev;	//直前の点座標
			int8_t yFirstSign = 0;	//輪郭内で初めてy座標が変化した方向(1 or -1)
			int8_t yPrevSign = 0;	//直前のy座標が変化した方向(1 or -1)
			for (Point<int32_t>& p : bspline.Splines(0, 0, xMax, yMax)) {
				//グリフ座標は左下原点、Image座標は左上原点なので上下反転
				uint32_t y = yMax - p.y;
				//座標(0, y)のピクセルへのポインタ
				System::Drawing::Pixel* line0 = pixels + (y * (xMax + 1));
				ret.SetPixel(p.x, y, contoursPixel);
				//現在の点の水平右方向のピクセルのアルファ値を反転する(0x00 <-> 0xff)
				if (first) {
					first = false;
					PixelInverse(line0, p.x, xMax);
				}
				else {
					int8_t ySign = static_cast<int8_t>(System::Math::Sign(static_cast<int64_t>(y) - prev.y));
					if (ySign) {
						PixelInverse(line0, p.x, xMax);
						//y座標の変位方向が反転したとき、直前の点でのアルファ値反転を取り消す
						if (yPrevSign && ySign != yPrevSign) {
							line0 += static_cast<int64_t>(yPrevSign) * (xMax + 1);
							PixelInverse(line0, prev.x, xMax);
						}
						yPrevSign = ySign;
						if (!yFirstSign) yFirstSign = ySign;
					}
				}
				prev.x = p.x;
				prev.y = y;
			}
			//輪郭の最後の点(最初の点)が上底もしくは下底のいずれでもない場合、
			//最後のアルファ値反転を取り消す
			if (yFirstSign && yFirstSign == yPrevSign) {
				System::Drawing::Pixel* line0 = pixels + (prev.y * (xMax + 1));
				PixelInverse(line0, prev.x, xMax);
			}
		}
		//アルファ値0x00のピクセルを黒色(0x000000ff)に塗りつぶす
		for (uint32_t y = 0; y <= yMax; ++y) {
			System::Drawing::Pixel* line0 = pixels + (xMax + 1ull) * y;
			for (uint32_t x = 0; x <= xMax; ++x) {
				if (line0[x].a == 0x00u) {
					line0[x].r = 0x00u;
					line0[x].g = 0x00u;
					line0[x].b = 0x00u;
					line0[x].a = 0xffu;
				}
				else if (line0[x].a == 0xaau) line0[x].a = 0xffu;
			}
		}
		return ret;
	}

	System::Drawing::Image CompositeGlyph::GetGlyphImage(const Glyph& glyph, float scale, const System::Drawing::Pixel& contoursPixel) noexcept {
		System::Drawing::Image ret{
			static_cast<uint32_t>((glyph.xMax - glyph.xMin) * scale) + 1,
			static_cast<uint32_t>((glyph.yMax - glyph.yMin) * scale) + 1
		};
		return ret;
	}
}

//GLYF
export namespace System {
	struct GLYF {
		//グリフIDでアクセスできるグリフ情報の配列。
		//要素数は[maxp.numGlyphs]
		Glyph* glyphs = nullptr;
	private:
		//MAXP::numGlyphsの値を保存する
		uint16_t m_numGlyphs;
	public:
		~GLYF() {
			delete[] glyphs;
			glyphs = nullptr;
			m_numGlyphs = 0;
		}
	public:
		//GLYFテーブルへのオフセットを指定してファイルを読み取り、構造体を初期化する
		//numGlyphs：MAXP::numGlyphsを指定する
		//locaOffsets：LOCA::offsetsを指定する(配列の大きさはMAXP::numGlyphs + 1)
		//maxComponentElements：MAXP::maxComponentElementsを指定する
		//hmtx：HMTXへの参照を指定する(AdvanceWidth取得用)
		bool Load(System::IO::FileStream& file, uint32_t offset, const MAXP& maxp, const HMTX& hmtx, const LOCA& loca) noexcept {
			m_numGlyphs = maxp.numGlyphs;
			file.Seek(offset, System::IO::SeekDir::Begin);
			glyphs = new Glyph[m_numGlyphs];
			for (uint16_t i = 0; i < m_numGlyphs; ++i) {
				//loca.offsetsが連続して同じ値をとるとき、グリフが定義されていない
				if (loca.offsets[i] == loca.offsets[i + 1]) glyphs[i].SetEmptyGlyph(hmtx, i);
				else glyphs[i].Load(file, offset, loca.offsets[i], maxp);
			}
			return static_cast<bool>(file);
		}
	public:
		System::Drawing::Image GetGlyphImageByGID(uint16_t gid, float scale, const System::Drawing::Pixel& contoursPixel) const noexcept {
			if (gid < m_numGlyphs) return glyphs[gid].GetGlyphImage(scale, contoursPixel);
			else return System::Drawing::Image();
		}
		int16_t GetXMin(uint16_t gid) const noexcept {
			return gid < m_numGlyphs ? glyphs[gid].GetXMin() : 0;
		}
		int16_t GetYMax(uint16_t gid) const noexcept {
			return gid < m_numGlyphs ? glyphs[gid].GetYMax() : 0;
		}
	};
}

//struct in CFF1
export namespace System {
	/// <summary>
	/// CFF1IndexData::GetDICTValue()関数が返す構造体
	/// keyに対応するT型の値を保持する配列と要素数を含む
	/// </summary>
	/// <typeparam name="T">GetDICTValue()関数に指定した型</typeparam>
	template<typename T>
	struct DICTValue {
		//要素数は[count]
		T* data = nullptr;
		size_t count = 0;
	public:
		DICTValue() noexcept = default;
		DICTValue(const DICTValue&) noexcept = delete;
		DICTValue(DICTValue&& value) noexcept : data(value.data), count(value.count) {
			value.data = nullptr;
			value.count = 0;
		}
		~DICTValue() {
			delete[] data;
			data = nullptr;
			count = 0;
		}
	public:
		DICTValue& operator=(const DICTValue&) noexcept = delete;
		DICTValue& operator=(DICTValue&& rhs) noexcept {
			if (this == &rhs) return *this;
			delete data;
			data = rhs.data;
			count = rhs.count;
			rhs.data = nullptr;
			rhs.count = 0;
			return *this;
		}
	};

	/// <summary>
	/// DICTValue型にDICTOperatorの種類を追加した構造体
	/// </summary>
	/// <typeparam name="T">内包するデータ配列の型</typeparam>
	template<typename T>
	struct DICTKeyValue : public DICTValue<T> {
		DICTOperator opr = DICTOperator::Undefined;
	public:
		DICTKeyValue() noexcept = default;
		DICTKeyValue(DICTOperator opr, DICTValue<T>&& arg) noexcept : DICTValue<T>(static_cast<DICTValue<T>&&>(arg)), opr(opr) {}
	public:
		DICTKeyValue& operator=(DICTKeyValue<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			opr = rhs.opr;
			DICTValue<T>::operator=(static_cast<DICTValue<T>&&>(rhs));
			return *this;
		}
	};

	/// <summary>
	/// CFF1Index::GetData()関数が返す構造体
	/// 指定した要素の先頭を指すポインタとバイト数を含む
	/// </summary>
	struct CFF1IndexData {
		//CFF1Index構造体内のある要素の先頭を指すポインタ
		const uint8_t* data = nullptr;
		//dataが指す要素のバイト長
		const uint32_t size = 0;
	public:
		CFF1IndexData() noexcept = delete;
		CFF1IndexData(const uint8_t* d, uint32_t s) noexcept : data(d), size(s) {}
		CFF1IndexData(const CFF1IndexData& arg) noexcept : data(arg.data), size(arg.size) {}
	public:
		CFF1IndexData& operator=(const CFF1IndexData&) noexcept = delete;
	private:
		/// <summary>
		/// DICT形式配列の指定したオフセット位置からNumber型を読み取り、指定した参照変数に代入する
		/// </summary>
		/// <typeparam name="T">参照変数の型</typeparam>
		/// <param name="variant">読み取ったNumber型を代入する参照変数</param>
		/// <param name="data">DICT形式配列へのポインタ</param>
		/// <param name="start">DICT形式配列のオフセット値</param>
		/// <param name="opr"></param>
		/// <returns>
		/// 次のオフセット値。
		/// data[opr]の演算子に関わるすべてのデータを読み取ったとき、指定したoprと一致する。
		/// Number型を読み取れなかった場合、0xffffffffuを返す
		/// </returns>
		template<typename T>
		static uint32_t LoadNumber(T& variant, const uint8_t* data, uint32_t start, uint32_t opr) noexcept {
			const uint8_t* b = &data[start];
			if (b[0] == 28) {
				variant = static_cast<T>((static_cast<int16_t>(b[1]) << 8) | b[2]);
				return start + 3;
			}
			else if (b[0] == 29) {
				variant = static_cast<T>((static_cast<int32_t>(b[1]) << 24) | (static_cast<int32_t>(b[2]) << 16) | (static_cast<int32_t>(b[3]) << 8) | static_cast<int32_t>(b[4]));
				return start + 5;
			}
			else if (b[0] == 30) {
				uint32_t size = opr - start;
				uint32_t integerPart = 0;
				uint32_t decimalPart = 0;
				uint32_t exponentPart = 0;
				int8_t integerSign = 1;
				int8_t exponentSign = 1;
				uint32_t* calc = &integerPart;
				uint32_t i = 1;
				for (; i < size; ++i) {
					uint8_t nibble = (b[i] & 0xf0) >> 4;
					if (nibble <= 9) {
						*calc *= 10;
						*calc += nibble;
					}
					else if (nibble == 0x0a) calc = &decimalPart;
					else if (nibble == 0x0b) calc = &exponentPart;
					else if (nibble == 0x0c) {
						calc = &exponentPart;
						exponentSign = -1;
					}
					else if (nibble == 0x0e) integerSign = -1;
					else if (nibble == 0x0f) break;
					nibble = b[i] & 0x0f;
					if (nibble <= 9) {
						*calc *= 10;
						*calc += nibble;
					}
					else if (nibble == 0x0a) calc = &decimalPart;
					else if (nibble == 0x0b) calc = &exponentPart;
					else if (nibble == 0x0c) {
						calc = &exponentPart;
						exponentSign = -1;
					}
					else if (nibble == 0x0e) integerSign = -1;
					else if (nibble == 0x0f) break;
				}
				double decimal = static_cast<double>(decimalPart);
				while (decimal >= 1.0) decimal /= 10.0;
				double e = 1.0;
				if (exponentSign == 1) for (uint32_t j = 0; j < exponentPart; ++j) e *= 10;
				else for (uint32_t j = 0; j < exponentPart; ++j) e *= 0.1;
				variant = static_cast<T>((integerSign * static_cast<double>(integerPart) + decimal) * e);
				return start + i + 1;
			}
			else if (b[0] <= 246) {
				variant = static_cast<T>(static_cast<int16_t>(b[0]) - 139);
				return start + 1;
			}
			else if (b[0] <= 250) {
				variant = static_cast<T>((b[0] - 247) * 256 + b[1] + 108);
				return start + 2;
			}
			else if (b[0] <= 254) {
				variant = static_cast<T>(-(b[0] - 251) * 256 - b[1] - 108);
				return start + 2;
			}
			return 0xffffffffu;
		}
		template<typename T>
		IEnumerator<DICTKeyValue<T>> GetDICTKeyValueEnumerator() const noexcept {
			uint32_t s = 0;
			for (uint32_t i = 0; i < size; ++i) {
				if (data[i] >= 22) {
					double tmp;
					i = LoadNumber(tmp, data, i, size - 1) - 1;
				}
				else {
					uint16_t tmpKey = 0;
					if (data[i] != 12) tmpKey = data[i];
					else tmpKey = static_cast<uint16_t>((data[i] << 8) | data[i + 1]);
					DICTOperator currentKey = static_cast<DICTOperator>(tmpKey);
					DICTKeyValue<T> ret;
					ret.opr = currentKey;
					switch (currentKey) {
						//Valueが一つ
					case DICTOperator::version:
					case DICTOperator::Notice:
					case DICTOperator::Fullname:
					case DICTOperator::FamilyName:
					case DICTOperator::Weight:
					case DICTOperator::isFixedPitch:
					case DICTOperator::ItalicAngle:
					case DICTOperator::UnderlinePosition:
					case DICTOperator::UnderlineThickness:
					case DICTOperator::PaintType:
					case DICTOperator::CharstringType:
					case DICTOperator::UniqueID:
					case DICTOperator::StrokeWidth:
					case DICTOperator::charset:
					case DICTOperator::Encoding:
					case DICTOperator::CharStrings:
					case DICTOperator::SyntheticBase:
					case DICTOperator::PostScript:
					case DICTOperator::BaseFontName:
					case DICTOperator::BaseFontBlend:
					case DICTOperator::CIDFontVersion:
					case DICTOperator::CIDFontRevision:
					case DICTOperator::CIDFontType:
					case DICTOperator::CIDCount:
					case DICTOperator::UIDBase:
					case DICTOperator::FDArray:
					case DICTOperator::FDSelect:
					case DICTOperator::FontName:
					case DICTOperator::BlueScale:
					case DICTOperator::BlueShift:
					case DICTOperator::BlueFuzz:
					case DICTOperator::StdHW:
					case DICTOperator::StdVW:
					case DICTOperator::ForceBold:
					case DICTOperator::LanguageGroup:
					case DICTOperator::ExpansionFactor:
					case DICTOperator::initialRandomSeed:
					case DICTOperator::Subrs:
					case DICTOperator::defaultWidthX:
					case DICTOperator::nominalWidthX:
					{
						ret.count = 1;
						ret.data = new T[1];
						LoadNumber(ret.data[0], data, s, i);
						break;
					}
					//Valueが2つ
					case DICTOperator::Private:
					{
						ret.count = 2;
						ret.data = new T[2];
						uint32_t next = LoadNumber(ret.data[0], data, s, i);
						LoadNumber(ret.data[1], data, next, i);
						break;
					}
					//Valueが3つ
					case DICTOperator::ROS:
					{
						ret.count = 3;
						ret.data = new T[3];
						uint32_t next = LoadNumber(ret.data[0], data, s, i);
						next = LoadNumber(ret.data[1], data, next, i);
						LoadNumber(ret.data[2], data, next, i);
						break;
					}
					//Valueが配列
					case DICTOperator::FontMatrix:
					case DICTOperator::FontBBox:
					case DICTOperator::XUID:
					case DICTOperator::BlueValues:
					case DICTOperator::OtherBlues:
					case DICTOperator::FamilyBlues:
					case DICTOperator::FamilyOtherBlues:
					case DICTOperator::StemSnapH:
					case DICTOperator::StemSnapV:
					{
						uint32_t ts = s;
						while (ts < i) {
							double tmp;
							ts = LoadNumber(tmp, data, ts, i);
							++ret.count;
						}
						ret.data = new T[ret.count];
						uint32_t next = s;
						for (uint32_t j = 0; j < ret.count; ++j) {
							next = LoadNumber(ret.data[j], data, next, i);
						}
						break;
					}
					default: co_return;
					}
					co_yield ret;
					if (data[i] == 12) ++i;
					s = i + 1;
				}
			}
		}
	public:
		template<typename T>
		DICTKeyValue<T> GetDICTKeyValue(uint32_t index) const noexcept {
			for (DICTKeyValue<T>& x : GetDICTKeyValues()) {
				if (index-- > 0) continue;
				return x;
			}
			return DICTKeyValue<T>();
		}
		template<typename T>
		IEnumerable<DICTKeyValue<T>> GetDICTKeyValues() const noexcept {
			return IEnumerable<DICTKeyValue<T>>(new IEnumerator<DICTKeyValue<T>>(GetDICTKeyValueEnumerator<T>(), nullptr));
		}
		/// <summary>
		/// DICTデータから指定したDICTOperatorに対応する値を検索して取得する
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="key">DICTOperator</param>
		/// <returns>
		/// 値の配列を持つDICTValue構造体。
		/// 値が存在しない場合、要素数0のDICTValue構造体を返す
		/// </returns>
		template<typename T>
		DICTValue<T> GetDICTValue(DICTOperator key) const noexcept {
			DICTValue<T> ret;
			uint32_t s = 0;
			for (uint32_t i = 0; i < size; ++i) {
				if (data[i] >= 22) {
					double tmp;
					i = LoadNumber(tmp, data, i, size - 1) - 1;
				}
				else {
					uint16_t tmpKey = 0;
					if (data[i] != 12) tmpKey = data[i];
					else tmpKey = static_cast<uint16_t>((data[i] << 8) | data[i + 1]);
					DICTOperator currentKey = static_cast<DICTOperator>(tmpKey);
					if (key != currentKey) {
						if (data[i] == 12) ++i;
						s = i + 1;
					}
					else {
						switch (currentKey) {
						//Valueが一つ
						case DICTOperator::version:
						case DICTOperator::Notice:
						case DICTOperator::Fullname:
						case DICTOperator::FamilyName:
						case DICTOperator::Weight:
						case DICTOperator::isFixedPitch:
						case DICTOperator::ItalicAngle:
						case DICTOperator::UnderlinePosition:
						case DICTOperator::UnderlineThickness:
						case DICTOperator::PaintType:
						case DICTOperator::CharstringType:
						case DICTOperator::UniqueID:
						case DICTOperator::StrokeWidth:
						case DICTOperator::charset:
						case DICTOperator::Encoding:
						case DICTOperator::CharStrings:
						case DICTOperator::SyntheticBase:
						case DICTOperator::PostScript:
						case DICTOperator::BaseFontName:
						case DICTOperator::BaseFontBlend:
						case DICTOperator::CIDFontVersion:
						case DICTOperator::CIDFontRevision:
						case DICTOperator::CIDFontType:
						case DICTOperator::CIDCount:
						case DICTOperator::UIDBase:
						case DICTOperator::FDArray:
						case DICTOperator::FDSelect:
						case DICTOperator::FontName:
						case DICTOperator::BlueScale:
						case DICTOperator::BlueShift:
						case DICTOperator::BlueFuzz:
						case DICTOperator::StdHW:
						case DICTOperator::StdVW:
						case DICTOperator::ForceBold:
						case DICTOperator::LanguageGroup:
						case DICTOperator::ExpansionFactor:
						case DICTOperator::initialRandomSeed:
						case DICTOperator::Subrs:
						case DICTOperator::defaultWidthX:
						case DICTOperator::nominalWidthX:
						{
							ret.count = 1;
							ret.data = new T[1];
							LoadNumber(ret.data[0], data, s, i);
							return ret;
						}
						//Valueが2つ
						case DICTOperator::Private:
						{
							ret.count = 2;
							ret.data = new T[2];
							uint32_t next = LoadNumber(ret.data[0], data, s, i);
							LoadNumber(ret.data[1], data, next, i);
							return ret;
						}
						//Valueが3つ
						case DICTOperator::ROS:
						{
							ret.count = 3;
							ret.data = new T[3];
							uint32_t next = LoadNumber(ret.data[0], data, s, i);
							next = LoadNumber(ret.data[1], data, next, i);
							LoadNumber(ret.data[2], data, next, i);
							return ret;
						}
						//Valueが配列
						case DICTOperator::FontMatrix:
						case DICTOperator::FontBBox:
						case DICTOperator::XUID:
						case DICTOperator::BlueValues:
						case DICTOperator::OtherBlues:
						case DICTOperator::FamilyBlues:
						case DICTOperator::FamilyOtherBlues:
						case DICTOperator::StemSnapH:
						case DICTOperator::StemSnapV:
						{
							uint32_t ts = s;
							while (ts < i) {
								double tmp;
								ts = LoadNumber(tmp, data, ts, i);
								++ret.count;
							}
							ret.data = new T[ret.count];
							uint32_t next = s;
							for (uint32_t j = 0; j < ret.count; ++j) {
								next = LoadNumber(ret.data[j], data, next, i);
							}
							return ret;
						}
						default: return ret;
						}
					}
				}
			}
			return ret;
		}
	};

	/// <summary>
	/// Index形式を表す構造体
	/// </summary>
	struct CFF1Index {
		//有効なオフセット配列の要素数
		uint16_t count = 0;
		//オフセット配列の一要素のバイト長
		uint8_t offSize = 0;
		//オフセット配列。要素数は[count + 1]。
		//最後の要素から1引いた数がバイト配列のバイト長
		union {
			//offSize == 1
			uint8_t* offset8 = nullptr;
			//offSize == 2
			uint16_t* offset16;
			//offSize == 3
			uint24_c* offset24;
			//offSize == 4
			uint32_t* offset32;
		};
		//オフセット配列の値を使用してアクセスするデータ配列。
		//バイト長は[offsetX[count] - 1]。
		//n = [0, count - 1]番目の要素には、
		//for (i = offsetX[n] - 1; i < offsetX[n + 1] - 1; ++i) data[i];
		//でアクセスする
		uint8_t* data = nullptr;
	public:
		~CFF1Index() {
			if (offSize == 1) delete[] offset8;
			else if (offSize == 2) delete[] offset16;
			else if (offSize == 3) delete[] offset24;
			else if (offSize == 4) delete[] offset32;
			offset8 = nullptr;
			delete[] data;
			data = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset, uint32_t indexOffset) {
			if (indexOffset) file.Seek(cff1Offset + indexOffset, System::IO::SeekDir::Begin);
			file.ReadBE(count);
			if (!count) return static_cast<bool>(file);
			file.ReadBE(offSize);
			uint32_t dataSize = 0;
			if (offSize == 1) {
				offset8 = new uint8_t[count + 1];
				file.ReadBE(offset8, count + 1);
				dataSize = offset8[count] - 1;
			}
			else if (offSize == 2) {
				offset16 = new uint16_t[count + 1];
				file.ReadBE(offset16, count + 1);
				dataSize = offset16[count] - 1;
			}
			else if (offSize == 3) {
				offset24 = new uint24_c[count + 1];
				file.ReadBE(offset24, count + 1);
				dataSize = offset24[count] - 1;
			}
			else if (offSize == 4) {
				offset32 = new uint32_t[count + 1];
				file.ReadBE(offset32, count + 1);
				dataSize = offset32[count] - 1;
			}
			data = new uint8_t[dataSize];
			file.ReadBE(data, dataSize);
			return static_cast<bool>(file);
		}
	public:
		/// <summary>
		/// i番目の要素を表すCFF1IndexDataを取得する
		/// </summary>
		/// <param name="i">要素のインデックス。範囲は[0, count - 1]</param>
		/// <returns>CFF1IndexがDICT形式(TopDICT, FDArray)の場合、CFF1IndexDataはDICTデータの配列を表す</returns>
		CFF1IndexData GetData(uint16_t i) const noexcept {
			if (i >= count) return CFF1IndexData(nullptr, 0);
			if (offSize == 1) return CFF1IndexData(&data[offset8[i] - 1], offset8[i + 1] - offset8[i]);
			else if (offSize == 2) return CFF1IndexData(&data[offset16[i] - 1], offset16[i + 1] - offset16[i]);
			else if (offSize == 3) return CFF1IndexData(&data[offset24[i] - 1], offset24[i + 1] - offset24[i]);
			else if (offSize == 4) return CFF1IndexData(&data[offset32[i] - 1], offset32[i + 1] - offset32[i]);
			return CFF1IndexData(nullptr, 0);
		}
	};

	/// <summary>
	/// サブルーチンのIndex形式を表す構造体
	/// </summary>
	struct CFF1SubrsIndex : public CFF1Index {
		uint8_t m_charstringType = 2;
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset, uint32_t indexOffset, uint8_t charstringType) noexcept {
			m_charstringType = charstringType;
			return CFF1Index::Load(file, cff1Offset, indexOffset);
		}
	public:
		/// <summary>
		/// サブルーチン番号に対応するサブルーチンデータを取得する
		/// </summary>
		/// <param name="subr">Type2Charstring内に出現するサブルーチン番号を指定する</param>
		/// <param name="charstringType">TopDICTから取得できるCharstringTypeを指定する</param>
		CFF1IndexData GetSubr(int16_t subr) const noexcept {
			uint16_t biasedIndex = static_cast<uint16_t>(subr + (m_charstringType == 1 ? 0 : count < 1240 ? 107 : count < 33900 ? 1131 : 32768));
			return CFF1Index::GetData(biasedIndex);
		}
	};

	/// <summary>
	/// StringIndex形式を表す構造体。
	/// SID(StringID):391以降に対応する文字列が格納されている
	/// </summary>
	struct StringIndex : public CFF1Index {
		~StringIndex() = default;
	private:
		using CFF1Index::GetData;
	public:
		using CFF1Index::Load;
	public:
		/// <summary>
		/// SIDに対応した文字列を取得する。
		/// </summary>
		/// <param name="SID">String ID</param>
		/// <returns>SIDに391より小さい値や存在しない値を指定した場合、空文字列を返す</returns>
		String GetString(uint16_t SID) const noexcept {
			int16_t i = static_cast<int32_t>(SID) - 391;
			CFF1IndexData stringData = GetData(i);
			return String(reinterpret_cast<const char*>(stringData.data), stringData.size);
		}
	};

	/// <summary>
	/// PrivateDICT形式を表す構造体
	/// </summary>
	struct PrivateDICT {
		//データを表すバイナリ配列。バイト長は[size]
		uint8_t* data = nullptr;
		//dataのバイト長
		uint32_t size = 0;
		//Local Subrs CFF1Index
		CFF1SubrsIndex subrs;
	public:
		~PrivateDICT() {
			delete[] data;
			data = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t privateDICTSize, uint32_t cff1Offset, uint32_t privateDICTOffset, uint8_t charstringType) noexcept {
			file.Seek(cff1Offset + privateDICTOffset, System::IO::SeekDir::Begin);
			size = privateDICTSize;
			data = new uint8_t[size];
			file.ReadBE(data, size);
			DICTValue<uint32_t> value = GetData().GetDICTValue<uint32_t>(DICTOperator::Subrs);
			if (value.data && !subrs.Load(file, cff1Offset, privateDICTOffset + value.data[0], charstringType)) return false;
			else return static_cast<bool>(file);
		}
	public:
		/// <summary>
		/// PrivateDICTを表すデータを取得する
		/// </summary>
		CFF1IndexData GetData() const noexcept { return CFF1IndexData(data, size); }
	};

	/// <summary>
	/// FDSelect形式を表す構造体
	/// </summary>
	struct FDSelect {
		struct Format3 {
			uint16_t nRanges = 0;
			//[nRanges]
			struct Range3 {
				uint16_t first = 0;
				uint8_t fd = 0;
			} *range3 = nullptr;
			uint16_t sentinel = 0;
		public:
			~Format3() {
				delete[] range3;
				range3 = nullptr;
			}
		};
		uint8_t format = 0xffu;
		union {
			//format == 0
			//[charString.count(numGlyphs)]
			uint8_t* fds = nullptr;
			//format == 3
			Format3* format3;
		};
	public:
		~FDSelect() {
			if (format == 0) delete[] fds;
			else if (format == 3) delete format3;
			fds = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset, uint32_t fdSelectOffset, uint16_t numGlyphs) noexcept {
			file.Seek(cff1Offset + fdSelectOffset, System::IO::SeekDir::Begin);
			file.ReadBE(format);
			if (format == 0) {
				fds = new uint8_t[numGlyphs];
				file.ReadBE(fds, numGlyphs);
			}
			else if (format == 3) {
				format3 = new Format3;
				file.ReadBE(format3->nRanges);
				format3->range3 = new Format3::Range3[format3->nRanges];
				for (uint16_t i = 0; i < format3->nRanges; ++i) {
					file.ReadBE(format3->range3[i].first);
					file.ReadBE(format3->range3[i].fd);
				}
				file.ReadBE(format3->sentinel);
			}
			return static_cast<bool>(file);
		}
	public:
		/// <summary>
		/// GIDに対応するFontDICTのインデックスを取得する
		/// </summary>
		/// <param name="gid">GID</param>
		/// <returns>FDSelectが正常に読み込まれていないとき、0を返す</returns>
		uint8_t GetFDIndex(uint16_t gid) const noexcept {
			if (format == 0) return fds[gid];
			else if (format == 3) {
				for (uint16_t i = 0; i < format3->nRanges; ++i) {
					if (format3->range3[i].first > gid) return format3->range3[i - 1].fd;
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// CFF1テーブルのヘッダーを表す構造体
	/// </summary>
	struct CFF1Header {
		uint8_t majorVersion;
		uint8_t minorVersion;
		uint8_t headerSize;
		uint8_t offSize;
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset) noexcept {
			file.Seek(cff1Offset, System::IO::SeekDir::Begin);
			file.ReadBE(majorVersion);
			file.ReadBE(minorVersion);
			file.ReadBE(headerSize);
			file.ReadBE(offSize);
			return static_cast<bool>(file);
		}
	};

	/// <summary>
	/// CFF1Encoding, CFF1Charsetで使用される構造体
	/// </summary>
	struct CFF1Range1 {
		uint8_t first;
		uint8_t nLeft;
	};

	/// <summary>
	/// CFF1Charsetで使用される構造体
	/// </summary>
	struct CFF1Range2 {
		uint16_t first = 0;
		uint16_t nLeft = 0;
	};

	struct CFF1Encoding {
		uint8_t format = 0;
		union {
			//format == 0
			uint8_t nCodes = 0;
			//format == 1
			uint8_t nRanges;
		};
		union {
			//[nCodes]
			uint8_t* code = nullptr;
			//[nRanges]
			CFF1Range1* range;
		};
	public:
		~CFF1Encoding() {
			if (format == 0) delete[] code;
			else delete[] range;
			code = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset, uint32_t encodingOffset) noexcept {
			file.Seek(cff1Offset + encodingOffset, System::IO::SeekDir::Begin);
			file.ReadBE(format);
			if (format == 0) {
				file.ReadBE(nCodes);
				code = new uint8_t[nCodes];
				file.ReadBE(code, nCodes);
			}
			else if (format == 1) {
				file.ReadBE(nRanges);
				range = new CFF1Range1[nRanges];
				for (uint8_t i = 0; i < nRanges; ++i) {
					file.ReadBE(range[i].first);
					file.ReadBE(range[i].nLeft);
				}
			}
			return static_cast<bool>(file);
		}
	};

	struct CFF1Charset {
		uint8_t format = 0;
		union {
			//format == 0
			//[charstring.count - 1]
			uint16_t* glyph = nullptr;
			//format == 1
			//[i] : (range1[i].nLeft + 1)の合計がcharString.countと一致したときのi
			CFF1Range1* range1;
			//format == 2
			//[i] : range1参照
			CFF1Range2* range2;
		};
	private:
		//glyph, range1, range2の要素数を保持する
		size_t m_size = 0;
	public:
		~CFF1Charset() {
			if (format == 0) delete[] glyph;
			else if (format == 1) delete[] range1;
			else if (format == 2) delete[] range2;
			glyph = nullptr;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t cff1Offset, uint32_t charsetOffset, uint16_t numGlyphs) noexcept {
			file.Seek(cff1Offset + charsetOffset, System::IO::SeekDir::Begin);
			file.ReadBE(format);
			if (format == 0) {
				glyph = new uint16_t[numGlyphs - 1];
				file.ReadBE(glyph, numGlyphs - 1);
				m_size = numGlyphs - 1;
			}
			else if (format == 1) {
				size_t arraySize = 0;
				uint16_t sumGlyph = 0;
				System::IO::StreamPos pos = file.TellPos();
				uint16_t maxGlyph = numGlyphs - 1;
				while (maxGlyph > sumGlyph) {
					uint16_t tFirst;
					uint8_t tNLeft;
					file.ReadBE(tFirst);
					file.ReadBE(tNLeft);
					sumGlyph += tNLeft + 1;
					++arraySize;
				}
				file.Seek(pos);
				range1 = new CFF1Range1[arraySize];
				for (size_t i = 0; i < arraySize; ++i) {
					file.ReadBE(range1[i].first);
					file.ReadBE(range1[i].nLeft);
				}
				m_size = arraySize;
			}
			else if (format == 2) {
				size_t arraySize = 0;
				uint16_t sumGlyph = 0;
				System::IO::StreamPos pos = file.TellPos();
				uint16_t maxGlyph = numGlyphs - 1;
				while (maxGlyph > sumGlyph) {
					uint16_t tFirst;
					uint16_t tNLeft;
					file.ReadBE(tFirst);
					file.ReadBE(tNLeft);
					sumGlyph += tNLeft + 1;
					++arraySize;
				}
				file.Seek(pos);
				range2 = new CFF1Range2[arraySize];
				for (size_t i = 0; i < arraySize; ++i) {
					file.ReadBE(range2[i].first);
					file.ReadBE(range2[i].nLeft);
				}
				m_size = arraySize;
			}
			return static_cast<bool>(file);
		}
	public:
		uint16_t GetGIDFromGID(uint16_t gid) const noexcept {
			switch (format) {
			case 0:
				return gid < m_size ? glyph[gid] : gid;
			case 1: {
				uint16_t count = 0;
				for (size_t i = 0; i < m_size; ++i) {
					if (range1[i].first <= gid) {
						uint16_t delta = gid - range1[i].first;
						if (delta <= range1[i].nLeft) return count + delta + 1;
					}
					else return 0;
					count += range1[i].nLeft + 1;
				}
			}
			case 2: {
				uint16_t count = 0;
				for (size_t i = 0; i < m_size; ++i) {
					if (range2[i].first <= gid) {
						uint16_t delta = gid - range2[i].first;
						if (delta <= range2[i].nLeft) return count + delta + 1;
					}
					else return 0;
					count += range2[i].nLeft + 1;
				}
			}
			default: return gid;
			}
		}
	};
}

//Type2CharstringParser
export namespace System {
	/// <summary>
	/// Type2 Charstringデータを解析しグリフデータを取得するクラス
	/// </summary>
	class Type2CharstringParser {
		const uint8_t* data = nullptr;	//Type2Charstring形式のデータ
		uint32_t size = 0;	//dataのバイト長
		const CFF1SubrsIndex& local;	//LocalSubrsIndexへの参照
		const CFF1SubrsIndex& global;	//GlobalSubrsIndexへの参照
	private:
		int16_t args[48] = {};	//解析時に使用するスタック領域
		uint8_t argCount = 0;	//現在の有効なargsの要素数
	private:
		//normalWidthと実際のグリフ幅の差分値。
		//PrivateDICTから取得できるDefaultWidthとhmtxテーブルのAdvanceWidthが等しい場合、
		//解析前に0を設定する必要がある。
		//設定されていない場合、Type2Charstringの最初の値をこの値として認識する
		Traits::InitializedVariant<int16_t> widthDelta;
		uint16_t normalWidth = 0;	//PrivateDICTから取得するNormalWidth(グリフの基準幅)
	private:
		int16_t* hStem = nullptr;	//水平ステムヒントの配列
		uint8_t hStemCountX2 = 0;	//hStemの要素数
		int16_t* vStem = nullptr;	//垂直ステムヒントの配列
		uint8_t vStemCountX2 = 0;	//vStemの要素数
		uint8_t currentHintMask[12] = {};	//解析中に使用する現在のヒントマスク
		uint8_t currentCntrMask[12] = {};	//解析中に使用する現在のコントロールマスク
	private:
		int16_t xMin = System::MAX_VALUE<int16_t>;	//解析したグリフの最小x座標
		int16_t yMin = System::MAX_VALUE<int16_t>;	//解析したグリフの最小y座標
		int16_t xMax = System::MIN_VALUE<int16_t>;	//解析したグリフの最大x座標
		int16_t yMax = System::MIN_VALUE<int16_t>;	//解析したグリフの最大y座標
		Vector<BSpline*> bsplines;	//解析したグリフの輪郭ごとのBスプライン曲線情報
		Vector<ControlPoint> points;	//解析中に使用する現在の輪郭の制御点を保持する配列
	public:
		Type2CharstringParser() noexcept = delete;
		Type2CharstringParser(const uint8_t* data, uint32_t size, const CFF1SubrsIndex& local, const CFF1SubrsIndex& global) noexcept
			: data(data), size(size), local(local), global(global) {}
		~Type2CharstringParser() noexcept {
			delete hStem;
			hStem = nullptr;
			delete vStem;
			vStem = nullptr;
			for (BSpline* x : bsplines) delete x;
		}
	private:
		//Charstring形式のバイナリデータdata[start]から数値を一つ読み取り、fixed32_c型引数numberに代入する
		//oprはdata[opr]がTypw2Charstring演算子の1バイト目を指すように設定し、start >= oprの場合、numberは0かつ戻り値はstart
		//戻り値：次のデータの始点を指すインデックス値
		//(戻り値がoprと一致する場合、data[opr]の演算子に関わるデータをすべて読み取ったことを示す)
		static uint32_t GetType2Value(fixed32_c& number, const uint8_t* data, uint32_t start, uint32_t opr) noexcept {
			if (start >= opr) {
				number = static_cast<int16_t>(0);
				return start;
			}
			const uint8_t* b = &data[start];
			if (b[0] == 28) {
				number = static_cast<int16_t>((b[1] << 8) | b[2]);
				return start + 3;
			}
			else if (b[0] <= 246) {
				number = static_cast<int16_t>(b[0] - 139);
				return start + 1;
			}
			else if (b[0] <= 250) {
				number = static_cast<int16_t>((b[0] - 247) * 256 + b[1] + 108);
				return start + 2;
			}
			else if (b[0] <= 254) {
				number = static_cast<int16_t>(-static_cast<int16_t>((b[0] - 251) * 256) - b[1] - 108);
				return start + 2;
			}
			else if (b[0] == 255) {
				number.operator=(b);
				return start + 5;
			}
			return start;
		}
	private:
		uint8_t GetMaskSize() const noexcept {
			uint8_t stemCount = (hStemCountX2 / 2) + (vStemCountX2 / 2);
			return stemCount % 8 == 0 ? stemCount / 8 : (stemCount / 8) + 1;
		}
		void SetHStem() noexcept {
			hStemCountX2 = argCount;
			argCount = 0;
			delete hStem;
			hStem = new int16_t[hStemCountX2];
			for (uint8_t i = 0; i < hStemCountX2; ++i) hStem[i] = args[i];
		}
		void SetVStem() noexcept {
			vStemCountX2 = argCount;
			argCount = 0;
			delete vStem;
			vStem = new int16_t[vStemCountX2];
			for (uint8_t i = 0; i < vStemCountX2; ++i) vStem[i] = args[i];
		}
		void SetHintMask(uint32_t& i) noexcept {
			for (uint8_t j = 0, size = GetMaskSize(); j < size; ++j) {
				currentHintMask[j] = data[++i];
			}
		}
		void SetCntrMask(uint32_t& i) noexcept {
			for (uint8_t j = 0, size = GetMaskSize(); j < size; ++j) {
				currentCntrMask[j] = data[++i];
			}
		}
	private:
		void Close() noexcept {
			size_t count = points.Count();
			if (count > 1) {
				BSpline* tmp = new BSpline();
				const uint32_t unit = 1500;
				const ControlPoint* ps = points.Items();
				for (size_t i = 0; i < count; ++i) {
					if (ps[i].x < xMin) xMin = ps[i].x;
					if (ps[i].x > xMax) xMax = ps[i].x;
					if (ps[i].y < yMin) yMin = ps[i].y;
					if (ps[i].y > yMax) yMax = ps[i].y;
				}
				tmp->SetFunction(ps, count, 3, unit, true);
				bsplines.Add(tmp);
			}
			points.Clear();
			points.Reserve(50);
		}
		void ResetArgCount() noexcept { argCount = 0; }
		template<Type2Operator opr>
		void Moveto() noexcept {
			if constexpr (opr == Type2Operator::Rmoveto) {
				if (argCount != 2) return ResetArgCount();
			}
			else {
				if (argCount != 1) return ResetArgCount();
			}
			ControlPoint tmp{ true, 0, 0 };
			if (points.Count() > 1) {
				tmp = points.Last();
				Close();
			}
			else {
				points.Clear();
			}
			if constexpr (opr != Type2Operator::Vmoveto) tmp.x += args[0];
			else tmp.y += args[0];
			if constexpr (opr == Type2Operator::Rmoveto) tmp.y += args[1];
			points.Add(tmp);
			ResetArgCount();
		}
		template<Type2Operator opr>
		void Lineto() noexcept {
			if constexpr (opr == Type2Operator::Rlineto) {
				if (argCount < 2 || argCount % 2) return ResetArgCount();
				ControlPoint tmp = points.Last();
				tmp.OnCurve = true;
				for (uint8_t i = 0; i < argCount; ++i) {
					tmp.x += args[i];
					tmp.y += args[++i];
					points.Add(tmp);
				}
			}
			else {
				if (!argCount) return ResetArgCount();
				ControlPoint tmp = points.Last();
				tmp.OnCurve = true;
				uint8_t i = 0;
				do {
					if constexpr (opr == Type2Operator::Hlineto) tmp.x += args[i];
					else tmp.y += args[i];
					points.Add(tmp);
					if (++i >= argCount) break;
					if constexpr (opr == Type2Operator::Hlineto) tmp.y += args[i];
					else tmp.x += args[i];
					points.Add(tmp);
				} while (++i < argCount);
			}
			ResetArgCount();
		}
		template<bool HStart, bool HEnd>
		void Curveto_Internal(ControlPoint& tmp, uint8_t& i) noexcept {
			tmp.OnCurve = false;
			if constexpr (HStart) tmp.x += args[i];
			else tmp.y += args[i];
			points.Add(tmp);
			tmp.x += args[++i];
			tmp.y += args[++i];
			points.Add(tmp);
			tmp.OnCurve = true;
			if constexpr (HEnd) tmp.x += args[++i];
			else tmp.y += args[++i];
			points.Add(tmp);
		}
		template<Type2Operator opr>
		void Curveto() noexcept {
			const uint8_t remainder = argCount % 4;
			if constexpr (opr == Type2Operator::Rrcurveto) {
				if (argCount < 6 || argCount % 6) return ResetArgCount();
				ControlPoint tmp = points.Last();
				for (uint8_t i = 0; i < argCount; ++i) {
					tmp.OnCurve = false;
					tmp.x += args[i];
					tmp.y += args[++i];
					points.Add(tmp);
					tmp.x += args[++i];
					tmp.y += args[++i];
					points.Add(tmp);
					tmp.OnCurve = true;
					tmp.x += args[++i];
					tmp.y += args[++i];
					points.Add(tmp);
				}
			}
			else {
				if (argCount < 4 || remainder > 1) return ResetArgCount();
				ControlPoint tmp = points.Last();
				constexpr bool HStart = opr == Type2Operator::Hhcurveto || opr == Type2Operator::Hvcurveto;
				constexpr bool HEnd = opr == Type2Operator::Hhcurveto || opr == Type2Operator::Vhcurveto;
				uint8_t i = 0;
				uint8_t end = remainder ? argCount - 1 : argCount;
				if constexpr (HStart == HEnd) {
					if (remainder) {
						tmp.OnCurve = true;
						if constexpr (HStart) tmp.y += args[i++];
						else tmp.x += args[i++];
						end = argCount;
					}
				}
				for (; i < end; ++i) {
					Curveto_Internal<HStart, HEnd>(tmp, i);
					if constexpr (HStart != HEnd) {
						if (++i >= end) break;
						Curveto_Internal<HEnd, HStart>(tmp, i);
					}
				}
				if constexpr (HStart != HEnd) {
					if (remainder) {
						ControlPoint& last = points.Last();
						if (static_cast<bool>(argCount % 8 == 5) == HStart) last.x += args[end];
						else last.y += args[end];
					}
				}
			}
			ResetArgCount();
		}
		template<Type2Operator opr>
		void CurveAndLine() noexcept {
			if constexpr (opr == Type2Operator::Rcurveline) {
				if (argCount % 6 != 2) return ResetArgCount();
				argCount -= 2;
				const uint8_t index = argCount;
				Curveto<Type2Operator::Rrcurveto>();
				argCount = 2;
				args[0] = args[index];
				args[1] = args[index + 1];
				Lineto<Type2Operator::Rlineto>();
			}
			else {
				if (argCount < 6 || argCount % 2) return ResetArgCount();
				argCount -= 6;
				const uint8_t index = argCount;
				Lineto<Type2Operator::Rlineto>();
				argCount = 6;
				args[0] = args[index];
				args[1] = args[index + 1];
				args[2] = args[index + 2];
				args[3] = args[index + 3];
				args[4] = args[index + 4];
				args[5] = args[index + 5];
				Curveto<Type2Operator::Rrcurveto>();
			}
			ResetArgCount();
		}
		template<Type2Operator opr>
		void Flex() noexcept {
			if constexpr (opr == Type2Operator::Hflex) {
				if (argCount != 7) return ResetArgCount();
				args[12] = 50;
				args[11] = 0;
				args[10] = args[6];
				args[9] = -args[2];
				args[8] = args[5];
				args[7] = 0;
				args[6] = args[4];
				args[5] = 0;
				args[4] = args[3];
				args[3] = args[2];
				args[2] = args[1];
				args[1] = 0;
				argCount = 13;
				return Flex<Type2Operator::Flex>();
			}
			else if constexpr (opr == Type2Operator::Hflex1) {
				if (argCount != 9) return ResetArgCount();
				args[12] = 50;
				args[11] = -(args[1] + args[3] + args[7]);
				args[10] = args[8];
				args[9] = -args[7];
				args[8] = args[6];
				args[7] = 0;
				args[6] = args[5];
				args[5] = 0;
				argCount = 13;
				return Flex<Type2Operator::Flex>();
			}
			else if constexpr (opr == Type2Operator::Flex1) {
				if (argCount != 11) return ResetArgCount();
				int16_t dx = args[0] + args[2] + args[4] + args[6] + args[8];
				int16_t dy = args[1] + args[3] + args[5] + args[7] + args[9];
				if (dx < 0) dx *= -1;
				if (dy < 0) dy *= -1;
				args[12] = 50;
				if (dx > dy) args[11] = 0;
				else {
					args[11] = args[10];
					args[10] = 0;
				}
				argCount = 13;
				return Flex<Type2Operator::Flex>();
			}
			else {
				if (argCount != 13) return ResetArgCount();
				ControlPoint tmp = points.Last();
				tmp.OnCurve = false;
				tmp.x += args[0];
				tmp.y += args[1];
				points.Add(tmp);
				tmp.x += args[2];
				tmp.y += args[3];
				points.Add(tmp);
				tmp.OnCurve = true;
				tmp.x += args[4];
				tmp.y += args[5];
				points.Add(tmp);
				tmp.OnCurve = false;
				tmp.x += args[6];
				tmp.y += args[7];
				points.Add(tmp);
				tmp.x += args[8];
				tmp.y += args[9];
				points.Add(tmp);
				tmp.OnCurve = true;
				tmp.x += args[10];
				tmp.y += args[11];
				points.Add(tmp);
				ResetArgCount();
			}
		}
		void CallPathOperator(Type2Operator opr) noexcept {
			switch (opr) {
			case Type2Operator::Return: return;
			case Type2Operator::Rmoveto: return Moveto<Type2Operator::Rmoveto>();
			case Type2Operator::Hmoveto: return Moveto<Type2Operator::Hmoveto>();
			case Type2Operator::Vmoveto: return Moveto<Type2Operator::Vmoveto>();
			case Type2Operator::Rlineto: return Lineto<Type2Operator::Rlineto>();
			case Type2Operator::Hlineto: return Lineto<Type2Operator::Hlineto>();
			case Type2Operator::Vlineto: return Lineto<Type2Operator::Vlineto>();
			case Type2Operator::Rrcurveto: return Curveto<Type2Operator::Rrcurveto>();
			case Type2Operator::Hhcurveto: return Curveto<Type2Operator::Hhcurveto>();
			case Type2Operator::Hvcurveto: return Curveto<Type2Operator::Hvcurveto>();
			case Type2Operator::Vhcurveto: return Curveto<Type2Operator::Vhcurveto>();
			case Type2Operator::Vvcurveto: return Curveto<Type2Operator::Vvcurveto>();
			case Type2Operator::Rcurveline: return CurveAndLine<Type2Operator::Rcurveline>();
			case Type2Operator::Rlinecurve: return CurveAndLine<Type2Operator::Rlinecurve>();
			case Type2Operator::Flex: return Flex<Type2Operator::Flex>();
			case Type2Operator::Hflex: return Flex<Type2Operator::Hflex>();
			case Type2Operator::Hflex1: return Flex<Type2Operator::Hflex1>();
			case Type2Operator::Flex1: return Flex<Type2Operator::Flex1>();
			case Type2Operator::Endchar: {
				Close();
				return ResetArgCount();
			}
			case Type2Operator::Abs: {
				if (argCount < 1) return ResetArgCount();
				else if (args[argCount - 1] < 0) args[argCount - 1] *= -1;
				return;
			}
			default: break;
			}
		}
	public:
		void SetNormalWidth(uint16_t width) noexcept { normalWidth = width; }
		void SetWidthDelta(int16_t delta) noexcept { widthDelta = delta; }
	public:
		uint32_t GetAdvanceWidth() const noexcept {
			return static_cast<uint32_t>(normalWidth + (widthDelta ? *widthDelta : 0));
		}
		int16_t GetXMin() const noexcept { return xMin; }
		int16_t GetYMin() const noexcept { return yMin; }
		int16_t GetXMax() const noexcept { return xMax; }
		int16_t GetYMax() const noexcept { return yMax; }
		uint32_t GetGlyphWidth() const noexcept { return static_cast<uint32_t>(xMax - xMin); }
		uint32_t GetGlyphHeight() const noexcept { return static_cast<uint32_t>(yMax - yMin); }
		const Vector<BSpline*>& GetBSplines() const noexcept { return bsplines; }
	public:
		void Parse() noexcept {
			for (uint32_t i = 0; i < size; ++i) {
				if (data[i] == 28 || data[i] >= 32) {
					fixed32_c tmp;
					i = GetType2Value(tmp, data, i, size - 1) - 1;
					args[argCount++] = static_cast<int16_t>(tmp);
				}
				else {
					System::Type2Operator opr = static_cast<System::Type2Operator>(data[i] != 12 ? data[i] : data[i] << 8 | data[++i]);
					if (opr == Type2Operator::Callsubr || opr == Type2Operator::Callgsubr) {
						int16_t subr = args[argCount - 1];
						args[--argCount] = 0;
						CFF1IndexData subrData = (opr == Type2Operator::Callsubr ? local : global).GetSubr(subr);
						const uint8_t* currentData = data;
						const uint32_t currentSize = size;
						data = subrData.data;
						size = subrData.size;
						Parse();
						data = currentData;
						size = currentSize;
					}
					else {
						if (!widthDelta && argCount) {
							widthDelta = args[0];
							for (uint8_t j = 1; j < argCount; ++j) args[j - 1] = args[j];
							--argCount;
						}
						switch (opr) {
						case Type2Operator::Hstem:
						case Type2Operator::Hstemhm:
							SetHStem();
							break;
						case Type2Operator::Vstem:
						case Type2Operator::Vstemhm:
							SetVStem();
							break;
						case Type2Operator::Hintmask:
						case Type2Operator::Cntrmask:
							if (argCount) {
								if (!hStemCountX2) SetHStem();
								else SetVStem();
							}
							if (opr == Type2Operator::Hintmask) SetHintMask(i);
							else SetCntrMask(i);
							break;
						default:
							CallPathOperator(opr);
							break;
						}
					}
				}
			}
		}
	};
}

//CFF1
export namespace System {
	struct CFF1 {
		CFF1Header header;
		CFF1Index name;
		CFF1Index topDict;
		StringIndex string;
		CFF1SubrsIndex globalSubrsIndex;
		CFF1Encoding encoding;
		CFF1Index charString;
		CFF1Charset charset;
		FDSelect fdSelect;
		CFF1Index fdArray;
		//PrivateDICTの配列。
		//要素数はtopDictにPrivateが存在する場合は[1]、それ以外の場合は[fdArray.count]
		PrivateDICT* privateDict = nullptr;
	private:
		//charStringに含まれるグリフ数を保持する。
		//charString.countが0の場合、MAXP::numGlyphsを保持する
		uint16_t m_numGlyphs = 0;
		//privateDictの要素数を保持する
		uint16_t m_privateDictCount = 0;
	private:
		mutable System::HashMap<uint16_t, Type2CharstringParser*> m_parsers = 3;
	public:
		~CFF1() noexcept {
			delete[] privateDict;
			privateDict = nullptr;
			for (Type2CharstringParser* x : m_parsers.Values()) delete x;
		}
	public:
		bool Load(System::IO::FileStream& file, uint32_t offset, const MAXP& maxp) {
			if (!header.Load(file, offset)) return false;
			if (!name.Load(file, 0, 0)) return false;
			if (!topDict.Load(file, 0, 0)) return false;
			if (!string.Load(file, 0, 0)) return false;
			DICTValue<uint8_t> charstringTypeValue = topDict.GetData(0).GetDICTValue<uint8_t>(DICTOperator::CharstringType);
			uint8_t charstringType = charstringTypeValue.count ? charstringTypeValue.data[0] : 2;
			if (!globalSubrsIndex.Load(file, 0, 0, charstringType)) return false;

			//Encoding
			DICTValue<uint32_t> value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::Encoding);
			if (value.data && !encoding.Load(file, offset, value.data[0])) return false;
			//FDArray
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::FDArray);
			if (value.data && !fdArray.Load(file, offset, value.data[0])) return false;
			//PrivateDICT
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::Private);
			if (value.data && value.count == 2) {
				m_privateDictCount = 1;
				privateDict = new PrivateDICT[m_privateDictCount];
				if (!privateDict[0].Load(file, value.data[0], offset, value.data[1], charstringType)) return false;
			}
			else {
				m_privateDictCount = fdArray.count;
				privateDict = new PrivateDICT[m_privateDictCount];
				for (uint16_t i = 0; i < m_privateDictCount; ++i) {
					DICTValue<uint32_t> fdValue = fdArray.GetData(i).GetDICTValue<uint32_t>(DICTOperator::Private);
					if (fdValue.data && fdValue.count == 2 && !privateDict[i].Load(file, fdValue.data[0], offset, fdValue.data[1], charstringType)) {
						m_privateDictCount = i;
						return false;
					}
				}
			}
			//Charstring
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::CharStrings);
			if (value.data && !charString.Load(file, offset, value.data[0])) return false;
			m_numGlyphs = charString.count ? charString.count : maxp.numGlyphs;
			//Charset
			if (m_numGlyphs) {
				value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::charset);
				if (value.data && !charset.Load(file, offset, value.data[0], m_numGlyphs)) return false;
			}
			//FDSelect
			if (m_numGlyphs) {
				value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::FDSelect);
				if (value.data && !fdSelect.Load(file, offset, value.data[0], m_numGlyphs)) return false;
			}
			return static_cast<bool>(file);
		}
	private:
		const PrivateDICT& GetPrivateDICT(uint16_t gid) const noexcept {
			return privateDict[m_privateDictCount == 1 ? 0 : fdSelect.GetFDIndex(gid)];
		}
		const CFF1SubrsIndex& GetLocalSubrsIndex(uint16_t gid) const noexcept {
			return GetPrivateDICT(gid).subrs;
		}
		uint16_t GetDefaultWidth(uint16_t gid) const noexcept {
			DICTValue<uint16_t> value = GetPrivateDICT(gid).GetData().GetDICTValue<uint16_t>(DICTOperator::defaultWidthX);
			return value.data ? value.data[0] : 0;
		}
		uint16_t GetNormalWidth(uint16_t gid) const noexcept {
			DICTValue<uint16_t> value = GetPrivateDICT(gid).GetData().GetDICTValue<uint16_t>(DICTOperator::nominalWidthX);
			return value.data ? value.data[0] : 0;
		}
	private:
		Type2CharstringParser* GetParser(uint16_t gid) const noexcept {
			return m_parsers.At(gid);
		}
		Type2CharstringParser* GetParser(uint16_t gid, uint16_t advanceWidth) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			if (!parser) {
				const CFF1IndexData type2 = charString.GetData(gid);
				parser = new Type2CharstringParser{ type2.data, type2.size, GetLocalSubrsIndex(gid), globalSubrsIndex };
				m_parsers.Insert(gid, parser);
				const uint16_t defaultWidth = GetDefaultWidth(gid);
				if (advanceWidth != defaultWidth) parser->SetNormalWidth(GetNormalWidth(gid));
				else {
					parser->SetNormalWidth(defaultWidth);
					parser->SetWidthDelta(0);
				}
				parser->Parse();
			}
			return parser;
		}
	private:
		static System::Drawing::Image GetGlyph_Internal(const Type2CharstringParser& parser, float scale, const System::Drawing::Pixel& contoursPixel) noexcept {
			const Vector<BSpline*>& bsplines = parser.GetBSplines();
			if (!bsplines.Count()) return System::Drawing::Image();
			System::Drawing::Image image{
				static_cast<uint32_t>(parser.GetGlyphWidth() * scale + 1),
				static_cast<uint32_t>(parser.GetGlyphHeight() * scale + 1)
			};
			System::Drawing::Pixel* pixels = image.Data();
			const uint32_t imageXMax = image.Width() - 1;
			const uint32_t imageYMax = image.Height() - 1;
			const int16_t xMin = parser.GetXMin();
			const int16_t yMin = parser.GetYMin();
			for (BSpline* bspline : bsplines) {
				bool first = true;	//輪郭の最初の点の処理時のみtrue
				Point<int32_t> prev;	//直前の点座標
				int8_t yFirstSign = 0;	//輪郭内で初めてy座標が変化した方向(1 or -1)
				int8_t yPrevSign = 0;	//直前のy座標が変化した方向(1 or -1)
				for (Point<int32_t> p : bspline->Splines(xMin, yMin, parser.GetXMax(), parser.GetYMax())) {
					uint32_t x = static_cast<uint32_t>((p.x - xMin) * scale);
					//グリフ座標は左下原点、Image座標は左上原点なので上下反転
					uint32_t y = imageYMax - static_cast<uint32_t>((p.y - yMin) * scale);
					//座標(0, y)のピクセルへのポインタ
					System::Drawing::Pixel* line0 = pixels + (y * (imageXMax + 1));
					image.SetPixel(x, y, contoursPixel);
					//現在の点の水平右方向のピクセルのアルファ値を反転する(0x00 <-> 0xff)
					if (first) {
						first = false;
						System::PixelInverse(line0, x, imageXMax);
					}
					else {
						int8_t ySign = static_cast<int8_t>(System::Math::Sign(static_cast<int64_t>(y) - prev.y));
						if (ySign) {
							System::PixelInverse(line0, x, imageXMax);
							//y座標の変位方向が反転したとき、直前の点でのアルファ値反転を取り消す
							if (yPrevSign && ySign != yPrevSign) {
								line0 += static_cast<int64_t>(yPrevSign) * (imageXMax + 1);
								System::PixelInverse(line0, prev.x, imageXMax);
							}
							yPrevSign = ySign;
							if (!yFirstSign) yFirstSign = ySign;
						}
					}
					prev.x = x;
					prev.y = y;
				}
				//輪郭の最後の点(最初の点)が上底もしくは下底のいずれでもない場合、
				//最後のアルファ値反転を取り消す
				if (yFirstSign && yFirstSign == yPrevSign) {
					System::Drawing::Pixel* line0 = pixels + (prev.y * (imageXMax + 1));
					System::PixelInverse(line0, prev.x, imageXMax);
				}
			}
			//アルファ値0x00のピクセルを黒色(0x000000ff)に塗りつぶす
			for (uint32_t y = 0; y <= imageYMax; ++y) {
				System::Drawing::Pixel* line0 = pixels + (imageXMax + 1ull) * y;
				for (uint32_t x = 0; x <= imageXMax; ++x) {
					if (line0[x].a == 0x00u) {
						line0[x].r = 0x00u;
						line0[x].g = 0x00u;
						line0[x].b = 0x00u;
						line0[x].a = 0xffu;
					}
					else if (line0[x].a == 0xaau) line0[x].a = 0xffu;
				}
			}
			return image;
		}
	public:
		int16_t GetXMin(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetXMin() : 0;
		}
		int16_t GetXMax(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetXMax() : 0;
		}
		int16_t GetYMin(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetYMin() : 0;
		}
		int16_t GetYMax(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetYMax() : 0;
		}
		uint16_t GetAdvanceWidth(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetAdvanceWidth() : 0;
		}
		uint16_t GetAdvanceWidth(uint16_t gid, uint16_t advanceWidth) const noexcept {
			Type2CharstringParser* parser = GetParser(gid, advanceWidth);
			return parser ? parser->GetAdvanceWidth() : 0;
		}
		/// <summary>
		/// 指定したグリフ画像を取得する
		/// </summary>
		/// <param name="gid">グリフID</param>
		/// <param name="scale">描画スケール</param>
		/// <param name="advancedWidth">hmtxテーブルから取得するグリフIDに対応する前進幅</param>
		System::Drawing::Image GetGlyphImageByGID(uint16_t gid, float scale, uint16_t advanceWidth, const System::Drawing::Pixel& contoursPixel) const noexcept {
			if (gid < m_numGlyphs) {
				Type2CharstringParser* parser = GetParser(gid, advanceWidth);
				if (parser) return GetGlyph_Internal(*parser, scale, contoursPixel);
			}
			return System::Drawing::Image();
		}
	};
}
