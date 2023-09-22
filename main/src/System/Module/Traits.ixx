export module Traits;
export import CSTDINT;
import <concepts>;	//std::same_as, std::convertible_to
import <iterator>;	//std::xxx_iterator, std::xxx_iterator_tag
import <type_traits>;	//std::is_class, std::is_union
import <iostream>;	//for operator<<() concepts

//g++のために用意されたコンセプト
export namespace System {
	namespace Traits::Concepts {
		template<class T>
		concept CHasVirtualDeleteType = requires {
			typename T::virtual_delete_type;
		};
	}
}

//他の定義に非依存な定義
export namespace System {
	//[固有] : テンプレート依存の定数(false_v, true_v, value_v)
	namespace Traits {
		template<class T, class ...Args> inline constexpr bool false_v = false;
		template<class T, class ...Args> inline constexpr bool true_v = true;
		template<class T, T value, class ...Args> inline constexpr T value_v = value;
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
	namespace Traits::PM {
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
		template<class P> struct pop_front;
		template<class T, class ...Args>
		struct pop_front<parameter_pack<T, Args...>> {
			using type = parameter_pack<Args...>;
		};
		template<class T, class P> struct push_back;
		template<class T, class ...Args>
		struct push_back<T, parameter_pack<Args...>> {
			using type = parameter_pack<Args..., T>;
		};
		template<class P> struct pop_back;
		template<class T, class ...Args>
		struct pop_back<parameter_pack<T, Args...>> {
			using type = push_front<T, typename pop_back<parameter_pack<Args...>>::type>::type;
		};
		template<class T>
		struct pop_back<parameter_pack<T>> {
			using type = parameter_pack<>;
		};
		template<class P1, class P2> struct connect;
		template<class ...Args1, class ...Args2>
		struct connect<parameter_pack<Args1...>, parameter_pack<Args2...>> {
			using type = parameter_pack<Args1..., Args2...>;
		};
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
		template<class P> struct to_function;
		template<class R, class ...Args>
		struct to_function<parameter_pack<R, Args...>> {
			using type = R(Args...);
		};
		template<class T> struct from_function;
		template<class R, class ...Args>
		struct from_function<R(Args...)> {
			using type = parameter_pack<R, Args...>;
			using without_return_type = parameter_pack<Args...>;
		};
		template<class R, class ...Args>
		struct from_function<R(Args...) noexcept> {
			using type = parameter_pack<R, Args...>;
			using without_return_type = parameter_pack<Args...>;
		};
	}
	namespace Traits {
		template<size_t i, class P>
		using at_from_param = PM::at<i, P>::type;
		template<class T, class ...Args>
		using push_front_param = PM::push_front<T, parameter_pack<Args...>>::type;
		template<class T, class ...Args>
		using pop_front_param = PM::pop_front<parameter_pack<T, Args...>>::type;
		template<class T, class ...Args>
		using push_back_param = PM::push_back<T, parameter_pack<Args...>>::type;
		template<class T, class ...Args>
		using pop_back_param = PM::pop_back<parameter_pack<T, Args...>>::type;
		template<class P1, class P2>
		using connect_param = PM::connect<P1, P2>::type;
		template<class Sentinel, class P>
		using split_param = PM::split<Sentinel, P>::type;
		template<class P1>
		using function_t_from_param = PM::to_function<P1>::type;
		template<class T>
		using from_function_param = PM::from_function<T>::type;
		template<class T>
		using from_function_without_return_param = PM::from_function<T>::without_return_type;
	}
	//[固有] : 型選択テンプレート(one_of_t)
	namespace Traits {
		//Args...からi番目の型を取得
		template<size_t i, class ...Args>
		struct one_of { using type = void; };
		template<size_t i, class Head, class ...Tail>
		struct one_of<i, Head, Tail...> : one_of<i - 1, Tail...> {};
		template<class Head, class ...Args>
		struct one_of<0, Head, Args...> { using type = Head; };
	}
	namespace Traits {
		//パラメータリストArgsのi番目の型(ゼロインデックス)
		template<size_t i, typename ...Args> using one_of_t = one_of<i, Args...>::type;
	}
	//[固有] : テンプレート依存の定数選択(one_of_value_v)
	namespace Traits {
		template<class T, size_t i, T ...args> struct one_of_value;
		template<class T, T head, T ...args>
		struct one_of_value<T, 0, head, args...> { static constexpr T value = head; };
		template<class T, size_t i, T head, T ...args>
		struct one_of_value<T, i, head, args...> : public one_of_value<T, i - 1, args...> {};
		template<class T, size_t i>
		struct one_of_value<T, i> {
			static constexpr T value = T();
			static_assert(false_v<T>, "Specified invalid index to one_of_value template parameter.");
		};
	}
	namespace Traits {
		template<class T, size_t i, T ...args>
		inline constexpr T one_of_value_v = one_of_value<T, i, args...>::value;
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
	//[固有]：テンプレート整数型(int_t, int_next)
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
	//[固有]：型サイズ比較(greater_t, less_t)
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
	//[固有]：2つの型を持つ共用体テンプレート(UnionSet)
	namespace Traits {
		//T型メンバfirstとU型メンバsecondを持つ共用体
		template<typename T, typename U>
		union UnionSet {
			T first;
			U second;
		};
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
	//[固有]：ラッパー型(InitializedVariant)
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
	//[iterator]互換：イテレータコンセプト
	namespace Traits::Concepts {
		template<class I> concept CInputIterator = std::input_iterator<I>;
		template<class I, class T> concept COutputIterator = std::output_iterator<I, T>;
		template<class I> concept CForwardIterator = std::forward_iterator<I>;
		template<class I> concept CBidirectionalIterator = std::bidirectional_iterator<I>;
		template<class I> concept CRandomAccessIterator = std::random_access_iterator<I>;
		template<class I> concept CContiguousIterator = std::contiguous_iterator<I>;
		template<class I> concept input_iterator = std::input_iterator<I>;
		template<class I, class T> concept output_iterator = std::output_iterator<I, T>;
		template<class I> concept forward_iterator = std::forward_iterator<I>;
		template<class I> concept bidirectional_iterator = std::bidirectional_iterator<I>;
		template<class I> concept random_access_iterator = std::random_access_iterator<I>;
		template<class I> concept contiguous_iterator = std::contiguous_iterator<I>;
	}
	//[iterator]互換：イテレータタグ(xxx_iterator_tag)
	//自作イテレータ内にusing iterator_concept = ...として定義する(C++20以上)
	namespace Traits {
		using input_iterator_tag = std::input_iterator_tag;
		using output_iterator_tag = std::output_iterator_tag;
		using forward_iterator_tag = std::forward_iterator_tag;
		using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
		using random_access_iterator_tag = std::random_access_iterator_tag;
		using contiguous_iterator_tag = std::contiguous_iterator_tag;
	}
	//[concepts]互換 : 複合要件で使用できるコンセプト({ 式 } -> concept;)
	namespace Traits::Concepts {
		template<class T1, class T2> concept same_as = std::same_as<T1, T2>;
		template<class From, class To> concept convertible_to = std::convertible_to<From, To>;
	}
	//[固有] : 型情報取得(extent_level_v, element_count_v)
	namespace Traits {
		template<class T> struct extent_level { static constexpr size_t value = 0; };
		template<class T, size_t N> struct extent_level<T[N]> { static constexpr size_t value = N; };

		template<class T, size_t ...args>
		struct element_count {
			static constexpr size_t value = (args * ...);
			using type = T;
		};
		template<class T>
		struct element_count<T> {
			static constexpr size_t value = 1;
			using type = T;
		};
		template<class T>
		struct element_count<T[]> : public element_count<T, 1> {};
		template<class T, size_t N>
		struct element_count<T[N]> : public element_count<T, N> {};
		template<class T, size_t N, size_t ...args>
		struct element_count<T[N], args...> : public element_count<T, args..., N> {};
	}
	namespace Traits {
		template<class T> inline constexpr size_t extent_level_v = extent_level<T>::value;
		template<class T> inline constexpr size_t element_count_v = element_count<T>::value;
	}	
	//[固有] : テンプレート型情報取得(template_info)
	namespace Traits {
		template<class T>
		struct template_info {
			static constexpr bool value = false;
			static constexpr size_t args_count = 0;
			using type = T;
			template<size_t i>
			using arg_type = void;
		};
		template<template<class ...Args> class T, class ...Args>
		struct template_info<T<Args...>> {
			static constexpr bool value = true;
			static constexpr size_t args_count = sizeof...(Args);
			using type = T<Args...>;
			template<size_t i>
			using arg_type = one_of_t<i, Args...>;
		};
	}
	namespace Traits {
		template<class T>
		inline constexpr size_t template_args_count_v = template_info<T>::args_count;
	}
	//[固有]：浮動小数点型ビット情報(EXP_DIGIT, FRAC_DIGIT, EXP_BIAS)
	namespace Traits {
		template<class T>
		struct float_info {
			static constexpr int EXP_DIGIT = T::exponent;
			static constexpr int FRAC_DIGIT = T::fraction;
			static constexpr int EXP_BIAS = (1ull << (EXP_DIGIT -1)) - 1;
		};
		template<>
		struct float_info<float> {
			static constexpr int EXP_DIGIT = 8;
			static constexpr int FRAC_DIGIT = 23;
			static constexpr int EXP_BIAS = (1ull << (EXP_DIGIT -1)) - 1;
		};
		template<>
		struct float_info<double> {
			static constexpr int EXP_DIGIT = 11;
			static constexpr int FRAC_DIGIT = 52;
			static constexpr int EXP_BIAS = (1ull << (EXP_DIGIT -1)) - 1;
		};
		template<>
		struct float_info<long double> {
			static constexpr int EXP_DIGIT = 11;
			static constexpr int FRAC_DIGIT = 52;
			static constexpr int EXP_BIAS = (1ull << (EXP_DIGIT -1)) - 1;
		};
		template<class T>
		struct float_info<T const> : float_info<T> {};
		template<class T>
		struct float_info<T volatile> : float_info<T> {};
		template<class T>
		struct float_info<T const volatile> : float_info<T> {};
		
		//IEEE-754に準拠した浮動小数点数型の指数部ビット数
		template<typename T> inline constexpr int EXP_DIGIT = float_info<T>::EXP_DIGIT;
		//IEEE-754に準拠した浮動小数点数型の仮数部ビット数
		template<typename T> inline constexpr int FRAC_DIGIT = float_info<T>::FRAC_DIGIT;
		//IEEE-754に準拠した浮動小数点数型の指数部にかかるバイアス値の絶対値
		template<typename T> inline constexpr int EXP_BIAS = float_info<T>::EXP_BIAS;
	}
}

//上のブロックもしくはこのブロック内の定義に依存する定義
export namespace System {
	//[type_traits]互換：型変換
	namespace Traits {
		template<typename T> struct remove_const { using type = T; };
		template<typename T> struct remove_const<T const> { using type = T; };
		template<typename T> struct remove_volatile { using type = T; };
		template<typename T> struct remove_volatile<T volatile> { using type = T; };
		template<typename T> struct remove_cv { using type = remove_const<typename remove_volatile<T>::type>::type; };
		template<typename T> struct add_const { using type = T const; };
		template<typename T> struct add_volatile { using type = T volatile; };
		template<typename T> struct add_cv { using type = add_const<typename add_volatile<T>::type>::type; };
		template<typename T> struct remove_reference { using type = T; };
		template<typename T> struct remove_reference<T&> { using type = T; };
		template<typename T> struct remove_reference<T&&> { using type = T; };
		template<typename T> struct add_lvalue_reference { using type = typename remove_reference<T>::type&; };
		template<> struct add_lvalue_reference<void> { using type = void; };
		template<typename T> struct add_rvalue_reference { using type = typename remove_reference<T>::type&&; };
		template<> struct add_rvalue_reference<void> { using type = void; };
		template<typename T> struct add_pointer { using type = typename remove_reference<T>::type*; };
		template<typename T> struct add_pointer<T&> { using type = T; };
		template<typename T> struct add_pointer<T&&> { using type = T; };
		template<typename T> struct remove_pointer { using type = T; };
		template<typename T> struct remove_pointer<T*> { using type = T; };
		template<typename T> struct remove_pointer<T* const> { using type = T; };
		template<typename T> struct remove_pointer<T* volatile> { using type = T; };
		template<typename T> struct remove_pointer<T* const volatile> { using type = T; };
		template<typename T> struct remove_extent { using type = T; };
		template<typename T> struct remove_extent<T[]> { using type = T; };
		template<typename T, size_t N> struct remove_extent<T[N]> { using type = T; };
		template<typename T> struct remove_all_extents { using type = T; };
		template<typename T> struct remove_all_extents<T[]> { using type = typename remove_all_extents<T>::type; };
		template<typename T, size_t N> struct remove_all_extents<T[N]> { using type = typename remove_all_extents<T>::type; };
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
		template<class T> using remove_extent_t = typename remove_extent<T>::type;
		template<class T> using remove_all_extents_t = typename remove_all_extents<T>::type;
	}
	//完全転送(move)
	namespace Traits {
		template<class T>
		constexpr T&& forward(remove_reference_t<T>& arg) noexcept { return static_cast<T&&>(arg); }
		template<class T>
		constexpr T&& forward(remove_reference_t<T>&& arg) noexcept { return static_cast<T&&>(arg); }
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
		template<class T> struct is_const<T const> : true_type {};
		template<class T> struct is_volatile : false_type {};
		template<class T> struct is_volatile<T volatile> : true_type {};
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
		template<class T> struct is_member_function_pointer : false_type {};
		template<class R, class T, class ...Args>
		struct is_member_function_pointer<R(T::*)(Args...)> : true_type {};
		template<class R, class T, class ...Args>
		struct is_member_function_pointer<R(T::*)(Args...) const> : true_type {};
		template<class R, class T, class ...Args>
		struct is_member_function_pointer<R(T::*)(Args...) noexcept> : true_type {};
		template<class R, class T, class ...Args>
		struct is_member_function_pointer<R(T::*)(Args...) const noexcept> : true_type {};
		template<typename T> struct is_reference : bool_constant<is_lvalue_reference<T>::value || is_rvalue_reference<T>::value> {};
		template<typename T> struct is_member_pointer : bool_constant<is_member_object_pointer<T>::value || is_member_function_pointer<T>::value> {};
		template<typename T> struct is_function : false_type {};
		template<typename T, typename ...Args> struct is_function<T(Args...)> : true_type {};
		template<typename T, typename ...Args> struct is_function<T(Args...) noexcept> : true_type {};
		template<typename T> struct is_enum : bool_constant<!(is_void<T>::value || is_integral<T>::value || is_floating_point<T>::value || is_array<T>::value || is_pointer<T>::value || is_reference<T>::value || is_member_pointer<T>::value || is_class_or_union<T>::value || is_function<T>::value)> {};
		template<typename T> struct is_arithmetic : bool_constant<is_integral<T>::value || is_floating_point<T>::value> {};
		template<typename T> struct is_fundamental : bool_constant<is_arithmetic<T>::value || is_void<T>::value || is_null_pointer<T>::value> {};
		template<typename T> struct is_scaler : bool_constant<is_arithmetic<T>::value || is_enum<T>::value || is_pointer<T>::value || is_member_pointer<T>::value || is_null_pointer<T>::value> {};
		template<typename T> struct is_object : bool_constant<is_scaler<T>::value || is_array<T>::value || is_class_or_union<T>::value> {};
		template<typename T> struct is_compound : bool_constant<!is_fundamental<T>::value> {};
		template<class From, class To> struct is_convertible : false_type {};
		template<class From, class To>
		requires requires(To x) { x = declval<From>(); }
		struct is_convertible<From, To> : true_type {};
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
		requires requires() { is_convertible<decltype(declval<remove_cv_t<F>>()(declval<Args>()...)), R>::value; }
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
		template<class L, class R>
		struct is_assignable : false_type {};
		template<class L, class R>
		requires requires(L lhs, R rhs) { lhs = rhs; }
		struct is_assignable<L, R> : true_type {};
		template<class L, class R>
		struct is_copy_assignable : is_assignable<L&, R const&> {};
		template<class L, class R>
		struct is_move_assignable : is_assignable<L&, R&&> {};
		namespace Internal {
			template<class T> struct is_empty_1 : public T { double value; };
			struct is_empty_2 { double value; };
			template<class T> struct is_empty_internal : public integral_constant<bool, sizeof(is_empty_1<T>) == sizeof(is_empty_2)> {};
		}
		template<class T> struct is_empty : public conditional_t<std::is_class<T>::value, Internal::is_empty_internal<T>, false_type> {};
	}
	namespace Traits {
		template<class C> using is_class = std::is_class<C>;
		template<class U> using is_union = std::is_union<U>;
		template<class T> inline constexpr bool is_class_or_union_v = is_class_or_union<T>::value;
	}
	namespace Traits {
		template<typename T> inline constexpr bool is_const_v = is_const<T>::value;
		template<typename T> inline constexpr bool is_volatile_v = is_volatile<T>::value;
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
		template<typename From, typename To> inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
		template<typename T> inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;
		template<typename T> inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
		template<typename T> inline constexpr bool is_signed_v = is_signed<T>::value;
		template<class Base, class Derived> inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
		template<class F, class ...Args> inline constexpr bool is_invocable_v = is_invocable<F, Args...>::value;
		template<class R, class F, class ...Args> inline constexpr bool is_invocable_r_v = is_invocable_r<R, F, Args...>::value;
		template<class T> inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
		template<class T> inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
		template<class T> inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
		template<class L, class R = L> inline constexpr bool is_assignable_v = is_assignable<L, R>::value;
		template<class L, class R = L> inline constexpr bool is_copy_assignable_v = is_copy_assignable<L, R>::value;
		template<class L, class R = L> inline constexpr bool is_move_assignable_v = is_move_assignable<L, R>::value;
		template<class T> inline constexpr bool is_empty_v = is_empty<T>::value;
	}
	//[固有] : パラメータパック判定
	namespace Traits {
		template<class P1, class P2>
		consteval bool same_pack() noexcept {
			if constexpr (P1::count != P2::count) return false;
			else if constexpr (is_same_v<typename P1::head_t, typename P2::head_t>) {
				if constexpr (P1::count == 0 && P2::count == 0) return true;
				else return same_pack<typename P1::next_t, typename P2::next_t>();
			}
			else return false;
		}
		template<class From, class To>
		consteval bool convertible_pack() noexcept {
			if constexpr (From::count != To::count) return false;
			else if constexpr (is_convertible_v<typename From::head_t, typename To::head_t>) {
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
			else if constexpr (is_convertible_v<typename From::head_t, typename To::head_t>) {
				if constexpr (From::count == 0 && To::count == 0) return true;
				else return implicit_convertible_pack<typename From::next_t, typename To::next_t>();
			}
			else return false;
		}

		template<class P1, class P2>
		inline constexpr bool is_same_pack_v = same_pack<P1, P2>();
		template<class FromPack, class ToPack>
		inline constexpr bool is_convertible_pack_v = convertible_pack<FromPack, ToPack>();
		template<class FromPack, class ToPack>
		inline constexpr bool is_implicit_convertible_pack_v = implicit_convertible_pack<FromPack, ToPack>();
	}
	//[固有] : ポインタ型情報取得(pointer_level_v, remove_all_pointer_t)
	namespace Traits {
		template<class T, size_t V>
		struct pointer_levels {
			static constexpr size_t value = V;
			using base_type = T;
		};
		template<class T, size_t V>
		struct pointer_levels<T*, V> : pointer_levels<remove_cv_t<T>, V + 1> {};
		template<class T, size_t V>
		struct pointer_levels<T&, V> : pointer_levels<remove_cv_t<T>, V + 1> {};
	}
	namespace Traits {
		template<class T>
		inline constexpr size_t pointer_levels_v = pointer_levels<T, 0>::value;
		template<class T>
		using remove_all_pointer_t = pointer_levels<T, 0>::base_type;
	}
	//[固有] : 定数比較
	namespace Traits {
		template<class T, T head, T ...args> struct is_same_value;
		template<class T, T lhs, T rhs, T ...args>
		struct is_same_value<T, lhs, rhs, args...> : conditional_t<lhs == rhs, is_same_value<T, lhs, args...>, false_type> {};
		template<class T, T lhs>
		struct is_same_value<T, lhs> : true_type {};
	}
	namespace Traits {
		template<class T, T lhs, T rhs, T ...args>
		inline constexpr bool is_same_value_v = is_same_value<T, lhs, rhs, args...>::value;
	}
	//[固有]：型選択テンプレート(conditional_select_t, select_tag)
	namespace Traits {
		//Cがtrueのとき、有効
		template<bool C, class T>
		struct select_tag {
			static constexpr bool value = C;
			using type = T;
		};
		namespace Concepts {
			//select_tag型をとるコンセプト
			template<class T>
			concept CSelectTag = requires(){
				{ T::value } -> Concepts::convertible_to<bool>;
				typename T::type;
			};
		}
		//Argsを先頭から探索し、初めて見つかった有効なselect_tagが持つ型typeを取得
		template<Concepts::CSelectTag ...Args> struct conditional_select;
		template<Concepts::CSelectTag Head, Concepts::CSelectTag ...Args>
		struct conditional_select<Head, Args...> : conditional_t<Head::value, Head, conditional_select<Args...>> {};
		template<>
		struct conditional_select<> {
			using type = void;
			conditional_select() noexcept {
				static_assert(is_void_v<type>, "conditional_selectには、有効なselect_tagが一つ以上必要です。");
			}
		};
	}
	namespace Traits {
		//パラメータリストArgsの初めて見つかった有効な型(なければstatic_assert)
		template<Concepts::CSelectTag ...Args> using conditional_select_t = conditional_select<Args...>::type;
	}
	//[固有] : 型変換
	namespace Traits {
		template<typename T> struct toggle_const { using type = add_const_t<T>; };
		template<typename T> struct toggle_const<T const> { using type = remove_const_t<T>; };
		template<class T> struct dereference_return { using type = T&; };
		template<> struct dereference_return<void> { using type = void; };
		template<> struct dereference_return<const void> { using type = void; };
		template<> struct dereference_return<volatile void> { using type = void; };
		template<> struct dereference_return<const volatile void> { using type = void; };
		template<class T> struct dereference_return<T[]> { using type = void; };
		template<class T, size_t N> struct dereference_return<T[N]> { using type = void; };
		template<class T> struct member_access_return { using type = T*; };
		template<class T> struct member_access_return<T[]> { using type = void; };
		template<class T, size_t N> struct member_access_return<T[N]> { using type = void; };
		template<class T> struct array_access_return { using type = void; };
		template<class T> struct array_access_return<T[]> { using type = T&; };
		template<class T, size_t N> struct array_access_return<T[N]> { using type = T&; };
		template<class T>
		struct remove_one_specifier {
			using type = conditional_select_t<
				select_tag<is_const_v<T> || is_volatile_v<T>, remove_cv_t<T>>,
				select_tag<is_reference_v<T>, remove_reference_t<T>>,
				select_tag<is_pointer_v<T>, remove_pointer_t<T>>,
				select_tag<is_array_v<T>, remove_extent_t<T>>,
				select_tag<true, T>
			>;
		};
		template<class T> struct remove_all_specifiers { using type = T; };
		template<class T> requires(!is_same_v<T, typename remove_one_specifier<T>::type>)
		struct remove_all_specifiers<T> : public remove_all_specifiers<typename remove_one_specifier<T>::type> {};
		template<class T>
		struct remove_all_specifiers_without_extents : remove_all_specifiers_without_extents<Traits::remove_pointer_t<Traits::remove_cvref_t<T>>> {};
		template<class T> requires(Traits::is_same_v<T, Traits::remove_pointer_t<Traits::remove_cvref_t<T>>>)
		struct remove_all_specifiers_without_extents<T> {
			using type = T;
		};
		template<class T, size_t levels, size_t ...args>
		struct add_extents : add_extents<T[one_of_value_v<size_t, levels - 1, args...>], levels - 1, args...> {};
		template<class T, size_t ...args> struct add_extents<T, 0, args...> { using type = T; };
		template<class T, size_t levels, size_t ...args>
		requires (!is_same_value_v<size_t, 0, levels> && is_same_value_v<size_t, one_of_value_v<size_t, levels - 1, args...>, 0>)
		struct add_extents<T, levels, args...> : add_extents<T[], levels - 1, args...> {};
	}
	namespace Traits {
		template<typename T> using toggle_const_t = toggle_const<T>::type;
		template<class T> using dereference_return_t = dereference_return<T>::type;
		template<class T> using member_access_return_t = member_access_return<T>::type;
		template<class T> using array_access_return_t = array_access_return<T>::type;
		template<class T> using remove_one_specifier_t = remove_one_specifier<T>::type;
		template<class T> using remove_all_specifiers_t = remove_all_specifiers<T>::type;
		template<class T> using remove_all_specifiers_without_extents_t = remove_all_specifiers_without_extents<T>::type;
		template<class T, size_t ...args> using add_extents_t = add_extents<T, sizeof...(args), args...>::type;
	}
	//[固有] : 型判定
	namespace Traits {
		template<class T> struct is_unknown_bound_array : false_type {};
		template<class T> struct is_unknown_bound_array<T[]> : true_type {};
		template<class T, size_t N> struct is_unknown_bound_array<T[N]> : false_type {};
		template<class T> struct is_known_bound_array : false_type {};
		template<class T> struct is_known_bound_array<T[]> : false_type {};
		template<class T, size_t N> struct is_known_bound_array<T[N]> : true_type {};
		template<class T, class U>
		struct is_same_template : false_type {};
		template<template<class ...Args1> class T, class ...Params1, template<class ...Args2> class U, class ...Params2>
		requires requires {
			requires is_same_v<T<Params1...>, U<Params1...>>;
			requires is_same_v<T<Params2...>, U<Params2...>>;
		}
		struct is_same_template<T<Params1...>, U<Params2...>> : true_type {};
	}
	namespace Traits {
		template<class T> inline constexpr bool is_unknown_bound_array_v = is_unknown_bound_array<T>::value;
		template<class T> inline constexpr bool is_known_bound_array_v = is_known_bound_array<T>::value;
		template<class T, class U> inline constexpr bool is_same_template_v = is_same_template<T, U>::value;
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
	//[固有] : コンセプト
	namespace Traits::Concepts {
		template<class T> concept CVoid = is_void_v<T>;
		template<class T> concept CUnsigned = is_unsigned_v<T>;
		template<class T> concept CSigned = is_signed_v<T>;
		template<class T> concept CIntegral = is_integral_v<T>;
		template<class T> concept CFloating = is_floating_point_v<T>;
		template<class T> concept CArithmetic = CIntegral<T> || CFloating<T>;
		template<class T> concept CCharType = is_any_of_v<T, char, char8_t, char16_t, char32_t, wchar_t>;
		template<class T> concept CNumber = CArithmetic<T> && !CCharType<T>;// is_any_of_v<T, int, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, float, double>;
		template<class T> concept CPointer = is_pointer_v<T>;
		template<class T> concept CMemberFunctionPointer = is_member_function_pointer_v<T>;
		template<class T> concept CEnum = is_enum_v<T>;
		template<class T> concept CClassOrUnion = is_class_or_union_v<T>;
		template<class T> concept CLValueReference = is_lvalue_reference_v<T>;
		template<class T> concept CRValueReference = is_rvalue_reference_v<T>;
		template<class T> concept CReference = is_reference_v<T>;
		template<class T> concept CConstLValueReference = is_lvalue_reference_v<T> && is_const_v<remove_reference_t<T>>;
		template<class T> concept CUnknownBoundArray = is_unknown_bound_array_v<T>;
		template<class T> concept CKnownBoundArray = is_known_bound_array_v<T>;
		template<class T> concept CArray = CUnknownBoundArray<T> || CKnownBoundArray<T>;
		template<class T> concept CNotVoid = !CVoid<T>;
		template<class T> concept CNotArray = !CArray<T>;
		template<class T, class U, class ...Args> concept CSame = is_same_v<T, U, Args...>;
		template<class T, class U> concept CSameRemoveCV = is_same_v<remove_cv_t<T>, remove_cv_t<U>>;
		template<class T, class U> concept CSameRemoveCVRef = is_same_v<remove_cvref_t<T>, remove_cvref_t<U>>;
		template<class T, class U, class ...Args> concept CNotSame = !CSame<T, U, Args...>;
		template<class T, class U, class ...Args> concept CAny = is_any_of_v<T, U, Args...>;
		template<class T, class U, class ...Args> concept CNotAny = !CAny<T, U, Args...>;
		template<class T, class U> concept CNotSameArg = CNotAny<T, U, U&, U&&, U const, U const&, U const&&>;
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
		template<class Src, class Dst>
		concept CConstructibleTo = CCopyConstructibleTo<Src, Dst> || CMoveConstructibleTo<Src, Dst>;
		template<class Dst, class Src>
		concept CConstructibleFrom = CCopyConstructibleFrom<Dst, Src> || CMoveConstructibleTo<Dst, Src>;
		template<class T> concept CAssignable = is_assignable_v<T>;
		template<class T> concept CCopyAssignable = is_copy_assignable_v<T>;
		template<class T> concept CMoveAssignable = is_move_assignable_v<T>;
		template<class L, class R> concept CAssignableFrom = is_assignable_v<L, R>;
		template<class L, class R> concept CCopyAssignableFrom = is_copy_assignable_v<L, R>;
		template<class L, class R> concept CMoveAssignableFrom = is_move_assignable_v<L, R>;
		template<class R, class L> concept CAssignableTo = is_assignable_v<L, R>;
		template<class R, class L> concept CCopyAssignableTo = is_copy_assignable_v<L, R>;
		template<class R, class L> concept CMoveAssignableTo = is_move_assignable_v<L, R>;
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
		template<class T>
		concept CHasMember = requires(int T::*ptr) {
			ptr = nullptr;
		};
		template<class T>
		concept CHasFunctionCallOperator = requires() {
			&remove_cvref_t<T>::operator();
		};
		template<class T>
		concept CIsCppFunction = is_function_v<T>;
		template<class T>
		concept CInvokable = CIsCppFunction<T> || CHasFunctionCallOperator<T>;
		template<class F, class T>
		concept CConditionFunction = requires(F cond, const T & obj) {
			{cond(obj)} -> same_as<bool>;
		};
		template<class F, class T>
		concept CCompareFunction = requires(F comp, const T & obj) {
			{comp(obj, obj)} -> same_as<decltype(1 <=> 1)>;
		};
		//関数型From, Toの持つ引数パラメータパックにおいて、それぞれFromからToに変換できることを表すコンセプト
		template<class From, class To>
		concept CConvertibleFunction = convertible_pack<from_function_without_return_param<To>, from_function_without_return_param<From>>();
	}
	//[固有] : メンバポインタ型
	namespace Traits {
		template<class M, class T> struct member_ptr { using type = M T::*; };
		template<class M, class T> requires (!Concepts::CHasMember<T>)
		struct member_ptr<M, T> { using type = void*; };
		template<class M, class T> using member_ptr_t = member_ptr<M, T>::type;
	}
	//[固有] : 関数型(function_t)
	namespace Traits {
		template<class T> struct function_info;
		template<class R, class... Args>
		struct function_info<R(*)(Args...)> {
			using ptr_type = R(*)(Args...);
			using without_noexcept_ptr_type = ptr_type;
			using without_object_ptr_type = R(*)(Args...);
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = void;
			using no_const_object_type = void;
			using const_object_type = void const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = arg_type<i>;
			template<size_t i> using global_reference_arg_type = arg_type<i>;
			using global_pointer_ptr_type = ptr_type;
			using global_reference_ptr_type = ptr_type;
			using global_pointer_without_noexcept_ptr_type = ptr_type;
			using global_reference_without_noexcept_ptr_type = ptr_type;
			using global_const_pointer_ptr_type = ptr_type;
			using global_const_reference_ptr_type = ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = ptr_type;
			using global_const_reference_without_noexcept_ptr_type = ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = false;
			static constexpr bool is_member_v = false;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class T, class... Args>
		struct function_info<R(T::*)(Args...)> {
			using ptr_type = R(T::*)(Args...);
			using without_noexcept_ptr_type = ptr_type;
			using without_object_ptr_type = R(*)(Args...);
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = T;
			using no_const_object_type = T;
			using const_object_type = T const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = one_of_t<i, T*, Args...>;
			template<size_t i> using global_reference_arg_type = one_of_t<i, T&, Args...>;
			using global_pointer_ptr_type = R(*)(T*, Args...);
			using global_reference_ptr_type = R(*)(T&, Args...);
			using global_pointer_without_noexcept_ptr_type = global_pointer_ptr_type;
			using global_reference_without_noexcept_ptr_type = global_reference_ptr_type;
			using global_const_pointer_ptr_type = R(*)(T const*, Args...);
			using global_const_reference_ptr_type = R(*)(T const&, Args...);
			using global_const_pointer_without_noexcept_ptr_type = global_const_pointer_ptr_type;
			using global_const_reference_without_noexcept_ptr_type = global_const_reference_ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = false;
			static constexpr bool is_member_v = true;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class T, class... Args>
		struct function_info<R(T::*)(Args...) const> {
			using ptr_type = R(T::*)(Args...) const;
			using without_noexcept_ptr_type = ptr_type;
			using without_object_ptr_type = R(*)(Args...);
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = T const;
			using no_const_object_type = T;
			using const_object_type = T const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = one_of_t<i, T const*, Args...>;
			template<size_t i> using global_reference_arg_type = one_of_t<i, T const&, Args...>;
			using global_pointer_ptr_type = R(*)(T const*, Args...);
			using global_reference_ptr_type = R(*)(T const&, Args...);
			using global_pointer_without_noexcept_ptr_type = global_pointer_ptr_type;
			using global_reference_without_noexcept_ptr_type = global_reference_ptr_type;
			using global_const_pointer_ptr_type = global_pointer_ptr_type;
			using global_const_reference_ptr_type = global_reference_ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = global_pointer_ptr_type;
			using global_const_reference_without_noexcept_ptr_type = global_reference_ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = false;
			static constexpr bool is_member_v = true;
			static constexpr bool is_const_member_v = true;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class... Args>
		struct function_info<R(*)(Args...) noexcept> {
			using ptr_type = R(*)(Args...) noexcept;
			using without_noexcept_ptr_type = R(*)(Args...);
			using without_object_ptr_type = R(*)(Args...) noexcept;
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = void;
			using no_const_object_type = void;
			using const_object_type = void const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = arg_type<i>;
			template<size_t i> using global_reference_arg_type = arg_type<i>;
			using global_pointer_ptr_type = ptr_type;
			using global_reference_ptr_type = ptr_type;
			using global_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_pointer_ptr_type = ptr_type;
			using global_const_reference_ptr_type = ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = true;
			static constexpr bool is_member_v = false;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class T, class... Args>
		struct function_info<R(T::*)(Args...) noexcept> {
			using ptr_type = R(T::*)(Args...) noexcept;
			using without_noexcept_ptr_type = R(T::*)(Args...);
			using without_object_ptr_type = R(*)(Args...) noexcept;
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = T;
			using no_const_object_type = T;
			using const_object_type = T const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = one_of_t<i, T*, Args...>;
			template<size_t i> using global_reference_arg_type = one_of_t<i, T&, Args...>;
			using global_pointer_ptr_type = R(*)(T*, Args...) noexcept;
			using global_reference_ptr_type = R(*)(T&, Args...) noexcept;
			using global_pointer_without_noexcept_ptr_type = R(*)(T*, Args...);
			using global_reference_without_noexcept_ptr_type = R(*)(T&, Args...);
			using global_const_pointer_ptr_type = R(*)(T const*, Args...) noexcept;
			using global_const_reference_ptr_type = R(*)(T const&, Args...) noexcept;
			using global_const_pointer_without_noexcept_ptr_type = R(*)(T const*, Args...);
			using global_const_reference_without_noexcept_ptr_type = R(*)(T const&, Args...);
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = true;
			static constexpr bool is_member_v = true;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class T, class... Args>
		struct function_info<R(T::*)(Args...) const noexcept> {
			using ptr_type = R(T::*)(Args...) const noexcept;
			using without_noexcept_ptr_type = R(T::*)(Args...) const;
			using without_object_ptr_type = R(*)(Args...) noexcept;
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = T const;
			using no_const_object_type = T;
			using const_object_type = T const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = one_of_t<i, T const*, Args...>;
			template<size_t i> using global_reference_arg_type = one_of_t<i, T const&, Args...>;
			using global_pointer_ptr_type = R(*)(T const*, Args...) noexcept;
			using global_reference_ptr_type = R(*)(T const&, Args...) noexcept;
			using global_pointer_without_noexcept_ptr_type = R(*)(T const*, Args...);
			using global_reference_without_noexcept_ptr_type = R(*)(T const&, Args...);
			using global_const_pointer_ptr_type = R(*)(T const*, Args...) noexcept;
			using global_const_reference_ptr_type = R(*)(T const&, Args...) noexcept;
			using global_const_pointer_without_noexcept_ptr_type = R(*)(T const*, Args...);
			using global_const_reference_without_noexcept_ptr_type = R(*)(T const&, Args...);
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = true;
			static constexpr bool is_noexcept_v = true;
			static constexpr bool is_member_v = true;
			static constexpr bool is_const_member_v = true;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class... Args>
		struct function_info<R(Args...)> {
			using ptr_type = R(*)(Args...);
			using without_noexcept_ptr_type = ptr_type;
			using without_object_ptr_type = R(*)(Args...);
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = void;
			using no_const_object_type = void;
			using const_object_type = void const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = arg_type<i>;
			template<size_t i> using global_reference_arg_type = arg_type<i>;
			using global_pointer_ptr_type = ptr_type;
			using global_reference_ptr_type = ptr_type;
			using global_pointer_without_noexcept_ptr_type = ptr_type;
			using global_reference_without_noexcept_ptr_type = ptr_type;
			using global_const_pointer_ptr_type = ptr_type;
			using global_const_reference_ptr_type = ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = ptr_type;
			using global_const_reference_without_noexcept_ptr_type = ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = true;
			static constexpr bool is_pointer_v = false;
			static constexpr bool is_noexcept_v = false;
			static constexpr bool is_member_v = false;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<class R, class... Args>
		struct function_info<R(Args...) noexcept> {
			using ptr_type = R(*)(Args...) noexcept;
			using without_noexcept_ptr_type = R(*)(Args...);
			using without_object_ptr_type = R(*)(Args...) noexcept;
			using without_object_and_noexcept_ptr_type = R(*)(Args...);
			using return_type = R;
			using object_type = void;
			using no_const_object_type = void;
			using const_object_type = void const;
			template<size_t i> using arg_type = one_of_t<i, Args...>;
			template<size_t i> using global_pointer_arg_type = arg_type<i>;
			template<size_t i> using global_reference_arg_type = arg_type<i>;
			using global_pointer_ptr_type = ptr_type;
			using global_reference_ptr_type = ptr_type;
			using global_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_pointer_ptr_type = ptr_type;
			using global_const_reference_ptr_type = ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = false;
			static constexpr bool is_type_v = true;
			static constexpr bool is_pointer_v = false;
			static constexpr bool is_noexcept_v = true;
			static constexpr bool is_member_v = false;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = sizeof...(Args);
		};
		template<Concepts::CHasFunctionCallOperator F>
		struct function_info<F> {
		private:
			using call_info = function_info<decltype(&Traits::remove_cvref_t<F>::operator())>;
		public:
			using ptr_type = call_info::without_object_ptr_type;
			using without_noexcept_ptr_type = call_info::without_object_and_noexcept_ptr_type;
			using without_object_ptr_type = ptr_type;
			using without_object_and_noexcept_ptr_type = without_noexcept_ptr_type;
			using return_type = call_info::return_type;
			using object_type = void;
			using no_const_object_type = void;
			using const_object_type = void const;
			template<size_t i> using arg_type = typename call_info::template arg_type<i>;
			template<size_t i> using global_pointer_arg_type = arg_type<i>;
			template<size_t i> using global_reference_arg_type = arg_type<i>;
			using global_pointer_ptr_type = ptr_type;
			using global_reference_ptr_type = ptr_type;
			using global_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_pointer_ptr_type = ptr_type;
			using global_const_reference_ptr_type = ptr_type;
			using global_const_pointer_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using global_const_reference_without_noexcept_ptr_type = without_noexcept_ptr_type;
			using invoke_type = remove_pointer_t<ptr_type>;
			using without_noexcept_invoke_type = remove_pointer_t<without_noexcept_ptr_type>;
			using without_object_invoke_type = remove_pointer_t<without_object_ptr_type>;
			using without_object_and_noexcept_invoke_type = remove_pointer_t<without_object_and_noexcept_ptr_type>;
			using global_pointer_invoke_type = remove_pointer_t<global_pointer_ptr_type>;
			using global_reference_invoke_type = remove_pointer_t<global_reference_ptr_type>;
			using global_pointer_without_noexcept_invoke_type = remove_pointer_t<global_pointer_without_noexcept_ptr_type>;
			using global_reference_without_noexcept_invoke_type = remove_pointer_t<global_reference_without_noexcept_ptr_type>;
			using global_const_pointer_invoke_type = remove_pointer_t<global_const_pointer_ptr_type>;
			using global_const_reference_invoke_type = remove_pointer_t<global_const_reference_ptr_type>;
			using global_const_pointer_without_noexcept_invoke_type = remove_pointer_t<global_const_pointer_without_noexcept_ptr_type>;
			using global_const_reference_without_noexcept_invoke_type = remove_pointer_t<global_const_reference_without_noexcept_ptr_type>;
			static constexpr bool is_object_v = true;
			static constexpr bool is_type_v = false;
			static constexpr bool is_pointer_v = false;
			static constexpr bool is_noexcept_v = call_info::is_noexcept_v;
			static constexpr bool is_member_v = false;
			static constexpr bool is_const_member_v = false;
			static constexpr size_t args_count = call_info::args_count;
		};

		/// <summary>
		/// 関数ポインタ、メンバ関数ポインタ、1つのoperator()を持つ関数オブジェクトのいずれかを指定し、関数型を取得する。
		/// 関数ポインタのとき、noexcept指定を取り除いた関数型となる。
		/// メンバ関数ポインタのとき、クラスを参照として第一引数に挿入し(const関数の場合、const修飾される)、noexcept指定を取り除いた関数型となる。
		/// 関数オブジェクトのとき、operator()をグローバル関数ポインタとしてみなしたうえで、noexceptを取り除いた関数型となる。
		/// </summary>
		template<class T>
		using function_t = typename function_info<Traits::remove_cvref_t<T>>::global_reference_without_noexcept_invoke_type;

		template<class R, class... Args>
		struct make_function {
			using type = R(Args...);
		};
		template<class R>
		struct make_function<R, void> {
			using type = R();
		};
		template<class R, class... Args>
		using make_function_t = make_function<R, Args...>::type;
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
	//[固有]：イテレータにも対応した参照外し型
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
	//[固有]：Enum class 演算子自動定義型
	//template<> struct enabling_***_enum : true_type {};で特殊化することで、
	//演算子を自動定義できる
	namespace Traits {
		template<Concepts::CEnum T> struct enabling_bitwise_for_enum : false_type {};
		template<Concepts::CEnum T> struct enabling_and_or_for_enum : enabling_bitwise_for_enum<T> {};
	}
	//[固有]：Enum class 演算子コンセプト
	namespace Traits::Concepts{
		template<class T>
		concept CBitwiseEnum = enabling_bitwise_for_enum<T>::value;
		template<class T>
		concept CAndOrEnum = enabling_and_or_for_enum<T>::value;
	}
	//[固有]：ビットフラグ操作
	namespace Traits {
		template<Concepts::CBitFlag T, Concepts::CBitFlag U>
		constexpr bool IncludeBitFlags(T value, U flags) noexcept {
			const size_t u = static_cast<size_t>(flags);
			return (static_cast<size_t>(value) & u) == u;
		}
		template<Concepts::CBitFlag T, Concepts::CBitFlag U>
		constexpr bool ExcludeBitFlags(T value, U flags) noexcept {
			const size_t u_inv = ~static_cast<size_t>(flags);
			return (static_cast<size_t>(value) | u_inv) == u_inv;
		}

	}
	//[memory]互換：pointer_traits
	namespace Traits {
		namespace Internal {
			template<class T, class U>
			struct DifferenceType { using type = U; };
			template<class T, class U>
			requires requires() { typename T::difference_type; }
			struct DifferenceType<T, U> { using type = T::difference_type; };
			template<class T, class U> struct RebindType;
			template<class Ptr, class U>
			requires requires() { typename Ptr::template rebind<U>; }
			struct RebindType<Ptr, U> {
				using type = typename Ptr::template rebind<U>;
			};
			template<template<class T, class ...Args> class Ptr, class U, class T, class ...Args>
			struct RebindType<Ptr<T, Args...>, U> {
				using type = Ptr<U, Args...>;
			};
			template<class T> struct ElementType;
			template<class Ptr>
			requires requires() { typename Ptr::element_type; }
			struct ElementType<Ptr> {
				using type = Ptr::element_type;
			};
			template<template<class T, class ...Args> class Ptr, class T, class ...Args>
			struct ElementType<Ptr<T, Args...>> {
				using type = T;
			};
			template<class Ptr, class E>
			concept CHasPointerTo = requires(E& r){
				Ptr::pointer_to(r);
			};
		}
		
		template<class Ptr>
		struct pointer_traits {
			using pointer = Ptr;
			using difference_type = typename Internal::DifferenceType<Ptr, ptrdiff_t>::type;
			using element_type =typename Internal::ElementType<Ptr>::type;
		public:
			template<class U>
			using rebind = typename Internal::RebindType<Ptr, U>::type;
		public:
			static pointer pointer_to(element_type& r) {
				static_assert(Internal::CHasPointerTo<Ptr, element_type>);
				return Ptr::pointer_to(r);
			}
		};
		template<class T>
		struct pointer_traits<T*> {
			using pointer = T*;
			using element_type = T;
			using difference_type = ptrdiff_t;
		public:
			template<class U>
			using rebind = U*;
		public:
			static constexpr pointer pointer_to(element_type& r) noexcept {
				return System::addressof(r);
			}
		};
	}
	//[type_traits]互換: make_unsigned
	namespace Traits {
		template<Concepts::CIntegral T>
		requires (!Concepts::CSame<T, bool>)
		struct make_unsigned {
			using type = int_t<false, sizeof(T)>;
		};
	}
	namespace Traits {
		template<Concepts::CIntegral T>
		using make_unsigned_t = make_unsigned<T>::type;
	}

	namespace Traits::Concepts {
		template<class T>
		concept CCanOutputStream = requires(std::ostream& os, T x) {
			os << x;
		};
	}
}

//未使用(参考のため残しておく)
#if false
export namespace System {
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
}
#endif

//Enum class 自動定義用演算子
using namespace System::Traits;
export template<Concepts::CAndOrEnum T>
constexpr T operator&(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) & static_cast<underlying_type_t<T>>(rhs));
}
export template<Concepts::CAndOrEnum T>
constexpr T operator|(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) | static_cast<underlying_type_t<T>>(rhs));
}
export template<Concepts::CBitwiseEnum T>
constexpr T operator^(T lhs, T rhs) noexcept {
	return static_cast<T>(static_cast<underlying_type_t<T>>(lhs) ^ static_cast<underlying_type_t<T>>(rhs));
}
export template<Concepts::CBitwiseEnum T>
constexpr T operator~(T x) noexcept {
	return static_cast<T>(~static_cast<underlying_type_t<T>>(x));
}
export template<Concepts::CAndOrEnum T>
constexpr T& operator&=(T& lhs, T rhs) noexcept {
	return lhs = lhs & rhs;
}
export template<Concepts::CAndOrEnum T>
constexpr T& operator|=(T& lhs, T rhs) noexcept {
	return lhs = lhs | rhs;
}
export template<Concepts::CBitwiseEnum T>
constexpr T& operator^=(T& lhs, T rhs) noexcept {
	return lhs = lhs ^ rhs;
}
