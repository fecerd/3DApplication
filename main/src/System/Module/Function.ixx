module;
#include "FUNCSIG.hpp"
export module Function;
import CSTDINT;
import Traits;
import Exception;
import <new>;

export namespace System {
	template<class N>
	class Function {
		Function() noexcept = delete;
		~Function() noexcept = delete;
	public:
		//template<class T, class R1, class ...Args1>
		//static Function<R1(T&, Args1...)> ToStaticFunction(R1(T::* f)(Args1...)) {
		//	return Function<R1(Args1...)>([f](T& obj, Args1 ...args) -> R1 { return (obj.*f)(args...); });
		//}
		template<class T, class R1, class ...Args1>
		static Function<R1(T&, Args1...)> ToStaticFunction(R1(T::* f)(Args1...)) {
			return Function<R1(T&, Args1...)>([f](T& obj, Args1 ...args) -> R1 { return (obj.*f)(args...); });
		}
		template<class T, class R1, class ...Args1>
		static Function<R1(T&, Args1...)> ToStaticFunction(R1(T::* f)(Args1...) const) {
			return Function<R1(T&, Args1...)>([f](T& obj, Args1 ...args) -> R1 { return (obj.*f)(args...); });
		}
	};
	template<class R, class ...Args>
	class Function<R(Args...)> {
		union Data {
			//(メンバ)関数ポインタやポインタ型より大きいサイズの関数オブジェクトへのポインタを保持する
			void* data = nullptr;
			//ポインタ型以下のサイズの関数オブジェクトを配置newで構成する
			uint8_t buf[sizeof(void*)];
		public:
			Data() noexcept = default;
			Data(void* p) noexcept : data(p) {}
			Data& operator=(const Data& rhs) noexcept { data = rhs.data; return *this; }
			Data& operator=(void* p) noexcept { data = p; return *this; }
		};
	private:
		using destruct_t = void(*)(Data&);
		using invoke_t = R(*)(Data&, Args&&...);
		using copy_t = void(*)(Data&, const Data&);
	private:
		Data data;	//関数オブジェクト/ポインタを保持する共用体
		destruct_t destruct_p = nullptr;	//dataを破棄する関数へのポインタ。dataが関数ポインタのとき、nullptr
		invoke_t invoke_p = nullptr;	//関数呼び出しを行う関数へのポインタ
		copy_t copy_p = nullptr;	//Functionクラスのコピーを行う関数へのポインタ
	public:
		Function() noexcept = default;
		Function(const Function& arg) {
			//デストラクタが存在しないとき、argは関数ポインタ
			if (!arg.destruct_p) {
				data = arg.data;
				invoke_p = arg.invoke_p;
			}
			//関数ポインタでないとき、copy_pがあるときのみコピー可能
			else if (arg.copy_p) {
				arg.copy_p(data, arg.data);
				invoke_p = arg.invoke_p;
				destruct_p = arg.destruct_p;
				copy_p = arg.copy_p;
			}
			else {
				throw InvalidOperationException(
					__FUNCSIG__, u"このFunction型変数はコピー不可のオブジェクトです。", __FILE__, __LINE__
				);
			}
		}
		Function(Function&& arg) noexcept : data(arg.data), destruct_p(arg.destruct_p), invoke_p(arg.invoke_p), copy_p(arg.copy_p) {
			arg.data = nullptr;
			arg.destruct_p = nullptr;
			arg.invoke_p = nullptr;
			arg.copy_p = nullptr;
		}
		template<class Functor>
		Function(Functor&& f) noexcept {
			using f_t = Traits::remove_cvref_t<Functor>;
			if constexpr (sizeof(f_t) <= sizeof(void*)) {
				new (data.buf) f_t(static_cast<f_t&&>(f));
				destruct_p = Destruct<f_t, false>;
				invoke_p = Invoke<f_t, false>;
				if constexpr (Traits::Concepts::CCopyConstructible<Functor>) copy_p = Copy<f_t, false>;
			}
			else {
				data = new f_t(static_cast<f_t&&>(f));
				destruct_p = Destruct<f_t, true>;
				invoke_p = Invoke<f_t, true>;
				if constexpr (Traits::Concepts::CCopyConstructible<Functor>) copy_p = Copy<f_t, true>;
			}
			static_assert(Traits::Concepts::CCopyConstructible<Functor>);
		}
		Function(R(*f)(Args...)) noexcept {
			data = f;
			invoke_p = Invoke;
		}
		~Function() noexcept {
			if (destruct_p) destruct_p(data);
			else data = nullptr;
			destruct_p = nullptr;
			invoke_p = nullptr;
			copy_p = nullptr;
		}
	public:/* メンバ関数ポインタを非メンバ関数として扱う */
		template<class T, class ...Tails>
		Function(R(T::* p)(Tails...) const) noexcept : Function([p](const T& obj, Tails... tails) -> R { return (obj.*p)(tails...); }) {}
		template<class T, class ...Tails>
		Function(R(T::* p)(Tails...)) noexcept : Function([p](T& obj, Tails... tails) -> R { return (obj.*p)(tails...); }) {}
	public:/* メンバ関数ポインタにオブジェクトをバインドする */
		template<class T>
		Function(R(T::* p)(Args...) const, const T& t) noexcept : Function([&t, p](Args ...args) -> R { return (t.*p)(args...); }) {}
		template<class T>
		Function(R(T::* p)(Args...), T& t) noexcept : Function([&t, p](Args ...args) -> R { return (t.*p)(args...); }) {}
	private:/* このクラスのコピーコンストラクトに使用する */
		template<Traits::Concepts::CCopyConstructible Func, bool Alloc>
		static void Copy(Data& data, const Data& src) {
			using f_t = System::Traits::remove_reference_t<System::Traits::remove_cv_t<Func>>;
			if constexpr (Alloc) {
				data = new f_t(*static_cast<f_t*>(src.data));
			}
			else {
				new (&data.buf[0]) f_t(*reinterpret_cast<const f_t*>(&src.buf[0]));
			}
		}
	private:/* 関数呼び出しに使用する */
		static R Invoke(Data& data, Args&& ...args) {
			R(*ptr)(Args...) = reinterpret_cast<R(*)(Args...)>(data.data);
			return ptr(static_cast<Args&&>(args)...);
		}
		template<class Func, bool Alloc>
		static R Invoke(Data& data, Args&& ...args) {
			Func* ptr;
			if constexpr (Alloc) ptr = reinterpret_cast<Func*>(data.data);
			else ptr = reinterpret_cast<Func*>(&data.buf[0]);
			return (*ptr)(static_cast<Args&&>(args)...);
		}
	private:/* dataの破棄に使用する */
		template<class Func, bool Alloc>
		static void Destruct(Data& data) {
			if constexpr (Alloc) delete reinterpret_cast<Func*>(data.data);
			else reinterpret_cast<Func*>(&data.buf[0])->~Func();
			data = nullptr;
		}
	public:
		Function& operator=(const Function& rhs) noexcept {
			if (this == &rhs) return *this;
			if (destruct_p) destruct_p(data);
			else data = nullptr;
			destruct_p = nullptr;
			invoke_p = nullptr;
			copy_p = nullptr;
			if (!rhs.destruct_p) {
				data = rhs.data;
				invoke_p = rhs.invoke_p;
				destruct_p = nullptr;
				copy_p = nullptr;
			}
			else if (rhs.copy_p) {
				rhs.copy_p(data, rhs.data);
				invoke_p = rhs.invoke_p;
				destruct_p = rhs.destruct_p;
				copy_p = rhs.copy_p;
			}
			return *this;
		}
		Function& operator=(Function&& rhs) noexcept {
			if (this == &rhs) return *this;
			if (destruct_p) destruct_p(data);
			else data = nullptr;
			destruct_p = nullptr;
			invoke_p = nullptr;
			copy_p = nullptr;
			data = rhs.data;
			destruct_p = rhs.destruct_p;
			invoke_p = rhs.invoke_p;
			copy_p = rhs.copy_p;
			rhs.data = nullptr;
			rhs.destruct_p = nullptr;
			rhs.invoke_p = nullptr;
			rhs.copy_p = nullptr;
			return *this;
		}
		bool operator==(const Function& rhs) const noexcept {
			return destruct_p ? destruct_p == rhs.destruct_p ? data.data == rhs.data.data : false : data.data == rhs.data.data;
		}
	public:
		R operator()(Args ...args) noexcept requires(sizeof...(Args) != 0) {
			if (invoke_p) return invoke_p(data, System::Traits::forward<Args>(args)...);
			else {
				if constexpr (System::Traits::is_void_v<R>) return;
				else {
					using rtype = System::Traits::remove_cvref_t<R>;
					rtype* ptr = nullptr;
					rtype& ref = *ptr;
					if constexpr (System::Traits::is_reference_v<R>) return ref;
					else return static_cast<rtype&&>(ref);
				}
			}
		}
		R operator()() noexcept requires(sizeof...(Args) == 0) {
			if (invoke_p)
				return invoke_p(data);
			else {
				if constexpr (System::Traits::is_void_v<R>)
					return;
				else {
					using rtype = System::Traits::remove_cvref_t<R>;
					rtype* ptr = nullptr;
					rtype& ref = *ptr;
					if constexpr (System::Traits::is_reference_v<R>)
						return ref;
					else
						return static_cast<rtype&&>(ref);
				}
			}
		}
		explicit operator bool() const noexcept { return invoke_p; }
	};
	template<class Functor>
	Function(Functor&&)noexcept->Function<Traits::function_t<Traits::remove_cvref_t<Functor>>>;
	template<class R, class ...Args>
	Function(R(*)(Args...)) noexcept->Function<R(Args...)>;
	template<class T, class R, class ...Args>
	Function(R(T::* f)(Args...)) noexcept->Function<R(T&, Args...)>;
	template<class T, class R, class ...Args>
	Function(R(T::*)(Args...), T&) noexcept->Function<R(Args...)>;
}
