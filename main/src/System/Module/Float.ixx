export module Float;
import CSTDINT;
import Math;

export namespace System {
	struct Float2 {
		union {
			struct {
				float x;
				float y;
			};
			struct {
				float r;
				float g;
			};
		};
	public:
		explicit constexpr Float2(float v) noexcept
			: x(v), y(v) {}
		constexpr Float2(float x, float y) noexcept
			: x(x), y(y) {}
		constexpr Float2(const Float2&) noexcept = default;
	public:
		static constexpr float Dot(const Float2& lhs, const Float2& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		static constexpr Float2 Cross(const Float2& x) noexcept {
			return Float2{ x.y, -x.x };
		}
		static constexpr Float2 Scale(const Float2& lhs, const Float2& rhs) noexcept {
			return Float2{ lhs.x * rhs.x, lhs.y * rhs.y };
		}
	public:
		constexpr float Magnitude() const noexcept {
			return System::Math::Sqrt(SqrMagnitude());
		}
		constexpr float SqrMagnitude() const noexcept {
			return x * x + y * y;
		}
		constexpr Float2 Normalized() const noexcept {
			const float mag = Math::SqrtInv(SqrMagnitude());
			return Float2{
				x * mag, y * mag
			};
		}
		constexpr Float2 Reciprocal() const noexcept {
			return Float2{
				1.f / x, 1.f / y
			};
		}
	public:
		constexpr Float2& Normalize() noexcept {
			return *this = Normalized();
		}
	public:
		constexpr float& operator[](int index) noexcept {
			return index ? y : x;
		}
		constexpr const float& operator[](int index) const noexcept {
			return index ? y : x;
		}
		constexpr Float2 operator+(const Float2& rhs) const noexcept {
			return Float2{ x + rhs.x, y + rhs.y };
		}
		constexpr Float2 operator-(const Float2& rhs) const noexcept {
			return Float2{ x - rhs.x, y - rhs.y };
		}
		constexpr Float2 operator*(float scale) const noexcept {
			return Float2{ x * scale, y * scale };
		}
		constexpr Float2 operator/(float d) const noexcept {
			const float inv = 1.f / d;
			return Float2{ x * inv, y * inv };
		}
		constexpr Float2& operator=(const Float2&) noexcept = default;
		constexpr Float2& operator+=(const Float2& rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		constexpr Float2& operator-=(const Float2& rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		constexpr Float2& operator*=(float scale) noexcept {
			x *= scale;
			y *= scale;
			return *this;
		}
		constexpr Float2& operator/=(float d) noexcept {
			const float inv = 1.f / d;
			x *= inv;
			y *= inv;
			return *this;
		}
	};

	struct Float3 {
		union {
			struct {
				union {
					float x;
					float r;
				};
				union {
					float y;
					float g;
				};
			};
			Float2 xy;
			Float2 rg;
		};
		union {
			float z;
			float b;
		};
	public:
		explicit constexpr Float3(float v) noexcept
			: x(v), y(v), z(v) {}
		constexpr Float3(float x, float y, float z) noexcept
			: x(x), y(y), z(z) {}
		constexpr Float3(const Float2& xy, float z) noexcept
			: x(xy.x), y(xy.y), z(z) {}
		constexpr Float3(const Float3&) noexcept = default;
	public:
		static constexpr float Dot(const Float3& lhs, const Float3& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		static constexpr Float3 Cross(const Float3& lhs, const Float3& rhs) noexcept {
			return Float3{
				lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x
			};
		}
		static constexpr Float3 Scale(const Float3& lhs, const Float3& rhs) noexcept {
			return Float3{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
		}
	public:
		constexpr float Magnitude() const noexcept {
			return System::Math::Sqrt(SqrMagnitude());
		}
		constexpr float SqrMagnitude() const noexcept {
			return x * x + y * y + z * z;
		}
		constexpr Float3 Normalized() const noexcept {
			const float mag = Math::SqrtInv(SqrMagnitude());
			return Float3{
				x * mag, y * mag, z * mag
			};
		}
		constexpr Float3 Reciprocal() const noexcept {
			return Float3{
				1.f / x, 1.f / y, 1.f / z
			};
		}
	public:
		constexpr Float3& Normalize() noexcept {
			return *this = Normalized();
		}
	public:
		constexpr float& operator[](int index) noexcept {
			return index < 2 ? index ? y : x : z;
		}
		constexpr const float& operator[](int index) const noexcept {
			return index < 2 ? index ? y : x : z;
		}
		constexpr Float3 operator+(const Float3& rhs) const noexcept {
			return Float3{ x + rhs.x, y + rhs.y, z + rhs.z };
		}
		constexpr Float3 operator-(const Float3& rhs) const noexcept {
			return Float3{ x - rhs.x, y - rhs.y, z - rhs.z };
		}
		constexpr Float3 operator*(float scale) const noexcept {
			return Float3{ x * scale, y * scale, z * scale };
		}
		constexpr Float3 operator/(float d) const noexcept {
			const float inv = 1.f / d;
			return Float3{ x * inv, y * inv, z * inv };
		}
		constexpr Float3& operator=(const Float3&) noexcept = default;
		constexpr Float3& operator+=(const Float3& rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		constexpr Float3& operator-=(const Float3& rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		constexpr Float3& operator*=(float scale) noexcept {
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}
		constexpr Float3& operator/=(float d) noexcept {
			const float inv = 1.f / d;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}
	};

	struct Float4 {
		// union {
		// 	struct {
		// 		union {
		// 			struct {
		// 				union {
		// 					float x;
		// 					float r;
		// 				};
		// 				union {
		// 					float y;
		// 					float g;
		// 				};
		// 			};
		// 			Float2 xy;
		// 			Float2 rg;
		// 		};
		// 		union ZB {
		// 			float z;
		// 			float b;
		// 		};
		// 	};
		// 	Float3 xyz;
		// 	Float3 rgb;
		// };
		// union {
		// 	float w;
		// 	float a;
		// };
		union{
			struct {
				float x;
				float y;
				float z;
				float w;
			};
			Float3 xyz;
			Float2 xy;
		};
	public:
		explicit constexpr Float4(float v) noexcept
			: x{ v }, y{ v }, z{ v }, w{ v } {}
		constexpr Float4(float x, float y, float z, float w) noexcept
			: x(x), y(y), z(z), w(w) {}
		constexpr Float4(const Float2& xy, float z, float w) noexcept
			: x(xy.x), y(xy.y), z(z), w(w) {}
		constexpr Float4(const Float3& xyz, float w) noexcept
			: x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
		constexpr Float4(const Float4& arg) noexcept
			: x{ arg.x }, y{ arg.y }, z{ arg.z }, w{ arg.w } {}
	public:
		static constexpr float Dot(const Float4& lhs, const Float4& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}
		static constexpr Float4 Cross(const Float4& x, const Float4& y, const Float4& z) noexcept {
			return Float4{
				x[1] * y[2] * z[3] + x[2] * y[3] * z[1] + x[3] * y[1] * z[2] - x[1] * y[3] * z[2] - x[2] * y[1] * z[3] - x[3] * y[2] * z[1],
				-x[2] * y[3] * z[0] - x[3] * y[0] * z[2] - x[0] * y[2] * z[3] + x[2] * y[0] * z[3] + x[3] * y[2] * z[0] + x[0] * y[3] * z[2],
				x[3] * y[0] * z[1] + x[0] * y[1] * z[3] + x[1] * y[3] * z[0] - x[3] * y[1] * z[0] - x[0] * y[3] * z[1] - x[1] * y[0] * z[3],
				-x[0] * y[1] * z[2] - x[1] * y[2] * z[0] - x[2] * y[0] * z[1] + x[0] * y[2] * z[1] + x[1] * y[0] * z[2] + x[2] * y[1] * z[0]
			};
		}
		static constexpr Float4 Scale(const Float4& lhs, const Float4& rhs) noexcept {
			return Float4{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
		}
	public:
		constexpr float Magnitude() const noexcept {
			return System::Math::Sqrt(SqrMagnitude());
		}
		constexpr float SqrMagnitude() const noexcept {
			return x * x + y * y + z * z + w * w;
		}
		constexpr Float4 Normalized() const noexcept {
			const float mag = Math::SqrtInv(SqrMagnitude());
			return Float4{
				x * mag, y * mag, z * mag, w * mag
			};
		}
		constexpr Float4 Reciprocal() const noexcept {
			return Float4{
				1.f / x, 1.f / y, 1.f / z, 1.f / w
			};
		}
	public:
		constexpr Float4& Normalize() noexcept {
			return *this = Normalized();
		}
	public:
		constexpr float& operator[](int index) noexcept {
			return index < 2 ? index ? y : x : index == 2 ? z : w;
		}
		constexpr const float& operator[](int index) const noexcept {
			return index < 2 ? index ? y : x : index == 2 ? z : w;
		}
		constexpr Float4 operator+(const Float4& rhs) const noexcept {
			return Float4{ x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}
		constexpr Float4 operator-(const Float4& rhs) const noexcept {
			return Float4{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
		constexpr Float4 operator*(float scale) const noexcept {
			return Float4{ x * scale, y * scale, z * scale, w * scale };
		}
		constexpr Float4 operator/(float d) const noexcept {
			const float inv = 1.f / d;
			return Float4{ x * inv, y * inv, z * inv, w * inv };
		}
		constexpr Float4& operator=(const Float4&) noexcept = default;
		constexpr Float4& operator+=(const Float4& rhs) noexcept {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
		constexpr Float4& operator-=(const Float4& rhs) noexcept {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}
		constexpr Float4& operator*=(float scale) noexcept {
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
			return *this;
		}
		constexpr Float4& operator/=(float d) noexcept {
			const float inv = 1.f / d;
			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
			return *this;
		}
	};
}
