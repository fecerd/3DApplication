export module Matrix;
import CSTDINT;
import Vector3;
import Math;

export namespace System {
	/// <summary>
	/// 4�s4��s���\���N���X�B
	/// float[16]�̓����l���D��Ƃ��Ĉ����B
	/// m�sn��̗v�f��mn�A4�����x�N�g���̊e�v�f��(x, y, z, w)�Ƃ���ƁA
	/// | 00, 01, 02, 03 | | x  |
	/// | 10, 11, 12, 13 | | y  |
	/// | 20, 21, 22, 23 | | z  |
	/// | 30, 31, 32, 33 | | w |
	/// �̂悤�ɏ�Z����B
	/// �����l��
	/// { 00, 10, 20, 30, 01, 11, 21, 31, 02, 12, 22, 32, 03, 13, 23, 33 }
	/// �̂悤�ɕ���ł���B
	/// </summary>
	class Matrix {
	public:
		float m_matrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	public:
		constexpr Matrix() noexcept = default;
		constexpr Matrix(const Matrix&) noexcept = default;
		/// <summary>
		/// �R���X�g���N�^�Ăяo��
		/// Matrix{
		///	00, 01, 02, 03,
		///	10, 11, 12, 13,
		///	20, 21, 22, 23,
		///	30, 31, 32, 33
		/// };
		/// �����̂܂܍s��Ƃ݂Ȃ��ď���������B
		/// �����̏�����Matrix�N���X�̓������������C�A�E�g�͈�v���Ȃ��̂Œ���
		/// </summary>
		constexpr Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept
			: m_matrix{ m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33 } {}
	public:
		/// <summary>
		/// �P�ʍs����擾����B�f�t�H���g�R���X�g���N�^�Ăяo���Ɠ����ł���
		/// </summary>
		static constexpr Matrix Identity() noexcept { return Matrix(); }
	public:
		/// <summary>
		/// �]�u�s����擾����
		/// </summary>
		constexpr Matrix Transpose() const noexcept {
			return Matrix{
				m_matrix[0], m_matrix[4], m_matrix[8], m_matrix[12],
				m_matrix[1], m_matrix[5], m_matrix[9], m_matrix[13],
				m_matrix[2], m_matrix[6], m_matrix[10], m_matrix[14],
				m_matrix[3], m_matrix[7], m_matrix[11], m_matrix[15]
			};
		}
	public:
		/// <summary>
		/// �s�񓯎m�̏�Z���v�Z����
		/// </summary>
		/// <param name="rhs">���̍s��ɉE�����Z����s��</param>
		constexpr Matrix Mul(const Matrix& rhs) const noexcept {
			Matrix ret;
			ret[0] = m_matrix[0] * rhs.m_matrix[0] + m_matrix[4] * rhs.m_matrix[1] + m_matrix[8] * rhs.m_matrix[2] + m_matrix[12] * rhs.m_matrix[3];
			ret[1] = m_matrix[1] * rhs.m_matrix[0] + m_matrix[5] * rhs.m_matrix[1] + m_matrix[9] * rhs.m_matrix[2] + m_matrix[13] * rhs.m_matrix[3];
			ret[2] = m_matrix[2] * rhs.m_matrix[0] + m_matrix[6] * rhs.m_matrix[1] + m_matrix[10] * rhs.m_matrix[2] + m_matrix[14] * rhs.m_matrix[3];
			ret[3] = m_matrix[3] * rhs.m_matrix[0] + m_matrix[7] * rhs.m_matrix[1] + m_matrix[11] * rhs.m_matrix[2] + m_matrix[15] * rhs.m_matrix[3];
			ret[4] = m_matrix[0] * rhs.m_matrix[4] + m_matrix[4] * rhs.m_matrix[5] + m_matrix[8] * rhs.m_matrix[6] + m_matrix[12] * rhs.m_matrix[7];
			ret[5] = m_matrix[1] * rhs.m_matrix[4] + m_matrix[5] * rhs.m_matrix[5] + m_matrix[9] * rhs.m_matrix[6] + m_matrix[13] * rhs.m_matrix[7];
			ret[6] = m_matrix[2] * rhs.m_matrix[4] + m_matrix[6] * rhs.m_matrix[5] + m_matrix[10] * rhs.m_matrix[6] + m_matrix[14] * rhs.m_matrix[7];
			ret[7] = m_matrix[3] * rhs.m_matrix[4] + m_matrix[7] * rhs.m_matrix[5] + m_matrix[11] * rhs.m_matrix[6] + m_matrix[15] * rhs.m_matrix[7];
			ret[8] = m_matrix[0] * rhs.m_matrix[8] + m_matrix[4] * rhs.m_matrix[9] + m_matrix[8] * rhs.m_matrix[10] + m_matrix[12] * rhs.m_matrix[11];
			ret[9] = m_matrix[1] * rhs.m_matrix[8] + m_matrix[5] * rhs.m_matrix[9] + m_matrix[9] * rhs.m_matrix[10] + m_matrix[13] * rhs.m_matrix[11];
			ret[10] = m_matrix[2] * rhs.m_matrix[8] + m_matrix[6] * rhs.m_matrix[9] + m_matrix[10] * rhs.m_matrix[10] + m_matrix[14] * rhs.m_matrix[11];
			ret[11] = m_matrix[3] * rhs.m_matrix[8] + m_matrix[7] * rhs.m_matrix[9] + m_matrix[11] * rhs.m_matrix[10] + m_matrix[15] * rhs.m_matrix[11];
			ret[12] = m_matrix[0] * rhs.m_matrix[12] + m_matrix[4] * rhs.m_matrix[13] + m_matrix[8] * rhs.m_matrix[14] + m_matrix[12] * rhs.m_matrix[15];
			ret[13] = m_matrix[1] * rhs.m_matrix[12] + m_matrix[5] * rhs.m_matrix[13] + m_matrix[9] * rhs.m_matrix[14] + m_matrix[13] * rhs.m_matrix[15];
			ret[14] = m_matrix[2] * rhs.m_matrix[12] + m_matrix[6] * rhs.m_matrix[13] + m_matrix[10] * rhs.m_matrix[14] + m_matrix[14] * rhs.m_matrix[15];
			ret[15] = m_matrix[3] * rhs.m_matrix[12] + m_matrix[7] * rhs.m_matrix[13] + m_matrix[11] * rhs.m_matrix[14] + m_matrix[15] * rhs.m_matrix[15];
			return ret;
		}
		/// <summary>
		/// ���̍s����O�����x�N�g���̍������Z����B
		/// �x�N�g����w������ǉ�����4�s1��̎l�����x�N�g��(x, y, z, w = 1)�Ƃ��Čv�Z����A
		/// �v�Z���w��x, y, z�������������l���O�����x�N�g���Ƃ��ĕԂ��B
		/// �v�Z���w��0�ɂȂ�Ƃ��A�S�����̐�Βl�͖�����ɂȂ�
		/// </summary>
		/// <param name="rhs">w = 1��ǉ�����4�s1��̍s��Ƃ��ĉ��߂����O�����x�N�g��</param>
		constexpr Vector3 Mul(const Vector3& rhs) const noexcept {
			float x, y, z, w;
			x = m_matrix[0] * rhs.x + m_matrix[4] * rhs.y + m_matrix[8] * rhs.z + m_matrix[12];
			y = m_matrix[1] * rhs.x + m_matrix[5] * rhs.y + m_matrix[9] * rhs.z + m_matrix[13];
			z = m_matrix[2] * rhs.x + m_matrix[6] * rhs.y + m_matrix[10] * rhs.z + m_matrix[14];
			w = m_matrix[3] * rhs.x + m_matrix[7] * rhs.y + m_matrix[11] * rhs.z + m_matrix[15];
			if (System::Math::EqualZero(w)) {
				return Vector3(
					System::Math::Sign(x) * System::FLOAT_INF,
					System::Math::Sign(y) * System::FLOAT_INF,
					System::Math::Sign(z) * System::FLOAT_INF
				);
			}
			else return Vector3(x / w, y / w, z / w);
		}
	public:
		constexpr float& operator[](size_t index) noexcept {
			return m_matrix[index < 16 ? index : (index % 16)];
		}
		constexpr const float& operator[](size_t index) const noexcept {
			return m_matrix[index < 16 ? index : (index % 16)];
		}
		constexpr Matrix operator*(const Matrix& rhs) const noexcept { return Mul(rhs); }
		constexpr Matrix& operator*=(const Matrix& rhs) noexcept { return *this = *this * rhs; }
		constexpr Vector3 operator*(const Vector3& rhs) const noexcept { return Mul(rhs); }
		friend constexpr Vector3 operator*(const Vector3& lhs, const Matrix& rhs) noexcept {
			float x = lhs.x * rhs.m_matrix[0] + lhs.y * rhs.m_matrix[1] + lhs.z * rhs.m_matrix[2] + rhs.m_matrix[3];
			float y = lhs.x * rhs.m_matrix[4] + lhs.y * rhs.m_matrix[5] + lhs.z * rhs.m_matrix[6] + rhs.m_matrix[7];
			float z = lhs.x * rhs.m_matrix[8] + lhs.y * rhs.m_matrix[9] + lhs.z * rhs.m_matrix[10] + rhs.m_matrix[11];
			float w = lhs.x * rhs.m_matrix[12] + lhs.y * rhs.m_matrix[13] + lhs.z * rhs.m_matrix[14] + rhs.m_matrix[15];
			if (System::Math::EqualZero(w))
				return Vector3(
					System::Math::Sign(x) * System::FLOAT_INF,
					System::Math::Sign(y) * System::FLOAT_INF,
					System::Math::Sign(z) * System::FLOAT_INF
				);
			else return Vector3(x / w, y / w, z / w);
		}
		constexpr bool operator==(const Matrix&) const noexcept = default;
	};
}
