export module TypeName;
import CSTDINT;
import Traits;
import StringBlock;

export namespace System {
	/// <summary>
	/// TypeName::GetTypeSpecifier()�֐����Ԃ��^�w��q��\���񋓌^
	/// </summary>
	enum class TypeSpecifier : uint8_t {
		NONE = 0,
		STRUCT,
		CLASS,
		ENUM
	};
}

namespace System {
	namespace TypeName_Internal {
		template<size_t size, size_t N>
		constexpr CStringBlock<char16_t, size> TrimName(const char(&name)[size], const char(&funcname)[N]) noexcept {
			size_t e = size;
			for (; e-- > 0;) if (name[e] == '>') break;
			if (e == 0) return CStringBlock<char16_t, size>();
			size_t s = 0;
			for (; s < e; ++s) {
				if (name[s] == funcname[0]) {
					size_t n = s;
					for (const char& c : funcname) {
						if (name[n] == c) ++n;
						else break;
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
			for (size_t i = 0, count = e - s + 1; i < count; ++i) ret.strcpy(static_cast<char16_t>(name[s + i]), i);
			ret.strcpy(u'\0', e - s + 1);
			return ret;
		}
		template<class T>
		constexpr auto GetTypeName() { return TrimName(__FUNCSIG__, "GetTypeName"); }
		template<class E, E V>
		constexpr auto GetEnumName() { return TrimName(__FUNCSIG__, "GetEnumName"); }
	}
}

export namespace System {
	using namespace TypeName_Internal;
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
		/// �^T�̖��O��ԂŏC�����ꂽ���O���擾����
		/// </summary>
		static constexpr const nametype& GetFullName() noexcept { return FullName; }
		/// <summary>
		/// �^T�̖��O���擾����
		/// </summary>
		static constexpr nametype GetTypeName() noexcept {
			const char16_t* data = FullName.c_str();
			size_t length = FullName.Length();
			SplitResult result = Split(data, data + length);
			return nametype(result.b, result.e - result.b);
		}
		/// <summary>
		/// �^T�̖��O��Ԃ��擾����
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
			else return nametype(data + s + 1, length - 2);	//�Ō��::������
		}
		/// <summary>
		/// �^T�̌^�w��q(struct, class union, enum)���擾����
		/// </summary>
		static constexpr TypeSpecifier GetTypeSpecifier() noexcept {
			constexpr const char16_t* data = FullName.c_str();
			if (data[0] == u'c') return TypeSpecifier::CLASS;
			else if (data[0] == u's') return TypeSpecifier::STRUCT;
			else if (data[0] == u'e') return TypeSpecifier::ENUM;
			else return TypeSpecifier::NONE;
		}
		/// <summary>
		/// �^T�̊ԐڎQ�ƃ��x��(�|�C���^�̐�)���擾����
		/// </summary>
		/// <returns>
		/// �^T���|�C���^�łȂ��ꍇ�A0��Ԃ�
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

export namespace System {
	template<class E>
	class EnumName {
	public:
		/// <summary>
		/// �񋓎q�����擾����
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