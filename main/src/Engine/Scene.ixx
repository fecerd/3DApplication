export module Scene;
import System;
import EventArgs;
import Common3D;
import Transform;
import Camera;
import Light;
import MeshFilter;
import LayerMask;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;

export namespace Engine { class ISceneManager; }

//SceneState
export namespace Engine {
	/// @brief Sceneの状態を表す列挙値
	enum class SceneState : uint8_t {
		/// @brief 初期値
		Unloaded = 0,
		/// @brief 読み込み済み
		Loaded,
		/// @brief ActiveSceneに追加された直後(最初のフレームでActivateに自動遷移)
		Added,
		/// @brief OnActivate()関数が呼び出される状態。EndActivate()関数でActiveに遷移
		Activate,
		/// @brief 通常のActiveSceneの状態
		Active,
		/// @brief OnDeactivate()関数が呼び出される状態。EndDeactivate()関数でRemovedに遷移
		Deactivate,
		/// @brief ActiveSceneから削除された直後(次のフレームでLoadedに自動遷移)
		Removed,
		/// @brief Sceneのデストラクタ呼び出し状態
		UnLoading
	};
}

//Scene_Impl
export namespace Engine {
	//Scene <-> GameObjectの循環参照を切るためtemplateで定義
	template<class GameObject>
	class Scene_Impl {
		const String m_name;
		SceneState m_state = SceneState::Unloaded;
		float m_depth = 1.f;
		float m_alpha = 1.f;
		float m_brightness = 1.f;
		bool m_visible = true;
		uint64_t m_frameCount = 0;
		nanoseconds m_elapsedTimeNs = nanoseconds(0);
		List<GameObject*> m_gameObjects;
		Heap m_sceneHeap;
		Resource m_sceneResource;
	public:
		Scene_Impl() noexcept = delete;
		Scene_Impl(const String& name) noexcept : m_name(name) {}
		virtual ~Scene_Impl() noexcept {
			m_state = SceneState::Unloaded;
			for (GameObject*& gObj : m_gameObjects) {
				delete gObj;
				gObj = nullptr;
			}
		}
	private:
		friend class ISceneManager;
		void SetState(SceneState state) noexcept { m_state = state; }
		SceneState GetState() const noexcept { return m_state; }
	public:
		void EndActivate() noexcept {
			OnEndActivate();
			if (m_state == SceneState::Activate) SetState(SceneState::Active);
			for (auto* gObj : m_gameObjects) gObj->SceneActivated();
			OnActivated();
		}
		void EndDeactivate() noexcept {
			OnEndDeactivate();
			for (auto* gObj : m_gameObjects) gObj->SceneDeactivated();
			if (m_state == SceneState::Deactivate) SetState(SceneState::Removed);
		}
	public:
		virtual void Init() noexcept = 0;
		void Awake() noexcept {
			for (auto* gObj : m_gameObjects) gObj->Awake();
		}
		void FixedUpdate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->FixedUpdate();
			OnFixedUpdate();
		}
		void Update() noexcept {
			for (auto* gObj : m_gameObjects) gObj->Update();
			OnUpdate();
		}
		void LastUpdate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->LastUpdate();
			OnLastUpdate();
		}
		void Draw() noexcept {
			OnPreDraw();
			//ヒープ更新
			for (GameObject* gObj : m_gameObjects) gObj->UpdateAllHeap();
			UpdateHeap();
			Heap sceneHeap = GetHeap();
			//カメラごとに描画
			for (Camera_Impl<GameObject>* camera : GetAllCamera()) {
				Heap cameraHeap = camera->GetHeap();
				RenderTarget renderTarget = camera->GetRenderTarget();
				renderTarget.BeginCommand(true, true, true);
				renderTarget.SetViewport();
				renderTarget.SetScissorRect();
				renderTarget.SetRenderer(Common3D::GetRenderer(Common3D::DefaultRendererName));
				renderTarget.SetHeap(HeapType::Camera, cameraHeap);
				renderTarget.SetHeap(HeapType::Scene, sceneHeap);
				const FlagSet<LayerMask>& cameraLayerMask = camera->GetCullingMask();
				const bool allRendering = cameraLayerMask == LayerMask::All;
				for (GameObject* gObj : m_gameObjects) {
					if (!gObj->template HasComponent<MeshFilter_Impl<GameObject>>()) continue;
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
		void Resized(const ResizeEventArgs& e) noexcept {
			//for (auto* gObj : m_gameObjects) gObj->OnResized(e);
			OnResized(e);
		}
		void Closing(CloseEventArgs& e) noexcept { OnClosing(e); }
		void Closed() noexcept { OnClosed(); }
		void CloseCanceled() noexcept { OnCloseCanceled(); }
		void Activate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->SceneActivate();
			OnActivate();
		}
		void Deactivate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->SceneDeactivate();
			OnDeactivate();
		}
		void BeginActivate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->SceneBeginActivate();
			OnBeginActivate();
		}
		void BeginDeactivate() noexcept {
			for (auto* gObj : m_gameObjects) gObj->SceneBeginDeactivate();
			OnBeginDeactivate();
		}
	public:
		virtual void OnBeginActivate() noexcept {}
		virtual void OnActivate() noexcept { EndActivate(); }
		virtual void OnEndActivate() noexcept {}
		virtual void OnActivated() noexcept {}
		virtual void OnBeginDeactivate() noexcept {}
		virtual void OnDeactivate() noexcept { EndDeactivate(); }
		virtual void OnEndDeactivate() noexcept {}
		virtual void OnFixedUpdate() noexcept {}
		virtual void OnUpdate() noexcept {}
		virtual void OnLastUpdate() noexcept {}
		virtual void OnPreDraw() noexcept{}
		virtual void OnPostDraw() noexcept{}
		virtual void OnResized(const ResizeEventArgs& e) noexcept {}
		virtual void OnClosing(CloseEventArgs& e) noexcept {}
		virtual void OnClosed() noexcept {}
		virtual void OnCloseCanceled() noexcept {}
	public:
		const String& GetName() const noexcept { return m_name; }
		uint64_t GetFrameCount() const noexcept { return m_frameCount; }
		nanoseconds GetElapsedTimeNs() const noexcept { return m_elapsedTimeNs; }
	public: /* ウィンドウへの描画時に使用するパラメータ */
		void SetDepth(float depth) noexcept { m_depth = System::Math::Clamp(depth, 0.f, 1.f); }
		float GetDepth() const noexcept { return m_depth; }
		void SetAlpha(float alpha) noexcept { m_alpha = System::Math::Clamp(alpha, 0.f, 1.f); }
		float GetAlpha() const noexcept { return m_alpha; }
		void SetBrightness(float brightness) noexcept { m_brightness = System::Math::Clamp(brightness, 0.f, 1.f); }
		float GetBrightness() const noexcept { return m_brightness; }
		void SetVisible(bool visible) noexcept { m_visible = visible; }
		bool GetVisible() const noexcept { return m_visible; }
	public:
		void AddObject(GameObject* object) noexcept {
			if (!object) return;
			if (!m_gameObjects.Contains(object)) {
				m_gameObjects.PushBack(object);
				object->SetScene(this);
			}
			for (Transform_Impl<GameObject>* child : object->GetChildren()) AddObject(&child->GetObject());
		}
		GameObject* GetObject(const String& name) const noexcept {
			GameObject** result = m_gameObjects.TryFirst([&name](GameObject* const& gObj) { return gObj->Name == name; });
			return result ? *result : nullptr;
		}
		List<GameObject*>& GetObjects() noexcept { return m_gameObjects; }
		Vector<Camera_Impl<GameObject>*> GetAllCamera() const noexcept {
			Vector<Camera_Impl<GameObject>*> camera;
			camera.Reserve(5);
			for (GameObject* gObj : m_gameObjects) {
				Camera_Impl<GameObject>* c = gObj->template GetComponentPtr<Camera_Impl<GameObject>>();
				if (c) camera.Add(c);
			}
			camera.Sort(
				true,
				[](Camera_Impl<GameObject>* const& lhs, Camera_Impl<GameObject>* const& rhs) {
					return lhs->GetRenderingOrder() <=> rhs->GetRenderingOrder();
				}
			);
			return camera;
		}
	public:
		Heap GetHeap() const noexcept { return m_sceneHeap; }
		void UpdateHeap() noexcept {
			SceneResourceDesc desc{};
			for (GameObject* gObj : m_gameObjects) {
				Light_Impl<GameObject>* light = gObj->template GetComponentPtr<Light_Impl<GameObject>>();
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
	};
}

//ISceneManager
export namespace Engine {
	class ISceneManager {
	public:
		ISceneManager() noexcept {}
		virtual ~ISceneManager() noexcept {}
	public:
		template<class GameObject>
		static void SetSceneState(Scene_Impl<GameObject>& scene, SceneState state) noexcept {
			scene.SetState(state);
		}
		template<class GameObject>
		static SceneState GetSceneState(Scene_Impl<GameObject>& scene) noexcept {
			return scene.GetState();
		}
		template<class GameObject>
		static void IncrementFrameCount(Scene_Impl<GameObject>& scene) noexcept {
			++scene.m_frameCount;
		}
		template<class GameObject>
		static void AddElapsedTimeNs(Scene_Impl<GameObject>& scene, nanoseconds deltaTime) noexcept {
			scene.m_elapsedTimeNs += deltaTime;
		}
		template<class GameObject>
		static void ResetSceneVariable(Scene_Impl<GameObject>& scene) noexcept {
			scene.m_frameCount = 0;
			scene.m_elapsedTimeNs = nanoseconds(0);
			scene.m_alpha = 1.f;
			scene.m_brightness = 1.f;
			scene.m_depth = 1.f;
			scene.m_visible = true;
		}
	public:
		static ISceneManager& GetSceneManager() noexcept;
	public:
		virtual Timer& GetTimer() noexcept = 0;
	};
}
