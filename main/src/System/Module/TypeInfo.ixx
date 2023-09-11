export module TypeInfo;
import CSTDINT;
import Traits;
import StringBlock;

//View
namespace System::Internal {
	struct View {
		const char* m_begin = nullptr;
		const char* m_end = nullptr;
		size_t m_size = 0;
	public:
		constexpr View() noexcept = default;
		constexpr View(char const* pBegin, char const* pEnd, size_t size) noexcept
			: m_begin(pBegin), m_end(pEnd), m_size(size) {}
		constexpr ~View() noexcept {}
	public:
		constexpr size_t Size() const noexcept { return m_size + 1; }
		template<size_t N>
		constexpr auto GetCString() const noexcept {
			return CStringBlock<char, N>{ m_begin, Size() };
		}
	};
}
using System::Internal::View;

//TypeInfo
export namespace System {
	template<class T>
	struct TypeInfo {
	public:
		static constexpr bool HasConst = !Traits::is_same_v<T, Traits::remove_const_t<T>>;
		static constexpr bool HasVolatile = !Traits::is_same_v<T, Traits::remove_volatile_t<T>>;
		static constexpr bool IsPointer = !Traits::is_same_v<T, Traits::remove_pointer_t<T>>;
		static constexpr bool IsLValueReference = Traits::is_lvalue_reference_v<T>;
		static constexpr bool IsRValueReference = Traits::is_rvalue_reference_v<T>;
		static constexpr bool IsReference = IsLValueReference || IsRValueReference;
		static constexpr bool HasNoQualifiers = !HasConst && !HasVolatile;
		static constexpr bool IsSimpleType = HasNoQualifiers && !IsPointer && !IsReference;
		static constexpr bool IsFundamental = Traits::is_fundamental_v<T>;
		static constexpr bool IsClass = Traits::is_class_v<T>;
		static constexpr bool IsUnion = Traits::is_union_v<T>;
		static constexpr bool IsEnum = Traits::is_enum_v<T>;
		static constexpr bool IsEnumClass = Traits::is_scoped_enum_v<T>;
		static constexpr bool IsArray = Traits::is_array_v<T>;
	public:
		using remove_cv_type = Traits::remove_cv_t<T>;
		using remove_pointer_type = Traits::remove_pointer_t<T>;
		using remove_reference_type = Traits::remove_reference_t<T>;
		using remove_all_extents_type = Traits::remove_all_extents_t<T>;
		using next_type = Traits::conditional_t<
			IsPointer,
			remove_pointer_type,
			Traits::conditional_t<IsReference, remove_reference_type, remove_cv_type>
		>;
		using root_type = Traits::remove_all_specifiers_without_extents_t<T>;
	public:
		static constexpr size_t TemplateArgsCount = Traits::template_args_count_v<root_type>;
		static constexpr size_t PointerLevels = Traits::pointer_levels_v<T>;
	private:
		static consteval auto GetView() noexcept {
	#if defined(__clang__) || defined(__GNUC__)
			return View{ __PRETTY_FUNCTION__, __PRETTY_FUNCTION__ + sizeof(__PRETTY_FUNCTION__) - sizeof("]"), sizeof(__PRETTY_FUNCTION__) - sizeof("]") };
	#elif defined(_MSC_VER)
			return View{ __FUNCSIG__, __FUNCSIG__ + sizeof(__FUNCSIG__) - sizeof(">::GetView(void) noexcept"), sizeof(__FUNCSIG__) - sizeof(">::GetView(void) noexcept") };
	#else
			return View{ };
	#endif
		}
	private:
		static constexpr bool Compare(const char* lhs, const char* rhs, size_t count) noexcept {
			for (size_t i = 0; i < count; ++i) if (lhs[i] != rhs[i]) return false;
			return true;
		}
	private:/* FullName 取得用関数 */
		static consteval View TrimView_MSVC(const View& view) noexcept {
			View ret = view;
			int32_t templateCount = 0;
			//先頭をトリミングする
			for (char const* c = ret.m_end; c-- > ret.m_begin; ) {
				if (*c == '>') ++templateCount;
				else if (*c == '<') {
					--templateCount;
					if (templateCount < 0) {
						ret.m_begin = c + 1;
						constexpr size_t size[5] = { sizeof("class"), sizeof("struct"), sizeof("union"), sizeof("enum class"), sizeof("enum") };
						constexpr char const* cstr[5] = { "class", "struct", "union", "enum class", "enum" };
						for (size_t i = 0; i < 5; ++i) {
							if (Compare(ret.m_begin, cstr[i], size[i] - 1)) {
								ret.m_begin += size[i];
								break;
							}
						}
						break;
					}
				}
				else if (*c == '[' && c[1] == 'w' && c[2] == 'i' && c[3] == 't' && c[4] == 'h') {
					ret.m_begin = c + sizeof("with T = ");
					break;
				}
			}
			//末尾をトリミングする
			for (const char* c = ret.m_end; c-- > ret.m_begin;) {
				if (*c != ' ') break;
				else ret.m_end = c;
			}
			ret.m_size = ret.m_end - ret.m_begin;
			return ret;
		}
		static consteval View TrimView_Clang(const View& view) noexcept {
			View ret = view;
			const char* beg = ret.m_begin;
			size_t dif = ret.Size();
			for (size_t i = 0; i < dif; ++i) {
				if (beg[i] != '[') continue;
				else if (i + 4 >= dif) {
					ret.m_begin = ret.m_end;
					ret.m_size = 0;
					break;
				}
				else if (beg[i + 1] == 'T' && beg[i + 2] == ' ' && beg[i + 3] == '=' && beg[i + 4] == ' ') {
					ret.m_begin = beg + i + 5;
					ret.m_size -= i + 5;
					break;
				}
			}
			return ret;
		}
		static consteval View TrimView_GCC(const View& view) noexcept {
			View ret = view;
			const char* beg = ret.m_begin;
			size_t dif = ret.Size();
			for (size_t i = 0; i < dif; ++i) {
				if (beg[i] != '[') continue;
				else if (i + 9 >= dif) {
					ret.m_begin = ret.m_end;
					ret.m_size = 0;
					break;
				}
				else if (Compare(beg + i + 1, "with T = ", 9)) {
					ret.m_begin = beg + i + 10;
					ret.m_size -= i + 10;
					break;
				}
			}
			return ret;
		}
		static consteval View TrimView(const View& view) noexcept {
#if defined(__clang__)
			return TrimView_Clang(view);
#elif defined(__GNUC__)
			return TrimView_GCC(view);
#elif defined(_MSC_VER)
			return TrimView_MSVC(view);
#else
			static_assert(false, "TypeInfoクラスが対応していないコンパイラです。");
#endif
		}
	private:/* TypeName　取得用関数 */
		static constexpr auto ToTypeNameView_Clang(const View& rootFullNameView) noexcept {
			View ret = rootFullNameView;
			int32_t templateLevel = 0;
			size_t dif = ret.Size();
			const char* beg = ret.m_begin;
			for (size_t i = dif; i-- > 0;) {
				if (beg[i] == '>') ++templateLevel;
				else if (beg[i] == '<') --templateLevel;
				else if (beg[i] == ':' && templateLevel == 0) {
					if (i != 0 && beg[i - 1] == ':') {
						ret.m_begin = beg + i + 1;
						ret.m_size -= i + 1;
						break;
					}
				}
			}
			return ret;
		}
		static constexpr auto ToTypeNameView_MSVC(const View& rootFullNameView) noexcept {
			View ret = rootFullNameView;
			int32_t templateLevel = 0;
			for (const char* c = ret.m_end; c-- > ret.m_begin;) {
				if (*c == '>') ++templateLevel;
				else if (*c == '<') --templateLevel;
				else if (*c == ':' && templateLevel == 0) {
					if (c != ret.m_begin && *(c - 1) == ':') {
						ret.m_begin = c + 1;
						break;
					}
				}
			}
			ret.m_size = ret.m_end - ret.m_begin;
			return ret;
		}
		static constexpr auto ToTypeNameView(const View& rootFullNameView) noexcept {
#if defined(__clang__)
			return ToTypeNameView_Clang(rootFullNameView);
#elif defined(__GNUC__)
			return ToTypeNameView_Clang(rootFullNameView);
#elif defined(_MSC_VER) 
			return ToTypeNameView_MSVC(rootFullNameView);
#else
			static_assert(false, "TypeInfoクラスが対応していないコンパイラです。");
#endif
		}
	private:/* Namespace 所得用関数 */
		static constexpr auto ToNamespaceView(const View& rootFullNameView) noexcept {
			View ret = ToTypeNameView(rootFullNameView);
			ret.m_end = ret.m_begin - 2;
			ret.m_begin = rootFullNameView.m_begin;
			ret.m_size = rootFullNameView.m_size - ret.m_size - 2;
			return ret;
		}
	public:
		static constexpr View FullNameView = TrimView(GetView());
		static constexpr View TypeNameView = ToTypeNameView(TypeInfo<root_type>::FullNameView);
		static constexpr View NamespaceView = ToNamespaceView(TypeInfo<root_type>::FullNameView);
	public:
		static constexpr auto GetFullName() noexcept {
			return FullNameView.GetCString<FullNameView.Size()>();
		}
		static constexpr auto GetTypeName() noexcept {
			return TypeNameView.GetCString<TypeNameView.Size()>();
		}
		static constexpr auto GetNamespace() noexcept {
			return NamespaceView.GetCString<NamespaceView.Size()>();
		}
	};
}

//EnumInfo
export namespace System {
	template<Traits::Concepts::CEnum E, E val>
	struct EnumInfo {
	private:
		template<E v = val>
		static consteval View GetView() noexcept {
	#if defined(__clang__) || defined(__GNUC__)
			return TypeInfo<EnumInfo<E, val>>::TypeNameView;
	#elif defined(_MSC_VER)
			return View{ __FUNCSIG__, __FUNCSIG__ + sizeof(__FUNCSIG__) - sizeof(">(void) noexcept"), sizeof(__FUNCSIG__) - sizeof(">(void) noexcept") };
	#else
			return View{ };
	#endif
		}
	private:
		static consteval View ToValueNameView_Internal(const View& rootInfoView) noexcept {
			View ret = rootInfoView;
			const char* beg = ret.m_begin;
			size_t dif = ret.Size();
			for (size_t i = dif; i-- > 0;) {
				if (beg[i] == '>') {
					ret.m_end = beg + i - 1;
					ret.m_size -= dif - i;
					break;
				}
			}
			dif = ret.Size();
			for (size_t i = dif; i-- > 0;) {
				if (beg[i] == ':') {
					ret.m_begin = beg + i + 1;
					ret.m_size -= i;
					break;
				}
			}
			return ret;
		}
		static consteval View ToValueNameView(const View& rootInfoView) noexcept {
	#if defined(__clang__)
			return ToValueNameView_Internal(rootInfoView);
	#elif defined(__GNUC__)
			return ToValueNameView_Internal(rootInfoView);
	#elif defined(_MSC_VER)
			return ToValueNameView_Internal(rootInfoView);
	#else
			static_assert(false, "EnumInfoクラスが対応していないコンパイラです。");
	#endif
		}
	private:
		static constexpr View ValueNameView = ToValueNameView(GetView());
	public:
		static constexpr auto GetEnumName() noexcept {
			return TypeInfo<E>::GetTypeName();
		}
		static constexpr auto GetValueName() noexcept {
			return ValueNameView.GetCString<ValueNameView.Size()>();
		}
	};
}
