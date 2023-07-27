module Application;
using namespace System;

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
import ControlManager;
import WinAPI;
namespace System::Application::Internal {
	void ReleaseManager() noexcept {
		System::Application::Windows::ControlManager::ReleaseInterface();
	}
}
#else	//WindowsÇ≈Ç‡SDLÇ≈Ç‡Ç»Ç¢èÍçá

#endif
#else	//SDL
namespace System::Application::Internal {
	void ReleaseManager() noexcept {

	}
}
#endif