export module EventHandler;
import CSTDINT;
import Traits;
import VectorBase;
import Function;

//EventHandler
export namespace System {
	template<class ...Args>
	class EventHandler {
		using f_t = Function<Traits::make_function_t<void, Args...>>;
		VectorBase<f_t> funcs;
	public:
		EventHandler() noexcept {}
		EventHandler(const EventHandler& arg) noexcept : funcs(arg.funcs) {}
		EventHandler(EventHandler&& arg) noexcept : funcs(System::move(arg.funcs)) {}
		~EventHandler() noexcept {}
	public:
		void Clear() noexcept { funcs.Clear(); }
		template<Traits::Concepts::CConstructibleTo<f_t> Func>
		EventHandler& operator+=(Func&& f) noexcept {
			funcs.Add(f_t(System::forward<Func>(f)));
			return *this;
		}
		template<Traits::Concepts::CConstructibleTo<f_t> Func>
		EventHandler& operator-=(Func&& f) noexcept {
			funcs.RemoveValue(f_t(System::forward<Func>(f)));
			return *this;
		}
		EventHandler& operator=(const EventHandler&) noexcept = delete;
		EventHandler& operator=(EventHandler&&) noexcept = default;
	public:
		template<class ...A>
		requires requires(f_t const& func, A&& ...args) { func(System::forward<A>(args)...); }
		void operator()(A&& ...args) requires(!Traits::is_same_v<void, void, Args...>) {
			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
				if (count != funcs.Count()) return;
				funcs[i].operator()(static_cast<A&&>(args)...);
			}
		}
		void operator()() requires(Traits::is_same_v<void, void, Args...>) {
			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
				if (count != funcs.Count()) return;
				funcs[i].operator()();
			}
		}
	};
}
