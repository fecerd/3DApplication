#if defined(__GNUC__) && !defined(__clang__)
import IWindow;
#else
module IWindow;
#endif

import System;
import NamespaceHelper;
import Application;
import IApplicationManager;	//GetManager()
import InputSystem;

namespace System::Application {
	IWindow::IWindow(const WindowDesc& desc) noexcept : m_name(desc.Name) {
		AddWindow(this, desc);
	}
	void IWindow::Show() noexcept { ShowWindow(this); }
	bool IWindow::Close() noexcept { return CloseWindow(this); }
	Point<int32_t> IWindow::GetPos() const noexcept {
		return GetManager().GetWindowPos(this);
	}
	Size<int32_t> IWindow::GetSize() const noexcept {
		return GetManager().GetWindowSize(this);
	}
	void IWindow::SetTitle(const String& str) noexcept {
		GetManager().SetWindowTitle(this, str);
	}
	WindowPlatformData IWindow::GetPlatformData() const noexcept {
		return GetManager().GetWindowData(this);
	}
	void IWindow::SetInputFocus() noexcept {
		InputSystem::GetReference()->SetTopLevelWindow(GetPlatformData());
	}
}

#if !defined(SDL)
#if defined(_WIN32) || defined(_WIN64)
namespace System::Application {}
#else	//WindowsでもSDLでもない場合

#endif
#else	//SDL
namespace System::Application {}
#endif