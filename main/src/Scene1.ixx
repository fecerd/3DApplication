export module Scene1;
import System;
import Common3D;
import Engine;
import Resource1;
import Script1;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace System::Drawing;
using namespace Engine;

GameObject* CreateMirror(const String& name = u"Mirror", uint8_t renderingOrder = 128 - 1) noexcept {
	GameObject* gObj = new GameObject(name);
	gObj->Layer = LayerMask::Layer2;
	gObj->GetTransform().LocalScale = Vector3(1, 1, 0.1f);
	MeshFilter& mf = gObj->AddComponent<MeshFilter>();
	mf.LoadCube();
	ReflectionsResourceDesc desc{};
	desc.SetSpecularColor(Drawing::Color(224, 224, 224));
	mf.UpdateReflections(0, desc);
	const float near = 10.f;
	GameObject* cameraObj = new GameObject(name + u" Camera");
	Transform& cameraTr = cameraObj->GetTransform();
	cameraTr.LocalPosition.z = near;
	cameraTr.LocalRotation = Quaternion::Euler(0, 180, 0);
	cameraTr.LocalScale.x *= -1;
	Camera& camera = cameraObj->AddComponent<Camera>();
	camera.SetRenderingOrder(renderingOrder);
	camera.SetCullingMask(LayerMask::Default);
	camera.SetNearFar(near, 1000.f);
	camera.SetFrustum(1, -1, -1, 1);
	camera.SetWindowRendering(false);
	camera.SetRenderTarget(1920, 1920);
	camera.PostDraw += [name](Camera& renderedCamera) {
		GameObject* planeObj = renderedCamera.GetObject().GetParent();
		if (!planeObj || planeObj->Name != name) return;
		planeObj->GetComponent<MeshFilter>().UpdateTexture(0, renderedCamera.GetTexture(0));
	};
	cameraObj->SetParent(gObj);
	return gObj;
}

import MediaPlayer;
class VideoScript : public Script {
	MediaPlayer m_player;
	Resource texture;
public:
	using Script::Script;
public:
	void Init() noexcept override {
		this->EnableSceneActivate = true;
		this->EnableUpdate = true;
		this->EnableSceneDeactivate = true;

		texture = Common3D::GetResource(Common3D::DefaultBlackTextureName);

		GameObject& gObj = this->GetObject();
		if (gObj.HasComponent<MeshFilter>()) {
			MeshFilter& mf = gObj.GetComponent<MeshFilter>();
			Heap materialHeap = mf.GetHeap();
			Vector<Material>& materials = mf.GetMaterials();
			uint32_t materialIndex = 0;
			Renderer videoRenderer = Common3D::GetRenderer(Common3D::DefaultVideoRendererName);
			for (Material& material : materials) {
				material.texture = texture;
				materialHeap.SetView(materialIndex * 5 + 1, ViewFormat::SRV, material.texture);
				material.renderer = videoRenderer;
				++materialIndex;
			}
		}
		m_player.Load(u"Test2", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Video::Test2));
	}
	void SceneActivate() noexcept override {
		m_player.Play(u"Test2", MediaPlayerSourceType::Local);
		this->EnableSceneActivate = false;
	}
	void Update() noexcept override {
		if (m_player.IsEnded()) return;
		m_player.Update();
		System::Drawing::Image frame = m_player.GetCurrentFrame();
		if (!frame.Enabled()) return;
		Size<uint32_t> currentSize = texture.GetSize();
		if (currentSize.width == frame.Width() && currentSize.height == frame.Height()) {
			texture.UpdateResource(frame);
		}
		else {
			texture = CreateResource(String::Empty(), frame);
			GameObject& gObj = this->GetObject();
			if (!gObj.HasComponent<MeshFilter>()) return;
			MeshFilter& mf = gObj.GetComponent<MeshFilter>();
			Heap materialHeap = mf.GetHeap();
			uint32_t index = 0;
			for (Material& material : mf.GetMaterials()) {
				material.texture = texture;
				materialHeap.SetView(index * 5 + 1, ViewFormat::SRV, material.texture);
				++index;
			}
			gObj.UpdateCommandList();
		}
	}
	void SceneDeactivate() noexcept override {
		m_player.Stop();
		this->EnableSceneDeactivate = false;
	}
};

export class Scene1 : public Scene {
	using Scene::Scene;
public:
	~Scene1() noexcept {}
public:
	void Init() noexcept override {
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
			camera.SetNearFar(1, 1000);
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
				GameObject* gObj = new GameObject(u"Miku");
				gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Rin));
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
				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\walk_to_jump.vmd)"), u"WalkToJump", false);
				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\jumping.vmd)"), u"Jumping", false);
				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\jump_to_fall.vmd)"), u"JumpToFall", false);
				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\falling.vmd)"), u"Falling", false);
				mikuAnimator.SetMotionByBone(ResourcePaths::GetPathName(uR"(.\Motion\Moves\fall_to_walk.vmd)"), u"FallToWalk", false);
				gObj->AddScript<ModelScript>();
				this->AddObject(gObj);
			}
			{
				GameObject* gObj = new GameObject(u"Rin");
				gObj->GetTransform().LocalPosition.x += 10;
				gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::Miku));
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
			GameObject* container = new GameObject(u"Field Container");
			this->AddObject(container);
			{
				GameObject* gObj = new GameObject(u"ClassRoom");
				gObj->AddComponent<MeshFilter>().LoadModelForPMD(ResourcePaths::GetPathName(ResourcePaths::Model::ClassRoom));
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
			material.renderer = Common3D::GetRenderer(Common3D::DefaultVideoRendererName);
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
	nanoseconds firstActivateNs = nanoseconds(0);
	void OnActivate() noexcept override {
		nanoseconds ns = this->GetElapsedTimeNs();
		if (firstActivateNs == nanoseconds(0)) firstActivateNs = ns;
		ns = ns - firstActivateNs;
		if (ns > seconds(5)) {
			this->SetBrightness(1.f);
			this->EndActivate();
		}
		else {
			float brightness = static_cast<float>(ns.count()) / static_cast<nanoseconds>(seconds(5)).count();
			this->SetBrightness(brightness);
		}
	}
};
