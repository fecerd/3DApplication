export module Components:Script;
import :Component;
import System;
export import EventArgs;
using namespace System;
using namespace System::Application;
using namespace Engine;

export namespace Engine { class Script; }

class Engine::Script : public Component {
	Function<Script* (GameObject*)> m_clone;
	uint32_t m_typeID = System::GetID<Script>();
public:
	Script() noexcept = delete;
	Script(GameObject* gObj) noexcept : Component(gObj) {}
	Script(GameObject* gObj, const Function<Script* (GameObject*)>& clone, uint32_t typeID) noexcept
		: Component(gObj), m_clone(clone), m_typeID(typeID) {}
	~Script() noexcept = default;
public:
	bool EnableAwake = false;
	bool EnableSceneActivate = false;
	bool EnableSceneDeactivate = false;
	bool EnableUpdate = false;
	bool EnableLastUpdate = false;
	bool EnableFixedUpdate = false;
	bool EnableKeyDown = false;
	bool EnableKeyUp = false;
	bool EnableInput = false;
	bool EnableWindowResized = false;
public:
	virtual void Init() noexcept {}
	virtual void Awake() noexcept {}
	virtual void SceneActivate() noexcept {}
	virtual void SceneDeactivate() noexcept {}
	virtual void Update() noexcept {}
	virtual void LastUpdate() noexcept {}
	virtual void FixedUpdate() noexcept {}
	virtual void KeyDown(const KeyEventArgs& e) noexcept {}
	virtual void KeyUp(const KeyEventArgs& e) noexcept {}
	virtual void Input(const InputEventArgs& e) noexcept {}
	virtual void WindowResized(const ResizeEventArgs& e) noexcept {}
	virtual void Clicked() noexcept {}
public:
	static Timer& GetTimer() noexcept;
public:
	Type GetType() const noexcept override { return Type::CreateType<Script>(); }
	String ToString() const noexcept override { return String(u"Script Object"); }
	uint32_t GetTypeID() const noexcept final { return m_typeID; }
	Script* Clone(GameObject* object) noexcept override;
};
