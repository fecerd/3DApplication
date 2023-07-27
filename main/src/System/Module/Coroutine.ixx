module;
#include<coroutine>
#include<exception>
export module Coroutine;

export namespace std {
	using coroutine_traits = std::coroutine_traits;
}

export namespace System {
	template<class T> using coroutine_handle = std::coroutine_handle<T>;
	using suspend_always = std::suspend_always;

	struct static_promise {
		static std::suspend_always suspend_always() { return std::suspend_always(); }
		static void terminate() { return std::terminate(); }
	};
}