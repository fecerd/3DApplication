export module Script;
export import Component;
export import EventArgs;
import System;
import Scene;	//ISceneManager
import NamespaceHelper;
using namespace System;
using namespace System::Application;
using namespace Engine;

export namespace Engine {
	template<class GameObject>
	class Script_Impl : public Component_Impl<GameObject> {
		/// @brief 派生先のScriptを作成する関数
		Function<Script_Impl<GameObject>*(GameObject*)> m_clone;
		/// @brief 派生先のScriptのTypeID
		uint32_t m_typeID = GetID<Script_Impl<GameObject>>();
	public:
		Script_Impl() noexcept = delete;
		Script_Impl(const Script_Impl&) noexcept = default;
		Script_Impl(Script_Impl&&) noexcept = default;
		Script_Impl(GameObject* gObj) noexcept : Component_Impl<GameObject>(gObj) {}
		Script_Impl(GameObject* gObj, const Function<Script_Impl<GameObject>*(GameObject*)>& clone, uint32_t typeID) noexcept
			: Component_Impl<GameObject>(gObj), m_clone(clone), m_typeID(typeID) {}
		virtual ~Script_Impl() noexcept {}
	public:/* 各種関数の有効/無効を設定する変数 */
		bool EnableAwake = false;
		bool EnableSceneBeginActivate = false;
		bool EnableSceneActivate = false;
		bool EnableSceneActivated = false;
		bool EnableSceneBeginDeactivate = false;
		bool EnableSceneDeactivate = false;
		bool EnableSceneDeactivated = false;
		bool EnableUpdate = false;
		bool EnableLastUpdate = false;
		bool EnableFixedUpdate = false;
		bool EnableKeyDown = false;
		bool EnableKeyUp = false;
		bool EnableInput = false;
		bool EnableWindowResized = false;
	public:/* 派生先でoverrideされるイベント関数 */
		virtual void Init() noexcept {}
		virtual void Awake() noexcept {}
		virtual void SceneBeginActivate() noexcept {}
		virtual void SceneActivate() noexcept {}
		virtual void SceneActivated() noexcept {}
		virtual void SceneBeginDeactivate() noexcept {}
		virtual void SceneDeactivate() noexcept {}
		virtual void SceneDeactivated() noexcept {}
		virtual void Update() noexcept {}
		virtual void LastUpdate() noexcept {}
		virtual void FixedUpdate() noexcept {}
		virtual void KeyDown(const KeyEventArgs& e) noexcept {}
		virtual void KeyUp(const KeyEventArgs& e) noexcept {}
		virtual void Input(const InputEventArgs& e) noexcept {}
		virtual void WindowResized(const ResizeEventArgs& e) noexcept {}
		virtual void Clicked() noexcept {}
	public:
		/// @brief アプリケーション共通のTimerを取得する。
		static Timer& GetTimer() noexcept { return ISceneManager::GetSceneManager().GetTimer(); }
	public:
		Type GetType() const noexcept override { return Type::CreateType<Script_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"Script Object"); }
		uint32_t GetTypeID() const noexcept final { return m_typeID; }
		/// @brief このScriptの実体と同じ派生クラスのコピーを作成する。
		/// @param object 作成されるScriptが参照するGameObject
		/// @return newで作成されたScript。呼び出し側でInit()を呼び出す必要がある。
		Script_Impl<GameObject>* Clone(GameObject* object) noexcept override {
			if (m_clone) {
				return new Script_Impl<GameObject>(object);
			}
			else {
				Script_Impl<GameObject>* ret = m_clone(object);
				ret->m_clone = m_clone;
				ret->m_typeID = m_typeID;
				//Initを呼び出す必要がある
				return ret;
			}
		}
	public:
		Script_Impl& operator=(const Script_Impl&) noexcept = default;
		Script_Impl& operator=(Script_Impl&&) noexcept = default;
	};
}
