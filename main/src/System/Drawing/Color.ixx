export module Color;
import CSTDINT;
//Color
export namespace System::Drawing {
	struct Color {
		uint8_t Reserved = 255;
		uint8_t B = 0;
		uint8_t G = 0;
		uint8_t R = 0;
	public:
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t reserved = 255) noexcept
			: B(blue), G(green), R(red), Reserved(reserved) {}
	public:
		constexpr operator uint32_t() const noexcept { return static_cast<uint32_t>((Reserved << 24) | (B << 16) | (G << 8) | R); }
	};

	namespace Colors {
		inline constexpr Color White = Color(255, 255, 255);
		inline constexpr Color Black = Color(0, 0, 0);
		inline constexpr Color Red = Color(255, 0, 0);
		inline constexpr Color Green = Color(0, 255, 0);
		inline constexpr Color Blue = Color(0, 0, 255);
	}
}
