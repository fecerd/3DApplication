module;
#include <cstdlib>
export module Atexit;

//atexit
export namespace System {
	int atexit(void (*f)(void)) noexcept {
		return std::atexit(f);
	}
}

