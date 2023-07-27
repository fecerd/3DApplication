export module CodePoint;
import CSTDINT;
import Traits;
using namespace System;
using namespace System::Traits;

//CodePointFlag
export namespace System::Encoding {
	enum class CodePageFlag {
		Unicode,
		Shift_JIS
	};
}

export namespace System::Encoding {
	/// <summary>
	/// char型のコードページを指定する構造体。GetCodePoint&lt;char&gt;()で使用される
	/// </summary>
	struct charset {
		static CodePageFlag char_CodePage;
	};
	CodePageFlag charset::char_CodePage = CodePageFlag::Shift_JIS;
}

//charset
export namespace System::Encoding {
	/// <summary>
	/// char型のコードページを指定する。
	/// GetCodePoint()関数において、char型文字列は
	/// ここで指定したコードページでエンコーディングされていると解釈される
	/// </summary>
	/// <param name="flag">指定するコードページを表す列挙値</param>
	void SetCharset(CodePageFlag flag) noexcept { charset::char_CodePage = flag; }
}

//CodePoint
export namespace System::Encoding {
	/// <summary>
	/// GetCodePoint関数が返す構造体
	/// </summary>
	struct CodePoint {
		//codePageで指定された文字エンコードの符号位置
		uint32_t point = 0;
		//GetCodePoint関数が使用した要素数
		uint8_t count = 0;
		//GetCodePoint関数のテンプレート引数がwchar, char8_t, char16_t, char32_tの場合、Unicode。
		//charの場合、UnicodeかShift_JIS(CP932)
		CodePageFlag codePage = CodePageFlag::Unicode;
	public:
		/// <summary>
		/// コードポイントがUVS(Unicode Variation Selector)か調べる
		/// </summary>
		bool IsUVS() const noexcept {
			if (codePage != CodePageFlag::Unicode) return false;
			return (0x180b <= point && point <= 0x180d) 
				|| (0xfe00 <= point && point <= 0xfe0f) 
				|| (0xe0100 <= point && point <= 0xe01ef);
		}
	};
}

//GetCodePoint
export namespace System::Encoding {
	CodePoint GetCodePoint_Shift_JIS(const char* str) noexcept {
		CodePoint ret;
		ret.codePage = CodePageFlag::Shift_JIS;
		if (!str) return ret;
		const uint8_t first = static_cast<uint8_t>(str[0]);
		if ((first >= 0x81u && first <= 0x9fu) || (first >= 0xe0u && first <= 0xefu)) {
			ret.point = (static_cast<uint32_t>(first) << 8) | (static_cast<uint32_t>(static_cast<uint8_t>(str[1])));
			ret.count = 2;
		}
		else {
			ret.point = static_cast<uint32_t>(first);
			ret.count = 1;
		}
		return ret;
	}

	template<CharType str_t>
	CodePoint GetCodePoint(const str_t* str) {
		CodePoint ret;
		if (!str) return ret;
		if constexpr (is_same_v<str_t, char>) {
			if (charset::char_CodePage == CodePageFlag::Shift_JIS) return GetCodePoint_Shift_JIS(str);
			else if (charset::char_CodePage == CodePageFlag::Unicode) return GetCodePoint(reinterpret_cast<const char8_t*>(str));
		}
		else if  constexpr (is_same_v<str_t, wchar_t>) {
			ret.codePage = CodePageFlag::Unicode;
			//sizeof(wchar_t) == 2の場合、UTF16とする
			if constexpr (sizeof(wchar_t) == 2) {
				//サロゲートペア(4バイト文字)
				if ((str[0] & BITMASK_M<11, 15>) == 0xd800u) {
					ret.count = 2;
					//0b110111xxxxxxxxxxはローサロゲート、0b110110xxxxxxxxxxはハイサロゲート
					if (str[0] & 0xdc00u) ret.point = (str[0] & 0x03ffu) | ((str[1] & 0x03ffu) << 10u);
					else ret.point = (str[1] & 0x03ffu) | ((str[0] & 0x03ffu) << 10u);
				}
				//2バイト文字
				else {
					ret.count = 1;
					ret.point = static_cast<uint32_t>(str[0]);
				}
			}
			//sizeof(wchar_t) == 4の場合、UTF32とする
			else if constexpr (sizeof(wchar_t) == 4) {
				ret.count = 1;
				ret.point = str[0];
			}
		}
		else if constexpr (is_same_v<str_t, char8_t>) {
			ret.codePage = CodePageFlag::Unicode;
			if (~str[0] & 0x80) ret.count = 1;
			else if (str[0] & 0x40) {
				if (~str[0] & 0x20) ret.count = 2;
				else if (~str[0] & 0x10) ret.count = 3;
				else if (~str[0] & 0x08) ret.count = 4;
				else return ret;
			}
			else return ret;
			if (ret.count == 1) ret.point = static_cast<uint32_t>(str[0]);
			else if (ret.count == 2) ret.point = (static_cast<uint32_t>(str[0] & 0x1f) << 6) + (str[1] & 0x3f);
			else if (ret.count == 3) ret.point = (static_cast<uint32_t>(str[0] & 0x0f) << 12) + (static_cast<uint32_t>(str[1] & 0x3f) << 6) + (str[2] & 0x3f);
			else if (ret.count == 4) ret.point = (static_cast<uint32_t>(str[0] & 0x07) << 18) + (static_cast<uint32_t>(str[1] & 0x3f) << 12) + (static_cast<uint32_t>(str[2] & 0x3f) << 6) + (str[3] & 0x3f);
		}
		else if constexpr (is_same_v<str_t, char16_t>) {
			ret.codePage = CodePageFlag::Unicode;
			//サロゲートペア(4バイト文字)
			if ((str[0] & BITMASK_M<11, 15>) == 0xd800u) {
				ret.count = 2;
				//0b110111xxxxxxxxxxはローサロゲート、0b110110xxxxxxxxxxはハイサロゲート
				if (str[0] & 0xdc00u) ret.point = (str[0] & 0x03ffu) | ((str[1] & 0x03ffu) << 10u);
				else ret.point = (str[1] & 0x03ffu) | ((str[0] & 0x03ffu) << 10u);
			}
			//2バイト文字
			else {
				ret.count = 1;
				ret.point = static_cast<uint32_t>(str[0]);
			}
		}
		else if constexpr (is_same_v<str_t, char32_t>) {
			ret.codePage = CodePageFlag::Unicode;
			ret.count = 1;
			ret.point = str[0];
		}
		return ret;
	}
}
