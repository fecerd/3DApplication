module Scene;
import Components;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

void Scene::AddObject(GameObject* object) noexcept {
	if (!object) return;
	if (!m_gameObjects.Contains(object)) {
		m_gameObjects.PushBack(object);
		object->SetScene(this);
	}
	for (Transform* child : object->GetChildren()) AddObject(&child->GetObject());
}

Vector<Camera*> Scene::GetAllCamera() const noexcept {
	Vector<Camera*> camera;
	camera.Reserve(5);
	for (GameObject* gObj : m_gameObjects) {
		Camera* c = gObj->GetComponentPtr<Camera>();
		if (c) camera.Add(c);
	}
	camera.Sort(true, [](Camera* const& lhs, Camera* const& rhs) { return lhs->GetRenderingOrder() <=> rhs->GetRenderingOrder(); });
	return camera;
}

void Scene::UpdateHeap() noexcept {
	SceneResourceDesc desc{};
	for (GameObject* gObj : m_gameObjects) {
		Light* light = gObj->GetComponentPtr<Light>();
		if (!light || light->GetLightType() != LightType::Directional) continue;
		desc.SetLightColor(light->GetColor());
		desc.lightDirection = light->GetDirection();
		break;
	}
	if (!m_sceneHeap) m_sceneHeap = CreateHeap(String::Empty(), HeapType::Scene);
	if (!m_sceneResource) {
		m_sceneResource = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
		m_sceneHeap.SetView(0, ViewFormat::CBV, m_sceneResource);
	}
	else m_sceneResource.UpdateResource(desc.GetData(), desc.GetCount());
}

void Scene::Draw() noexcept {
	OnPreDraw();
	//ヒープ更新
	for (GameObject* gObj : m_gameObjects) gObj->UpdateAllHeap();
	this->UpdateHeap();
	Heap sceneHeap = this->GetHeap();
	//カメラごとに描画
	for (Camera* camera : this->GetAllCamera()) {
		Heap cameraHeap = camera->GetHeap();
		RenderTarget renderTarget = camera->GetRenderTarget();
		renderTarget.BeginCommand(true, true, true);
		renderTarget.SetViewport();
		renderTarget.SetScissorRect();
		renderTarget.SetHeap(HeapType::Camera, cameraHeap);
		renderTarget.SetHeap(HeapType::Scene, sceneHeap);
		const FlagSet<LayerMask>& cameraLayerMask = camera->GetCullingMask();
		const bool allRendering = cameraLayerMask == LayerMask::All;
		for (GameObject* gObj : m_gameObjects) {
			if (!gObj->HasComponent<MeshFilter>()) continue;
			if (!allRendering && !(cameraLayerMask & gObj->Layer)) continue;
			renderTarget.SetCommandList(gObj->GetCommandList());
		}
		renderTarget.EndCommand(true);
		//RenderTarget deferredRenderTarget = camera->GetDeferredRenderTarget();
		//deferredRenderTarget.BeginCommand(true, true, true);
		//deferredRenderTarget.SetViewport();
		//deferredRenderTarget.SetScissorRect();
		//deferredRenderTarget.SetHeap(HeapType::Camera, Common3D::GetHeap(Common3D::DefaultCameraHeapName));
		//deferredRenderTarget.SetHeap(HeapType::Scene, Common3D::GetHeap(Common3D::DefaultSceneHeapName));
		camera->PostDraw(*camera);
	}
	OnPostDraw();
}
