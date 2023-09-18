export module Application;
import System;
import IApplicationManager;	//GetManager()
export import IWindow;
export import ApplicationEnum;	//MessageBox
export import Log;

//Application Member
namespace System::Application::Internal {
	HashMap<String, IWindow*>& GetWindows() noexcept {
		static HashMap<String, IWindow*> ret;
		return ret;
	}
	void ReleaseManager() noexcept;
}

//Application Function
export namespace System::Application {
	void AddWindow(IWindow* arg, const WindowDesc& desc) noexcept {
		if (!GetManager().CreateWindow(arg, desc)) return;
		HashMap<String, IWindow*>& windows = Internal::GetWindows();
		windows.Insert(desc.Name, arg);
		if (windows.Count() == 1) arg->SetInputFocus();
	}
	IWindow* GetWindow(const String& name) noexcept {
		return Internal::GetWindows().At(name);
	}
	const HashMap<String, IWindow*>& GetWindows() noexcept {
		return Internal::GetWindows();
	}
	bool CloseWindow(IWindow* arg) noexcept {
		return GetManager().CloseWindow(arg);
	}
	void ShowWindow(IWindow* arg) noexcept {
		GetManager().ShowWindow(arg);
	}
	WindowPlatformData GetWindowData(const IWindow* arg) noexcept {
		return GetManager().GetWindowData(arg);
	}
	Timer& GetTimer() noexcept {
		return GetManager().GetTimer();
	}
	void SetFPS(uint32_t fps) noexcept { GetManager().SetFPS(fps); }
	int RunLoop(bool release = true) noexcept {
		int ret = GetManager().RunLoop();
		if (release) Internal::ReleaseManager();
		return ret;
	}
	MessageBoxResult MessageBox(IWindow* parent, const String& text, const String& title, MessageBoxType type = MessageBoxType::OK) noexcept {
		return GetManager().MessageBox(parent, text, title, type);
	}
}
