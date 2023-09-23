module SplashScreenScene;
import Engine;
import Common3D;
import Application;

import Scene1;

using namespace System;
using namespace System::Drawing;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

namespace ResourcePaths::Picture {
	inline constexpr CString SplashScreenImage = u"./Picture/SplashScreen.png";
}

void SplashScreenScene::Init() noexcept {
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
		Image image = Image::FromFile(ResourcePaths::GetPathName(ResourcePaths::Picture::SplashScreenImage));
		Drawing::BMP::FromImage(image).Save(ResourcePaths::ExePath.CreatePath(u"./splash.bmp").PathName());
		Resource texture = CreateResource(u"SplashScreenImage", image);
		mainMF.UpdateTexture(0, texture);

		this->AddObject(mainCameraScreen);
	}
}

void SplashScreenScene::OnBeginActivate() noexcept {
	SceneManager& manager = SceneManager::GetManager();
	manager.LoadScene<Scene1>(u"Scene1");
	manager.ActivateScene(u"Scene1");
	this->SetDepth(0.f);
}
void SplashScreenScene::OnActivated() noexcept {}

void SplashScreenScene::OnUpdate() noexcept {
	auto cond = [this]() -> bool {
		return SceneManager::GetManager().IsActive(u"Scene1");
	};
	if (!cond()) return;
	SceneManager& manager = SceneManager::GetManager();
	manager.DeactivateScene(this->GetName());
}

void SplashScreenScene::OnBeginDeactivate() noexcept {
	m_finalizeStartTime = this->GetElapsedTimeNs();
}

void SplashScreenScene::OnDeactivate() noexcept {
	auto func = [this]() -> bool {
		nanoseconds ns = this->GetElapsedTimeNs() - m_finalizeStartTime;
		if (m_finalizeTime.count() > ns.count()) {
			float brightness = 1.f - (static_cast<float>(ns.count()) / m_finalizeTime.count());
			this->SetBrightness(brightness);
			this->SetAlpha(brightness);
			return false;
		}
		else {
			return true;
		}
	};
	if (func()) this->EndDeactivate();
}

void SplashScreenScene::OnEndDeactivate() noexcept {}
