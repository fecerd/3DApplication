module;
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module CodePage;
import CSTDINT;
import Traits;
import Vector;
import CodePoint;
import CP932;
using namespace System::Traits;

namespace System::Encoding {
	class CodePage {
		/// <summary>
		/// �R�[�h�y�[�W�t�@�C�����ʎq�BCP_2�̂Ƃ��A2�o�C�g�����BCP_4�̂Ƃ��A4�o�C�g����
		/// </summary>
		uint8_t magic[4] = {};
		/// <summary>
		/// �R�[�h�y�[�W�ԍ�
		/// </summary>
		uint16_t codePageNumber = 0;
		/// <summary>
		/// �v�f��
		/// </summary>
		uint32_t count = 0;
		/// <summary>
		/// �ϊ��O�̕����ʒu�̔z��
		/// </summary>
		union {
			uint16_t* u16 = nullptr;
			uint32_t* u32;
		} src;
		/// <summary>
		/// �ϊ���̕����ʒu�̔z��
		/// </summary>
		union {
			uint16_t* u16 = nullptr;
			uint32_t* u32;
		} dst;
	public:
		CodePage() = delete;
		CodePage(const CodePage&) = delete;
		CodePage(const uint8_t* binary_data, size_t N) noexcept {
			size_t pos = 0;
			for (size_t i = 0; i < 4; ++i, ++pos) magic[i] = binary_data[pos];
			codePageNumber = static_cast<uint16_t>(binary_data[pos]) | (static_cast<uint16_t>(binary_data[pos + 1]) << 8);
			pos += 2;
			for (size_t i = 0; i < 4; ++i, ++pos) count |= static_cast<uint32_t>(binary_data[pos]) << (i * 8);
			if (magic[3] == u8'2') {
				src.u16 = new uint16_t[count];
				for (uint32_t i = 0; i < count; ++i, pos += 2) src.u16[i] = static_cast<uint16_t>(binary_data[pos]) | (static_cast<uint16_t>(binary_data[pos + 1]) << 8);
				dst.u16 = new uint16_t[count];
				for (uint32_t i = 0; i < count; ++i, pos += 2) dst.u16[i] = static_cast<uint16_t>(binary_data[pos]) | (static_cast<uint16_t>(binary_data[pos + 1]) << 8);
			}
			else if (magic[3] == u8'4') {
				src.u32 = new uint32_t[count]{};
				for (uint32_t i = 0; i < count; ++i) {
					for (size_t j = 0; j < 4; ++j, ++pos) src.u32[i] |= static_cast<uint32_t>(binary_data[pos]) << (j * 8);
				}
				dst.u32 = new uint32_t[count]{};
				for (uint32_t i = 0; i < count; ++i) {
					for (size_t j = 0; j < 4; ++j, ++pos) dst.u32[i] |= static_cast<uint32_t>(binary_data[pos]) << (j * 8);
				}
			}
		}
		~CodePage() {
			if (magic[3] == u8'2') {
				delete[] src.u16;
				delete[] dst.u16;
			}
			else if (magic[3] == u8'4') {
				delete[] src.u32;
				delete[] dst.u32;
			}
			src.u16 = nullptr;
			dst.u16 = nullptr;
		}
	public:
		/// <summary>
		/// �R�[�h�y�[�W�t�@�C�����ǂݍ��܂�Ă��邩���ׂ�
		/// </summary>
		/// <returns>�ǂݍ��܂�Ă���ꍇ�Atrue</returns>
		bool isLoaded() const noexcept { return src.u16 && dst.u16; }
	public:
		/// <summary>
		/// src�Ɠ����`���̕����ʒucodePoint��dst�Ɠ����`���̕����ʒu�ɃG���R�[�h���ĕԂ��B
		/// </summary>
		/// <param name="codePoint">�ϊ����镄���ʒu</param>
		/// <returns>�ϊ���̕����ʒu�B�ϊ������݂��Ȃ��ꍇ�A0</returns>
		uint32_t SrcToDst(const uint32_t codePoint) noexcept {
			if (magic[3] == u8'2') {
				uint16_t tCodePoint = static_cast<uint16_t>(codePoint);
				for (uint32_t i = 0; i < count; ++i) if (src.u16[i] == tCodePoint) return static_cast<uint32_t>(dst.u16[i]);
			}
			else if (magic[3] == u8'4') {
				for (uint32_t i = 0; i < count; ++i) if (src.u32[i] == codePoint) return dst.u32[i];
			}
			return 0;
		}
		/// <summary>
		/// dst�Ɠ����`���̕����ʒucodePoint��src�Ɠ����`���̕����ʒu�ɃG���R�[�h���ĕԂ�
		/// </summary>
		/// <param name="codePoint">�ϊ����镄���ʒu</param>
		/// <returns>�ϊ���̕����ʒu�B�ϊ������݂��Ȃ��ꍇ�A0</returns>
		uint32_t DstToSrc(const uint32_t codePoint) noexcept {
			if (magic[3] == u8'2') {
				uint16_t tCodePoint = static_cast<uint16_t>(codePoint);
				for (uint32_t i = 0; i < count; ++i) if (dst.u16[i] == tCodePoint) return static_cast<uint32_t>(src.u16[i]);
			}
			else if (magic[3] == u8'4') {
				for (uint32_t i = 0; i < count; ++i) if (dst.u32[i] == codePoint) return src.u32[i];
			}
			return 0;
		}
	public:
		CodePage& operator=(const CodePage&) noexcept = delete;
		CodePage& operator=(CodePage&& rhs) noexcept {
			if (this == &rhs) return *this;
			if (magic[3] == u8'2') {
				delete[] src.u16;
				delete[] dst.u16;
			}
			else if (magic[3] == u8'4') {
				delete[] src.u32;
				delete[] dst.u32;
			}
			magic[0] = rhs.magic[0];
			magic[1] = rhs.magic[1];
			magic[2] = rhs.magic[2];
			magic[3] = rhs.magic[3];
			codePageNumber = rhs.codePageNumber;
			count = rhs.count;
			src.u32 = rhs.src.u32;
			dst.u32 = rhs.dst.u32;

			rhs.magic[0] = 0;
			rhs.magic[1] = 0;
			rhs.magic[2] = 0;
			rhs.magic[3] = 0;
			rhs.codePageNumber = 0;
			rhs.count = 0;
			rhs.src.u32 = nullptr;
			rhs.dst.u32 = nullptr;
			return *this;
		}
	public:
		static CodePage& CP932() noexcept {
			_CrtSetDbgFlag(0);
			static CodePage* cp932 = new CodePage(Binary::CP932_bin, sizeof(Binary::CP932_bin));
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			return *cp932;
		}
	};
}

export namespace System::Encoding {
	/// <summary>
	/// CodePoint�\���̂�Unicode�ɕϊ�����
	/// </summary>
	/// <param name="src">�ϊ�����CodePoint�\����</param>
	/// <returns>Unicode�����ʒu�������A������1��CodePoint�\����</returns>
	CodePoint ToUnicode(CodePoint src) noexcept {
		if (src.codePage == CodePageFlag::Unicode) {
			src.count = 1;
			return src;
		}
		src.codePage = CodePageFlag::Unicode;
		src.point = CodePage::CP932().SrcToDst(src.point);
		src.count = 1;
		return src;
	}

	/// <summary>
	/// CodePoint�\���̂�Shift_JIS(CP932)�ɕϊ�����
	/// </summary>
	/// <param name="src">�ϊ�����CodePoint�\����</param>
	/// <returns>Shift_JIS�����ʒu������CodePoint�\���́B�������͕����ʒu��0xff�ȉ��̂Ƃ��A1�B����ȊO�̂Ƃ��A2</returns>
	CodePoint ToCP932(CodePoint src) noexcept {
		if (src.codePage == CodePageFlag::Shift_JIS) return src;
		src.codePage = CodePageFlag::Shift_JIS;
		src.point = CodePage::CP932().DstToSrc(src.point);
		src.count = src.point <= 0xffu ? 1 : 2;
		return src;
	}

	/// <summary>
	/// CodePoint�\���̂��}���`�o�C�g�����R�[�h�ɕϊ�����
	/// </summary>
	/// <param name="src">�ϊ�����CodePoint�\����</param>
	/// <returns>charset::char_CodePage�Ŏw�肳��Ă���R�[�h�y�[�W��CodePoint�\���́B</returns>
	CodePoint ToMultiByte(CodePoint src) noexcept {
		if (charset::char_CodePage == CodePageFlag::Shift_JIS) return ToCP932(src);
		else if (charset::char_CodePage == CodePageFlag::Unicode) return ToUnicode(src);
		else return src;
	}

	/// <summary>
	/// CodePoint�\���̂���Shift_JIS�ɃG���R�[�h���ꂽ�������擾����
	/// </summary>
	/// <param name="src">����������Shift_JIS�����ʒu</param>
	/// <returns>Shift_JIS�ɃG���R�[�h���ꂽchar�^�����̔z��B�G���[�̂Ƃ��A�傫��0�̔z���Ԃ��A����ȊO�̏ꍇ�A�傫��1�܂���2�̔z���Ԃ�</returns>
	Vector<char> GetCP932Char(CodePoint src) noexcept {
		if (src.codePage != CodePageFlag::Shift_JIS) return Vector<char>();
		Vector<char> ret;
		uint8_t first = static_cast<uint8_t>((src.point & Traits::BITMASK_M<9, 16>) >> 8);
		if ((0x81u <= first && first <= 0x9fu) || (0xe0u <= first && first <= 0xefu)) ret.AddRange(static_cast<char>(first), static_cast<char>(src.point & Traits::BITMASK_M<1, 8>));
		else ret.Add(static_cast<char>(src.point & Traits::BITMASK_M<1, 8>));
		return ret;
	}

	/// <summary>
	/// CodePoint�\���̂���UTF8�ɃG���R�[�h���ꂽ�������擾����
	/// </summary>
	/// <param name="src">����������Unicode�����ʒu</param>
	/// <returns>UTF8�ɃG���R�[�h���ꂽchar�^�����̔z��B�G���[�̂Ƃ��A�傫��0�̔z���Ԃ��A����ȊO�̏ꍇ�A�傫��1�`4�̔z���Ԃ�</returns>
	Vector<char> GetUTF8Char(CodePoint src) noexcept {
		if (src.codePage != CodePageFlag::Unicode) return Vector<char>();
		Vector<char> ret;
		//1�o�C�g
		if (src.point <= 0x7fu) ret.Add(static_cast<char>(src.point));
		//2�o�C�g
		else if (src.point <= 0x7ffu) {
			ret.AddRange(
				static_cast<char>(0xc0u | ((src.point & Traits::BITMASK_M<7, 11>) >> 6)),
				static_cast<char>(0x80u | (src.point & Traits::BITMASK_M<1, 6>))
			);
		}
		//3�o�C�g
		else if (src.point <= 0xffffu) {
			ret.AddRange(
				static_cast<char>(0xe0u | ((src.point & Traits::BITMASK_M<13, 16>) >> 12)),
				static_cast<char>(0x80u | ((src.point & Traits::BITMASK_M<7, 12>) >> 6)),
				static_cast<char>(0x80u | (src.point & Traits::BITMASK_M<1, 6>))
			);
		}
		//4�o�C�g
		else {
			ret.AddRange(
				static_cast<char>(0xf0u | ((src.point & Traits::BITMASK_M<19, 21>) >> 18)),
				static_cast<char>(0x80u | ((src.point & Traits::BITMASK_M<13, 18>) >> 12)),
				static_cast<char>(0x80u | ((src.point & Traits::BITMASK_M<7, 12>) >> 6)),
				static_cast<char>(0x80u | (src.point & Traits::BITMASK_M<1, 6>))
			);
		}
		return ret;
	}

	/// <summary>
	/// CodePoint�\���̂���}���`�o�C�g�������擾����
	/// </summary>
	/// <param name="src">���������镄���ʒu���</param>
	/// <returns>charset::char_CodePage�Ŏw�肳��Ă��镶���R�[�h�ɃG���R�[�h���ꂽchar�^�����̔z��B�G���[�̂Ƃ��̂݁A�傫��0�̔z���Ԃ�</returns>
	Vector<char> GetMultiByteChar(CodePoint src) noexcept {
		if (charset::char_CodePage == CodePageFlag::Shift_JIS) return GetCP932Char(src);
		else if (charset::char_CodePage == CodePageFlag::Unicode) return GetUTF8Char(src);
		else return Vector<char>();
	}
}