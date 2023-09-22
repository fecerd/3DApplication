#if defined(__GNUC__) //&& !defined(__clang__)
module Components;
import :GameObject;
#else
module Components:Animator;
import Components;
import :Animator_Internal;
#endif
import System;
import Common3D;
using namespace System;
using namespace Engine;
using namespace System::Application::Common3D;

Animator::Animator() noexcept : Animator(nullptr) {}
Animator::Animator(GameObject* object) noexcept : Component(object) {
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
	Transform tr{};
	for (size_t i = 0; i < BoneMatrixCount; ++i) m_currentBones.Add(tr);
	m_controller = new AnimationController();
}
Animator::~Animator() noexcept {
	delete m_controller;
	m_controller = nullptr;
	m_currentBones.Clear();
}

void Animator::Update() noexcept {
	GameObject& gObj = GetObject();
	if (!m_currentState || !gObj.HasComponent<MeshFilter>()) return;
	--m_updateCount;
	if (m_updateCount) return;	//m_updateCountが0になるまでなにもしない
	m_updateCount = m_updateCountPerFrame;
	++m_currentFrameNo;
	//状態遷移を確認
	const String& currentName = m_currentState->GetName();
	const String& nextName = m_controller->GetCurrentMotionName(currentName, gObj);
	if (currentName != nextName) {
		ManagedObject<State> nextState = ManagedObject<State>::GetObject(nextName);
		if (nextState) {
			m_currentState = nextState;
			m_currentFrameNo = 0;
			m_end = false;
			for (Transform& t : m_currentBones) t = Transform();
		}
	}
	/// <summary>
	/// フレーム補間位置xをベジェ曲線上の座標によってyに変換する
	/// </summary>
	auto GetBezier = [](uint32_t x, const ControlPoint* controlPoint) -> uint32_t {
		if (x == 0) return 0;
		else if (x >= 127) return 127;
		/*
			ベジェ曲線：f(t) = (127 - 3(c2 - c1))t^3 + (3c2 - 6c1)t^2 + 3c1t (c0 = (0, 0), c3 = (127, 127))
			p[n] = p[n - 1] - (f(p[n - 1] / 127) - x) (p[0] = x)
			pはf(t/127).x == xとなるtに収束する
		*/
		float A = static_cast<float>(127 - 3 * (controlPoint[1].x - controlPoint[0].x));
		float B = static_cast<float>(3 * controlPoint[1].x - 6 * controlPoint[0].x);
		float C = static_cast<float>(3 * controlPoint[0].x);
		float p = static_cast<float>(x);
		for (uint32_t i = 0; i < 5; ++i) {
			float t = p / 127;
			float tt = t * t;
			float f = A * (tt * t) + B * tt + C * t;
			p = p - f + x;
		}
		//f(p / 127).yが求めたい補間係数
		float t = p / 127;
		float tt = t * t;
		return static_cast<uint32_t>(
			(127 - 3 * (controlPoint[1].y - controlPoint[0].y)) * (tt * t)
			+ (3 * controlPoint[1].y - 6 * controlPoint[0].y) * tt
			+ 3 * controlPoint[0].y * t
			);
	};

	if (m_currentState->IsFrameMotion()) {
		//アニメーションのリピート・終了を確認
		Motion* motion = m_currentState->GetMotion();
		if (!motion) return;
		if (motion->GetEndFrameNo() < m_currentFrameNo) {
			if (m_currentState->GetRepeat()) m_currentFrameNo = 0;
			else {
				m_end = true;
				return;
			}
		}
		Frames frames = motion->GetFrames(m_currentFrameNo);
		//ボーンを更新
		MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
		Vector<Bone>& bones = meshFilter.GetBones();	//モデルで定義されたボーン情報
		size_t boneCount = m_currentBones.Count();	//ボーン行列の個数
		if (frames.prev && frames.next) {
			if (frames.prev == frames.next) {	//キーフレームと現在のフレーム番号が一致する(補間しなくてよい)
				for (Bone& nbone : frames.next->bones.Values()) {
					size_t id = meshFilter.GetBoneID(nbone.name);
					if (id >= boneCount) continue;
					Transform& cbone = m_currentBones[id];
					//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
					cbone.LocalPosition.x = nbone.position.x;
					cbone.LocalPosition.y = nbone.position.y;
					cbone.LocalPosition.z = nbone.position.z;
					//}
					cbone.LocalRotation = nbone.rotation;
				}
			}
			else {
				uint32_t t = static_cast<uint32_t>(frames.t * 127);	//フレーム位置を[0, 127]で表す
				for (Bone& nbone : frames.next->bones.Values()) {
					size_t id = meshFilter.GetBoneID(nbone.name);
					if (id >= boneCount) continue;
					Bone* pbone = frames.prev->bones.AtPtr<0>(nbone.name);
					Transform& cbone = m_currentBones[id];
					if (pbone) {	//前フレームのボーンと補間する
						//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
						float xT = GetBezier(t, nbone.xTranslatePoint) / 127.0f;
						cbone.LocalPosition.x = pbone->position.x * (1.0f - xT) + nbone.position.x * xT;
						float yT = GetBezier(t, nbone.yTranslatePoint) / 127.0f;
						cbone.LocalPosition.y = pbone->position.y * (1.0f - yT) + nbone.position.y * yT;
						float zT = GetBezier(t, nbone.zTranslatePoint) / 127.0f;
						cbone.LocalPosition.z = pbone->position.z * (1.0f - zT) + nbone.position.z * zT;
						//}
						float rotT = GetBezier(t, nbone.rotationPoint) / 127.0f;
						cbone.LocalRotation = Quaternion::Slerp(pbone->rotation, nbone.rotation, rotT);
					}
					else {	//初期姿勢と補間する(初期姿勢でないボーンが省略されているとは考えづらいため)
						//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
						float xT = GetBezier(t, nbone.xTranslatePoint) / 127.0f;
						cbone.LocalPosition.x = nbone.position.x * xT;
						float yT = GetBezier(t, nbone.yTranslatePoint) / 127.0f;
						cbone.LocalPosition.y = nbone.position.y * yT;
						float zT = GetBezier(t, nbone.zTranslatePoint) / 127.0f;
						cbone.LocalPosition.z = nbone.position.z * zT;
						//}
						float rotT = GetBezier(t, nbone.rotationPoint) / 127.0f;
						cbone.LocalRotation = Quaternion::Slerp(Quaternion::Identity(), nbone.rotation, rotT);
					}
				}
			}
		}
		else if (!frames.next) {	//最後のフレームより現在のフレーム番号が大きい(通常はありえない)
			return;	//ボーンを更新せずに終了
		}
		else {	//最初のフレームより現在のフレーム番号が小さい
			uint32_t t = static_cast<uint32_t>(frames.t * 127);	//フレーム位置を[0, 127]で表す
			for (Bone& nbone : frames.next->bones.Values()) {
				size_t id = meshFilter.GetBoneID(nbone.name);
				if (id >= boneCount) continue;
				Transform& cbone = m_currentBones[id];
				//初期姿勢と補間する
				//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
				float xT = GetBezier(t, nbone.xTranslatePoint) / 127.0f;
				cbone.LocalPosition.x = nbone.position.x * xT;
				float yT = GetBezier(t, nbone.yTranslatePoint) / 127.0f;
				cbone.LocalPosition.y = nbone.position.y * yT;
				float zT = GetBezier(t, nbone.zTranslatePoint) / 127.0f;
				cbone.LocalPosition.z = nbone.position.z * zT;
				//}
				float rotT = GetBezier(t, nbone.rotationPoint) / 127.0f;
				cbone.LocalRotation = Quaternion::Slerp(Quaternion::Identity(), nbone.rotation, rotT);
			}
		}
	}
	else {
		//アニメーションのリピート・終了を確認
		MotionByBone* motion = m_currentState->GetMotion<false>();
		if (!motion) return;
		if (motion->GetEndFrameNo() < m_currentFrameNo) {
			if (m_currentState->GetRepeat()) {
				m_currentFrameNo = 0;
			}
			else {
				m_end = true;
				return;
			}
		}
		//ボーンを更新
		MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
		Vector<Bone>& bones = meshFilter.GetBones();	//モデルで定義されたボーン情報
		size_t boneCount = m_currentBones.Count();	//ボーン行列の個数
		for (KeyBone& k : motion->GetKeyBones()) {
			size_t count = k.bones.Count();
			if (!count) continue;
			size_t id = meshFilter.GetBoneID(k.bones[0]->name);
			if (id >= boneCount) continue;
			Bone* pbone = nullptr;	//現在のフレーム番号の直前のボーン。nullptrのとき初期姿勢
			Bone* nbone = nullptr;	//現在のフレーム番号の直後のボーン。存在しない場合、一番最後のキーフレームボーン
			for (Bone*& b : k.bones) {
				if (b->id < m_currentFrameNo) {
					pbone = b;
					continue;
				}
				nbone = b;
				break;
			}
			if (!nbone) nbone = k.bones[k.bones.Count() - 1];
			size_t pid = pbone ? pbone->id : 0;
			float ft = nbone->id ? static_cast<float>(m_currentFrameNo - pid) / (nbone->id - pid) : 0;	//フレーム補間位置
			uint32_t t = static_cast<uint32_t>(ft * 127);	//フレーム位置を[0, 127]で表す
			if (pid == nbone->id) {	//補間不要
				Transform& cbone = m_currentBones[id];
				//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
				cbone.LocalPosition.x = nbone->position.x;
				cbone.LocalPosition.y = nbone->position.y;
				cbone.LocalPosition.z = nbone->position.z;
				//}
				cbone.LocalRotation = nbone->rotation;
			}
			else if (!pbone) {	//初期姿勢と補間する
				Transform& cbone = m_currentBones[id];
				//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
				float xT = GetBezier(t, nbone->xTranslatePoint) / 127.0f;
				cbone.LocalPosition.x = nbone->position.x * xT;
				float yT = GetBezier(t, nbone->yTranslatePoint) / 127.0f;
				cbone.LocalPosition.y = nbone->position.y * yT;
				float zT = GetBezier(t, nbone->zTranslatePoint) / 127.0f;
				cbone.LocalPosition.z = nbone->position.z * zT;
				//}
				float rotT = GetBezier(t, nbone->rotationPoint) / 127.0f;
				cbone.LocalRotation = Quaternion::Slerp(Quaternion::Identity(), nbone->rotation, rotT);
			}
			else {
				Transform& cbone = m_currentBones[id];
				//if (bones[id].isIKBone) {	//IKボーンのみ平行移動を計算する
				float xT = GetBezier(t, nbone->xTranslatePoint) / 127.0f;
				cbone.LocalPosition.x = pbone->position.x * (1 - xT) + nbone->position.x * xT;
				float yT = GetBezier(t, nbone->yTranslatePoint) / 127.0f;
				cbone.LocalPosition.y = pbone->position.y * (1 - yT) + nbone->position.y * yT;
				float zT = GetBezier(t, nbone->zTranslatePoint) / 127.0f;
				cbone.LocalPosition.z = pbone->position.z * (1 - zT) + nbone->position.z * zT;
				//}
				float rotT = GetBezier(t, nbone->rotationPoint) / 127.0f;
				cbone.LocalRotation = Quaternion::Slerp(pbone->rotation, nbone->rotation, rotT);
			}
		}
	}

	//IK解決
	SolveIK();
}

void Animator::SolveLookAt(const IK& ik, Vector<Bone>& bones) noexcept {
	const size_t boneCount = Math::Min(bones.Count(), m_currentBones.Count());
	if (boneCount <= ik.id || boneCount <= ik.chainID[0]) return;
	Vector3 pos[2] = {
		bones[ik.chainID[0]].position,
		bones[ik.targetID].position
	};

	DualQuaternion dual;
	size_t id = bones[ik.chainID[0]].parent;	//ルートボーンの親ボーンのIDからセンターボーンまで遡る
	while (id < boneCount) {
		Bone& bone = bones[id];	//ボーンの初期位置, ID
		dual = DualQuaternion::RotateOrigin(bone.rotation, bone.position) * dual;
		id = bone.parent;
	}
	Vector3 t = (bones[ik.id].position + m_currentBones[ik.id].LocalPosition) - (pos[0] * dual);	//アニメーションが適用されたルートボーンの位置→IKボーンの位置を表すベクトル
	Vector3 targetPos = pos[0] + (t * dual.Inverse());	//ベクトルをアニメーションと逆回転させ、ルートボーンの初期位置・姿勢から見たIKボーンの向きとし、IKボーン位置を取得

	Vector3 p1p2 = pos[1] - pos[0];
	Vector3 p1t = targetPos - pos[0];
	Vector3 axis = Vector3::Cross(p1p2, p1t);
	float theta = Vector3::Angle(p1p2, p1t);
	m_currentBones[ik.chainID[0]].LocalRotation = Quaternion::AngleAxis(theta, axis);
	m_currentBones[ik.id].LocalPosition = Vector3::Zero();
	m_currentBones[ik.id].LocalRotation = Quaternion::Identity();
}

//#define _Cosine
//#ifdef _Cosine
//	void Animator::SolveCosine(const IK& ik, Vector<Bone>& bones) noexcept {
//		const size_t boneCount = System::Math::Min(bones.Count(), m_currentBones.Count());
//		if (boneCount <= ik.id || boneCount <= ik.chainID[0] || boneCount <= ik.chainID[1]) return;
//		if (m_currentBones[ik.id].LocalPosition == Vector3::Zero()) return;
//		Vector3 pos[3] = {
//			bones[ik.chainID[1]].transform.LocalPosition,
//			bones[ik.chainID[0]].transform.LocalPosition,
//			bones[ik.targetID].transform.LocalPosition
//		};
//		Vector3 targetPos = bones[ik.id].transform.LocalPosition + m_currentBones[ik.id].LocalPosition;
//		Matrix forward;
//		size_t id = bones[ik.chainID[1]].parent;
//		while (id < boneCount) {
//			Bone& bone = bones[id];
//			forward = m_currentBones[id].World(bone.transform.LocalPosition) * forward;
//			id = bone.parent;
//		}
//		float p1p2m = (pos[1] - pos[0]).Magnitude();
//		float p2p3m = (pos[2] - pos[1]).Magnitude();
//		pos[0] = pos[0] * forward;
//		Vector3 p1t = targetPos - pos[0];
//		float p1tm = p1t.Magnitude();
//		float theta1 = System::Math::AcosDeg((p1p2m * p1p2m + p1tm * p1tm - p2p3m * p2p3m) / (2 * p1p2m * p1tm));
//		float theta2 = System::Math::AcosDeg((p1p2m * p1p2m + p2p3m * p2p3m - p1tm * p1tm) / (2 * p1p2m * p2p3m));
//		Vector3 axis;
//		if (bones[ik.chainID[0]].name.find(u"ひざ")) axis = Vector3::Right();
//		else {
//			axis = Vector3::Cross(pos[1] * forward * m_currentBones[ik.chainID[1]].World(bones[ik.chainID[1]].transform.LocalPosition) - pos[0], p1t);
//		}
//		m_currentBones[ik.chainID[1]].LocalRotation = Quaternion::AngleAxis(theta1, axis);
//		m_currentBones[ik.chainID[0]].LocalRotation = Quaternion::AngleAxis(theta2 - 180.0f, axis);
//		m_currentBones[ik.id].LocalPosition = Vector3::Zero();
//		m_currentBones[ik.id].LocalRotation = Quaternion::Identity();
//	}
//#else
void Animator::SolveCosine(const IK& ik, Vector<Bone>& bones) noexcept {
	const size_t boneCount = Math::Min(bones.Count(), m_currentBones.Count());
	if (boneCount <= ik.id || boneCount <= ik.chainID[0] || boneCount <= ik.chainID[1]) return;
	if (m_currentBones[ik.id].LocalPosition == Vector3::Zero()) return;
	Vector3 pos[3] = {
		bones[ik.chainID[1]].position,	//回転だけするルートボーンの初期位置
		bones[ik.chainID[0]].position,	//中間ボーンの初期位置
		bones[ik.targetID].position	//末端ボーンの初期位置
	};

	DualQuaternion dual;
	size_t id = bones[ik.chainID[1]].parent;	//ルートボーンの親ボーンのIDからセンターボーンまで遡る
	while (id < boneCount) {
		Bone& bone = bones[id];	//ボーンの初期位置, ID
		dual = DualQuaternion::RotateOrigin(bone.rotation, bone.position) * dual;
		id = bone.parent;
	}
	Vector3 t = (bones[ik.id].position + m_currentBones[ik.id].LocalPosition) - (pos[0] * dual);	//アニメーションが適用されたルートボーンの位置→IKボーンの位置を表すベクトル
	Vector3 targetPos = pos[0] + (t * dual.Inverse());	//ベクトルをアニメーションと逆回転させ、ルートボーンの初期位置・姿勢から見たIKボーンの向きとし、IKボーン位置を取得

	Vector3 p1p2 = pos[1] - pos[0];
	Vector3 p1t = targetPos - pos[0];
	Vector3 p2p3 = pos[2] - pos[1];
	float p1p2m = p1p2.Magnitude();
	float p1tm = p1t.Magnitude();
	float p2p3m = p2p3.Magnitude();
	float gamma = Vector3::Angle(p1p2, p1t);	//(ルート→中間)と(ルート→IKボーン)のベクトル間の角度(°)
	float beta = Math::AcosDeg((p1p2m * p1p2m + p1tm * p1tm - p2p3m * p2p3m) / (2 * p1p2m * p1tm));	//ルート、中間、IKボーンが三角形をなすときのルートの角度(°)
	Vector3 axis = Vector3::Cross(p1p2, p1t);	//回転軸は(ルート→中間)と(ルート→IKボーン)のベクトルがなす平面の法線
	//回転方向を補正する(暫定的?)
	if (Vector3::Dot(axis, Vector3::Right()) < 0) {
		gamma *= -1;
		axis = -axis;
	}
	if (bones[ik.chainID[0]].name.find(u"ひざ")) axis = Vector3::Right();
	float alpha = Math::AcosDeg((p1p2m * p1p2m + p2p3m * p2p3m - p1tm * p1tm) / (2 * p1p2m * p2p3m));	//ルート、中間、IKボーンが三角形をなすときの中間の角度(°)
	//ルートと中間の回転を上書き
	m_currentBones[ik.chainID[1]].LocalRotation = Quaternion::AngleAxis(beta + gamma, axis);
	m_currentBones[ik.chainID[0]].LocalRotation = Quaternion::AngleAxis(alpha - 180.0f, axis);
	//IKボーンの位置をリセット(GetBoneMatrix関数への影響を考慮して)
	m_currentBones[ik.id].LocalPosition = Vector3::Zero();
	m_currentBones[ik.id].LocalRotation = Quaternion::Identity();
}
//#endif

void Animator::SolveCCDIK(const IK& ik, Vector<Bone>& bones) noexcept {

}

void Animator::SolveIK() noexcept {
	GameObject& gObj = static_cast<GameObject&>(GetObject());
	if (!gObj.HasComponent<MeshFilter>()) return;
	MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
	Vector<IK>& iks = meshFilter.GetIKs();	//モデルで定義されたIK情報
	Vector<Bone> bones = meshFilter.GetBones();	//モデルで定義されたボーン情報
	for (IK& ik : iks) {
		size_t childCount = ik.chainID.Count();
		switch (childCount) {
		case 0:
			break;
		case 1:
			SolveLookAt(ik, bones);
			break;
		case 2:
			SolveCosine(ik, bones);
			break;
		default:
			SolveCCDIK(ik, bones);
			break;
		}
	}
}

bool Animator::GetBoneMatrix(Matrix(&out)[BoneMatrixCount]) noexcept {
	auto f = [&out] {
		Matrix identity{};
		for (size_t i = 0; i < BoneMatrixCount; ++i) out[i] = identity;
		return false;
	};
	GameObject& gObj = static_cast<GameObject&>(GetObject());
	if (!m_currentState || !gObj.HasComponent<MeshFilter>()) return f();
	MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
	Vector<Bone>& bones = meshFilter.GetBones();	//モデルで定義されているボーン情報
	const size_t boneCount = bones.Count();
	//rootとなるボーンを検索
	Bone* root = nullptr;
	for (Bone& b : bones) {
		//親ボーンのIDがボーン数より大きい && IKボーンでない && IDがボーン数以下 => rootボーン
		if (b.parent >= boneCount && !b.isIKBone && b.id < boneCount) {
			root = &b;
			break;
		}
	}
	if (!root) return f();
	//rootボーンに対応するアニメーションのボーンから行列を取得
	//rootボーンを原点に移す -> アニメーションのスケール・回転・移動 -> 原点からもとの座標に戻す		
	out[root->id] = m_currentBones[root->id].LocalOrigin(root->position);
	GetBoneMatrix_Internal(out, *root, bones.Items(), boneCount);
	Matrix identity{};
	for (size_t i = boneCount; i < BoneMatrixCount; ++i) out[i] = identity;
	return true;
}

Animator* Animator::Clone(GameObject* object) noexcept {
	Animator* ret = new Animator(object);
	*ret->m_controller = *m_controller;
	return ret;
}
