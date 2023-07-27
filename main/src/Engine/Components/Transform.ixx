export module Components:Transform;
import :Component;
import System;
using namespace System;
using namespace Engine;

export namespace Engine { class Transform; }

export class Engine::Transform : public Component {
public:
	Vector3 LocalPosition = Vector3::Zero();
	Quaternion LocalRotation = Quaternion::Identity();
	Vector3 LocalScale = Vector3::One();
private:
	Transform* m_parent = nullptr;
	Vector<Transform*> m_children;
public:
	using Component::Component;
	Transform() noexcept = default;
	Transform(GameObject* gObj) noexcept : Component(gObj) {}
	Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scale) noexcept
		: LocalPosition(pos), LocalRotation(rot), LocalScale(scale) {}
public:
	Transform* GetParent() const noexcept { return m_parent; }
	void SetParent(Transform* parent) noexcept {
		if (m_parent) m_parent->m_children.RemoveValue(this);
		m_parent = parent;
		if (m_parent) m_parent->m_children.Add(this);
	}
	bool HasParent() const noexcept { return m_parent; }
	const Vector<Transform*>& GetChildren() const noexcept { return m_children; }
	Transform* GetRoot() const noexcept {
		Transform* ret = const_cast<Transform*>(this);
		while (ret->m_parent) ret = ret->m_parent;
		return ret;
	}
public:
	/// <summary>
	/// ���[���h��ԏ�ł̈ʒu���擾����
	/// </summary>
	Vector3 Position() const noexcept {
		//���g�̃X�P�[�����O�Ɖ�]�̓��[���h��ԏ�ł̈ʒu�ɉe�����Ȃ�
		Vector3 ret = LocalPosition;
		Transform* parent = m_parent;
		while (parent) {
			ret = parent->LocalPosition + (parent->LocalRotation * Vector3::Scale(parent->LocalScale, ret));
			parent = parent->m_parent;
		}
		return ret;
		//return m_parent ? m_parent->World() * LocalPosition : LocalPosition;
	}
	/// <summary>
	/// ���[���h��ԏ�ł̈ʒu��ݒ肷��
	/// </summary>
	void Position(const Vector3& position) noexcept {
		//�e���W�n�ł̈ʒu�ɕϊ����Đݒ肷��
		if (!m_parent) LocalPosition = position;
		else LocalPosition = m_parent->WorldInverse() * position;
	}
	/// <summary>
	/// ���[���h��ԏ�ł̎p�����擾����B
	/// </summary>
	Quaternion Rotation() const noexcept {
		//�e�̃X�P�[�����O�╽�s�ړ��͎p���ɉe�����Ȃ�
		return m_parent ? m_parent->Rotation() * LocalRotation : LocalRotation;
	}
	/// <summary>
	/// ���[���h��ԏ�ł̉�]��ݒ肷��
	/// </summary>
	void Rotation(const Quaternion& rotation) noexcept {
		//�e���W�n�ł̉�]�ɕϊ����Đݒ肷��
		if (!m_parent) LocalRotation = rotation;
		else LocalRotation = m_parent->Rotation().Inverse() * rotation;
	}
	/// <summary>
	/// ���[���h��ԏ�ł̃X�P�[��
	/// </summary>
	Vector3 Scale() const noexcept {
		if (!m_parent) return LocalScale;
		//��D��̃��[���h�ϊ��s��ɂ����Ċe����x�N�g���ƌ����Ƃ��A
		//���̒����͂��ꂼ��X, Y, Z���̃X�P�[���l�ł���
		Matrix world = World();
		return Vector3(
			Vector3(world[0], world[1], world[2]).Magnitude(),
			Vector3(world[4], world[5], world[6]).Magnitude(),
			Vector3(world[8], world[9], world[10]).Magnitude()
		);
	}
	/// <summary>
	/// ���[���h��ԏ�ł̃X�P�[����ݒ肷��
	/// </summary>
	void Scale(const Vector3& scale) noexcept {
		//�e���W�n�ł̉�]�ɕϊ����Đݒ肷��
		if (!m_parent) LocalScale = scale;
		else {
			// mat * (����Transform�̃X�P�[�����O�s��) = (���[���h�s��)
			// |	m00	m01	m02	m03	||	Sx		0		0		0 |
			//	|	m10	m11	m12	m13	||	0		Sy		0		0 |
			// |	m20	m21	m22	m23	||	0		0		Sz		0 |
			// |		0		0		0		1	||	0		0		0		1 |
			// ���[���h�s��̃X�P�[����
			// Vector3(
			//		(Vector3(m00, m10, m20) * Sx).Magnitude(),
			//		(Vector3(m01, m11, m21) * Sy).Magnitude(),
			//		(Vector3(m02, m12, m22) * Sz).Magnitude(),
			//	);
			// ���̂Ƃ��A
			// WorldSx
			// == (Vector3(m00, m10, m20) * Sx).Magnitude()
			// == Vector3(m00, m10, m20).Magnitude() * Sx
			// �ł��邱�Ƃ𗘗p����
			Matrix mat = m_parent->World() * LocalRotation.ToRotationMatrix();
			float xMag = Vector3(mat[0], mat[1], mat[2]).Magnitude();
			float yMag = Vector3(mat[4], mat[5], mat[6]).Magnitude();
			float zMag = Vector3(mat[8], mat[9], mat[10]).Magnitude();
			LocalScale.x = xMag != 0.f ? scale.x / xMag : LocalScale.x;
			LocalScale.y = yMag != 0.f ? scale.y / yMag : LocalScale.y;
			LocalScale.z = zMag != 0.f ? scale.z / zMag : LocalScale.z;
		}
	}
public:
	/// <summary>
	/// ����Transform�̃��[�J��Z�������[���h��ԏ�Ō����Ă���������擾����
	/// </summary>
	Vector3 Forward() const noexcept {
		return Rotation() * Vector3::Forward();
	}
	/// <summary>
	/// ����Transform�̃��[�J��X�������[���h��ԏ�Ō����Ă���������擾����
	/// </summary>
	Vector3 Right() const noexcept {
		return Rotation() * Vector3::Right();
	}
	/// <summary>
	/// ����Transform�̃��[�J��Y�������[���h��ԏ�Ō����Ă���������擾����
	/// </summary>
	Vector3 Up() const noexcept {
		return Rotation() * Vector3::Up();
	}
public:
	/// <summary>
	/// �e��Ԃւ̕ϊ��s����擾����B
	/// �e�����Ȃ��ꍇ�A���[���h���W�n�ւ̕ϊ��s��Ɠ�����
	/// </summary>
	Matrix Local() const noexcept {
		Matrix rot = LocalRotation.ToRotationMatrix();
		return Matrix{
			LocalScale.x * rot[0], LocalScale.y * rot[4], LocalScale.z * rot[8], LocalPosition.x,
			LocalScale.x * rot[1], LocalScale.y * rot[5], LocalScale.z * rot[9], LocalPosition.y,
			LocalScale.x * rot[2], LocalScale.y * rot[6], LocalScale.z * rot[10], LocalPosition.z,
			0, 0, 0, 1
		};
	}
	/// <summary>
	/// �e��Ԃւ̕ϊ���ł������ϊ��s����擾����B
	/// �e��Ԃ̍��W(����Transform���e��Ԃɑ��݂���)�ɂ����邱�ƂŁA
	/// ����Transform�̍��W�n�ɕϊ��ł���
	/// </summary>
	Matrix LocalInverse() const noexcept {
		Matrix inv = LocalRotation.Inverse().ToRotationMatrix();
		float sx = 1.f / LocalScale.x;
		float sy = 1.f / LocalScale.y;
		float sz = 1.f / LocalScale.z;
		return Matrix{
			sx * inv[0], sx * inv[4], sx * inv[8], -sx * (inv[0] * LocalPosition.x + inv[4] * LocalPosition.y + inv[8] * LocalPosition.z),
			sy * inv[1], sy * inv[5], sy * inv[9], -sy * (inv[1] * LocalPosition.x + inv[5] * LocalPosition.y + inv[9] * LocalPosition.z),
			sz * inv[2], sz * inv[6], sz * inv[10], -sz * (inv[2] * LocalPosition.x + inv[6] * LocalPosition.y + inv[10] * LocalPosition.z),
			0, 0, 0, 1
		};
	}
	/// <summary>
	/// �w�肵���x�N�g�����������s�ړ����Ă���e��Ԃւ̕ϊ����s���A
	/// ����ɍŏ��̕��s�ړ��̋t�֕��s�ړ����鑀���\���s����擾����
	/// </summary>
	/// <param name="origin">�e��Ԃւ̕ϊ����s���Ƃ��̌��_</param>
	Matrix LocalOrigin(const Vector3& origin) const noexcept {
		Matrix ret = Local();
		ret[12] += -origin.x * (ret[0] - 1) - origin.y * ret[4] - origin.z * ret[8];
		ret[13] += -origin.x * ret[1] - origin.y * (ret[5] - 1) - origin.z * ret[9];
		ret[14] += -origin.x * ret[2] - origin.y * ret[6] - origin.z * (ret[10] - 1);
		return ret;
	}
	/// <summary>
	/// ���[���h��Ԃւ̕ϊ��s����擾����
	/// </summary>
	Matrix World() const noexcept {
		return m_parent ? m_parent->World() * Local() : Local();
	}
	/// <summary>
	/// ���[���h��Ԃւ̕ϊ���ł������ϊ��s����擾����B
	/// ���[���h��Ԃ̍��W�ɂ����邱�ƂŁA����Transform�̍��W�n�ɕϊ��ł���B
	/// ����Transform�Ƃ̈ʒu�֌W���v�Z����ꍇ�A����Transform�̐e��WorldInverse()���g�p���āA
	/// ����Transform�̐e�̍��W�n�ɕϊ�����(����Transform�̌Z��ɂ���)�K�v������
	/// </summary>
	Matrix WorldInverse() const noexcept {
		return m_parent ? LocalInverse() * m_parent->WorldInverse() : LocalInverse();
	}
public:
	/// <summary>
	/// �e��ԍ��W�n�̓_������Transform��ԍ��W�n�̓_�ɕϊ�����
	/// </summary>
	/// <param name="parentSpacePos">�e��ԍ��W�n�̓_</param>
	Vector3 ToLocalSpace(const Vector3& parentSpacePos) const noexcept {
		return Vector3::Scale((LocalRotation.Inverse() * (parentSpacePos - LocalPosition)), LocalScale.Reciprocal());
	}
public:
	Type GetType() const noexcept override { return Type::CreateType<Transform>(); }
	String ToString() const noexcept override { return String(u"Transform Component"); }
	uint32_t GetTypeID() const noexcept override { return System::GetID<Transform>(); }
	Transform* Clone(GameObject* object) noexcept override {
		Transform* ret = new Transform(object);
		ret->LocalPosition = LocalPosition;
		ret->LocalRotation = LocalRotation;
		ret->LocalScale = LocalScale;
		return ret;
	}
};

//export namespace Engine {
//	class Transform : public Component {
//	public:
//		Vector3 LocalPosition = Vector3::Zero();
//		Quaternion LocalRotation = Quaternion::Identity();
//		Vector3 LocalScale = Vector3::One();
//		uint32_t Width = 0;
//		uint32_t Height = 0;
//	private:
//		Transform* m_parent = nullptr;
//		Vector<Transform*> m_children;
//	public:
//		using Component::Component;
//	public:
//		static Matrix Local(const Vector3& position, const Quaternion& rotation, const Vector3& scale) noexcept {
//			float w2 = rotation.w * rotation.w;
//			float x2 = rotation.x * rotation.x;
//			float y2 = rotation.y * rotation.y;
//			float z2 = rotation.z * rotation.z;
//			float xy2 = rotation.x * rotation.y * 2;
//			float xz2 = rotation.x * rotation.z * 2;
//			float xw2 = rotation.x * rotation.w * 2;
//			float yz2 = rotation.y * rotation.z * 2;
//			float yw2 = rotation.y * rotation.w * 2;
//			float zw2 = rotation.z * rotation.w * 2;
//			return Matrix{
//				scale.x * (w2 + x2 - y2 - z2), scale.y * (xy2 - zw2), scale.z * (xz2 + yw2), position.x,
//				scale.x * (xy2 + zw2), scale.y * (w2 - x2 + y2 - z2), scale.z * (yz2 - xw2), position.y,
//				scale.x * (xz2 - yw2), scale.y * (yz2 + xw2), scale.z * (w2 - x2 - y2 + z2), position.z,
//				0, 0, 0, 1
//			};
//		}
//		//static Matrix LocalForScreenSize(const Vector3& position, const Quaternion& rotation, const Vector3& scale, uint32_t parentWidth, uint32_t parentHeight) noexcept {
//		//	float rot[9];
//		//	rotation.ToRotationMatrix(rot);
//		//	Matrix ret;
//		//	float w_r = parentWidth ? 1.f / parentWidth : 0;
//		//	float h_r = parentHeight ? 1.f / parentHeight : 0;
//		//	ret.m_matrix[0] = scale.x * rot[0];
//		//	ret.m_matrix[1] = scale.y * rot[1] * parentHeight * w_r;
//		//	ret.m_matrix[2] = 2 * rot[2] * w_r;
//		//	ret.m_matrix[3] = 2 * position.x;
//		//	ret.m_matrix[4] = scale.x * rot[3] * parentWidth * h_r;
//		//	ret.m_matrix[5] = scale.y * rot[4];
//		//	ret.m_matrix[6] = 2 * rot[5] * h_r;
//		//	ret.m_matrix[7] = 2 * position.y;
//		//	ret.m_matrix[8] = scale.x * rot[6] * parentWidth / 2;
//		//	ret.m_matrix[9] = scale.y * rot[7] * parentHeight / 2;
//		//	ret.m_matrix[10] = rot[8];
//		//	ret.m_matrix[11] = 0;
//		//	ret.m_matrix[12] = 0;
//		//	ret.m_matrix[13] = 0;
//		//	ret.m_matrix[14] = 0;
//		//	ret.m_matrix[15] = 1;
//		//	return ret;
//		//}
//		//static Matrix LocalForPixelSize(const Vector3& position, const Quaternion& rotation, uint32_t width, uint32_t height, uint32_t parentWidth, uint32_t parentHeight) noexcept {
//		//	float rot[9];
//		//	rotation.ToRotationMatrix(rot);
//		//	Matrix ret;
//		//	float w_r = parentWidth ? 1.f / parentWidth : 0;
//		//	float h_r = parentHeight ? 1.f / parentHeight : 0;
//		//	ret.m_matrix[0] = width * w_r * rot[0];
//		//	ret.m_matrix[1] = height * w_r * rot[1];
//		//	ret.m_matrix[2] = 2 * rot[2] * w_r;
//		//	ret.m_matrix[3] = 2 * position.x * w_r;
//		//	ret.m_matrix[4] = width * h_r * rot[3];
//		//	ret.m_matrix[5] = height * h_r * rot[4];
//		//	ret.m_matrix[6] = 2 * rot[5] * h_r;
//		//	ret.m_matrix[7] = 2 * position.y * h_r;
//		//	ret.m_matrix[8] = width * rot[6] / 2;
//		//	ret.m_matrix[9] = height * rot[7] / 2;
//		//	ret.m_matrix[10] = rot[8];
//		//	ret.m_matrix[11] = 0;
//		//	ret.m_matrix[12] = 0;
//		//	ret.m_matrix[13] = 0;
//		//	ret.m_matrix[14] = 0;
//		//	ret.m_matrix[15] = 1;
//		//	return ret;
//		//}
//		static Matrix LocalInverse(const Vector3& position, const Quaternion& rotation, const Vector3& scale) noexcept {
//			float w2 = rotation.w * rotation.w;
//			float x2 = rotation.x * rotation.x;
//			float y2 = rotation.y * rotation.y;
//			float z2 = rotation.z * rotation.z;
//			float xy2 = rotation.x * rotation.y * 2;
//			float xz2 = rotation.x * rotation.z * 2;
//			float xw2 = rotation.x * rotation.w * 2;
//			float yz2 = rotation.y * rotation.z * 2;
//			float yw2 = rotation.y * rotation.w * 2;
//			float zw2 = rotation.z * rotation.w * 2;
//			float m00 = w2 + x2 - y2 - z2;
//			float m10 = xy2 + zw2;
//			float m20 = xz2 - yw2;
//			float m01 = xy2 - zw2;
//			float m11 = w2 - x2 + y2 - z2;
//			float m21 = yz2 + xw2;
//			float m02 = xz2 + yw2;
//			float m12 = yz2 - xw2;
//			float m22 = w2 - x2 - y2 + z2;
//			float sx = 1.f / scale.x;
//			float sy = 1.f / scale.y;
//			float sz = 1.f / scale.z;
//			return Matrix{
//				sx * m00, sx * m10, sx * m20, -sx * (m00 * position.x + m10 * position.y + m20 * position.z),
//				sy * m01, sy * m11, sy * m21, -sy * (m01 * position.x + m11 * position.y + m21 * position.z),
//				sz * m02, sz * m12, sz * m22, -sz * (m02 * position.x + m12 * position.y + m22 * position.z),
//				0, 0, 0, 1
//			};
//		}
//		/*static Matrix LocalInverseForScreenSize(const Vector3& position, const Quaternion& rotation, const Vector3& scale, uint32_t parentWidth, uint32_t parentHeight) noexcept {
//			Terminate(Exception(__FUNCSIG__, u"�������ł��B", __FILE__, __LINE__));
//			return Matrix();
//		}
//		static Matrix LocalInverseForPixelSize(const Vector3& position, const Quaternion& rotation, uint32_t width, uint32_t height, uint32_t parentWidth, uint32_t parentHeight) noexcept {
//			Terminate(Exception(__FUNCSIG__, u"�������ł��B", __FILE__, __LINE__));
//			return Matrix();
//		}*/
//		static Matrix LocalOrigin(const Vector3& position, const Quaternion& rotation, const Vector3& scale, const Vector3& origin) noexcept {
//			Matrix ret = Local(position, rotation, scale);
//			ret[12] = -origin.x * (ret[0] - 1) - origin.y * ret[4] - origin.z * ret[8] + position.x;
//			ret[13] = -origin.x * ret[1] - origin.y * (ret[5] - 1) - origin.z * ret[9] + position.y;
//			ret[14] = -origin.x * ret[2] - origin.y * ret[6] - origin.z * (ret[10] - 1) + position.z;
//			return ret;
//		}
//	public:
//		/// <summary>
//		/// ����Transform�ɐe�ƂȂ�Transform��ݒ肷��
//		/// </summary>
//		void SetParent(Transform* parent) noexcept {
//			if (parent == m_parent) return;
//			if (m_parent) m_parent->m_children.RemoveValue(this);
//			m_parent = parent;
//			if (m_parent) m_parent->m_children.Add(this);
//		}
//		Transform* GetParent() const noexcept {
//			return m_parent;
//		}
//		Vector<Transform*>& GetChildren() noexcept {
//			return m_children;
//		}
//		const Vector<Transform*>& GetChildren() const noexcept {
//			return m_children;
//		}
//	public:
//		Vector3 Forward() const noexcept { return Rotation() * Vector3::Forward(); }
//		Vector3 Right() const noexcept { return Rotation() * Vector3::Right(); }
//		Vector3 Up() const noexcept { return Rotation() * Vector3::Up(); }
//	public:
//		void Position(const Vector3& pos) noexcept {
//			if (m_parent) {
//				Transform* parent = m_parent;
//				Matrix mat = parent->LocalInverse();
//				parent = parent->GetParent();
//				while (parent) {
//					mat = parent->LocalInverse() * mat;
//					parent = parent->GetParent();
//				}
//				LocalPosition = pos * mat;
//			}
//			else LocalPosition = pos;
//		}
//		void Rotation(const Quaternion& rot) noexcept {
//			if (m_parent) {
//				Transform* parent = m_parent;
//				Quaternion q = parent->LocalRotation.Inverse();
//				parent = parent->GetParent();
//				while (parent) {
//					q = parent->LocalRotation.Inverse() * q;
//					parent = parent->GetParent();
//				}
//				LocalRotation = rot * q;
//			}
//			else LocalRotation = rot;
//		}
//		void Scale(const Vector3 scale) noexcept {
//			if (m_parent) {
//				Vector3 worldScale = Scale();
//				LocalScale.x *= scale.x / worldScale.x;
//				LocalScale.y *= scale.y / worldScale.y;
//				LocalScale.z *= scale.z / worldScale.z;
//				//Matrix mat;
//				//LocalRotation.Inverse().ToRotationMatrix(mat);
//				//Transform* parent = m_parent;
//				//while (parent)
//				//{
//				//	Matrix rot;
//				//	parent->LocalRotation.Inverse().ToRotationMatrix(rot);
//				//	Matrix sc = Matrix::Identity();
//				//	sc.m_matrix[0] = 1.f / parent->LocalScale.x;
//				//	sc.m_matrix[5] = 1.f / parent->LocalScale.y;
//				//	sc.m_matrix[10] = 1.f / parent->LocalScale.z;
//				//	mat = rot * sc * mat;
//				//	parent = parent->GetParent();
//				//}
//				//LocalScale = scale * mat;
//			}
//			else LocalScale = scale;
//		}
//	public:
//		/// <summary>
//		/// ���[���h��ԏ�ł̈ʒu���擾����
//		/// </summary>
//		Vector3 Position() const noexcept {
//			return m_parent ? m_parent->Rotation() * LocalPosition + m_parent->Position() : LocalPosition;
//		}
//		/// <summary>
//		/// ���[���h��ԏ�ł̉�]���擾����
//		/// </summary>
//		Quaternion Rotation() const noexcept {
//			return m_parent ? m_parent->Rotation() * LocalRotation : LocalRotation;
//		}
//		/// <summary>
//		/// ���[���h��ԏ�ł̃X�P�[�����擾����
//		/// </summary>
//		Vector3 Scale() const noexcept {
//			if (m_parent) {
//				Matrix world = World();
//				return Vector3{
//					Vector3(world[0], world[1], world[2]).Magnitude(),
//					Vector3(world[4], world[5], world[6]).Magnitude(),
//					Vector3(world[8], world[9], world[10]).Magnitude()
//				};
//				//float rot[9];
//				//LocalRotation.ToRotationMatrix(rot);
//				//Vector3 parentScale = m_parent->Scale();
//				//return Vector3(
//				//	LocalScale.x * Vector3(parentScale.x * rot[0], parentScale.y * rot[1], parentScale.z * rot[2]).Magnitude(),
//				//	LocalScale.y * Vector3(parentScale.x * rot[4], parentScale.y * rot[5], parentScale.z * rot[6]).Magnitude(),
//				//	LocalScale.z * Vector3(parentScale.x * rot[8], parentScale.y * rot[9], parentScale.z * rot[10]).Magnitude()
//				//);
//			}
//			else return LocalScale;
//		}
//		/// <summary>
//		/// ����Transform���\���p�����s��(��D��)�Ŏ擾����B
//		/// ���̊֐����Ԃ��s��M�͎�[(x, y, z, 1) * M]�ɂ���ăx�N�g���ϊ��Ɏg�p�ł���B
//		/// �܂��A�s��M�͓]�u���邱�Ƃōs�D��̎p���s��ƂȂ�A��[M * (x, y, z, 1)]��
//		/// ���l�ɕϊ��ł���B
//		/// ���̃x�N�g���ϊ��ɂ��e�̋�ԍ��W�n�ɕϊ�����邽�߁A
//		/// ��[(x, y, z, 1) * M * parentM * grandParentM * ...]�ɂ�胏�[���h��ԏ�ł�
//		/// �x�N�g�����擾�ł���
//		/// </summary>
//		/// <param name="mode">Transform�����l�̉��߂��w�肷��</param>
//		/// <param name="parentWidth">�e��Ԃ̕��B�X�N���[����łȂ��ꍇ�A�s�v</param>
//		/// <param name="parentHeight">�e��Ԃ̍����B�X�N���[����łȂ��ꍇ�A�s�v</param>
//		Matrix Local(CanvasScalingMode mode = CanvasScalingMode::World, uint32_t parentWidth = 1, uint32_t parentHeight = 1) const noexcept {
//			return Local(LocalPosition, LocalRotation, LocalScale);
//			//if (mode == CanvasScalingMode::World) return Local(LocalPosition, LocalRotation, LocalScale);
//			//else if (mode == CanvasScalingMode::ScreenSize) return LocalForScreenSize(LocalPosition, LocalRotation, LocalScale, parentWidth, parentHeight);
//			//else return LocalForPixelSize(LocalPosition, LocalRotation, Width, Height, parentWidth, parentHeight);
//		}
//		Matrix LocalInverse(CanvasScalingMode mode = CanvasScalingMode::World, uint32_t parentWidth = 1, uint32_t parentHeight = 1) const noexcept {
//			return LocalInverse(LocalPosition, LocalRotation, LocalScale);
//			//if (mode == CanvasScalingMode::World) return LocalInverse(LocalPosition, LocalRotation, LocalScale);
//			//else if (mode == CanvasScalingMode::ScreenSize) return LocalInverseForScreenSize(LocalPosition, LocalRotation, LocalScale, parentWidth, parentHeight);
//			//else return LocalInverseForPixelSize(LocalPosition, LocalRotation, Width, Height, parentWidth, parentHeight);
//		}
//		/// <summary>
//		/// ���_�֕��s�ړ�->Local()->���̈ʒu�ɕ��s�ړ����s���s����擾����
//		/// </summary>
//		/// <param name="origin">���̈ʒu</param>
//		Matrix LocalOrigin(const Vector3& origin) noexcept {
//			return LocalOrigin(LocalPosition, LocalRotation, LocalScale, origin);
//		}
//	private:
//		static Matrix World_Internal(const Transform& tr, CanvasScalingMode mode, uint32_t width, uint32_t height) noexcept {
//			Transform* parent = tr.GetParent();
//			if (parent) {
//				Matrix parentWorld = World_Internal(*parent, mode, width, height);
//				Vector3 wh = parentWorld * Vector3(static_cast<float>(width), static_cast<float>(height));
//				return parentWorld * tr.Local(mode, static_cast<uint32_t>(wh.x), static_cast<uint32_t>(wh.y));
//			}
//			else return tr.Local(mode, width, height);
//		}
//	public:
//		Matrix World(CanvasScalingMode mode = CanvasScalingMode::World, uint32_t rootWidth = 0, uint32_t rootHeight = 0) const noexcept {
//			if (mode != CanvasScalingMode::World && (rootWidth == 0 || rootHeight == 0)) return Matrix();
//			Matrix ret = World_Internal(*this, mode, rootWidth, rootHeight);
//			return ret;
//		}
//	public:
//		void RotateAround(Vector3 centerPos, Vector3 upAxis, float angle) noexcept {
//			const Vector3 pos = Position();
//			const Quaternion rot = Quaternion::AngleAxis(angle, upAxis);
//			LocalRotation = rot * LocalRotation;
//			const Vector3 delta = (pos - centerPos) * rot;
//			Position(centerPos + delta);
//		}
//	public:
//		Type GetType() const noexcept override { return Type::CreateType<Transform>(); }
//		String ToString() const noexcept override {
//			return String::Joint(
//				u"Transform { ",
//				u" LocalPosition: ", LocalPosition.ToString(),
//				u", LocalRotation: ", LocalRotation.ToString(),
//				u", LocalScale: ", LocalScale.ToString(),
//				u" }"
//			);
//		}
//		uint32_t GetTypeID() const noexcept override { return GetID<Transform>(); }
//	};
//}
