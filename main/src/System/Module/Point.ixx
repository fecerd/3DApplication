export module Point;
import CSTDINT;
import Traits;
import Objects;

export namespace System {
	/// <summary>
	/// 二次元座標型構造体
	/// </summary>
	template<Traits::Concepts::CArithmetic T>
	struct Point {
		T x = 0;
		T y = 0;
	public:
		constexpr Point() noexcept = default;
		constexpr Point(const Point&) noexcept = default;
		constexpr Point(T x, T y) noexcept : x(x), y(y) {}
		constexpr Point(uint32_t dw) noexcept requires(Traits::Concepts::CIntegral<T> && sizeof(T) >= 2)
			: x(static_cast<T>((dw >> 16) & 0xffff)), y(static_cast<T>(dw & 0xffff)) {}
		constexpr ~Point() noexcept = default;
	public:
		constexpr bool IsEmpty() const noexcept { return x == 0 && y == 0; }
		String ToString() const noexcept {
			return String::Joint(u"{ x = ", x, u", y = ", y, u" }");
		}
	public:
		constexpr bool operator==(const Point<T>&) const noexcept = default;
	};
}