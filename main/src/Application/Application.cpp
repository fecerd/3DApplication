module Application;
import NamespaceHelper;

#if !defined(SDL)

#if defined(_WIN32) || defined(_WIN64)
import ControlManager;
import WinAPI;

namespace System::Application::Internal {
	void ReleaseManager() noexcept {
		Windows::ControlManager::ReleaseInterface();
	}
}

#else	//WindowsでもSDLでもない場合

#endif
#else	//SDL

namespace System::Application::Internal {
	void ReleaseManager() noexcept {}
}

#endif
