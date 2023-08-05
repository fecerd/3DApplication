export module Col;
import CSTDINT;
import Traits;
import Function;
import <exception>;
import <coroutine>;
using namespace System;

export namespace Test {
	template<class T>
	struct ColIterator;

	template<class T>
	struct Col {
		struct promise_type {
			T* value;
			Col<T> get_return_object() { return Col<T>(std::coroutine_handle<Col<T>::promise_type>::from_promise(*this)); }
			std::suspend_always initial_suspend() { return std::suspend_always(); }
			std::suspend_always final_suspend() noexcept { return std::suspend_always(); }
			void unhandled_exception() { std::terminate(); }
			void return_void() {}
			std::suspend_always yield_value(T& value) {
				this->value = &value;
				return std::suspend_always();
			}
		};
	protected:
		using handle = std::coroutine_handle<Col<T>::promise_type>;
	public:
		handle m_handle = handle();
		Function<Col<T>(bool)> m_enumFunc;
		bool m_reverse = false;
		bool m_done = false;
	public:
		constexpr Col() noexcept = delete;
		constexpr Col(handle h) noexcept
		    : m_handle(h) {}
		template<Traits::Concepts::CConstructibleTo<Function<Col<T>(bool)>> F>
		constexpr Col(F&& getEnumFunc, bool reverse = false) noexcept
		    : m_enumFunc(System::move(getEnumFunc))
		    , m_reverse(reverse) {
			Col&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		template<Traits::Concepts::CConstructibleTo<Function<Col<T>(bool)>> F>
		constexpr Col(F const& getEnumFunc, bool reverse = false) noexcept
		    : m_enumFunc(getEnumFunc)
		    , m_reverse(reverse) {
			Col&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr Col(const Col& arg) noexcept
		    : Col(arg.m_enumFunc, arg.m_reverse) {}
		constexpr Col(Col&& arg) noexcept
		    : m_handle(arg.m_handle)
		    , m_enumFunc(System::move(arg.m_enumFunc))
		    , m_reverse(arg.m_reverse)
		    , m_done(arg.m_done) {
			arg.m_handle = handle();
			arg.m_done = true;
		}
		constexpr ~Col() noexcept {
			if (m_handle) m_handle.destroy();
			m_handle = handle();
			m_done = true;
		}
	private:
		void Reset(bool reverse) noexcept {
			if (m_handle) m_handle.destroy();
			m_reverse = reverse;
			Col&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			m_done = false;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
	public:
		constexpr ColIterator<T> begin() noexcept;
		constexpr ColIterator<T> end() const noexcept;
		constexpr Col& operator++() noexcept {
			if (m_handle) {
				m_handle.resume();
				m_done = m_handle.done();
			}
			return *this;
		}
		constexpr T& operator*() noexcept {
			return *m_handle.promise().value;
		}
	public:
		constexpr bool Done() const noexcept { return m_done; }
		constexpr Col<T>* Clone() noexcept {
			return new Col<T>(m_enumFunc, m_reverse);
		}
		constexpr void Reverse() noexcept {
			Reset(!m_reverse);
		}
	};

	template<class T>
	struct ColIterator {
	public:
		Col<T>* m_obj = nullptr;
	public:
		constexpr ColIterator() noexcept = default;
		constexpr ColIterator(Col<T>* obj) noexcept
		    : m_obj(obj) {}
		constexpr ~ColIterator() noexcept { m_obj = nullptr; }
	public:
		constexpr bool Done() const noexcept {
			return m_obj ? m_obj->Done() : true;
		}
	public:
		constexpr ColIterator& operator++() noexcept {
			if (m_obj) m_obj->operator++();
			return *this;
		}
		constexpr T& operator*() const {
			return m_obj->operator*();
		}
		constexpr bool operator==(const ColIterator& rhs) const noexcept {
			return Done() == rhs.Done();
		}
		constexpr bool operator!=(const ColIterator& rhs) const noexcept {
			return Done() != rhs.Done();
		}
		constexpr ColIterator& operator=(const ColIterator& rhs) noexcept {
			if (this == &rhs) return *this;
			m_obj = rhs.m_obj;
			return *this;
		}
	};

	template<class T>
	constexpr ColIterator<T> Col<T>::begin() noexcept {
		if (m_handle) Reset(m_reverse);
		operator++();
		return ColIterator<T>(this);
	}
	template<class T>
	constexpr ColIterator<T> Col<T>::end() const noexcept {
		return ColIterator<T>();
	}

	Col<double> col_func() {
		double ret = 0;
		for (int i = 0; i < 10; ++i, ret += 0.5)
			co_yield ret;
	}

	auto CreateCol() noexcept -> decltype(col_func()) {
		using ret_type = decltype(col_func());
		return ret_type([](bool reverse) { return col_func(); }, false);
	}
}