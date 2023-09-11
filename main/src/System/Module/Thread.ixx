export module Thread;
import CSTDINT;
import Traits;
import Function;
export import <mutex>;	//std::mutex, std::lock_guard<>, std::unique_lock<>
export import <condition_variable>;	//std::condition_variable

//g++のみboost::threadを使用する
#if defined(__GNUC__) && !defined(__clang__)
import BoostThread;
using internal_thread = System::Boost::Thread;
#else
import <thread>;	//std::thread
using internal_thread = std::thread;
#endif

//Thread
export namespace System {
	class Thread {
		internal_thread m_thread = internal_thread();
	public:
		Thread() noexcept = default;
		template<class R, class ...Args>
		Thread(const Function<R(Args...)>& func, Args&& ...args) noexcept : m_thread(func, static_cast<Args&&>(args)...) {}
		template<class Functor, class ...Args>
		Thread(Functor&& f, Args&& ...args) noexcept : m_thread(Function<Traits::function_t<Functor>>(System::move(f)), static_cast<Args&&>(args)...) {}
		Thread(const Thread&) noexcept = delete;
		Thread(Thread&&) noexcept = default;
		~Thread() noexcept = default;
	public:
		bool Joinable() const noexcept { return m_thread.joinable(); }
		bool joinable() const noexcept { return Joinable(); }
		void Join() noexcept { if (m_thread.joinable()) m_thread.join(); }
		void join() noexcept { Join(); }
	public:
		Thread& operator=(const Thread&) noexcept = delete;
		Thread& operator=(Thread&&) noexcept = default;
	};
}

//Mutex, ConditionVariable
export namespace System {
	using Mutex = std::mutex;
	using RecursiveMutex = std::recursive_mutex;
	using ConditionVariable = std::condition_variable;
}

//LockGuard, UniqueLock
export namespace System {
	template<class M>
	class LockGuard : public std::lock_guard<M> {
	public:
		using std::lock_guard<M>::lock_guard;
	public:
		operator std::lock_guard<M>&() noexcept {
			return static_cast<std::lock_guard<M>&>(*this);
		}
	};
	template<class M>
	LockGuard(M&)->LockGuard<M>;

	template<class M>
	class UniqueLock : public std::unique_lock<M> {
	public:
		using std::unique_lock<M>::unique_lock;
	public:
		operator std::unique_lock<M>&() noexcept {
			return static_cast<std::unique_lock<M>&>(*this);
		}
	};
	template<class M>
	UniqueLock(M&)->UniqueLock<M>;
}

//SleepFor
export namespace System {
	template<class T>
	void SleepFor(T const& rel_time) {
		std::this_thread::sleep_for(rel_time);
	}
}
