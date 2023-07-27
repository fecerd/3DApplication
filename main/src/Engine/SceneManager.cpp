module SceneManager;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

void SceneManager::Rendering(RenderTarget renderTarget) noexcept {
	if (!cameraObj) {
		cameraObj = new GameObject(u"SceneManager: CameraObj");
		cameraObj->AddComponent<Camera>();
	}
	if (!letterboxObj) {
		letterboxObj = new GameObject(u"SceneManager: LetterBoxObj");
		letterboxObj->GetTransform().LocalPosition.z = 1.f;
		letterboxObj->UpdateHeap();
		MeshFilter& mf = letterboxObj->AddComponent<MeshFilter>();
		mf.LoadPlane();
		ReflectionsResourceDesc desc{};
		desc.SetDiffuseColor(Drawing::Colors::Black);
		mf.UpdateReflections(0, desc);
	}
	if (!sceneHeap) {
		sceneHeap = CreateHeap(String::Empty(), HeapType::Scene);
		SceneResourceDesc desc{};
		desc.lightDirection = Vector3::Forward();
		sceneResource = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
		sceneHeap.SetView(0, ViewFormat::CBV, sceneResource);
	}
	Size<uint32_t> size = renderTarget.GetSize();
	if (!size.width || !size.height) return;
	//�V�[���̃r���[�|�[�g�v�Z
	float rtAspect = static_cast<float>(size.width) / size.height;
	float sceneAspect = 1920.f / 1080.f;
	float rate = sceneAspect / rtAspect;
	float x = 0.f;
	float y = 0.f;
	float w = 1.f;
	float h = 1.f;
	if (rate < 1.f) {
		w = rate;
		x = 0.5f - w * 0.5f;
	}
	else {
		h = 1.f / rate;
		y = 0.5f - h * 0.5f;
	}
	Camera& camera = cameraObj->GetComponent<Camera>();
	camera.SetScreenAspect(rtAspect);
	camera.UpdateHeap();
	//���^�[�{�b�N�X�`��
	renderTarget.BeginCommand(true, true, true);
	renderTarget.SetScissorRect();
	renderTarget.SetViewport();
	renderTarget.SetHeap(HeapType::Scene, sceneHeap);
	renderTarget.SetHeap(HeapType::Camera, camera.GetHeap());
	renderTarget.SetCommandList(letterboxObj->GetCommandList());
	//�V�[���`��
	renderTarget.SetViewport(ViewportDesc{ x, y, w, h });
	{
		LockGuard lock{ m_mtx };	//m_activeScenes���L
		for (Scene* scene : m_activeScenes) {
			//�V�[���̏�Ԃ��m�F
			if (!Renderable(scene)) continue;
			//�J�������Ƃ�
			for (Camera* sceneCamera : scene->GetAllCamera()) {
				if (!sceneCamera->IsUsedWindowRendering()) continue;
				//���C���J������RenderTarget����`��ς݃e�N�X�`�����擾
				Resource texture = sceneCamera->GetTexture(0);
				//�V�[�����Ƃɔ|���S�����쐬
				GameObject* planeObj = planeObjects.At(scene);
				if (!planeObj) {
					planeObj = new GameObject(String::Joint(u"SceneManger: ", scene->GetName(), u": PlaneObj"));
					MeshFilter& mf = planeObj->AddComponent<MeshFilter>();
					mf.LoadPlane();
					Material& material = mf.GetMaterials()[0];
					material.renderer = Common3D::GetRenderer(Common3D::DefaultVideoRendererName);
					planeObjects.Insert(scene, planeObj);
				}
				//�f�v�X�l�͕ύX�����ꍇ������
				planeObj->GetTransform().LocalPosition.z = scene->GetDepth();
				planeObj->UpdateHeap();
				//�V�[���̖��邳�ƃA���t�@�l�͕ύX�����ꍇ������
				MeshFilter& mf = planeObj->GetComponent<MeshFilter>();
				ReflectionsResourceDesc desc{};
				desc.diffuse[0] = scene->GetBrightness();
				desc.diffuse[1] = desc.diffuse[0];
				desc.diffuse[2] = desc.diffuse[0];
				desc.diffuse[3] = scene->GetAlpha();
				mf.UpdateReflections(0, desc);
				//�e�N�X�`����ݒ�
				mf.UpdateTexture(0, texture);
				renderTarget.SetCommandList(planeObj->GetCommandList());
			}
		}
	}
	renderTarget.EndCommand(true);
}
