module IWindow;
import Application;
import IApplicationManager;
import InputSystem;
using namespace System;

namespace System::Application {
	IWindow::IWindow(const WindowDesc& desc) noexcept : m_name(desc.Name) {
		System::Application::AddWindow(this, desc);
	}
	void IWindow::Show() noexcept { System::Application::ShowWindow(this); }
	bool IWindow::Close() noexcept { return System::Application::CloseWindow(this); }
	Point<int32_t> IWindow::GetPos() const noexcept {
		return System::Application::GetManager().GetWindowPos(this);
	}
	Size<int32_t> IWindow::GetSize() const noexcept {
		return System::Application::GetManager().GetWindowSize(this);
	}
	void IWindow::SetTitle(const String& str) noexcept {
		System::Application::GetManager().SetWindowTitle(this, str);
	}
	WindowPlatformData IWindow::GetPlatformData() const noexcept {
		return System::Application::GetManager().GetWindowData(this);
	}
	void IWindow::SetInputFocus() noexcept {
		InputSystem::GetReference()->SetTopLevelWindow(GetPlatformData());
	}
}

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
namespace System::Application {
}
#else	//WindowsでもSDLでもない場合

#endif
#else	//SDL
namespace System::Application {
}
#endif