export module Counter;
import Traits;
import Thread;

namespace System {
	class MutexWrapper {
		mutable Mutex m_mtx;
	public:
		Mutex& GetMutex() const noexcept { return m_mtx; }
	};
	class DummyMutexWrapper {
	public:
		Mutex& GetMutex() const noexcept;
	};
}

export namespace System {
	template<Traits::Concepts::CIncrementable T, bool ThreadSafe = false>
	class Counter : Traits::conditional_t<ThreadSafe, MutexWrapper, DummyMutexWrapper> {
	protected:
		T m_value;
	public:
		constexpr Counter() noexcept : m_value() {}
		constexpr Counter(const T& val) noexcept : m_value(val) {}
		constexpr Counter(T&& val) noexcept : m_value(System::move(val)) {}
		constexpr Counter(const Counter<T, ThreadSafe>&) noexcept = default;
		constexpr Counter(Counter<T, ThreadSafe>&&) noexcept = default;
		constexpr ~Counter() noexcept = default;
	public:
		constexpr T const& GetValue() const noexcept {
			if constexpr (ThreadSafe) {
				LockGuard lock{ this->GetMutex() };
				return m_value;
			}
			else return m_value;
		}
		constexpr void Increment() noexcept {
			if constexpr (ThreadSafe) {
				LockGuard lock{ this->GetMutex() };
				++m_value;
			}
			else ++m_value;
		}
		constexpr void Decrement() noexcept requires(Traits::Concepts::CDecrementable<T>) {
			if constexpr (ThreadSafe) {
				LockGuard lock{ this->GetMutex() };
				--m_value;
			}
			else --m_value;
		}
	public:
		Counter& operator=(const Counter<T, ThreadSafe>&) noexcept = default;
		Counter& operator=(Counter<T, ThreadSafe>&&) noexcept = default;
	};

	template<Traits::Concepts::CIncrementable T>
	class RangeCounter : public Counter<T> {
		T m_start;
		T m_end;
	public:
		constexpr RangeCounter() noexcept = delete;
		constexpr RangeCounter(T const& val, T const& startVal, T const& endValue) noexcept
			: Counter<T>(val), m_start(startVal), m_end(endValue) {}
		constexpr RangeCounter(T&& val, T&& startVal, T&& endValue) noexcept
			: Counter<T>(System::move(val)), m_start(System::move(startVal)), m_end(System::move(endValue)) {}
		constexpr RangeCounter(const RangeCounter<T>&) noexcept = default;
		constexpr RangeCounter(RangeCounter<T>&&) noexcept = default;
		constexpr ~RangeCounter() noexcept = default;
	public:
		constexpr void Increment() noexcept {
			if (++this->m_value > m_end) this->m_value = m_start;
		}
		constexpr void Decrement() noexcept {
			if (this->m_value == m_start) this->m_value = m_end;
			else --this->m_value;
		}
	public:
		constexpr RangeCounter& operator=(const RangeCounter<T>&) noexcept = default;
		constexpr RangeCounter& operator=(RangeCounter<T>&&) noexcept = default;
	};
}
