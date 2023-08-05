export module Coroutine;
export import <coroutine>;	//std::coroutine_traits
import <exception>;	//std::terminate

export namespace System {
	template <class T>
	using coroutine_handle = std::coroutine_handle<T>;
	using suspend_always = std::suspend_always;

	struct static_promise {
		static std::suspend_always suspend_always() { return std::suspend_always(); }
		static void terminate() { return std::terminate(); }
	};
}

// export namespace std {
// 	template <class R, class... ArgTypes>
// 	using coroutine_traits = std::coroutine_traits<R, ArgTypes...>;
// 	template <class T>
// 	using coroutine_handle = System::coroutine_handle<T>;
// }
