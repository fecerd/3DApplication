export module CString;
import Objects;
import StringBlock;

export namespace System {
	template<size_t N>
	class CString : public CStringBlock<char16_t, N> {
	public:
		static constexpr size_t npos = CStringBlock<char16_t, N>::npos;
	public:
		using CStringBlock<char16_t, N>::CStringBlock;
		constexpr CString(const CString& arg) noexcept : CString(static_cast<const CStringBlock<char16_t, N>&>(arg)) {}
		constexpr ~CString() noexcept {}
	public:
		const wchar_t* w_str() const noexcept { return reinterpret_cast<const wchar_t*>(CStringBlock<char16_t, N>::c_str()); }
	public:
		static constexpr CString<3> Empty() noexcept { return CString<3>(); }
	public:
		Type GetType() const noexcept { return Type::CreateType<CString<N>>(); }
		System::String ToString() const noexcept { return System::String(*this); }
		uint32_t GetTypeID() const noexcept { return System::GetID<CString<N>>(); }
	public:
		//i�Ԗ�([0, count() - 1])�̕����萔��Ԃ�
		//i���͈͊O�̏ꍇ�A�k��������Ԃ�
		constexpr auto operator[](const size_t i) const noexcept { return CStringBlock<char16_t, N>::Length() <= i ? Empty() : CString<2>(this->value[i]); }
		//������萔���m���������A�V���ȕ�����萔��Ԃ�
		template<size_t n>
		constexpr auto operator+(const CString<n>& rhs) const noexcept {
			return CString<N + n - 1>(CStringBlock<char16_t, N>::operator+(rhs));
		}
		//������萔���m�����S��v����ꍇ�Atrue��Ԃ�
		template<size_t n>
		constexpr bool operator==(const CString<n>& rhs) const noexcept {
			return CStringBlock<char16_t, N>::operator==(rhs);
		}
		//������萔���m�����S��v���Ȃ��ꍇ�Atrue��Ԃ�
		template<size_t n> constexpr bool operator!=(const CString<n>& rhs) const noexcept { return !(*this == rhs); }
	public:
		//System::String�ɕϊ�����
		explicit operator System::String() const noexcept { return ToString(); }
	public:
		/// <summary>
		/// ���I������^�Ɣ�r���A�������ꍇ��true��Ԃ�
		/// </summary>
		/// <param name="rhs">String�^�ϐ�</param>
		bool operator==(const System::String& rhs) const noexcept {
			size_t len = CStringBlock<char16_t, N>::Length();
			size_t rlen = rhs.Length();
			if (len != rlen) return false;
			const char16_t* l = this->c_str();
			const char16_t* r = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (l[i] != r[i]) return false;
			return true;
		}
		template<size_t n>
		friend bool operator==(const System::String& lhs, const CString<n>& rhs) noexcept { return rhs == lhs; }
		bool operator!=(const System::String& rhs) const noexcept { return !(*this == rhs); }
		template<size_t n>
		friend bool operator!=(const System::String& lhs, const CString<n>& rhs) noexcept { return rhs != lhs; }
	};
	template<size_t N> CString(const char16_t(&)[N])->CString<N>;
}