export module DualQuaternion;
import Quaternion;
import Vector3;
import Math;
using Quaternion = System::Quaternion;
using Vector3 = System::Vector3;

export namespace System {
	/// <summary>
	/// �o�l������\���N���X
	/// Q = (w + iv1) + ��(w + iv2) (i^2 = -1, ��^2 = 0)
	/// </summary>
	class DualQuaternion {
	public:
		Quaternion q0;
		Quaternion q1;
	public:
		constexpr DualQuaternion() noexcept : q0(0, 0, 0, 1), q1(0, 0, 0, 0) {}
		constexpr DualQuaternion(float q0x, float q0y, float q0z, float q0w, float q1x, float q1y, float q1z, float q1w) noexcept
			: q0(q0x, q0y, q0z, q0w), q1(q1x, q1y, q1z, q1w) {}
		explicit constexpr DualQuaternion(const Vector3& translate) noexcept : q0(0, 0, 0, 1), q1(translate.x / 2, translate.y / 2, translate.z / 2, 0) {}
		explicit constexpr DualQuaternion(const Quaternion& rot) noexcept : q0(rot), q1(0, 0, 0, 0) {}
	public:
		/// <summary>
		/// ���W��o�l�����ŕ\���������̂��擾����
		/// </summary>
		/// <param name="position">���W</param>
		static constexpr DualQuaternion Position(const Vector3& position) noexcept {
			return DualQuaternion(0, 0, 0, 1, position.x, position.y, position.z, 0);
		}
		/// <summary>
		/// ���s�ړ���o�l�����ŕ\���������̂��擾����
		/// </summary>
		/// <param name="translate">�ړ���</param>
		static constexpr DualQuaternion Translate(const Vector3& translate) noexcept {
			return DualQuaternion(translate);
		}
		/// <summary>
		/// ��]��o�l�����ŕ\���������̂��擾����
		/// </summary>
		/// <param name="rotate">��]</param>
		static constexpr DualQuaternion Rotate(const Quaternion& rotate) noexcept {
			return DualQuaternion(rotate);
		}
		/// <summary>
		/// �w�肵�����W���猴�_�ɕ��s�ړ�������A�w�肵����]���s���A���_����w�肵�����W�֕��s�ړ�����ό`���擾����
		/// </summary>
		/// <param name="rotate">��]</param>
		/// <param name="pos">���W</param>
		static constexpr DualQuaternion RotateOrigin(const Quaternion& rotate, const Vector3 pos) noexcept {
			Vector3 tmp = Vector3::Cross(Vector3(pos.x / 2, pos.y / 2, pos.z / 2), Vector3(rotate.x, rotate.y, rotate.z)) * 2;
			return DualQuaternion(rotate.x, rotate.y, rotate.z, rotate.w, tmp.x, tmp.y, tmp.z, 0);
		}
		/// <summary>
		/// �����̑o�l���� Q' = (w - iv1) + ��(-w + iv2)���擾����
		/// </summary>
		constexpr DualQuaternion Conjugate() const noexcept {
			DualQuaternion ret = *this;
			ret.q0.x *= -1;
			ret.q0.y *= -1;
			ret.q0.z *= -1;
			ret.q1.w *= -1;
			return ret;
		}
		/// <summary>
		/// ���̎l�������\���ϊ��̋t���擾����B
		/// *this * Inverse() == DualQuaternion()�ƂȂ�
		/// </summary>
		constexpr DualQuaternion Inverse() const noexcept {
			return DualQuaternion(-q0.x, -q0.y, -q0.z, q0.w, -q1.x, -q1.y, -q1.z, q1.w);
		}
		/// <summary>
		/// ���W��o�l�����ŕϊ�����B
		/// �����ȑo�l������Q'�Ƃ��āApos' = Q' * pos * Q���v�Z����
		/// </summary>
		/// <param name="pos">�ϊ�������W</param>
		constexpr Vector3 Multiple(const Vector3& pos) const noexcept {
			return *this * pos;
		}
	public:
		/// <summary>
		/// ��Βl���v�Z�@�G�v�V���������̐����̒l��0�ɂ���
		/// </summary>
		constexpr DualQuaternion& FloorEpsilon() noexcept {
			if (System::Math::Abs(q0.w) < System::Math::EPSILON<float>) q0.w *= 0;
			if (System::Math::Abs(q0.x) < System::Math::EPSILON<float>) q0.x *= 0;
			if (System::Math::Abs(q0.y) < System::Math::EPSILON<float>) q0.y *= 0;
			if (System::Math::Abs(q0.z) < System::Math::EPSILON<float>) q0.z *= 0;
			if (System::Math::Abs(q1.w) < System::Math::EPSILON<float>) q1.w *= 0;
			if (System::Math::Abs(q1.x) < System::Math::EPSILON<float>) q1.x *= 0;
			if (System::Math::Abs(q1.y) < System::Math::EPSILON<float>) q1.y *= 0;
			if (System::Math::Abs(q1.z) < System::Math::EPSILON<float>) q1.z *= 0;
			return *this;
		}
	public:
		/// <summary>
		/// ���W��o�l�����ŕϊ�����B
		/// �����ȑo�l������Q'�Ƃ��āApos' = Q' * pos * Q���v�Z����
		/// </summary>
		/// <param name="pos">�ϊ�������W</param>
		constexpr Vector3 operator*(const Vector3& rhs) const noexcept {
			float x2, y2, z2, w2, xy, xz, xw, yz, yw, zw;
			x2 = q0.x * q0.x;
			y2 = q0.y * q0.y;
			z2 = q0.z * q0.z;
			w2 = q0.w * q0.w;
			xy = q0.x * q0.y;
			xz = q0.x * q0.z;
			xw = q0.x * q0.w;
			yz = q0.y * q0.z;
			yw = q0.y * q0.w;
			zw = q0.z * q0.w;
			return Vector3(
				(w2 + x2 - y2 - z2) * rhs.x + 2.0f * (xy - zw) * rhs.y + 2.0f * (xz + yw) * rhs.z + 2.0f * (q0.w * q1.x - q0.x * q1.w + q0.y * q1.z - q0.z * q1.y),
				2.0f * (xy + zw) * rhs.x + (w2 - x2 + y2 - z2) * rhs.y + 2.0f * (yz - xw) * rhs.z + 2.0f * (q0.w * q1.y - q0.x * q1.z - q0.y * q1.w + q0.z * q1.x),
				2.0f * (xz - yw) * rhs.x + 2.0f * (xw + yz) * rhs.y + (w2 - x2 - y2 + z2) * rhs.z + 2.0f * (q0.w * q1.z + q0.x * q1.y - q0.y * q1.x - q0.z * q1.w)
			).FloorEpsilon();
		}
		/// <summary>
		/// ���W��o�l�����ŕϊ�����B
		/// �����ȑo�l������Q'�Ƃ��āApos' = Q' * pos * Q���v�Z����
		/// </summary>
		/// <param name="pos">�ϊ�������W</param>
		friend constexpr Vector3 operator*(const Vector3& lhs, const DualQuaternion& rhs) noexcept {
			return rhs * lhs;
		}
		/// <summary>
		/// �o�l�������m����Z����B�ό`���͍��Ӂ��E��
		/// </summary>
		constexpr DualQuaternion operator*(const DualQuaternion& rhs) const noexcept {
			DualQuaternion ret;
			ret.q0.w = q0.w * rhs.q0.w - (q0.x * rhs.q0.x + q0.y * rhs.q0.y + q0.z * rhs.q0.z);
			ret.q0.x = q0.w * rhs.q0.x + rhs.q0.w * q0.x - (q0.y * rhs.q0.z - q0.z * rhs.q0.y);
			ret.q0.y = q0.w * rhs.q0.y + rhs.q0.w * q0.y - (q0.z * rhs.q0.x - q0.x * rhs.q0.z);
			ret.q0.z = q0.w * rhs.q0.z + rhs.q0.w * q0.z - (q0.x * rhs.q0.y - q0.y * rhs.q0.x);
			ret.q1.w = q0.w * rhs.q1.w + q1.w * rhs.q0.w - (q0.x * rhs.q1.x + q0.y * rhs.q1.y + q0.z * rhs.q1.z) - (q1.x * rhs.q0.x + q1.y * rhs.q0.y + q1.z * rhs.q0.z);
			ret.q1.x = q0.w * rhs.q1.x + rhs.q1.w * q0.x - (q0.y * rhs.q1.z - q0.z * rhs.q1.y) + q1.w * rhs.q0.x + rhs.q0.w * q1.x - (q1.y * rhs.q0.z - q1.z * rhs.q0.y);
			ret.q1.y = q0.w * rhs.q1.y + rhs.q1.w * q0.y - (q0.z * rhs.q1.x - q0.x * rhs.q1.z) + q1.w * rhs.q0.y + rhs.q0.w * q1.y - (q1.z * rhs.q0.x - q1.x * rhs.q0.z);
			ret.q1.z = q0.w * rhs.q1.z + rhs.q1.w * q0.z - (q0.x * rhs.q1.y - q0.y * rhs.q1.x) + q1.w * rhs.q0.z + rhs.q0.w * q1.z - (q1.x * rhs.q0.y - q1.y * rhs.q0.x);
			return ret.FloorEpsilon();
		}
		/// <summary>
		/// �o�l�������m����Z����B�ό`���͍��Ӂ��E��
		/// </summary>
		constexpr DualQuaternion& operator*=(const DualQuaternion& rhs) noexcept { return *this = *this * rhs; }
		constexpr DualQuaternion& operator=(const DualQuaternion&) noexcept = default;
		constexpr bool operator==(const DualQuaternion&) const noexcept = default;
		/// <summary>
		/// �o�l���������W��\���Ă���Ƃ݂Ȃ��āA���W���擾����
		/// </summary>
		explicit constexpr operator System::Vector3() const noexcept { return System::Vector3(q1.x, q1.y, q1.z); }
	};
}