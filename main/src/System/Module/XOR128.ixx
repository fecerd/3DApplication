export module XOR128;
import CSTDINT;

//XOR128
export namespace System {
	class XOR128 {
		uint32_t x = 123456789;
		uint32_t y = 362436069;
		uint32_t z = 521288629;
		uint32_t w = 88675123;
	public:
		constexpr XOR128() noexcept = default;
		constexpr XOR128(uint32_t x, uint32_t y, uint32_t z, uint32_t w) noexcept : x(x), y(y), z(z), w(w) {}
	public:
		constexpr uint32_t Get() noexcept {
			uint32_t t = (x ^ (x << 11));
			x = y;
			y = z;
			z = w;
			return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
		}
		constexpr uint32_t Get(uint32_t range) noexcept { return Get() % range; }
	};
}