//module;
//#include "Object.hpp"
//#include "String.hpp"
//#include "Type.hpp"
export module Objects_Internal;
export import CSTDINT;

export import Traits;
import Math;
export import StringBlock;

import TypeInfo;

//export namespace System {
//	using Object = System::Object;
//	using String = System::String;
//	using Type = System::Type;
//}

//Object
export namespace System {
	class String;
	class Type;

	/// <summary>
	/// System名前空間のpublicなクラスの基底クラス
	/// </summary>
	class Object {
	public:
		constexpr Object() noexcept {}
		virtual constexpr ~Object() noexcept {}
	public:
		/// <summary>
		/// このインスタンスと引数をDerived::operator==(const Derived&amp;)で比較する
		/// </summary>
		/// <param name="obj">比較するObject</param>
		/// <returns>Derived型として等価の場合、true。それ以外の場合、false</returns>
		virtual bool Equals(const Object& obj) const noexcept { return *this == obj; }
		/// <summary>
		/// ハッシュ値を取得する
		/// </summary>
		/// <returns>実装されている型の値に対して固有のハッシュ値</returns>
		virtual size_t GetHashCode() const noexcept { return static_cast<size_t>(reinterpret_cast<uintptr_t>(this)); }
		/// <summary>
		/// Typeオブジェクトを取得する
		/// </summary>
		/// <returns>このインスタンスの型を指すTypeオブジェクト</returns>
		virtual Type GetType() const noexcept = 0;
		/// <summary>
		/// 文字列化する
		/// </summary>
		/// <returns>このインスタンスが持つ値を表すStringオブジェクト</returns>
		virtual String ToString() const noexcept = 0;
		/// <summary>
		/// 型固有のIDを取得する
		/// </summary>
		/// <returns>型固有の4バイト整数値</returns>
		virtual uint32_t GetTypeID() const noexcept = 0;
	public:
		bool operator==(const Object& obj) const noexcept { return this == &obj; }
	};
}

//String
export namespace System {
	template<class Src>
	concept ConvertibleToString = Traits::is_convertible_v<Src, String>;

	class String : public StringBlock<char16_t> {
	public:
		static constexpr size_t npos = System::MAX_VALUE<size_t>;
	public:
		String() noexcept
		    : StringBlock<char16_t>() {}
		String(const String& arg) noexcept
		    : StringBlock<char16_t>(arg) {}
		String(String&& arg) noexcept
		    : StringBlock<char16_t>(System::move(arg)) {}
		String(const StringBlock<char16_t>& arg) noexcept
		    : StringBlock<char16_t>(arg) {}
		String(StringBlock<char16_t>&& arg) noexcept
		    : StringBlock<char16_t>(System::move(arg)) {}
		template<Traits::Concepts::CCharType str_t>
		String(const str_t* data, size_t N = npos) noexcept
		    : StringBlock<char16_t>(StringBlock<str_t>(data, N).ToU16StringBlock()) {}
		template<Traits::Concepts::CCharType str_t>
		String(str_t c) noexcept
		    : StringBlock<char16_t>(StringBlock<str_t>(c).ToU16StringBlock()) {}
		template<class T>
		requires Traits::Concepts::CIntegral<T> && (!Traits::Concepts::CCharType<T>) && (!Traits::Concepts::CFloating<T>)
		String(T n) noexcept
		    : String(GetCStringBlock<char16_t>(n)) {}
		template<class T>
		requires Traits::Concepts::CFloating<T>
		String(T n) noexcept
		    : String(GetCStringBlock<char16_t, 10>(n)) {}
		String(bool b) noexcept
		    : String(b ? u"true" : u"false") {}
		String(nullptr_t ptr) noexcept
		    : String() {}
		template<size_t N>
		String(const CStringBlock<char16_t, N>& arg) noexcept
		    : StringBlock<char16_t>(arg) {}
		template<size_t N>
		String(CStringBlock<char16_t, N>&& arg) noexcept
		    : StringBlock<char16_t>(System::move(arg)) {}
		template<Traits::Concepts::CCharType str_t, size_t N>
		String(const CStringBlock<str_t, N>& arg) noexcept
			: StringBlock<char16_t>(StringBlock<str_t>(arg).ToU16StringBlock()) {}
		~String() noexcept {}
	public:/* g++ではStringBlock<char16_t>側の関数の呼び出しに無駄なアセンブリ命令が混ざり、Segmentation faultとなるため、こちら側にも関数定義する */
		size_t Length() const noexcept { return StringBlock<char16_t>::Length(); }
		char16_t* c_str() noexcept { return this->value; }
		const char16_t* c_str() const noexcept { return this->value; }
		wchar_t* w_str() noexcept { return reinterpret_cast<wchar_t*>(c_str()); }
		const wchar_t* w_str() const noexcept { return reinterpret_cast<const wchar_t*>(c_str()); }
	public:
		static const String& Empty() noexcept {
			static String ret{};
			return ret;
		}
		static String CreateString(size_t size) noexcept { return StringBlock<char16_t>::CreateStringBlock(size); }
	public:
		String& strcpy(const char16_t* src, size_t n, size_t pos) noexcept {
			StringBlock<char16_t>::strcpy(src, n, pos);
			return *this;
		}
		String& strcpy(char16_t c, size_t pos) noexcept {
			StringBlock<char16_t>::strcpy(c, pos);
			return *this;
		}
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
			return Math::murmur3_32(c_str(), Length() * sizeof(char16_t), GetTypeID());
		}
	public:
		template<Traits::Concepts::CCharType str_t>
		StringBlock<str_t> Convert() const noexcept {
			if constexpr (Traits::is_same_v<str_t, char>)
				return static_cast<const StringBlock<char16_t>&>(*this).ToMultiByteStringBlock();
			else if constexpr (Traits::is_same_v<str_t, char8_t>)
				return static_cast<const StringBlock<char16_t>&>(*this).ToU8StringBlock();
			else if constexpr (Traits::is_same_v<str_t, char16_t>)
				return StringBlock<char16_t>(static_cast<const StringBlock<char16_t>&>(*this));
			else if constexpr (Traits::is_same_v<str_t, char32_t>)
				return static_cast<const StringBlock<char16_t>&>(*this).ToU32StringBlock();
			else if constexpr (Traits::is_same_v<str_t, wchar_t>)
				return static_cast<const StringBlock<char16_t>&>(*this).ToWideStringBlock();
		}
	public:
		bool operator==(const StringBlock<char16_t>& rhs) const noexcept {
			return StringBlock<char16_t>::operator==(rhs);
		}
		bool operator!=(const StringBlock<char16_t>& rhs) const noexcept {
			return StringBlock<char16_t>::operator!=(rhs);
		}
		String& operator=(const String& rhs) noexcept {
			StringBlock<char16_t>::operator=(rhs);
			return *this;
		}
		String& operator=(String&& rhs) noexcept {
			StringBlock<char16_t>::operator=(System::move(rhs));
			return *this;
		}
	private:
		template<class T>
		static String ConvertString(T&& t) noexcept {
			using type = Traits::remove_cvref_t<T>;
			if constexpr (Traits::Concepts::CNumber<type>)
				return String(t);
			else {
				if constexpr (Traits::is_lvalue_reference_v<T>)
					return String(t);
				else
					return String(static_cast<T&&>(t));
			}
		}
		template<class Head, class... Args>
		static size_t LengthAll(Head&& head, Args&&... args) noexcept {
			if constexpr (sizeof...(Args) == 0)
				return head.Length();
			else
				return head.Length() + LengthAll(static_cast<Args&&>(args)...);
		}
		template<class Head, class... Args>
		static void Joint_Impl(String& str, size_t& pos, Head&& head, Args&&... args) noexcept {
			size_t length = head.Length();
			str.strcpy(head.c_str(), length, pos);
			pos += length;
			if constexpr (sizeof...(Args) != 0) Joint_Impl(str, pos, static_cast<Args&&>(args)...);
		}
		template<class... Args>
		static String Joint_Internal(Args&&... args) noexcept {
			size_t length = LengthAll(args...);
			String ret = CreateString(length + 1);
			size_t pos = 0;
			Joint_Impl(ret, pos, static_cast<Args&&>(args)...);
			return ret;
		}
	public:
		template<ConvertibleToString Head, ConvertibleToString... Args>
		static String Joint(Head&& head, Args&&... args) noexcept {
			return String::Joint_Internal(ConvertString(static_cast<Head&&>(head)), ConvertString(static_cast<Args&&>(args))...);
		}
	};
}

//Type
export namespace System {
	class Type : public Object {
		String* m_typeName = nullptr;
		String* m_templateArgs = nullptr;
		size_t m_templateCount = 0;
	public:
		Type() noexcept {}
		Type(const Type&) noexcept;
		Type(Type&& arg) noexcept
		    : m_typeName(arg.m_typeName)
		    , m_templateArgs(arg.m_templateArgs)
		    , m_templateCount(arg.m_templateCount) {
			arg.m_typeName = nullptr;
			arg.m_templateArgs = nullptr;
			arg.m_templateCount = 0;
		}
		Type(const char16_t*) noexcept;
		~Type() noexcept;
	public:
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Type&>(obj) : false; }
		Type GetType() const noexcept override { return Type(String(TypeInfo<Type>::GetFullName()).c_str()); }
		String ToString() const noexcept override;
		uint32_t GetTypeID() const noexcept override;
	public:
		bool operator==(const Type& rhs) const noexcept;
	public:
		template<class T>
		static Type CreateType() { return Type(String(TypeInfo<T>::GetFullName()).c_str()); }
	};
}

namespace System {
	Type::Type(const Type& arg) noexcept
	    : m_templateCount(arg.m_templateCount) {
		if (arg.m_typeName)
			m_typeName = new String(*arg.m_typeName);
		if (arg.m_templateArgs) {
			m_templateArgs = new String[m_templateCount];
			for (size_t i = 0; i < m_templateCount; ++i)
				m_templateArgs[i] = arg.m_templateArgs[i];
		}
	}
	Type::Type(const char16_t* name) noexcept {
	}
	Type::~Type() noexcept {
		delete m_typeName;
		m_typeName = nullptr;
		delete m_templateArgs;
		m_templateArgs = nullptr;
		m_templateCount = 0;
	}
	String Type::ToString() const noexcept {
		String ret;

		return ret;
	}
	bool Type::operator==(const Type& rhs) const noexcept {
		if (!m_typeName || !rhs.m_typeName)
			return m_typeName == rhs.m_typeName;
		if (*m_typeName != *rhs.m_typeName)
			return false;
		if (m_templateCount != rhs.m_templateCount)
			return false;
		for (size_t i = 0; i < m_templateCount; ++i) {
			if (m_templateArgs[i] != rhs.m_templateArgs[i])
				return false;
		}
		return true;
	}
}
