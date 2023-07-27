export module Quaternion;
import CSTDINT;
import Objects;
import Vector3;
import Matrix;
import Math;

using Vector3 = System::Vector3;

export namespace System {
	class Quaternion {
	public:
		float w = 1.f;
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	public:
		constexpr Quaternion() noexcept = default;
		constexpr Quaternion(const Quaternion&) noexcept = default;
		~Quaternion() noexcept = default;
	public:
		constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
	public:
		/// <summary>
		/// 単位回転を取得する
		/// </summary>
		/// <returns>単位回転(x, y, z, w) = (0, 0, 0, 1)を表す四元数</returns>
		static constexpr Quaternion Identity() noexcept { return Quaternion(); }
	public:
		/// <summary>
		/// 四元数がZ軸、X軸、Y軸の順に回転した姿勢を表しているとみなし、オイラー角(x, y, z)[°]に変換する
		/// </summary>
		/// <returns>この四元数と同じ姿勢を示す各軸の回転している角度</returns>
		constexpr Vector3 EulerAngle() const noexcept {
			Vector3 result;
			const float sx = Math::Clamp(-2.0f * (y * z - x * w), -1.0f, 1.0f);
			const float cx = Math::Sqrt(1.0f - (sx * sx));
			result.x = Math::AcosDeg(cx) * (Math::EqualZero(sx) ? 1.0f : Math::Sign(sx));
			float sy, cy, sz, cz;
			if (Math::InOpenRange(Math::Abs(result.x), 89.9f, 90.1f)) {
				sy = Math::Clamp(2.0f * (x * y - w * z), -1.0f, 1.0f);
				cy = Math::Clamp(2.0f * (y * z + x * w), -1.0f, 1.0f);
				if (result.x < 0.0f) {
					sy *= -1.0f;
					cy *= -1.0f;
				}
				//cx == 0.0f(X軸回転が90°)に近い場合、Z軸回転を0°とみなす
				sz = 0.0f;
				cz = 1.0f;
			}
			else {
				sy = Math::Clamp((2.0f * (x * z + w * y)) / cx, -1.0f, 1.0f);
				cy = Math::Clamp((2.0f * (w * w + z * z) - 1.0f) / cx, -1.0f, 1.0f);
				sz = Math::Clamp((2.0f * (x * y + w * z)) / cx, -1.0f, 1.0f);
				cz = Math::Clamp((2.0f * (w * w + y * y) - 1.0f) / cx, -1.0f, 1.0f);
			}
			result.y = Math::AcosDeg(cy) * (Math::EqualZero(sy) ? 1.0f : Math::Sign(sy));
			result.z = Math::AcosDeg(cz) * (Math::EqualZero(sz) ? 1.0f : Math::Sign(sz));
			//計算誤差をなくす
			result.x = Math::Abs(result.x) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.x), 89.9f, 90.1f) ? Math::Sign(result.x) * 90.0f : result.x;
			result.y = Math::Abs(result.y) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.y), 89.9f, 90.1f) ? Math::Sign(result.y) * 90.0f : result.y;
			result.z = Math::Abs(result.z) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.z), 89.9f, 90.1f) ? Math::Sign(result.z) * 90.0f : result.z;
			return result;
		}
		/// <summary>
		/// 正規化した四元数を取得する
		/// (回転を表す四元数はすでに正規化されているため、明示的に使用する必要はない)
		/// </summary>
		/// <returns>この四元数を正規化した四元数</returns>
		constexpr Quaternion Normalized() const noexcept {
			return *this / System::Math::Sqrt(x * x + y * y + z * z + w * w);
			//Quaternion ret = Quaternion(
			//	System::Math::EqualZero(x) ? 0 : x,
			//	System::Math::EqualZero(y) ? 0 : y,
			//	System::Math::EqualZero(z) ? 0 : z,
			//	System::Math::EqualZero(w) ? 0 : w
			//);
			//return ret / Math::Sqrt(ret.x * ret.x + ret.y * ret.y + ret.z * ret.z + ret.w * ret.w);
		}
		/// <summary>
		/// 回転の大きさ([°])と軸を取得する
		/// </summary>
		/// <param name="angle">回転の大きさが入る変数への参照</param>
		/// <param name="axis">回転の軸が入る変数への参照</param>
		constexpr void ToAngleAxis(float& angle, Vector3& axis) const noexcept {
			const float sin = Math::Sqrt(1.0f - w * w);
			if (Math::EqualZero(sin)) {
				axis = Vector3::Zero();
				angle = 0.0f;
			}
			else {
				axis = Vector3(x / sin, y / sin, z / sin);
				angle = Math::AsinDeg(sin) * 2.0f * (w != 0.0f ? Math::Sign(w) : 1.0f);
			}
		}
		/// <summary>
		/// 逆回転を取得する
		/// </summary>
		/// <returns>この回転の逆回転を表す四元数</returns>
		constexpr Quaternion Inverse() const noexcept { return Quaternion(-x, -y, -z, w); }
	public:		
		/// <summary>
		/// オイラー角(x, y, z)[°]から、Z軸にz、X軸にx、Y軸にyの順に回転する回転を設定する
		/// </summary>
		/// <param name="euler">設定するオイラー角</param>
		constexpr void EulerAngle(const Vector3& euler) noexcept { *this = Euler(euler); }
		/// <summary>
		/// オイラー角(x, y, z)[°]から、Z軸にz、X軸にx、Y軸にyの順に回転する回転を設定する
		/// </summary>
		/// <param name="x">X軸に対する回転の角度[°]</param>
		/// <param name="y">Y軸に対する回転の角度[°]</param>
		/// <param name="z">Z軸に対する回転の角度[°]</param>
		constexpr void EulerAngle(float x, float y, float z) noexcept { *this = Euler(x, y, z); }
		/// <summary>
		/// 自身を正規化する
		/// (回転を表す四元数はすでに正規化されているため、明示的に使用する必要はない)
		/// </summary>
		constexpr void Normalize() noexcept { *this = Normalized(); }
		/// <summary>
		/// 四元数の各成分を設定し、正規化する
		/// </summary>
		constexpr void Set(float newX, float newY, float newZ, float newW) noexcept {
			w = newW;
			x = newX;
			y = newY;
			z = newZ;
			Normalize();
		}
		/// <summary>
		/// あるベクトルから、もう一方のベクトルへの回転を設定する
		/// </summary>
		/// <param name="fromDirection">始点となるベクトル</param>
		/// <param name="toDirection">終点となるベクトル</param>
		constexpr void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection) noexcept { *this = FromToRotation(fromDirection, toDirection); }
		/// <summary>
		/// 前方向と上方向のベクトルから姿勢を設定する
		/// </summary>
		/// <param name="view">前方向のベクトル</param>
		/// <param name="up">上方向の仮ベクトル。実際の上方向のベクトルには第一引数とこの値の外積(右方向のベクトル)とこの値の外積が設定される。</param>
		constexpr void SetLookRotation(const Vector3& view, const Vector3& up = Vector3::Up()) noexcept { *this = LookRotation(view, up); }
	public:
		constexpr Matrix ToRotationMatrix() const noexcept {
			float w2 = w * w;
			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;
			float xy2 = x * y * 2;
			float xz2 = x * z * 2;
			float xw2 = x * w * 2;
			float yz2 = y * z * 2;
			float yw2 = y * w * 2;
			float zw2 = z * w * 2;
			return Matrix{
				w2 + x2 - y2 - z2, xy2 - zw2, xz2 + yw2, 0,
				xy2 + zw2, w2 - x2 + y2 - z2, yz2 - xw2, 0,
				xz2 - yw2, yz2 + xw2, w2 - x2 - y2 + z2, 0,
				0, 0, 0, 1
			};
		}
	public:
		/// <summary>
		/// 二つの回転の間の角度([°])を取得する
		/// </summary>
		/// <returns>二つの回転の間の角度([°])</returns>
		static constexpr float Angle(const Quaternion& a, const Quaternion& b) noexcept { return Math::AcosDeg(Dot(a, b)) * 2.0f; }
		/// <summary>
		/// 角度([°])と回転軸から回転を取得する
		/// </summary>
		/// <param name="angle">回転の角度([°])</param>
		/// <param name="axis">回転軸</param>
		/// <returns>回転を表す四元数</returns>
		static constexpr Quaternion AngleAxis(float angle, const Vector3& axis) noexcept {
			Vector3 n = axis.Normalized();
			if (n.SqrMagnitude() < 0.98f) return Quaternion::Identity();
			n = n * Math::SinDeg(angle / 2.0f);
			return Quaternion(n.x, n.y, n.z, Math::CosDeg(angle / 2.0f));
		}
		/// <summary>
		/// 四元数同士の内積を計算する
		/// </summary>
		/// <returns>内積の値</returns>
		static constexpr float Dot(const Quaternion& a, const Quaternion& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
		/// <summary>
		/// オイラー角(x, y, z)[°]から、Z軸にz、X軸にx、Y軸にyの順に回転する回転を取得する
		/// </summary>
		/// <param name="euler">それぞれの軸で回転する角度[°]</param>
		/// <returns>Z軸、X軸、Y軸の順で回転したときの回転と同等の回転を表す四元数</returns>
		static constexpr Quaternion Euler(const Vector3& euler) noexcept {
			return Quaternion(0.0f, Math::SinDeg(euler.y / 2.0f), 0.0f, Math::CosDeg(euler.y / 2.0f))
				* Quaternion(Math::SinDeg(euler.x / 2.0f), 0.0f, 0.0f, Math::CosDeg(euler.x / 2.0f))
				* Quaternion(0.0f, 0.0f, Math::SinDeg(euler.z / 2.0f), Math::CosDeg(euler.z / 2.0f));
		}
		/// <summary>
		/// オイラー角(x, y, z)[°]から、Z軸にz、X軸にx、Y軸にyの順に回転する回転を取得する
		/// </summary>
		/// <param name="x">X軸に対する回転の角度[°]</param>
		/// <param name="y">Y軸に対する回転の角度[°]</param>
		/// <param name="z">Z軸に対する回転の角度[°]</param>
		/// <returns>Z軸、X軸、Y軸の順で回転したときの回転と同等の回転を表す四元数</returns>
		static constexpr Quaternion Euler(float x, float y, float z) noexcept { return Euler(Vector3(x, y, z)); }
		/// <summary>
		/// あるベクトルから、もう一方のベクトルへの回転を取得する
		/// </summary>
		/// <param name="fromDirection">始点となるベクトル</param>
		/// <param name="toDirection">終点となるベクトル</param>
		/// <returns>回転を表す四元数</returns>
		static constexpr Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) {
			float angle = Vector3::Angle(fromDirection, toDirection);
			Vector3 axis = Vector3::Cross(fromDirection, toDirection);
			if (axis.Equal(Vector3::Zero())) {
				axis = Vector3::Cross(fromDirection, Vector3::Up());
				if (axis.Equal(Vector3::Zero())) axis = Vector3::Cross(fromDirection, Vector3::Right());
			}
			return AngleAxis(angle, axis);
		}
		/// <summary>
		/// 二つの姿勢を媒介変数[0, 1]によって線形補間した結果を取得する
		/// </summary>
		/// <param name="a">始点となる姿勢</param>
		/// <param name="b">終点となる姿勢</param>
		/// <param name="t">媒介変数。[0, 1]に収められる</param>
		/// <returns>線形補間によって得られた姿勢を表す四元数</returns>
		static constexpr Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) noexcept { return LerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f)); }
		/// <summary>
		/// 二つの姿勢を媒介変数[-Inf, Inf]によって線形補間した結果を取得する
		/// </summary>
		/// <param name="a">始点となる姿勢</param>
		/// <param name="b">終点となる姿勢</param>
		/// <param name="t">媒介変数</param>
		/// <returns>線形補間によって得られた姿勢を表す四元数</returns>
		static constexpr Quaternion LerpUnclamped(const Quaternion& a, const Quaternion b, float t) noexcept { 
			return ((1.0f - t) * a + t * b * (Dot(a, b) >= 0.0f ? 1.f : -1.f)).Normalized();
		}
		/// <summary>
		/// 前方向と上方向のベクトルから姿勢を取得する
		/// </summary>
		/// <param name="forward">前方向のベクトル</param>
		/// <param name="upwards">上方向の仮ベクトル。実際の上方向のベクトルには第一引数とこの値の外積(右方向のベクトル)とこの値の外積が設定される。</param>
		static constexpr Quaternion LookRotation(const Vector3& forward, const Vector3& upwards = Vector3::Up()) noexcept {
			//(0, 0, 1)をforwardに向ける回転rotと、rotを(0, 1, 0)に適用したベクトルを
			//Cross(forward, Cross(upwards, forward))に向ける回転を合成した結果を返す
			const Quaternion rot = FromToRotation(Vector3::Forward(), forward);
			return FromToRotation(rot * Vector3::Up(), Vector3::Cross(forward, Vector3::Cross(upwards, forward))) * rot;
		}
		/// <summary>
		/// 四元数を正規化する
		/// (回転を表す四元数はすでに正規化されているため、明示的に使用する必要はない)
		/// </summary>
		/// <param name="q">正規化したい四元数の参照</param>
		static constexpr void Normalize(Quaternion& q) noexcept { q.Normalize(); }
		/// <summary>
		/// 二つの姿勢を補間する回転のうち、指定した角度[°]だけ進めたときの姿勢を取得する
		/// </summary>
		/// <param name="from">始点となる姿勢</param>
		/// <param name="to">終点となる姿勢</param>
		/// <param name="maxDegreeDelta">進める角度。補間される回転がこの角度に満たない場合、終点の姿勢を返す</param>
		/// <returns>姿勢を表す四元数</returns>
		static constexpr Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreeDelta) noexcept {
			const float t = maxDegreeDelta / Angle(from, to);
			return Math::GreaterEq(t, 1.0f) ? to : Lerp(from, to, t);
		}
		/// <summary>
		/// 二つの姿勢を媒介変数[0, 1]によって球面線形補間した結果を取得する
		/// </summary>
		/// <param name="a">始点となる姿勢</param>
		/// <param name="b">終点となる姿勢</param>
		/// <param name="t">媒介変数。[0, 1]に収められる</param>
		/// <returns>球面線形補間によって得られた姿勢を表す四元数</returns>
		static constexpr Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) noexcept { return SlerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f)); }
		/// <summary>
		/// 二つの姿勢を媒介変数[-Inf, Inf]によって球面線形補間した結果を取得する
		/// </summary>
		/// <param name="a">始点となる姿勢</param>
		/// <param name="b">終点となる姿勢</param>
		/// <param name="t">媒介変数</param>
		/// <returns>球面線形補間によって得られた姿勢を表す四元数</returns>
		static constexpr Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t) noexcept {
			float dot = System::Math::Clamp(Dot(a, b), -1.0f, 1.0f);	//[-1, 1]
			//内積の絶対値が1のとき、aとbは同じ回転(正負は異なってもよい)
			if (System::Math::Equal(System::Math::Abs(dot), 1.0f)) {
				return a;
			}
			if (dot < 0) {
				dot = Dot(a, -b);	//[0, 1) (内積が正のとき、最短距離で補間される)
				float theta = System::Math::AcosDeg(dot);	//(0, 90]
				return ((System::Math::SinDeg(theta * (1.0f - t)) * a + System::Math::SinDeg(theta * t) * -b) / System::Math::SinDeg(theta)).Normalized();
			}
			else {
				float theta = System::Math::AcosDeg(dot);	//(0, 90]
				return ((System::Math::SinDeg(theta * (1.0f - t)) * a + System::Math::SinDeg(theta * t) * b) / System::Math::SinDeg(theta)).Normalized();
			}
		}
	public:
		static constexpr Quaternion FromAxes(Vector3 right, Vector3 up, Vector3 forward) noexcept {
			if (System::Math::Abs(right.x) < 1e-6f) right.x = 0;
			if (System::Math::Abs(right.y) < 1e-6f) right.y = 0;
			if (System::Math::Abs(right.z) < 1e-6f) right.z = 0;
			if (System::Math::Abs(up.x) < 1e-6f) up.x = 0;
			if (System::Math::Abs(up.y) < 1e-6f) up.y = 0;
			if (System::Math::Abs(up.z) < 1e-6f) up.z = 0;
			if (System::Math::Abs(forward.x) < 1e-6f) forward.x = 0;
			if (System::Math::Abs(forward.y) < 1e-6f) forward.y = 0;
			if (System::Math::Abs(forward.z) < 1e-6f) forward.z = 0;

			float rx_uy = right.x - up.y;	//2(x + y)(x - y), |x|==|y|
			float uy_fz = up.y - forward.z;	//2(y + z)(y - z), |y|==|z|
			float rx_fz = right.x - forward.z;	//2(z + x)(z - x), |x|==|z|
			float uz_fy = up.z - forward.y;	//4xw
			float fx_rz = forward.x - right.z;	//4yw
			float ry_ux = right.y - up.x;	//4zw

			float w2 = rx_uy != 0 ? (uz_fy + fx_rz) * (uz_fy - fx_rz) / (8 * rx_uy)
				: uy_fz != 0 ? (fx_rz + ry_ux) * (fx_rz - ry_ux) / (8 * uy_fz)
				: rx_fz != 0 ? (uz_fy + fx_rz) * (uz_fy - fx_rz) / (8 * rx_fz)
				: System::FLOAT_NAN;
			float w = System::Math::Sqrt(w2);
			if (System::Math::EqualZero(w)) w = 0;
			if (!System::Math::IsNan(w) && w != 0) {
				float w_4 = w * 4;
				return Quaternion(uz_fy / w_4, fx_rz / w_4, ry_ux / w_4, w).Normalized();
			}

			float uyfz = up.y + forward.z;	//2(w + x)(w - x), |x|==|w|
			float rxfz = right.x + forward.z;	//2(w + y)(w - y), |w|==|y|
			float rxuy = right.x + up.y;	//2(w + z)(w - z), |w|==|z|
			float ryux = right.y + up.x;	//4xy
			float uzfy = up.z + forward.y;	//4yz

			float y2 = rx_fz != 0 ? (uzfy + ryux) * (uzfy - ryux) / (8 * rx_fz)
				: uyfz != 0 ? (fx_rz + ryux) * (fx_rz - ryux) / (8 * uyfz)
				: rxuy != 0 ? (fx_rz + uzfy) * (fx_rz - uzfy) / (8 * rxuy)
				: System::FLOAT_NAN;
			float y = System::Math::Sqrt(y2);
			if (System::Math::EqualZero(y)) y = 0;
			if (!System::Math::IsNan(y) && y != 0) {
				float y_4 = y * 4;
				return Quaternion(ryux / y_4, y, uzfy / y_4, fx_rz / y_4).Normalized();
			}

			float fxrz = forward.x + right.z;	//4xz

			float z2 = rx_uy != 0 ? (fxrz + uzfy) * (fxrz - uzfy) / (8 * rx_uy)
				: uyfz != 0 ? (fxrz + ry_ux) * (fxrz - ry_ux) / (8 * uyfz)
				: rxfz != 0 ? (ry_ux + uzfy) * (ry_ux - uzfy) / (8 * rxfz)
				: System::FLOAT_NAN;
			float z = System::Math::Sqrt(z2);
			if (System::Math::EqualZero(z)) z = 0;
			if (!System::Math::IsNan(z) && z != 0) {
				float z_4 = z * 4;
				return Quaternion(fxrz / z_4, uzfy / z_4, z, ry_ux / z_4).Normalized();
			}

			float x2 = uy_fz != 0 ? (ryux + fxrz) * (ryux - fxrz) / (8 * uy_fz)
				: rxfz != 0 ? (uz_fy + ryux) * (uz_fy - ryux) / (8 * rxfz)
				: rxuy != 0 ? (uz_fy + fxrz) * (uz_fy - fxrz) / (8 * rxuy)
				: System::FLOAT_NAN;
			float x = System::Math::Sqrt(x2);
			if (System::Math::EqualZero(x)) x = 0;
			if (!System::Math::IsNan(x) && x != 0) {
				float x_4 = x * 4;
				return Quaternion(x, ryux / x_4, fxrz / x_4, uz_fy / x_4).Normalized();
			}
			return Quaternion(
				x == 0 ? 0.0f : 1.0f,
				y == 0 ? 0.0f : 1.0f,
				z == 0 ? 0.0f : 1.0f,
				w == 0 ? 0.0f : 1.0f
			).Normalized();
		}
	public:
		String ToString() const noexcept { return String::Joint(u"{ x = ", x, u", y = ", y, u", z = ", z, u", w = ", w, u" }"); }
		Type GetType() const noexcept { return Type::CreateType<Quaternion>(); }
	public:
		/// <summary>
		/// 各要素を取得する
		/// </summary>
		/// <param name="index">各要素へのインデックス。[0, 1, 2, 3] -> [x, y, z, w]</param>
		/// <returns>各要素の参照。範囲外の値が指定されたとき、Mod 4のインデックスの値への参照を取得する</returns>
		constexpr float& operator[](uint32_t index) noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else if (index == 2) return z;
			else if (index == 3) return w;
			else return this->operator[](index % 4);
		}
		constexpr float operator[](uint32_t index) const noexcept {
			if (index == 0) return x;
			else if (index == 1) return y;
			else if (index == 2) return z;
			else if (index == 3) return w;
			else return this->operator[](index % 4);
		}
		constexpr Quaternion operator+(const Quaternion& rhs) const noexcept { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		constexpr Quaternion operator-(const Quaternion& rhs) const noexcept { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		constexpr Quaternion operator-() const noexcept { return Quaternion(-x, -y, -z, -w); }
		/// <summary>
		/// 四元数同士のクロス積を取得する
		/// </summary>
		/// <param name="rhs">四元数</param>
		/// <returns>*this × rhsの値</returns>
		constexpr Quaternion operator*(const Quaternion& rhs) const noexcept {
			const Vector3 v1{ x, y, z };
			const Vector3 v2{ rhs.x, rhs.y, rhs.z };
			const Vector3 v = Vector3::Cross(v1, v2) + rhs.w * v1 + w * v2;
			return Quaternion(v.x, v.y, v.z, w * rhs.w - Vector3::Dot(v1, v2));
		}

		constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept { return *this = *this * rhs; }
		/// <summary>
		/// 四元数によってベクトルを回転した結果のベクトルを取得する
		/// </summary>
		/// <param name="rhs">回転するベクトル</param>
		/// <returns>*this × rhs × (*this).Inverse()の値</returns>
		constexpr Vector3 operator*(const Vector3& rhs) const noexcept {
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
			return Vector3(
				(w2 + x2 - y2 - z2) * rhs.x + 2.0f * (xy - zw) * rhs.y + 2.0f * (xz + yw) * rhs.z,
				2.0f * (xy + zw) * rhs.x + (w2 - x2 + y2 - z2) * rhs.y + 2.0f * (yz - xw) * rhs.z,
				2.0f * (xz - yw) * rhs.x + 2.0f * (xw + yz) * rhs.y + (w2 - x2 - y2 + z2) * rhs.z
			);
		}
		friend constexpr Vector3 operator*(const Vector3& lhs, const Quaternion& rhs) noexcept {
			float x2, y2, z2, w2, xy, xz, xw, yz, yw, zw;
			x2 = rhs.x * rhs.x;
			y2 = rhs.y * rhs.y;
			z2 = rhs.z * rhs.z;
			w2 = rhs.w * rhs.w;
			xy = rhs.x * rhs.y;
			xz = rhs.x * rhs.z;
			xw = rhs.x * rhs.w;
			yz = rhs.y * rhs.z;
			yw = rhs.y * rhs.w;
			zw = rhs.z * rhs.w;
			return Vector3(
				(w2 + x2 - y2 - z2) * lhs.x + 2.0f * (xy - zw) * lhs.y + 2.0f * (xz + yw) * lhs.z,
				2.0f * (xy + zw) * lhs.x + (w2 - x2 + y2 - z2) * lhs.y + 2.0f * (yz - xw) * lhs.z,
				2.0f * (xz - yw) * lhs.x + 2.0f * (xw + yz) * lhs.y + (w2 - x2 - y2 + z2) * lhs.z
			);
		}
		/// <summary>
		/// 各要素を定数倍した四元数を取得する
		/// </summary>
		/// <param name="scalar">係数</param>
		/// <returns>定数倍された四元数(正規化されない)</returns>
		constexpr Quaternion operator*(float scalar) const noexcept { return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar); }
		/// <summary>
		/// 各要素を定数倍した四元数を取得する
		/// </summary>
		/// <param name="scalar">係数</param>
		/// <param name="rhs">定数倍する四元数</param>
		/// <returns>定数倍された四元数(正規化されない)</returns>
		friend constexpr Quaternion operator*(float scalar, const Quaternion& rhs) noexcept { return rhs * scalar; }
		/// <summary>
		/// 各要素を除算した四元数を取得する
		/// </summary>
		/// <param name="scalar">除数</param>
		/// <returns>各要素を除算した四元数(正規化されない)</returns>
		constexpr Quaternion operator/(float scalar) const noexcept { return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar); }
		Quaternion& operator=(const Quaternion&) = default;
		constexpr bool operator==(const Quaternion & rhs) const noexcept { return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z) && Math::Equal(w, rhs.w); }
		constexpr bool operator!=(const Quaternion & rhs) const noexcept { return !(*this == rhs); }
	};
}