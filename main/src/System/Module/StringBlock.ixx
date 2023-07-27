module;
#include<crtdbg.h>
export module StringBlock;
import CSTDINT;
import Traits;
import Math;
import VectorBase;
import CodePoint;
using namespace System::Traits;

//CStringBlock
namespace System {
	template<CharType str_t> class StringBlock;

	export template<CharType str_t, size_t N>
	class CStringBlock {
		friend class StringBlock<str_t>;
	protected:
		str_t value[N];
	public:
		static constexpr size_t npos = MAX_VALUE<size_t>;
	public:
		constexpr CStringBlock() : value() {}
		constexpr CStringBlock(const CStringBlock<str_t, N>& arg) noexcept {
			for (size_t i = 0; i < N; ++i) value[i] = arg.value[i];
		}
		explicit constexpr CStringBlock(const str_t c) noexcept : value{ c, '\0'} {}
		constexpr CStringBlock(const str_t(&arg)[N]) noexcept {
			for (size_t i = 0; i < N; ++i) value[i] = arg[i];
			value[N - 1] = '\0';
		}
		constexpr CStringBlock(const str_t* arg, size_t n) noexcept {
			if (n == 0) n = N - 1;
			size_t i = 0;
			if (arg) for (; i < N - 1 && arg[i] != '\0' && i < n; ++i) value[i] = arg[i];
			for (; i <= N - 1; ++i) value[i] = '\0';			
		}
		~CStringBlock() = default;
	public:
		static constexpr auto Empty() noexcept {
			if constexpr (is_same_v<str_t, char>) return CString<str_t, 3>();
			else if constexpr (is_same_v<str_t, char8_t>) return CString<str_t, 5>();
			else if constexpr (is_same_v<str_t, char16_t>) return CString<str_t, 3>();
			else if constexpr (is_same_v<str_t, char32_t>) return CString<str_t, 2>();
			else if constexpr (is_same_v<str_t, wchar_t>) return CString<str_t, 3>();
		}
	public:
		constexpr auto strcpy(const str_t* src, const size_t pos, const size_t n) noexcept {
			for (size_t i = 0; i < n && pos + i < N - 1; ++i) value[pos + i] = src[i];
			return *this;
		}
		constexpr auto strcpy(const str_t src, const size_t pos) noexcept {
			if (pos < N) value[pos] = src;
			return *this;
		}
	public:
		constexpr const str_t* c_str() const noexcept { return value; }
		constexpr size_t Length() const noexcept { for (size_t i = 0; i < N; ++i) if (value[i] == '\0') return i; return 0; }
	public:
		template<size_t n>
		constexpr size_t find(const CStringBlock<str_t, n>& src) const noexcept {
			size_t len = Length(), srcLen = src.Length();
			if (len < srcLen) return npos;
			const str_t* srcData = src.c_str();
			for (size_t i = 0; i < len; ++i) {
				if (value[i] != srcData[0]) continue;
				if (len - i < srcLen) return npos;
				size_t j = 1;
				for (; j < srcLen; ++j) if (value[i + j] != srcData[j]) break;
				if (j == srcLen) return i;
			}
			return npos;
		}
		template<size_t n>
		constexpr size_t rfind(const CStringBlock<str_t, n>& src) const noexcept {
			size_t len = Length(), srcLen = src.Length();
			if (len < srcLen) return npos;
			const str_t* srcData = src.c_str();
			for (size_t i = len, srcEnd = srcLen - 1; i-- > 0;) {
				if (value[i] != srcData[srcEnd]) continue;
				if (i + 1 < srcLen) return npos;
				size_t j = 1;
				for (; j <= srcEnd; ++j) {
					if (value[i - j] != srcData[srcEnd - j]) break;
				}
				if (j > srcEnd) return i - j + 1;
			}
			return npos;
		}
		constexpr CStringBlock<str_t, N> substr(const size_t pos, const size_t n = npos) const noexcept {
			if (pos >= Length()) return CStringBlock<str_t, N>();
			else return CStringBlock<str_t, N>(value + pos, n);
		}
	public:
		constexpr auto operator[](const size_t i) const noexcept { return Length() <= i ? Empty() : CStringBlock(value[i]); }
		template<size_t n>
		constexpr auto operator+(const CStringBlock<str_t, n>& rhs) const noexcept {
			CStringBlock<str_t, N + n - 1> ret;
			const size_t len = Length();
			ret.strcpy(value, 0, len);
			const size_t rlen = rhs.Length();
			ret.strcpy(rhs.c_str(), len, rlen);
			ret.strcpy('\0', len + rlen);
			return ret;
		}
		template<size_t n>
		constexpr bool operator==(const CStringBlock<str_t, n>& rhs) const noexcept {
			size_t len = Length(), rhsLen = rhs.Length();
			if (len != rhsLen) return false;
			const str_t* rdata = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (value[i] != rdata[i]) return false;
			return true;
		}
		template<size_t n> constexpr bool operator!=(const CStringBlock<str_t, n>& rhs) const noexcept { return !(*this == rhs); }
		constexpr operator bool() const noexcept { return value[0] != '\0'; }
	};

	export template<CharType str_t, Integral T>
	constexpr CStringBlock<str_t, 21> GetCStringBlock(T n) noexcept {
		CStringBlock<str_t, 21> ret;
		size_t digit = Math::CountDigit(n);
		size_t i = 0;
		if (n < 0) ret.strcpy('-', i++);
		size_t abs = Math::Abs(n);
		while (digit > 0) {
			ret.strcpy('0' + (abs % 10), digit-- - 1 + i);
			abs /= 10;
		}
		return ret;
	}

	export enum class NormalizeMode : uint8_t {
		Auto,			//�����I��
		Fixed,			//�ϊ����Ȃ�
		Normalize	//�����I�ɕϊ�����
	};

	/// <summary>
	/// ���������_���𕶎���萔�ɕϊ�����
	/// </summary>
	/// <typeparam name="str_t">�ϊ���̕����^</typeparam>
	/// <typeparam name="digit">�L������</typeparam>
	/// <param name="d">���������_��</param>
	/// <param name="mode">�w���\�L(x.xxe-10�Ȃ�)�ϊ����[�h</param>
	/// <returns>
	/// �w�肵���L�������̕�����萔�B
	/// ����mode��Auto�̂Ƃ��A�L��������d�̐������̌��������������A
	/// �������͗L�����������ׂ�0�Ŗ��܂�(0.0001�ɑ΂��ėL������4�ȉ�)�ꍇ�A�w���\�L�ƂȂ�
	/// </returns>
	export template<CharType str_t, size_t digit = 10>
	constexpr auto GetCStringBlock(double d, NormalizeMode mode = NormalizeMode::Auto) noexcept {
		//���e����L�������̑傫��
		//1075�� = 1��(������) + 1074��(������)
		//2^-1074(double�ŕ\�����ԏ�������)�̍ŉ��ʂ̌���10^-1074�̈ʂł��邱�ƂɗR������
		constexpr size_t max_digit = 1075;
		if constexpr (digit == 0) return GetCStringBlock<str_t, 1>(d, mode);	//�L������0����1���Ƃ��Ĉ���
		else if constexpr (digit > max_digit) return GetCStringBlock<str_t, max_digit>(d, mode);	//�L��������max_digit�܂�
		else {
			CStringBlock<str_t, digit + 8> ret;	//�L������+8����(�����_+e+�w������+�w��(�ő�3��)+�k���I�[����)
			if (Math::IsNan(d)) {
				ret.strcpy('n', 0);
				ret.strcpy('a', 1);
				ret.strcpy('n', 2);
			}
			else if (Math::IsInf(d)) {
				size_t pos = 0;
				if (d < 0) ret.strcpy('-', pos++);
				ret.strcpy('i', pos++);
				ret.strcpy('n', pos++);
				ret.strcpy('f', pos++);
			}
			else {
				//���������_���̓����\�������
				//�������r�b�g�𕄍����������Ƃ��ĕ�����
				auto frac_str = GetCStringBlock<str_t>(Math::GetFracValue(d));
				//�k���I�[�������܂�frac_str�̕�����
				const size_t frac_size = frac_str.Length() + 1;
				//��������53�r�b�g�����͍ő�16���̂��߁Afrac_size�̓k���I�[�������܂߂�17�����Ɏ��܂�͂�
				//(18�����ȏ�̏ꍇ�A�󕶎����Ԃ�)
				if (frac_size >= 18) return ret;
				//���������_���̎w��2^exp + ������52�r�b�g���̃V�t�g
				int exp = Math::GetExpValue(d) - 52;
				//�ő�L������+3(����+�����_+�k���I�[����)
				constexpr size_t BUF_SIZE = max_digit + 3;
				//�v�Z�p�o�b�t�@
				str_t r_buf[BUF_SIZE] = {};
				if (d < 0) r_buf[0] = '-';
				//�����L�����������o�b�t�@�̐擪
				str_t* const buf = d >= 0 ? r_buf : r_buf + 1;
				//buf[length]�͌��݂̃k���I�[����
				size_t length = 0;
				for (const str_t* ptr = frac_str.c_str(); length < frac_size; ++length) buf[length] = ptr[length];
				buf[length - 1] = '.';
				buf[length++] = '0';
				//�������̂��߁A2^n�{���ƂɌv�Z����(exp < n�̏ꍇ�A2�{���ƂɌv�Z����)
				constexpr int n = 16;
				//������buf�̐��l��(2^exp)�{����(exp < 0�̏ꍇ�A����Z�ɂȂ�)
				if (exp > 0) {
					for (int j = exp; j-- > 0;) {
						int mul = 1 << (j >= n ? n : 1);
						if (j >= n) j -= n - 1;
						int up = 0;	//�J��オ��
						for (size_t i = length; i-- > 0;) {
							if (buf[i] == '.') continue;
							int tmp = (buf[i] - '0') * mul + up;
							buf[i] = '0' + (tmp % 10);
							up = tmp / 10;
						}
						//�J��オ�肪�c���Ă���ꍇ�A�o�b�t�@�S�̂��E�V�t�g���Č��𑝂₷
						if (up) {
							size_t delta = Math::CountDigit(up);
							length = (buf + length + delta > r_buf + BUF_SIZE - 1) ? BUF_SIZE - 1 - (buf - r_buf) : length + delta;
							for (size_t i = length - delta; i-- > 0;) buf[i + delta] = buf[i];
							for (size_t i = delta; i-- > 0;) {
								buf[i] = '0' + (up % 10);
								up /= 10;
							}
						}
					}
				}
				else if (exp < 0) {
					for (int j = -exp; j-- > 0;) {
						int div = 1 << (j >= n ? n : 1);
						if (j >= n) j -= n - 1;
						int down = 0;	//�J�艺����
						for (size_t i = 0; i < length; ++i) {
							if (buf[i] == '.') continue;
							int tmp = (buf[i] - '0') + down;
							buf[i] = '0' + (tmp / div);
							down = (tmp % div) * 10;
						}
						//"0.dddd..."�̌`���łȂ����ɁA�擪��'0'�Ŏn�܂��Ă���ꍇ�A'0'����������
						while (buf[0] == '0' && buf[1] != '.') {
							for (size_t i = 0; i < length; ++i) buf[i] = buf[i + 1];
							--length;
						}
						//�J�艺���肪0�ɂȂ邩�A�o�b�t�@�̏I�[�ɓ��B����܂Ŋ���Z�𑱂���
						while (down && (buf + length < r_buf + BUF_SIZE - 1)) {
							buf[length++] = '0' + (down / div);
							down = (down % div) * 10;
						}
						buf[length] = '\0';
					}
				}
				//buf[dot]�͌��݂̏����_
				size_t dot = 0;
				while (buf[dot] != '.') ++dot;
				//buf[first]�͏��߂ďo������0�ȊO�̐���
				size_t first = buf[0] == '0' ? 2 : 0;
				while (buf[first] == '0') ++first;
				//buf[r]�͗L������+1�̈ʂ̐���
				size_t r = digit < dot ? digit : (digit + first + 1 < length ? digit + first + 1 : length);
				//�L�������ɍ��킹�Ċۂߏ���(�����ۂ�)
				if (r < length) {
					//�ۂ߂錅�̐���
					int tmp = buf[r] - '0';
					//�J��オ��(-1�͋����ۂ�)
					int up = tmp > 5 ? 1 : tmp < 5 ? 0 : -1;
					buf[r] = '0';
					for (size_t i = r; i-- > 0;) {
						if (i == dot) continue;
						tmp = buf[i] - '0';
						if (up == -1) up = tmp % 2 != 0 ? 1 : 0;
						//0 <= tmp + up < 20
						if (tmp + up == 10) {
							buf[i] = '0';
							up = 1;
						}
						else {
							buf[i] = '0' + tmp + up;
							up = 0;
						}
						if (!up) break;
					}
					//�ۂ߂̌��ʁA���̌J��オ�肪�N�������ꍇ�̏���
					if (up) {
						for (size_t i = r; i-- > 0;) buf[i + 1] = buf[i];
						buf[0] = '0' + 1;
						++dot;
						++r;
					}
				}
				int e = 0;	//N.xxx * 10^e�̌`�ɐ��K�������Ƃ��̎w��
				//���K��
				if ((mode == NormalizeMode::Auto && (digit < dot || digit < first)) || mode == NormalizeMode::Normalize) {
					//"x.xxxxxxx"�̏ꍇ
					if (dot == 1) {
						if (first) {
							buf[0] = buf[first];
							for (size_t i = 1; first + i <= length; ++i) buf[1 + i] = buf[first + i];
							length -= first - 1;
							e = -static_cast<int>(first) + 1;
						}
					}
					//"xxxx.xxxx"�̏ꍇ
					else {
						if (r < dot && r > 1) ++r;
						for (size_t i = dot; i-- > 1;) buf[i + 1] = buf[i];
						e = static_cast<int>(dot) - 1;
						dot = 1;
						buf[dot] = '.';
					}
				}
				//�L�����������𕶎���萔�ɃR�s�[
				size_t pos = digit + 1 < length ? digit : length - 1;
				if (dot < digit) ++pos;
				if (d < 0) ++pos;
				ret.strcpy(r_buf, 0, pos);
				//�w���\�L�𕶎���萔�ɒǉ�
				if (e != 0) {
					ret.strcpy('e', pos++);
					if (e > 0) ret.strcpy('+', pos++);
					else ret.strcpy('-', pos++);
					e = e > 0 ? e : -e;
					int tmp = static_cast<int>(Math::Pow(10, Math::CountDigit(e) - 1));
					if (tmp == 1) ret.strcpy('0', pos++);
					while (e != 0) {
						int n = e / tmp;
						ret.strcpy('0' + n, pos++);
						e -= n * tmp;
						tmp /= 10;
					}
				}
			}
			return ret;
		}
	}
}

//ToStringBlock
export namespace System::Encoding {
	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf8������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char8_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char8_t> ToU8StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf16������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char16_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char16_t> ToU16StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�utf32������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char32_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char32_t> ToU32StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂烏�C�h������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>wchar_t�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<wchar_t> ToWideStringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint�\���̂̔z�񂩂�}���`�o�C�g������𐶐�����
	/// </summary>
	/// <param name="codePoints">CodePoint�\���̂̔z��ւ̃|�C���^</param>
	/// <param name="count">�z��̒���</param>
	/// <returns>char�^�̕����������StringBlock�I�u�W�F�N�g</returns>
	StringBlock<char> ToMultiByteStringBlock(const CodePoint* codePoints, size_t count) noexcept;
}

inline void* __cdecl operator new[](size_t _Size, int, const char*, int);// { return ::operator new[](_Size); }

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

//StringBlock
namespace System {
	export template<CharType str_t>
	class StringBlock {
	protected:
		str_t* value = nullptr;
		size_t size = 0;
	public:
		static constexpr size_t npos = MAX_VALUE<size_t>;
	public:
		StringBlock() noexcept {
			size = 1;
			value = new str_t[size];
			value[0] = '\0';
		}
		StringBlock(const StringBlock<str_t>& arg) noexcept {
			size = arg.size;
			value = new str_t[size];
			for (size_t i = 0; i < size; ++i) value[i] = arg.value[i];
		}
		StringBlock(StringBlock<str_t>&& arg) noexcept : size(arg.size), value(arg.value) {
			arg.size = 0;
			arg.value = nullptr;
		}
		StringBlock(const str_t c) noexcept {
			size = 2;
			value = new str_t[size];
			value[0] = c;
			value[1] = '\0';
		}
		template<size_t N>
		StringBlock(const str_t(&arg)[N]) noexcept {
			size = N;
			value = new str_t[size];
			for (size_t i = 0; i < size; ++i) value[i] = arg[i];
			value[size - 1] = '\0';
		}
		StringBlock(const str_t* arg, size_t n = npos) noexcept {
			if (!arg) {
				size = 1;
				value = new str_t[size];
				value[0] = '\0';
			}
			else {
				if (n == 0) n = npos;
				size_t len = 0;
				while (arg[len] != '\0' && ++len < n);
				if (len == npos) {
					len = 0;
					size = 1;
				}
				else size = len + 1;
				value = new str_t[size];
				for (size_t i = 0; i < len; ++i) value[i] = arg[i];
				value[size - 1] = '\0';
			}
		}
		StringBlock(const nullptr_t arg) noexcept {}
		template<size_t N>
		StringBlock(const CStringBlock<str_t, N>& arg) noexcept {
			size_t len = arg.Length();
			size = len + 1;
			value = new str_t[size];
			for (size_t i = 0; i < len; ++i) value[i] = arg.value[i];
			value[size - 1] = '\0';
		}
		~StringBlock() noexcept {
			delete[] value;
			value = nullptr;
			size = 0;
		}
	public:
		/// <summary>
		/// �󕶎�����擾����
		/// </summary>
		static StringBlock Empty() noexcept { return StringBlock(); }
		/// <summary>
		/// �w�肵���v�f���̗̈�����󕶎�����擾����
		/// </summary>
		/// <param name="size">
		/// �v�f���B
		/// n�����̕�����Ɏg�p����ꍇ�A�k���I�[�����̂��߂�(n+1)��
		/// �w�肷��K�v������
		/// </param>
		static StringBlock CreateStringBlock(size_t size) noexcept {
			StringBlock<str_t> ret;
			delete[] ret.value;
			ret.value = new str_t[size]{};
			ret.size = size;
			return ret;
		}
	public:/* �����z��擾 */
		/// <summary>
		/// �����^�z����擾����(const��)
		/// </summary>
		const str_t* c_str() const noexcept { return value; }
		/// <summary>
		/// �����^�z����擾����
		/// </summary>
		str_t* c_str() noexcept { return value; }
	public:/* �����񑀍�(const) */
		/// <summary>
		/// �w�肵���������擪���猟������
		/// </summary>
		/// <param name="src">�������镶����</param>
		/// <param name="pos">�������n�߂�C���f�b�N�X</param>
		/// <returns>
		/// �w�肵��������ƍŏ��Ɉ�v�����ʒu(�擪�̃C���f�b�N�X)�B
		/// ��v���镔�������݂��Ȃ��ꍇ�AStringBlock::npos��Ԃ�
		/// </returns>
		size_t find(const StringBlock<str_t>& src, size_t pos = 0) const noexcept {
			const size_t selfLen = Length();
			const size_t srcLen = src.Length();
			if (selfLen < srcLen) return npos;
			const size_t searchEnd = selfLen - srcLen;
			const str_t* srcValue = src.c_str();
			for (size_t i = pos; i <= searchEnd; ++i) {
				if (value[i] != srcValue[0]) continue;
				size_t cur = 1;
				for (; cur < srcLen; ++cur) {
					if (value[i + cur] != srcValue[cur]) break;
				}
				if (cur == srcLen) return i;
			}
			return npos;
		}
		/// <summary>
		/// �w�肵��������𖖔����猟������
		/// </summary>
		/// <param name="src">�������镶����</param>
		/// <param name="pos">�������n�߂�C���f�b�N�X�B����͕�����̐擪�ƂȂ�C���f�b�N�X�ł���</param>
		/// <returns>
		/// �w�肵��������ƍŏ��Ɉ�v�����ʒu(�擪�̃C���f�b�N�X)�B
		/// ��v���镔�������݂��Ȃ��ꍇ�AStringBlock::npos��Ԃ�
		/// </returns>
		size_t rfind(const StringBlock<str_t>& src, size_t pos = npos) const noexcept {
			const size_t selfLen = Length();
			const size_t srcLen = src.Length();
			if (selfLen < srcLen) return npos;
			const size_t searchBegin = selfLen - srcLen;
			const str_t* srcValue = src.c_str();
			for (size_t i = (searchBegin < pos ? searchBegin : pos) + 1; i-- > 0;) {
				if (value[i] != srcValue[0]) continue;
				size_t cur = 1;
				for (; cur < srcLen; ++cur) {
					if (value[i + cur] != srcValue[cur]) break;
				}
				if (cur == srcLen) return i;
			}
			return npos;
		}
		/// <summary>
		/// ������������擾����
		/// </summary>
		/// <param name="pos">
		/// �擾���镔��������̊J�n�ʒu(�C���f�b�N�X)�B
		/// ���̕�����̃k���I�[�����̃C���f�b�N�X�ȏ�̒l���w�肵���ꍇ�A�K���󕶎����Ԃ�
		/// </param>
		/// <param name="n">
		/// �擾���镶����̗v�f���B
		/// 0���w�肵���ꍇ�A�K���󕶎����Ԃ��B
		/// ���̕�����̗v�f�����傫���l���w�肵���ꍇ�A�k���I�[�����܂Ŏ擾����
		/// </param>
		StringBlock<str_t> substr(size_t pos, size_t n = npos) const noexcept {
			if (!n || pos >= size - 1) return StringBlock<str_t>();
			else return StringBlock<str_t>(value + pos, n);
		}
	public:/* �����񑀍�(��const) */
		/// <summary>
		/// �w�肵����������w�肵���ʒu�ɏ�������
		/// </summary>
		/// <param name="src">�������ޕ�����̐擪���w���|�C���^</param>
		/// <param name="n">�������ޕ�����</param>
		/// <param name="pos">�������݂��n�߂邱�̕�����̃C���f�b�N�X</param>
		StringBlock& strcpy(const str_t* src, size_t n, size_t pos) noexcept {
			//�͈͊O�ւ̏������݂͋֎~
			if (pos >= size) return *this;
			//���ۂɏ������ޕ�����
			size_t count = size - pos - 1;
			if (n < count) count = n;
			//�k���I�[�����͏������܂Ȃ�(������̒����͕ς��Ȃ�)
			for (size_t i = 0; i < count && src[i]; ++i) value[pos + i] = src[i];
			return *this;
		}
		/// <summary>
		/// �w�肵���������w�肵���ʒu�ɏ�������
		/// </summary>
		/// <param name="c">�������ޕ����B��v�f�ŕ\���ł��Ȃ������͕������strcpy���g�p����</param>
		/// <param name="pos">�������ݐ�̃C���f�b�N�X�B�k���I�[�����͏㏑���ł��Ȃ�</param>
		StringBlock& strcpy(const str_t c, size_t pos) noexcept {
			if (pos < size - 1) value[pos] = c;
			return *this;
		}
	public:/* �g�������񑀍�(const) */
		/// <summary>
		/// ������̒���(�k���I�[�����������v�f��)���擾����B
		/// �����̗v�f�ňꕶ����\�������^�̏ꍇ�A�������ƈ�v���Ȃ��\��������
		/// </summary>
		/// <returns>�����񂪃k���I�[�̂Ƃ��A�߂�l�̓k���I�[�����̃C���f�b�N�X�ɓ�����</returns>
		size_t Length() const noexcept { for (size_t i = 0; i < size; ++i) if (value[i] == '\0') return i; return size; }
		/// <summary>
		/// �����Ɋm�ۂ���Ă���v�f�����擾����
		/// </summary>
		size_t Capacity() const noexcept { return size; }
		/// <summary>
		/// ���̕����񂪋󕶎��񂩒��ׂ�
		/// </summary>
		bool IsNullOrEmpty() const noexcept { return !(value && size && value[0]); }
		/// <summary>
		/// �w�肵����������������ׂĒu��������������擾����
		/// </summary>
		/// <param name="src">�������镶����</param>
		/// <param name="dst">�u����̕�����</param>
		/// <returns>���̕�����̌���������ƈ�v���镔�������ׂĒu������������</returns>
		StringBlock<str_t> Replace(const StringBlock<str_t>& src, const StringBlock<str_t>& dst) const noexcept {
			const size_t srcLen = src.Length();
			const size_t dstLen = dst.Length();
			VectorBase<size_t> vec;	//�������ʂ̔z��
			size_t pos = 0;
			do {
				pos = find(src, pos);
				if (pos == npos) break;
				vec.Add(pos);
				pos += srcLen;
			} while (true);
			//���������Ȃ��ꍇ�A�u�����Ȃ�
			if (!vec.Count()) return *this;
			//�v�f����{���̕�����̗v�f�� - (�u���� * �u��������̗v�f���̍�) + �k���I�[���� }
			StringBlock<str_t> ret = CreateStringBlock(Length() + vec.Count() * (dstLen - srcLen) + 1);
			size_t selfCur = 0;
			size_t retCur = 0;
			for (size_t i : vec) {
				if (retCur < i) {
					ret.strcpy(value + selfCur, i - selfCur, retCur);
					retCur += i - selfCur;
					selfCur = i;
				}
				ret.strcpy(dst.value, dstLen, retCur);
				retCur += dstLen;
				selfCur += srcLen;
			}
			if (value[selfCur]) ret.strcpy(value + selfCur, npos, retCur);
			return ret;
		}
	public:
		StringBlock<str_t> operator[](const size_t i) const noexcept { return Length() <= i ? Empty() : StringBlock(value[i]); }
		StringBlock<str_t>& operator=(const StringBlock<str_t>& rhs) noexcept {
			if (this == &rhs) return *this;
			size = rhs.size;
			delete[] value;
			value = new str_t[size];
			for (size_t i = 0; i < size; ++i) value[i] = rhs.value[i];
			return *this;
		}
		StringBlock<str_t>& operator=(StringBlock<str_t>&& rhs) noexcept {
			if (this == &rhs) return *this;
			size = rhs.size;
			delete[] value;
			value = rhs.value;
			rhs.size = 0;
			rhs.value = nullptr;
			return *this;
		}
		StringBlock<str_t> operator+(const StringBlock<str_t>& rhs) const noexcept {
			const size_t len = Length();
			const size_t rlen = rhs.Length();
			StringBlock<str_t> ret = CreateStringBlock(len + rlen + 1);
			for (size_t i = 0; i < len; ++i) ret.value[i] = value[i];
			for (size_t i = 0; i < rlen; ++i) ret.value[len + i] = rhs.value[i];
			ret.value[len + rlen] = '\0';
			return ret;
		}
		StringBlock<str_t>& operator+=(const StringBlock<str_t>& rhs) noexcept { return *this = *this + rhs; }
		bool operator==(const StringBlock<str_t>& rhs) const noexcept {
			size_t len = Length(), rhsLen = rhs.Length();
			if (len != rhsLen) return false;
			const str_t* rdata = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (value[i] != rdata[i]) return false;
			return true;
		}
		bool operator!=(const StringBlock<str_t>& rhs) const noexcept { return !(*this == rhs); }
	private:
		/// <summary>
		/// �����R�[�h�̔z����擾����
		/// </summary>
		VectorBase<Encoding::CodePoint> ToCodePoints() const noexcept {
			using namespace Encoding;
			VectorBase<CodePoint> ret;
			CodePoint point;
			size_t pos = 0;
			do {
				point = GetCodePoint(value + pos);
				pos += point.count;
				ret.Add(point);
			} while (point.point != 0 && pos < size);
			return ret;
		}
	public:/* �����^�ϊ� */
		StringBlock<char8_t> ToU8StringBlock() const noexcept {
			if constexpr (is_same_v<char8_t, str_t>) return StringBlock<char8_t>(*this);
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char8_t>(Encoding::ToU8StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char16_t> ToU16StringBlock() const noexcept {
			if constexpr (is_same_v<char16_t, str_t>) return StringBlock<char16_t>(*this);
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char16_t>(Encoding::ToU16StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char32_t> ToU32StringBlock() const noexcept {
			if constexpr (is_same_v<char32_t, str_t>) return StringBlock<char32_t>(*this);
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char32_t>(Encoding::ToU32StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<wchar_t> ToWideStringBlock() const noexcept {
			if constexpr (is_same_v<wchar_t, str_t>) return StringBlock<wchar_t>(*this);
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<wchar_t>(Encoding::ToWideStringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char> ToMultiByteStringBlock() const noexcept {
			if constexpr (is_same_v<char, str_t>) return StringBlock<char>(*this);
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char>(Encoding::ToMultiByteStringBlock(tmp.Items(), tmp.Count()));
			}
		}
	};
}
