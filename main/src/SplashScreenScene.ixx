export module SplashScreenScene;
export import Engine;
export import Common3D;
import Application;
using namespace System;
using namespace System::Drawing;
using namespace System::Application::Common3D;
using namespace Engine;

namespace ResourcePaths::Picture {
	inline constexpr CString SplashScreenImage = uR"(.\Picture\SplashScreen.png)";
}

import Scene1;

export class SplashScreenScene : public Scene {
	using Scene::Scene;
public:
	void Init() noexcept override {
		this->SetDepth(0.f);
		GameObject* gObj = new GameObject(u"Plane");
		gObj->Layer = LayerMask::UI;
		gObj->GetTransform().LocalPosition.z = 1.f;
		MeshFilter& mf = gObj->AddComponent<MeshFilter>();
		mf.LoadPlane();
		Renderer videoRenderer = GetRenderer(DefaultVideoRendererName);
		Image image = Image::FromFile(ResourcePaths::GetPathName(ResourcePaths::Picture::SplashScreenImage));
		Resource texture = CreateResource(u"SplashScreenImage", image);
		for (Material& material : mf.GetMaterials()) {
			material.renderer = videoRenderer;
			if (texture) material.texture = texture;
			else material.texture = GetResource(DefaultBlackTextureName);
		}
		this->AddObject(gObj);
		GameObject* cameraObj = new GameObject(u"Camera");
		Camera& camera = cameraObj->AddComponent<Camera>();
		camera.SetCullingMask(LayerMask::UI);
		camera.SetNearFar(0.f, 1.f);
		camera.SetScreenAspect(1080.f / 1920.f);
		camera.SetRenderTarget(1920, 1080);
		camera.SetRenderingOrder(128 + 1);
		camera.SetWindowRendering(true);
		this->AddObject(cameraObj);
	}
	const milliseconds m_activateTime = milliseconds(1000);
	nanoseconds m_firstTime = nanoseconds(0);
	SceneState m_prevState = SceneState::Loaded;
	void OnActivate() noexcept override {
		if (m_prevState == SceneState::Loaded) {
			m_firstTime = nanoseconds(0);
			SceneManager& manager = SceneManager::GetManager();
			manager.LoadScene<Scene1>(u"Scene1");
			manager.ActivateScene(u"Scene1");
			m_prevState = SceneState::Activate;
		}
		nanoseconds ns = this->GetElapsedTimeNs();
		ns -= m_firstTime;
		float brightness = static_cast<float>(ns.count()) / nanoseconds(m_activateTime).count();
		if (brightness > 1.f) {
			this->SetBrightness(1.f);
			this->EndActivate();
		}
		else this->SetBrightness(brightness);
	}
	void OnUpdate() noexcept override {
		if (m_prevState == SceneState::Activate) {
			SceneManager& manager = SceneManager::GetManager();
			manager.DeactivateScene(u"SplashScreenScene");
			m_prevState = SceneState::Active;
		}
	}
	const milliseconds m_deactivateTime = milliseconds(1000);
	bool m_deactivateWait = false;
	void OnDeactivate() noexcept override {
		if (m_deactivateWait) {
			SceneManager& manager = SceneManager::GetManager();
			if (manager.IsActive(u"Scene1")) {
				this->EndDeactivate();
				m_deactivateWait = false;
			}
		}
		else {
			if (m_prevState == SceneState::Active) {
				m_firstTime = nanoseconds(0);
				m_prevState = SceneState::Deactivate;
			}
			nanoseconds ns = this->GetElapsedTimeNs();
			ns -= m_firstTime;
			float brightness = 1.f - (static_cast<float>(ns.count()) / nanoseconds(m_deactivateTime).count());
			if (brightness < 0.f) {
				this->SetBrightness(0.f);
				m_deactivateWait = true;
			}
			else this->SetBrightness(brightness);
		}
	}
};