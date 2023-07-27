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
#else	//Windows�ł�SDL�ł��Ȃ��ꍇ

#endif
#else	//SDL
import SDLWindowManager;
namespace System::Application {
	IApplicationManager& GetManager() noexcept {
		return System::Application::SDL::SDLWindowManager::GetInterface();
	}
}
#endif