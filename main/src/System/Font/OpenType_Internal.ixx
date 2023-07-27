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

//�����pEnum
export namespace System {
	//�����G���R�[�h�ݒ�
	enum class EncodingType : uint8_t {
		NONE,
		UNICODE,
		SHIFT_JIS
	};

	//DICT�`���Ŏg�p���鉉�Z�q
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
		Undefined	//DICTKeyValue�p
	};

	System::String GetDICTOperatorName(DICTOperator opr) noexcept {
		switch (opr) {
			//Value�����
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

	//GLYF�e�[�u����͗p�萔
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

	//Type2 CharString�`���Ŏg�p���鉉�Z�q
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
	/// �I�t�Z�b�g�e�[�u����\���\���́B
	/// OpenType�t�H���g�t�@�C���̐擪�ɔz�u����A�t�@�C���^�C�v�⎟�ɔz�u�����
	/// �e�[�u�����R�[�h�z��̗v�f���Ȃǂ������B
	/// </summary>
	struct OffsetTable {
	private://�A�E�g���C���f�[�^�̌`����\���萔�Bsfnt_version�Ɣ�r����B
		static constexpr uint32_t TRUETYPE_VERSION = 0x00010000u;	//TrueType�A�E�g���C��
		static constexpr uint32_t CFF_VERSION = 0x4f54544fu;	//CFF(PostScript�A�E�g���C��)
	public:
		uint32_t sfnt_version = 0;	//�A�E�g���C���`��
		uint16_t numTables = 0;	//�e�[�u�����R�[�h�z��̗v�f��
		uint16_t searchRange = 0;
		uint16_t entrySelector = 0;
		uint16_t rangeShift = 0;
	public:
		/// <summary>
		/// �t�@�C������I�t�Z�b�g�e�[�u����ǂݎ��A���̃C���X�^���X���X�V����B
		/// </summary>
		/// <param name="file">
		/// OpenType�t�H���g�t�@�C����ǂݎ��\�̃o�C�i���f�[�^�Ƃ��ĊJ�����t�@�C���X�g���[���B
		/// </param>
		/// <returns>
		/// �ǂݎ����FileStream�̏�Ԓl�B
		/// �擪4�o�C�g��OpenType�t�H���g�t�@�C���̂��̂ƈ�v���Ȃ��ꍇ�Afalse�B
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
	/// �e�[�u�����R�[�h��\���\���́B
	/// �I�t�Z�b�g�e�[�u���ɑ����z��Ƃ��Ĕz�u����A
	/// �e�e�[�u���ւ̃I�t�Z�b�g�Ƃ��̃e�[�u���̃o�C�g���������B
	/// </summary>
	struct TableRecord {
		//�e�[�u���̎��ʎq
		char tag[4] = {};
		//�e�[�u����4�o�C�g�`�F�b�N�T���B
		//HEAD::checkSumAdjustment������Ɍv�Z����B
		//(HEAD�e�[�u���v�Z���ɂ�HEAD::checkSumAdjustment��0�Ƃ��Ĉ���)
		uint32_t checkSum = 0;
		//�t�@�C���̐擪����e�[�u���ւ̃I�t�Z�b�g(�o�C�g)
		uint32_t offset = 0;
		//�e�[�u���̃o�C�g��
		uint32_t length = 0;
	public:
		/// <summary>
		/// �t�@�C������e�[�u�����R�[�h��ǂݎ��A���̃C���X�^���X���X�V����B
		/// </summary>
		/// <param name="file">
		/// OpenType�t�H���g�t�@�C����ǂݎ��\�̃o�C�i���f�[�^�Ƃ��ĊJ�����t�@�C���X�g���[���B
		/// ���݂̃V�[�N�ʒu����ǂݎ��B
		/// </param>
		/// <returns>�ǂݎ����FileStream�̏�Ԓl</returns>
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
	/// CMAP�N���X�Ŏg�p�����T�u�e�[�u���̊��N���X
	/// </summary>
	struct SubTable {
		//�T�u�e�[�u���̃t�H�[�}�b�g�ԍ�
		uint16_t format;
	public:
		SubTable() noexcept : SubTable(0xffff) {}
		SubTable(uint16_t format) noexcept : format(format) {}
		virtual ~SubTable() noexcept = default;
	public:
		/// <summary>
		/// �R�[�h�|�C���g�ɑΉ�����O���tID���擾����
		/// </summary>
		/// <param name="codePoint">�R�[�h�|�C���g</param>
		/// <returns>�O���tID�B���݂��Ȃ��R�[�h�|�C���g�̏ꍇ�A0��Ԃ��B</returns>
		virtual uint16_t GetGID(uint32_t codePoint) const noexcept = 0;
		/// <summary>
		/// �T�u�e�[�u���̃o�C�g�����擾����
		/// </summary>
		virtual uint32_t Length() const noexcept = 0;
	};

	/// <summary>
	/// SubTableFormat0, 2, 4, 6�̌p����
	/// </summary>
	struct SubTableBase16 : public SubTable {
		//�T�u�e�[�u���̃o�C�g��
		uint16_t length;
		//����ŗL�̃G���R�[�f�B���O���w�肷��B
		//����SubTable�ւ̃I�t�Z�b�g������EncodingRecord��platformID�����o��1�ȊO�̏ꍇ�A0�Œ�B
		//1(Macintosh)�̏ꍇ�A�w�肵���������Macintosh����ID��1�����Z�����l���w�肷��B
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
	/// SubTableFormat8, 10, 12, 13, 14�̌p����
	/// </summary>
	struct SubTableBase32 : public SubTable {
		//�T�u�e�[�u���̃o�C�g��
		uint32_t length;
		//����ŗL�̃G���R�[�f�B���O���w�肷��B
		//����SubTable�ւ̃I�t�Z�b�g������EncodingRecord��platformID�����o��1�ȊO�̏ꍇ�A0�Œ�B
		//1(Macintosh)�̏ꍇ�A�w�肵���������Macintosh����ID��1�����Z�����l���w�肷��B
		//Format14�ɂ͂��̃����o�����݂��Ȃ��B
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
	/// 1�o�C�g�����ƃO���t���֘A�t����t�H�[�}�b�g�B
	/// </summary>
	struct SubTableFormat0 : public SubTableBase16 {
		//�O���t�C���f�b�N�X�̔z��B
		//�����R�[�h��z��̓Y�����Ƃ����Ƃ��A�l�̓O���t�C���f�b�N�X��\���B
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
	/// 1�o�C�g������2�o�C�g�������������ꂽ�����R�[�h(���{��A������A�؍���)�ƃO���t���֘A�t����t�H�[�}�b�g�B
	/// </summary>
	struct SubTableFormat2 : public SubTableBase16 {
		/// <summary>
		/// �����R�[�h�̉��ʃo�C�g����GlyphIndexArray�ւ̑Ή���\���\����
		/// </summary>
		struct SubHeader {
			//���ʃo�C�g�̍ŏ��̒l(�ŏ��l)
			uint16_t firstCode;
			//���ʃo�C�g�͈̔�([firstCode, firstCode + entryCode])
			uint16_t entryCount;
			//glyphIndexArray[]�̑Ή�����l�ɉ��Z���鍷���l�B
			//�@��65536�Ȃ̂ŁA�O���t�C���f�b�N�X��(uint16_t)(glyphIndexArray[n] + idDelta)�B
			int16_t idDelta;
			//����SubHeader�̎��̃o�C�g����A����SubHeader�ɑΉ�����glyphIndexArray[]�܂ł̃o�C�g��
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
		//�����R�[�h�̏�ʃo�C�g���C���f�b�N�X�Ƃ��A�Ή�����subHeaders[]�̃C���f�b�N�X��8�{��l�Ƃ���z��
		uint16_t subHeaderKeys[256];
		//�v�f����(subHeaderKeys[]��0�łȂ��v�f�� + 1)
		Vector<SubHeader> subHeaders;
		// �O���t�C���f�b�N�X�̔z��B
		// subHeaders[n]�ɂ̓C���f�b�N�X�ԍ�
		// (subHeaders[n].idRangeOffset - (subHeaders.Count() - n - 1) * sizeof(SubHeader)) / sizeof(glyphIndexArray[0])���Ή�����B
		uint16_t* glyphIndexArray = nullptr;
	public:
		SubTableFormat2(System::IO::FileStream& file) noexcept : SubTableBase16(file, 2) {
			file.ReadBE(subHeaderKeys);
			uint16_t subHeaderCount = 0;
			//subHeaderKeys�̒��ň�ԑ傫���l��8�Ŋ������l��SubHeaders�̍Ō�̃C���f�b�N�X
			for (uint16_t i = 0; i < 256; ++i) {
				if (subHeaderCount < subHeaderKeys[i]) subHeaderCount = subHeaderKeys[i];
			}
			subHeaderCount /= 8;
			++subHeaderCount;
			subHeaders.Reserve(subHeaderCount);
			for (uint16_t i = 0; i < subHeaderCount; ++i) subHeaders.Add(SubHeader(file));
			//glyphIndexArray[]�̃o�C�g����(�T�u�e�[�u���̃o�C�g��)-(�����܂œǂݍ��񂾃o�C�g��)
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
				else return 0;	//��`�゠�肦�Ȃ�
			}
		}
	};

	/// <summary>
	/// Unicode��U+0000����U+FFFF(��{������ʕ���)�݂̂��O���t�Ɋ֘A�t����t�H�[�}�b�g
	/// </summary>
	struct SubTableFormat4 : public SubTableBase16 {
		//�Z�O�����g�̌���2�{�����l�B
		//endCode[], startCode[], idDelta[], idRangeOffset[]�̗v�f����(segCountX2 / 2)�B
		uint16_t segCountX2;
		//2 * (2^floor(log2(segCount)))������
		uint16_t searchRange;
		//log2(searchRange / 2)������
		uint16_t entrySelector;
		//segCountX2 - searchRange������
		uint16_t rangeShift;
		//�Ή�����Z�O�����g�̍Ō�̕����R�[�h�������z��B�Ō�̗v�f��0xffff�Œ�B
		uint16_t* endCode = nullptr;
		//0�Œ�
		uint16_t reservedPad;
		//�Ή�����Z�O�����g�̍ŏ��̕����R�[�h�������z��
		uint16_t* startCode = nullptr;
		//�Ή�����Z�O�����g�Ɋ܂܂�邷�ׂĂ̕����R�[�h�ɉ��Z����鍷���l�̔z��
		int16_t* idDelta = nullptr;
		//�Ή�����glyphIDArray�̈ʒu�܂ł̃I�t�Z�b�g�̔z��
		uint16_t* idRangeOffset = nullptr;
		//�v�f����(length - (sizeof(uint16_t) * (8 + segCount * 4))) / sizeof(uint16_t)
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
					else return 0;	//�Z�O�����g�͈͊O
				}
			}
			return 0;
		}
	};

	/// <summary>
	/// Format4�ɂ����ăt�H���g���̕����R�[�h���A�������P��͈̔͂ɑ��݂���ꍇ�Ɏg�p�����t�H�[�}�b�g�B
	/// 16bitUnicode��\���T�u�e�[�u����Format4��Format6�̈���̂݃t�@�C���Ɋ܂܂��B
	/// (��������ꍇ�AFormat4��D�悷��)
	/// </summary>
	struct SubTableFormat6 : public SubTableBase16 {
		//�A�����镶���R�[�h�͈͂̍ŏ��̒l
		uint16_t firstCode;
		//�A�����镶���R�[�h�͈͂Ɋ܂܂�镶���R�[�h�̐�
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
	/// Format8, 12�Ŏg�p����镶���R�[�h�̃O���[�v��\���\����
	/// </summary>
	struct SequentialMapGroup {
		//���̃O���[�v�̍ŏ��̕����R�[�h�B���O�̃O���[�v��endCharCode�����傫���B
		uint32_t startCharCode;
		//���̃O���[�v�̍Ō�̕����R�[�h
		uint32_t endCharCode;
		union {
			//startCharCode�ɑΉ�����O���tID�BFormat8, 12�Ŏg�p
			uint32_t startGlyphID;
			//���̃O���[�v���ׂĂ̕����R�[�h�ɑΉ�����O���tID�BFormat13�Ŏg�p
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
	/// Format13�Ŏg�p����镶���R�[�h�̃O���[�v��\���\����
	/// </summary>
	using ConstantMapGroup = SequentialMapGroup;

	/// <summary>
	/// 16bitUnicode(0x0000-0xffff)��32bitUnicode(0x10000-0x10ffff)�����������t�H�[�}�b�g
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
	/// �����R�[�h(Unicode 0x0000-0x10ffff)���A�������P��͈̔͂ɑ��݂���ꍇ�Ɏg�p�����t�H�[�}�b�g
	/// </summary>
	struct SubTableFormat10 : public SubTableBase32 {
		//�A�����镶���R�[�h�͈͂̍ŏ��̒l
		uint32_t startCharCode;
		//�A�����镶���R�[�h�͈͂Ɋ܂܂�镶���R�[�h�̐�
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
	/// Unicode 0x0000-0x10ffff�̕����R�[�h���O���[�v�P�ʂŃO���tID�Ɗ֘A�t����t�H�[�}�b�g�B
	/// Format8�ƈقȂ�A���ׂĂ̕����R�[�h��32bit�Ƃ��Ĉ���
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
	/// Format12�Ɠ����\���̃t�H�[�}�b�g�B
	/// �O���[�v���̕����R�[�h���ׂĂɓ����O���tID��Ή�������_�݂̂��قȂ�
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
	/// �Ή�����ّ̎��Z���N�^(VariationSelector::varSelector)���ݒ肳��Ă���
	/// �����R�[�h��\���\����
	/// </summary>
	struct DefaultUVS {
		/// <summary>
		/// �����R�[�h�͈͂�\���\����
		/// </summary>
		struct UnicodeRange {
			//���͈̔͂̍ŏ��̕����R�[�h
			uint24_c startUnicodeValue;
			//startUnicodeValue�ɑ������͈̔͂̕����R�[�h���B
			//startUnicodeValue + additionalCount�����͈̔͂̍Ō�̕����R�[�h�ƂȂ�
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
		//�Ή�����ّ̎��Z���N�^���ݒ肳��Ă��镶���R�[�h�͈͂̔z��
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
	/// �Ή�����ّ̎��Z���N�^(VariationSelector::varSelector)���ݒ肳��Ă���
	/// �����R�[�h�ƃO���tID�̑g�ݍ��킹��\���\����
	/// </summary>
	struct NonDefaultUVS {
		/// <summary>
		/// �����R�[�h�ɑΉ�����O���tID��\���\����
		/// </summary>
		struct UVSMapping {
			//�����R�[�h
			uint24_c unicodeValue;
			//unicodeValue + �ّ̎��Z���N�^�ɑΉ�����O���tID
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
		//�����R�[�h�ƃO���tID�̑g�ݍ��킹�̔z��
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
	/// Format14�Ŏg�p�����ّ̎��Z���N�^�Ƃ��̃}�b�s���O�Ɏg�p�����
	/// UVS�e�[�u���ւ̃I�t�Z�b�g��\���\����
	/// </summary>
	struct VariationSelector {
		//�ّ̎��Z���N�^
		uint24_c varSelector;
		//�T�u�e�[�u���̐擪����defaultUVS�ւ̃I�t�Z�b�g�B
		//SubTableFormat14::defaultUVS�ǂݍ��݌�̓C���f�b�N�X���i�[����
		uint32_t defaultUVSOffset;
		//�T�u�e�[�u���̐擪����nonDefaultUVS�ւ̃I�t�Z�b�g
		//SubTableFormat14::nonDefaultUVS�ǂݍ��݌�̓C���f�b�N�X���i�[����
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
	/// Unicode�o���G�[�V�����V�[�N�G���X(�ّ̎��Z���N�^)�ɑΉ������t�H�[�}�b�g�B
	/// EncodingRecord��platformID: 0����encodingID: 5�̏ꍇ�̂ݎg�p����
	/// </summary>
	struct SubTableFormat14 : public SubTableBase32 {
		uint32_t numVarSelectorRecords;
		//[numVarSelectorRecords]
		VariationSelector* varSelector = nullptr;
		//varSelector[n].defaultUVSOffset���C���f�b�N�X�Ƃ��铮�I�z��
		Vector<DefaultUVS> defaultUVS;
		//varSelector[n].nonDefaultUVSOffset���C���f�b�N�X�Ƃ��铮�I�z��
		Vector<NonDefaultUVS> nonDefaultUVS;
	public:
		SubTableFormat14(System::IO::FileStream& file) noexcept : SubTableBase32(file, 14) {
			//SubTableBase32�œǂݍ��܂��o�C�g���߂��āA�T�u�e�[�u���̐擪�ʒu���擾����
			file.Seek(-6, System::IO::SeekDir::Current);
			System::IO::StreamPos subTableBegin = file.TellPos();
			file.Seek(6, System::IO::SeekDir::Current);
			file.ReadBE(numVarSelectorRecords);
			varSelector = new VariationSelector[numVarSelectorRecords];
			for (uint32_t i = 0; i < numVarSelectorRecords; ++i) varSelector[i].Load(file);
			defaultUVS.Reserve(numVarSelectorRecords);
			nonDefaultUVS.Reserve(numVarSelectorRecords);
			//���łɓǂݍ��񂾃I�t�Z�b�g�l���L�^����
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
	/// cmap�e�[�u����\���\���́B
	/// �����R�[�h����O���t�C���f�b�N�X�ւ̃}�b�s���O���`���Ă���
	/// </summary>
	struct CMAP {
		/// <summary>
		/// �T�u�e�[�u���̃G���R�[�f�B���O����\���\����
		/// </summary>
		struct EncodingRecord {
			//�v���b�g�t�H�[�����ʎq
			//0 : Unicode
			//1 : Macintosh(�񐄏��E���ʌ݊��ێ��p)
			//2 : do not use
			//3 : Microsoft
			uint16_t platformID = 0;
			//�v���b�g�t�H�[���ŗL�G���R�[�h���ʎq
			//�EplatformID��0(Unicode)�̏ꍇ
			//0 : �f�t�H���g�̃Z�}���e�B�N�X
			//1 : version 1.1�̃Z�}���e�B�N�X
			//2 : ISO 10646 1993�̃Z�}���e�B�N�X
			//3 : Unicode 2.0�ȍ~�̃Z�}���e�B�N�X(BMP�̂�)
			//4 : Unicode 2.0�ȍ~�̃Z�}���e�B�N�X
			//5 : Unicode Variation Sequence
			//6 : Unicode�����S�ԗ�(OpenType��13.0cmaps���g�p)
			//�EplatformID��3(Microsoft)�̏ꍇ
			//0 : �V���{��
			//1 : Unicode BMP�̂�
			//2 : Shift-JIS
			//3 : PRC
			//4 : BigFive
			//5 : Johab
			//10 : Unicode UCS-4
			uint16_t encodingID = 0;
			//CMAP�e�[�u���擪����T�u�e�[�u���ւ̃I�t�Z�b�g(�o�C�g)
			uint32_t offset = 0;
		};
	public:
		//�o�[�W������(0�Œ�)
		uint16_t version = 0;
		//encodingRecords[]�̗v�f��
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
		/// �t�@�C������CMAP�e�[�u����ǂݎ��A���̃C���X�^���X���X�V����
		/// </summary>
		/// <param name="file">
		/// OpenType�t�H���g�t�@�C����ǂݎ��\�̃o�C�i���f�[�^�Ƃ��ĊJ�����t�@�C���X�g���[��
		/// </param>
		/// <param name="offset">
		/// �t�@�C���擪����CMAP�e�[�u���ւ̃I�t�Z�b�g(�o�C�g)�B
		/// TableRecord::offset�̒l���w�肷��B
		/// </param>
		/// <returns>�ǂݎ����FileStream�̏�Ԓl</returns>
		bool Load(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			System::IO::StreamPos cmapPos = file.TellPos();
			file.ReadBE(version);
			file.ReadBE(numTables);
			//�G���R�[�f�B���O���R�[�h�擾
			encodingRecords = new EncodingRecord[numTables];
			for (uint16_t i = 0; i < numTables; ++i) {
				file.ReadBE(encodingRecords[i].platformID);
				file.ReadBE(encodingRecords[i].encodingID);
				file.ReadBE(encodingRecords[i].offset);
			}
			//��Ԕ͈͂̑傫��Unicode�pSubTable�̃C���f�b�N�X��ۑ�����
			m_unicodeSubtableIndex = GetUnicodeSubTableIndex();
			//�T�u�e�[�u���擾
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
		/// �t�@�C������CMAP�o�[�W�����Ƃ��ׂẴG���R�[�f�B���O���R�[�h�A
		/// ����ш�ԍL��Unicode�͈͂ɑΉ������T�u�e�[�u����I�����ēǂݎ��A
		/// ���̃C���X�^���X���X�V����
		/// </summary>
		/// <param name="file">
		/// OpenType�t�H���g�t�@�C����ǂݎ��\�̃o�C�i���f�[�^�Ƃ��ĊJ�����t�@�C���X�g���[��
		/// </param>
		/// <param name="offset">
		/// �t�@�C���擪����CMAP�e�[�u���ւ̃I�t�Z�b�g(�o�C�g)�B
		/// TableRecord::offset�̒l���w�肷��B
		/// </param>
		/// <returns>�ǂݎ����FileStream�̏�Ԓl</returns>
		bool QuickLoad(System::IO::FileStream& file, uint32_t offset) noexcept {
			file.Seek(offset, System::IO::SeekDir::Begin);
			System::IO::StreamPos cmapPos = file.TellPos();
			file.ReadBE(version);
			file.ReadBE(numTables);
			//�G���R�[�f�B���O���R�[�h�擾
			encodingRecords = new EncodingRecord[numTables];
			for (uint16_t i = 0; i < numTables; ++i) {
				file.ReadBE(encodingRecords[i].platformID);
				file.ReadBE(encodingRecords[i].encodingID);
				file.ReadBE(encodingRecords[i].offset);
			}
			//��Ԕ͈͂̑傫��Unicode�pSubTable�̃C���f�b�N�X��ۑ�����
			m_unicodeSubtableIndex = GetUnicodeSubTableIndex();
			//(m_unicodeSubtableIndex)�Ԗڂ̃T�u�e�[�u���̂ݎ擾
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
	/// head�e�[�u����\���\���́B
	/// �t�H���g�Ɋւ���O���[�o���ȏ�񂪊i�[����Ă���
	/// </summary>
	struct HEAD {
		//0x0001�Œ�
		uint16_t majorVersion = 0;	//0x0001�Œ�
		//0x0000�Œ�
		uint16_t minorVersion = 0;
		//�t�H���g���[�J�[���w��B16.16�̌Œ菬���_��(Fixed)�B
		//�t�H���g�̃o�[�W�����͂��̃v���p�e�B�ł͂Ȃ��Aname�e�[�u�����Q�Ƃ���
		int32_t fontRevision = 0;
		//�t�H���g�S�̂�4�o�C�g�`�F�b�N�T������0xb1b0afba���������l
		uint32_t checkSumAdjustment = 0;
		//0x5f0f3cf5�Œ�
		uint32_t magicNumber = 0;
		//�e�r�b�g�t���O�̘a
		//0: �t�H���g�̃x�[�X���C����y=0
		//1: ���T�C�h�x�A�����O��x=0(TrueType���X�^�C�U�̂�)
		//2: �|�C���g�T�C�Y�ɂ���ĈӖ����قȂ�
		//3: �����X�P�[�����Z�Ő���ppem����������B�I�t�̎��A����ppem���g�p�\�B
		//4: 
		//5: �g�p���Ȃ�(�ꕔ�v���b�g�t�H�[���Ő������C�A�E�g�ɉe��)
		//6-10: �g�p���Ȃ�
		//11: ���X���X���k����Ă���BDSIG�e�[�u���������ɂȂ�\������
		//12: ���g���N�X�݊�
		//13: ClearType�ɍœK������Ă���BEBDT���g�p����t�H���g��ClearType�ł͂Ȃ�(�̂ŃI�t)�B
		//14: �I�t�̎��Acmap����擾�����O���t�C���f�b�N�X���R�[�h�|�C���g�ɑΉ������O���t��\���Ă���
		//15: 0�Œ�
		uint16_t flags = 0;
		//[16, 16384]�̂����ꂩ�BTrueType�ł�2�̗ݏ搄��
		uint16_t unitsPerEm = 0;
		//�쐬����(1904/1/1 00:00����̕b��)
		int64_t created = 0;
		//�C������(1904/1/1 00:00����̕b��)
		int64_t modified = 0;
		//���ׂẴO���t���E�{�b�N�X�̍ŏ�x���W
		int16_t xMin = 0;
		//���ׂẴO���t���E�{�b�N�X�̍ŏ�y���W
		int16_t yMin = 0;
		//���ׂẴO���t���E�{�b�N�X�̍ő�x���W
		int16_t xMax = 0;
		//���ׂẴO���t���E�{�b�N�X�̍ő�y���W
		int16_t yMax = 0;
		//�e�r�b�g�t���O�̘a�BOS_2::fsSelection�ƈ�v����B
		//0: ����(Bold)
		//1: �C�^���b�N��(Italic)
		//2: ����(Underline)
		//3: �֊s��(Outline)
		//4: �e�t��(Shadow)
		//5: �������ׂ�(Condensed)
		//6: ����������(Extended)
		//7-15: �\��̈�(0�Œ�)
		uint16_t macStyle = 0;
		//�ǂݎ��\�ȍŏ��s�N�Z��
		uint16_t lowestRecPPEM = 0;
		//�����q���g�B�񐄏��ȃv���p�e�B�ł���A2�ɌŒ肳���B
		int16_t fontDirectionHint = 0;
		//loca.offsets[maxp.numGlyphs + 1]�̌^���w��
		//0 : uint16_t(���ۂ̃I�t�Z�b�g�l��2�Ŋ��������̂��ۑ�����Ă���)
		//1 : uint32_t(���ۂ̃I�t�Z�b�g�l���ۑ�����Ă���)
		int16_t indexToLocFormat = 0;
		//0�Œ�
		int16_t glyphDataFormat = 0;
	public:
		/// <summary>
		/// �t�@�C������HEAD�e�[�u����ǂݎ��A���̃C���X�^���X���X�V����
		/// </summary>
		/// <param name="file">
		/// OpenType�t�H���g�t�@�C����ǂݎ��\�̃o�C�i���f�[�^�Ƃ��ĊJ�����t�@�C���X�g���[��
		/// </param>
		/// <param name="offset">
		/// �t�@�C���擪����HEAD�e�[�u���ւ̃I�t�Z�b�g(�o�C�g)�B
		/// TableRecord::offset�̒l���w�肷��B
		/// </param>
		/// <returns>�ǂݎ����FileStream�̏�Ԓl</returns>
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
	/// hhea�e�[�u����\���\���́B
	/// �����������C�A�E�g�����܂�ł���
	/// </summary>
	struct HHEA {
		//�e�[�u���̃��W���[�o�[�W�����B1�Œ�
		uint16_t majorVersion = 0;
		//�e�[�u���̃}�C�i�[�o�[�W�����B0�Œ�
		uint16_t minorVersion = 0;
		//�x�[�X���C������ł������A�Z���_�[���C���܂ł̋���
		int16_t ascender = 0;
		//�x�[�X���C������ł��Ⴂ�f�B�Z���_�[���C���܂ł̋���
		int16_t descender = 0;
		//�s�Ԃ̕�
		int16_t lineGap = 0;
		//hmtx�e�[�u���̍ő�O�i��
		uint16_t advanceWidthMax = 0;
		//hmtx�e�[�u���̍ŏ����T�C�h�x�A�����O�l(�֊s�������O���t�̂�)
		int16_t minLeftSideBearing = 0;
		//hmtx�e�[�u���̍ŏ��E�T�C�h�x�A�����O�l(�֊s�������O���t�̂�)
		int16_t minRightSideBearing = 0;
		//���T�C�h�x�A�����O��(xMax-XMin)�̘a�̍ő�l
		int16_t xMaxExtent = 0;
		//���̓J�[�\���̌��z�̕��q(�����̏ꍇ�A1)
		int16_t caretSlopeRise = 0;
		//���̓J�[�\���̌��z�̕���(�����̏ꍇ�A0)
		int16_t caretSlopeRun = 0;
		//�Α̃t�H���g�̏ꍇ�A���̓J�[�\�����ړ�������I�t�Z�b�g�B���̑��̏ꍇ�A0
		int16_t caretOffset = 0;
		//0�Œ�
		/*int16_t reserved[4] */
		//0�Œ�
		int16_t metricDataFormat = 0;
		//HMTX::hMetric�̗v�f��
		uint16_t numberOfHMetrics = 0;
	public:
		//HHEA�e�[�u���ւ̃I�t�Z�b�g���w�肵�ăt�@�C����ǂݎ��A�\���̂�����������
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
	/// maxp�e�[�u����\���\���́B
	/// �t�H���g�̎��l�X�ȏ��̍ő�l���܂�ł���
	/// </summary>
	struct MAXP {
		//MAXP�e�[�u���̃o�[�W�����B16.16�̌Œ菬���_���ŕ\���B
		//0x00010000(1.0)�̏ꍇ�A���ׂẴv���p�e�B������
		//0x00005000(0.5)�̏ꍇ�Aversion, numGlyphs�̂ݎ���
		int32_t version;
		//�t�H���g�Ɋ܂܂��O���t�̑���
		uint16_t numGlyphs;
		//�񕡍��O���t�̍ő吧��_��
		uint16_t maxPoints;
		//�񕡍��O���t�̍ő�֊s��
		uint16_t maxContours;
		//�����O���t�̍ő吧��_��
		uint16_t maxCompositePoints;
		//�����O���t�̍ő�֊s��
		uint16_t maxCompositeContours;
		//�]�[���̍ő吔�B1��������2���w�肷��B
		//2�̏ꍇ�A�ʏ�̃]�[��(Z1)�ɉ����ăO���t��ɑ��݂��Ȃ��_
		//���i�[����]�[��(Z0)���g�p����
		uint16_t maxZones;
		//Z0�Ɋ܂܂��_�̍ő吔
		uint16_t maxTwilightPoints;
		//�X�g���[�W�G���A�̏ꏊ�̐�
		uint16_t maxStorage;
		//�ő�֐��ԍ�+1�̐�
		uint16_t maxFunctionDefs;
		//IDEFs�̐�
		uint16_t maxInstructionDefs;
		//fpgm, prep�̃v���O������glyf�̖��߂̍ő�X�^�b�N��
		uint16_t maxStackElements;
		//�O���t���߂̍ő�o�C�g��
		uint16_t maxSizeOfInstructions;
		//�����O���t�̃g�b�v���x���ŎQ�Ƃ����ő�R���|�[�l���g��
		uint16_t maxComponentElements;
		//�R���|�[�l���g�̍ő�ċA�񐔁B�񕡍��O���t��1
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
	/// �O���t�̑O�i���ƍ��T�C�h�x�A�����O�̒l��
	/// �\���\����
	/// </summary>
	struct LongHorMetric {
		//�O�i��
		uint16_t advanceWidth;
		//���T�C�h�x�A�����O
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
	/// hmtx�e�[�u����\���\���́B
	/// �O���t�̑O�i���ƍ��T�C�h�x�A�����O���܂�ł���B
	/// �O���t�A�E�g���C���f�[�^�ƂƂ��Ɏg�p�����
	/// </summary>
	struct HMTX {
		//�O���tID�ŃA�N�Z�X�\�ȃO���t�̑O�i���ƍ��T�C�h�x�A�����O��\���z��B
		//�v�f����[HHEA::numberOfHMetrics]
		LongHorMetric* hMetrics = nullptr;
		//HHEA::numberOfMetrics�ȏ�̃O���tID�����O���t�̍��T�C�h�x�A�����O�l�̔z��
		//�v�f����[MAXP::numGlyphs - HHEA::numberOfHMetrics]
		int16_t* leftSideBearings = nullptr;
	private:
		//HHEA::numberOfHMetrics��ۑ�
		uint16_t m_numberOfHMetrics;
		//MAXP::numGlyphs - HHEA::numberOfHMetrics��ۑ�
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
		/// �O���tID�ɑΉ������O�i�����擾����
		/// </summary>
		constexpr uint16_t GetAdvanceWidth(uint16_t gid) const noexcept {
			return gid < m_numberOfHMetrics ? hMetrics[gid].advanceWidth : 0;
		}
		/// <summary>
		/// �O���tID�ɑΉ��������T�C�h�x�A�����O���擾����
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
		//�O���tID�ŃA�N�Z�X�ł���O���t�����܂ރe�[�u���̐擪����̃I�t�Z�b�g�B
		//HEAD::IndexToLocFormat��0�̏ꍇ�Auint16_t�̔z��ɂȂ�B
		//�Ō�̗v�f�͍Ō�̃O���t���̎��̃o�C�g���w���A
		//�Ō�̃O���t���̃o�C�g�������߂邽�߂Ɏg�p�ł���B
		//�v�f����[maxp.numGlyphs + 1]
		uint32_t* offsets = nullptr;
	private:
		//offsets�̗v�f���Bmaxp.numGlyphs + 1��ۑ�����
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
		/// �O���tID�ɑΉ�����O���t�����܂ރe�[�u���̐擪����̃I�t�Z�b�g���擾����B
		/// ���݂��Ȃ��O���tID�̏ꍇ�A0xffffffffu��Ԃ�
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
		//�e�֊s�̍Ō�̐���_�̃C���f�b�N�X�B
		//�Ō�̗v�f��1���������������̃O���t�Ɋ܂܂�鐧��_�̑����ƂȂ�B
		//�v�f����[Glyph::numberOfContours]
		uint16_t* endPtsOfContours = nullptr;
		//instructions�̃o�C�g��
		uint16_t instructionLength;
		//���߂̔z��B
		//�v�f����[instructionLength]
		uint8_t* instructions = nullptr;
		//�񋓌^SimpleGlyphFlags�̘a�̔z��B
		//�t�@�C����ł́ASimpleGlyphFlags::REPEAT_FLAG�ɂ����
		//�A�����铯���t���O���ȗ�����邽�߁A�ǂݍ��ނ܂Œ����͕s���B
		//�ǂݍ��ݎ��ɓW�J���邽�߁A�v�f����[endPtsOfContours[Glyph::numberOFContours - 1]]
		uint8_t* flags = nullptr;
		//�t�@�C����ł́A�O�̍��W(�n�_�̏ꍇ(0,0))����̑��΍��W��uint8_t��int16_t�ŕۑ�����Ă���A
		//�^�Ɣz��T�C�Y��flags����v�Z�����
		//�i�[���@�F(xMin, yMin)���������̌��_(0, 0)�Ƃ����΍��W�ɕϊ����Ċi�[
		//[endPtsOfCountours[Glyph::numberOfContours - 1]]
		int16_t* xCoordinates = nullptr;
		//xCoodinates�Q��
		int16_t* yCoordinates = nullptr;
	private:
		//����ۑ�
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
		//�񋓌^CompositeGlyphFlags�̘a
		uint16_t flags;
		//�Q�Ƃ���O���tID
		uint16_t glyphIndex;
		//�O���t�ɓK�p����x���W�̃I�t�Z�b�g�l
		union {
			uint8_t u8;
			int8_t s8;
			uint16_t u16;
			int16_t s16;
		} argument1;
		//�O���t�ɓK�p����y���W�̃I�t�Z�b�g�l
		union {
			uint8_t u8;
			int8_t s8;
			uint16_t u16;
			int16_t s16;
		} argument2;
		//�O���t�ɓK�p����X�P�[���l�B
		//1�̏ꍇ�A�P���ȃX�P�[���l��\���B
		//2�̏ꍇ�A���ꂼ��X���AY���̃X�P�[���l��\���B
		//4�̏ꍇ�A2*2�̕ϊ��s��(?)�ɂ��X�P�[�����O��\���B
		f2dot14_c* transformation = nullptr;
	private:
		//transformation�̗v�f����ۑ�����
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
		//�z��T�C�Y��CompositeGlyphFlags::MORE_COMPONENTS����v�Z����
		CompositeComponent* components = nullptr;
		//instructions�̃o�C�g��
		uint16_t instructionLength;
		//���߂̔z��B
		//�v�f����[instructionLength]
		uint8_t* instructions = nullptr;
	private:
		//components�̗v�f����ۑ�����
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
			//�R���|�[�l���g�̗̈���m��(�ő吔��MAXP::maxComponentElements)
			CompositeComponent* tmps = new CompositeComponent[maxp.maxComponentElements];
			m_componentCount = 0;
			//CompositeComponent::flags��MORE_COMPONENT���܂܂��ԁA�ǂݍ��ݑ�����
			for (; m_componentCount < maxp.maxComponentElements; ++m_componentCount) {
				tmps[m_componentCount].Load(file);
				if (!(tmps[m_componentCount].flags & CompositeGlyphFlags::MORE_COMPONENTS)) break;
			}
			++m_componentCount;	//�Ō�̃C���f�b�N�X���w���Ă���̂ŃC���N�������g
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
		//�O���t�̎��֊s���B
		//����(-1������)�̏ꍇ�A�����O���t��\��
		int16_t numberOfContours;
		//�O���t�̍��W�f�[�^�̍ŏ��l�E�ő�l
		//�O���t�͍�����(xMin, yMin)�A�E���(xMax, yMax)�̒����`�ɓ��ڂ���
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
		//����_��
		const uint32_t pointCount = endPtsOfContours[glyph.numberOfContours - 1] + 1;
		const uint16_t endPtsOfGlyph = endPtsOfContours[glyph.numberOfContours - 1];
		//flags��ǂݍ��݁A����_���Ɠ������v�f�������z��ɓW�J����
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
		//x���W��ǂݍ��݁A����_���Ɠ������v�f�������z��ɓW�J����
		xCoordinates = new int16_t[pointCount];
		int16_t currentX = -glyph.xMin;	//x���W�̍ŏ��l����l(���_)�Ƃ���
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
		//y���W��ǂݍ��݁A����_���Ɠ������v�f�������z��ɓW�J����
		yCoordinates = new int16_t[pointCount];
		int16_t currentY = -glyph.yMin;	//y���W�̍ŏ��l����l(���_)�Ƃ���
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
		//�֊s���Ƃɏ���
		for (uint16_t i = 0; i < glyph.numberOfContours; ++i) {
			//�֊s���\�����鐧��_��
			const uint16_t numPoints = i == 0 ? endPtsOfContours[i] + 1 : endPtsOfContours[i] - endPtsOfContours[i - 1];
			ControlPoint* points = new ControlPoint[numPoints];
			for (uint16_t j = 0; j < numPoints; ++j) {
				points[j].OnCurve = *(currentFlag++) & SimpleGlyphFlags::ON_CURVE_POINT;
				points[j].x = static_cast<int16_t>(xCoordinates[xIndex++] * scale);
				points[j].y = static_cast<int16_t>(yCoordinates[yIndex++] * scale);
			}
			//����_����2��B�X�v���C���Ȑ��𐶐�
			System::BSpline bspline;
			bspline.SetFunction(points, numPoints, 2, System::Math::Max(xMax, yMax), true);
			delete[] points;
			//�R���[�`���ɂ���ĘA�������Ȑ���̓_���擾���ĕ`�悷��B
			//�R���[�`���͍ŏ��̓_�ƍŌ�̓_�͓������W��Ԃ��B
			//�s�N�Z���̃A���t�@�l�ɂ���Ĉȉ��̂悤�ɏ�Ԃ��Ǘ�����B
			//0x00: ���h��, 0xaa: �Ȑ���, 0xff: ���h��
			bool first = true;	//�֊s�̍ŏ��̓_�̏������̂�true
			Point<int32_t> prev;	//���O�̓_���W
			int8_t yFirstSign = 0;	//�֊s���ŏ��߂�y���W���ω���������(1 or -1)
			int8_t yPrevSign = 0;	//���O��y���W���ω���������(1 or -1)
			for (Point<int32_t>& p : bspline.Splines(0, 0, xMax, yMax)) {
				//�O���t���W�͍������_�AImage���W�͍��㌴�_�Ȃ̂ŏ㉺���]
				uint32_t y = yMax - p.y;
				//���W(0, y)�̃s�N�Z���ւ̃|�C���^
				System::Drawing::Pixel* line0 = pixels + (y * (xMax + 1));
				ret.SetPixel(p.x, y, contoursPixel);
				//���݂̓_�̐����E�����̃s�N�Z���̃A���t�@�l�𔽓]����(0x00 <-> 0xff)
				if (first) {
					first = false;
					PixelInverse(line0, p.x, xMax);
				}
				else {
					int8_t ySign = static_cast<int8_t>(System::Math::Sign(static_cast<int64_t>(y) - prev.y));
					if (ySign) {
						PixelInverse(line0, p.x, xMax);
						//y���W�̕ψʕ��������]�����Ƃ��A���O�̓_�ł̃A���t�@�l���]��������
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
			//�֊s�̍Ō�̓_(�ŏ��̓_)�����������͉���̂�����ł��Ȃ��ꍇ�A
			//�Ō�̃A���t�@�l���]��������
			if (yFirstSign && yFirstSign == yPrevSign) {
				System::Drawing::Pixel* line0 = pixels + (prev.y * (xMax + 1));
				PixelInverse(line0, prev.x, xMax);
			}
		}
		//�A���t�@�l0x00�̃s�N�Z�������F(0x000000ff)�ɓh��Ԃ�
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
		//�O���tID�ŃA�N�Z�X�ł���O���t���̔z��B
		//�v�f����[maxp.numGlyphs]
		Glyph* glyphs = nullptr;
	private:
		//MAXP::numGlyphs�̒l��ۑ�����
		uint16_t m_numGlyphs;
	public:
		~GLYF() {
			delete[] glyphs;
			glyphs = nullptr;
			m_numGlyphs = 0;
		}
	public:
		//GLYF�e�[�u���ւ̃I�t�Z�b�g���w�肵�ăt�@�C����ǂݎ��A�\���̂�����������
		//numGlyphs�FMAXP::numGlyphs���w�肷��
		//locaOffsets�FLOCA::offsets���w�肷��(�z��̑傫����MAXP::numGlyphs + 1)
		//maxComponentElements�FMAXP::maxComponentElements���w�肷��
		//hmtx�FHMTX�ւ̎Q�Ƃ��w�肷��(AdvanceWidth�擾�p)
		bool Load(System::IO::FileStream& file, uint32_t offset, const MAXP& maxp, const HMTX& hmtx, const LOCA& loca) noexcept {
			m_numGlyphs = maxp.numGlyphs;
			file.Seek(offset, System::IO::SeekDir::Begin);
			glyphs = new Glyph[m_numGlyphs];
			for (uint16_t i = 0; i < m_numGlyphs; ++i) {
				//loca.offsets���A�����ē����l���Ƃ�Ƃ��A�O���t����`����Ă��Ȃ�
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
	/// CFF1IndexData::GetDICTValue()�֐����Ԃ��\����
	/// key�ɑΉ�����T�^�̒l��ێ�����z��Ɨv�f�����܂�
	/// </summary>
	/// <typeparam name="T">GetDICTValue()�֐��Ɏw�肵���^</typeparam>
	template<typename T>
	struct DICTValue {
		//�v�f����[count]
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
	/// DICTValue�^��DICTOperator�̎�ނ�ǉ������\����
	/// </summary>
	/// <typeparam name="T">�����f�[�^�z��̌^</typeparam>
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
	/// CFF1Index::GetData()�֐����Ԃ��\����
	/// �w�肵���v�f�̐擪���w���|�C���^�ƃo�C�g�����܂�
	/// </summary>
	struct CFF1IndexData {
		//CFF1Index�\���̓��̂���v�f�̐擪���w���|�C���^
		const uint8_t* data = nullptr;
		//data���w���v�f�̃o�C�g��
		const uint32_t size = 0;
	public:
		CFF1IndexData() noexcept = delete;
		CFF1IndexData(const uint8_t* d, uint32_t s) noexcept : data(d), size(s) {}
		CFF1IndexData(const CFF1IndexData& arg) noexcept : data(arg.data), size(arg.size) {}
	public:
		CFF1IndexData& operator=(const CFF1IndexData&) noexcept = delete;
	private:
		/// <summary>
		/// DICT�`���z��̎w�肵���I�t�Z�b�g�ʒu����Number�^��ǂݎ��A�w�肵���Q�ƕϐ��ɑ������
		/// </summary>
		/// <typeparam name="T">�Q�ƕϐ��̌^</typeparam>
		/// <param name="variant">�ǂݎ����Number�^��������Q�ƕϐ�</param>
		/// <param name="data">DICT�`���z��ւ̃|�C���^</param>
		/// <param name="start">DICT�`���z��̃I�t�Z�b�g�l</param>
		/// <param name="opr"></param>
		/// <returns>
		/// ���̃I�t�Z�b�g�l�B
		/// data[opr]�̉��Z�q�Ɋւ�邷�ׂẴf�[�^��ǂݎ�����Ƃ��A�w�肵��opr�ƈ�v����B
		/// Number�^��ǂݎ��Ȃ������ꍇ�A0xffffffffu��Ԃ�
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
						//Value�����
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
					//Value��2��
					case DICTOperator::Private:
					{
						ret.count = 2;
						ret.data = new T[2];
						uint32_t next = LoadNumber(ret.data[0], data, s, i);
						LoadNumber(ret.data[1], data, next, i);
						break;
					}
					//Value��3��
					case DICTOperator::ROS:
					{
						ret.count = 3;
						ret.data = new T[3];
						uint32_t next = LoadNumber(ret.data[0], data, s, i);
						next = LoadNumber(ret.data[1], data, next, i);
						LoadNumber(ret.data[2], data, next, i);
						break;
					}
					//Value���z��
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
		/// DICT�f�[�^����w�肵��DICTOperator�ɑΉ�����l���������Ď擾����
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="key">DICTOperator</param>
		/// <returns>
		/// �l�̔z�������DICTValue�\���́B
		/// �l�����݂��Ȃ��ꍇ�A�v�f��0��DICTValue�\���̂�Ԃ�
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
						//Value�����
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
						//Value��2��
						case DICTOperator::Private:
						{
							ret.count = 2;
							ret.data = new T[2];
							uint32_t next = LoadNumber(ret.data[0], data, s, i);
							LoadNumber(ret.data[1], data, next, i);
							return ret;
						}
						//Value��3��
						case DICTOperator::ROS:
						{
							ret.count = 3;
							ret.data = new T[3];
							uint32_t next = LoadNumber(ret.data[0], data, s, i);
							next = LoadNumber(ret.data[1], data, next, i);
							LoadNumber(ret.data[2], data, next, i);
							return ret;
						}
						//Value���z��
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
	/// Index�`����\���\����
	/// </summary>
	struct CFF1Index {
		//�L���ȃI�t�Z�b�g�z��̗v�f��
		uint16_t count = 0;
		//�I�t�Z�b�g�z��̈�v�f�̃o�C�g��
		uint8_t offSize = 0;
		//�I�t�Z�b�g�z��B�v�f����[count + 1]�B
		//�Ō�̗v�f����1�����������o�C�g�z��̃o�C�g��
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
		//�I�t�Z�b�g�z��̒l���g�p���ăA�N�Z�X����f�[�^�z��B
		//�o�C�g����[offsetX[count] - 1]�B
		//n = [0, count - 1]�Ԗڂ̗v�f�ɂ́A
		//for (i = offsetX[n] - 1; i < offsetX[n + 1] - 1; ++i) data[i];
		//�ŃA�N�Z�X����
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
		/// i�Ԗڂ̗v�f��\��CFF1IndexData���擾����
		/// </summary>
		/// <param name="i">�v�f�̃C���f�b�N�X�B�͈͂�[0, count - 1]</param>
		/// <returns>CFF1Index��DICT�`��(TopDICT, FDArray)�̏ꍇ�ACFF1IndexData��DICT�f�[�^�̔z���\��</returns>
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
	/// �T�u���[�`����Index�`����\���\����
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
		/// �T�u���[�`���ԍ��ɑΉ�����T�u���[�`���f�[�^���擾����
		/// </summary>
		/// <param name="subr">Type2Charstring���ɏo������T�u���[�`���ԍ����w�肷��</param>
		/// <param name="charstringType">TopDICT����擾�ł���CharstringType���w�肷��</param>
		CFF1IndexData GetSubr(int16_t subr) const noexcept {
			uint16_t biasedIndex = static_cast<uint16_t>(subr + (m_charstringType == 1 ? 0 : count < 1240 ? 107 : count < 33900 ? 1131 : 32768));
			return CFF1Index::GetData(biasedIndex);
		}
	};

	/// <summary>
	/// StringIndex�`����\���\���́B
	/// SID(StringID):391�ȍ~�ɑΉ����镶���񂪊i�[����Ă���
	/// </summary>
	struct StringIndex : public CFF1Index {
		~StringIndex() = default;
	private:
		using CFF1Index::GetData;
	public:
		using CFF1Index::Load;
	public:
		/// <summary>
		/// SID�ɑΉ�������������擾����B
		/// </summary>
		/// <param name="SID">String ID</param>
		/// <returns>SID��391��菬�����l�⑶�݂��Ȃ��l���w�肵���ꍇ�A�󕶎����Ԃ�</returns>
		String GetString(uint16_t SID) const noexcept {
			int16_t i = static_cast<int32_t>(SID) - 391;
			CFF1IndexData stringData = GetData(i);
			return String(reinterpret_cast<const char*>(stringData.data), stringData.size);
		}
	};

	/// <summary>
	/// PrivateDICT�`����\���\����
	/// </summary>
	struct PrivateDICT {
		//�f�[�^��\���o�C�i���z��B�o�C�g����[size]
		uint8_t* data = nullptr;
		//data�̃o�C�g��
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
		/// PrivateDICT��\���f�[�^���擾����
		/// </summary>
		CFF1IndexData GetData() const noexcept { return CFF1IndexData(data, size); }
	};

	/// <summary>
	/// FDSelect�`����\���\����
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
		/// GID�ɑΉ�����FontDICT�̃C���f�b�N�X���擾����
		/// </summary>
		/// <param name="gid">GID</param>
		/// <returns>FDSelect������ɓǂݍ��܂�Ă��Ȃ��Ƃ��A0��Ԃ�</returns>
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
	/// CFF1�e�[�u���̃w�b�_�[��\���\����
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
	/// CFF1Encoding, CFF1Charset�Ŏg�p�����\����
	/// </summary>
	struct CFF1Range1 {
		uint8_t first;
		uint8_t nLeft;
	};

	/// <summary>
	/// CFF1Charset�Ŏg�p�����\����
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
			//[i] : (range1[i].nLeft + 1)�̍��v��charString.count�ƈ�v�����Ƃ���i
			CFF1Range1* range1;
			//format == 2
			//[i] : range1�Q��
			CFF1Range2* range2;
		};
	private:
		//glyph, range1, range2�̗v�f����ێ�����
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
	/// Type2 Charstring�f�[�^����͂��O���t�f�[�^���擾����N���X
	/// </summary>
	class Type2CharstringParser {
		const uint8_t* data = nullptr;	//Type2Charstring�`���̃f�[�^
		uint32_t size = 0;	//data�̃o�C�g��
		const CFF1SubrsIndex& local;	//LocalSubrsIndex�ւ̎Q��
		const CFF1SubrsIndex& global;	//GlobalSubrsIndex�ւ̎Q��
	private:
		int16_t args[48] = {};	//��͎��Ɏg�p����X�^�b�N�̈�
		uint8_t argCount = 0;	//���݂̗L����args�̗v�f��
	private:
		//normalWidth�Ǝ��ۂ̃O���t���̍����l�B
		//PrivateDICT����擾�ł���DefaultWidth��hmtx�e�[�u����AdvanceWidth���������ꍇ�A
		//��͑O��0��ݒ肷��K�v������B
		//�ݒ肳��Ă��Ȃ��ꍇ�AType2Charstring�̍ŏ��̒l�����̒l�Ƃ��ĔF������
		Traits::InitializedVariant<int16_t> widthDelta;
		uint16_t normalWidth = 0;	//PrivateDICT����擾����NormalWidth(�O���t�̊��)
	private:
		int16_t* hStem = nullptr;	//�����X�e���q���g�̔z��
		uint8_t hStemCountX2 = 0;	//hStem�̗v�f��
		int16_t* vStem = nullptr;	//�����X�e���q���g�̔z��
		uint8_t vStemCountX2 = 0;	//vStem�̗v�f��
		uint8_t currentHintMask[12] = {};	//��͒��Ɏg�p���錻�݂̃q���g�}�X�N
		uint8_t currentCntrMask[12] = {};	//��͒��Ɏg�p���錻�݂̃R���g���[���}�X�N
	private:
		int16_t xMin = System::MAX_VALUE<int16_t>;	//��͂����O���t�̍ŏ�x���W
		int16_t yMin = System::MAX_VALUE<int16_t>;	//��͂����O���t�̍ŏ�y���W
		int16_t xMax = System::MIN_VALUE<int16_t>;	//��͂����O���t�̍ő�x���W
		int16_t yMax = System::MIN_VALUE<int16_t>;	//��͂����O���t�̍ő�y���W
		Vector<BSpline*> bsplines;	//��͂����O���t�̗֊s���Ƃ�B�X�v���C���Ȑ����
		Vector<ControlPoint> points;	//��͒��Ɏg�p���錻�݂̗֊s�̐���_��ێ�����z��
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
		//Charstring�`���̃o�C�i���f�[�^data[start]���琔�l����ǂݎ��Afixed32_c�^����number�ɑ������
		//opr��data[opr]��Typw2Charstring���Z�q��1�o�C�g�ڂ��w���悤�ɐݒ肵�Astart >= opr�̏ꍇ�Anumber��0���߂�l��start
		//�߂�l�F���̃f�[�^�̎n�_���w���C���f�b�N�X�l
		//(�߂�l��opr�ƈ�v����ꍇ�Adata[opr]�̉��Z�q�Ɋւ��f�[�^�����ׂēǂݎ�������Ƃ�����)
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
		//PrivateDICT�̔z��B
		//�v�f����topDict��Private�����݂���ꍇ��[1]�A����ȊO�̏ꍇ��[fdArray.count]
		PrivateDICT* privateDict = nullptr;
	private:
		//charString�Ɋ܂܂��O���t����ێ�����B
		//charString.count��0�̏ꍇ�AMAXP::numGlyphs��ێ�����
		uint16_t m_numGlyphs = 0;
		//privateDict�̗v�f����ێ�����
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
				bool first = true;	//�֊s�̍ŏ��̓_�̏������̂�true
				Point<int32_t> prev;	//���O�̓_���W
				int8_t yFirstSign = 0;	//�֊s���ŏ��߂�y���W���ω���������(1 or -1)
				int8_t yPrevSign = 0;	//���O��y���W���ω���������(1 or -1)
				for (Point<int32_t> p : bspline->Splines(xMin, yMin, parser.GetXMax(), parser.GetYMax())) {
					uint32_t x = static_cast<uint32_t>((p.x - xMin) * scale);
					//�O���t���W�͍������_�AImage���W�͍��㌴�_�Ȃ̂ŏ㉺���]
					uint32_t y = imageYMax - static_cast<uint32_t>((p.y - yMin) * scale);
					//���W(0, y)�̃s�N�Z���ւ̃|�C���^
					System::Drawing::Pixel* line0 = pixels + (y * (imageXMax + 1));
					image.SetPixel(x, y, contoursPixel);
					//���݂̓_�̐����E�����̃s�N�Z���̃A���t�@�l�𔽓]����(0x00 <-> 0xff)
					if (first) {
						first = false;
						System::PixelInverse(line0, x, imageXMax);
					}
					else {
						int8_t ySign = static_cast<int8_t>(System::Math::Sign(static_cast<int64_t>(y) - prev.y));
						if (ySign) {
							System::PixelInverse(line0, x, imageXMax);
							//y���W�̕ψʕ��������]�����Ƃ��A���O�̓_�ł̃A���t�@�l���]��������
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
				//�֊s�̍Ō�̓_(�ŏ��̓_)�����������͉���̂�����ł��Ȃ��ꍇ�A
				//�Ō�̃A���t�@�l���]��������
				if (yFirstSign && yFirstSign == yPrevSign) {
					System::Drawing::Pixel* line0 = pixels + (prev.y * (imageXMax + 1));
					System::PixelInverse(line0, prev.x, imageXMax);
				}
			}
			//�A���t�@�l0x00�̃s�N�Z�������F(0x000000ff)�ɓh��Ԃ�
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
		/// �w�肵���O���t�摜���擾����
		/// </summary>
		/// <param name="gid">�O���tID</param>
		/// <param name="scale">�`��X�P�[��</param>
		/// <param name="advancedWidth">hmtx�e�[�u������擾����O���tID�ɑΉ�����O�i��</param>
		System::Drawing::Image GetGlyphImageByGID(uint16_t gid, float scale, uint16_t advanceWidth, const System::Drawing::Pixel& contoursPixel) const noexcept {
			if (gid < m_numGlyphs) {
				Type2CharstringParser* parser = GetParser(gid, advanceWidth);
				if (parser) return GetGlyph_Internal(*parser, scale, contoursPixel);
			}
			return System::Drawing::Image();
		}
	};
}
