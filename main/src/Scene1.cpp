module Scene1;
import System;
import Common3D;
import Engine;
import Resource1;
import Script1;
import MediaPlayer;

using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace System::Drawing;
using namespace Engine;

class TreeScript : public Script {
	GameObject* m_player = nullptr;
	Transform* m_transform = nullptr;
public:
	using Script::Script;
public:
	void Init() noexcept override {
		this->EnableUpdate = true;
		m_transform = &this->GetObject().GetTransform();
	}
	void SetPlayer(GameObject* player) noexcept {
		m_player = player;
	}
	void Update() noexcept override {
		if (!m_player || !m_transform) return;
		Vector3 playerPos = m_player->GetTransform().Position();
		playerPos.y = 0.f;
		Vector3 pos = m_transform->Position();
		pos.y = 0.f;
		Vector3 vec = playerPos - pos;
		Quaternion rot = Quaternion::FromToRotation(Vector3::Back(), vec.Normalized());
		m_transform->Rotation(rot);
	}
};

GameObject* CreateField(GameObject* player) noexcept {
	GameObject* container = new GameObject(u"Field Container");
	{
		GameObject* gObj = new GameObject(u"Field");
		gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Field));
		gObj->SetParent(container);
	}
	{
		Image image { ResourcePaths::GetPathName(u"./Model/Field1/tree3b.png") };
		Resource texture = CreateResource(ResourcePaths::GetPathName(u"./Model/Field1/tree3b.png"), image);

		RendererDesc rendererDesc{};
		rendererDesc.vs.name = u"TreePlaneVS";
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(u"./HLSL/TreePlaneVS.hlsl");
		rendererDesc.vs.entry = Common3D::DefaultShaderEntry;
		rendererDesc.vs.target = Common3D::DefaultVSTarget;
		rendererDesc.ps.name = u"TreePlanePS";
		rendererDesc.ps.filepath = ResourcePaths::GetPathName(u"./HLSL/TreePlanePS.hlsl");
		rendererDesc.ps.entry = Common3D::DefaultShaderEntry;
		rendererDesc.ps.target = Common3D::DefaultPSTarget;
		rendererDesc.culling = CullingMode::Back;
		rendererDesc.depthEnabled = true;
		RenderTargetDesc rtDesc{};
		rtDesc.blendEnabled = true;
		rtDesc.dstFactor = BlendFactors::SrcAlphaInv;
		rtDesc.srcFactor = BlendFactors::One;
		rtDesc.colorOp = BlendOp::Add;
		rtDesc.dstAlphaFactor = BlendFactors::SrcAlphaInv;
		rtDesc.srcAlphaFactor = BlendFactors::One;
		rtDesc.alphaOp = BlendOp::Add;
		rendererDesc.rt.Add(rtDesc);
		Renderer planeRenderer = Common3D::CreateRenderer(u"PlaneRenderer", rendererDesc);

		float xs[] = {
			-70.f
			-60.f,
			-40.f,
			-20.f,
			0.f,
			20.f,
			40.f,
			60.f,
			80.f,
			100.f
		};
		float zs[] = {
			-60.f,
			-40.f,
			-20.f,
			20.f,
			40.f,
			60.f,
			80.f			
		};
		float ys[] = {
			8.f,
			9.f,
			9.f,
			12.f,
			16.f,
			20.f,
			20.f
		};

		XOR128 rng = XOR128::GetRNG();
		for (int i = 0; i < 16; ++i) {
			GameObject* gObj = new GameObject(u"TreePlane");
			Transform& tr = gObj->GetTransform();
			uint32_t x = rng.Get(sizeof(xs) / sizeof(float));
			uint32_t yz = rng.Get(sizeof(ys) / sizeof(float));
			tr.LocalPosition = Vector3(xs[x], ys[yz], zs[yz]);
			// tr.LocalPosition.x = 20.f * (5 - static_cast<int32_t>(rng.Get(11)));
			// tr.LocalPosition.z = 20.f * (5 - static_cast<int32_t>(rng.Get(11)));
			// tr.LocalPosition.y = 10.f + (tr.LocalPosition.z * 0.1f);
			tr.LocalScale = Vector3(10.f, 10.f, 10.f);
			MeshFilter& mf = gObj->AddComponent<MeshFilter>();
			mf.LoadPlane();

			mf.UpdateTexture(0, texture);
			mf.UpdateRenderer(0, planeRenderer);
			gObj->SetParent(container);

			TreeScript& script = gObj->AddScript<TreeScript>();
			script.SetPlayer(player);
		}
	}

	return container;
}


void Scene1::Init() noexcept {
	//Audio Object
	GameObject* audioObj = new GameObject(u"Audio");
	audioObj->AddScript<AudioScript>();
	this->AddObject(audioObj);
	//Directional Light
	GameObject* directionalLight = new GameObject(u"Directional Light");
	directionalLight->GetTransform().LocalRotation = Quaternion::Euler(50, -30, 0);
	Light& light = directionalLight->AddComponent<Light>();
	light.SetLightType(LightType::Directional);
	light.SetColor(Color(224, 224, 224));
	this->AddObject(directionalLight);
	//Main Camera Object
	GameObject* mainCamera = nullptr;
	{
		mainCamera = new GameObject(u"MainCamera");
		Camera& camera = mainCamera->AddComponent<Camera>();
		camera.SetNearFar(1, 3000);
		camera.SetFovY(1080.f / 1920.f, 30);
		camera.SetRenderTarget(1920, 1080);
		camera.SetWindowRendering(false);
		camera.PostDraw += [this](Camera& renderedCamera) {
			GameObject* gObj = this->GetObject(u"MainCameraScene");
			if (!gObj) return;
			MeshFilter* mf = gObj->GetComponentPtr<MeshFilter>();
			if (!mf) return;
			mf->UpdateTexture(0, renderedCamera.GetTexture(0));
			gObj->UpdateCommandList();
		};
		this->AddObject(mainCamera);
	}
	CameraManager* cameraManager = nullptr;
	//Camera Container
	{
		GameObject* cameraChild = new GameObject(u"CameraChild");
		GameObject* cameraParent = new GameObject(u"CameraParent");
		mainCamera->SetParent(cameraChild);
		cameraChild->SetParent(cameraParent);
		CameraManager& manager = cameraParent->AddScript<CameraManager>();
		manager.Distance = 10.f;
		manager.SetLookRotation(20, 0);
		manager.OffsetPosition = Vector3(0, 15.f, -40.f);
		this->AddObject(cameraChild);
		this->AddObject(cameraParent);
		cameraManager = &manager;
	}
	//Characters
	{
		//PMD Container
		GameObject* container = new GameObject(u"PMDContainer");
		{
			container->AddScript<ControllerScript>().SetCamera(mainCamera).SetDelta(20);
			container->AddComponent<Rigidbody>();
			this->AddObject(container);
			cameraManager->LookTarget = &container->GetTransform();
		}
		{
			GameObject* gObj = new GameObject(u"Rin");
			gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Rin));
			gObj->GetTransform().LocalRotation = Quaternion::Euler(0, 180, 0);
			gObj->SetParent(container);
			Animator& mikuAnimator = gObj->AddComponent<Animator>();
			mikuAnimator.SetUpdateCountPerFrame(1);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::StopToWalk), u"StopToWalk", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::Walking), u"Walking", true);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::WalkToStop), u"WalkToStop", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::StopToDash), u"StopToDash", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::Dash), u"Dash", true);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::DashToStop), u"DashToStop", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::DashToWalk), u"DashToWalk", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::WalkToDash), u"WalkToDash", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::Stopping), u"Stopping", true);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::WalkToJump), u"WalkToJump", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::Jumping), u"Jumping", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::JumpToFall), u"JumpToFall", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::Falling), u"Falling", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(ResourcePaths::Motion::Moves::FallToWalk), u"FallToWalk", false);
			gObj->AddScript<ModelScript>();
			this->AddObject(gObj);
		}
		container = new GameObject(u"MikuContainer");
		{
			container->AddComponent<Rigidbody>();
			container->GetTransform().LocalPosition.x += 10;
			this->AddObject(container);
		}
		{
			GameObject* gObj = new GameObject(u"Miku");
			gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Miku));
			gObj->GetTransform().LocalRotation = Quaternion::Euler(0, 180, 0);
			gObj->SetParent(container);
			Animator& mikuAnimator = gObj->AddComponent<Animator>();
			mikuAnimator.SetUpdateCountPerFrame(1);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\stop_to_walk.vmd)"), u"StopToWalk", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\walking.vmd)"), u"Walking", true);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\walk_to_stop.vmd)"), u"WalkToStop", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\stop_to_dash.vmd)"), u"StopToDash", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\dash.vmd)"), u"Dash", true);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\dash_to_stop.vmd)"), u"DashToStop", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\dash_to_walk.vmd)"), u"DashToWalk", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\walk_to_dash.vmd)"), u"WalkToDash", false);
			mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\stopping.vmd)"), u"Stopping", true);
//				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\walk_to_jump.vmd)"), u"WalkToJump", false);
//				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\jumping.vmd)"), u"Jumping", false);
//				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\jump_to_fall.vmd)"), u"JumpToFall", false);
//				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\falling.vmd)"), u"Falling", false);
//				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\fall_to_walk.vmd)"), u"FallToWalk", false);
			gObj->AddScript<CPUScript>();
			this->AddObject(gObj);
		}

	}
	//Mirror
	GameObject* mirror = CreateMirror();
	mirror->GetTransform().LocalPosition = Vector3(-10, 15, 20);
	mirror->GetTransform().LocalScale = Vector3(10, 10, 1);
	mirror->AddScript<MirrorScript>();
	this->AddObject(mirror);
	//Sphere
	GameObject* sphere = new GameObject(u"Sphere");
	sphere->GetTransform().LocalPosition = Vector3(-10, 15, 0);
	sphere->AddComponent<MeshFilter>().LoadSphere();
	this->AddObject(sphere);
	//フィールド
	{
		GameObject* container = new GameObject(u"Classroom Container");
		this->AddObject(container);
		{
			GameObject* gObj = new GameObject(u"Classroom");
			gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Classroom));
			gObj->SetParent(container);
			this->AddObject(gObj);
		}
		{
			GameObject* gObj = new GameObject(u"Desks");
			gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Desk));
			gObj->SetParent(container);
			this->AddObject(gObj);
		}
	}
	{
		GameObject* field = CreateField(this->GetObject(u"PMDContainer"));
		Transform& fieldTr = field->GetTransform();
		fieldTr.LocalPosition = Vector3(-800.f, 0.f, 0.f);
		fieldTr.LocalScale = Vector3(5.f, 5.f, 5.f);
		this->AddObject(field);
	}

	//UI
	{
		GameObject* uiCamera = new GameObject(u"UI Camera");
		Camera& camera = uiCamera->AddComponent<Camera>();
		camera.SetCullingMask(LayerMask::UI);
		camera.SetNearFar(0.f, 1.f);
		camera.SetScreenAspect(1080.f / 1920.f);
		camera.SetRenderTarget(1920, 1080);
		camera.SetRenderingOrder(128 + 1);
		camera.SetWindowRendering(true);
		this->AddObject(uiCamera);

		GameObject* mainCameraScreen = new GameObject(u"MainCameraScene");
		mainCameraScreen->Layer = LayerMask::UI;
		mainCameraScreen->GetTransform().LocalPosition.z = 1.f;
		MeshFilter& mainMF = mainCameraScreen->AddComponent<MeshFilter>();
		mainMF.LoadPlane();
		Material& material = mainMF.GetMaterials()[0];
		material.SetRenderer(Common3D::GetRenderer(Common3D::DefaultVideoRendererName));
		this->AddObject(mainCameraScreen);

		GameObject* uiCanvas = new GameObject(u"UI Canvas");
		GameObject* uiObj = new GameObject(u"UI Object");
		uiObj->GetTransform().LocalPosition = Vector3(0.5f, -0.5f, 0.f);
		uiObj->GetTransform().LocalScale = Vector3(0.3f, 0.3f, 1.f);
		uiObj->SetParent(uiCanvas);
		uiObj->Layer = LayerMask::UI;
		MeshFilter& mf = uiObj->AddComponent<MeshFilter>();
		mf.LoadPlane();
		uiObj->AddScript<VideoScript>();


		this->AddObject(uiCanvas);
		this->AddObject(uiObj);
	}

}


