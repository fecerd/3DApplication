module;
#include <concepts>
#include <iterator>
#include <type_traits>
export module Traits;
//import <type_traits>;
export import CSTDINT;

export namespace System {
	//[type_traits]互換：型変換
	namespace Traits {
		template<typename T> struct remove_const { using type = T; };
		template<typename T> struct remove_const<const T> { using type = T; };
		template<typename T> struct remove_volatile { using type = T; };
		template<typename T> struct remove_volatile<volatile T> { using type = T; };
		template<typename T> struct remove_cv { using type = remove_const<typename remove_volatile<T>::type>::type; };
		template<typename T> struct add_const { using type = const T; };
		template<typename T> struct add_volatile { using type = volatile T; };
		template<typename T> struct add_cv { using type = add_const<typename add_volatile<T>::type>::type; };
		template<typename T> struct remove_reference { using type = T; };
		template<typename T> struct remove_reference<T&> { using type = T; };
		template<typename T> struct remove_reference<T&&> { using type = T; };
		template<typename T> struct add_lvalue_reference { using type = typename remove_reference<T>::type&; };
		template<typename T> struct add_rvalue_reference { using type = T&&; };
		template<> struct add_rvalue_reference<void> { using type = void; };
		template<typename T> struct add_pointer { using type = typename remove_reference<T>::type*; };
		template<typename T> struct remove_pointer { using type = T; };
		template<typename T> struct remove_pointer<T*> { using type = T; };
	}
	namespace Traits {
		template<typename T> using remove_const_t = remove_const<T>::type;
		template<typename T> using remove_volatile_t = remove_volatile<T>::type;
		template<typename T> using remove_cv_t = remove_cv<T>::type;
		template<typename T> using add_const_t = add_const<T>::type;
		template<typename T> using add_volatile_t = add_volatile<T>::type;
		template<typename T> using add_cv_t = add_cv<T>::type;
		template<typename T> using remove_reference_t = remove_reference<T>::type;
		template<typename T> using add_lvalue_reference_t = add_lvalue_reference<T>::type;
		template<typename T> using add_rvalue_reference_t = add_rvalue_reference<T>::type;
		template<typename T> using add_pointer_t = add_pointer<T>::type;
		template<typename T> using remove_pointer_t = remove_pointer<T>::type;
		template<typename ...Args> using void_t = void;
		template<class T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;
	}
	//[type_traits]互換：ヘルパー型
	namespace Traits {
		template<bool B, class T, class F> struct conditional { using type = T; };
		template<class T, class F> struct conditional<true, T, F> { using type = T; };
		template<class T, class F> struct conditional<false, T, F> { using type = F; };
		template<class T, T v>
		struct integral_constant {
			static constexpr T value = v;
			using value_type = T;
			using type = integral_constant<T, v>;
		public:
			constexpr operator value_type() noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; }
		};
		template<bool B, typename T> struct enable_if {};
		template<typename T> struct enable_if<true, T> { using type = T; };
	}
	namespace Traits {
		template<bool B, class T, class F> using conditional_t = conditional<B, T, F>::type;
		template<bool B> using bool_constant = integral_constant<bool, B>;
		using true_type = bool_constant<true>;
		using false_type = bool_constant<false>;
		template<bool B, typename T> using enable_if_t = enable_if<B, T>::type;
	}
	//[type_traits]互換：型同士の判定
	namespace Traits {
		//第一テンプレート引数T型(CV修飾含む)が第二テンプレート引数以降すべてと一致しているか判定
		template<class T1, class T2, class ...Args> struct is_same : false_type {};
		template<class T1, class ...Args> struct is_same<T1, T1, Args...> : is_same<T1, Args...> {};
		template<class T> struct is_same<T, T> : true_type {};
		//第一テンプレート引数T型(CV修飾含む)が第二テンプレート引数以降に含まれているか判定
		template<class T, class First, class ...Args> struct is_any_of : is_any_of<T, Args...> {};
		template<class T, class ...Args> struct is_any_of<T, T, Args...> : true_type {};
		template<class T1, class T2> struct is_any_of<T1, T2> : false_type {};
		template<class T> struct is_any_of<T, T> : true_type {};
	}
	namespace Traits {
		template<class T1, class T2, class ...Args> inline constexpr bool is_same_v = is_same<T1, T2, Args...>::value;
		template<class T, class First, class ...Args> inline constexpr bool is_any_of_v = is_any_of<T, First, Args...>::value;
	}
	//[type_traits]互換：評価されない式で右辺値を使用する
	namespace Traits {
		//評価されない式で右辺値を使用する
		template<typename T> typename add_rvalue_reference<T>::type declval() noexcept;
	}
	//[type_traits]互換：型判定
	namespace Traits {
		template<typename T> struct is_pointer_internal : false_type {};
		template<typename T> struct is_pointer_internal<T*> : true_type {};
		template<typename T, typename = void> struct is_member_object_pointer_internal : false_type {};
		template<typename T, typename M> struct is_member_object_pointer_internal<M T::*> : true_type {};
		template<typename T, typename = void> struct is_member_function_pointer_internal : false_type {};
		template<typename T, typename M> struct is_member_function_pointer_internal<M(T::*)()> : true_type {};
		template<typename T, typename = void> struct is_class_or_union : false_type {};
		template<typename T> struct is_class_or_union<T, void_t<int T::*>> : true_type {};
	}
	namespace Traits {
		template<class T> struct is_const : false_type {};
		template<class T> struct is_const<const T> : true_type {};
		template<typename T> struct is_void : bool_constant<is_same<typename remove_cv<typename remove_reference<T>::type>::type, void>::value> {};
		template<typename T> struct is_null_pointer : bool_constant<is_same<typename remove_cv<T>::type, std::nullptr_t>::value> {};
		template<typename T>
		struct is_integral : bool_constant<is_any_of<typename remove_cv<T>::type, bool, char8_t, char16_t, char32_t, wchar_t, int, unsigned int, char, signed char, unsigned char, short, unsigned short, long, unsigned long, long long, unsigned long long>::value> {};
		template<typename T> struct is_floating_point : bool_constant<is_any_of<typename remove_cv<T>::type, float, double, long double>::value> {};
		template<typename T> struct is_array : false_type {};
		template<typename T> struct is_array<T[]> : true_type {};
		template<typename T, size_t N> struct is_array<T[N]> : true_type {};
		template<typename T> struct is_pointer : is_pointer_internal<typename remove_cv<T>::type> {};
		template<typename T> struct is_lvalue_reference : false_type {};
		template<typename T> struct is_lvalue_reference<T&> : true_type {};
		template<typename T> struct is_rvalue_reference : false_type {};
		template<typename T> struct is_rvalue_reference<T&&> : true_type {};
		template<typename T, typename = void> struct is_member_object_pointer : false_type {};
		template<typename T, typename M> struct is_member_object_pointer<M T::*> : is_member_object_pointer_internal<typename remove_cv<M>::type T::*> {};
		template<typename T, typename M> struct is_member_object_pointer<M T::* const> : is_member_object_pointer_internal<typename remove_cv<M>::type T::*> {};
		template<typename T, typename M> struct is_member_object_pointer<M T::* volatile> : is_member_object_pointer_internal<typename remove_cv<M>::type T::*> {};
		template<typename T, typename M> struct is_member_object_pointer<M T::* const volatile> : is_member_object_pointer_internal<typename remove_cv<M>::type T::*> {};
		template<typename T, typename = void> struct is_member_function_pointer : false_type {};
		template<typename T, typename M> struct is_member_function_pointer<M(T::*)()> : is_member_function_pointer_internal<typename remove_cv<M>::type(T::*)()> {};
		template<typename T, typename M> struct is_member_function_pointer<M(T::* const)()> : is_member_function_pointer_internal<typename remove_cv<M>::type(T::*)()> {};
		template<typename T, typename M> struct is_member_function_pointer<M(T::* volatile)()> : is_member_function_pointer_internal<typename remove_cv<M>::type(T::*)()> {};
		template<typename T, typename M> struct is_member_function_pointer<M(T::* const volatile)()> : is_member_function_pointer_internal<typename remove_cv<M>::type(T::*)()> {};
		template<typename T> struct is_reference : bool_constant<is_lvalue_reference<T>::value || is_rvalue_reference<T>::value> {};
		template<typename T> struct is_member_pointer : bool_constant<is_member_object_pointer<T>::value || is_member_function_pointer<T>::value> {};
		template<typename T> struct is_function : false_type {};
		template<typename T, typename ...Args> struct is_function<T(Args...)> : true_type {};
		template<typename T> struct is_enum : bool_constant<!(is_void<T>::value || is_integral<T>::value || is_floating_point<T>::value || is_array<T>::value || is_pointer<T>::value || is_reference<T>::value || is_member_pointer<T>::value || is_class_or_union<T>::value || is_function<T>::value)> {};
		template<typename T> struct is_arithmetic : bool_constant<is_integral<T>::value || is_floating_point<T>::value> {};
		template<typename T> struct is_fundamental : bool_constant<is_arithmetic<T>::value || is_void<T>::value || is_null_pointer<T>::value> {};
		template<typename T> struct is_scaler : bool_constant<is_arithmetic<T>::value || is_enum<T>::value || is_pointer<T>::value || is_member_pointer<T>::value || is_null_pointer<T>::value> {};
		template<typename T> struct is_object : bool_constant<is_scaler<T>::value || is_array<T>::value || is_class_or_union<T>::value> {};
		template<typename T> struct is_compound : bool_constant<!is_fundamental<T>::value> {};
		template<class From, class To, bool b = is_void<From>::value || is_function<To>::value || is_array<To>::value>
		struct is_convertible : bool_constant<is_void<To>::value> {};
		template<class From, class To>
		struct is_convertible<From, To, false> {
		private:
			template<typename T> static void implicit_convert(T);
			template<class F, class T> static decltype(is_convertible::implicit_convert<T>(declval<F>()), uint32_t()) sfinae_func(int);
			template<class, class> static uint8_t sfinae_func(...);
		public:
			static constexpr bool value = sizeof(is_convertible::sfinae_func<From, To>(0)) == sizeof(uint32_t);
		};
		template<typename T> struct is_scoped_enum : bool_constant<is_enum<T>::value && !(is_convertible<T, int>::value)> {};
		template<typename T, typename = void> struct is_unsigned : false_type {};
		template<typename T> requires(is_arithmetic<T>::value) struct is_unsigned<T, void_t<T>> : bool_constant < static_cast<T>(0) < static_cast<T>(-1) > {};
		template<typename T, typename = void> struct is_signed : false_type {};
		template<typename T> requires(is_arithmetic<T>::value) struct is_signed<T, void_t<T>> : bool_constant < static_cast<T>(-1) < static_cast<T>(0) > {};
		template<class Base, class Derived, class = void> struct is_base_of : false_type {};
		template<class Base, class Derived> requires requires(Base* b, Derived* d) {
			b = d;
			d = static_cast<Derived*>(b);
		}
		struct is_base_of<Base, Derived, void_t<Base, Derived>> : true_type {};
		template<class F, class ...Args>
		struct is_invocable : System::Traits::false_type {};
		template<class F, class ...Args>
		requires requires() { System::Traits::declval<remove_cv_t<F>>()(System::Traits::declval<Args>()...); }
		struct is_invocable<F, Args...> : System::Traits::true_type {};
		template<class R, class F, class ...Args>
		struct is_invocable_r : false_type {};
		template<class R, class F, class ...Args>
		requires requires() { is_convertible_v<decltype(declval<remove_cv_t<F>>()(declval<Args>()...)), R>; }
		struct is_invocable_r<R, F, Args...> : true_type {};
		template<class T>
		struct is_default_constructible : false_type{};
		template<class T>
		requires requires() { T(); }
		struct is_default_constructible<T> : true_type {};
		template<class T>
		struct is_copy_constructible : false_type {};
		template<class T>
		requires requires(const T& x) { T(x); }
		struct is_copy_constructible<T> : true_type {};
		template<class T>
		struct is_move_constructible : false_type {};
		template<class T>
		requires requires(T&& x) { T(static_cast<T&&>(x)); }
		struct is_move_constructible<T> : true_type {};
	}
	namespace Traits {
		template<class C> using is_class = std::is_class<C>;
		template<class U> using is_union = std::is_union<U>;
	}
	namespace Traits {
		template<typename T> inline constexpr bool is_const_v = is_const<T>::value;
		template<typename T> inline constexpr bool is_void_v = is_void<T>::value;
		template<typename T> inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
		template<typename T> inline constexpr bool is_integral_v = is_integral<T>::value;
		template<typename T> inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
		template<typename T> inline constexpr bool is_array_v = is_array<T>::value;
		template<typename T> inline constexpr bool is_pointer_v = is_pointer<T>::value;
		template<typename T> inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
		template<typename T> inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
		template<typename T> inline constexpr bool is_reference_v = is_reference<T>::value;
		template<typename T> inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;
		template<typename T> inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;
		template<typename T> inline constexpr bool is_enum_v = is_enum<T>::value;
		template<class C> inline constexpr bool is_class_v = is_class<C>::value;
		template<class U> inline constexpr bool is_union_v = is_union<U>::value;
		template<typename T> inline constexpr bool is_function_v = is_function<T>::value;
		template<typename T> inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
		template<typename T> inline constexpr bool is_fundamental_v = is_fundamental<T>::value;
		template<typename T> inline constexpr bool is_object_v = is_object<T>::value;
		template<typename T> inline constexpr bool is_scaler_v = is_scaler<T>::value;
		template<typename T> inline constexpr bool is_compound_v = is_compound<T>::value;
		template<typename T> inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;
		template<typename From, typename To> inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
		template<typename T> inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
		template<typename T> inline constexpr bool is_signed_v = is_signed<T>::value;
		template<class Base, class Derived> inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
		template<class F, class ...Args> inline constexpr bool is_invocable_v = is_invocable<F, Args...>::value;
		template<class R, class F, class ...Args> inline constexpr bool is_invocable_r_v = is_invocable_r<R, F, Args...>::value;
		template<class T> inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
		template<class T> inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
		template<class T> inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
	}
	//完全転送
	namespace Traits {
		template<class T>
		constexpr T&& forward(remove_reference_t<T>& arg) noexcept { return static_cast<T&&>(arg); }
		template<class T>
		constexpr T&& forward(remove_reference_t<T>&& arg) noexcept { return static_cast<T&&>(arg); }
	}
	//[固有]：テンプレート整数型
	namespace Traits {
		template<bool, size_t>
		struct int_type {
			using type = void;
			using next = void;
		};
		template<>
		struct int_type<true, sizeof(int8_t)> {
			using type = int8_t;
			using next = int16_t;
		};
		template<>
		struct int_type<true, sizeof(int16_t)> {
			using type = int16_t;
			using next = int32_t;
		};
		template<>
		struct int_type<true, sizeof(int32_t)> {
			using type = int32_t;
			using next = int64_t;
		};
		template<>
		struct int_type<true, sizeof(int64_t)> {
			using type = int64_t;
			using next = int64_t;
		};
		template<>
		struct int_type<false, sizeof(uint8_t)> {
			using type = uint8_t;
			using next = uint16_t;
		};
		template<>
		struct int_type<false, sizeof(uint16_t)> {
			using type = uint16_t;
			using next = uint32_t;
		};
		template<>
		struct int_type<false, sizeof(uint32_t)> {
			using type = uint32_t;
			using next = uint64_t;
		};
		template<>
		struct int_type<false, sizeof(uint64_t)> {
			using type = uint64_t;
			using next = uint64_t;
		};
	}
	namespace Traits {
		//符号の有無と型サイズから指定される整数型
		//F：true -> 符号付き, false -> 符号なし
		//N：バイト数
		template<bool isSigned, size_t Byte> using int_t = typename int_type<isSigned, Byte>::type;
		//int_t<isSigned, Byte>の次に大きいサイズの整数型
		template<bool isSigned, size_t Byte> using int_next = typename int_type<isSigned, Byte>::next;
	}
	//[type_traits]互換：列挙型の基底型変換
	namespace Traits {
		template<typename EnumType, bool b = is_enum<EnumType>::value>
		struct underlying_type {};
		template<typename EnumType>
		struct underlying_type<EnumType, true> {
			using type = int_type<(static_cast<EnumType>(-1) < static_cast<EnumType>(1)), sizeof(EnumType)>::type;
		};
	}
	namespace Traits {
		template<typename E> using underlying_type_t = typename underlying_type<E>::type;
	}
	//[固有]：非型テンプレート比較
	namespace Traits {
		//N1 < N2
		template<typename T, T N1, T N2> inline constexpr bool is_less_v = N1 < N2;
		//N1 == N2
		template<typename T, T N1, T N2> inline constexpr bool is_eq_v = N1 == N2;
		//N1 > N2
		template<typename T, T N1, T N2> inline constexpr bool is_greater_v = !(N1 < N2) && !(N1 == N2);
		//N1 <= N2
		template<typename T, T N1, T N2> inline constexpr bool is_less_eq_v = is_less_v<T, N1, N2> || is_eq_v<T, N1, N2>;
		//N1 >= N2
		template<typename T, T N1, T N2> inline constexpr bool is_greater_eq_v = is_greater_v<T, N1, N2> || is_eq_v<T, N1, N2>;
	}
	//[固有]：型サイズ比較
	namespace Traits {
		template<typename T, typename U, bool F = sizeof(T) >= sizeof(U)>
		struct compare_type {
			using greater = T;
			using less = U;
		};
		template<typename T, typename U>
		struct compare_type<T, U, false> {
			using greater = U;
			using less = T;
		};
	}
	namespace Traits {
		//T型とU型のうち、型サイズが大きい型
		//ただし、型サイズが等しい場合、T型
		template<typename T, typename U> using greater_t = typename compare_type<T, U>::greater;
		//T型とU型のうち、型サイズが小さい型
		//ただし、型サイズが等しい場合、U型
		template<typename T, typename U> using less_t = typename compare_type<T, U>::less;
	}
	//[固有]：2つの型を持つ共用体テンプレート
	namespace Traits {
		//T型メンバfirstとU型メンバsecondを持つ共用体
		template<typename T, typename U>
		union UnionSet {
			T first;
			U second;
		};
	}
	//[固有]：浮動小数点型ビット情報
	namespace Traits {
		//IEEE-754に準拠した浮動小数点数型の指数部ビット数
		template<typename T> inline constexpr int EXP_DIGIT = T::exponent;
		template<> inline constexpr int EXP_DIGIT<float> = 8;
		template<> inline constexpr int EXP_DIGIT<double> = 11;
		//IEEE-754に準拠した浮動小数点数型の仮数部ビット数
		template<typename T> inline constexpr int FRAC_DIGIT = T::fraction;
		template<> inline constexpr int FRAC_DIGIT<float> = 23;
		template<> inline constexpr int FRAC_DIGIT<double> = 52;
		//IEEE-754に準拠した浮動小数点数型の指数部にかかるバイアス値の絶対値
		template<typename T> inline constexpr int EXP_BIAS = (1ull << (EXP_DIGIT<T> -1)) - 1;
	}
	//[固有]：ビットマスク
	namespace Traits {
		//下位Nビットが立ったsize_t型のビットマスク定数
		//BITMASK_M<1, N>と等しい
		template<unsigned int N> inline constexpr uint64_t BITMASK_L = (1ull << (N - 1)) | BITMASK_L<N - 1>;
		template<> inline constexpr uint64_t BITMASK_L<0> = 0;
		//LSBを1ビット目として、Lビット目からHビット目までのビットが立ったsize_t型のビットマスク定数
		//テンプレート引数はL <= Hでなければならない
		template<unsigned int L, unsigned int H> inline constexpr uint64_t BITMASK_M = (1ull << (H - 1)) | BITMASK_M<L, H - 1>;
		template<unsigned int L> inline constexpr uint64_t BITMASK_M<L, L> = (1ull << (L - 1));
	}
	//[固有]：型選択テンプレート
	namespace Traits {
		//Args...からi番目の型を取得
		template<size_t i, class ...Args> struct one_of {
			using type = void;
		};
		template<size_t i, class Head, class ...Tail>
		struct one_of<i, Head, Tail...> : one_of<i - 1, Tail...> {};
		template<class Head, class ...Args>
		struct one_of<0, Head, Args...> {
			using type = Head;
		};
	}
	namespace Traits {
		//パラメータリストArgsのi番目の型(ゼロインデックス)
		template<size_t i, typename ...Args> using one_of_t = one_of<i, Args...>::type;
	}
	//[固有] : 複合要件で使用できるコンセプト({ 式 } -> concept;)
	namespace Traits::Concepts {
		template<class T1, class T2> concept same_as = std::same_as<T1, T2>;
		template<class From, class To> concept convertible_to = std::convertible_to<From, To>;
	}
	//[固有]：コンセプト
	namespace Traits {
		template<class T> concept Unsigned = is_unsigned_v<T>;
		template<class T> concept Signed = is_signed_v<T>;
		template<class T> concept Integral = is_integral_v<T>;
		template<class T> concept Floating = is_floating_point_v<T>;
		template<class T> concept Arithmetic = Integral<T> || Floating<T>;
		template<class T> concept CharType = is_any_of_v<T, char, char8_t, char16_t, char32_t, wchar_t>;
		template<class T> concept Number = Arithmetic<T> && !CharType<T>;// is_any_of_v<T, int, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, float, double>;
		template<class T> concept Pointer = is_pointer_v<T>;
		template<class T> concept Enum = is_enum_v<T>;
		template<class Src, class Dst>
		concept CopyConstructible = requires(const remove_cvref_t<Src>& x) {
			remove_cvref_t<Dst>(x);
		};
		template<class Src, class Dst>
		concept MoveConstructible = requires(remove_cvref_t<Src>&& x) {
			remove_cvref_t<Dst>(static_cast<remove_cvref_t<Src>&&>(x));
		};
		template<class Src, class Dst>
		concept Constructible = CopyConstructible<Src, Dst> || MoveConstructible<Src, Dst>;

		template<class T>
		struct array_to_ptr {
			using type = T;
			static constexpr size_t Length = 0;
		};
		template<class T>
		struct array_to_ptr<T[]> {
			using type = T*;
			static constexpr size_t Length = 0;
		};
		template<class T, size_t N>
		struct array_to_ptr<T[N]> {
			using type = T*;
			static constexpr size_t Length = N;
		};
		template<class T> using array_to_ptr_t = array_to_ptr<T>::type;
		template<class Dst, class Src>
		concept ConvertibleFromPointer = requires() {
			Pointer<Src>;
			Traits::remove_reference_t<Dst>(Traits::declval<Src>());
		};
		template<class Dst, class Src>
		concept Convertible = requires() {
			Traits::remove_reference_t<Dst>(Traits::declval<Src>());
		};

		template<class Dst, class Src>
		concept ConvertibleEx = requires() {
			Convertible<Dst, Src> || ConvertibleFromPointer<Dst, Traits::array_to_ptr_t<Src>>;
		};
	}
	//[固有] : コンセプト
	namespace Traits::Concepts {
		template<class T> concept CUnsigned = is_unsigned_v<T>;
		template<class T> concept CSigned = is_signed_v<T>;
		template<class T> concept CIntegral = is_integral_v<T>;
		template<class T> concept CFloating = is_floating_point_v<T>;
		template<class T> concept CArithmetic = Integral<T> || Floating<T>;
		template<class T> concept CCharType = is_any_of_v<T, char, char8_t, char16_t, char32_t, wchar_t>;
		template<class T> concept CNumber = Arithmetic<T> && !CharType<T>;// is_any_of_v<T, int, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, float, double>;
		template<class T> concept CPointer = is_pointer_v<T>;
		template<class T> concept CEnum = is_enum_v<T>;
		template<class T, class U> concept CSame = is_same_v<T, U>;
		template<class T> concept CBitFlag = CIntegral<T> || CEnum<T>;
		template<class T> concept CDefaultConstructible = is_default_constructible_v<T>;
		template<class T> concept CCopyConstructible = is_copy_constructible_v<T>;
		template<class T> concept CMoveConstructible = is_move_constructible_v<T>;
		template<class From, class To> concept CConvertibleTo = is_convertible_v<From, To>;
		template<class To, class From> concept CConvertibleFrom = is_convertible_v<From, To>;
		template<class Src, class Dst>
		concept CCopyConstructibleTo = requires(const remove_cvref_t<Src>& x) {
			remove_cvref_t<Dst>(x);
		};
		template<class Src, class Dst>
		concept CMoveConstructibleTo = requires(remove_cvref_t<Src>&& x) {
			remove_cvref_t<Dst>(static_cast<remove_cvref_t<Src>&&>(x));
		};
		template<class Dst, class Src>
		concept CCopyConstructibleFrom = CCopyConstructibleTo<Src, Dst>;
		template<class Dst, class Src>
		concept CMoveConstructibleFrom = CMoveConstructibleTo<Src, Dst>;
		template<class T>
		concept CIncrementable = requires(T & x) {
			{ ++x } -> same_as<T&>;
			{ x++ } -> same_as<T>;
		};
		template<class T>
		concept CDecrementable = requires(T & x) {
			{ --x } -> same_as<T&>;
			{ x-- } -> same_as<T>;
		};
	}
	//[固有] : メンバポインタ型
	namespace Traits {
		template<class R, class T> using MemberPtr = R typename T::*;
	}
	//[固有] : 関数型(function_t)
	namespace Traits {
		//args
		namespace Internal {
			struct args_void {
				using type = void;
				static constexpr bool exists = false;
			};
			template<size_t i, class Head, class ...Args>
			struct args : Traits::conditional_t<sizeof...(Args) == 1, Traits::conditional_t<i == 1, args<i - 1, Args...>, args_void>, args<i - 1, Args...>> {};
			template<class Head, class ...Args>
			struct args<0, Head, Args...> {
				using type = Head;
				static constexpr bool exists = true;
			};
			template<class L, class R, class ...Args>
			auto get_return(R(L::*)(Args...) const)->R;
			template<class L, class R, class ...Args>
			auto get_return(R(L::*)(Args...))->R;
			template<size_t i, class L, class R, class ...Args>
			auto get_args(R(L::*)(Args...) const)->args<i, Args...>;
			template<class R, class ...Args>
			using func_pointer = R(*)(Args...);
			template<class L, class R, class ...Args>
			func_pointer<R, Args...> get_funcptr(R(L::*)(Args...) const);
			template<class L, class R, class ...Args>
			func_pointer<R, Args...> get_funcptr(R(L::*)(Args...));
		}
		using namespace Internal;
		template<class T>
		struct function_type {
			using return_type = void;
			template<size_t i>
			using args_type = void;
			template<size_t i>
			static constexpr bool exists_arg_v = false;
		};
		template<class T> requires requires(T x) { x.operator(); }
		struct function_type<T> {
			using type = Traits::remove_pointer_t<typename decltype(get_funcptr(&T::operator()))>;
			using return_type = typename decltype(get_return(&T::operator()));
			template<size_t i>
			using args_type = typename decltype(get_args<i>(&T::operator()))::type;
			template<size_t i>
			static constexpr bool exists_arg_v = decltype(get_args<i>(&T::operator()))::exists;
		};
		template<class R, class ...Args>
		struct function_type<R(Args...)> {
			using type = R(Args...);
			using return_type = R;
			template<size_t i>
			using args_type = args<i, Args...>::type;
			template<size_t i>
			static constexpr bool exists_arg_v = args<i, Args...>::exists;
		};
		template<class T>
		using function_t = function_type<T>::type;
	}
	//[固有] : パラメータパック型(parameter_pack)
	namespace Traits {
		template<class ...Args> struct parameter_pack {
			static constexpr size_t count = 0;
			static constexpr bool empty = true;
			using head_t = void;
		};
		template<class Head, class ...Args>
		struct parameter_pack<Head, Args...> {
			static constexpr size_t count = sizeof...(Args);
			static constexpr bool empty = false;
			using head_t = Head;
			using next_t = parameter_pack<Args...>;
		};
		template<class Head>
		struct parameter_pack<Head> {
			static constexpr size_t count = 0;
			static constexpr bool empty = false;
			using head_t = Head;
		};
	}
	//[固有] : パラメータパック操作
	namespace Traits {
		template<size_t N, class P> struct at { using type = void; };
		template<size_t N, class T, class ...Args>
		struct at<N, parameter_pack<T, Args...>> : public at<N - 1, parameter_pack<Args...>> {
			static_assert(N - 1 < sizeof...(Args), "parameter_pack at: out of range");
		};
		template<class T, class ...Args>
		struct at<0, parameter_pack<T, Args...>> {
			using type = T;
		};

		template<class T, class P> struct push_front;
		template<class T, class ...Args>
		struct push_front<T, parameter_pack<Args...>> {
			using type = parameter_pack<T, Args...>;
		};
		template<class T, class ...Args>
		using push_front_param = typename push_front<T, parameter_pack<Args...>>::type;
		template<class P> struct pop_front;
		template<class T, class ...Args>
		struct pop_front<parameter_pack<T, Args...>> {
			using type = parameter_pack<Args...>;
		};
		template<class T, class ...Args>
		using pop_front_param = typename pop_front<parameter_pack<T, Args...>>::type;
		template<class T, class P> struct push_back;
		template<class T, class ...Args>
		struct push_back<T, parameter_pack<Args...>> {
			using type = parameter_pack<Args..., T>;
		};
		template<class T, class ...Args>
		using push_back_param = typename push_back<T, parameter_pack<Args...>>::type;
		template<class P> struct pop_back;
		template<class T, class ...Args>
		struct pop_back<parameter_pack<T, Args...>> {
			using type = push_front<T, typename pop_back<parameter_pack<Args...>>::type>::type;
		};
		template<class T>
		struct pop_back<parameter_pack<T>> {
			using type = parameter_pack<>;
		};
		template<class T, class ...Args>
		using pop_back_param = typename pop_back<parameter_pack<T, Args...>>::type;
		template<class P1, class P2> struct connect;
		template<class ...Args1, class ...Args2>
		struct connect<parameter_pack<Args1...>, parameter_pack<Args2...>> {
			using type = parameter_pack<Args1..., Args2...>;
		};
		template<class P1, class P2>
		using connect_param = typename connect<P1, P2>::type;
		template<class Sentinel, class P> struct split;
		template<class Sentinel, class Head, class ...Args>
		struct split<Sentinel, parameter_pack<Head, Args...>> {
			using type = push_front<Head, typename split<Sentinel, parameter_pack<Args...>>::type>::type;
		};
		template<class Sentinel, class ...Args>
		struct split<Sentinel, parameter_pack<Sentinel, Args...>> {
			using type = parameter_pack<>;
		};
		template<class Sentinel>
		struct split<Sentinel, parameter_pack<>> {
			using type = parameter_pack<>;
		};
		template<class P> struct to_functor;
		template<class R, class ...Args>
		struct to_functor<parameter_pack<R, Args...>> {
			using type = R(Args...);
		};
		template<class Sentinel, class P>
		using split_param = typename split<Sentinel, P>::type;
	}
	//[固有] : パラメータパック判定
	namespace Traits {
		template<class P1, class P2>
		consteval bool same_pack() noexcept {
			if constexpr (P1::count != P2::count) return false;
			else if constexpr (System::Traits::is_same_v<typename P1::head_t, typename P2::head_t>) {
				if constexpr (P1::count == 0 && P2::count == 0) return true;
				else return same_pack<typename P1::next_t, typename P2::next_t>();
			}
			else return false;
		}
		template<class From, class To>
		consteval bool convertible_pack() noexcept {
			if constexpr (From::count != To::count) return false;
			else if constexpr (Concepts::convertible_to<typename From::head_t, typename To::head_t>) {
				if constexpr (From::count == 0 && To::count == 0) return true;
				else return convertible_pack<typename From::next_t, typename To::next_t>();
			}
			else return false;
		}
		template<class From, class To>
		consteval bool implicit_convertible_pack() noexcept {
			if constexpr (From::count != To::count) return false;
			else if constexpr (From::empty || To::empty) {
				return From::empty && To::empty;
			}
			else if constexpr (System::Traits::is_convertible_v<typename From::head_t, typename To::head_t>) {
				if constexpr (From::count == 0 && To::count == 0) return true;
				else return implicit_convertible_pack<typename From::next_t, typename To::next_t>();
			}
			else return false;
		}
	}
	//[固有] : 関数コンセプト
	namespace Traits::Concepts {
		template<class F, class T>
		concept CConditionFunction = requires(F cond, const T & obj) {
			{cond(obj)} -> same_as<bool>;
		};
		template<class F, class T>
		concept CCompareFunction = requires(F comp, const T & obj) {
			{comp(obj, obj)} -> same_as<strong_ordering>;
		};
	}
	//[固有] : Invokableコンセプト(parameter_packの使用例)
	namespace Traits::Concepts {
		template<class F, class ...Args>
		concept Invokable_Internal = requires(F f, Args ...args) {
			f(args...);
		};
		template<class F, class ...Args>
		concept Invokable = requires(F x, split_param<void, parameter_pack<Args...>> param) {
			requires[]<template<class ...> class P, class ...A>(F f, P<A...> param) consteval { return Invokable_Internal<F, A...>; }(x, param);
		};

		template<class F, class R, class ...Args>
		concept Invokable_R_Internal = requires(F f, Args ...args) {
			{f(args...)} -> same_as<R>;
		};
		template<class F, class R, class ...Args>
		concept Invokable_R = requires(F x, split_param<void, parameter_pack<Args...>> param) {
			requires[]<template<class ...> class P, class ...A>(F f, P<A...> param) consteval { return Invokable_R_Internal<F, R, A...>; }(x, param);
		};
	}
	//イテレータ関連
	namespace Traits {
		namespace Internal {
			template<class T> using with_reference = T&;
		}
		namespace Concepts {
			template<class T>
			concept CCanReference = requires{
				typename Internal::with_reference<T>;
			};
		}
		template<class T>
		struct is_deferenceable : false_type {};
		template<class T>
		requires requires(T& x) { { *x } -> Concepts::CCanReference; }
		struct is_deferenceable<T> : true_type {};
		template<class T>
		inline constexpr bool is_deferenceable_v = is_deferenceable<T>::value;
		namespace Concepts {
			template<class T>
			concept CDereferenceable = is_deferenceable_v<T>;
		}
		template<class T>
		struct iterator_value { using type = T; };
		template<Concepts::CDereferenceable T>
		struct iterator_value<T> {
			using type = remove_cvref_t<decltype(*declval<T>())>;
		};
		template<class T>
		using iterator_value_t = iterator_value<T>::type;
		template<class T>
		struct iterator_dereference_value { using type = T; };
		template<Concepts::CDereferenceable T>
		struct iterator_dereference_value<T> {
			using type = decltype(*declval<T>());
		};
		template<class T>
		using iterator_dereference_value_t = iterator_dereference_value<T>::type;
	}
	//[固有] : イテレータにも対応した参照外し型
	namespace Traits {
		template<class T>
		struct dereference { using type = T; };
		template<Concepts::CDereferenceable T>
		struct dereference<T> {
			using type = remove_cvref_t<conditional_t<is_pointer_v<T>, T, decltype(*declval<T>())>>;
		};
		template<class T>
		using dereference_t = dereference<T>::type;
	}
	//[固有] : イテレータコンセプト
	namespace Traits::Concepts {
		template<class I> concept CInputIterator = std::input_iterator<I>;
		template<class I, class T> concept COutputIterator = std::output_iterator<I, T>;
		template<class I> concept CForwardIterator = std::forward_iterator<I>;
		template<class I> concept CBidirectionalIterator = std::bidirectional_iterator<I>;
		template<class I> concept CRandomAccessIterator = std::random_access_iterator<I>;
		template<class I> concept CContiguousIterator = std::contiguous_iterator<I>;
	}
	//自作イテレータ内にusing iterator_concept = ...として定義する(C++20以上)
	namespace Traits {
		using input_iterator_tag = std::input_iterator_tag;
		using output_iterator_tag = std::output_iterator_tag;
		using forward_iterator_tag = std::forward_iterator_tag;
		using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
		using random_access_iterator_tag = std::random_access_iterator_tag;
		using contiguous_iterator_tag = std::contiguous_iterator_tag;
	}
	//[固有] : ラッパー型
	namespace Traits {
		/// <summary>
		/// 初期化状態を保持するT型変数のラッパークラス
		/// </summary>
		/// <typeparam name="T">任意の型</typeparam>
		template<class T>
		class InitializedVariant {
			T m_value;
			bool m_initialized = false;	//初期化済みのとき、true
		public:
			constexpr InitializedVariant() noexcept = default;
			constexpr InitializedVariant(const InitializedVariant<T>& arg) noexcept : m_value(arg.m_value), m_initialized(true) {}
			constexpr InitializedVariant(const T& value) noexcept : m_value(value), m_initialized(true) {}
			constexpr InitializedVariant(T&& value) noexcept : m_value(static_cast<T&&>(value)), m_initialized(true) {}
			constexpr ~InitializedVariant() noexcept { m_initialized = false; }
		public:
			constexpr InitializedVariant<T>& operator=(const T& rhs) noexcept {
				m_value = rhs;
				m_initialized = true;
				return *this;
			}
			constexpr InitializedVariant<T>& operator=(T&& rhs) noexcept {
				m_value = static_cast<T&&>(rhs);
				m_initialized = true;
				return *this;
			}
		public:
			constexpr T& operator*() noexcept { return m_value; }
			constexpr const T& operator*() const noexcept { return m_value; }
			constexpr T* operator->() noexcept { return &m_value; }
			constexpr const T* operator->() const noexcept { return &m_value; }
			constexpr operator T() const noexcept { return m_value; }
			explicit constexpr operator bool() const noexcept { return m_initialized; }
		};
	}
	//[固有] : Enum class 演算子自動定義型
	//template<> struct enabling_***_enum : true_type {};で特殊化することで、
	//演算子を自動定義できる
	namespace Traits {
		template<Enum T> struct enabling_bitwise_for_enum : false_type {};
		template<Enum T> struct enabling_and_or_for_enum : enabling_bitwise_for_enum<T> {};
		template<class T> concept BitwiseEnum = enabling_bitwise_for_enum<T>::value;
		template<class T> concept AndOrEnum = enabling_and_or_for_enum<T>::value;
	}
	//[固有] : ビットフラグ操作
	namespace Traits {
		template<Concepts::CBitFlag T, Concepts::CBitFlag U>
		constexpr bool IncludeBitFlags(T value, U flags) noexcept {
			const size_t u = static_cast<size_t>(flags);
			return static_cast<size_t>(value) & u == u;
		}
		template<Concepts::CBitFlag T, Concepts::CBitFlag U>
		constexpr bool ExcludeBitFlags(T value, U flags) noexcept {
			const size_t u_inv = ~static_cast<size_t>(flags);
			return static_cast<size_t>(value) | u_inv == u_inv;
		}

	}
}

//Enum class 自動定義用演算子
using namespace System::Traits;
export template<AndOrEnum T>
constexpr T operator&(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) & static_cast<underlying_type_t<T>>(rhs));
}
export template<AndOrEnum T>
constexpr T operator|(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) | static_cast<underlying_type_t<T>>(rhs));
}
export template<BitwiseEnum T>
constexpr T operator^(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) ^ static_cast<underlying_type_t<T>>(rhs));
}
export template<BitwiseEnum T>
constexpr T operator~(T x) noexcept {
	return static_cast<T>(~static_cast<underlying_type_t<T>>(x));
}
export template<AndOrEnum T>
constexpr T& operator&=(T& lhs, T rhs) noexcept {
	return lhs = lhs & rhs;
}
export template<AndOrEnum T>
constexpr T& operator|=(T& lhs, T rhs) noexcept {
	return lhs = lhs | rhs;
}
export template<BitwiseEnum T>
constexpr T& operator^=(T& lhs, T rhs) noexcept {
	return lhs = lhs ^ rhs;
}
