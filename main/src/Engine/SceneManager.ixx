export module SceneManager;
import System;
import Common3D;
import Log;
import InputSystem;
export import Scene;
export import Components;	//Scene = Scene_Impl<GameObject>;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;

//コンセプト
export namespace Engine {
	template<class T> concept IsScene = Traits::is_base_of_v<Scene_Impl<GameObject>, T>;
}

//SceneManager
export namespace Engine {
	class SceneManager : public ISceneManager {
		static constexpr uint32_t FenceLevel = 0;
		static constexpr uint32_t UpdateLevel = 1;
		static constexpr uint32_t DrawLevelBase = 2;
		static constexpr uint32_t LoadingLevel = 0x10000;
		static constexpr uint32_t UnLoadingLevel = 0x10001;
		static constexpr uint32_t ActivateLevel = 0x10002;
	private:
		Timer m_timer;
		TaskWorker* m_loader = nullptr;
		TaskFuture<bool, bool> m_fence;
		List<Scene*> m_loadedScenes;
		List<Scene*> m_activeScenes;
		mutable RecursiveMutex m_mtx;
		float m_prevTimeScale = 1.f;
		Atomic<bool> m_end = false;
		ManagedObject<System::Application::InputSystem> m_inputSystem;
	public:
		static SceneManager& GetManager() noexcept {
			static UniquePtr<SceneManager> ret = nullptr;
			if (!ret) ret = UniquePtr<SceneManager>(new SceneManager());
			return *ret;
		}
	private:
		SceneManager() noexcept {
			m_inputSystem = System::Application::InputSystem::GetReference();
			m_timer.Start();
			m_loader = new TaskWorker(2);
			m_fence = m_loader->Push<bool, bool>(
				[this](TaskPromise<bool, bool>& p) {
					if (!p.HasSuspendValue() || !p.GetSuspendValue()) p.Suspend();
					m_inputSystem->Update();
				},
				FenceLevel, false
			);
			m_fence.Wait(TaskState::Suspended);
			m_loader->Push<bool, void>(
				[this](TaskPromise<bool, void>&) {
					LockGuard lock{ m_mtx };
					m_timer.Update();
					nanoseconds deltaTime = nanoseconds(m_timer.DeltaTime<nanoseconds>());
					uint64_t fixedUpdateCount = m_timer.GetFixedUpdateCount();
					//上限
					if (fixedUpdateCount > 4) fixedUpdateCount = 4;

					for (ListIterator<Scene*> ite = m_activeScenes.begin(), e = m_activeScenes.end(); ite != e;) {
						Scene* scene = *ite;
						IncrementFrameCount(*scene);
						AddElapsedTimeNs(*scene, deltaTime);
						if (GetSceneState(*scene) == SceneState::Added) SetSceneState(*scene, SceneState::Activate);
						if (GetSceneState(*scene) == SceneState::Activate) {
							scene->Activate();
							if (m_end.load(memory_order::acquire)) scene->EndActivate();
						}
						if (GetSceneState(*scene) == SceneState::Deactivate) {
							scene->Deactivate();
							if (m_end.load(memory_order::acquire)) scene->EndDeactivate();
						}
						if (GetSceneState(*scene) == SceneState::Removed) {
							SetSceneState(*scene, SceneState::Loaded);
							ListIterator<Scene*> cur = ite;
							m_activeScenes.Remove(cur);
							++ite;
						}
						else ++ite;
					}
					for (Scene* scene : m_activeScenes) {
						for (uint64_t i = 0; i < fixedUpdateCount; ++i) scene->FixedUpdate();
					}
					for (Scene* scene : m_activeScenes) scene->Update();
					for (Scene* scene : m_activeScenes) scene->LastUpdate();
				},
				UpdateLevel, false
					);
		}
	public:
		~SceneManager() noexcept {
			m_fence.Wait(TaskState::Suspended);
			m_end.store(true, memory_order::release);
			for (Scene* scene : m_loadedScenes) {
				SceneState state = GetSceneState(*scene);
				switch (state) {
				case SceneState::Added:
				case SceneState::Activate:
				case SceneState::Active:
					SetSceneState(*scene, SceneState::Deactivate);
					break;
				default:
					break;
				}
				UnloadScene(scene->GetName());
			}
			do {
				m_fence.Run();
				m_fence.Wait(TaskState::Suspended);
			} while (m_activeScenes.Any() || m_loadedScenes.Any());
			m_fence.SetSuspendValue(true);
			m_fence.Run();
			m_fence = TaskFuture<bool, bool>();
			delete m_loader;
			m_loader = nullptr;			
			delete cameraObj;
			cameraObj = nullptr;
			delete letterboxObj;
			letterboxObj = nullptr;
			for (GameObject* gObj : planeObjects.Values()) delete gObj;
			planeObjects.Clear();
		}
	public:/* ISceneManager override */
		Timer& GetTimer() noexcept override { return m_timer; }
	public:
		void SetTimeScale(float scale) noexcept {
			m_prevTimeScale = m_timer.TimeScale();
			m_timer.TimeScale(scale);
		}
		void RestoreTimeScale() noexcept {
			m_timer.TimeScale(m_prevTimeScale);
		}
	public:
		template<Engine::IsScene T>
		void LoadScene(const String& name) noexcept {
			m_loader->Push<bool, void>(
				[name, this](TaskPromise<bool, void>&) {
					try {
						T* scene = new T(name);
						scene->Init();
						scene->Awake();
						SetSceneState(*scene, SceneState::Loaded);
						{
							LockGuard lock{ m_mtx };
							m_loadedScenes.PushBack(scene);
						}
					}
					catch (std_exception& ex) {
						System::Application::Log(ex.what());
					}
				},
				LoadingLevel, true
			);
		}
		void ActivateScene(const String& name, uint16_t level = 0) noexcept;
		void DeactivateScene(const String& name) noexcept {
			LockGuard lock{ m_mtx };
			Scene** pScene = m_activeScenes.TryFirst([&name](Scene* const& scene) { return scene->GetName() == name; });
			if (!pScene) return;
			Scene* scene = *pScene;
			if (GetSceneState(*scene) != SceneState::Active) return;
			SetSceneState(*scene, SceneState::Deactivate);
		}
		void UnloadScene(const String& name) noexcept {
			Scene* scene = nullptr;
			{
				LockGuard lock{ m_mtx };
				Scene** pScene = m_loadedScenes.TryFirst([&name](Scene* const& scene) { return scene->GetName() == name; });
				if (!pScene) return;
				scene = *pScene;
				SceneState state = GetSceneState(*scene);
				if (state != SceneState::Deactivate && state != SceneState::Removed && state != SceneState::Loaded) return;
			}
			m_loader->Push<bool, void>(
				[this, scene](TaskPromise<bool, void>& p) {
					SceneState state = GetSceneState(*scene);
					if (state == SceneState::Deactivate || state == SceneState::Removed) return;
					else if (state == SceneState::Loaded) {
						SetSceneState(*scene, SceneState::UnLoading);
						{
							LockGuard lock{ m_mtx };
							m_loadedScenes.Remove(m_loadedScenes.FindFront(scene));
						}
						delete scene;
						p.RemoveTask();
					}
					else p.RemoveTask();
				},
				UnLoadingLevel, false
					);
		}
		bool IsActive(const String& name) const noexcept {
			LockGuard lock{ m_mtx };
			return m_activeScenes.TryFirst([&name](Scene* const& scene) { return scene->GetName() == name; });
		}
		bool IsLoaded(const String& name) const noexcept {
			LockGuard lock{ m_mtx };
			return m_loadedScenes.TryFirst([&name](Scene* const& scene) { return scene->GetName() == name; });
		}
	public:
		void Update() noexcept {
			m_fence.Run();
			m_fence.Wait(TaskState::Suspended);
		}
		void Resized(const ResizeEventArgs& e) noexcept {
			LockGuard lock{ m_mtx };
			for (Scene* scene : m_activeScenes) scene->Resized(e);
		}
		void Closing(CloseEventArgs& e) noexcept {
			LockGuard lock{ m_mtx };
			SetTimeScale(0.f);
			for (Scene* scene : m_activeScenes) scene->Closing(e);
		}
		void Closed() noexcept {
			m_timer.Update(false);
			RestoreTimeScale();
		}
		void CloseCanceled() noexcept {
			m_timer.Update(false);
			RestoreTimeScale();
		}
	private:
		static bool Renderable(Scene* scene) noexcept {
			SceneState state = GetSceneState(*scene);
			return state == SceneState::Activate || state == SceneState::Active || state == SceneState::Deactivate;
		}
	public:
		GameObject* cameraObj = nullptr;
		GameObject* letterboxObj = nullptr;
		Heap sceneHeap;
		Resource sceneResource;
		HashMap<Scene*, GameObject*> planeObjects;
		void Rendering(RenderTarget renderTarget) noexcept;
	};
}
