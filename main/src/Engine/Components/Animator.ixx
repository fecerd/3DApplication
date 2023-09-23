export module Animator;
export import Component;
export import Transform;
export import MeshFilter;
import System;
import Common3D;
import Bone;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

//内部クラス
export namespace Engine::Internal {
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

	/// @brief キーフレームのボーン情報をボーン単位で管理する構造体
	struct KeyBone {
		size_t hash = 0;
		VectorBase<Bone*> bones;
	public:
		KeyBone() noexcept = default;
		KeyBone(const KeyBone&) noexcept = delete;
		KeyBone(KeyBone&& arg) noexcept = default;
		~KeyBone() noexcept {
			bones.DeleteAll();
			hash = 0;
		}
	public:
		KeyBone& operator=(const KeyBone&) noexcept = delete;
		KeyBone& operator=(KeyBone&& rhs) noexcept = default;
	};

	/// @brief ボーン単位でアニメーション情報を管理するクラス
	class MotionByBone {
		/// @brief キーフレームが打たれているボーン情報をボーン単位でまとめたものの動的配列
		VectorBase<KeyBone> m_keyBones;
		/// @brief キーフレームのボーン情報が絶対位置のとき、true。初期姿勢からの相対変化のとき、false
		bool m_isAbsoluteTransform = false;
		/// @brief すべてのキーフレームの中で一番最後のフレーム番号
		size_t m_maxFrameNo = 0;
	public:
		MotionByBone() noexcept = default;
		MotionByBone(const MotionByBone&) noexcept = delete;
		MotionByBone(MotionByBone&&) noexcept = default;
		/// @brief VMDファイルからモーションデータを読み取る
		/// @param filePath VMDファイルのパス
		MotionByBone(const String& filePath) noexcept {
			m_isAbsoluteTransform = false;	//VMDファイルは初期姿勢からの相対変化が記録されている
			Formats::VMD vmd(filePath);
			//VMDMotionはあるキーフレームのあるボーン状態を表す構造体
			for (Formats::VMDMotion& mot : vmd.motions) {
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
					m_keyBones.Add(System::move(tmp));
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
		VectorBase<KeyBone>& GetKeyBones() noexcept { return m_keyBones; }
		size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
		bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
	public:
		MotionByBone& operator=(const MotionByBone&) noexcept = delete;
		MotionByBone& operator=(MotionByBone&&) noexcept = default;
	};

	/// @brief アニメーションデータの1キーフレームを表す構造体
	struct Frame {
		/// @brief ボーン名でアクセスできるこのキーフレームに含まれるボーン情報
		HashMap<String, Bone> bones = 3;
		/// @brief フレーム番号
		size_t id = System::MAX_VALUE<size_t>;
	public:
		Frame() noexcept = default;
		/// @brief HashMapがコピーできないため、delete
		Frame(const Frame&) noexcept = delete;
		Frame(Frame&&) noexcept = default;
		~Frame() noexcept = default;
	public:
		Frame& operator=(const Frame&) noexcept = delete;
		Frame& operator=(Frame&&) noexcept = default;
	};

	/// @brief あるフレーム番号の前のキーフレームと次のキーフレームを含む構造体
	struct Frames {
		/// @brief 前のキーフレーム
		Frame* prev = nullptr;
		/// @brief 次のキーフレーム
		Frame* next = nullptr;
		/// @brief 0を前のキーフレーム、1を次のキーフレームとしたときの現在のフレーム位置
		float t = 0;
	};

	/// @brief アニメーション情報を管理するクラス
	class Motion {
		/// @brief すべてのキーフレームの配列(順序は不定)
		VectorBase<Frame*> m_frames;
		/// @brief キーフレームのボーン情報が絶対位置のとき、true。初期姿勢からの相対変化のとき、false
		bool m_isAbsoluteTransform = false;
		/// @brief すべてのキーフレームの中で一番最後のフレーム番号
		size_t m_maxFrameNo = 0;
	public:
		Motion() noexcept = default;
		Motion(const Motion&) noexcept = delete;
		Motion(Motion&&) noexcept = default;
		/// @brief VMDファイルからモーション情報を読み取る
		/// @param filePath VMDファイルのパス
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
				frame->bones.Insert(mot.boneName, System::move(bone));
			}
		}
		~Motion() noexcept { m_frames.DeleteAll(); }
	public:
		/// @brief 指定したフレーム番号の前後のキーフレームを取得する
		/// @param currentFrame フレーム番号
		/// @return 指定したフレーム番号から設定されたFrames構造体。
		/// @retval 指定したフレーム番号と一致するキーフレームが存在するとき、前後のキーフレームは一致し、フレーム補間位置は0となる。
		/// @retval 指定したフレーム番号が最初のキーフレーム番号より小さいとき、前のキーフレームはnullptrとなる。
		/// @retval 指定したフレーム番号が最後のキーフレーム番号より大きいとき、次のキーフレームはnullptrとなり、フレーム補間位置は0となる
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
		/// @brief 最後のキーフレーム番号を取得する
		size_t GetEndFrameNo() const noexcept { return m_maxFrameNo; }
		/// @brief キーフレームのボーン情報の格納方法を取得する
		/// @retval 絶対位置の場合、true
		/// @retval 初期姿勢からの相対変化の場合、false
		bool IsAbsoluteTransform() const noexcept { return m_isAbsoluteTransform; }
	public:
		Motion& operator=(const Motion&) noexcept = delete;
		Motion& operator=(Motion&&) noexcept = default;
	};

	/// @brief Animationクラスが管理する状態を表すクラス
	class State {
		/// @brief この状態の名前(Animationクラスのインスタンス内で一意)
		String m_name;
		/// @brief trueのときMotionクラスを、falseのときMotionByBoneクラスを使用する
		bool m_useFrameMotion = true;
		/// @brief この状態時のアニメーション
		union MotionUnion {
			Motion* motion = nullptr;
			MotionByBone* motionByBone;
		} m_motions;
		/// @brief アニメーションをリピートするか否か
		bool m_repeat = true;
	public:
		State() noexcept = default;
		State(const State&) noexcept = delete;
		State(State&&) noexcept = delete;
		State(const String& name) noexcept : m_name(name) {}
		~State() noexcept {
			if (m_useFrameMotion) delete m_motions.motion;
			else delete m_motions.motionByBone;
			m_motions.motion = nullptr;
		}
	public:
		void SetName(const String& name) noexcept { m_name = name; }
		constexpr void SetRepeat(bool repeat) noexcept { m_repeat = repeat; }
	public:
		/// @brief 状態名"Empty"、初期姿勢のアニメーションを設定する
		/// @param useFrameMotion Motionクラスを使用するとき、true
		void SetEmptyMotion(bool useFrameMotion = true) noexcept {
			if (m_motions.motion) {
				if (m_useFrameMotion) delete m_motions.motion;
				else delete m_motions.motionByBone;
			}
			m_useFrameMotion = useFrameMotion;
			if (m_useFrameMotion) m_motions.motion = new Motion();
			else m_motions.motionByBone = new MotionByBone();
			m_name = u"Empty";
		}
		/// @brief VMDファイルからアニメーション情報を読み取る
		/// @param filePath VMDファイル名
		/// @param useFrameMotion Motionクラスを使用するとき、true
		void SetMotion(const String& filePath, bool useFrameMotion = true) noexcept {
			if (m_motions.motion) {
				if (m_useFrameMotion) delete m_motions.motion;
				else delete m_motions.motionByBone;
			}
			m_useFrameMotion = useFrameMotion;
			if (m_useFrameMotion) m_motions.motion = new Motion(filePath);
			else m_motions.motionByBone = new MotionByBone(filePath);
		}
	public:
		const String& GetName() const noexcept { return m_name; }
		constexpr bool GetRepeat() const noexcept { return m_repeat; }
		constexpr bool IsFrameMotion() const noexcept { return m_useFrameMotion; }
	public:
		/// @brief この状態のアニメーション情報を取得する
		/// @tparam useFrameMotion Motionクラスを使用している場合、true
		/// @retval useFrameMotionがtrueのとき、Motionクラスのポインタ
		/// @retval useFrameMotionがfalseのとき、MotionByBoneクラスのポインタ
		template<bool useFrameMotion = true>
		Traits::conditional_t<useFrameMotion, Motion*, MotionByBone*> GetMotion() noexcept {
			if constexpr (useFrameMotion) return m_motions.motion;
			else return m_motions.motionByBone;
		}
	public:
		State& operator=(const State&) noexcept = delete;
		State& operator=(State&&) noexcept = delete;
	};
}

//GameObject依存の内部クラス
export namespace Engine::Internal {
	/// @brief GameObjectに依存しないTransitionの機能を実装した構造体
	struct Transition_Internal {
		/// @brief 遷移前の状態名
		String before;
		/// @brief 遷移後の状態名
		String after;
	public:
		Transition_Internal() noexcept = default;
		Transition_Internal(const Transition_Internal&) noexcept = default;
		Transition_Internal(Transition_Internal&&) noexcept = default;
		Transition_Internal(const String& before, const String& after) noexcept
			: before(before), after(after) {}
		~Transition_Internal() noexcept = default;
	public:
		bool operator==(const Transition_Internal& rhs) noexcept {
			return before == rhs.before && after == rhs.after;
		}
		Transition_Internal& operator=(const Transition_Internal&) noexcept = default;
		Transition_Internal& operator=(Transition_Internal&&) noexcept = default;
	};

	/// @brief 状態の遷移条件の設定を表す構造体
	template<class GameObject>
	struct Transition_Impl : public Transition_Internal {
		/// @brief 遷移条件式。trueのとき、状態が遷移する
		Function<bool(GameObject&)> condition;
	public:
		Transition_Impl() noexcept {}
		Transition_Impl(const String& before, const String& after, const Function<bool(GameObject&)>& condition) noexcept
			: Transition_Internal(before, after), condition(condition) {}
		~Transition_Impl() noexcept {}
	public:
		bool operator()(GameObject& gObj) noexcept { return condition(gObj); }
	};
}
using namespace Engine::Internal;

//AnimationController
export namespace Engine {
	/// @brief Animatorクラスで使用される状態遷移を管理するクラス
	template<class GameObject>
	class AnimationController_Impl {
		/// @brief 状態の遷移条件の配列
		VectorBase<Transition_Impl<GameObject>> m_transitions;
	public:
		/// @brief 遷移後の状態名を取得する
		/// @param before 現在の状態名
		/// @param gObj AnimatorクラスがアタッチされているGameObject
		/// @return 遷移後の状態名。複数の遷移先が存在するとき、登録順が早い状態の名前を返す。
		const String& GetCurrentMotionName(const String& before, GameObject& gObj) noexcept {
			for (auto& t : m_transitions) {
				if (t.before != before) continue;
				if (t(gObj)) return t.after;
			}
			return before;
		}
		/// @brief 遷移条件を追加する。
		/// @param before 遷移前の状態名
		/// @param after 遷移先の状態名
		/// @param condition 遷移条件式
		/// @details 遷移前と遷移先の状態名はAnimatorクラスのインスタンス内にない状態名も追加できる。
		///			 すでに存在する遷移前と遷移先の組み合わせの場合、条件式のみ変更する。
		void AddTransition(const String& before, const String& after, const Function<bool(GameObject&)>& condition) noexcept {
			for (auto& t : m_transitions) {
				if (t.before == before && t.after == after) {
					t.condition = condition;
					return;
				}
			}
			m_transitions.Add(Transition_Impl<GameObject>{ before, after, condition });
		}
		/// @brief 遷移条件を削除する
		/// @param before 遷移前の状態名
		/// @param after 遷移先の状態名
		/// @return 削除に成功したとき、true
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
}

//Animator_NoObj
export namespace Engine {
	class Animator_NoObj {
		/// @brief 現在の状態
		ManagedObject<State> m_currentState;
		/// @brief 現在のフレーム番号
		size_t m_currentFrameNo = 0;
		/// @brief 現在のボーンの姿勢(ボーン行列)
		Vector<Transform_NoObj> m_currentBones;
		/// @brief 現在の状態のアニメーションが終了している場合、true(アニメーション計算をスキップする)
		bool m_end = false;
		/// @brief 次のフレーム番号に移るために必要なUpdate関数の呼び出し回数(0にはならない)
		size_t m_updateCountPerFrame = 1;
		/// @brief Update関数の呼び出しごとにデクリメントされ、0になったとき次のフレーム番号に移る
		size_t m_updateCount = 1;
	private:
		/// @brief シェーダに渡されるボーン行列などのリソース
		Resource m_animationResource;
		/// @brief リソースへのDescriptorを配置するHeap
		Heap m_animationHeap;
	protected:
		static List<ManagedObject<State>>& GetStates() noexcept {
			static List<ManagedObject<State>> ret;
			return ret;
		}
	public:
		Animator_NoObj() noexcept {
			//初期姿勢として、"Empty"を追加する
			ManagedObject<State> empty = ManagedObject<State>::GetObject(u"Empty");
			if (!empty) {
				State* tmp = new State(u"Empty");
				tmp->SetEmptyMotion();
				tmp->SetRepeat(false);
				GetStates().PushBack(ManagedObject<State>(u"Empty", tmp));
				empty = ManagedObject<State>::GetObject(u"Empty");
			}
			m_currentState = empty;
			//ボーン姿勢をボーン行列の個数だけ追加する
			m_currentBones.Reserve(BoneMatrixCount);
			Transform_NoObj tr{};
			for (size_t i = 0; i < BoneMatrixCount; ++i) m_currentBones.Add(tr);
//			m_controller = new AnimationController();
		}
		Animator_NoObj(const Animator_NoObj&) noexcept = delete;
		Animator_NoObj(Animator_NoObj&&) noexcept = delete;
		~Animator_NoObj() noexcept {
			m_currentBones.Clear();
		}
	public:/* モーション登録関連 */
		/// @brief モーションを持つ状態を登録する。
		/// @param filePath VMDファイルのパス
		/// @param name 状態名
		/// @param repeat アニメーションをリピート再生する場合、true
		/// @retval 登録に成功した場合、true。
		/// @retval 登録に失敗した場合、false。すでに同じ名前の状態が存在するとき、登録に失敗する。
		bool SetMotion(const String& filePath, const String& name, bool repeat = true) noexcept {
			if (ManagedObject<State>::GetObject(name)) return false;
			State* tmp = new State(name);
			tmp->SetMotion(filePath);
			tmp->SetRepeat(repeat);
			GetStates().PushBack(ManagedObject<State>(name, tmp));
			return true;
		}
		/// @brief モーションを持つ状態を登録する。(MotionByBoneクラスを使用する)
		/// @param filePath VMDファイルのパス
		/// @param name 状態名
		/// @param repeat アニメーションをリピート再生する場合、true
		/// @retval 登録に成功した場合、true。
		/// @retval 登録に失敗した場合、false。すでに同じ名前の状態が存在するとき、登録に失敗する。
		bool SetMotionByBone(const String& filePath, const String& name, bool repeat = true) noexcept {
			if (ManagedObject<State>::GetObject(name)) return false;
			State* tmp = new State(name);
			tmp->SetMotion(filePath, false);
			tmp->SetRepeat(repeat);
			GetStates().PushBack(ManagedObject<State>(name, tmp));
			return true;
		}
		/// @brief 指定した状態のアニメーションをリピート再生するか否かを設定する
		/// @param name 状態名
		/// @param repeat アニメーションをリピート再生するとき、true
		/// @retval 設定に成功した場合、true。
		/// @retval 存在しない状態名を指定したとき、false。
		bool SetRepeat(const String& name, bool repeat) noexcept {
			ManagedObject<State> state = ManagedObject<State>::GetObject(name);
			if (!state) return false;
			state->SetRepeat(repeat);
			return true;
		}
	public:
		/// @brief 次のフレーム番号に移るために必要なUpdate関数の呼び出し回数を設定する。
		/// @param count 必要な呼び出し回数。0を指定した場合、1として扱う。
		void SetUpdateCountPerFrame(size_t count) noexcept {
			m_updateCountPerFrame = count ? count : 1;
			if (m_updateCount > m_updateCountPerFrame) m_updateCount = m_updateCountPerFrame;
		}
	public:
		/// @brief 現在の状態のアニメーションが終了しているか調べる。
		/// @return 終了している場合、true。
		bool IsEnd() const noexcept { return m_end; }
	private:
		/// @brief 再帰的に呼び出してボーン行列を更新する。
		/// @param mat ボーン行列の配列
		/// @param root 親ボーン
		/// @param bones モデルで定義されているボーン情報の配列
		/// @param boneCount モデルで定義されているボーンの個数
		void GetBoneMatrix_Internal(Matrix* mat, const Bone& root, const Bone* bones, size_t boneCount) noexcept {
			for (size_t i = 0; i < boneCount; ++i) {
				const Bone& b = bones[i];
				if (b.parent != root.id || b.id >= boneCount) continue;
				//このボーンの行列に親ボーンの行列を乗算する
				mat[b.id] = mat[root.id] * m_currentBones[b.id].LocalOrigin(b.position);
				GetBoneMatrix_Internal(mat, b, bones, boneCount);
			}
		}
	protected:
		bool GetBoneMatrix(const VectorBase<Bone>& modelBones, Matrix(&out)[Common3D::BoneMatrixCount]) noexcept;
	public:
		bool IsUpdateFrame() noexcept;
		const String& GetCurrentStateName() const noexcept {
			if (!m_currentState) return String::Empty();
			else return m_currentState->GetName();
		}
		void Update(const String& nextName, MeshFilter_NoObj& meshFilter) noexcept;
	private:
		/// @brief 1リンクIKを計算する。
		/// @param ik IKボーン情報
		/// @param bones モデルで定義されたボーン情報の配列
		void SolveLookAt(const IK& ik, Vector<Bone>& bones) noexcept;
		/// @brief 2リンクIKを余弦定理を利用して計算する。
		/// @param ik IKボーン情報
		/// @param bones モデルで定義されたボーン情報の配列
		void SolveCosine(const IK& ik, Vector<Bone>& bones) noexcept;
		/// @brief 3リンク以上のIKをCCDIKを用いて計算する。
		/// @param ik IKボーン情報
		/// @param bones モデルで定義されたボーン情報の配列
		void SolveCCDIK(const IK& ik, Vector<Bone>& bones) noexcept;
		/// @brief すべてのIKボーンを計算し、ボーン姿勢を更新する。
		void SolveIK(MeshFilter_NoObj& meshFilter) noexcept;
	public:
		Heap GetHeap() const noexcept { return m_animationHeap; }
		void UpdateHeap(const VectorBase<Bone>& modelBones) noexcept {
			AnimationResourceDesc desc;
			GetBoneMatrix(modelBones, desc.bone);
			if (!m_animationResource) {
				m_animationResource = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
			}
			else m_animationResource.UpdateResource(desc.GetData(), desc.GetCount());
			if (!m_animationHeap) {
				m_animationHeap = CreateHeap(String::Empty(), HeapType::Animation);
				m_animationHeap.SetView(0, ViewFormat::CBV, m_animationResource);
			}
		}
	};
}

//Animator
export namespace Engine {
	template<class GameObject>
	class Animator_Impl : public Component_Impl<GameObject>, public Animator_NoObj {
		/// @brief 状態遷移を管理するコントローラ
		AnimationController_Impl<GameObject>* m_controller = nullptr;
	public:
		Animator_Impl() noexcept : Animator_Impl(nullptr) {}
		Animator_Impl(const Animator_Impl&) noexcept = delete;
		Animator_Impl(Animator_Impl&&) noexcept = delete;
		Animator_Impl(GameObject* object) noexcept : Component_Impl<GameObject>(object), Animator_NoObj() {
			m_controller = new AnimationController_Impl<GameObject>();
		}
		virtual ~Animator_Impl() noexcept {
			delete m_controller;
			m_controller = nullptr;
		}
	public:
		AnimationController_Impl<GameObject>& GetAnimationController() noexcept { return *m_controller; }
	public:
		/// @brief 現在の状態のアニメーションを更新する。フレームが進んだとき、ボーンの姿勢が再計算される。
		void Update() noexcept {
			if (!IsUpdateFrame()) return;
			GameObject& gObj = Component_Impl<GameObject>::GetObject();
			const String& currentName = GetCurrentStateName();
			const String& nextName = m_controller->GetCurrentMotionName(currentName, gObj);
			Animator_NoObj::Update(nextName, gObj.template GetComponent<MeshFilter_Impl<GameObject>>());
		}
		/// @brief ボーン行列の配列を取得する。
		/// @param out ボーン行列の出力先
		/// @details 使用されないボーンは単位行列でクリアされる。
		/// @retval ボーン配列を取得した場合、true。
		/// @retval falseを返したとき、引数はすべて単位行列にクリアされる。
		bool GetBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept {
			GameObject& gObj = Component_Impl<GameObject>::GetObject();
			if (!gObj.template HasComponent<MeshFilter_Impl<GameObject>>()) return false;
			MeshFilter_Impl<GameObject>& meshFilter = gObj.template GetComponent<MeshFilter_Impl<GameObject>>();
			return Animator_NoObj::GetBoneMatrix(meshFilter.GetBones(), out);
		}
		/// @brief 単位行列でクリアされたボーン行列を取得する
		/// @param out ボーン行列の出力先
		static void GetIdentityBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept {
			Matrix identity{};
			for (size_t i = 0; i < Common3D::BoneMatrixCount; ++i) out[i] = identity;
		}
	public:/* Component override */
		Type GetType() const noexcept override { return Type::CreateType<Animator_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"Animator Component"); }
		uint32_t GetTypeID() const noexcept override { return GetID<Animator_Impl<GameObject>>(); }
		Animator_Impl<GameObject>* Clone(GameObject* object) noexcept override {
			Animator_Impl<GameObject>* ret = new Animator_Impl<GameObject>(object);
			if (ret->m_controller && m_controller) {
				*ret->m_controller = *m_controller;
			}
			return ret;
		}
	public:
		void UpdateHeap() noexcept {
			GameObject& gObj = Component_Impl<GameObject>::GetObject();
			if (!gObj.template HasComponent<MeshFilter_Impl<GameObject>>()) return;
			MeshFilter_Impl<GameObject>& meshFilter = gObj.template GetComponent<MeshFilter_Impl<GameObject>>();
			Animator_NoObj::UpdateHeap(meshFilter.GetBones());
		}
	public:
		Animator_Impl& operator=(const Animator_Impl&) noexcept = delete;
		Animator_Impl& operator=(Animator_Impl&&) noexcept = delete;
	};
}
