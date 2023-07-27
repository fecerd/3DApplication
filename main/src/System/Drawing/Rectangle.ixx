export module Rectangle;
import CSTDINT;

export namespace System::Drawing {
	struct Rectangle {
		int32_t x = 0;
		int32_t y = 0;
		int32_t width = 0;
		int32_t height = 0;
	public:
		constexpr Rectangle() noexcept = default;
		constexpr Rectangle(int32_t x, int32_t y, int32_t width, int32_t height) noexcept 
			: x(x), y(y), width(width), height(height) {}
	public:
		constexpr int32_t Left() const noexcept { return x; }
		constexpr int32_t Top() const noexcept { return y; }
		constexpr int32_t Right() const noexcept { return x + width; }
		constexpr int32_t Bottom() const noexcept { return y + height; }
	public:
		constexpr bool IsEmpty() const noexcept { return x == 0 && y == 0 && width == 0 && height == 0; }
	};
}