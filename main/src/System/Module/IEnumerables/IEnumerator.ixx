export module IEnumerator;
import CSTDINT;
import Traits;
export import IEnumeratorBase;
export import Function;
export import Coroutine;
export import BoostCoroutine;

#if defined(__GNUC__) && !defined(__clang__)

//IEnumerator
export namespace System {
	template<class T>
	struct IEnumerator : public IEnumeratorBase<T> {
		Function<IEnumerator<T>(bool)> m_enumFunc;
		Boost::pull_type<T&> m_source;
		bool m_reverse = false;
	public:
		constexpr IEnumerator() noexcept = delete;
		constexpr IEnumerator(const Function<IEnumerator<T>(bool)>& getEnumFunc, bool reverse = false)
			: IEnumerator(getEnumFunc(reverse))
		{
			m_enumFunc = getEnumFunc;
			m_reverse = reverse;
		}
		constexpr IEnumerator(Function<IEnumerator<T>(bool)>&& getEnumFunc, bool reverse = false)
			: IEnumerator(getEnumFunc(reverse))
		{
			m_enumFunc = System::move(getEnumFunc);
			m_reverse = reverse;
		}
		constexpr IEnumerator(IEnumerator&& arg) noexcept
			: m_enumFunc(System::move(arg.m_enumFunc))
			, m_reverse(arg.m_reverse), m_source(System::move(arg.m_source))
		{
			this->m_done = arg.m_done;
			arg.m_done = true;
		}
		template<class F>
		requires requires (F&& f) { Boost::push_type<T&>(System::move(f)); }
		constexpr IEnumerator(F&& f) : m_source(System::move(f)) {
			this->m_done = !static_cast<bool>(m_source);
		}
		virtual constexpr ~IEnumerator() noexcept {
			while (m_source) m_source();
			this->m_done = true;
		}
	private:
		void Reset(bool reverse) noexcept {
			if (m_enumFunc) {
				while (m_source) m_source();
				this->m_done = true;
				m_reverse = reverse;
				IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
				m_source = System::move(tmp.m_source);
				this->m_done = false;
				tmp.m_done = true;
			}
		}
	public:
		constexpr EnumerableIterator<T> begin() noexcept override {
			//すでに実行されているコルーチンを閉じる
			Reset(m_reverse);
			if (m_source) return EnumerableIterator<T>(this);
			else return EnumerableIterator<T>();
		}
	public:
		constexpr IEnumerator<T>& operator++() noexcept override {
			if (m_source) {
				m_source();
				this->m_done = !static_cast<bool>(m_source);
			}
			return *this;
		}
		constexpr T& operator*() noexcept override { return m_source.get(); }
	public:
		constexpr IEnumerator<T>* Clone() const noexcept override {
			return new IEnumerator<T>(m_enumFunc, m_reverse);
		}
		constexpr void Reverse() noexcept override { Reset(!m_reverse); }
	};
}

#else

//promise_type
export namespace System {
	template<class T, class G>
	struct promise_type {
		T* value;
		auto get_return_object() { return G(coroutine_handle<promise_type<T, G>>::from_promise(*this)); }
		auto initial_suspend() { return suspend_always(); }
		auto final_suspend() noexcept { return suspend_always(); }
		void unhandled_exception() { static_promise::terminate(); }
		void return_void() {}
		auto yield_value(T& value) {
			this->value = &value;
			return suspend_always();
		}
	};

	template<class T, class G>
	struct promise_type<T&, G> {
		T* value;
		auto get_return_object() { return G(coroutine_handle<promise_type<T&, G>>::from_promise(*this)); }
		auto initial_suspend() { return suspend_always(); }
		auto final_suspend() noexcept { return suspend_always(); }
		void unhandled_exception() { static_promise::terminate(); }
		void return_void() {}
		auto yield_value(T& value) {
			this->value = &value;
			return suspend_always();
		}
	};
}

//IEnumerator
export namespace System {
	/// <summary>
	/// コルーチン関数の戻り値型
	/// </summary>
	/// <typeparam name="T">コルーチンが返す要素型</typeparam>
	template<class T>
	struct IEnumerator : public IEnumeratorBase<T> {
		//コルーチン関数の引数型に必須なエイリアス
		using promise_type = System::promise_type<T, IEnumerator<T>>;
		//ICollectionの要素型
		using object_type = System::Traits::remove_cv_t<T>;
	public:
		//GetEnumerator()関数が生成するコルーチンハンドル型
		using handle = coroutine_handle<System::promise_type<T, IEnumerator<T>>>;
	public:
		//コルーチンハンドル
		handle m_handle = handle();
		//逆順序を指定するbool値を引数にとり、IEnumeratorを生成する関数
		//[this(コルーチンを生成するインスタンス)](bool reverse) { return reverse ? GetReverseEnumerator() : GetEnumerator(); }
		//のようなラムダ式を保持し、再生成と反転を可能にする
		Function<IEnumerator<T>(bool)> m_enumFunc;
		//現在のコルーチンが逆順序のとき、true
		bool m_reverse = false;
	public:
		constexpr IEnumerator() noexcept = delete;
		//promise_type内で使用されるため必須
		constexpr IEnumerator(handle&& h) noexcept : m_handle(System::move(h)) {
			h = handle();
		}
		constexpr IEnumerator(const Function<IEnumerator<T>(bool)>& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(getEnumFunc)
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = System::move(tmp.m_handle);
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(Function<IEnumerator<T>(bool)>&& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(System::move(getEnumFunc))
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = System::move(tmp.m_handle);
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(IEnumerator<T>&& arg) noexcept
			: m_handle(System::move(arg.m_handle)), m_reverse(arg.m_reverse), m_enumFunc(System::move(arg.m_enumFunc))
		{
			arg.m_handle = handle();
			arg.m_done = true;
		}
		constexpr ~IEnumerator() noexcept {
			if (m_handle) m_handle.destroy();
			m_handle = handle();
			this->m_done = true;
		}
	private:
		void Reset(bool reverse) noexcept {
			if (m_enumFunc) {
				if (m_handle) m_handle.destroy();
				m_reverse = reverse;
				IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
				m_handle = tmp.m_handle;
				this->m_done = false;
				tmp.m_handle = handle();
				tmp.m_done = true;
			}
		}
	public:
		/// <summary>
		/// 最初の要素を指すイテレータを取得する
		/// </summary>
		constexpr EnumerableIterator<T> begin() noexcept override {
			//すでに実行されているコルーチンを閉じる
			Reset(m_reverse);
			operator++();	//m_handle.promise().valueに最初の要素を入れる
			return EnumerableIterator<T>(this);
		}
	public:
		constexpr IEnumerator<T>& operator++() noexcept override {
			if (m_handle) {
				m_handle.resume();
				this->m_done = m_handle.done();
			}
			return *this;
		}
		constexpr T& operator*() noexcept override { return *m_handle.promise().value; }
	public:
		constexpr IEnumerator<T>* Clone() const noexcept override {
			return new IEnumerator<T>(m_enumFunc, m_reverse);
		}
		constexpr void Reverse() noexcept override {
			Reset(!m_reverse);
		}
	};
}

#endif
