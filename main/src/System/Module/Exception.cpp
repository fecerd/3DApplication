module;
#include <iostream>
#ifdef _WINDOWS
#include <Windows.h>
#endif
module Exception;

void System::Terminate(const LogicException& ex) noexcept {
#ifdef _DEBUG
#ifdef _CONSOLE
	std::cout << ex.what() << std::endl;
#elif _WINDOWS
	OutputDebugStringA(ex.what());
	OutputDebugStringA('\n');
#endif
#endif
	std::terminate();
}

void System::Terminate(const Exception& ex) noexcept {
#ifdef _DEBUG
#ifdef _CONSOLE
	std::cout << ex.what() << std::endl;
#elif _WINDOWS
	OutputDebugStringA(ex.what());
	OutputDebugStringA('\n');
#endif
#endif
	std::terminate();
}