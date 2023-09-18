export module Function;
import CSTDINT;
import Traits;
import Allocator;
import Exception;

//FuncData
namespace System {
	union FuncData {
		//(メンバ)関数ポインタやポインタ型より大きいサイズの関数オブジェクトへのポインタを保持する
		void* ptr;
		//ポインタ型以下のサイズの関数オブジェクトを配置newで構成する
		uint8_t buf[sizeof(void*)];
	public:
		FuncData() noexcept : ptr(nullptr) {}
		template<class F>
		FuncData(F&& functor) noexcept : ptr(nullptr) {
			using type = Traits::remove_cvref_t<F>;
			if constexpr (Traits::is_member_function_pointer_v<type>) {
				ptr = new type(functor);
			}
			else if constexpr (Traits::is_pointer_v<type>) {
				ptr = reinterpret_cast<void*>(functor);
			}
			else if constexpr (sizeof(type) <= sizeof(buf)) {
				System::construct_at(reinterpret_cast<type*>(buf), System::move(functor));
			}
			else {
				ptr = new type(System::move(functor));
			}
		}
		FuncData(const FuncData&) noexcept = delete;
		FuncData(FuncData&& arg) noexcept : ptr(arg.ptr) {
			arg.ptr = nullptr;
		}
	public:
		FuncData& operator=(const FuncData&) noexcept = delete;
		FuncData& operator=(FuncData&& rhs) noexcept {
			if (this == &rhs) return *this;
			ptr = rhs.ptr;
			rhs.ptr = nullptr;
			return *this;
		}
	};
}

//Function
export namespace System {
	template<class T> class Function;
	template<class R, class ...Args>
	class Function<R(Args...)> {
		template<class U> friend class Function;
		using invoke_pointer_t = R(*)(FuncData&, Args...);
		using clone_or_destruct_pointer_t = FuncData(*)(FuncData&, bool);
	private:
		mutable FuncData m_data;
		invoke_pointer_t m_invoke;
		clone_or_destruct_pointer_t m_clone_or_destruct;
	private:/* 内部操作関数 */
		template<class F>
		static void Destruct(FuncData& data) noexcept {
			using type = Traits::remove_cvref_t<F>;
			if constexpr (Traits::is_member_function_pointer_v<type>) {
				delete reinterpret_cast<type*>(data.ptr);
				data.ptr = nullptr;
			}
			else if constexpr (Traits::is_pointer_v<type>) {
				data.ptr = nullptr;
			}
			else if constexpr (sizeof(type) <= sizeof(FuncData::buf)){
				reinterpret_cast<type&>(*data.buf).~type();
				data.ptr = nullptr;
			}
			else {
				delete static_cast<type*>(data.ptr);
				data.ptr = nullptr;
			}
		}
		template<class F>
		static FuncData Clone(FuncData& data) noexcept {
			using type = Traits::remove_cvref_t<F>;
			if constexpr (Traits::is_member_function_pointer_v<type>) {
				return FuncData(*reinterpret_cast<type*>(data.ptr));
			}
			else if constexpr (Traits::is_pointer_v<type>) {
				return FuncData(reinterpret_cast<type>(data.ptr));
			}
			else if constexpr (sizeof(type) <= sizeof(FuncData::buf)) {
				return FuncData(reinterpret_cast<type&>(*data.buf));
			}
			else {
				return FuncData(*static_cast<type*>(data.ptr));
			}
		}
		template<class F>
		static FuncData CloneOrDestruct(FuncData& data, bool clone) noexcept {
			if (clone) {
				return Clone<F>(data);
			}
			else {
				Destruct<F>(data);
				//Dummyを返す(使用しない)
				return FuncData();
			}
		}
		template<class F>
		static R Invoke(FuncData& data, Args ...args) noexcept {
			using type = Traits::remove_cvref_t<F>;
			if constexpr (Traits::is_member_function_pointer_v<type>) {
				type& pfunc = *reinterpret_cast<type*>(data.ptr);
				auto internal = [pfunc]<class Head, class ...Params>(Head&& head, Params&& ...params) {
					if constexpr (Traits::is_pointer_v<Traits::remove_reference_t<Head>>) {
						return (head->*pfunc)(params...);
					}
					else {
						return (head.*pfunc)(params...);
					}
				};
				return internal(System::move(args)...);
			}
			else if constexpr (Traits::is_pointer_v<type>) {
				type pfunc = reinterpret_cast<type>(data.ptr);
				return pfunc(args...);
			}
			else if constexpr (sizeof(type) <= sizeof(FuncData::buf)) {
				type& functor = reinterpret_cast<type&>(*data.buf);
				return functor(args...);
			}
			else {
				type& functor = *static_cast<type*>(data.ptr);
				return functor(args...);
			}
		}
	public:
		Function() noexcept : m_data(), m_invoke(nullptr)
							, m_clone_or_destruct(nullptr) {}
		Function(const Function& arg) noexcept
			: m_data(arg.Clone()), m_invoke(arg.m_invoke)
			, m_clone_or_destruct(arg.m_clone_or_destruct) {}
		Function(Function&& arg) noexcept
			: m_data(System::move(arg.m_data)), m_invoke(arg.m_invoke)
			, m_clone_or_destruct(arg.m_clone_or_destruct)
		{
			arg.m_invoke = nullptr;
			arg.m_clone_or_destruct = nullptr;
		}
		template<Traits::Concepts::CConvertibleFunction<R(Args...)> U>
		Function(const Function<U>& arg) noexcept : m_data(Function<U>(arg))
		{
			using type = Function<U>;
			m_invoke = Invoke<type>;
			m_clone_or_destruct = CloneOrDestruct<type>;
		}
		template<Traits::Concepts::CConvertibleFunction<R(Args...)> U>
		Function(Function<U>&& arg) noexcept : m_data(System::move(arg))
		{
			using type = Function<U>;
			m_invoke = Invoke<type>;
			m_clone_or_destruct = CloneOrDestruct<type>;
		}
		template<Traits::Concepts::CNotSameArg<Function<R(Args...)>> F>
		requires (!Traits::is_same_template_v<Traits::remove_cvref_t<F>, Function<R(Args...)>>)
		Function(F&& functor) noexcept : m_data(System::move(functor)) {
			using type = Traits::remove_cvref_t<F>;
			m_invoke = Invoke<type>;
			m_clone_or_destruct = CloneOrDestruct<type>;
		}
		~Function() noexcept { Reset(); }
	private: /* 内部で使用するClone */
		FuncData Clone() const noexcept {
			if (m_clone_or_destruct) return m_clone_or_destruct(m_data, true);
			else return FuncData();
		}
	public:
		void Reset() noexcept {
			if (m_clone_or_destruct) m_clone_or_destruct(m_data, false);
			m_invoke = nullptr;
			m_clone_or_destruct = nullptr;
		}
	public:
		template<class ...Params>
		R operator()(Params&& ...params) const {
			if (!m_invoke) {
				//assert
				throw InvalidOperationException(u"初期化されていないFunction型インスタンスを使用しました。");
			}
			return m_invoke(m_data, params...);
		}
		explicit operator bool() const noexcept { return static_cast<bool>(m_invoke); }
		Function& operator=(const Function& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset();
			m_data = rhs.Clone();
			m_invoke = rhs.m_invoke;
			m_clone_or_destruct = rhs.m_clone_or_destruct;
			return *this;
		}
		template<class T>
		inline constexpr Traits::remove_reference_t<T>&& move_in(T&& t) noexcept {
			return static_cast<Traits::remove_reference_t<T>&&>(t);
		}
		Function& operator=(Function&& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset();
			m_data = System::move(rhs.m_data);
			m_invoke = rhs.m_invoke;
			m_clone_or_destruct = rhs.m_clone_or_destruct;
			rhs.m_invoke = nullptr;
			rhs.m_clone_or_destruct = nullptr;
			return *this;
		}
		template<Traits::Concepts::CConvertibleFunction<R(Args...)> U>
		Function& operator=(const Function<U>& rhs) noexcept {
			Reset();
			m_data = FuncData(Function<U>(rhs));
			m_invoke = Invoke<Function<U>>;
			m_clone_or_destruct = CloneOrDestruct<Function<U>>;
			return *this;
		}
		template<Traits::Concepts::CConvertibleFunction<R(Args...)> U>
		Function& operator=(Function<U>&& rhs) noexcept {
			Reset();
			m_data = FuncData(Function<U>(System::move(rhs)));
			m_invoke = Invoke<Function<U>>;
			m_clone_or_destruct = CloneOrDestruct<Function<U>>;
			return *this;
		}
	private:/* 不正なコピー/ムーブ操作のエラーを見やすくするための定義 */
		template<class U>
		Function& operator=(const Function<U>& rhs) noexcept {
			static_assert(Traits::false_v<U>);
			return *this;
		}
		template<class U>
		Function& operator=(Function<U>&& rhs) noexcept {
			static_assert(Traits::false_v<U>);
			return *this;
		}
	};
	template<Traits::Concepts::CHasFunctionCallOperator F>
	Function(F&&)->Function<Traits::function_t<F>>;
	template<Traits::Concepts::CPointer P>
	Function(P)->Function<Traits::function_t<P>>;
	template<Traits::Concepts::CMemberFunctionPointer P>
	Function(P)->Function<Traits::function_t<P>>;

	template class Function<int(int)>;
}
