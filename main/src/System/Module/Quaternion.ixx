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
		/// �P�ʉ�]���擾����
		/// </summary>
		/// <returns>�P�ʉ�](x, y, z, w) = (0, 0, 0, 1)��\���l����</returns>
		static constexpr Quaternion Identity() noexcept { return Quaternion(); }
	public:
		/// <summary>
		/// �l������Z���AX���AY���̏��ɉ�]�����p����\���Ă���Ƃ݂Ȃ��A�I�C���[�p(x, y, z)[��]�ɕϊ�����
		/// </summary>
		/// <returns>���̎l�����Ɠ����p���������e���̉�]���Ă���p�x</returns>
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
				//cx == 0.0f(X����]��90��)�ɋ߂��ꍇ�AZ����]��0���Ƃ݂Ȃ�
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
			//�v�Z�덷���Ȃ���
			result.x = Math::Abs(result.x) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.x), 89.9f, 90.1f) ? Math::Sign(result.x) * 90.0f : result.x;
			result.y = Math::Abs(result.y) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.y), 89.9f, 90.1f) ? Math::Sign(result.y) * 90.0f : result.y;
			result.z = Math::Abs(result.z) < 0.1f ? 0.0f : Math::InOpenRange(Math::Abs(result.z), 89.9f, 90.1f) ? Math::Sign(result.z) * 90.0f : result.z;
			return result;
		}
		/// <summary>
		/// ���K�������l�������擾����
		/// (��]��\���l�����͂��łɐ��K������Ă��邽�߁A�����I�Ɏg�p����K�v�͂Ȃ�)
		/// </summary>
		/// <returns>���̎l�����𐳋K�������l����</returns>
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
		/// ��]�̑傫��([��])�Ǝ����擾����
		/// </summary>
		/// <param name="angle">��]�̑傫��������ϐ��ւ̎Q��</param>
		/// <param name="axis">��]�̎�������ϐ��ւ̎Q��</param>
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
		/// �t��]���擾����
		/// </summary>
		/// <returns>���̉�]�̋t��]��\���l����</returns>
		constexpr Quaternion Inverse() const noexcept { return Quaternion(-x, -y, -z, w); }
	public:		
		/// <summary>
		/// �I�C���[�p(x, y, z)[��]����AZ����z�AX����x�AY����y�̏��ɉ�]�����]��ݒ肷��
		/// </summary>
		/// <param name="euler">�ݒ肷��I�C���[�p</param>
		constexpr void EulerAngle(const Vector3& euler) noexcept { *this = Euler(euler); }
		/// <summary>
		/// �I�C���[�p(x, y, z)[��]����AZ����z�AX����x�AY����y�̏��ɉ�]�����]��ݒ肷��
		/// </summary>
		/// <param name="x">X���ɑ΂����]�̊p�x[��]</param>
		/// <param name="y">Y���ɑ΂����]�̊p�x[��]</param>
		/// <param name="z">Z���ɑ΂����]�̊p�x[��]</param>
		constexpr void EulerAngle(float x, float y, float z) noexcept { *this = Euler(x, y, z); }
		/// <summary>
		/// ���g�𐳋K������
		/// (��]��\���l�����͂��łɐ��K������Ă��邽�߁A�����I�Ɏg�p����K�v�͂Ȃ�)
		/// </summary>
		constexpr void Normalize() noexcept { *this = Normalized(); }
		/// <summary>
		/// �l�����̊e������ݒ肵�A���K������
		/// </summary>
		constexpr void Set(float newX, float newY, float newZ, float newW) noexcept {
			w = newW;
			x = newX;
			y = newY;
			z = newZ;
			Normalize();
		}
		/// <summary>
		/// ����x�N�g������A��������̃x�N�g���ւ̉�]��ݒ肷��
		/// </summary>
		/// <param name="fromDirection">�n�_�ƂȂ�x�N�g��</param>
		/// <param name="toDirection">�I�_�ƂȂ�x�N�g��</param>
		constexpr void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection) noexcept { *this = FromToRotation(fromDirection, toDirection); }
		/// <summary>
		/// �O�����Ə�����̃x�N�g������p����ݒ肷��
		/// </summary>
		/// <param name="view">�O�����̃x�N�g��</param>
		/// <param name="up">������̉��x�N�g���B���ۂ̏�����̃x�N�g���ɂ͑������Ƃ��̒l�̊O��(�E�����̃x�N�g��)�Ƃ��̒l�̊O�ς��ݒ肳���B</param>
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
		/// ��̉�]�̊Ԃ̊p�x([��])���擾����
		/// </summary>
		/// <returns>��̉�]�̊Ԃ̊p�x([��])</returns>
		static constexpr float Angle(const Quaternion& a, const Quaternion& b) noexcept { return Math::AcosDeg(Dot(a, b)) * 2.0f; }
		/// <summary>
		/// �p�x([��])�Ɖ�]�������]���擾����
		/// </summary>
		/// <param name="angle">��]�̊p�x([��])</param>
		/// <param name="axis">��]��</param>
		/// <returns>��]��\���l����</returns>
		static constexpr Quaternion AngleAxis(float angle, const Vector3& axis) noexcept {
			Vector3 n = axis.Normalized();
			if (n.SqrMagnitude() < 0.98f) return Quaternion::Identity();
			n = n * Math::SinDeg(angle / 2.0f);
			return Quaternion(n.x, n.y, n.z, Math::CosDeg(angle / 2.0f));
		}
		/// <summary>
		/// �l�������m�̓��ς��v�Z����
		/// </summary>
		/// <returns>���ς̒l</returns>
		static constexpr float Dot(const Quaternion& a, const Quaternion& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
		/// <summary>
		/// �I�C���[�p(x, y, z)[��]����AZ����z�AX����x�AY����y�̏��ɉ�]�����]���擾����
		/// </summary>
		/// <param name="euler">���ꂼ��̎��ŉ�]����p�x[��]</param>
		/// <returns>Z���AX���AY���̏��ŉ�]�����Ƃ��̉�]�Ɠ����̉�]��\���l����</returns>
		static constexpr Quaternion Euler(const Vector3& euler) noexcept {
			return Quaternion(0.0f, Math::SinDeg(euler.y / 2.0f), 0.0f, Math::CosDeg(euler.y / 2.0f))
				* Quaternion(Math::SinDeg(euler.x / 2.0f), 0.0f, 0.0f, Math::CosDeg(euler.x / 2.0f))
				* Quaternion(0.0f, 0.0f, Math::SinDeg(euler.z / 2.0f), Math::CosDeg(euler.z / 2.0f));
		}
		/// <summary>
		/// �I�C���[�p(x, y, z)[��]����AZ����z�AX����x�AY����y�̏��ɉ�]�����]���擾����
		/// </summary>
		/// <param name="x">X���ɑ΂����]�̊p�x[��]</param>
		/// <param name="y">Y���ɑ΂����]�̊p�x[��]</param>
		/// <param name="z">Z���ɑ΂����]�̊p�x[��]</param>
		/// <returns>Z���AX���AY���̏��ŉ�]�����Ƃ��̉�]�Ɠ����̉�]��\���l����</returns>
		static constexpr Quaternion Euler(float x, float y, float z) noexcept { return Euler(Vector3(x, y, z)); }
		/// <summary>
		/// ����x�N�g������A��������̃x�N�g���ւ̉�]���擾����
		/// </summary>
		/// <param name="fromDirection">�n�_�ƂȂ�x�N�g��</param>
		/// <param name="toDirection">�I�_�ƂȂ�x�N�g��</param>
		/// <returns>��]��\���l����</returns>
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
		/// ��̎p����}��ϐ�[0, 1]�ɂ���Đ��`��Ԃ������ʂ��擾����
		/// </summary>
		/// <param name="a">�n�_�ƂȂ�p��</param>
		/// <param name="b">�I�_�ƂȂ�p��</param>
		/// <param name="t">�}��ϐ��B[0, 1]�Ɏ��߂���</param>
		/// <returns>���`��Ԃɂ���ē���ꂽ�p����\���l����</returns>
		static constexpr Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) noexcept { return LerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f)); }
		/// <summary>
		/// ��̎p����}��ϐ�[-Inf, Inf]�ɂ���Đ��`��Ԃ������ʂ��擾����
		/// </summary>
		/// <param name="a">�n�_�ƂȂ�p��</param>
		/// <param name="b">�I�_�ƂȂ�p��</param>
		/// <param name="t">�}��ϐ�</param>
		/// <returns>���`��Ԃɂ���ē���ꂽ�p����\���l����</returns>
		static constexpr Quaternion LerpUnclamped(const Quaternion& a, const Quaternion b, float t) noexcept { 
			return ((1.0f - t) * a + t * b * (Dot(a, b) >= 0.0f ? 1.f : -1.f)).Normalized();
		}
		/// <summary>
		/// �O�����Ə�����̃x�N�g������p�����擾����
		/// </summary>
		/// <param name="forward">�O�����̃x�N�g��</param>
		/// <param name="upwards">������̉��x�N�g���B���ۂ̏�����̃x�N�g���ɂ͑������Ƃ��̒l�̊O��(�E�����̃x�N�g��)�Ƃ��̒l�̊O�ς��ݒ肳���B</param>
		static constexpr Quaternion LookRotation(const Vector3& forward, const Vector3& upwards = Vector3::Up()) noexcept {
			//(0, 0, 1)��forward�Ɍ������]rot�ƁArot��(0, 1, 0)�ɓK�p�����x�N�g����
			//Cross(forward, Cross(upwards, forward))�Ɍ������]�������������ʂ�Ԃ�
			const Quaternion rot = FromToRotation(Vector3::Forward(), forward);
			return FromToRotation(rot * Vector3::Up(), Vector3::Cross(forward, Vector3::Cross(upwards, forward))) * rot;
		}
		/// <summary>
		/// �l�����𐳋K������
		/// (��]��\���l�����͂��łɐ��K������Ă��邽�߁A�����I�Ɏg�p����K�v�͂Ȃ�)
		/// </summary>
		/// <param name="q">���K���������l�����̎Q��</param>
		static constexpr void Normalize(Quaternion& q) noexcept { q.Normalize(); }
		/// <summary>
		/// ��̎p�����Ԃ����]�̂����A�w�肵���p�x[��]�����i�߂��Ƃ��̎p�����擾����
		/// </summary>
		/// <param name="from">�n�_�ƂȂ�p��</param>
		/// <param name="to">�I�_�ƂȂ�p��</param>
		/// <param name="maxDegreeDelta">�i�߂�p�x�B��Ԃ�����]�����̊p�x�ɖ����Ȃ��ꍇ�A�I�_�̎p����Ԃ�</param>
		/// <returns>�p����\���l����</returns>
		static constexpr Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreeDelta) noexcept {
			const float t = maxDegreeDelta / Angle(from, to);
			return Math::GreaterEq(t, 1.0f) ? to : Lerp(from, to, t);
		}
		/// <summary>
		/// ��̎p����}��ϐ�[0, 1]�ɂ���ċ��ʐ��`��Ԃ������ʂ��擾����
		/// </summary>
		/// <param name="a">�n�_�ƂȂ�p��</param>
		/// <param name="b">�I�_�ƂȂ�p��</param>
		/// <param name="t">�}��ϐ��B[0, 1]�Ɏ��߂���</param>
		/// <returns>���ʐ��`��Ԃɂ���ē���ꂽ�p����\���l����</returns>
		static constexpr Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) noexcept { return SlerpUnclamped(a, b, Math::Clamp(t, 0.0f, 1.0f)); }
		/// <summary>
		/// ��̎p����}��ϐ�[-Inf, Inf]�ɂ���ċ��ʐ��`��Ԃ������ʂ��擾����
		/// </summary>
		/// <param name="a">�n�_�ƂȂ�p��</param>
		/// <param name="b">�I�_�ƂȂ�p��</param>
		/// <param name="t">�}��ϐ�</param>
		/// <returns>���ʐ��`��Ԃɂ���ē���ꂽ�p����\���l����</returns>
		static constexpr Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t) noexcept {
			float dot = System::Math::Clamp(Dot(a, b), -1.0f, 1.0f);	//[-1, 1]
			//���ς̐�Βl��1�̂Ƃ��Aa��b�͓�����](�����͈قȂ��Ă��悢)
			if (System::Math::Equal(System::Math::Abs(dot), 1.0f)) {
				return a;
			}
			if (dot < 0) {
				dot = Dot(a, -b);	//[0, 1) (���ς����̂Ƃ��A�ŒZ�����ŕ�Ԃ����)
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
		/// �e�v�f���擾����
		/// </summary>
		/// <param name="index">�e�v�f�ւ̃C���f�b�N�X�B[0, 1, 2, 3] -> [x, y, z, w]</param>
		/// <returns>�e�v�f�̎Q�ƁB�͈͊O�̒l���w�肳�ꂽ�Ƃ��AMod 4�̃C���f�b�N�X�̒l�ւ̎Q�Ƃ��擾����</returns>
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
		/// �l�������m�̃N���X�ς��擾����
		/// </summary>
		/// <param name="rhs">�l����</param>
		/// <returns>*this �~ rhs�̒l</returns>
		constexpr Quaternion operator*(const Quaternion& rhs) const noexcept {
			const Vector3 v1{ x, y, z };
			const Vector3 v2{ rhs.x, rhs.y, rhs.z };
			const Vector3 v = Vector3::Cross(v1, v2) + rhs.w * v1 + w * v2;
			return Quaternion(v.x, v.y, v.z, w * rhs.w - Vector3::Dot(v1, v2));
		}

		constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept { return *this = *this * rhs; }
		/// <summary>
		/// �l�����ɂ���ăx�N�g������]�������ʂ̃x�N�g�����擾����
		/// </summary>
		/// <param name="rhs">��]����x�N�g��</param>
		/// <returns>*this �~ rhs �~ (*this).Inverse()�̒l</returns>
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
		/// �e�v�f��萔�{�����l�������擾����
		/// </summary>
		/// <param name="scalar">�W��</param>
		/// <returns>�萔�{���ꂽ�l����(���K������Ȃ�)</returns>
		constexpr Quaternion operator*(float scalar) const noexcept { return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar); }
		/// <summary>
		/// �e�v�f��萔�{�����l�������擾����
		/// </summary>
		/// <param name="scalar">�W��</param>
		/// <param name="rhs">�萔�{����l����</param>
		/// <returns>�萔�{���ꂽ�l����(���K������Ȃ�)</returns>
		friend constexpr Quaternion operator*(float scalar, const Quaternion& rhs) noexcept { return rhs * scalar; }
		/// <summary>
		/// �e�v�f�����Z�����l�������擾����
		/// </summary>
		/// <param name="scalar">����</param>
		/// <returns>�e�v�f�����Z�����l����(���K������Ȃ�)</returns>
		constexpr Quaternion operator/(float scalar) const noexcept { return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar); }
		Quaternion& operator=(const Quaternion&) = default;
		constexpr bool operator==(const Quaternion & rhs) const noexcept { return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z) && Math::Equal(w, rhs.w); }
		constexpr bool operator!=(const Quaternion & rhs) const noexcept { return !(*this == rhs); }
	};
}