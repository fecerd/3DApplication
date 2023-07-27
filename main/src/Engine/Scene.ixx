export module Scene;
import System;
import EventArgs;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;

//Scene_Impl
export namespace Engine {
	enum class SceneState : uint8_t {
		Unloaded = 0,	//�����l
		Loaded,	//�ǂݍ��ݍς�
		Added,	//ActiveScene�ɒǉ����ꂽ����(�ŏ��̃t���[����Activate�Ɏ����J��)
		Activate,	//OnActivate()�֐����Ăяo������ԁBEndActivate()�֐���Active�ɑJ��
		Active,	//�ʏ��ActiveScene�̏��
		Deactivate,	//OnDeactivate()�֐����Ăяo������ԁBEndDeactivate()�֐���Removed�ɑJ��
		Removed,	//ActiveScene����폜���ꂽ����(���̃t���[����Loaded�Ɏ����J��)
		UnLoading	//Scene�̃f�X�g���N�^�Ăяo�����
	};

	//Scene <-> GameObject�̏z�Q�Ƃ�؂邽��template�Œ�`
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
		friend class SceneManager;
		void SetState(SceneState state) noexcept { m_state = state; }
		SceneState GetState() const noexcept { return m_state; }
	public:
		void EndActivate() noexcept {
			if (m_state == SceneState::Activate) SetState(SceneState::Active);
		}
		void EndDeactivate() noexcept {
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
		void Draw() noexcept;
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
	public:
		virtual void OnActivate() noexcept { EndActivate(); }
		virtual void OnDeactivate() noexcept { EndDeactivate(); }
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
	public: /* �E�B���h�E�ւ̕`�掞�Ɏg�p����p�����[�^ */
		void SetDepth(float depth) noexcept { m_depth = System::Math::Clamp(depth, 0.f, 1.f); }
		float GetDepth() const noexcept { return m_depth; }
		void SetAlpha(float alpha) noexcept { m_alpha = System::Math::Clamp(alpha, 0.f, 1.f); }
		float GetAlpha() const noexcept { return m_alpha; }
		void SetBrightness(float brightness) noexcept { m_brightness = System::Math::Clamp(brightness, 0.f, 1.f); }
		float GetBrightness() const noexcept { return m_brightness; }
		void SetVisible(bool visible) noexcept { m_visible = visible; }
		bool GetVisible() const noexcept { return m_visible; }
	public:
		void AddObject(GameObject* object) noexcept;
		GameObject* GetObject(const String& name) const noexcept {
			GameObject** result = m_gameObjects.TryFirst([&name](GameObject* const& gObj) { return gObj->Name == name; });
			return result ? *result : nullptr;
		}
		List<GameObject*>& GetObjects() noexcept { return m_gameObjects; }
		Vector<typename GameObject::camera_type*> GetAllCamera() const noexcept;
	public:
		Heap GetHeap() const noexcept { return m_sceneHeap; }
		void UpdateHeap() noexcept;
	};
}
