module IApplicationManager;

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
import ControlManager;
import WinAPI;
namespace System::Application {
	IApplicationManager& GetManager() noexcept {
		return System::Application::Windows::ControlManager::GetInterface();
	}
}
#else	//WindowsÇ≈Ç‡SDLÇ≈Ç‡Ç»Ç¢èÍçá

#endif
#else	//SDL
import SDLWindowManager;
namespace System::Application {
	IApplicationManager& GetManager() noexcept {
		return System::Application::SDL::SDLWindowManager::GetInterface();
	}
}
#endif