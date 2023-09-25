export module Scene1;
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
				Resource& tex = material.GetResource(1);
				tex = texture;
				materialHeap.SetView(materialIndex * 5 + 1, ViewFormat::SRV, tex);
				material.SetRenderer(videoRenderer);
				++materialIndex;
			}
		}
		m_player.Load(u"Test2", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Video::CountDown));
		m_player.SetLoopMode(true);
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
				Resource& tex = material.GetResource(1);
				tex = texture;
				materialHeap.SetView(index * 5 + 1, ViewFormat::SRV, tex);
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
	void Init() noexcept override;
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
