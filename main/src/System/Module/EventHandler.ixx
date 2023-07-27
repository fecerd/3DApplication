export module EventHandler;
import CSTDINT;
import Traits;
import Vector;
import Function;

//EventHandler
export namespace System {
	template<class ...Args>
	class EventHandler {
		using f_t = Function<void(Args...)>;
		Vector<f_t> funcs;
	public:
		EventHandler() noexcept = default;
		EventHandler(const EventHandler&) noexcept = default;
		EventHandler(EventHandler&&) noexcept = default;
	public:
		void Clear() noexcept { funcs.Clear(); }
		template<System::Traits::Constructible<f_t> Func>
		EventHandler& operator+=(Func&& f) noexcept {
			funcs.Add(f_t(System::move(f)));
			return *this;
		}
		template<System::Traits::Constructible<f_t> Func>
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
				funcs[i](static_cast<A&&>(args)...);
			}
		}
		void operator()() requires(Traits::is_same_v<void, Args...>) {
			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
				if (count != funcs.Count()) return;
				funcs[i]();
			}
		}
	};
}

//export namespace System {
//	template<class ...Args>
//	class EventHandler {
//		Vector<Function<void(Args...)>> funcs;
//	public:
//		EventHandler() noexcept = default;
//		EventHandler(const EventHandler&) noexcept = default;
//		EventHandler(EventHandler&&) noexcept = default;
//	public:
//		void Clear() noexcept { funcs.Clear(); }
//	public:
//		EventHandler& operator+=(Function<void(Args...)>&& f) noexcept {
//			funcs.Add(static_cast<Function<void(Args...)>&&>(f));
//			return *this;
//		}
//		template<class Func>
//		requires requires() {
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, void(Args...)>;
//			System::Traits::is_invocable_r_v<void, Func, Args&&...>;
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, Function<void(Args...)>>;
//		}
//		EventHandler& operator+=(Func&& f) noexcept {
//			funcs.Add(static_cast<Func&&>(f));
//			return *this;
//		}
//	public:
//		EventHandler& operator-=(Function<void(Args...)>&& f) noexcept {
//			funcs.Remove(static_cast<Function<void(Args...)>&&>(f));
//			return *this;
//		}
//		template<class Func>
//		requires requires() {
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, void(Args...)>;
//			System::Traits::is_invocable_r_v<void, Func, Args&&...>;
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, Function<void(Args...)>>;
//		}
//		EventHandler& operator-=(Func&& f) noexcept {
//			funcs.Remove(static_cast<Func&&>(f));
//			return *this;
//		}
//		EventHandler& operator=(const EventHandler&) noexcept = delete;
//		EventHandler& operator=(EventHandler&&) noexcept = default;
//	public:
//		void operator()(Args&& ...args) {
//			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
//				if (count != funcs.Count()) return;
//				funcs[i](static_cast<Args&&>(args)...);
//			}
//		}
//	};
//
//	template<>
//	class EventHandler<void> {
//		Vector<Function<void(void)>> funcs;
//	public:
//		EventHandler() noexcept = default;
//		EventHandler(const EventHandler&) noexcept = default;
//		EventHandler(EventHandler&&) noexcept = default;
//	public:
//		void Clear() noexcept { funcs.Clear(); }
//	public:
//		EventHandler& operator+=(Function<void(void)> && f) noexcept {
//			funcs.Add(static_cast<Function<void(void)>&&>(f));
//			return *this;
//		}
//		template<class Func>
//		requires requires() {
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, void(void)>;
//			System::Traits::is_invocable_r_v<void, Func, void>;
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, Function<void(void)>>;
//		}
//		EventHandler& operator+=(Func && f) noexcept {
//			funcs.Add(static_cast<Func&&>(f));
//			return *this;
//		}
//	public:
//		EventHandler& operator-=(Function<void(void)>&& f) noexcept {
//			funcs.RemoveValue(f);
//			return *this;
//		}
//		template<class Func>
//		requires requires() {
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, void(void)>;
//			System::Traits::is_invocable_r_v<void, Func, void>;
//			!System::Traits::is_same_v<System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>, Function<void(void)>>;
//		}
//		EventHandler& operator-=(Func && f) noexcept {
//			funcs.Remove(static_cast<Func&&>(f));
//			return *this;
//		}
//		EventHandler& operator=(const EventHandler<void>&) noexcept = delete;
//		EventHandler& operator=(EventHandler<void>&&) noexcept = default;
//	public:
//		void operator()() {
//			for (size_t i = 0, count = funcs.Count(); i < count; ++i) {
//				if (count != funcs.Count()) return;
//				funcs[i]();
//			}
//		}
//	};
//}
