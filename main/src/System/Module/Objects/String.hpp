#pragma once
#ifndef String_H
#define String_H
#include "..\..\..\CRTDBG\crtdbg_wrapper.hpp"
#include "Object.hpp"
import Traits;
import Math;
import StringBlock;
using namespace System::Traits;

namespace System {
	template<class Src>
	concept ConvertibleToString = is_convertible_v<Src, String>;

	class String : public StringBlock<char16_t> {
	public:
		static constexpr size_t npos = System::MAX_VALUE<size_t>;
	public:
		String() noexcept : StringBlock<char16_t>() {}
		String(const String& arg) noexcept : StringBlock<char16_t>(static_cast<const StringBlock<char16_t>&>(arg)) {}
		String(String&& arg) noexcept : StringBlock<char16_t>(static_cast<StringBlock<char16_t>&&>(arg)) {}
		String(const StringBlock<char16_t>& arg) noexcept : StringBlock<char16_t>(arg) {}
		String(StringBlock<char16_t>&& arg) noexcept : StringBlock<char16_t>(static_cast<StringBlock<char16_t>&&>(arg)) {}
		template<CharType str_t>
		String(const str_t* data, size_t N = npos) noexcept : StringBlock<char16_t>(StringBlock<str_t>(data, N).ToU16StringBlock()) {}
		template<CharType str_t>
		String(str_t c) noexcept : StringBlock<char16_t>(StringBlock<str_t>(c).ToU16StringBlock()) {}
		template<class T> requires Integral<T> && (!CharType<T>) && (!Floating<T>)
		String(T n) noexcept : String(GetCStringBlock<char16_t>(n)) {}
		template<class T> requires Floating<T>
		String(T n) noexcept : String(GetCStringBlock<char16_t, 10>(n)) {}
		String(bool b) noexcept : String(b ? u"true" : u"false") {}
		String(nullptr_t ptr) noexcept : String() {}
		template<size_t N>
		String(const CStringBlock<char16_t, N>& arg) noexcept : StringBlock<char16_t>(arg) {}
		template<size_t N>
		String(CStringBlock<char16_t, N>&& arg) noexcept : StringBlock<char16_t>(static_cast<CStringBlock<char16_t, N>&&>(arg)) {}
		~String() noexcept = default;
	public:
		wchar_t* w_str() noexcept { return reinterpret_cast<wchar_t*>(c_str()); }
		const wchar_t* w_str() const noexcept { return reinterpret_cast<const wchar_t*>(c_str()); }
	public:
		static const String& Empty() noexcept { static String ret{}; return ret; }
		static String CreateString(size_t size) noexcept { return StringBlock<char16_t>::CreateStringBlock(size); }
	public:
		String& strcpy(const char16_t* src, size_t n, size_t pos) noexcept { StringBlock<char16_t>::strcpy(src, n, pos); return *this; }
		String& strcpy(char16_t c, size_t pos) noexcept { StringBlock<char16_t>::strcpy(c, pos); return *this; }
	public:
		String substr(size_t pos, size_t n = npos) const noexcept { return String(StringBlock<char16_t>::substr(pos, n)); }
		String Replace(const String& src, const String& dst) const noexcept {
			return String(StringBlock<char16_t>::Replace(src, dst));
		}
	public:
		Type GetType() const noexcept;
		String ToString() const noexcept { return *this; }
		uint32_t GetTypeID() const noexcept;
		size_t GetHashCode() const noexcept {
			return Math::murmur3_32(this->c_str(), this->Length() * sizeof(char16_t), GetTypeID());
		}
	public:
		template<CharType str_t>
		StringBlock<str_t> Convert() const noexcept {
			if constexpr (is_same_v<str_t, char>) return static_cast<const StringBlock<char16_t>&>(*this).ToMultiByteStringBlock();
			else if constexpr (is_same_v<str_t, char8_t>) return static_cast<const StringBlock<char16_t>&>(*this).ToU8StringBlock();
			else if constexpr (is_same_v<str_t, char16_t>) return StringBlock<char16_t>(static_cast<const StringBlock<char16_t>&>(*this));
			else if constexpr (is_same_v<str_t, char32_t>) return static_cast<const StringBlock<char16_t>&>(*this).ToU32StringBlock();
			else if constexpr (is_same_v<str_t, wchar_t>) return static_cast<const StringBlock<char16_t>&>(*this).ToWideStringBlock();
		}
	public:
		using StringBlock<char16_t>::operator==;
		String& operator=(const String& rhs) noexcept {
			StringBlock<char16_t>::operator=(static_cast<const StringBlock<char16_t>&>(rhs));
			return *this;
		}
		String& operator=(String&& rhs) noexcept {
			StringBlock<char16_t>::operator=(static_cast<StringBlock<char16_t>&&>(rhs));
			return *this;
		}
	private:
		template<class T>
		static String ConvertString(T&& t) noexcept {
			using type = remove_cvref_t<T>;
			if constexpr (Number<type>) return String(t);
			else {
				if constexpr (is_lvalue_reference_v<T>) return String(t);
				else return String(static_cast<T&&>(t));
			}
		}
		template<class Head, class ...Args>
		static size_t LengthAll(Head&& head, Args&& ...args) noexcept {
			if constexpr (sizeof...(Args) == 0) return head.Length();
			else return head.Length() + LengthAll(static_cast<Args&&>(args)...);
		}
		template<class Head, class ...Args>
		static void Joint_Impl(String& str, size_t& pos, Head&& head, Args&& ...args) noexcept {
			size_t length = head.Length();
			str.strcpy(head.c_str(), length, pos);
			pos += length;
			if constexpr (sizeof...(Args) != 0) Joint_Impl(str, pos, static_cast<Args&&...>(args)...);
		}
		template<class ...Args>
		static String Joint_Internal(Args&& ...args) noexcept {
			size_t length = LengthAll(args...);
			String ret = CreateString(length + 1);
			size_t pos = 0;
			Joint_Impl(ret, pos, static_cast<Args&&>(args)...);
			return ret;
		}
	public:
		template<ConvertibleToString Head, ConvertibleToString ...Args>
		static String Joint(Head&& head, Args&& ...args) noexcept {
			return String::Joint_Internal(ConvertString(static_cast<Head&&>(head)), ConvertString(static_cast<Args&&>(args))...);
		}
	};
}

#endif