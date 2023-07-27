module;
#include <thread>
export module Thread;
import Function;
export import <mutex>;
export import <condition_variable>;
using Function = System::Function;

export namespace System {
	class Thread {
		std::thread m_thread = std::thread();
	public:
		Thread() noexcept = default;
		template<class R, class ...Args>
		Thread(const Function<R(Args...)>& func, Args&& ...args) noexcept : m_thread(func, static_cast<Args&&>(args)...) {}
		template<class Functor, class ...Args>
		Thread(Functor&& f, Args&& ...args) noexcept : m_thread(Function(f), static_cast<Args&&>(args)...) {}
		Thread(const Thread&) noexcept = delete;
		Thread(Thread&&) noexcept = default;
		~Thread() noexcept = default;
	public:
		bool Joinable() const noexcept { return m_thread.joinable(); }
		void Join() noexcept { if (m_thread.joinable()) m_thread.join(); }
	public:
		Thread& operator=(const Thread&) noexcept = delete;
		Thread& operator=(Thread&&) noexcept = default;
	};
}

export namespace System {
	using Mutex = std::mutex;
	using RecursiveMutex = std::recursive_mutex;
	//template<class T> using LockGuard = std::lock_guard<T>;
	using UniqueLock = std::unique_lock<Mutex>;
	using ConditionVariable = std::condition_variable;
}

export namespace System {
	template<class T>
	class LockGuard : public std::lock_guard<T> {
	public:
		using std::lock_guard<T>::lock_guard;
	};
}
