export module Pixel;
import CSTDINT;
import Objects;
import Color;

export namespace System::Drawing {
	struct Pixel {
		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
		uint8_t a = 255;
	public:
		constexpr Pixel() noexcept = default;
		constexpr Pixel(uint8_t r, uint8_t g, uint8_t b) noexcept : b(b), g(g), r(r) {}
		constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept : b(b), g(g), r(r), a(a) {}
		constexpr Pixel(uint32_t pixel) noexcept : a(pixel & 0xff), b((pixel >> 8) & 0xff), g((pixel >> 16) & 0xff), r((pixel >> 24) & 0xff) {}
		constexpr Pixel(const Color& color) noexcept : r(color.R), g(color.G), b(color.B), a(color.Reserved) {}
	public:
		constexpr Pixel& AddRGB(const Pixel& rhs) noexcept {
			b += rhs.b;
			g += rhs.g;
			r += rhs.r;
			return *this;
		}
		constexpr Pixel& DivRGB(uint8_t n) noexcept {
			b /= n;
			g /= n;
			r /= n;
			return *this;
		}
		constexpr Pixel AverageRGB(const Pixel& rhs) const noexcept {
			return Pixel((r + rhs.r) / 2, (g + rhs.g) / 2, (b + rhs.b) / 2);
		}
	public:
		/// <summary>
		/// ARGB値をビット反転する
		/// </summary>
		constexpr void Inversion() noexcept { a = ~a; b = ~b; g = ~g; r = ~r; }
		/// <summary>
		/// アルファ値を除くRGB値が等しいか調べる
		/// </summary>
		constexpr bool EqualColor(const Pixel& rhs) const noexcept {
			return r == rhs.r && g == rhs.g && b == rhs.b;
		}
	public:
		Type GetType() const noexcept { return Type::CreateType<Pixel>(); }
		String ToString() const noexcept {
			return String::Joint(u'(', r, u", ", g, u", ", b, u", ", a, u')');
		}
	public:
		constexpr Pixel& operator=(const Pixel&) noexcept = default;
		constexpr Pixel operator+(const Pixel& rhs) const noexcept { return Pixel(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); }
		constexpr Pixel& operator+=(const Pixel& rhs) noexcept {
			a += rhs.a;
			b += rhs.b;
			g += rhs.g;
			r += rhs.r;
			return *this;
		}
		constexpr Pixel& operator|=(const Pixel& rhs) noexcept {
			r |= rhs.r;
			g |= rhs.g;
			b |= rhs.b;
			a |= rhs.a;
			return *this;
		}
		constexpr Pixel operator/(uint8_t n) const noexcept { return Pixel(r / n, b / n, g / n, a / n); }
		constexpr bool operator==(const Pixel&) const noexcept = default;
	};
}

export namespace System::Drawing {
	namespace Pixels {
		constexpr Pixel White = Pixel(255, 255, 255, 255);
		constexpr Pixel Black = Pixel(0, 0, 0, 255);
	}
}