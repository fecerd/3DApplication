export module Math;
import CSTDINT;
import Traits;
using namespace System::Traits;
#undef NAN

//Math_Internal
namespace System::Math_Internal {
	template<size_t N> inline constexpr size_t Exp2 = 2ull * Exp2<N - 1>;
	template<> inline constexpr size_t Exp2<0ull> = 1ull;
	template<size_t N> inline constexpr size_t Exp10 = 10ull * Exp10<N - 1>;
	template<> inline constexpr size_t Exp10<0ull> = 1ull;
	template<Concepts::CFloating T> inline constexpr T EPSILON = 0.0;
	template<> inline constexpr float EPSILON<float> = 1.19209e-07f;
	template<> inline constexpr double EPSILON<double> = 2.22044604925031e-16;
	template<> inline constexpr long double EPSILON<long double> = 2.22044604925031e-16;
#undef INFINITY
}
using namespace System::Math_Internal;

//Math
export namespace System {
	class Math {
		Math() = delete;
		~Math() = delete;
	public:
		template<Concepts::CFloating T>
		static constexpr T PI = static_cast<T>(3.1415926535);
		template<Concepts::CFloating T, uint32_t numerator, uint32_t denominator>
		static constexpr T PIConstant = PI<T> * numerator / denominator;
		static constexpr double E = 2.71828182845904523536;
		static constexpr double LN2 = 0.693147180559945309417;
		static constexpr double Root2 = 1.4142135623730950;
		static constexpr double Deg2Rad = PI<double> * 2.0 / 360.0;
		static constexpr double Rad2Deg = 1.0 / Deg2Rad;
		static constexpr float Deg2Radf = static_cast<float>(Deg2Rad);
		static constexpr float Rad2Degf = static_cast<float>(Rad2Deg);
	public:
		static constexpr float INFINITY = FLOAT_INF;
		static constexpr float NAN = FLOAT_NAN;
	public:
		//計算機イプシロンテンプレート
		template<Concepts::CFloating T> static constexpr T EPSILON = Math_Internal::EPSILON<T>;
	public:
		template<size_t N> static constexpr size_t Exp2 = Math_Internal::Exp2<N>;
		template<size_t N> static constexpr size_t Exp10 = Math_Internal::Exp10<N>;
	public:
		static constexpr size_t LOG_LIMIT = 10;	//Math::log()の計算を打ち切る項数
		static constexpr size_t EXP_LIMIT = 30;	//Math::exp()の計算を打ち切る項数
		static constexpr size_t SQRT_LIMIT = 10;	//Math::sqrt()の計算を打ち切る項数
		static constexpr size_t TRIGO_LIMIT = 10;	//三角関数の計算を打ち切る項数
		template<Concepts::CFloating T> static constexpr T SIGNIFICANT_DIGIT = static_cast<T>(0.000000001);	//第n項までの和と第n-1項までの和の絶対値の差がこの定数に収まる場合、級数計算を打ち切る
	public:
		/// <summary>
		/// 引数の中で一番小さい値を取得する
		/// </summary>
		template<Concepts::CArithmetic T, Concepts::CArithmetic U, Concepts::CArithmetic ...Args>
		static constexpr T Min(T first, U second, Args ...args) noexcept {
			if constexpr (sizeof...(Args) == 0) return first < second ? first : second;
			else return first < second ? min(first, args...) : min(second, args...);
		}
		/// <summary>
		/// 引数の中で一番大きい値を取得する
		/// </summary>
		template<Concepts::CArithmetic T, Concepts::CArithmetic U , Concepts::CArithmetic ...Args>
		static constexpr T Max(T first, U second, Args ...args) noexcept {
			if constexpr (sizeof...(Args) == 0) return first < second ? second : first;
			else return first < second ? Math::Max(second, args...) : Math::Max(first, args...);
		}
		/// <summary>
		/// 絶対値を取得する
		/// </summary>
		template<Concepts::CArithmetic T>
		static constexpr auto Abs(T n) noexcept -> Traits::conditional_t<Traits::is_integral_v<T>, Traits::int_t<false, sizeof(T)>, T> {
			if constexpr (Traits::is_integral_v<T>) {
				if constexpr (Traits::is_unsigned_v<T>) return static_cast<Traits::int_t<false, sizeof(T)>>(n);
				else return static_cast<Traits::int_t<false, sizeof(T)>>(n >= 0 ? n : n * -1);
			}
			else return n >= 0 ? n : n * -1;
		}
		/// <summary>
		/// 符号を取得する
		/// </summary>
		/// <return>引数が正のときには1を、0のときには0を、負のときには-1を返す</return>
		template<Concepts::CArithmetic T>
		static constexpr T Sign(const T x) noexcept {
			if constexpr (is_unsigned_v<T>) return static_cast<T>(x == 0 ? 0 : 1);
			else return static_cast<T>(x < 0 ? -1 : x == 0 ? 0 : 1);
		}
		/// <summary>
		/// 同じ数値型の変数の値を入れ替える
		/// </summary>
		template<Concepts::CArithmetic T>
		static constexpr void Swap(T& x, T& y) noexcept {
			if (&x == &y) return;
			if constexpr (is_integral_v<T>) {
				x = x ^ y;
				y = x ^ y;
				x = x ^ y;
			}
			else {
				T tmp = x;
				x = y;
				y = tmp;
			}
		}
	public:
		/// <summary>
		/// 浮動小数点数の小数点以下を切り上げた整数値を取得する
		/// </summary>
		/// <typename name="T">浮動小数点数型</typename>
		/// <typeparam name="R">関数の戻り値型。省略した場合、T型と同じバイト長の符号付き整数型</typeparam>
		/// <param name="val">浮動小数点数</param>
		template<Concepts::CFloating T, class R = Traits::int_t<true, sizeof(T)>>
		static constexpr R Ceil(T val) noexcept {
			if (val < 0) return -Floor(-val);
			return val > static_cast<int64_t>(val) ? static_cast<R>(static_cast<int64_t>(val + 1)) : static_cast<R>(static_cast<int64_t>(val));
		}
		/// <summary>
		/// 浮動小数点数の小数点以下を切り捨てた整数値を取得する
		/// </summary>
		/// <typename name="T">浮動小数点数型</typename>
		/// <typeparam name="R">関数の戻り値型。省略した場合、T型と同じバイト長の符号付き整数型</typeparam>
		/// <param name="val">浮動小数点数</param>
		template<Concepts::CFloating T, class R = Traits::int_t<true, sizeof(T)>>
		static constexpr R Floor(T val) noexcept {
			return val >= 0 ? static_cast<R>(static_cast<int64_t>(val)) : -Ceil<R>(-val);
		}
		/// <summary>
		/// 浮動小数点数の小数点以下を四捨五入した整数値を取得する
		/// </summary>
		/// <typename name="T">浮動小数点数型</typename>
		/// <typeparam name="R">関数の戻り値型。省略した場合、T型と同じバイト長の符号付き整数型</typeparam>
		/// <param name="val">浮動小数点数</param>
		template<Concepts::CFloating T, class R = Traits::int_t<true, sizeof(T)>>
		static constexpr R Round(T val) noexcept {
			return static_cast<R>(static_cast<int64_t>(val >= 0 ? val + 0.5 : val - 0.5));
		}
	public:
		/// <summary>
		/// 正の平方根を取得する
		/// </summary>
		/// <returns>
		/// 正の平方根を返す。
		/// 引数が負数もしくはNanのとき、Nanを返す。
		/// 引数が正の無限大のとき、正の無限大を返す。
		/// 戻り値型は引数型が浮動小数点数型のときには同じ型、それ以外の型のときにはdoubleになる
		/// </returns>
		template<Concepts::CArithmetic T>
		static constexpr auto Sqrt(T x) noexcept -> conditional_t<Concepts::CFloating<T>, T, double> {
			if (Math::EqualZero(x)) return static_cast<conditional_t<Concepts::CFloating<T>, T, double>>(x);
			else if (x < 0 || Math::IsNan(x)) return Math::NAN;
			else if (x == Math::INFINITY) return Math::INFINITY;
			double dx = static_cast<double>(x);
			uint64_t X = Math::GetBit(dx);
			uint64_t Y = 0x5FE6EB50C7B537A9 - (X >> 1);
			double y = Math::GenerateFloat<double>(Y);
			double x2 = x * 0.5;
			for (size_t i = 0; i < 5; ++i) y = y * (1.5 - (x2 * y * y));
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(x * y);
			else return x * y;
		}
		/// <summary>
		/// 正の平方根の逆数を取得する
		/// </summary>
		/// <returns>
		/// 正の平方根の逆数を返す。
		/// 引数が負数もしくはNanのとき、Nanを返す。
		/// 引数が正の無限大のとき、0を返す。
		/// 戻り値型は引数型が浮動小数点数型のときには同じ型、それ以外の型のときにはdoubleになる
		/// </returns>
		template<Concepts::CArithmetic T>
		static constexpr auto SqrtInv(T x) noexcept -> conditional_t<Concepts::CFloating<T>, T, double> {
			if (Math::EqualZero(x)) return static_cast<conditional_t<Concepts::CFloating<T>, T, double>>(x);
			else if (x < 0 || Math::IsNan(x)) return Math::NAN;
			else if (x == Math::INFINITY) return 0;
			double dx = static_cast<double>(x);
			uint64_t X = Math::GetBit(dx);
			uint64_t Y = 0x5FE6EB50C7B537A9 - (X >> 1);
			double y = Math::GenerateFloat<double>(Y);
			double x2 = x * 0.5;
			for (size_t i = 0; i < 5; ++i) y = y * (1.5 - (x2 * y * y));
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(y);
			else return y;
		}
	public:
		template<Concepts::CFloating T>
		static constexpr bool IsInteger(T x) noexcept {
			if (x == Math::INFINITY || x == -Math::INFINITY || x != x) return false;	//無限大と非数は整数でない
			if (Math::EqualZero(x)) return true;	//0は整数
			x = Math::Abs(x);	//xを正数化
			if (x < 1) return false;
			if (Math::LessEq(static_cast<T>(static_cast<int_t<false, sizeof(T)>>(1ull) << FRAC_DIGIT<T>), x)) return true;	//1.00...0 * 2^(仮数部ビット数)以上のとき、小数点以下の桁は存在しない
			return static_cast<T>(static_cast<int_t<false, sizeof(T)>>(x)) == x;	//xが[1, 1 * 2^(仮数部ビット数))の範囲内の場合、同じサイズ間で浮動小数点数型 -> 整数型 -> 浮動小数点数型とキャストすることで整数部が取得できる
		}
		template<Concepts::CFloating T>
		static constexpr bool IsOdd(T x) noexcept {
			if (x == Math::INFINITY || x == -Math::INFINITY || x != x) return false;	//無限大と非数は奇数でない
			if (Math::EqualZero(x)) return false;	//0は奇数でない
			x = Math::Abs(x);	//xを正数化
			if (x < 1) return false;	//(0, 1)の範囲に奇数はない
			if (Math::LessEq(static_cast<T>(static_cast<int_t<false, sizeof(T)>>(1ull) << (FRAC_DIGIT<T> +1)), x)) return false;	//1.00...0 * 2^(仮数部ビット数+1)以上のとき、最下位ビットは0 -> 奇数でない
			int_t<false, sizeof(T)> i = static_cast<int_t<false, sizeof(T)>>(x);	//xが[1, 1 * 2^(仮数部ビット数+1))の範囲内の場合、同じサイズ間で浮動小数点数型 -> 整数型 -> 浮動小数点数型とキャストすることで整数部が取得できる
			return static_cast<T>(i) != x ? false : i % 2 != 0;
		}
		template<Concepts::CFloating T>
		static constexpr bool IsEven(T x) noexcept {
			if (x == Math::INFINITY || x == -Math::INFINITY || x != x) return false;	//無限大と非数は偶数でない
			if (Math::EqualZero(x)) return true;	//0は偶数
			x = Math::Abs(x);	//xを正数化
			if (x < 1) return false;	//(0, 1)の範囲に偶数はない
			if (Math::LessEq(static_cast<T>(static_cast<int_t<false, sizeof(T)>>(1ull) << (FRAC_DIGIT<T> +1)), x)) return true;	//1.00...0 * 2^(仮数部ビット数+1)以上のとき、最下位ビットは0 -> 偶数
			int_t<false, sizeof(T)> i = static_cast<int_t<false, sizeof(T)>>(x);	//xが[1, 1 * 2^(仮数部ビット数+1))の範囲内の場合、同じサイズ間で浮動小数点数型 -> 整数型 -> 浮動小数点数型とキャストすることで整数部が取得できる
			return static_cast<T>(i) != x ? false : i % 2 == 0;
		}
	public:
		template<Concepts::CIntegral T> static constexpr T Factorial(const T x) noexcept { return x == 0 ? 1 : x * Factorial(x - 1); }
	public:
		template<Concepts::CArithmetic T>
		static constexpr auto Exp(const T x) noexcept {
			if constexpr (is_floating_point_v<T>) {
				if (x == INFINITY) return static_cast<T>(INFINITY);	//inf = e^inf
				else if (x == -INFINITY) return static_cast<T>(0);	//0 = e^-inf
			}
			if (EqualZero(x)) {
				if constexpr (is_floating_point_v<T>) return static_cast<T>(1.0);	//1 = e^0
				return 1.0;
			}
			double ret = 1.0;	//戻り値
			double u = Math::Abs(x);
			size_t count = 0;	//x = sign * u(0.dd...d) * 2^count
			while (u > 1.0) {
				u /= 2.0;
				++count;
			}
			const double u1 = u;	//uの値を保存
			double prev = ret;	//ひとつ前の級数計算の結果を保存する
			//e^x == e^(±u * 2^count) == (e^±u)^(1 << count)
			for (size_t n = 1; n < EXP_LIMIT; ++n) {
				ret += u / Math::Factorial(n);	//冪級数n = [0, inf] (x^n / n!)を使用する
				if (Math::Abs(ret - prev) < SIGNIFICANT_DIGIT<double>) break;	//級数計算の結果が変わらなくなった場合、ループを抜ける
				prev = ret;
				u *= u1;	//冪級数の次項の分子u1^n = u * u1
			}
			const double tmp = ret;	//e^uの結果
			for (uint64_t i = 0, end = 1ull << count; i < end && ret != INFINITY && ret != -INFINITY; ++i) ret *= tmp;	//ret^(2 ^ count)
			if (x < 0) ret = 1.0 / ret;	//e^-n == 1 / (e^n)
			if constexpr (is_floating_point_v<T>) return static_cast<T>(ret);
			else return ret;
		}
		template<Concepts::CArithmetic T>
		static constexpr auto Log(const T x) noexcept {
			double ret = 0.0;	//戻り値
			if (x < 0) ret = Math::NAN;	//底が負数の場合、対数は定義されない
			else if (Math::EqualZero(x)) ret = -Math::INFINITY;	//ln(0) = -inf
			else if (Math::Equal(x, 1.0)) ret = 0.0;	//ln(1)	= 0
			else if (x == Math::INFINITY) ret = Math::INFINITY;	//ln(inf) = inf
			else {
				double u = static_cast<double>(x);
				size_t count = 0;	//x = u(1.dd...d) * 2^count
				while (u > 2.0) {
					u /= 2.0;
					++count;
				}
				u = (u - 1.0) / (u + 1.0);
				double u2 = u * u;
				double prev = 0.0;	//ひとつ前の級数計算の結果を保存する
				for (size_t n = 1; n < LOG_LIMIT; ++n) {
					ret += u / (2.0 * n - 1.0);	//級数n = [1, inf] 2 * Σ{(x - 1)/(x + 1)}^(2n - 1) / (2n - 1)を使用する
					if (Math::Abs(ret - prev) < SIGNIFICANT_DIGIT<double>) break;	//級数計算の結果が変わらなくなった場合、ループを抜ける
					prev = ret;
					u *= u2;
				}
				ret *= 2.0;
				ret += count * Math::LN2;	//x = u * 2^countより、ln(x) = count * ln(2) + ln(u)
			}
			if constexpr (is_floating_point_v<T>) return static_cast<T>(ret);
			else return ret;
		}
		template<Concepts::CArithmetic T, Concepts::CArithmetic U>
		static constexpr auto Pow(T x, U y) noexcept {
			if constexpr (is_integral_v<T> && is_unsigned_v<U>) {
				int_t<is_signed_v<T>, sizeof(size_t)> ret = x;
				if (y == 0) return static_cast<decltype(ret)>(1);
				for (U i = 1; i < y; ++i) ret *= x;
				return ret;
			}
			else {
				double ret = 1.0;	//戻り値
				//指数が整数型の場合、単純な乗算の繰り返しで計算する
				if constexpr (is_integral_v<U>) {
					double tx = y < 0 ? 1.0 / static_cast<double>(x) : static_cast<double>(x);
					uint64_t ty = y < 0 ? static_cast<uint64_t>(-static_cast<int64_t>(y)) : static_cast<uint64_t>(y);
					for (uint64_t i = 0; i < ty && ret != Math::INFINITY && ret != -Math::INFINITY; ++i) ret *= tx;
				}
				else {
					double tx = static_cast<double>(x);
					double ty = static_cast<double>(y);
					if (Math::Equal(tx, 1.0) || Math::EqualZero(ty)) ret = 1.0;	//1 = 1^y or x^0
					else if (Math::Equal(ty, 1.0)) ret = tx;	//x = x^1
					else if (tx == Math::INFINITY) ret = ty < 0 ? 0.0 : Math::INFINITY;	//0 = inf^-n, inf = inf^n
					else if (tx == -Math::INFINITY) ret = ty < 0
						? (Math::IsOdd(ty) ? -0.0 : 0.0)	//±0 = -inf^-n
						: (Math::IsOdd(ty) ? -Math::INFINITY : Math::INFINITY);	//±inf = -inf^n
					else if (Math::EqualZero(tx)) ret = ty < 0
						? (Math::IsOdd(ty) ? (Math::SignBit(tx) ? -Math::INFINITY : Math::INFINITY) : Math::INFINITY) //±inf = ±0^-n
						: (Math::IsOdd(ty) ? (Math::SignBit(tx) ? -0.0 : 0.0) : 0.0);	//±0 = ±0^n
					else if (Math::Equal(tx, -1.0) && Math::IsInf(ty)) ret = 1.0;	//1 = -1^±inf
					else if (Math::IsInf(ty)) ret = Math::Less(Math::Abs(tx), 1.0)
						? (ty > 0 ? 0.0 : Math::INFINITY)	//0 = ±0.dd...d^inf, inf = ±0.dd...d^-inf
						: (ty > 0 ? Math::INFINITY : 0.0);	//inf = ±d.dd...d^inf, 0 = ±d.dd...d^-inf
					else {
						//指数が63ビットで表現可能な整数の場合、単純な乗算の繰り返しで計算する
						if (Math::IsInteger(ty) && Math::Less(Math::Abs(ty), static_cast<double>(1ull << 63))) {
							if (ty < 0) {
								tx = 1.0 / tx;
								ty = -ty;
							}
							for (uint64_t i = 0, iy = static_cast<uint64_t>(ty); i < iy && ret != Math::INFINITY && ret != -Math::INFINITY; ++i) ret *= tx;
						}
						else {
							if (tx > 0) ret = Math::Exp(ty * Math::Log(tx));	//x^y == e^(y * loge(x))
							else ret = Math::NAN;	//x < 0かつyが整数・無限大でない場合、x^yは定義されない
						}
					}
				}
				if constexpr (is_floating_point_v<T>) return static_cast<T>(ret);
				else return ret;
			}
		}
	public:
		//lhs < rhs -> -1, lhs == rhs -> 0, lhs > rhs -> 1
		template<Concepts::CArithmetic L, Concepts::CArithmetic R>
		static constexpr int Compare(const L lhs, const R rhs) noexcept {
			if constexpr (Traits::is_floating_point_v<L> || Traits::is_floating_point_v<R>) {
				double sub_abs = Math::Abs(static_cast<double>(lhs) - static_cast<double>(rhs));
				double ep = Math::Max(1.0, Math::Abs<double>(lhs), Math::Abs<double>(rhs)) * Math::EPSILON<double>;
				return sub_abs < ep ? 0 : (lhs < rhs ? -1 : 1);
			}
			else if constexpr ((Traits::is_unsigned_v<L> && Traits::is_unsigned_v<R>) || (Traits::is_signed_v<L> && Traits::is_signed_v<R>)) return (lhs < rhs) ? -1 : (lhs == rhs ? 0 : 1);
			else if constexpr (Traits::is_signed_v<L>) return (lhs < 0 || lhs < rhs) ? -1 : (lhs == rhs ? 0 : 1);
			else return rhs < 0 ? 1 : (lhs < rhs ? -1 : (lhs == rhs ? 0 : 1));
		}
		template<Concepts::CArithmetic L, Concepts::CArithmetic R> static constexpr bool Equal(const L lhs, const R rhs) noexcept { return Math::Compare(lhs, rhs) == 0; }
		template<Concepts::CArithmetic L, Concepts::CArithmetic R> static constexpr bool Less(const L lhs, const R rhs) noexcept { return Math::Compare(lhs, rhs) < 0; }
		template<Concepts::CArithmetic L, Concepts::CArithmetic R> static constexpr bool Greater(const L lhs, const R rhs) noexcept { return Math::Compare(lhs, rhs) > 0; }
		template<Concepts::CArithmetic L, Concepts::CArithmetic R> static constexpr bool LessEq(const L lhs, const R rhs) noexcept { return Math::Compare(lhs, rhs) <= 0; }
		template<Concepts::CArithmetic L, Concepts::CArithmetic R> static constexpr bool GreaterEq(const L lhs, const R rhs) noexcept { return Math::Compare(lhs, rhs) >= 0; }
		template<Concepts::CArithmetic T> static constexpr bool EqualZero(const T x) noexcept { return Math::Compare(x, static_cast<T>(0)) == 0; }
		//xが区画{leftend, rightend}内に存在する -> true, other -> false
		//テンプレート引数はtrue -> 閉区間、false -> 開区間
		template<Concepts::CArithmetic T, bool left_closed, bool right_closed>
		static constexpr bool InRange(const T x, const T leftend, const T rightend) noexcept {
			if constexpr (left_closed && right_closed) return Math::LessEq(leftend, x) && Math::LessEq(x, rightend);
			else if constexpr (left_closed) return Math::LessEq(leftend, x) && Math::Less(x, rightend);
			else if constexpr (right_closed) return Math::Less(leftend, x) && Math::LessEq(x, rightend);
			else return Math::Less(leftend, x) && Math::Less(x, rightend);
		}
		//xが開区間(leftend, rightend)内に存在する -> true, other -> false
		template<Concepts::CArithmetic T> static constexpr bool InOpenRange(const T x, const T leftend, const T rightend) noexcept { return Math::InRange<T, false, false>(x, leftend, rightend); }
		//xが閉区間[leftend, rightend]内に存在する -> true, other -> false
		template<Concepts::CArithmetic T> static constexpr bool InClosedRange(const T x, const T leftend, const T rightend) noexcept { return Math::InRange<T, true, true>(x, leftend, rightend); }
	public:
		template<Concepts::CArithmetic T>
		static constexpr T Clamp(T x, T leftend, T rightend) noexcept { return Math::Less(x, leftend) ? leftend : Math::Greater(x, rightend) ? rightend : x; }
#undef min
#undef max
		template<Concepts::CArithmetic T>
		static constexpr T ClampPeriodic(T x, T min, T max) noexcept {
			if (!Less(min, max)) return x;
			T range = max - min;
			while (x > max) x -= range;
			while (x < min) x += range;
			return x;
		}
	public:
		/// <summary>
		/// 整数xの10^nの位の数字を取得する
		/// </summary>
		/// <typeparam name="T">整数型</typeparam>
		/// <param name="x">整数</param>
		/// <param name="n">乗数。(例: 0 -> 1(=10^0)の位、 3 -> 1000(=10^3)の位)</param>
		/// <returns>
		/// 指定した位の数字[0, 9]。
		/// 存在しない位を指定した場合、0を返す
		/// </returns>
		template<Concepts::CIntegral T>
		static constexpr size_t GetDigit(T x, size_t n) noexcept {
			if (CountDigit(x) <= n) return 0;
			uint64_t t = 0;
			if constexpr (Traits::is_signed_v<T>) t = static_cast<uint64_t>(x >= 0 ? x : -x);
			else t = static_cast<uint64_t>(x);
			for (size_t i = 0; i < n; ++i) t /= 10ull;
			return t % 10;
		}
		/// <summary>
		/// 10進整数xの桁数を取得する
		/// </summary>
		/// <typeparam name="T">整数型</typeparam>
		/// <param name="n">整数</param>
		template<Concepts::CIntegral T>
		static constexpr size_t CountDigit(T n) noexcept {
			size_t tmp = Abs(n);
			if (tmp >= Exp10<10>) {
				if (tmp >= Exp10<15>) {
					if (tmp >= Exp10<18>) return tmp >= Exp10<19> ? 20 : 19;
					else return tmp >= Exp10<17> ? 18 : tmp >= Exp10<16> ? 17 : 16;
				}
				else {
					if (tmp >= Exp10<13>) return tmp >= Exp10<14> ? 15 : 14;
					else return tmp >= Exp10<12> ? 13 : tmp >= Exp10<11> ? 12 : 11;
				}
			}
			else {
				if (tmp >= Exp10<5>) {
					if (tmp >= Exp10<8>) return tmp >= Exp10<9> ? 10 : 9;
					else return tmp >= Exp10<7> ? 8 : tmp >= Exp10<6> ? 7 : 6;
				}
				else {
					if (tmp >= Exp10<3>) return tmp >= Exp10<4> ? 5 : 4;
					else return tmp >= Exp10<2> ? 3 : tmp >= Exp10<1> ? 2 : 1;
				}
			}
			return 0;
		}
	public:
		template<Concepts::CFloating T> static constexpr bool IsInf(T n) noexcept { return n == static_cast<T>(Math::INFINITY) || n == -static_cast<T>(Math::INFINITY); }
		template<Concepts::CFloating T> static constexpr bool IsNan(T n) noexcept { return n != n; }
		//浮動小数点数xの符号ビットが1 -> true, それ以外 -> false
		//get_bit<>()が自己定義の場合、-0.0とNanの符号を正として扱う
		template<Concepts::CFloating T> static constexpr bool SignBit(T x) noexcept { return Math::GetBit<T>(x) & Traits::BITMASK_M<sizeof(T) * 8, sizeof(T) * 8>; }
	public:
		//IEEE754標準における浮動小数点数の内部表現のビット列から、浮動小数点数型を生成する
		//符号なし4バイト整数型 -> float(単精度)
		//符号なし8バイト整数型 -> double(倍精度)
		template<Concepts::CFloating T>
		static constexpr T GenerateFloat(const Traits::int_t<false, sizeof(T)> x) noexcept {
			using BITTYPE = Traits::int_t<false, sizeof(T)>;
			using STYPE = Traits::int_t<true, sizeof(T)>;
			T sign = static_cast<T>(x & Traits::BITMASK_M<sizeof(T) * 8, sizeof(T) * 8> ? -1 : 1);	//符号ビットから符号取得
			STYPE exp = (x >> Traits::FRAC_DIGIT<T>) & Traits::BITMASK_L<Traits::EXP_DIGIT<T>>;	//指数部ビット(バイアスなし)取得
			BITTYPE frac = x & Traits::BITMASK_L<Traits::FRAC_DIGIT<T>>;	//仮数部ビット取得
			if (exp == 0) {
				if (frac == 0) return sign * static_cast<T>(0);	//指数部・仮数部がともに0 -> 0
				else exp += 1;	//仮数部が0以外 -> 非正規化数(仮数は1 - バイアス値)
			}
			else if (exp == Traits::BITMASK_L<Traits::EXP_DIGIT<T>>) return frac == 0 ? sign * Math::INFINITY : Math::NAN;	//指数部が最大かつ仮数部が0 -> 無限大, 仮数部が0以外 -> 非数
			else frac |= Traits::BITMASK_M<Traits::FRAC_DIGIT<T> +1, Traits::FRAC_DIGIT<T> +1>;	//正規化数(仮数部ビットにケチ表現の1ビットを追加)
			exp -= static_cast<STYPE>(Traits::BITMASK_L<Traits::EXP_DIGIT<T> -1>);	//指数部からバイアス値を引く
			T ret = static_cast<T>(frac);	//整数fracは2^0 * 1.frac(0.frac)として表現可能
			for (int i = 0; i < Traits::FRAC_DIGIT<T>; ++i) ret /= static_cast<T>(2.0);	//2^-(仮数部ビット数) * 1.frac(0.frac)
			//指数部の値の範囲内であれば2倍と1/2倍の計算は仮数部ビットの情報を落とさない
			if (exp >= 0) for (STYPE i = 0; i < exp; ++i) ret *= static_cast<T>(2.0);
			else for (STYPE i = 0, end = -exp; i < end; ++i) ret /= static_cast<T>(2.0);
			return ret * sign;
		}
		/// <summary>
		/// 浮動小数点数からIEEE754標準における浮動小数点数の内部表現のビット列を生成する
		/// </summary>
		/// <param name="x">変換する浮動小数点数</param>
		/// <returns>内部表現のビット列。Nanの仮数部は0固定</returns>
		template<Concepts::CFloating T>
		static constexpr Traits::int_t<false, sizeof(T)> GetBit(T x) noexcept {
			using BITTYPE = Traits::int_t<false, sizeof(T)>;
			using STYPE = Traits::int_t<true, sizeof(T)>;
			if (x == Math::INFINITY) return Traits::BITMASK_M<Traits::FRAC_DIGIT<T> +1, Traits::FRAC_DIGIT<T> +Traits::EXP_DIGIT<T>>;
			else if (x == -Math::INFINITY) return Traits::BITMASK_M<Traits::FRAC_DIGIT<T> +1, sizeof(T) * 8>;
			else if (x != x) return Traits::BITMASK_M<Traits::FRAC_DIGIT<T>, Traits::FRAC_DIGIT<T> +Traits::EXP_DIGIT<T>>;
			BITTYPE sign = x < 0 || x == -0.0 ? Traits::BITMASK_M<sizeof(T) * 8, sizeof(T) * 8> : 0;	//符号
			x = sign == 0 ? x : -x;	//xを非負数化
			STYPE exp = 0;
			while (Math::GreaterEq<T>(x, 2)) {
				++exp;
				x /= static_cast<T>(2);
			}
			while (x < 1) {
				--exp;
				if (exp == -static_cast<STYPE>(Traits::BITMASK_L<Traits::EXP_DIGIT<T> -1>)) break;
				x *= static_cast<T>(2);
			}
			BITTYPE e = exp > -static_cast<STYPE>(Traits::BITMASK_L<Traits::EXP_DIGIT<T> -1>) ? static_cast<BITTYPE>(exp + static_cast<STYPE>(Traits::BITMASK_L<Traits::EXP_DIGIT<T> -1>)) << Traits::FRAC_DIGIT<T> : 0;
			BITTYPE frac = static_cast<BITTYPE>(x * static_cast<T>(1ull << Traits::FRAC_DIGIT<T>));
			frac &= Traits::BITMASK_L<Traits::FRAC_DIGIT<T>>;
			return sign | e | frac;
		}
	public:
		//浮動小数点数fの指数部が示す値を取得する
		//float型は0から255、double型は0から2047を返す
		template<Concepts::CFloating T>
		static constexpr uint64_t GetExp(T f) noexcept { return (Traits::BITMASK_M<Traits::FRAC_DIGIT<T> +1, Traits::FRAC_DIGIT<T> +Traits::EXP_DIGIT<T>> &Math::GetBit(f)) >> Traits::FRAC_DIGIT<T>; }
		//浮動小数点数f = (-1)^s * 2^exp * 1.frac(0.frac)の指数expを取得する
		//正規化数とNan、Infは(内部表現の値) - (バイアス値)を返し、非正規化数と0は1 - (バイアス値)を返す
		template<Concepts::CFloating T> static constexpr int GetExpValue(T f) noexcept {
			uint64_t val = GetExp(f);
			return val == 0 ? 1 - Traits::EXP_BIAS<T> : static_cast<int>(val) - Traits::EXP_BIAS<T>;
		}
		//浮動小数点数fが正規化数の場合、trueを返す
		//非正規化数、Inf、Nan、0の場合、falseを返す
		template<Concepts::CFloating T> static constexpr bool IsNormal(T f) noexcept { return Math::InClosedRange<uint64_t>(GetExp(f), 1, Traits::BITMASK_L<Traits::EXP_DIGIT<T>> -1); }
		//浮動小数点数f = (-1)^s * 2^exp * 1.frac(0.frac)の仮数部1.frac(0.frac)を整数型で取得する
		//Nanは実際の内部値にかかわらず0.10....0、Infは0.00....0として計算する
		template<Concepts::CFloating T>
		static constexpr uint64_t GetFracValue(T f) noexcept { return (Math::GetBit(f) & Traits::BITMASK_L<Traits::FRAC_DIGIT<T>>) | ((Math::IsNormal(f) ? 1ull : 0ull) << Traits::FRAC_DIGIT<T>); }
	public:
		//x[rad]の正弦を返す
		template<Concepts::CArithmetic T>
		static constexpr auto Sin(T x) noexcept {
			double ret = 0.0;	//戻り値
			if (EqualZero(x)) ret = static_cast<double>(x);	//0 = sin(±0)
			else if (IsInf(x)) ret = Math::NAN;	//sin(±inf)は定義されない
			else {
				double rad = static_cast<double>(x);	//正弦を求めるラジアン
				constexpr double PI2 = PIConstant<double, 2, 1>;
				//sin(rad) = sin(rad ± 2π)のため、radを[0, 2π]の範囲に収める
				while (rad > PI2) rad -= PI2;
				while (rad < 0) rad += PI2;
				if (EqualZero(rad) || Equal(rad, PI<double>)) ret = 0.0;	//0 = sin(0) or sin(π)
				else if (Equal(rad, PIConstant<double, 1, 2>)) ret = 1.0;	//1 = sin(π/2)
				else if (Equal(rad, PIConstant<double, 3, 2>)) ret = -1.0;	//-1 = sin(3π/2)
				else {
					double sign = 1.0;	//戻り値の符号
					//radが(π, 2π)の範囲のとき、sin(rad) = -sin(rad - π)
					if (rad > PI<double>) {
						rad -= PI<double>;
						sign *= -1.0;
					}
					if (rad > (PIConstant<double, 1, 2>)) rad = PI<double> - rad;	//radが(π/2, π)の範囲のとき、sin(rad) = sin(π - rad)
					double tx = rad;	//マクローリン展開の分子
					double fact = 1.0;	//マクローリン展開の分母
					for (size_t i = 1; i < TRIGO_LIMIT; ++i) {
						if (i % 2 != 0) ret += tx / fact;	//級数n = [1, inf] rad^(2n-1) / (2n - 1)!を使用する
						else ret -= tx / fact;
						tx *= rad * rad;
						fact *= (2.0 * i) * (2.0 * i + 1.0);
					}
					ret *= sign;
				}
			}
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//x[rad]の余弦を返す
		//Math::Sin(x + Math::PI / 2.0)による実装(cos(x) = sin(x + π/2))
		template<Concepts::CArithmetic T>
		static constexpr auto Cos(T x) noexcept { return Math::Sin(static_cast<T>(x + PIConstant<double, 1, 2>)); }
		/// <summary>
		/// x[rad]の正弦と余弦を取得する
		/// </summary>
		/// <param name="outSin">正弦を返す変数への参照</param>
		/// <param name="outCos">余弦を返す変数への参照</param>
		template<Concepts::CFloating T>
		static constexpr void SinCos(T x, T& outSin, T& outCos) noexcept {
			x = ClampPeriodic<T>(x, 0.f, PIConstant<T, 2, 1>);
			outSin = Sin(x);
			outCos = static_cast<T>(Sqrt(1.0 - outSin * outSin));
			if (PIConstant<T, 1, 2> < x && x < PIConstant<T, 3, 2>) {
				outCos *= -1;
			}
		}
		//x[rad]の正接を返す
		//Math::Sin(x) / Math::Cos(x)による実装(tan(x) = sin(x) / cos(x))
		template<Concepts::CArithmetic T>
		static constexpr auto Tan(T x) noexcept {
			const auto s = Math::Sin(x);
			const auto c = Math::Cos(x);
			double ret = Math::Equal(c, 0) ? s > 0 ? Math::INFINITY : -Math::INFINITY : s / c;
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆正接を返す([rad])
//[x != Nan] -> [-PI / 2 <= ret <= PI / 2]
		template<Concepts::CArithmetic T>
		static constexpr auto Atan(T x) noexcept {
			double ret = 0.0;	//戻り値
			if (EqualZero(x)) ret = static_cast<double>(x);	//±0 = atan(±0)
			else if (IsInf(x)) ret = (x < 0 ? -1.0 : 1.0) * PIConstant<double, 1, 2>;
			else {
				const double sign = x < 0 ? -1.0 : 1.0;	//戻り値の符号(xの符号と一致する)
				const double x_abs = Abs(x);	//xの絶対値
				//マクローリン展開
				auto maclaurin = [](double tx) -> double {
					double result = tx;	//計算結果
					double xn = tx * tx * tx;	//項の分子
					for (size_t i = 1; i < TRIGO_LIMIT; ++i) {
						result += (i % 2 == 0 ? xn : -xn) / (2.0 * i + 1.0);	//級数n = [0, inf] -1^n * x^(2n + 1) / (2n + 1)を使用
						xn *= tx * tx;
					}
					return result;
				};
				//xの絶対値の大きさによって近似式を使い分ける
				if (x_abs < 0.42) ret = sign * maclaurin(x_abs);
				else if (x_abs < 2.4) ret = sign * (PIConstant<double, 1, 4> + maclaurin((x_abs - 1.0) / (x_abs + 1.0)));
				else ret = sign * (PIConstant<double, 1, 2> - maclaurin(1.0 / x_abs));
			}
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//対辺yと隣辺xから逆正接を返す([rad])
		//[x != Nan && y != Nan] -> [-PI <= ret <= PI]
		//Math::Atan(y / x) + (0, PI / 2, -PI / 2)による実装
		template<Concepts::CArithmetic T>
		static constexpr auto Atan2(T y, T x) noexcept {
			double ret = 0.0;
			if (EqualZero(y)) ret = Math::GreaterEq(x, 0) ? static_cast<double>(y) : PI<double>;
			else if (EqualZero(x)) ret = PIConstant<double, 1, 2> * (y > 0 ? 1.0 : -1.0);
			else if (IsInf(y)) ret = Sign(y) * (x == Math::INFINITY ? PIConstant<double, 1, 4> : (x == -Math::INFINITY ? PIConstant<double, 3, 4> : PIConstant<double, 1, 2>));
			else if (IsInf(x)) ret = x == Math::INFINITY ? static_cast<double>(y) * 0.0 : Sign(y) * PI<double>;
			else ret = (x > 0 ? 0.0 : (y < 0 ? -PI<double> : PI<double>)) + Math::Atan(static_cast<double>(y) / static_cast<double>(x));
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆正弦を返す([rad])
		//[-1.0 <= x <= 1.0] -> [-PI / 2 <= ret <= PI / 2]
		//Math::Atan(x / (1 - x^2))による実装
		template<Concepts::CArithmetic T>
		static constexpr auto Asin(T x) noexcept {
			double ret = 0.0;
			if (EqualZero(x)) ret = static_cast<double>(x);
			else if (Abs(x) > 1.0) ret = Math::NAN;
			else {
				const double s = static_cast<double>(x);
				const double c = Sqrt(1.0 - s * s);
				ret = EqualZero(c) ? Sign(s) * PIConstant<double, 1, 2> : Math::Atan(s / c);
			}
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆余弦を返す([rad])
		//[-1.0 <= x <= 1.0] -> [0 <= ret <= PI]
		//PI / 2 - Math::Asin(x)による実装
		template<Concepts::CArithmetic T>
		static constexpr auto Acos(T x) noexcept {
			double ret = 0.0;
			if (Equal(x, 1.0)) ret = 0.0;
			else if (Equal(x, -1.0)) ret = PI<double>;
			else if (Abs(x) > 1.0) ret = Math::NAN;
			else ret = (PIConstant<double, 1, 2>) - Math::Asin(static_cast<double>(x));
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
	public:
		//x[°]の正弦を返す
		template<Concepts::CArithmetic T>
		static constexpr auto SinDeg(T x) noexcept {
			double ret = Math::Clamp(Math::Sin(static_cast<double>(x) * Math::Deg2Rad), -1.0, 1.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//x[°]の余弦を返す
		template<Concepts::CArithmetic T>
		static constexpr auto CosDeg(T x) noexcept {
			const double ret = Math::Clamp(Math::Cos(static_cast<double>(x) * Math::Deg2Rad), -1.0, 1.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//x[°]の正弦と余弦を取得する
		template<Concepts::CFloating T>
		static constexpr void SinCosDeg(T x, T& outSin, T& outCos) noexcept {
			SinCos(x * Math::Deg2Radf, outSin, outCos);
		}
		//x[°]の正接を返す
		template<Concepts::CArithmetic T>
		static constexpr auto TanDeg(T x) noexcept {
			const double ret = Math::Tan(static_cast<double>(x) * Math::Deg2Rad);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆正弦を返す([°])
		//[-1.0 <= x <= 1.0] -> [-90.0 <= ret <= 90.0]
		template<Concepts::CArithmetic T>
		static constexpr auto AsinDeg(T x) noexcept {
			const double ret = Clamp(Asin<double>(x) * Math::Rad2Deg, -90.0, 90.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆余弦を返す([°])
		//[-1.0 <= x <= 1.0] -> [0.0 <= ret <= 180.0]
		template<Concepts::CArithmetic T>
		static constexpr auto AcosDeg(T x) noexcept {
			const double ret = Clamp(Acos<double>(Clamp<double>(x, -1.0, 1.0)) * Math::Rad2Deg, 0.0, 180.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//xの逆正接を返す([°])
		//[x != Nan] -> [-90.0 <= ret <= 90.0]
		template<Concepts::CArithmetic T>
		static constexpr auto AtanDeg(T x) noexcept {
			const double ret = Clamp(Atan<double>(x) * Math::Rad2Deg, -90.0, 90.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
		//対辺yと隣辺xから逆正接を返す([°])
		//[x != Nan && y != Nan] -> [-180.0 <= ret <= 180.0]
		template<Concepts::CArithmetic T>
		static constexpr auto Atan2Deg(T y, T x) noexcept {
			const double ret = Clamp(Atan2<double>(y, x) * Math::Rad2Deg, -180.0, 180.0);
			if constexpr (Concepts::CFloating<T>) return static_cast<T>(ret);
			else return ret;
		}
	private:
		static constexpr uint32_t murmur_32_scramble(uint32_t k) {
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			return k;
		}
	public:
		static inline uint32_t murmur3_32(const void* data, size_t len, uint32_t seed)
		{
			const uint8_t* key = static_cast<const uint8_t*>(data);
			uint32_t h = seed;
			uint32_t k;
			/* Read in groups of 4. */
			for (size_t i = len >> 2; i; i--) {
				// Here is a source of differing results across endiannesses.
				// A swap here has no effects on hash properties though.
				k = (key[3] << 24) | (key[2] << 16) | (key[1] << 8) | (key[0]);
				key += sizeof(uint32_t);
				h ^= murmur_32_scramble(k);
				h = (h << 13) | (h >> 19);
				h = h * 5 + 0xe6546b64;
			}
			/* Read the rest. */
			k = 0;
			for (size_t i = len & 3; i; i--) {
				k <<= 8;
				k |= key[i - 1];
			}
			// A swap is *not* necessary here because the preceding loop already
			// places the low bytes in the low places according to whatever endianness
			// we use. Swaps only apply when the memory is copied in a chunk.
			h ^= murmur_32_scramble(k);
			/* Finalize. */
			h ^= len;
			h ^= h >> 16;
			h *= 0x85ebca6b;
			h ^= h >> 13;
			h *= 0xc2b2ae35;
			h ^= h >> 16;
			return h;
		}
	};
}
