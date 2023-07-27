export module Vector2;
import CSTDINT;
import Math;
import Objects;

export namespace System {
	class Vector2 {
	public:
		union {
			float data[2] = { 0.0f, 0.0f };
			struct {
				float x;
				float y;
			};
			struct {
				float u;
				float v;
			};
		};
	public:
		constexpr Vector2() noexcept = default;
		constexpr Vector2(const Vector2&) noexcept = default;
		constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}
	public:
		static constexpr Vector2 Zero() noexcept { return Vector2(0, 0); }
	public:
		//ベクトルの大きさを返す
		constexpr float Magnitude() const noexcept { return Math::Sqrt(x * x + y * y); }
		//正規化したベクトルを返す
		constexpr Vector2 Normalized() const noexcept {
			float mag = Magnitude();
			return Math::EqualZero(mag) ? Zero() : Vector2(x / mag, y / mag);
		}
		//ベクトルの大きさの2乗を返す
		constexpr float SqrMagnitude() const noexcept { return x * x + y * y; }
		//各成分の逆数をとったベクトルを返す
		constexpr Vector2 Reciprocal() const noexcept { return Vector2(1.0f / x, 1.0f / y); }
	public:
		//*this == otherと等しい
		constexpr bool Equal(const Vector2& other) const noexcept { return *this == other; }
		//各成分を設定する
		constexpr void Set(float newX, float newY) noexcept { x = newX; y = newY; }
		//自身を正規化する
		constexpr void Normalize() noexcept { *this = Normalized(); }
		//各成分をスケーリングする
		constexpr void Scale(const Vector2& scale) noexcept {
			x *= scale.x;
			y *= scale.y;
		}
	public:
		//fromとtoのなす角を返す([°])
		//fromとtoにゼロベクトルが含まれる場合、0を返す
		static constexpr float Angle(const Vector2& from, const Vector2& to) noexcept {
			return from != Zero() && to != Zero() ? Math::AcosDeg(Dot(from, to) / (from.Magnitude() * to.Magnitude())) : 0.0f;
		}
		//vectorと同じ向きで大きさmaxLengthのベクトルを返す
		static constexpr Vector2 ClampMagnitude(const Vector2& vector, float maxLength) noexcept {
			return vector.Normalized() * maxLength;
		}
		//lhsとrhsの外積の大きさを返す
		static constexpr float CrossMagnitude(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x * rhs.y - lhs.y * rhs.x;
		}
		//位置ベクトルa,b間の距離を返す
		static constexpr float Distance(const Vector2& a, const Vector2& b) noexcept {
			return (a - b).Magnitude();
		}
		//lhsとrhsの内積を返す
		static constexpr float Dot(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		//ベクトルa,bを媒介変数t[0, 1]で線形補間した結果を返す
		static constexpr Vector2 Lerp(const Vector2& a, const Vector2& b, float t) noexcept {
			return LerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f));
		}
		//ベクトルa,bを媒介変数t(-Inf, Inf)で線形補間した結果を返す
		static constexpr Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t) noexcept {
			return (1.0f - t) * a + t * b;
		}
		//lhsとrhsの各成分で一番大きい値を使用してベクトルを作成する
		static constexpr Vector2 Max(const Vector2& lhs, const Vector2& rhs) noexcept {
			return Vector2(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y));
		}
		//lhsとrhsの各成分で一番小さい値を使用してベクトルを作成する
		static constexpr Vector2 Min(const Vector2& lhs, const Vector2& rhs) noexcept {
			return Vector2(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y));
		}
		//currentからtargetに向かってmaxDistanceDeltaだけ進んだ時の位置ベクトルを返す
		//ただし、(target - current).magnitude() < maxDistanceDeltaの場合、targetを返す
		static constexpr Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta) noexcept {
			return (target - current).SqrMagnitude() < maxDistanceDelta * maxDistanceDelta
				? target : current + (target - current).Normalized() * maxDistanceDelta;
		}
		//vectorを正規化する
		static constexpr Vector2& Normalize(Vector2& vector) noexcept {
			return vector = vector.Normalized();
		}
		//normalを正規化する
		//tangentをnormalと直交させて正規化する
		static constexpr void OrthoNormalize(Vector2& normal, Vector2& tangent) noexcept {
			normal.Normalize();
			float c = CrossMagnitude(normal, tangent);
			if (c > 0) tangent = Vector2(-normal.y, normal.x);
			else tangent = Vector2(normal.y, -normal.x);
		}
		//vectorをonNormal上に投影したベクトルを返す
		static constexpr Vector2 Project(const Vector2& vector, const Vector2& onNormal) noexcept {
			Vector2 n = onNormal.Normalized();
			return n * Dot(vector, n);
		}
		//方向ベクトルcurrentからtargetまでの回転のうち、
		//maxRadianDelta[rad]だけ回転を適用したcurrentを返す
		//currentがゼロベクトルの場合、targetを返す
		static constexpr Vector2 RotateTowords(const Vector2& current, const Vector2& target, float maxRadianDelta, float /*maxMagnitudeDelta*/) noexcept {
			Vector2 t = CrossMagnitude(current, target) == 0 ? target - Vector2(0.00001f, 0.00001f) : target;
			float theta = Math::Deg2Radf * Angle(current, t);
			float one = (1.0f / theta) * maxRadianDelta;
			if (Math::GreaterEq(one, 1.0f)) return target;
			return ((Math::Sin(theta * (1.0f - one)) * current + Math::Sin(theta * one) * t) / Math::Sin(theta)).Normalized();
		}
		//aとbの各成分が乗算されたベクトルを返す	
		static constexpr Vector2 Scale(const Vector2& a, const Vector2& b) noexcept {
			return Vector2(a.x * b.x, a.y * b.y);
		}
		//fromとtoがなす角に符号をつけて返す([°])
		//回転軸はZ軸正方向である
		static constexpr float SignedAngle(const Vector2& from, const Vector2& to) noexcept {
			return Angle(from, to) * Math::Sign(CrossMagnitude(from, to));
		}
	public:
		String ToString() const noexcept { return String::Joint(u"{ x = ", x, u", y = ", y, u" }"); }
		Type GetType() const noexcept { return Type::CreateType<Vector2>(); }
	public:
		constexpr Vector2 operator+(const Vector2& rhs) const noexcept { return Vector2(x + rhs.x, y + rhs.y); }
		constexpr Vector2 operator-(const Vector2& rhs) const noexcept { return Vector2(x - rhs.x, y - rhs.y); }
		constexpr Vector2 operator-() const noexcept { return Vector2(-x, -y); }
		constexpr Vector2 operator*(float d) const noexcept { return Vector2(x * d, y * d); }
		constexpr Vector2 operator/(float d) const noexcept { return Vector2(x / d, y / d); }
		friend constexpr Vector2 operator*(float d, const Vector2& rhs) noexcept { return Vector2(rhs.x * d, rhs.y * d); }
		constexpr Vector2& operator=(const Vector2&) = default;
		constexpr Vector2& operator=(const float rhs[2]) noexcept {
			x = rhs[0];
			y = rhs[1];
			return *this;
		}
		constexpr Vector2& operator+=(const Vector2& rhs) noexcept { return *this = *this + rhs; }
		constexpr Vector2& operator-=(const Vector2& rhs) noexcept { return *this = *this - rhs; }
		constexpr Vector2& operator*=(float rhs) noexcept { return *this = *this * rhs; }
		constexpr Vector2& operator/=(float rhs) noexcept { return *this = *this / rhs; }
		constexpr float& operator[](uint32_t index) noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else return this->operator[](index % 2);
		}
		constexpr float operator[](uint32_t index) const noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else return this->operator[](index % 2);
		}
		constexpr bool operator==(const Vector2& rhs) const noexcept {
			return Math::EqualZero(Vector2(x - rhs.x, y - rhs.y).SqrMagnitude());
		}
		constexpr bool operator!=(const Vector2& rhs) const noexcept { return !(*this == rhs); }
	};
}