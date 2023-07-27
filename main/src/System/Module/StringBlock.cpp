module StringBlock;
import Vector;
import Encoding;
using namespace System::Traits;

//ToStringBlock IMPL
namespace System::Encoding {
	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf8������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char8_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char8_t> ToU8StringBlock(const CodePoint* codePoints, size_t count) noexcept {
		if (!codePoints || count == 0) return StringBlock<char8_t>();
		Vector<char8_t> chars;
		for (size_t i = 0; i < count; ++i) {
			CodePoint current = codePoints[i];
			if (current.codePage != CodePageFlag::Unicode) current = ToUnicode(current);
			//1�o�C�g
			if (current.point <= 0x7fu) {
				chars.Add(static_cast<char8_t>(current.point));
			}
			//2�o�C�g
			else if (current.point <= 0x7ffu) {
				chars.AddRange(
					static_cast<char8_t>(0xc0u | ((BITMASK_M<7, 11> &current.point) >> 6)),
					static_cast<char8_t>(0x80u | (BITMASK_M<1, 6> &current.point))
				);
			}
			//3�o�C�g
			else if (current.point <= 0xffffu) {
				chars.AddRange(
					static_cast<char8_t>(0xe0u | ((BITMASK_M<13, 16> &current.point) >> 12)),
					static_cast<char8_t>(0x80u | ((BITMASK_M<7, 12> &current.point) >> 6)),
					static_cast<char8_t>(0x80u | (BITMASK_M<1, 6> &current.point))
				);
			}
			//4�o�C�g
			else {
				chars.AddRange(
					static_cast<char8_t>(0xf0u | ((BITMASK_M<19, 21> &current.point) >> 18)),
					static_cast<char8_t>(0x80u | ((BITMASK_M<13, 18> &current.point) >> 12)),
					static_cast<char8_t>(0x80u | ((BITMASK_M<7, 12> &current.point) >> 6)),
					static_cast<char8_t>(0x80u | (BITMASK_M<1, 6> &current.point))
				);
			}
			if (current.point == 0) break;
		}
		if (chars.Count() == 0 || chars[chars.Count() - 1] != u8'\0') chars.Add(u8'\0');
		return StringBlock<char8_t>(chars.Items());
	}

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf16������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char16_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char16_t> ToU16StringBlock(const CodePoint* codePoints, size_t count) noexcept {
		if (!codePoints || count == 0) return StringBlock<char16_t>();
		Vector<char16_t> chars;
		for (size_t i = 0; i < count; ++i) {
			CodePoint current = codePoints[i];
			if (current.codePage != CodePageFlag::Unicode) current = ToUnicode(current);
			//BMP�̈�(2�o�C�g)
			if (current.point <= 0xd7ff || (0xe000 <= current.point && current.point <= 0xffff)) chars.Add(static_cast<char16_t>(current.point));
			//�T���Q�[�g�y�A(4�o�C�g)
			else {
				chars.AddRange(
					static_cast<char16_t>(0xd800 | ((((current.point & BITMASK_M<17, 21>) >> 16) - 1) << 6) | ((current.point & BITMASK_M<11, 16>) >> 10)),
					static_cast<char16_t>(0xdc00 | (current.point & BITMASK_M<1, 10>))
				);
			}
			if (current.point == 0) break;
		}
		if (chars.Count() == 0 || chars[chars.Count() - 1] != u'\0') chars.Add(u'\0');
		return StringBlock<char16_t>(chars.Items());
	}

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf32������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char32_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char32_t> ToU32StringBlock(const CodePoint* codePoints, size_t count) noexcept {
		if (!codePoints || count == 0) return StringBlock<char32_t>();
		Vector<char32_t> chars;
		for (size_t i = 0; i < count; ++i) {
			CodePoint current = codePoints[i];
			if (current.codePage != CodePageFlag::Unicode) current = ToUnicode(current);
			chars.Add(current.point);
			if (current.point == 0) break;
		}
		if (chars.Count() == 0 || chars[chars.Count() - 1] != U'\0') chars.Add(U'\0');
		return StringBlock<char32_t>(chars.Items());
	}

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂烏�C�h������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>wchar_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<wchar_t> ToWideStringBlock(const CodePoint* codePoints, size_t count) noexcept {
		if (!codePoints || count == 0) return StringBlock<wchar_t>();
		Vector<wchar_t> chars;
		for (size_t i = 0; i < count; ++i) {
			CodePoint current = codePoints[i];
			if (current.codePage != CodePageFlag::Unicode) current = ToUnicode(current);
			if constexpr (sizeof(wchar_t) == 2) {
				//BMP�̈�(2�o�C�g)
				if (current.point <= 0xd7ff || (0xe000 <= current.point && current.point <= 0xffff)) chars.Add(static_cast<wchar_t>(current.point));
				//�T���Q�[�g�y�A(4�o�C�g)
				else {
					chars.AddRange(
						static_cast<wchar_t>(0xd800 | ((((current.point & BITMASK_M<17, 21>) >> 16) - 1) << 6) | ((current.point & BITMASK_M<11, 16>) >> 10)),
						static_cast<wchar_t>(0xdc00 | (current.point & BITMASK_M<1, 10>))
					);
				}
			}
			else if constexpr (sizeof(wchar_t) == 4) chars.Add(current.point);
			if (current.point == 0) break;
		}
		if (chars.Count() == 0 || chars[chars.Count() - 1] != L'\0') chars.Add(L'\0');
		return StringBlock<wchar_t>(chars.Items());
	}

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�}���`�o�C�g������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char> ToMultiByteStringBlock(const CodePoint* codePoints, size_t count) noexcept {
		if (!codePoints || count == 0) return StringBlock<char>();
		Vector<char> chars;
		for (size_t i = 0; i < count; ++i) {
			CodePoint current = codePoints[i];
			if (current.codePage == CodePageFlag::Unicode) current = ToMultiByte(current);
			Vector<char> tmp = GetMultiByteChar(current);
			for (size_t i = 0, end = tmp.Count(); i < end; ++i) chars.Add(tmp[i]);
			if (current.point == 0) break;
		}
		if (chars.Count() == 0 || chars[chars.Count() - 1] != '\0') chars.Add('\0');
		return StringBlock<char>(chars.Items());
	}
}
