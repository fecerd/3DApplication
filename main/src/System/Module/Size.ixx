export module Size;
import CSTDINT;
import Traits;
import Objects;

export namespace System {
	template<Traits::Concepts::CArithmetic T>
	struct Size {
		T width = 0;
		T height = 0;
	public:
		constexpr Size() noexcept = default;
		constexpr Size(const Size&) noexcept = default;
		constexpr Size(T w, T h) noexcept : width(w), height(h) {}
		constexpr Size(uint32_t dw) noexcept
			: width(static_cast<T>((dw >> 16) & 0xffff)), height(static_cast<T>(dw & 0xffff)) {}
		constexpr ~Size() noexcept = default;
	public:
		constexpr bool IsEmpty() const noexcept { return width == 0 && height == 0; }
		String ToString() const noexcept {
			return String::Joint(u"{ width = ", width, u", height = ", height, u" }");
		}
	public:
		constexpr bool operator==(const Size<T>&) const noexcept = default;
	};
}