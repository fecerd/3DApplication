module InputSystem;
import NamespaceHelper;
import <mutex>;	//clang++で到達不可能のエラーが出るためつけておく

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
import DirectInput;
namespace System::Application {
	InputSystem::InputSystem() noexcept {
		m_manager = Windows::DirectInputManager::CreateManager();
	}
}
#else	//WindowsでもSDLでもない場合

#endif
#else	//SDL
namespace System::Application {
	InputSystem::InputSystem() noexcept {
		m_manager = nullptr;
	}
}
#endif