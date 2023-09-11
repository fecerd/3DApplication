export module CString;
import CSTDINT;
import Objects;

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
		//i番目([0, count() - 1])の文字定数を返す
		//iが範囲外の場合、ヌル文字を返す
		constexpr auto operator[](const size_t i) const noexcept { return CStringBlock<char16_t, N>::Length() <= i ? Empty() : CString<2>(this->value[i]); }
		//文字列定数同士を結合し、新たな文字列定数を返す
		template<size_t n>
		constexpr auto operator+(const CString<n>& rhs) const noexcept {
			return CString<N + n - 1>(CStringBlock<char16_t, N>::operator+(rhs));
		}
		//文字列定数同士が完全一致する場合、trueを返す
		template<size_t n>
		constexpr bool operator==(const CString<n>& rhs) const noexcept {
			return CStringBlock<char16_t, N>::operator==(rhs);
		}
		//文字列定数同士が完全一致しない場合、trueを返す
		template<size_t n> constexpr bool operator!=(const CString<n>& rhs) const noexcept { return !(*this == rhs); }
	public:
		//System::Stringに変換する
		explicit operator System::String() const noexcept { return ToString(); }
	public:
		/// <summary>
		/// 動的文字列型と比較し、等しい場合にtrueを返す
		/// </summary>
		/// <param name="rhs">String型変数</param>
		bool operator==(const System::String& rhs) const noexcept {
			size_t len = CStringBlock<char16_t, N>::Length();
			size_t rlen = rhs.Length();
			if (len != rlen) return false;
			const char16_t* l = this->c_str();
			const char16_t* r = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (l[i] != r[i]) return false;
			return true;
		}
		bool operator!=(const System::String& rhs) const noexcept { return !(*this == rhs); }
	};
	template<size_t N> CString(const char16_t(&)[N])->CString<N>;
}

export namespace System {
	template<size_t n>
	inline bool operator==(const String& lhs, const CString<n>& rhs) noexcept { return rhs == lhs; }
	template<size_t n>
	inline bool operator!=(const String& lhs, const CString<n>& rhs) noexcept { return rhs != lhs; }
}
