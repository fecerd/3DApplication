export module Vector3;
import CSTDINT;
import Math;
import Objects;

export namespace System {
	class Vector3 {
	public:
		float x = 0;
		float y = 0;
		float z = 0;
	public:
		constexpr Vector3() noexcept = default;
		constexpr Vector3(const Vector3&) noexcept = default;
		constexpr Vector3(float x, float y, float z = 0.0f) noexcept : x(x), y(y), z(z) {}
		~Vector3() noexcept = default;
	public:
		static constexpr Vector3 Back() noexcept { return Vector3(0, 0, -1); }
		static constexpr Vector3 Down() noexcept { return Vector3(0, -1, 0); }
		static constexpr Vector3 Forward() noexcept { return Vector3(0, 0, 1); }
		static constexpr Vector3 Left() noexcept { return Vector3(-1, 0, 0); }
		static constexpr Vector3 NegativeInfinity() noexcept { return Vector3(-Math::INFINITY, -Math::INFINITY, -Math::INFINITY); }
		static constexpr Vector3 One() noexcept { return Vector3(1, 1, 1); }
		static constexpr Vector3 PositiveInfinity() noexcept { return Vector3(Math::INFINITY, Math::INFINITY, Math::INFINITY); }
		static constexpr Vector3 Right() noexcept { return Vector3(1, 0, 0); }
		static constexpr Vector3 Up() noexcept { return Vector3(0, 1, 0); }
		static constexpr Vector3 Zero() noexcept { return Vector3(0, 0, 0); }
	public:
		//ベクトルの大きさを返す
		constexpr float Magnitude() const noexcept { return Math::Sqrt(x * x + y * y + z * z); }
		//正規化したベクトルを返す
		constexpr Vector3 Normalized() const noexcept {
			float mag = Magnitude();
			return Math::EqualZero(mag) ? Zero() : Vector3(x / mag, y / mag, z / mag);
		}
		//ベクトルの大きさの2乗を返す
		constexpr float SqrMagnitude() const noexcept { return x * x + y * y + z * z; }
		//各成分の逆数をとったベクトルを返す
		constexpr Vector3 Reciprocal() const noexcept { return Vector3(1.0f / x, 1.0f / y, 1.0f / z); }
	public:
		//*this == otherと等しい
		constexpr bool Equal(const Vector3& other) const noexcept { return *this == other; }
		//各成分を設定する
		constexpr void Set(float newX, float newY, float newZ) noexcept { x = newX; y = newY; z = newZ; }
		//自身を正規化する
		constexpr void Normalize() noexcept { *this = Normalized(); }
		//各成分をスケーリングする
		constexpr void Scale(const Vector3& scale) noexcept {
			x *= scale.x;
			y *= scale.y;
			z *= scale.z;
		}
	public:
		//絶対値が計算機エプシロン未満の成分の値を0にする
		constexpr Vector3& FloorEpsilon() noexcept {
			if (System::Math::Abs(x) < System::Math::EPSILON<float>) x *= 0;
			if (System::Math::Abs(y) < System::Math::EPSILON<float>) y *= 0;
			if (System::Math::Abs(z) < System::Math::EPSILON<float>) z *= 0;
			return *this;
		}
	public:
		//fromとtoのなす角を返す([°])
		//fromとtoにゼロベクトルが含まれる場合、0を返す
		static constexpr float Angle(const Vector3& from, const Vector3& to) noexcept {
			return from != Zero() && to != Zero() ? Math::AcosDeg(Dot(from, to) / (from.Magnitude() * to.Magnitude())) : 0.0f;
		}
		//vectorと同じ向きで大きさmaxLengthのベクトルを返す
		static constexpr Vector3 ClampMagnitude(const Vector3& vector, const float maxLength) noexcept {
			return vector.Normalized() * maxLength;
		}
		//lhsとrhsの外積を返す
		static constexpr Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept {
			return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
		}
		//位置ベクトルa,b間の距離を返す
		static constexpr float Distance(const Vector3& a, const Vector3& b) noexcept {
			return (a - b).Magnitude();
		}
		//lhsとrhsの内積を返す
		static constexpr float Dot(const Vector3& lhs, const Vector3& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		//ベクトルa,bを媒介変数t[0, 1]で線形補間した結果を返す
		static constexpr Vector3 Lerp(const Vector3& a, const Vector3& b, float t) noexcept {
			return LerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f));
		}
		//ベクトルa,bを媒介変数t(-Inf, Inf)で線形補間した結果を返す
		static constexpr Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t) noexcept {
			return (1.0f - t) * a + t * b;
		}
		//lhsとrhsの各成分で一番大きい値を使用してベクトルを作成する
		static constexpr Vector3 Max(const Vector3& lhs, const Vector3& rhs) noexcept {
			return Vector3(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y), Math::Max(lhs.z, rhs.z));
		}
		//lhsとrhsの各成分で一番小さい値を使用してベクトルを作成する
		static constexpr Vector3 Min(const Vector3& lhs, const Vector3& rhs) noexcept {
			return Vector3(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y), Math::Min(lhs.z, rhs.z));
		}
		//currentからtargetに向かってmaxDistanceDeltaだけ進んだ時の位置ベクトルを返す
		//ただし、(target - current).magnitude() < maxDistanceDeltaの場合、targetを返す
		static constexpr Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta) noexcept {
			return (target - current).SqrMagnitude() < maxDistanceDelta * maxDistanceDelta
				? target : current + (target - current).Normalized() * maxDistanceDelta;
		}
		//vectorを正規化する
		static constexpr Vector3& Normalize(Vector3& vector) noexcept {
			return vector = vector.Normalized();
		}
		//normalを正規化する
		//tangentをnormalと直交させて正規化する
		static constexpr void OrthoNormalize(Vector3& normal, Vector3& tangent) noexcept {
			Vector3 bi;
			OrthoNormalize(normal, tangent, bi);
		}
		//normalを正規化する
		//tangentをnormalと直交させて正規化する
		//binormalには、normalとtangentの両方と直交するベクトルが代入される
		static constexpr void OrthoNormalize(Vector3& normal, Vector3& tangent, Vector3& binormal) noexcept {
			if (normal == Zero()) {
				normal = Right();
				tangent = Up();
				binormal = Forward();
			}
			else {
				normal.Normalize();
				tangent = ProjectOnPlane(tangent, normal);
				if (tangent == Zero()) tangent = Cross(Forward(), normal);
				if (tangent == Zero()) tangent = Cross(Right(), normal);
				tangent.Normalize();
				binormal = Cross(normal, tangent).Normalized();
			}
		}
		//vectorをonNormal上に投影したベクトルを返す
		static constexpr Vector3 Project(const Vector3& vector, const Vector3& onNormal) noexcept {
			const Vector3 n = onNormal.Normalized();
			return n * Dot(vector, n);
		}
		//vectorを法線ベクトルplaneNormalで表される平面に投影したベクトルを返す
		static constexpr Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal) noexcept {
			return vector - Project(vector, planeNormal);
		}
		//inDirectionを法線ベクトルinNormalで表される平面で反射したベクトルを返す
		static constexpr Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal) noexcept {
			return inDirection - 2 * Project(inDirection, inNormal);
		}
		//方向ベクトルcurrentからtargetまでの回転のうち、
		//maxRadianDelta[rad]だけ回転を適用したcurrentを返す
		//currentがゼロベクトルの場合、targetを返す
		static constexpr Vector3 RotateTowords(const Vector3& current, const Vector3& target, float maxRadianDelta, float /*maxMagnitudeDelta*/) noexcept {
			const Vector3 t = Cross(current, target) == Zero() ? target - Vector3(0.00001f, 0.00001f, 0.0f) : target;
			const float theta = Math::Deg2Radf * Angle(current, t);
			const float one = (1.0f / theta) * maxRadianDelta;
			if (Math::GreaterEq(one, 1.0f)) return target;
			return ((Math::Sin(theta * (1.0f - one)) * current + Math::Sin(theta * one) * t) / Math::Sin(theta)).Normalized();
		}
		//aとbの各成分が乗算されたベクトルを返す	
		static constexpr Vector3 Scale(const Vector3& a, const Vector3& b) noexcept {
			return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
		}
		//fromとtoがなす角に符号をつけて返す([°])
		//法線ベクトルCross(from, to)で表される平面に対して、axisが表を向いている場合、
		//法線ベクトルを軸として時計回りが正、反時計回りが負である
		//axisが裏を向いている場合、符号は逆であり、axisが平面上の場合は0を返す
		static constexpr float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis) noexcept {
			return Angle(from, to) * Math::Sign(Dot(Cross(from, to), axis));
		}
		//ベクトルa,bを媒介変数t[0, 1]で球面線形補間した結果を返す
		static constexpr Vector3 Slerp(const Vector3& a, const Vector3& b, float t) noexcept {
			return SlerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f));
		}
		//ベクトルa,bを媒介変数t(-Inf, Inf)で球面線形補間した結果を返す
		static constexpr Vector3 SlerpUnclamped(const Vector3& a, const Vector3& b, float t) noexcept {
			float theta = Angle(a, b) * t;
			Vector3 axis = Cross(a, b);
			Vector3 n = axis.Normalized();
			float x = 0, y = 0, z = 0, w = 1;
			if (n.SqrMagnitude() >= 0.98f) {
				n = n * System::Math::SinDeg(theta / 2.0f);
				x = n.x;
				y = n.y;
				z = n.z;
				w = System::Math::CosDeg(theta / 2.0f);
			}
			float x2, y2, z2, w2, xy, xz, xw, yz, yw, zw;
			x2 = x * x;
			y2 = y * y;
			z2 = z * z;
			w2 = w * w;
			xy = x * y;
			xz = x * z;
			xw = x * w;
			yz = y * z;
			yw = y * w;
			zw = z * w;
			Vector3 ret = Vector3(
				(w2 + x2 - y2 - z2) * a.x + 2.0f * (xy - zw) * a.y + 2.0f * (xz + yw) * a.z,
				2.0f * (xy + zw) * a.x + (w2 - x2 + y2 - z2) * a.y + 2.0f * (yz - xw) * a.z,
				2.0f * (xz - yw) * a.x + 2.0f * (xw + yz) * a.y + (w2 - x2 - y2 + z2) * a.z
			);
			float mag = (b - a).Magnitude();
			float am = a.Magnitude();
			return ret.Normalized() * (am + mag * t);

			float sinTheta = System::Math::SinDeg(theta);
			if (System::Math::EqualZero(sinTheta)) return a + (b - a) * t;
			float Ps = System::Math::SinDeg(theta * (1 - t));
			float Pe = System::Math::SinDeg(theta * t);
			return ((Ps * a + Pe * b) / sinTheta);

			//float theta = SignedAngle(a, b, Up());
			//const float sinth = Math::SinDeg(theta);
			//theta += Math::EqualZero(sinth) ? 0.01f : 0.0f;
			//return (Math::SinDeg(theta * (1.0f - t)) * a + Math::SinDeg(theta * t) * b) / Math::SinDeg(theta);
		}
	public:
		String ToString() const noexcept { return String::Joint(u"{ x = ", x, u", y = ", y, u", z = ", z, u" }"); }
		Type GetType() const noexcept { return Type::CreateType<Vector3>(); }
	public:
		constexpr Vector3 operator+(const Vector3& rhs) const noexcept { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
		constexpr Vector3 operator-(const Vector3& rhs) const noexcept { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
		constexpr Vector3 operator-() const noexcept { return Vector3(-x, -y, -z); }
		constexpr Vector3 operator*(float d) const noexcept { return Vector3(x * d, y * d, z * d); }
		constexpr Vector3 operator/(float d) const noexcept { return Vector3(x / d, y / d, z / d); }
		friend constexpr Vector3 operator*(float d, const Vector3& rhs) noexcept { return Vector3(rhs.x * d, rhs.y * d, rhs.z * d); }
		constexpr Vector3& operator=(const Vector3&) = default;
		constexpr Vector3& operator=(const float rhs[3]) noexcept {
			x = rhs[0];
			y = rhs[1];
			z = rhs[2];
			return *this;
		}
		constexpr Vector3& operator+=(const Vector3& rhs) noexcept { return *this = *this + rhs; }
		constexpr Vector3& operator-=(const Vector3& rhs) noexcept { return *this = *this - rhs; }
		constexpr Vector3& operator*=(float rhs) noexcept { return *this = *this * rhs; }
		constexpr Vector3& operator/=(const float rhs) noexcept { return *this = *this / rhs; }
		constexpr float& operator[](uint32_t index) noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else if (index == 2) return z;
			else return this->operator[](index % 3);
		}
		constexpr float operator[](uint32_t index) const noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else if (index == 2) return z;
			else return this->operator[](index % 3);
		}
		constexpr bool operator==(const Vector3& rhs) const noexcept {
			return Math::EqualZero(Vector3(x - rhs.x, y - rhs.y, z - rhs.z).SqrMagnitude());
		}
		constexpr bool operator!=(const Vector3& rhs) const noexcept { return !(*this == rhs); }
	};
}