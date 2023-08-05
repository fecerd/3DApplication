export module MyWindow2;
import DefaultWindow;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

import TaskWorker;
import Resource1;
import Script1;

export class MyScene2 : public Scene {
public:
	using Scene::Scene;
	~MyScene2() noexcept = default;
public:
	void Init() noexcept override {
		Common3D::RendererDesc desc{};
		desc.vs.name = u"NoHeapVS";
		desc.vs.filepath = ResourcePaths::GetPathName(uR"(.\HLSL\NoHeapVS.hlsl)");
		desc.vs.entry = u"main";
		desc.vs.target = u"vs_5_0";
		desc.ps.name = u"NoHeapPS";
		desc.ps.filepath = ResourcePaths::GetPathName(uR"(.\HLSL\NoHeapPS.hlsl)");
		desc.ps.entry = u"main";
		desc.ps.target = u"ps_5_0";
		desc.depthEnabled = true;
		desc.depthFunc = Common3D::DepthFuncType::LessEq;
		desc.stencilEnabled = false;
		desc.rt.Add(RenderTargetDesc{});
		Renderer noheapRenderer = Common3D::CreateRenderer(u"NoHeap", desc);

		GameObject* audioObj = new GameObject(u"AudioObj");
		audioObj->AddScript<AudioScript>();
		this->AddObject(audioObj);

		GameObject* cameraObj = new GameObject(u"MainCamera");
		Camera& camera = cameraObj->AddComponent<Camera>();
		camera.SetScreenAspect(1080.f / 1920.f);
		camera.SetRenderTarget(1920, 1080);
		this->AddObject(cameraObj);

		GameObject* background = new GameObject(u"Background");
		background->AddComponent<MeshFilter>().LoadPlane();
		for (Material& material : background->GetComponent<MeshFilter>().GetMaterials()) {
			ReflectionsResourceDesc desc{};
			desc.diffuse[0] = 1.f;
			desc.diffuse[1] = 0.f;
			desc.diffuse[2] = 0.f;
			desc.diffuse[3] = 1.f;
			material.reflections.UpdateResource(desc.GetData(), desc.GetCount());
		}
		this->AddObject(background);

		GameObject* triangle = new GameObject(u"Triangle");
		triangle->AddComponent<MeshFilter>().LoadTriangle();
		for (Material& material : triangle->GetComponent<MeshFilter>().GetMaterials()) {
			material.renderer = noheapRenderer;
		}
		this->AddObject(triangle);
	}
	void OnPreDraw() noexcept override {
		GameObject* background = this->GetObject(u"Background");
		if (!background) return;
		static uint8_t r = 255;
		static bool up = false;
		for (Material& material : background->GetComponent<MeshFilter>().GetMaterials()) {
			ReflectionsResourceDesc desc{};
			desc.diffuse[0] = r / 255.f;
			desc.diffuse[1] = 0.f;
			desc.diffuse[2] = 0.f;
			desc.diffuse[3] = 1.f;
			material.reflections.UpdateResource(desc.GetData(), desc.GetCount());
		}
		if (up) {
			if (++r == 255) up = false;
		}
		else if (--r == 0) up = true;
	}
};

import MediaPlayer;
export class MyScene1 : public Scene {
	Resource texture;
	System::Application::MediaPlayer m_player;
public:
	using Scene::Scene;
	~MyScene1() noexcept = default;
	void Init() noexcept override {
		m_player.Load(u"Test2", System::Application::MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Video::Test2));
		texture = Common3D::GetResource(DefaultBlackTextureName);

		Renderer videoRenderer = Common3D::GetRenderer(Common3D::DefaultVideoRendererName);

		GameObject* cameraObj = new GameObject(u"MainCamera");
		Camera& camera = cameraObj->AddComponent<Camera>();
		camera.SetScreenAspect(1080.f / 1920.f);
		camera.SetRenderTarget(1920, 1080);
		this->AddObject(cameraObj);

		GameObject* gObj = new GameObject(u"Plane");
		MeshFilter& meshFilter = gObj->AddComponent<MeshFilter>();
		meshFilter.LoadPlane();
		Heap materialHeap = meshFilter.GetHeap();
		uint32_t materialIndex = 0;
		for (Material& material : meshFilter.GetMaterials()) {
			material.texture = texture;
			materialHeap.SetView(materialIndex * 5 + 1, ViewFormat::SRV, material.texture);
			material.renderer = videoRenderer;
			++materialIndex;
		}
		this->AddObject(gObj);
	}
	void OnActivate() noexcept {
		m_player.Play(u"Test2", System::Application::MediaPlayerSourceType::Local);
		this->EndActivate();
	}
	void OnUpdate() noexcept override {
		if (m_player.IsEnded()) {
			SceneManager::GetManager().LoadScene<MyScene2>(u"MyScene2");
			SceneManager::GetManager().ActivateScene(u"MyScene2", 0);
			SceneManager::GetManager().DeactivateScene(this->GetName());
			return;
		}
		m_player.Update();
		System::Drawing::Image frame = m_player.GetCurrentFrame();
		if (!frame.Enabled()) return;
		Size<uint32_t> currentSize = texture.GetSize();
		if (currentSize.width == frame.Width() && currentSize.height == frame.Height()) {
			texture.UpdateResource(frame);
		}
		else {
			texture = CreateResource(String::Empty(), frame);
			GameObject* plane = this->GetObject(u"Plane");
			if (!plane || !plane->HasComponent<MeshFilter>()) return;
			MeshFilter& meshFilter = plane->GetComponent<MeshFilter>();
			Heap materialHeap = meshFilter.GetHeap();
			uint32_t index = 0;
			for (Material& material : meshFilter.GetMaterials()) {
				material.texture = texture;
				materialHeap.SetView(index * 5 + 1, ViewFormat::SRV, material.texture);
				++index;
			}
			plane->UpdateCommandList();
		}
	}
	nanoseconds firstDeactivateNs = nanoseconds(0);
	void OnDeactivate() noexcept override {
		nanoseconds ns = this->GetElapsedTimeNs();
		if (firstDeactivateNs == nanoseconds(0)) {
			firstDeactivateNs = ns;
		}
		ns = ns - firstDeactivateNs;
		if (SceneManager::GetManager().IsActive(u"MyScene2")) {
			m_player.Stop();
			firstDeactivateNs = nanoseconds(0);
			this->SetBrightness(1.f);
			this->EndDeactivate();
		}
		else {
			static const nanoseconds limit = nanoseconds(seconds(2));
			float brightness = (ns > limit ? 0.f : static_cast<float>(ns.count()) / limit.count()) + 0.2f;
			this->SetBrightness(brightness);
		}
	}
};

export class MyWindow2 : public DefaultWindow {
public:
	MyWindow2() noexcept : DefaultWindow(u"MyWindow2", StartScene<MyScene1>()) {}
};
