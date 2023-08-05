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
/// キーフレームのボーン情報をボーン単位で管理する構造体
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
/// ボーン単位でアニメーション情報を管理するクラス
/// </summary>
class Engine::MotionByBone {
	Vector<KeyBone> m_keyBones;	//キーフレームが打たれているボーン情報をボーン単位でまとめたものの動的配列
	bool m_isAbsoluteTransform = false;	//キーフレームのボーン情報が絶対位置のとき、true。初期姿勢からの相対変化のとき、false
	size_t m_maxFrameNo = 0;	//すべてのキーフレームの中で一番最後のフレーム番号
public:
	MotionByBone() noexcept = default;
	MotionByBone(const MotionByBone&) noexcept = delete;
	MotionByBone(MotionByBone&&) noexcept = default;
	MotionByBone(const String& filePath) noexcept {
		m_isAbsoluteTransform = false;	//VMDファイルは初期姿勢からの相対変化が記録されている
		Formats::VMD vmd(filePath);
		for (Formats::VMDMotion& mot : vmd.motions) {	//VMDMotionはあるキーフレームのあるボーン状態を表す構造体
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
			//ボーン情報を変換して格納
			Bone* bone = new Bone();
			CreateBoneFromVMDMotion(*bone, mot);
			keyBone->bones.Add(bone);
			//最後のフレーム番号を保存
			if (m_maxFrameNo < mot.frameNo) m_maxFrameNo = mot.frameNo;
		}
		for (KeyBone& k : m_keyBones) k.bones.Sort(true, [](Bone* const& lhs, Bone* const& rhs) { return lhs->id <=> rhs->id; });
	}
	~MotionByBone() noexcept = default;
public:
	/// <summary>
	/// ボーン情報をボーン単位でまとめた構造体の動的配列を取得する
	/// </summary>
	Vector<KeyBone>& GetKeyBones() noexcept { return m_keyBones; }
	/// <summary>
	/// 最後のキーフレーム番号を取得する
	/// </summary>
	size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
	/// <summary>
	/// キーフレームのボーン情報の格納方法を取得する
	/// </summary>
	/// <returns>絶対位置の場合はtrueを、初期姿勢からの相対変化の場合はfalseを返す</returns>
	bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
public:
	MotionByBone& operator=(const MotionByBone&) noexcept = delete;
	MotionByBone& operator=(MotionByBone&&) noexcept = default;
};

/// <summary>
/// アニメーションデータの1キーフレームを表す構造体
/// </summary>
struct Engine::Frame {
	HashMap<String, Bone> bones = 3;	//ボーン名でアクセスできるこのキーフレームに含まれるボーン情報。
	size_t id = System::MAX_VALUE<size_t>;	//フレーム番号
};

/// <summary>
/// あるフレーム番号の前のキーフレームと次のキーフレームを含む構造体
/// </summary>
struct Engine::Frames {
	Frame* prev = nullptr;	//前のキーフレーム
	Frame* next = nullptr;	//次のキーフレーム
	float t = 0;	//0を前のキーフレーム、1を次のキーフレームとしたときの現在のフレーム位置
};

/// <summary>
/// アニメーション情報を管理するクラス
/// </summary>
class Engine::Motion {
	Vector<Frame*> m_frames;	//すべてのキーフレームの配列(順序は不定)
	bool m_isAbsoluteTransform = false;	//キーフレームのボーン情報が絶対位置のとき、true。初期姿勢からの相対変化のとき、false
	size_t m_maxFrameNo = 0;	//すべてのキーフレームの中で一番最後のフレーム番号
public:
	Motion() noexcept = default;
	Motion(const Motion&) noexcept = delete;
	Motion(Motion&&) noexcept = default;
	Motion(const String& filePath) noexcept {
		m_isAbsoluteTransform = false;	//VMDファイルは初期姿勢からの相対変化が記録されている
		Formats::VMD vmd(filePath);
		for (Formats::VMDMotion& mot : vmd.motions) {	//VMDMotionはあるキーフレームのあるボーン状態を表す構造体
			Frame* frame = nullptr;
			//すでに追加されているフレームなら取得
			for (Frame*& f : m_frames) {
				if (f->id == mot.frameNo) {
					frame = f;
					break;
				}
			}
			//未追加のフレームなら追加
			if (!frame) {
				frame = new Frame();
				frame->id = mot.frameNo;
				if (frame->id > m_maxFrameNo) m_maxFrameNo = frame->id;
				m_frames.Add(frame);
			}
			//ボーン情報を変換して格納
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
	/// 指定したフレーム番号の前後のキーフレームを取得する
	/// </summary>
	/// <param name="currentFrame">フレーム番号</param>
	/// <returns>
	/// 指定したフレーム番号から設定されたFrames構造体。
	/// 指定したフレーム番号と一致するキーフレームが存在するとき、前後のキーフレームは一致し、フレーム補間位置は0となる。
	/// 指定したフレーム番号が最初のキーフレーム番号より小さいとき、前のキーフレームはnullptrとなる。
	/// 指定したフレーム番号が最後のキーフレーム番号より大きいとき、次のキーフレームはnullptrとなり、フレーム補間位置は0となる
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
	/// 最後のキーフレーム番号を取得する
	/// </summary>
	size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
	/// <summary>
	/// キーフレームのボーン情報の格納方法を取得する
	/// </summary>
	/// <returns>絶対位置の場合はtrueを、初期姿勢からの相対変化の場合はfalseを返す</returns>
	bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
public:
	Motion& operator=(const Motion&) noexcept = delete;
	Motion& operator=(Motion&&) noexcept = default;
};

/// <summary>
/// Animationクラスが管理する状態を表すクラス
/// </summary>
class Engine::State {
	String m_name;	//この状態の名前(Animationクラスのインスタンス内で一意)
	bool m_useFrameMotion = true;	//trueのときMotionクラスを、falseのときMotionByBoneクラスを使用する
	union {
		Motion* m_motion = nullptr;	//この状態時のアニメーション
		MotionByBone* m_motionByBone;
	};
	bool m_repeat = true;	//アニメーションをリピートするか否か
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
	/// 状態名を設定する
	/// </summary>
	void SetName(const String& name) noexcept { m_name = name; }
	/// <summary>
	/// この状態のアニメーションをリピート再生するか否かを設定する
	/// </summary>
	constexpr void SetRepeat(bool repeat) noexcept { m_repeat = repeat; }
public:
	/// <summary>
	/// 状態名"Empty"、初期姿勢のアニメーションを設定する
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
	/// アニメーションファイルからアニメーションを設定する
	/// </summary>
	/// <param name="filePath">VMDファイル名</param>
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
	/// 状態名を取得する
	/// </summary>
	const String& GetName() const noexcept { return m_name; }
	/// <summary>
	/// この状態のアニメーションがリピート再生するか否かを取得する
	/// </summary>
	constexpr bool GetRepeat() const noexcept { return m_repeat; }
	/// <summary>
	/// Motionクラスを使用している場合はtrueを、MotionByBoneクラスを使用している場合はfalseを返す
	/// </summary>
	constexpr bool IsFrameMotion() const noexcept { return m_useFrameMotion; }
public:
	/// <summary>
	/// この状態のアニメーション情報を取得する
	/// </summary>
	template<bool useFrameMotion = true>
	Traits::conditional_t<useFrameMotion, Motion*, MotionByBone*> GetMotion() noexcept {
		if constexpr (useFrameMotion) return m_motion;
		else return m_motionByBone;
	}
};

/// <summary>
/// 状態の遷移条件の設定を表す構造体
/// </summary>
struct Engine::Transition {
	String before;	//遷移前の状態名
	String after;	//遷移後の状態名
	Function<bool(GameObject&)> condition;	//遷移条件式。trueのとき、状態が遷移する
public:
	bool operator()(GameObject& gObj) noexcept { return condition(gObj); }
	bool operator==(const Transition& rhs) noexcept { return before == rhs.before && after == rhs.after && condition == rhs.condition; }
};

/// <summary>
/// Animatorクラスで使用される状態遷移を管理するクラス
/// </summary>
class Engine::AnimationController {
	Vector<Transition> m_transitions;	//状態の遷移条件の配列
public:
	/// <summary>
	/// 遷移後の状態名を取得する
	/// </summary>
	/// <param name="before">現在の状態名</param>
	/// <param name="gObj">AnimatorクラスがアタッチされているGameObject</param>
	/// <returns>遷移後の状態名。複数の遷移先が存在するとき、登録順が早い状態の名前を返す。</returns>
	const String& GetCurrentMotionName(const String& before, GameObject& gObj) noexcept {
		for (auto& t : m_transitions) {
			if (t.before != before) continue;
			if (t(gObj)) return t.after;
		}
		return before;
	}
	/// <summary>
	/// 遷移条件を追加する。
	/// 遷移前と遷移先の状態名はAnimatorクラスのインスタンス内にない状態名も追加できる。
	/// すでに存在する遷移前と遷移先の組み合わせの場合、条件式のみ変更する
	/// </summary>
	/// <param name="before">遷移前の状態名</param>
	/// <param name="after">遷移先の状態名</param>
	/// <param name="condition">遷移条件式</param>
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
	/// 遷移条件を削除する
	/// </summary>
	/// <param name="before">遷移前の状態名</param>
	/// <param name="after">遷移先の状態名</param>
	/// <returns>削除に成功したとき、true</returns>
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
