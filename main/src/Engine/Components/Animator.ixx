export module Components:Animator;
import :Component;
import :Transform;
import :Animator_Internal;
import System;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

export namespace Engine { class Animator; }

List<ManagedObject<State>>& GetStates() noexcept {
	static List<ManagedObject<State>> ret;
	return ret;
}

/// <summary>
/// �A�j���[�V�������Ǘ�����R���|�[�l���g�N���X
/// </summary>
export class Engine::Animator : public Engine::Component {
	ManagedObject<State> m_currentState;	//���݂̏��
	size_t m_currentFrameNo = 0;	//���݂̃t���[���ԍ�
	Vector<Transform> m_currentBones;	//���݂̃{�[���̎p��(�{�[���s��)
	bool m_end = false;	//���݂̏�Ԃ̃A�j���[�V�������I�����Ă���ꍇ�Atrue(�A�j���[�V�����v�Z���X�L�b�v����)
	size_t m_updateCountPerFrame = 1;	//���̃t���[���ԍ��Ɉڂ邽�߂ɕK�v��Update�֐��̌Ăяo����(0�ɂ͂Ȃ�Ȃ�)
	size_t m_updateCount = 1;	//Update�֐��̌Ăяo�����ƂɃf�N�������g����A0�ɂȂ����Ƃ����̃t���[���ԍ��Ɉڂ�
	AnimationController* m_controller = nullptr;	//��ԑJ�ڂ��Ǘ�����R���g���[��
	Resource m_animationResource;
	Heap m_animationHeap;
public:
	Animator() noexcept;
	Animator(GameObject* object) noexcept;
	~Animator() noexcept;
public: /* ���[�V�����o�^�֘A */
	/// <summary>
	/// ��Ԃ�o�^����
	/// </summary>
	/// <param name="filePath">�A�j���[�V�����t�@�C���̐�΃p�X</param>
	/// <param name="name">��Ԗ�</param>
	/// <param name="repeat">�A�j���[�V���������s�[�g�Đ����邩�ۂ�</param>
	/// <returns>�o�^�ɐ��������ꍇ�Atrue�B���łɓ������O�̏�Ԃ����݂���Ƃ��A�o�^�Ɏ��s����</returns>
	bool SetMotion(const String& filePath, const String& name, bool repeat = true) noexcept {
		if (ManagedObject<State>::GetObject(name)) return false;
		State* tmp = new State(name);
		tmp->SetMotion(filePath);
		tmp->SetRepeat(repeat);
		GetStates().PushBack(ManagedObject<State>(name, tmp));
		return true;
	}
	bool SetMotionByBone(const String& filePath, const String& name, bool repeat = true) noexcept {
		if (ManagedObject<State>::GetObject(name)) return false;
		State* tmp = new State(name);
		tmp->SetMotion(filePath, false);
		tmp->SetRepeat(repeat);
		GetStates().PushBack(ManagedObject<State>(name, tmp));
		return true;
	}
	/// <summary>
	/// �w�肵����Ԃ̃A�j���[�V���������s�[�g�Đ����邩�ۂ���ݒ肷��
	/// </summary>
	/// <param name="name">��Ԗ�</param>
	/// <param name="repeat">�A�j���[�V���������s�[�g�Đ����邩�ۂ�</param>
	/// <returns>�ݒ�ɐ��������ꍇ�Atrue</returns>
	bool SetRepeat(const String& name, bool repeat) noexcept {
		ManagedObject<State> state = ManagedObject<State>::GetObject(name);
		if (!state) return false;
		state->SetRepeat(repeat);
		return true;
	}
public:
	/// <summary>
	/// ���̃t���[���ԍ��Ɉڂ邽�߂ɕK�v��Update�֐��̌Ăяo���񐔂�ݒ肷��B
	/// 0�͎w��ł��Ȃ�(1���w�肵���ꍇ�Ɠ����ɂȂ�)
	/// </summary>
	void SetUpdateCountPerFrame(size_t count) noexcept {
		m_updateCountPerFrame = count ? count : 1;
		if (m_updateCount > m_updateCountPerFrame) m_updateCount = m_updateCountPerFrame;
	}
public:
	/// <summary>
	/// AnimationController���擾����
	/// </summary>
	AnimationController& GetAnimationController() noexcept { return *m_controller; }
	/// <summary>
	/// ���݂̏�Ԃ̃A�j���[�V�������I�����Ă��邩�ۂ����擾����
	/// </summary>
	bool IsEnd() const noexcept { return m_end; }
private:
	/// <summary>
	/// �ċA�I�ɌĂяo���ă{�[���s����X�V����
	/// </summary>
	/// <param name="mat">�{�[���s��̔z��</param>
	/// <param name="root">�e�{�[��</param>
	/// <param name="bones">���f���Œ�`����Ă���{�[�����̔z��</param>
	/// <param name="boneCount">���f���Œ�`����Ă���{�[���̌�</param>
	void GetBoneMatrix_Internal(Matrix* mat, Bone& root, Bone* bones, size_t boneCount) noexcept {
		for (size_t i = 0; i < boneCount; ++i) {
			Bone& b = bones[i];
			if (b.parent != root.id || b.id >= boneCount) continue;
			//���̃{�[���̍s��ɐe�{�[���̍s�����Z����
			mat[b.id] = mat[root.id] * m_currentBones[b.id].LocalOrigin(b.position);
			GetBoneMatrix_Internal(mat, b, bones, boneCount);
		}
	}
public:
	/// <summary>
	/// ���݂̏�Ԃ̃A�j���[�V�������X�V����B�t���[�����i�񂾂Ƃ��A�{�[���̎p�����Čv�Z�����
	/// </summary>
	void Update() noexcept;
	/// <summary>
	/// �{�[���s��̔z����擾����B
	/// �g�p����Ȃ��{�[���͒P�ʍs��ŃN���A�����
	/// </summary>
	/// <param name="out">�{�[���s��̏o�͐�</param>
	/// <returns>�{�[���z����擾�����ꍇ�Atrue�Bfalse��Ԃ��Ƃ��A�����͂��ׂĒP�ʍs��ɃN���A�����</returns>
	bool GetBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept;
	/// <summary>
	/// �P�ʍs��ŃN���A���ꂽ�{�[���s����擾����
	/// </summary>
	static void GetIdentityBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept {
		Matrix identity{};
		for (size_t i = 0; i < Common3D::BoneMatrixCount; ++i) out[i] = identity;
	}
private:
	/// <summary>
	/// 1�����NIK���v�Z����
	/// </summary>
	/// <param name="ik">IK�{�[�����</param>
	/// <param name="bones">���f���Œ�`���ꂽ�{�[�����̔z��</param>
	void SolveLookAt(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// 2�����NIK��]���藝�𗘗p���Čv�Z����
	/// </summary>
	/// <param name="ik">IK�{�[�����</param>
	/// <param name="bones">���f���Œ�`���ꂽ�{�[�����̔z��</param>
	void SolveCosine(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// 3�����N�ȏ��IK��CCDIK��p���Čv�Z����
	/// </summary>
	/// <param name="ik">IK�{�[�����</param>
	/// <param name="bones">���f���Œ�`���ꂽ�{�[�����̔z��</param>
	void SolveCCDIK(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// ���ׂĂ�IK�{�[�����v�Z���A�{�[���p�����X�V����
	/// </summary>
	void SolveIK() noexcept;
public:
	Heap GetHeap() const noexcept { return m_animationHeap; }
	void UpdateHeap() noexcept {
		AnimationResourceDesc desc;
		GetBoneMatrix(desc.bone);
		if (!m_animationResource) {
			m_animationResource = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
		}
		else m_animationResource.UpdateResource(desc.GetData(), desc.GetCount());
		if (!m_animationHeap) {
			m_animationHeap = CreateHeap(String::Empty(), HeapType::Animation);
			m_animationHeap.SetView(0, ViewFormat::CBV, m_animationResource);
		}
	}
public:
	Type GetType() const noexcept override { return Type::CreateType<Animator>(); }
	String ToString() const noexcept override { return String(u"Animator Component"); }
	uint32_t GetTypeID() const noexcept override { return System::GetID<Animator>(); }
	Animator* Clone(GameObject* object) noexcept override;
};
