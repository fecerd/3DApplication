export module GameObject;
export import Component;
export import Transform;
export import Script;
export import Camera;
export import Animator;
export import Light;
export import MeshFilter;
export import Rigidbody;
export import LayerMask;
export import Scene;
import System;
import Application;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;

//クラス名定義
export namespace Engine {
	class GameObject;
	using Component = Component_Impl<GameObject>;
	using Animator = Animator_Impl<GameObject>;
	using AnimationController = AnimationController_Impl<GameObject>;
	using Camera = Camera_Impl<GameObject>;
	using Light = Light_Impl<GameObject>;
	using MeshFilter = MeshFilter_Impl<GameObject>;
	using Rigidbody = Rigidbody_Impl<GameObject>;
	using Script = Script_Impl<GameObject>;
	using Transform = Transform_Impl<GameObject>;
	using Scene = Scene_Impl<GameObject>;
}

//コンセプト
namespace Engine {
	template<class S> concept CScript = Traits::is_base_of_v<Script, S>;
	template<class C> concept CComponent = Traits::is_base_of_v<Component, C> && !CScript<C>;
}

//GameObject
export namespace Engine {
	class GameObject {
	protected:
		Vector<Component*> m_components;
		Vector<Script*> m_scripts;
		/// @brief m_componentsが保持しているTransformコンポーネントへのポインタ
		Transform* m_transform = nullptr;
		Scene* m_scene = nullptr;
	protected:
		Resource m_objectResource;
		Heap m_objectHeap;
		CommandList m_commandList;
	public:
		String Name;
		FlagSet<LayerMask> Layer = LayerMask::Default;
	public:
		GameObject() noexcept : GameObject(String::Empty()) {}
		GameObject(const GameObject&) noexcept = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject(const String& name) noexcept : Name(name) {
			m_components.Reserve(5);
			m_transform = &AddComponent<Transform>();
		}
		~GameObject() noexcept {
			m_scripts.DeleteAll();
			m_components.DeleteAll();
			m_transform = nullptr;
			m_scene = nullptr;
		}
	public:
		template<CComponent C>
		C* GetComponentPtr() const noexcept {
			uint32_t id = System::GetID<C>();
			for (Component* c : m_components) {
				if (c->GetTypeID() == id) return static_cast<C*>(c);
			}
			return nullptr;
		}
		template<CComponent C>
		C& AddComponent() noexcept {
			C* tmp = GetComponentPtr<C>();
			if (tmp) return *tmp;
			tmp = new C(this);
			m_components.Add(tmp);
			return *tmp;
		}
		template<CComponent C>
		C& GetComponent() const {
			C* tmp = GetComponentPtr<C>();
			if (tmp) return *tmp;
			throw InvalidOperationException(u"保持していないComponentが要求されました。");
		}
		template<CComponent C>
		bool HasComponent() const noexcept { return GetComponentPtr<C>(); }
		template<CComponent C>
		bool RemoveComponent() noexcept {
			C* tmp = GetComponentPtr<C>();
			if (!tmp) return false;
			m_components.RemoveValue(tmp);
			Component* comp = tmp;
			delete comp;
			return true;
		}
		template<CScript S>
		S* GetScriptPtr() const noexcept {
			uint32_t id = System::GetID<S>();
			for (Script* s : m_scripts) {
				if (s->GetTypeID() == id) return static_cast<S*>(s);
			}
			return nullptr;
		}
		template<CScript S>
		S& AddScript() noexcept {
			S* tmp = GetScriptPtr<S>();
			if (tmp) return *tmp;
			tmp = new S(this, [](GameObject* gObj) { return new S(gObj); }, System::GetID<S>());
			tmp->Init();
			m_scripts.Add(tmp);
			return *tmp;
		}
		template<CScript S>
		S& GetScript() const {
			S* tmp = GetScriptPtr<S>();
			if (tmp) return *tmp;
			throw InvalidOperationException(u"保持していないScriptが要求されました。");
		}
		template<CScript S>
		bool HasScript() const noexcept { return GetScriptPtr<S>(); }
		template<CScript S>
		bool RemoveScript() noexcept {
			S* tmp = GetScriptPtr<S>();
			if (!tmp) return false;
			m_scripts.RemoveValue(tmp);
			delete tmp;
			return true;
		}
	public:
		Transform& GetTransform() const noexcept { return *m_transform; }
		void SetScene(Scene* scene) noexcept { m_scene = scene; }
		Scene* GetScene() const noexcept { return m_scene; }
	public:
		void SetParent(GameObject* parent) noexcept {
			m_transform->SetParent(parent ? parent->m_transform : nullptr);
		}
		GameObject* GetParent() noexcept {
			Transform* parent = m_transform->GetParent();
			return parent ? &parent->GetObject() : nullptr;
		}
		const Vector<Transform*>& GetChildren() const noexcept {
			return m_transform->GetChildren();
		}
	public:
		void Awake() noexcept {
			for (Script* s : m_scripts) if (s->EnableAwake) s->Awake();
		}
		void SceneActivate() noexcept {
			for (Script* s : m_scripts) if (s->EnableSceneActivate) s->SceneActivate();
		}
		void SceneDeactivate() noexcept {
			for (Script* s : m_scripts) if (s->EnableSceneDeactivate) s->SceneDeactivate();
		}
		void Update() noexcept {
			for (Script* s : m_scripts) if (s->EnableUpdate) s->Update();
		}
		void LastUpdate() noexcept {
			for (Script* s : m_scripts) if (s->EnableLastUpdate) s->LastUpdate();
			Animator* animator = GetComponentPtr<Animator>();
			if (animator) animator->Update();
		}
		void FixedUpdate() noexcept {
			for (Script* s : m_scripts) if (s->EnableFixedUpdate) s->FixedUpdate();
			Rigidbody* rigidbody = GetComponentPtr<Rigidbody>();
			if (rigidbody) rigidbody->FixedUpdate();
		}
		void KeyDown(const KeyEventArgs& e) noexcept {
			for (Script* s : m_scripts) if (s->EnableKeyDown) s->KeyDown(e);
		}
		void KeyUp(const KeyEventArgs& e) noexcept {
			for (Script* s : m_scripts) if (s->EnableKeyUp) s->KeyUp(e);
		}
		void Input(const InputEventArgs& e) noexcept {
			for (Script* s : m_scripts) if (s->EnableInput) s->Input(e);
		}
		void WindowResized(const ResizeEventArgs& e) noexcept {
			for (Script* s : m_scripts) if (s->EnableWindowResized) s->WindowResized(e);
		}
	public:
		void UpdateHeap() noexcept {
			Common3D::ObjectResourceDesc desc{};
			desc.world = m_transform->World();
			if (!m_objectHeap) m_objectHeap = CreateHeap(String::Empty(), Common3D::HeapType::Object);
			if (!m_objectHeap) return;
			if (!m_objectResource) {
				m_objectResource = Common3D::CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
				m_objectHeap.SetView(0, ViewFormat::CBV, m_objectResource);
			}
			else m_objectResource.UpdateResource(desc.GetData(), desc.GetCount());
		}
		Heap GetHeap() const noexcept { return m_objectHeap; }
		void UpdateAllHeap() noexcept;
		bool UpdateCommandList() noexcept;
		CommandList GetCommandList() noexcept {
			if (m_commandList) return m_commandList;
			UpdateCommandList();
			return m_commandList;
		}
	};
}
