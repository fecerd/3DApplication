export module Matrix;
import CSTDINT;
import Vector3;
import Math;

export namespace System {
	/// <summary>
	/// 4行4列行列を表すクラス。
	/// float[16]の内部値を列優先として扱う。
	/// m行n列の要素をmn、4次元ベクトルの各要素を(x, y, z, w)とすると、
	/// | 00, 01, 02, 03 | | x  |
	/// | 10, 11, 12, 13 | | y  |
	/// | 20, 21, 22, 23 | | z  |
	/// | 30, 31, 32, 33 | | w |
	/// のように乗算する。
	/// 内部値は
	/// { 00, 10, 20, 30, 01, 11, 21, 31, 02, 12, 22, 32, 03, 13, 23, 33 }
	/// のように並んでいる。
	/// </summary>
	class Matrix {
	public:
		float m_matrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	public:
		constexpr Matrix() noexcept = default;
		constexpr Matrix(const Matrix&) noexcept = default;
		/// <summary>
		/// コンストラクタ呼び出し
		/// Matrix{
		///	00, 01, 02, 03,
		///	10, 11, 12, 13,
		///	20, 21, 22, 23,
		///	30, 31, 32, 33
		/// };
		/// をそのまま行列とみなして初期化する。
		/// 引数の順序とMatrixクラスの内部メモリレイアウトは一致しないので注意
		/// </summary>
		constexpr Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept
			: m_matrix{ m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33 } {}
	public:
		/// <summary>
		/// 単位行列を取得する。デフォルトコンストラクタ呼び出しと同等である
		/// </summary>
		static constexpr Matrix Identity() noexcept { return Matrix(); }
	public:
		/// <summary>
		/// 転置行列を取得する
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
		/// 行列同士の乗算を計算する
		/// </summary>
		/// <param name="rhs">この行列に右から乗算する行列</param>
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
		/// この行列を三次元ベクトルの左から乗算する。
		/// ベクトルはw成分を追加した4行1列の四次元ベクトル(x, y, z, w = 1)として計算され、
		/// 計算後のwでx, y, z成分を割った値を三次元ベクトルとして返す。
		/// 計算後のwが0になるとき、全成分の絶対値は無限大になる
		/// </summary>
		/// <param name="rhs">w = 1を追加した4行1列の行列として解釈される三次元ベクトル</param>
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
