export module Components:Animator;
import :Component;
import :Declaration;
import :Transform;
export import :Animator_Internal;
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
/// アニメーションを管理するコンポーネントクラス
/// </summary>
export class Engine::Animator : public Engine::Component {
	ManagedObject<State> m_currentState;	//現在の状態
	size_t m_currentFrameNo = 0;	//現在のフレーム番号
	Vector<Transform> m_currentBones;	//現在のボーンの姿勢(ボーン行列)
	bool m_end = false;	//現在の状態のアニメーションが終了している場合、true(アニメーション計算をスキップする)
	size_t m_updateCountPerFrame = 1;	//次のフレーム番号に移るために必要なUpdate関数の呼び出し回数(0にはならない)
	size_t m_updateCount = 1;	//Update関数の呼び出しごとにデクリメントされ、0になったとき次のフレーム番号に移る
	AnimationController* m_controller = nullptr;	//状態遷移を管理するコントローラ
	Resource m_animationResource;
	Heap m_animationHeap;
public:
	Animator() noexcept;
	Animator(GameObject* object) noexcept;
	~Animator() noexcept;
public: /* モーション登録関連 */
	/// <summary>
	/// 状態を登録する
	/// </summary>
	/// <param name="filePath">アニメーションファイルの絶対パス</param>
	/// <param name="name">状態名</param>
	/// <param name="repeat">アニメーションをリピート再生するか否か</param>
	/// <returns>登録に成功した場合、true。すでに同じ名前の状態が存在するとき、登録に失敗する</returns>
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
	/// 指定した状態のアニメーションをリピート再生するか否かを設定する
	/// </summary>
	/// <param name="name">状態名</param>
	/// <param name="repeat">アニメーションをリピート再生するか否か</param>
	/// <returns>設定に成功した場合、true</returns>
	bool SetRepeat(const String& name, bool repeat) noexcept {
		ManagedObject<State> state = ManagedObject<State>::GetObject(name);
		if (!state) return false;
		state->SetRepeat(repeat);
		return true;
	}
public:
	/// <summary>
	/// 次のフレーム番号に移るために必要なUpdate関数の呼び出し回数を設定する。
	/// 0は指定できない(1を指定した場合と同じになる)
	/// </summary>
	void SetUpdateCountPerFrame(size_t count) noexcept {
		m_updateCountPerFrame = count ? count : 1;
		if (m_updateCount > m_updateCountPerFrame) m_updateCount = m_updateCountPerFrame;
	}
public:
	/// <summary>
	/// AnimationControllerを取得する
	/// </summary>
	AnimationController& GetAnimationController() noexcept { return *m_controller; }
	/// <summary>
	/// 現在の状態のアニメーションが終了しているか否かを取得する
	/// </summary>
	bool IsEnd() const noexcept { return m_end; }
private:
	/// <summary>
	/// 再帰的に呼び出してボーン行列を更新する
	/// </summary>
	/// <param name="mat">ボーン行列の配列</param>
	/// <param name="root">親ボーン</param>
	/// <param name="bones">モデルで定義されているボーン情報の配列</param>
	/// <param name="boneCount">モデルで定義されているボーンの個数</param>
	void GetBoneMatrix_Internal(Matrix* mat, Bone& root, Bone* bones, size_t boneCount) noexcept {
		for (size_t i = 0; i < boneCount; ++i) {
			Bone& b = bones[i];
			if (b.parent != root.id || b.id >= boneCount) continue;
			//このボーンの行列に親ボーンの行列を乗算する
			mat[b.id] = mat[root.id] * m_currentBones[b.id].LocalOrigin(b.position);
			GetBoneMatrix_Internal(mat, b, bones, boneCount);
		}
	}
public:
	/// <summary>
	/// 現在の状態のアニメーションを更新する。フレームが進んだとき、ボーンの姿勢が再計算される
	/// </summary>
	void Update() noexcept;
	/// <summary>
	/// ボーン行列の配列を取得する。
	/// 使用されないボーンは単位行列でクリアされる
	/// </summary>
	/// <param name="out">ボーン行列の出力先</param>
	/// <returns>ボーン配列を取得した場合、true。falseを返すとき、引数はすべて単位行列にクリアされる</returns>
	bool GetBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept;
	/// <summary>
	/// 単位行列でクリアされたボーン行列を取得する
	/// </summary>
	static void GetIdentityBoneMatrix(Matrix(&out)[Common3D::BoneMatrixCount]) noexcept {
		Matrix identity{};
		for (size_t i = 0; i < Common3D::BoneMatrixCount; ++i) out[i] = identity;
	}
private:
	/// <summary>
	/// 1リンクIKを計算する
	/// </summary>
	/// <param name="ik">IKボーン情報</param>
	/// <param name="bones">モデルで定義されたボーン情報の配列</param>
	void SolveLookAt(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// 2リンクIKを余弦定理を利用して計算する
	/// </summary>
	/// <param name="ik">IKボーン情報</param>
	/// <param name="bones">モデルで定義されたボーン情報の配列</param>
	void SolveCosine(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// 3リンク以上のIKをCCDIKを用いて計算する
	/// </summary>
	/// <param name="ik">IKボーン情報</param>
	/// <param name="bones">モデルで定義されたボーン情報の配列</param>
	void SolveCCDIK(const IK& ik, Vector<Bone>& bones) noexcept;
	/// <summary>
	/// すべてのIKボーンを計算し、ボーン姿勢を更新する
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
	uint32_t GetTypeID() const noexcept override { return GetID<Animator>(); }
	Animator* Clone(GameObject* object) noexcept override;
};
