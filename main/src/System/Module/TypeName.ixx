export module TypeName;
import CSTDINT;
import Traits;
import StringBlock;

export namespace System {
	/// <summary>
	/// TypeName::GetTypeSpecifier()関数が返す型指定子を表す列挙型
	/// </summary>
	enum class TypeSpecifier : uint8_t {
		NONE = 0,
		STRUCT,
		CLASS,
		ENUM
	};
}

namespace System::TypeName_Internal {
	template<size_t size, size_t N>
	constexpr CStringBlock<char16_t, size> TrimName(const char (&name)[size], const char (&funcname)[N]) noexcept {
		return CStringBlock<char16_t, size>();
		size_t e = size;
		for (; e-- > 0;)
			if (name[e] == '>') break;
		if (e == 0) return CStringBlock<char16_t, size>();
		size_t s = 0;
		for (; s < e; ++s) {
			if (name[s] == funcname[0]) {
				size_t n = s;
				for (const char& c : funcname) {
					if (name[n] == c)
						++n;
					else
						break;
				}
				if (name[n] == '<' && n == (s + N - 1)) {
					s = n + 1;
					break;
				}
			}
		}
		if (s >= e) return CStringBlock<char16_t, size>();
		--e;
		CStringBlock<char16_t, size> ret;
		for (size_t i = 0, count = e - s + 1; i < count; ++i)
			ret.strcpy(static_cast<char16_t>(name[s + i]), i);
		ret.strcpy(u'\0', e - s + 1);
		return ret;
	}
	template<class T>
	constexpr auto GetTypeName() {
#if defined(__clang__) || defined(__GNUC__)
		return TrimName(__PRETTY_FUNCTION__, "GetTypeName");
#elif defined(_MSC_VER)
		return TrimName(__FUNCSIG__, "GetTypeName");
#else
		return CStringBlock<char16_t, 1>();
#endif
	}
	template<class E, E V>
	constexpr auto GetEnumName() {
#if defined(__clang__) || defined(__GNUC__)
		return TrimName(__PRETTY_FUNCTION__, "GetEnumName");
#elif defined(_MSC_VER)
		return TrimName(__FUNCSIG__, "GetEnumName");
#else
		return CStringBlock<char16_t, 1>();
#endif
	}
}
using namespace System::TypeName_Internal;

//TypeName
export namespace System {
	template<class T>
	class TypeName {
		using nametype = decltype(GetTypeName<T>());
		static constexpr nametype FullName = GetTypeName<T>();
	private:
		struct SplitResult {
			const char16_t* b = nullptr;
			const char16_t* e = nullptr;
		};
		static constexpr SplitResult Split(const char16_t* b, const char16_t* e) {
			size_t level = GetLevelsOfPointer();
			e -= level;
			const char16_t* current = b;
			const char16_t* s = b;
			for (; current != e; ++current) {
				if (*current == u':' && current + 1 != e && current[1] == u':') {
					++current;
					s = current + 1;
				}
				else if (*current == u'<') {
					size_t count = 1;
					const char16_t* tmp = current + 1;
					while (count && tmp != e) {
						if (*tmp == u'<') ++count;
						else if (*tmp == u'>') --count;
						++tmp;
					}
					if (count) return SplitResult{ e + level, e + level };
					if (tmp == e) return SplitResult{ s, e + level };
					current = tmp - 1;
					s = tmp;
				}
			}
			return SplitResult{ s, e + level };
		}
	public:
		/// <summary>
		/// 型Tの名前空間で修飾された名前を取得する
		/// </summary>
		static constexpr const nametype& GetFullName() noexcept { return FullName; }
		/// <summary>
		/// 型Tの名前を取得する
		/// </summary>
		static constexpr nametype GetTypeName() noexcept {
			const char16_t* data = FullName.c_str();
			size_t length = FullName.Length();
			SplitResult result = Split(data, data + length);
			return nametype(result.b, result.e - result.b);
		}
		/// <summary>
		/// 型Tの名前空間を取得する
		/// </summary>
		static constexpr nametype GetNamespace() noexcept {
			constexpr nametype typeName = GetTypeName();
			size_t typeNameLength = typeName.Length();
			size_t fullNameLength = FullName.Length();
			const char16_t* data = FullName.c_str();
			size_t s = 0;
			for (; s < fullNameLength; ++s) if (data[s] == u' ') break;
			size_t length = fullNameLength - typeNameLength - (s + 1);
			if (length < 2) return nametype();
			else return nametype(data + s + 1, length - 2);	//最後の::を除く
		}
		/// <summary>
		/// 型Tの型指定子(struct, class union, enum)を取得する
		/// </summary>
		static constexpr TypeSpecifier GetTypeSpecifier() noexcept {
			constexpr const char16_t* data = FullName.c_str();
			if (data[0] == u'c') return TypeSpecifier::CLASS;
			else if (data[0] == u's') return TypeSpecifier::STRUCT;
			else if (data[0] == u'e') return TypeSpecifier::ENUM;
			else return TypeSpecifier::NONE;
		}
		/// <summary>
		/// 型Tの間接参照レベル(ポインタの数)を取得する
		/// </summary>
		/// <returns>
		/// 型Tがポインタでない場合、0を返す
		/// </returns>
		static constexpr size_t GetLevelsOfPointer() noexcept {
			constexpr const char16_t* data = FullName.c_str();
			size_t ret = 0;
			for (size_t i = FullName.Length(); i-- > 0;) {
				if (data[i] != u'*') break;
				else ++ret;
			}
			return ret;
		}
	};
}

//EnumName
export namespace System {
	template<class E>
	class EnumName {
	public:
		/// <summary>
		/// 列挙子名を取得する
		/// </summary>
		/// <returns></returns>
		template<E val>
		static constexpr auto GetName() noexcept {
			constexpr auto full = GetEnumName<E, val>();
			const char16_t* data = full.c_str();
			size_t length = full.Length();
			for (size_t i = length; i-- > 0;) {
				if (data[i] == u':') {
					System::Traits::remove_cv_t<decltype(full)> ret;
					ret.strcpy(data + i + 1, 0, length - i - 1);
					ret.strcpy(u'\0', length - i - 1);
					return ret;
				}
			}
			return System::Traits::remove_cv_t<decltype(full)>();
		}
	};
}
