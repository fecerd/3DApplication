#if defined(__GNUC__) && !defined(__clang__)
import IApplicationManager;
#else
module IApplicationManager;
#endif
import NamespaceHelper;

#if !defined(SDL)
#if defined(_WIN32) || defined(_WIN64)

import ControlManager;
namespace System::Application {
	IApplicationManager& Implementation::GetManager_Internal() noexcept {
		return Windows::ControlManager::GetInterface();
	}
}

#else	//WindowsでもSDLでもない場合

#endif
#else	//SDL
import SDLWindowManager;

namespace System::Application {
	IApplicationManager& Implementation::GetManager_Internal() noexcept {
		return SDL::SDLWindowManager::GetInterface();
	}
}

#endif