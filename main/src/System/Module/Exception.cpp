module;
#ifdef _WINDOWS
#include <Windows.h>
#endif
module Exception;
import <iostream>;

namespace System{
	void Terminate(const LogicException &ex) noexcept {
#ifdef _DEBUG
#ifdef _CONSOLE
		std::cout << ex.what() << std::endl;
#elif _WINDOWS
		OutputDebugStringA(ex.what());
		OutputDebugStringA("\n");
#endif
#endif
		std::terminate();
	}

	void Terminate(const Exception &ex) noexcept {
#ifdef _DEBUG
#ifdef _CONSOLE
		std::cout << ex.what() << std::endl;
#elif _WINDOWS
		OutputDebugStringA(ex.what());
		OutputDebugStringA("\n");
#endif
#endif
		std::terminate();
	}
}
