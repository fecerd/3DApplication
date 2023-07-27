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
		/// コードページファイル識別子。CP_2のとき、2バイト文字。CP_4のとき、4バイト文字
		/// </summary>
		uint8_t magic[4] = {};
		/// <summary>
		/// コードページ番号
		/// </summary>
		uint16_t codePageNumber = 0;
		/// <summary>
		/// 要素数
		/// </summary>
		uint32_t count = 0;
		/// <summary>
		/// 変換前の符号位置の配列
		/// </summary>
		union {
			uint16_t* u16 = nullptr;
			uint32_t* u32;
		} src;
		/// <summary>
		/// 変換後の符号位置の配列
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
		/// コードページファイルが読み込まれているか調べる
		/// </summary>
		/// <returns>読み込まれている場合、true</returns>
		bool isLoaded() const noexcept { return src.u16 && dst.u16; }
	public:
		/// <summary>
		/// srcと同じ形式の符号位置codePointをdstと同じ形式の符号位置にエンコードして返す。
		/// </summary>
		/// <param name="codePoint">変換する符号位置</param>
		/// <returns>変換後の符号位置。変換が存在しない場合、0</returns>
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
		/// dstと同じ形式の符号位置codePointをsrcと同じ形式の符号位置にエンコードして返す
		/// </summary>
		/// <param name="codePoint">変換する符号位置</param>
		/// <returns>変換後の符号位置。変換が存在しない場合、0</returns>
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
	/// CodePoint構造体をUnicodeに変換する
	/// </summary>
	/// <param name="src">変換するCodePoint構造体</param>
	/// <returns>Unicode符号位置情報を持つ、文字数1のCodePoint構造体</returns>
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
	/// CodePoint構造体をShift_JIS(CP932)に変換する
	/// </summary>
	/// <param name="src">変換するCodePoint構造体</param>
	/// <returns>Shift_JIS符号位置を持つCodePoint構造体。文字数は符号位置が0xff以下のとき、1。それ以外のとき、2</returns>
	CodePoint ToCP932(CodePoint src) noexcept {
		if (src.codePage == CodePageFlag::Shift_JIS) return src;
		src.codePage = CodePageFlag::Shift_JIS;
		src.point = CodePage::CP932().DstToSrc(src.point);
		src.count = src.point <= 0xffu ? 1 : 2;
		return src;
	}

	/// <summary>
	/// CodePoint構造体をマルチバイト文字コードに変換する
	/// </summary>
	/// <param name="src">変換するCodePoint構造体</param>
	/// <returns>charset::char_CodePageで指定されているコードページのCodePoint構造体。</returns>
	CodePoint ToMultiByte(CodePoint src) noexcept {
		if (charset::char_CodePage == CodePageFlag::Shift_JIS) return ToCP932(src);
		else if (charset::char_CodePage == CodePageFlag::Unicode) return ToUnicode(src);
		else return src;
	}

	/// <summary>
	/// CodePoint構造体からShift_JISにエンコードされた文字を取得する
	/// </summary>
	/// <param name="src">文字化するShift_JIS符号位置</param>
	/// <returns>Shift_JISにエンコードされたchar型文字の配列。エラーのとき、大きさ0の配列を返し、それ以外の場合、大きさ1または2の配列を返す</returns>
	Vector<char> GetCP932Char(CodePoint src) noexcept {
		if (src.codePage != CodePageFlag::Shift_JIS) return Vector<char>();
		Vector<char> ret;
		uint8_t first = static_cast<uint8_t>((src.point & Traits::BITMASK_M<9, 16>) >> 8);
		if ((0x81u <= first && first <= 0x9fu) || (0xe0u <= first && first <= 0xefu)) ret.AddRange(static_cast<char>(first), static_cast<char>(src.point & Traits::BITMASK_M<1, 8>));
		else ret.Add(static_cast<char>(src.point & Traits::BITMASK_M<1, 8>));
		return ret;
	}

	/// <summary>
	/// CodePoint構造体からUTF8にエンコードされた文字を取得する
	/// </summary>
	/// <param name="src">文字化するUnicode符号位置</param>
	/// <returns>UTF8にエンコードされたchar型文字の配列。エラーのとき、大きさ0の配列を返し、それ以外の場合、大きさ1～4の配列を返す</returns>
	Vector<char> GetUTF8Char(CodePoint src) noexcept {
		if (src.codePage != CodePageFlag::Unicode) return Vector<char>();
		Vector<char> ret;
		//1バイト
		if (src.point <= 0x7fu) ret.Add(static_cast<char>(src.point));
		//2バイト
		else if (src.point <= 0x7ffu) {
			ret.AddRange(
				static_cast<char>(0xc0u | ((src.point & Traits::BITMASK_M<7, 11>) >> 6)),
				static_cast<char>(0x80u | (src.point & Traits::BITMASK_M<1, 6>))
			);
		}
		//3バイト
		else if (src.point <= 0xffffu) {
			ret.AddRange(
				static_cast<char>(0xe0u | ((src.point & Traits::BITMASK_M<13, 16>) >> 12)),
				static_cast<char>(0x80u | ((src.point & Traits::BITMASK_M<7, 12>) >> 6)),
				static_cast<char>(0x80u | (src.point & Traits::BITMASK_M<1, 6>))
			);
		}
		//4バイト
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
	/// CodePoint構造体からマルチバイト文字を取得する
	/// </summary>
	/// <param name="src">文字化する符号位置情報</param>
	/// <returns>charset::char_CodePageで指定されている文字コードにエンコードされたchar型文字の配列。エラーのときのみ、大きさ0の配列を返す</returns>
	Vector<char> GetMultiByteChar(CodePoint src) noexcept {
		if (charset::char_CodePage == CodePageFlag::Shift_JIS) return GetCP932Char(src);
		else if (charset::char_CodePage == CodePageFlag::Unicode) return GetUTF8Char(src);
		else return Vector<char>();
	}
}