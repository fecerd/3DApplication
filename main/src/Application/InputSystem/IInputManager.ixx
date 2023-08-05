export module IInputManager;
import System;
export import InputSystemUtility;
export import ApplicationUtility;

export namespace System::Application {
	class IInputManager {
	public:
		virtual ~IInputManager() noexcept {}
	public:
		virtual bool Update() noexcept = 0;
		virtual InputState GetState(InputValueType type, const InputBinding& binding) noexcept = 0;
		virtual bool SetTopLevelWindow(const WindowPlatformData& data) noexcept = 0;
		virtual Point<int32_t> GetCursorPosOnScreen() noexcept = 0;
		virtual Point<int32_t> GetCursorPosOnWindow() noexcept = 0;
	};
}