export module Components:Animator_Internal;
import System;
export import Bone;
import :Declaration;
using namespace System;
using namespace Engine;

export namespace Engine {
	struct KeyBone;
	class MotionByBone;
	struct Frame;
	struct Frames;
	class Motion;
	class State;
	struct Transition;
	class AnimationController;
}

namespace Animator_Internal {
	void CreateBoneFromVMDMotion(Bone& out, const Formats::VMDMotion& mot) noexcept {
		out.name = mot.boneName;
		out.id = mot.frameNo;
		out.position = mot.location;
		out.rotation = mot.rotation;
		out.scale = Vector3::One();
		out.rotationPoint[0] = ControlPoint(false, mot.bezier[3], mot.bezier[7]);
		out.rotationPoint[1] = ControlPoint(false, mot.bezier[11], mot.bezier[15]);
		out.xTranslatePoint[0] = ControlPoint(false, mot.bezier[0], mot.bezier[4]);
		out.xTranslatePoint[1] = ControlPoint(false, mot.bezier[8], mot.bezier[12]);
		out.yTranslatePoint[0] = ControlPoint(false, mot.bezier[1], mot.bezier[5]);
		out.yTranslatePoint[1] = ControlPoint(false, mot.bezier[9], mot.bezier[13]);
		out.zTranslatePoint[0] = ControlPoint(false, mot.bezier[2], mot.bezier[6]);
		out.zTranslatePoint[1] = ControlPoint(false, mot.bezier[10], mot.bezier[14]);
	}
}
using namespace Animator_Internal;

/// <summary>
/// �L�[�t���[���̃{�[�������{�[���P�ʂŊǗ�����\����
/// </summary>
struct Engine::KeyBone {
	size_t hash = 0;
	Vector<Bone*> bones;
public:
	KeyBone() noexcept = default;
	KeyBone(KeyBone&&) noexcept = default;
	~KeyBone() noexcept {
		bones.DeleteAll();
		hash = 0;
	}
public:
	KeyBone& operator=(KeyBone&&) noexcept = default;
};

/// <summary>
/// �{�[���P�ʂŃA�j���[�V���������Ǘ�����N���X
/// </summary>
class Engine::MotionByBone {
	Vector<KeyBone> m_keyBones;	//�L�[�t���[�����ł���Ă���{�[�������{�[���P�ʂł܂Ƃ߂����̂̓��I�z��
	bool m_isAbsoluteTransform = false;	//�L�[�t���[���̃{�[����񂪐�Έʒu�̂Ƃ��Atrue�B�����p������̑��Εω��̂Ƃ��Afalse
	size_t m_maxFrameNo = 0;	//���ׂẴL�[�t���[���̒��ň�ԍŌ�̃t���[���ԍ�
public:
	MotionByBone() noexcept = default;
	MotionByBone(const MotionByBone&) noexcept = delete;
	MotionByBone(MotionByBone&&) noexcept = default;
	MotionByBone(const String& filePath) noexcept {
		m_isAbsoluteTransform = false;	//VMD�t�@�C���͏����p������̑��Εω����L�^����Ă���
		Formats::VMD vmd(filePath);
		for (Formats::VMDMotion& mot : vmd.motions) {	//VMDMotion�͂���L�[�t���[���̂���{�[����Ԃ�\���\����
			String name = String(mot.boneName, 20);
			size_t hash = name.GetHashCode();
			KeyBone* keyBone = nullptr;
			for (KeyBone& b : m_keyBones) {
				if (b.hash != hash) continue;
				if (b.bones[0]->name != name) continue;
				keyBone = &b;
				break;
			}
			if (!keyBone) {
				KeyBone tmp;
				tmp.hash = hash;
				m_keyBones.Add(static_cast<KeyBone&&>(tmp));
				keyBone = m_keyBones.Items() + (m_keyBones.Count() - 1);
			}
			//�{�[������ϊ����Ċi�[
			Bone* bone = new Bone();
			CreateBoneFromVMDMotion(*bone, mot);
			keyBone->bones.Add(bone);
			//�Ō�̃t���[���ԍ���ۑ�
			if (m_maxFrameNo < mot.frameNo) m_maxFrameNo = mot.frameNo;
		}
		for (KeyBone& k : m_keyBones) k.bones.Sort(true, [](Bone* const& lhs, Bone* const& rhs) { return lhs->id <=> rhs->id; });
	}
	~MotionByBone() noexcept = default;
public:
	/// <summary>
	/// �{�[�������{�[���P�ʂł܂Ƃ߂��\���̂̓��I�z����擾����
	/// </summary>
	Vector<KeyBone>& GetKeyBones() noexcept { return m_keyBones; }
	/// <summary>
	/// �Ō�̃L�[�t���[���ԍ����擾����
	/// </summary>
	size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
	/// <summary>
	/// �L�[�t���[���̃{�[�����̊i�[���@���擾����
	/// </summary>
	/// <returns>��Έʒu�̏ꍇ��true���A�����p������̑��Εω��̏ꍇ��false��Ԃ�</returns>
	bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
public:
	MotionByBone& operator=(const MotionByBone&) noexcept = delete;
	MotionByBone& operator=(MotionByBone&&) noexcept = default;
};

/// <summary>
/// �A�j���[�V�����f�[�^��1�L�[�t���[����\���\����
/// </summary>
struct Engine::Frame {
	HashMap<String, Bone> bones = 3;	//�{�[�����ŃA�N�Z�X�ł��邱�̃L�[�t���[���Ɋ܂܂��{�[�����B
	size_t id = System::MAX_VALUE<size_t>;	//�t���[���ԍ�
};

/// <summary>
/// ����t���[���ԍ��̑O�̃L�[�t���[���Ǝ��̃L�[�t���[�����܂ލ\����
/// </summary>
struct Engine::Frames {
	Frame* prev = nullptr;	//�O�̃L�[�t���[��
	Frame* next = nullptr;	//���̃L�[�t���[��
	float t = 0;	//0��O�̃L�[�t���[���A1�����̃L�[�t���[���Ƃ����Ƃ��̌��݂̃t���[���ʒu
};

/// <summary>
/// �A�j���[�V���������Ǘ�����N���X
/// </summary>
class Engine::Motion {
	Vector<Frame*> m_frames;	//���ׂẴL�[�t���[���̔z��(�����͕s��)
	bool m_isAbsoluteTransform = false;	//�L�[�t���[���̃{�[����񂪐�Έʒu�̂Ƃ��Atrue�B�����p������̑��Εω��̂Ƃ��Afalse
	size_t m_maxFrameNo = 0;	//���ׂẴL�[�t���[���̒��ň�ԍŌ�̃t���[���ԍ�
public:
	Motion() noexcept = default;
	Motion(const Motion&) noexcept = delete;
	Motion(Motion&&) noexcept = default;
	Motion(const String& filePath) noexcept {
		m_isAbsoluteTransform = false;	//VMD�t�@�C���͏����p������̑��Εω����L�^����Ă���
		Formats::VMD vmd(filePath);
		for (Formats::VMDMotion& mot : vmd.motions) {	//VMDMotion�͂���L�[�t���[���̂���{�[����Ԃ�\���\����
			Frame* frame = nullptr;
			//���łɒǉ�����Ă���t���[���Ȃ�擾
			for (Frame*& f : m_frames) {
				if (f->id == mot.frameNo) {
					frame = f;
					break;
				}
			}
			//���ǉ��̃t���[���Ȃ�ǉ�
			if (!frame) {
				frame = new Frame();
				frame->id = mot.frameNo;
				if (frame->id > m_maxFrameNo) m_maxFrameNo = frame->id;
				m_frames.Add(frame);
			}
			//�{�[������ϊ����Ċi�[
			Bone bone;
			CreateBoneFromVMDMotion(bone, mot);
			frame->bones.Insert(mot.boneName, static_cast<Bone&&>(bone));
		}
	}
	~Motion() noexcept {
		m_frames.DeleteAll();
	}
public:
	/// <summary>
	/// �w�肵���t���[���ԍ��̑O��̃L�[�t���[�����擾����
	/// </summary>
	/// <param name="currentFrame">�t���[���ԍ�</param>
	/// <returns>
	/// �w�肵���t���[���ԍ�����ݒ肳�ꂽFrames�\���́B
	/// �w�肵���t���[���ԍ��ƈ�v����L�[�t���[�������݂���Ƃ��A�O��̃L�[�t���[���͈�v���A�t���[����Ԉʒu��0�ƂȂ�B
	/// �w�肵���t���[���ԍ����ŏ��̃L�[�t���[���ԍ���菬�����Ƃ��A�O�̃L�[�t���[����nullptr�ƂȂ�B
	/// �w�肵���t���[���ԍ����Ō�̃L�[�t���[���ԍ����傫���Ƃ��A���̃L�[�t���[����nullptr�ƂȂ�A�t���[����Ԉʒu��0�ƂȂ�
	/// </returns>
	Frames GetFrames(size_t currentFrame) noexcept {
		Frames ret;
		Frame* end = nullptr;
		for (size_t i = 0, count = m_frames.Count(); i < count; ++i) {
			Frame* tmp = m_frames[i];
			if (tmp->id <= currentFrame) {
				if (!ret.prev || ret.prev->id < tmp->id) ret.prev = tmp;
			}
			if (tmp->id >= currentFrame) {
				if (!ret.next || ret.next->id > tmp->id) ret.next = tmp;
			}
			if (!end || end->id < tmp->id) end = tmp;
		}
		size_t p = ret.prev ? ret.prev->id : 0;
		size_t n = ret.next ? ret.next->id : end ? end->id : 0;
		ret.t = n > p ? static_cast<float>(currentFrame - p) / (n - p) : 0;
		return ret;
	}
	/// <summary>
	/// �Ō�̃L�[�t���[���ԍ����擾����
	/// </summary>
	size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
	/// <summary>
	/// �L�[�t���[���̃{�[�����̊i�[���@���擾����
	/// </summary>
	/// <returns>��Έʒu�̏ꍇ��true���A�����p������̑��Εω��̏ꍇ��false��Ԃ�</returns>
	bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
public:
	Motion& operator=(const Motion&) noexcept = delete;
	Motion& operator=(Motion&&) noexcept = default;
};

/// <summary>
/// Animation�N���X���Ǘ������Ԃ�\���N���X
/// </summary>
class Engine::State {
	String m_name;	//���̏�Ԃ̖��O(Animation�N���X�̃C���X�^���X���ň��)
	bool m_useFrameMotion = true;	//true�̂Ƃ�Motion�N���X���Afalse�̂Ƃ�MotionByBone�N���X���g�p����
	union {
		Motion* m_motion = nullptr;	//���̏�Ԏ��̃A�j���[�V����
		MotionByBone* m_motionByBone;
	};
	bool m_repeat = true;	//�A�j���[�V���������s�[�g���邩�ۂ�
public:
	State() noexcept = default;
	State(const String& name) noexcept : m_name(name) {}
	~State() noexcept {
		if (m_useFrameMotion) delete m_motion;
		else delete m_motionByBone;
		m_motion = nullptr;
	}
public:
	/// <summary>
	/// ��Ԗ���ݒ肷��
	/// </summary>
	void SetName(const String& name) noexcept { m_name = name; }
	/// <summary>
	/// ���̏�Ԃ̃A�j���[�V���������s�[�g�Đ����邩�ۂ���ݒ肷��
	/// </summary>
	constexpr void SetRepeat(bool repeat) noexcept { m_repeat = repeat; }
public:
	/// <summary>
	/// ��Ԗ�"Empty"�A�����p���̃A�j���[�V������ݒ肷��
	/// </summary>
	void SetEmptyMotion(bool useFrameMotion = true) noexcept {
		if (m_motion) {
			if (m_useFrameMotion) delete m_motion;
			else delete m_motionByBone;
		}
		m_useFrameMotion = useFrameMotion;
		if (m_useFrameMotion) m_motion = new Motion();
		else m_motionByBone = new MotionByBone();
		m_name = u"Empty";
	}
	/// <summary>
	/// �A�j���[�V�����t�@�C������A�j���[�V������ݒ肷��
	/// </summary>
	/// <param name="filePath">VMD�t�@�C����</param>
	void SetMotion(const String& filePath, bool useFrameMotion = true) noexcept {
		if (m_motion) {
			if (m_useFrameMotion) delete m_motion;
			else delete m_motionByBone;
		}
		m_useFrameMotion = useFrameMotion;
		if (m_useFrameMotion) m_motion = new Motion(filePath);
		else m_motionByBone = new MotionByBone(filePath);
	}
public:
	/// <summary>
	/// ��Ԗ����擾����
	/// </summary>
	const String& GetName() const noexcept { return m_name; }
	/// <summary>
	/// ���̏�Ԃ̃A�j���[�V���������s�[�g�Đ����邩�ۂ����擾����
	/// </summary>
	constexpr bool GetRepeat() const noexcept { return m_repeat; }
	/// <summary>
	/// Motion�N���X���g�p���Ă���ꍇ��true���AMotionByBone�N���X���g�p���Ă���ꍇ��false��Ԃ�
	/// </summary>
	constexpr bool IsFrameMotion() const noexcept { return m_useFrameMotion; }
public:
	/// <summary>
	/// ���̏�Ԃ̃A�j���[�V���������擾����
	/// </summary>
	template<bool useFrameMotion = true>
	Traits::conditional_t<useFrameMotion, Motion*, MotionByBone*> GetMotion() noexcept {
		if constexpr (useFrameMotion) return m_motion;
		else return m_motionByBone;
	}
};

/// <summary>
/// ��Ԃ̑J�ڏ����̐ݒ��\���\����
/// </summary>
struct Engine::Transition {
	String before;	//�J�ڑO�̏�Ԗ�
	String after;	//�J�ڌ�̏�Ԗ�
	Function<bool(GameObject&)> condition;	//�J�ڏ������Btrue�̂Ƃ��A��Ԃ��J�ڂ���
public:
	bool operator()(GameObject& gObj) noexcept { return condition(gObj); }
	bool operator==(const Transition& rhs) noexcept { return before == rhs.before && after == rhs.after && condition == rhs.condition; }
};

/// <summary>
/// Animator�N���X�Ŏg�p������ԑJ�ڂ��Ǘ�����N���X
/// </summary>
class Engine::AnimationController {
	Vector<Transition> m_transitions;	//��Ԃ̑J�ڏ����̔z��
public:
	/// <summary>
	/// �J�ڌ�̏�Ԗ����擾����
	/// </summary>
	/// <param name="before">���݂̏�Ԗ�</param>
	/// <param name="gObj">Animator�N���X���A�^�b�`����Ă���GameObject</param>
	/// <returns>�J�ڌ�̏�Ԗ��B�����̑J�ڐ悪���݂���Ƃ��A�o�^����������Ԃ̖��O��Ԃ��B</returns>
	const String& GetCurrentMotionName(const String& before, GameObject& gObj) noexcept {
		for (auto& t : m_transitions) {
			if (t.before != before) continue;
			if (t(gObj)) return t.after;
		}
		return before;
	}
	/// <summary>
	/// �J�ڏ�����ǉ�����B
	/// �J�ڑO�ƑJ�ڐ�̏�Ԗ���Animator�N���X�̃C���X�^���X���ɂȂ���Ԗ����ǉ��ł���B
	/// ���łɑ��݂���J�ڑO�ƑJ�ڐ�̑g�ݍ��킹�̏ꍇ�A�������̂ݕύX����
	/// </summary>
	/// <param name="before">�J�ڑO�̏�Ԗ�</param>
	/// <param name="after">�J�ڐ�̏�Ԗ�</param>
	/// <param name="condition">�J�ڏ�����</param>
	void AddTransition(const String& before, const String& after, const Function<bool(GameObject&)>& condition) noexcept {
		for (auto& t : m_transitions) {
			if (t.before == before && t.after == after) {
				t.condition = condition;
				return;
			}
		}
		m_transitions.Add(Transition{ before, after, condition });
	}
	/// <summary>
	/// �J�ڏ������폜����
	/// </summary>
	/// <param name="before">�J�ڑO�̏�Ԗ�</param>
	/// <param name="after">�J�ڐ�̏�Ԗ�</param>
	/// <returns>�폜�ɐ��������Ƃ��Atrue</returns>
	bool RemoveTransition(const String& before, const String& after) noexcept {
		for (auto& t : m_transitions) {
			if (t.before == before && t.after == after) {
				m_transitions.RemoveValue(t);
				return true;
			}
		}
		return false;
	}
};
