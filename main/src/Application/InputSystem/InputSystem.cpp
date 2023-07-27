module InputSystem;

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
import DirectInput;
namespace System::Application {
	InputSystem::InputSystem() noexcept {
		m_manager = System::Application::Windows::DirectInputManager::CreateManager();
	}
}
#else	//Windows�ł�SDL�ł��Ȃ��ꍇ

#endif
#else	//SDL
namespace System::Application {
	InputSystem::InputSystem() noexcept {
		m_manager = nullptr;
	}
}
#endif