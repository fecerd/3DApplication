export module EventHandler;
import CSTDINT;
import Traits;
import Vector;
import Function;

//EventHandler
export namespace System {
	template<class ...Args>
	class EventHandler {
		using f_t = Function<Traits::make_function_t<void, Args...>>;
		Vector<f_t> funcs;
	public:
		EventHandler() noexcept = default;
		EventHandler(const EventHandler&) noexcept = default;
		EventHandler(EventHandler&&) noexcept = default;
	public:
		void Clear() noexcept { funcs.Clear(); }
		template<Traits::Concepts::CConstructibleTo<f_t> Func>
		EventHandler& operator+=(Func&& f) noexcept {
			funcs.Add(f_t(System::move(f)));
			return *this;
		}
		template<Traits::Concepts::CConstructibleTo<f_t> Func>
		EventHandler& operator-=(Func&& f) noexcept {
			funcs.RemoveValue(f_t(System::move(f)));
			return *this;
		}
		EventHandler& operator=(const EventHandler&) noexcept = delete;
		EventHandler& operator=(EventHandler&&) noexcept = default;
	public:
		template<class ...A> requires(Traits::implicit_convertible_pack<Traits::parameter_pack<A...>, Traits::parameter_pack<Args...>>())
		void operator()(A&& ...args) requires(!Traits::is_same_v<void, Args...>) {
			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
				if (count != funcs.Count()) return;
				funcs[i].operator()(static_cast<A&&>(args)...);
			}
		}
		void operator()() requires(Traits::is_same_v<void, Args...>) {
			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
				if (count != funcs.Count()) return;
				funcs[i].operator()();
			}
		}
	};
}
