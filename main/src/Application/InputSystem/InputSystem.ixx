export module InputSystem;
import System;
export import InputSystemUtility;
import IInputManager;
import ApplicationUtility;

export namespace System::Application {
	class ActionContext {
	public:
		const InputState& State;
		const InputBinding& Binding;
	public:
		ActionContext() noexcept = delete;
		ActionContext(const ActionContext&) noexcept = delete;
		ActionContext(const InputState& state, const InputBinding& binding) noexcept
			: State(state), Binding(binding) {}
		~ActionContext() noexcept = default;
	public:
		Point<int32_t> GetCursorPosOnScreen() const noexcept;
		Point<int32_t> GetCursorPosOnWindow() const noexcept;
	public:
		ActionContext& operator=(const ActionContext&) noexcept = delete;
	};

	class Action {
		String m_name;
	public:
		InputValueType ValueType = InputValueType::Button;
		Vector<InputBinding> Bindings;
		EventHandler<const ActionContext&> Started;
		EventHandler<const ActionContext&> Performed;
		EventHandler<const ActionContext&> Canceled;
	public:
		Action() noexcept = delete;
		Action(const Action&) noexcept = delete;
		Action(Action&&) noexcept = default;
		Action(const String& name, InputValueType type = InputValueType::Button) noexcept : m_name(name), ValueType(type) {}
		~Action() noexcept = default;
	public:
		const String& GetName() noexcept { return m_name; }
		InputBinding& AddBinding(InputBindingType type = InputBindingType::Simple) noexcept {
			InputBinding tmp;
			tmp.Type = type;
			Bindings.Add(tmp);
			return Bindings.Last();
		}
	public:
		Action& operator=(const Action&) noexcept = delete;
		Action& operator=(Action&&) noexcept = default;
	};

	class InputActions {
		HashMap<String, Action> m_actions;
	public:
		InputActions() noexcept;
		virtual ~InputActions() noexcept;
	public:
		bool RegisterAction(Action&& action) noexcept {
			String name = action.GetName();
			return m_actions.Insert(static_cast<String&&>(name), static_cast<Action&&>(action));
		}
		void UnregisterAction(const String& name) noexcept {
			m_actions.Remove(name);
		}
	public:
		System::IEnumerable<Action> EnumerateActions() noexcept {
			return m_actions.Values();
		}
	};
}

export namespace System::Application {
	class InputSystem {
		friend class InputActions;
		Mutex m_actions_mtx;
		Mutex m_manager_mtx;
		Vector<InputActions*> m_registeredInputActions;
		IInputManager* m_manager = nullptr;
	private:
		InputSystem() noexcept;
	public:
		~InputSystem() noexcept {
			delete m_manager;
			m_manager = nullptr;
		}
	public:
		/// <summary>
		/// InputSystemへの共有オブジェクトを取得する。
		/// InputSystemを使用するオブジェクトはこのオブジェクトをメンバにすることでInputSystemが破棄されていないことを保証できる
		/// </summary>
		static ManagedObject<InputSystem> GetReference() noexcept {
			static ManagedObject<InputSystem> ret;
			if (!ret) {
				ret = ManagedObject<InputSystem>(u"SingletonInputSystem", new InputSystem());
			}
			return ret;
		}
	private:
		/// <summary>
		/// InputSystemにInputActionsを登録する。
		/// InputActionsのコンストラクタ以外からは呼び出さない
		/// </summary>
		/// <param name="arg">InputActionsのthisポインタ</param>
		bool Register(InputActions* arg) noexcept {
			LockGuard lock{ m_actions_mtx };
			return m_registeredInputActions.Add(arg) != Vector<InputActions>::ErrorValue;
		}
		/// <summary>
		/// InputSystemに登録されているInputActionsを削除する。
		/// InputActionsのデストラクタ以外からは呼び出さない
		/// </summary>
		/// <param name="arg">InputActionsのthisポインタ</param>
		void Unregister(InputActions* arg) noexcept {
			LockGuard lock{ m_actions_mtx };
			m_registeredInputActions.RemoveValue(arg);
		}
	public:
		void Update() noexcept {
			LockGuard manager_lock{ m_manager_mtx };
			if (!m_manager) return;
			m_manager->Update();
			{
				LockGuard lock{ m_actions_mtx };
				for (InputActions* ia : m_registeredInputActions) {
					for (Action& action : ia->EnumerateActions()) {
						for (InputBinding& binding : action.Bindings) {
							InputState state = m_manager->GetState(action.ValueType, binding);
							ActionContext context{ state, binding };
							if (state.Started) action.Started(context);
							if (state.Performed) action.Performed(context);
							if (state.Canceled) action.Canceled(context);
						}
					}
				}
			}
		}
		bool SetTopLevelWindow(const WindowPlatformData& data) noexcept {
			LockGuard lock{ m_manager_mtx };
			return m_manager ? m_manager->SetTopLevelWindow(data) : false;
		}
		Point<int32_t> GetCursorPosOnScreen() noexcept {
			LockGuard lock{ m_manager_mtx };
			return m_manager ? m_manager->GetCursorPosOnScreen() : Point<int32_t>{ 0, 0 };
		}
		Point<int32_t> GetCursorPosOnWindow() noexcept {
			LockGuard lock{ m_manager_mtx };
			return m_manager ? m_manager->GetCursorPosOnWindow() : Point<int32_t>{ 0, 0 };
		}
	};

	//class InputSystem {
	//	friend class InputActions;
	//	inline static Mutex actions_mtx;
	//	static Vector<InputActions*>& GetRegisteredInputActions() noexcept {
	//		static Vector<InputActions*> ret;
	//		return ret;
	//	}
	//	inline static Mutex manager_mtx;
	//	static IInputManager& GetManager() noexcept;
	//	static bool Register(InputActions* arg) noexcept {
	//		LockGuard lock{ actions_mtx };
	//		return GetRegisteredInputActions().Add(arg) != Vector<InputActions>::ErrorValue;
	//	}
	//	static void Unregister(InputActions* arg) noexcept {
	//		LockGuard lock{ actions_mtx };
	//		GetRegisteredInputActions().RemoveValue(arg);
	//	}
	//public:
	//	static void Update() noexcept {
	//		LockGuard manager_lock{ manager_mtx };
	//		IInputManager& manager = GetManager();
	//		manager.Update();
	//		{
	//			LockGuard lock{ actions_mtx };
	//			for (InputActions* ia : GetRegisteredInputActions()) {
	//				for (Action& action : ia->EnumerateActions()) {
	//					for (InputBinding& binding : action.Bindings) {
	//						InputState state = manager.GetState(action.ValueType, binding);
	//						ActionContext context{ state, binding };
	//						if (state.Started) action.Started(context);
	//						if (state.Performed) action.Performed(context);
	//						if (state.Canceled) action.Canceled(context);
	//					}
	//				}
	//			}
	//		}
	//	}
	//	static bool SetTopLevelWindow(const WindowPlatformData& data) noexcept {
	//		LockGuard lock{ manager_mtx };
	//		return GetManager().SetTopLevelWindow(data);
	//	}
	//	static Point<int32_t> GetCursorPosOnScreen() noexcept {
	//		LockGuard lock{ manager_mtx };
	//		return GetManager().GetCursorPosOnScreen();
	//	}
	//	static Point<int32_t> GetCursorPosOnWindow() noexcept {
	//		LockGuard lock{ manager_mtx };
	//		return GetManager().GetCursorPosOnWindow();
	//	}
	//};
}

namespace System::Application {
	InputActions::InputActions() noexcept {
		InputSystem::GetReference()->Register(this);
	}
	InputActions::~InputActions() noexcept {
		InputSystem::GetReference()->Unregister(this);
	}

	Point<int32_t> ActionContext::GetCursorPosOnScreen() const noexcept {
		return InputSystem::GetReference()->GetCursorPosOnScreen();
	}
	Point<int32_t> ActionContext::GetCursorPosOnWindow() const noexcept {
		return InputSystem::GetReference()->GetCursorPosOnWindow();
	}
}
