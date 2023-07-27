module Components:Animator;
import :GameObject;
import :MeshFilter;
using namespace System;
using namespace Engine;
using namespace System::Application::Common3D;

Animator::Animator() noexcept : Animator(nullptr) {}
Animator::Animator(GameObject* object) noexcept : Component(object) {
	//�����p���Ƃ��āA"Empty"��ǉ�����
	ManagedObject<State> empty = ManagedObject<State>::GetObject(u"Empty");
	if (!empty) {
		State* tmp = new State(u"Empty");
		tmp->SetEmptyMotion();
		tmp->SetRepeat(false);
		GetStates().PushBack(ManagedObject<State>(u"Empty", tmp));
		empty = ManagedObject<State>::GetObject(u"Empty");
	}
	m_currentState = empty;
	//�{�[���p�����{�[���s��̌������ǉ�����
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
	if (m_updateCount) return;	//m_updateCount��0�ɂȂ�܂łȂɂ����Ȃ�
	m_updateCount = m_updateCountPerFrame;
	++m_currentFrameNo;
	//��ԑJ�ڂ��m�F
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
	/// �t���[����Ԉʒux���x�W�F�Ȑ���̍��W�ɂ����y�ɕϊ�����
	/// </summary>
	auto GetBezier = [](uint32_t x, const ControlPoint* controlPoint) -> uint32_t {
		if (x == 0) return 0;
		else if (x >= 127) return 127;
		/*
			�x�W�F�Ȑ��Ff(t) = (127 - 3(c2 - c1))t^3 + (3c2 - 6c1)t^2 + 3c1t (c0 = (0, 0), c3 = (127, 127))
			p[n] = p[n - 1] - (f(p[n - 1] / 127) - x) (p[0] = x)
			p��f(t/127).x == x�ƂȂ�t�Ɏ�������
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
		//f(p / 127).y�����߂�����ԌW��
		float t = p / 127;
		float tt = t * t;
		return static_cast<uint32_t>(
			(127 - 3 * (controlPoint[1].y - controlPoint[0].y)) * (tt * t)
			+ (3 * controlPoint[1].y - 6 * controlPoint[0].y) * tt
			+ 3 * controlPoint[0].y * t
			);
	};

	if (m_currentState->IsFrameMotion()) {
		//�A�j���[�V�����̃��s�[�g�E�I�����m�F
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
		//�{�[�����X�V
		MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
		Vector<Bone>& bones = meshFilter.GetBones();	//���f���Œ�`���ꂽ�{�[�����
		size_t boneCount = m_currentBones.Count();	//�{�[���s��̌�
		if (frames.prev && frames.next) {
			if (frames.prev == frames.next) {	//�L�[�t���[���ƌ��݂̃t���[���ԍ�����v����(��Ԃ��Ȃ��Ă悢)
				for (Bone& nbone : frames.next->bones.Values()) {
					size_t id = meshFilter.GetBoneID(nbone.name);
					if (id >= boneCount) continue;
					Transform& cbone = m_currentBones[id];
					//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
					cbone.LocalPosition.x = nbone.position.x;
					cbone.LocalPosition.y = nbone.position.y;
					cbone.LocalPosition.z = nbone.position.z;
					//}
					cbone.LocalRotation = nbone.rotation;
				}
			}
			else {
				uint32_t t = static_cast<uint32_t>(frames.t * 127);	//�t���[���ʒu��[0, 127]�ŕ\��
				for (Bone& nbone : frames.next->bones.Values()) {
					size_t id = meshFilter.GetBoneID(nbone.name);
					if (id >= boneCount) continue;
					Bone* pbone = frames.prev->bones.AtPtr<0>(nbone.name);
					Transform& cbone = m_currentBones[id];
					if (pbone) {	//�O�t���[���̃{�[���ƕ�Ԃ���
						//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
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
					else {	//�����p���ƕ�Ԃ���(�����p���łȂ��{�[�����ȗ�����Ă���Ƃ͍l���Â炢����)
						//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
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
		else if (!frames.next) {	//�Ō�̃t���[����茻�݂̃t���[���ԍ����傫��(�ʏ�͂��肦�Ȃ�)
			return;	//�{�[�����X�V�����ɏI��
		}
		else {	//�ŏ��̃t���[����茻�݂̃t���[���ԍ���������
			uint32_t t = static_cast<uint32_t>(frames.t * 127);	//�t���[���ʒu��[0, 127]�ŕ\��
			for (Bone& nbone : frames.next->bones.Values()) {
				size_t id = meshFilter.GetBoneID(nbone.name);
				if (id >= boneCount) continue;
				Transform& cbone = m_currentBones[id];
				//�����p���ƕ�Ԃ���
				//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
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
		//�A�j���[�V�����̃��s�[�g�E�I�����m�F
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
		//�{�[�����X�V
		MeshFilter& meshFilter = gObj.GetComponent<MeshFilter>();
		Vector<Bone>& bones = meshFilter.GetBones();	//���f���Œ�`���ꂽ�{�[�����
		size_t boneCount = m_currentBones.Count();	//�{�[���s��̌�
		for (KeyBone& k : motion->GetKeyBones()) {
			size_t count = k.bones.Count();
			if (!count) continue;
			size_t id = meshFilter.GetBoneID(k.bones[0]->name);
			if (id >= boneCount) continue;
			Bone* pbone = nullptr;	//���݂̃t���[���ԍ��̒��O�̃{�[���Bnullptr�̂Ƃ������p��
			Bone* nbone = nullptr;	//���݂̃t���[���ԍ��̒���̃{�[���B���݂��Ȃ��ꍇ�A��ԍŌ�̃L�[�t���[���{�[��
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
			float ft = nbone->id ? static_cast<float>(m_currentFrameNo - pid) / (nbone->id - pid) : 0;	//�t���[����Ԉʒu
			uint32_t t = static_cast<uint32_t>(ft * 127);	//�t���[���ʒu��[0, 127]�ŕ\��
			if (pid == nbone->id) {	//��ԕs�v
				Transform& cbone = m_currentBones[id];
				//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
				cbone.LocalPosition.x = nbone->position.x;
				cbone.LocalPosition.y = nbone->position.y;
				cbone.LocalPosition.z = nbone->position.z;
				//}
				cbone.LocalRotation = nbone->rotation;
			}
			else if (!pbone) {	//�����p���ƕ�Ԃ���
				Transform& cbone = m_currentBones[id];
				//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
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
				//if (bones[id].isIKBone) {	//IK�{�[���̂ݕ��s�ړ����v�Z����
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

	//IK����
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
	size_t id = bones[ik.chainID[0]].parent;	//���[�g�{�[���̐e�{�[����ID����Z���^�[�{�[���܂ők��
	while (id < boneCount) {
		Bone& bone = bones[id];	//�{�[���̏����ʒu, ID
		dual = DualQuaternion::RotateOrigin(bone.rotation, bone.position) * dual;
		id = bone.parent;
	}
	Vector3 t = (bones[ik.id].position + m_currentBones[ik.id].LocalPosition) - (pos[0] * dual);	//�A�j���[�V�������K�p���ꂽ���[�g�{�[���̈ʒu��IK�{�[���̈ʒu��\���x�N�g��
	Vector3 targetPos = pos[0] + (t * dual.Inverse());	//�x�N�g�����A�j���[�V�����Ƌt��]�����A���[�g�{�[���̏����ʒu�E�p�����猩��IK�{�[���̌����Ƃ��AIK�{�[���ʒu���擾

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
//		if (bones[ik.chainID[0]].name.find(u"�Ђ�")) axis = Vector3::Right();
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
		bones[ik.chainID[1]].position,	//��]�������郋�[�g�{�[���̏����ʒu
		bones[ik.chainID[0]].position,	//���ԃ{�[���̏����ʒu
		bones[ik.targetID].position	//���[�{�[���̏����ʒu
	};

	DualQuaternion dual;
	size_t id = bones[ik.chainID[1]].parent;	//���[�g�{�[���̐e�{�[����ID����Z���^�[�{�[���܂ők��
	while (id < boneCount) {
		Bone& bone = bones[id];	//�{�[���̏����ʒu, ID
		dual = DualQuaternion::RotateOrigin(bone.rotation, bone.position) * dual;
		id = bone.parent;
	}
	Vector3 t = (bones[ik.id].position + m_currentBones[ik.id].LocalPosition) - (pos[0] * dual);	//�A�j���[�V�������K�p���ꂽ���[�g�{�[���̈ʒu��IK�{�[���̈ʒu��\���x�N�g��
	Vector3 targetPos = pos[0] + (t * dual.Inverse());	//�x�N�g�����A�j���[�V�����Ƌt��]�����A���[�g�{�[���̏����ʒu�E�p�����猩��IK�{�[���̌����Ƃ��AIK�{�[���ʒu���擾

	Vector3 p1p2 = pos[1] - pos[0];
	Vector3 p1t = targetPos - pos[0];
	Vector3 p2p3 = pos[2] - pos[1];
	float p1p2m = p1p2.Magnitude();
	float p1tm = p1t.Magnitude();
	float p2p3m = p2p3.Magnitude();
	float gamma = Vector3::Angle(p1p2, p1t);	//(���[�g������)��(���[�g��IK�{�[��)�̃x�N�g���Ԃ̊p�x(��)
	float beta = Math::AcosDeg((p1p2m * p1p2m + p1tm * p1tm - p2p3m * p2p3m) / (2 * p1p2m * p1tm));	//���[�g�A���ԁAIK�{�[�����O�p�`���Ȃ��Ƃ��̃��[�g�̊p�x(��)
	Vector3 axis = Vector3::Cross(p1p2, p1t);	//��]����(���[�g������)��(���[�g��IK�{�[��)�̃x�N�g�����Ȃ����ʂ̖@��
	//��]������␳����(�b��I?)
	if (Vector3::Dot(axis, Vector3::Right()) < 0) {
		gamma *= -1;
		axis = -axis;
	}
	if (bones[ik.chainID[0]].name.find(u"�Ђ�")) axis = Vector3::Right();
	float alpha = Math::AcosDeg((p1p2m * p1p2m + p2p3m * p2p3m - p1tm * p1tm) / (2 * p1p2m * p2p3m));	//���[�g�A���ԁAIK�{�[�����O�p�`���Ȃ��Ƃ��̒��Ԃ̊p�x(��)
	//���[�g�ƒ��Ԃ̉�]���㏑��
	m_currentBones[ik.chainID[1]].LocalRotation = Quaternion::AngleAxis(beta + gamma, axis);
	m_currentBones[ik.chainID[0]].LocalRotation = Quaternion::AngleAxis(alpha - 180.0f, axis);
	//IK�{�[���̈ʒu�����Z�b�g(GetBoneMatrix�֐��ւ̉e�����l������)
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
	Vector<IK>& iks = meshFilter.GetIKs();	//���f���Œ�`���ꂽIK���
	Vector<Bone> bones = meshFilter.GetBones();	//���f���Œ�`���ꂽ�{�[�����
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
	Vector<Bone>& bones = meshFilter.GetBones();	//���f���Œ�`����Ă���{�[�����
	const size_t boneCount = bones.Count();
	//root�ƂȂ�{�[��������
	Bone* root = nullptr;
	for (Bone& b : bones) {
		//�e�{�[����ID���{�[�������傫�� && IK�{�[���łȂ� && ID���{�[�����ȉ� => root�{�[��
		if (b.parent >= boneCount && !b.isIKBone && b.id < boneCount) {
			root = &b;
			break;
		}
	}
	if (!root) return f();
	//root�{�[���ɑΉ�����A�j���[�V�����̃{�[������s����擾
	//root�{�[�������_�Ɉڂ� -> �A�j���[�V�����̃X�P�[���E��]�E�ړ� -> ���_������Ƃ̍��W�ɖ߂�		
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
