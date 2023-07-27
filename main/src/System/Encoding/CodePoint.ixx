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
	/// char�^�̃R�[�h�y�[�W���w�肷��\���́BGetCodePoint&lt;char&gt;()�Ŏg�p�����
	/// </summary>
	struct charset {
		static CodePageFlag char_CodePage;
	};
	CodePageFlag charset::char_CodePage = CodePageFlag::Shift_JIS;
}

//charset
export namespace System::Encoding {
	/// <summary>
	/// char�^�̃R�[�h�y�[�W���w�肷��B
	/// GetCodePoint()�֐��ɂ����āAchar�^�������
	/// �����Ŏw�肵���R�[�h�y�[�W�ŃG���R�[�f�B���O����Ă���Ɖ��߂����
	/// </summary>
	/// <param name="flag">�w�肷��R�[�h�y�[�W��\���񋓒l</param>
	void SetCharset(CodePageFlag flag) noexcept { charset::char_CodePage = flag; }
}

//CodePoint
export namespace System::Encoding {
	/// <summary>
	/// GetCodePoint�֐����Ԃ��\����
	/// </summary>
	struct CodePoint {
		//codePage�Ŏw�肳�ꂽ�����G���R�[�h�̕����ʒu
		uint32_t point = 0;
		//GetCodePoint�֐����g�p�����v�f��
		uint8_t count = 0;
		//GetCodePoint�֐��̃e���v���[�g������wchar, char8_t, char16_t, char32_t�̏ꍇ�AUnicode�B
		//char�̏ꍇ�AUnicode��Shift_JIS(CP932)
		CodePageFlag codePage = CodePageFlag::Unicode;
	public:
		/// <summary>
		/// �R�[�h�|�C���g��UVS(Unicode Variation Selector)�����ׂ�
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
			//sizeof(wchar_t) == 2�̏ꍇ�AUTF16�Ƃ���
			if constexpr (sizeof(wchar_t) == 2) {
				//�T���Q�[�g�y�A(4�o�C�g����)
				if ((str[0] & BITMASK_M<11, 15>) == 0xd800u) {
					ret.count = 2;
					//0b110111xxxxxxxxxx�̓��[�T���Q�[�g�A0b110110xxxxxxxxxx�̓n�C�T���Q�[�g
					if (str[0] & 0xdc00u) ret.point = (str[0] & 0x03ffu) | ((str[1] & 0x03ffu) << 10u);
					else ret.point = (str[1] & 0x03ffu) | ((str[0] & 0x03ffu) << 10u);
				}
				//2�o�C�g����
				else {
					ret.count = 1;
					ret.point = static_cast<uint32_t>(str[0]);
				}
			}
			//sizeof(wchar_t) == 4�̏ꍇ�AUTF32�Ƃ���
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
			//�T���Q�[�g�y�A(4�o�C�g����)
			if ((str[0] & BITMASK_M<11, 15>) == 0xd800u) {
				ret.count = 2;
				//0b110111xxxxxxxxxx�̓��[�T���Q�[�g�A0b110110xxxxxxxxxx�̓n�C�T���Q�[�g
				if (str[0] & 0xdc00u) ret.point = (str[0] & 0x03ffu) | ((str[1] & 0x03ffu) << 10u);
				else ret.point = (str[1] & 0x03ffu) | ((str[0] & 0x03ffu) << 10u);
			}
			//2�o�C�g����
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
