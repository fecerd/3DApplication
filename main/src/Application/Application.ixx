export module Application;
import System;
import IApplicationManager;	//GetManager()
export import IWindow;
export import ApplicationEnum;	//MessageBox
export import Log;
using namespace System;

//Application Member
namespace System::Application::Internal {
	//Vector<IWindow*>& Windows() noexcept {
	//	static Vector<IWindow*> ret;
	//	return ret;
	//}
	HashMap<String, IWindow*>& Windows() noexcept {
		static HashMap<String, IWindow*> ret;
		return ret;
	}
	void ReleaseManager() noexcept;
}
using namespace System::Application::Internal;

//Application Function
export namespace System::Application {
	void AddWindow(IWindow* arg, const WindowDesc& desc) noexcept {
		if (!System::Application::GetManager().CreateWindow(arg, desc)) return;
		HashMap<String, IWindow*>& windows = Internal::Windows();
		windows.Insert(desc.Name, arg);
		//Vector<IWindow*>& windows = System::Application::Internal::Windows();
		//windows.Add(arg);
		if (windows.Count() == 1) arg->SetInputFocus();
	}
	IWindow* GetWindow(const String& name) noexcept {
		return Internal::Windows().At(name);
	}
	const HashMap<String, IWindow*>& GetWindows() noexcept {
		return Internal::Windows();
	}
	bool CloseWindow(IWindow* arg) noexcept {
		return System::Application::GetManager().CloseWindow(arg);
	}
	void ShowWindow(IWindow* arg) noexcept {
		System::Application::GetManager().ShowWindow(arg);
	}
	WindowPlatformData GetWindowData(const IWindow* arg) noexcept {
		return System::Application::GetManager().GetWindowData(arg);
	}
	Timer& GetTimer() noexcept {
		return GetManager().GetTimer();
	}
	void SetFPS(uint32_t fps) noexcept { GetManager().SetFPS(fps); }
	int RunLoop(bool release = true) noexcept {
		int ret = GetManager().RunLoop();
		if (release) System::Application::Internal::ReleaseManager();
		return ret;
	}
	MessageBoxResult MessageBox(IWindow* parent, const String& text, const String& title, MessageBoxType type = MessageBoxType::OK) noexcept {
		return System::Application::GetManager().MessageBox(parent, text, title, type);
	}
}
