module;
#include<crtdbg.h>
#undef SIZE_MAX
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module IEnumerable;
import Exception;
import Function;
import Traits;
import Coroutine;
import Iterators;

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

//IEnumeratorBase
export namespace System {
	template<class T> struct EnumerableIterator;

	/// <summary>
	/// 各Enumeratorの基底型。
	/// 純粋仮想関数begin, operator++, operator*をオーバーライドする必要がある
	/// </summary>
	/// <typeparam name="T">要素型</typeparam>
	template<class T>
	struct IEnumeratorBase {
	protected:
		bool m_done = false;	//coroutine_handle<promise_type<T, IEnumerator<T>>>::done()の結果を記録する
	public:
		constexpr IEnumeratorBase() noexcept = default;
		constexpr IEnumeratorBase(IEnumeratorBase&& arg) noexcept : m_done(arg.m_done) { arg.m_done = true; }
		constexpr IEnumeratorBase(bool done) noexcept : m_done(done) {}
		virtual constexpr ~IEnumeratorBase() noexcept { m_done = true; }
	public:
		constexpr IEnumeratorBase(const IEnumeratorBase&) noexcept = delete;
		constexpr IEnumeratorBase& operator=(const IEnumeratorBase&) noexcept = delete;
	public:
		/// <summary>
		/// Enumeratorの最初の要素を指すイテレータを取得する
		/// </summary>
		/// <remarks>
		/// [実装時の注意]
		/// 保持しているコルーチンが終了状態のとき、それを破棄し、新たにコルーチンを生成する。
		/// 初期化されたコルーチンをインクリメントして、最初の要素で停止させなければならない
		/// </remarks>
		virtual constexpr EnumerableIterator<T> begin() noexcept = 0;
		/// <summary>
		/// コルーチンの終了状態を表すイテレータを取得する
		/// </summary>
		constexpr EnumerableIterator<T> end() const noexcept { return EnumerableIterator<T>(); }
		/// <summary>
		/// コルーチンが終了しているかを調べる
		/// </summary>
		constexpr bool Done() const noexcept { return m_done; }
	public:
		/// <summary>
		/// コルーチンを進める
		/// </summary>
		/// <remarks>
		/// [実装時の注意]
		/// <remarks>
		virtual constexpr IEnumeratorBase<T>& operator++() noexcept = 0;
		/// <summary>
		/// 現在指している要素を返す
		/// </summary>
		virtual constexpr T& operator*() noexcept = 0;
	public:
		/// <summary>
		/// コルーチンを複製し、新たなIEnumeratorへのポインタを返す。
		/// 確保されたメモリはIEnumerableのデストラクタで、CommonEnumerator::m_prevを
		/// 再帰的に辿って解放される
		/// </summary>
		virtual constexpr IEnumeratorBase<T>* Clone() const noexcept = 0;
		/// <summary>
		/// コルーチンを逆順にする
		/// </summary>
		virtual constexpr void Reverse() noexcept = 0;
	};
}

//IEnumerator
export namespace System {
	template<class T> class ICollection;

	/// <summary>
	/// コルーチン関数の戻り値型
	/// </summary>
	/// <typeparam name="T">コルーチンが返す要素型</typeparam>
	template<class T>
	struct IEnumerator : public IEnumeratorBase<T> {
		using promise_type = System::promise_type<T, IEnumerator<T>>;	//コルーチン関数の引数型に必須なエイリアス
	protected:
		//ICollectionの要素型
		using object_type = System::Traits::remove_cv_t<T>;
		//このIEnumeratorを生成したICollection型
		using collection_type = System::Traits::conditional_t<System::Traits::is_const_v<T>, const ICollection<object_type>, ICollection<object_type>>;
		//GetEnumerator()関数が生成するコルーチンハンドル型
		using handle = coroutine_handle<System::promise_type<T, IEnumerator<T>>>;
	protected:
		//コルーチンハンドル
		handle m_handle = handle();
		//逆順序を指定するbool値を引数にとり、IEnumeratorを生成する関数
		//[this(コルーチンを生成するインスタンス)](bool reverse) { return reverse ? GetReverseEnumerator() : GetEnumerator(); }
		//のようなラムダ式を保持し、再生成と反転を可能にする
		Function<IEnumerator<T>(bool)> m_enumFunc;
		//現在のコルーチンが逆順序のとき、true
		bool m_reverse = false;
	public:
		//promise_type内で使用されるため必須
		constexpr IEnumerator(handle h) noexcept : m_handle(h) {}
		constexpr IEnumerator(const Function<IEnumerator<T>(bool)>& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(getEnumFunc)
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(Function<IEnumerator<T>(bool)>&& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(static_cast<Function<IEnumerator<T>(bool)>&&>(getEnumFunc))
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(IEnumerator<T>&& arg) noexcept
			: m_handle(arg.m_handle), m_reverse(arg.m_reverse), m_enumFunc(static_cast<Function<IEnumerator<T>(bool)>&&>(arg.m_enumFunc))
		{
			arg.m_handle = handle();
			arg.m_done = true;
		}
		constexpr ~IEnumerator() noexcept {
			if (m_handle) m_handle.destroy();
			m_handle = handle();
		}
	public:
		/// <summary>
		/// 最初の要素を指すイテレータを取得する
		/// </summary>
		constexpr EnumerableIterator<T> begin() noexcept override {
			//このコルーチンがすでに実行されている(終了を含む)とき、新たに生成する
			if (m_handle) *this = IEnumerator<T>(m_enumFunc, m_reverse);
			operator++();	//m_handle.promise().valueに最初の要素を入れる
			return EnumerableIterator<T>(this);
		}
	public:
		constexpr IEnumerator<T>& operator++() noexcept override {
			m_handle.resume();
			this->m_done = m_handle.done();
			return *this;
		}
		constexpr T& operator*() noexcept override { return *m_handle.promise().value; }
	public:
		constexpr IEnumerator<T>* Clone() const noexcept override {
			return new IEnumerator<T>(m_enumFunc, m_reverse);
		}
		constexpr void Reverse() noexcept override {
			*this = IEnumerator<T>(m_enumFunc, !m_reverse);
		}
	private:
		//begin(), Reverse()内でのみ使用するためprivate
		constexpr IEnumerator<T>& operator=(IEnumerator<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			if (m_handle) m_handle.destroy();
			this->m_done = rhs.m_done;
			m_handle = rhs.m_handle;
			m_enumFunc = static_cast<System::Function<IEnumerator<T>(bool)>&&>(rhs.m_enumFunc);
			m_reverse = rhs.m_reverse;
			rhs.m_done = true;
			rhs.m_handle = handle();
			return *this;
		}
	};
	template<class T>
	IEnumerator(IEnumerator<T>&&, ICollection<T>*)->IEnumerator<T>;
}

//CommonEnumerator
export namespace System {
	template<class Dst, class Src>
	struct CommonEnumerator : public IEnumeratorBase<Dst> {
		IEnumeratorBase<Src>* m_prev = nullptr;	//直前のEnumeratorへのポインタ
	public:
		constexpr CommonEnumerator(IEnumeratorBase<Src>* prev) noexcept : m_prev(prev) {}
		virtual constexpr ~CommonEnumerator() noexcept {
			delete m_prev;
			m_prev = nullptr;
		}
	public:
		CommonEnumerator(const CommonEnumerator&) noexcept = delete;
		CommonEnumerator(CommonEnumerator<Dst, Src>&& arg) noexcept = delete;
		CommonEnumerator& operator=(const CommonEnumerator&) noexcept = delete;
		CommonEnumerator& operator=(CommonEnumerator&&) noexcept = delete;
	public:
		constexpr void Reverse() noexcept override { m_prev->Reverse(); }
	};
}

//Enumerators
export namespace System {
	/// <summary>
	/// Select関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="Dst">変換後の要素型</typeparam>
	/// <typeparam name="Src">変換前の要素型</typeparam>
	template<class Dst, class Src>
	struct SelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//this->GetEnumerator()が生成するコルーチンを持つEnumeratorへのポインタ
		System::Function<Dst(Src&)> m_func;	//変換用関数
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr SelectEnumerator(IEnumeratorBase<Src>* prev, const System::Function<Dst(Src&)>& func) noexcept : CommonEnumerator<Dst, Src>(prev), m_func(func) {
			m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
		}
		constexpr ~SelectEnumerator() noexcept {
			delete m_converter;
			m_converter = nullptr;
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			//変換に使用するコルーチンが終了している場合、新たに生成する
			if (m_converter->Done()) {
				delete m_converter;
				m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
			}
			m_converter->begin();	//m_conveter内でm_prevが使用されるため、m_prev->begin()の後に呼び出す
			return EnumerableIterator<Dst>(m_converter);
		}
	public:
		constexpr SelectEnumerator<Dst, Src>& operator++() noexcept override {
			m_converter->operator++();
			this->m_done = m_converter->Done();
			return *this;
		}
		constexpr Dst& operator*() noexcept override { return m_converter->operator*(); }
	public:
		constexpr SelectEnumerator<Dst, Src>* Clone() const noexcept override { return new SelectEnumerator<Dst, Src>(m_prev->Clone(), m_func); }
	private:
		/// <summary>
		/// 直前のEnumeratorの値を変換用関数で変換し、列挙する
		/// </summary>
		IEnumerator<Dst> GetEnumerator() noexcept {
			while (m_prev && !m_prev->Done()) {
				Dst tmp = m_func(m_prev->operator*());
				co_yield tmp;
				m_prev->operator++();
			}
		}
	};

	/// <summary>
	/// MemberSelect関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="Dst">メンバ変数の型。イテレータはDst&を返す</typeparam>
	/// <typeparam name="Src">直前のEnumeratorの型。メンバ変数を持つ</typeparam>
	template<class Dst, class Src>
	struct MemberSelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//this->GetEnumerator()が生成するコルーチンを持つEnumeratorへのポインタ
		Dst Src::* m_member = nullptr;	//メンバ変数ポインタ
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr MemberSelectEnumerator(IEnumeratorBase<Src>* prev, Dst Src::* member) noexcept : CommonEnumerator<Dst, Src>(prev), m_member(member) {
			m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
		}
		constexpr ~MemberSelectEnumerator() noexcept {
			delete m_converter;
			m_converter = nullptr;
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			//変換に使用するコルーチンが終了している場合、新たに生成する
			if (m_converter->Done()) {
				delete m_converter;
				m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
			}
			m_converter->begin();	//m_conveter内でm_prevが使用されるため、m_prev->begin()の後に呼び出す
			return EnumerableIterator<Dst>(m_converter);
		}
	public:
		constexpr MemberSelectEnumerator<Dst, Src>& operator++() noexcept override {
			m_converter->operator++();
			this->m_done = m_converter->Done();
			return *this;
		}
		constexpr Dst& operator*() noexcept override { return m_converter->operator*(); }
	public:
		constexpr MemberSelectEnumerator<Dst, Src>* Clone() const noexcept override { return new MemberSelectEnumerator<Dst, Src>(m_prev->Clone(), m_member); }
	public:
		/// <summary>
		/// 直前のEnumeratorの値の持つメンバ変数(参照)を列挙する
		/// </summary>
		IEnumerator<Dst> GetEnumerator() noexcept {
			while (m_prev && !m_prev->Done()) {
				co_yield m_prev->operator*().*m_member;
				m_prev->operator++();
			}
		}
	};

	/// <summary>
	/// Where関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="T">要素型</typeparam>
	template<class T>
	struct WhereEnumerator : public CommonEnumerator<T, T> {
		System::Function<bool(T const&)> m_func;	//条件式
		using CommonEnumerator<T, T>::m_prev;
	public:
		constexpr WhereEnumerator(IEnumeratorBase<T>* prev, const System::Function<bool(T const&)>& func) noexcept : CommonEnumerator<T, T>(prev), m_func(func) {}
	public:
		constexpr EnumerableIterator<T> begin() noexcept override {
			if (!this->m_prev) return EnumerableIterator<T>();
			m_prev->begin();
			this->m_done = m_prev->Done();
			while (!this->m_done && !m_func(m_prev->operator*())) {
				m_prev->operator++();
				this->m_done = m_prev->Done();
			}
			return EnumerableIterator<T>(this);
		}
	public:
		constexpr WhereEnumerator<T>& operator++() noexcept override {
			do {
				m_prev->operator++();
				this->m_done = m_prev->Done();
			} while (!this->m_done && !m_func(m_prev->operator*()));
			return *this;
		}
		constexpr T& operator*() noexcept override { return m_prev->operator*(); }
	public:
		constexpr WhereEnumerator<T>* Clone() const noexcept override { return new WhereEnumerator<T>(m_prev->Clone(), m_func); }
	};
}

//EnumerableIterator
export namespace System {
	/// <summary>
	/// IEnumerableが返すイテレータ型
	/// </summary>
	/// <typeparam name="T">要素型。operator*で参照が取得可能</typeparam>
	template<class T>
	struct EnumerableIterator {
		using value_type = T;
		using iterator_concept = System::Traits::forward_iterator_tag;
	protected:
		IEnumeratorBase<T>* m_enumerator = nullptr;	//T型要素を列挙するEnumeratorへのポインタ
	public:
		EnumerableIterator() noexcept = default;
		EnumerableIterator(IEnumeratorBase<T>* enumerator) noexcept : m_enumerator(enumerator) {}
		~EnumerableIterator() noexcept { m_enumerator = nullptr; }
	protected:
		/// <summary>
		/// イテレータが最後の要素を越えているか調べる
		/// </summary>
		constexpr bool Done() const noexcept { return m_enumerator ? m_enumerator->Done() : true; }
	public:
		constexpr EnumerableIterator<T>& operator++() noexcept {
			if (m_enumerator) m_enumerator->operator++();
			return *this;
		}
		constexpr T& operator*() const {
			if (!m_enumerator) throw System::InvalidOperationException(__FUNCSIG__, "イテレータがIEnumeratorへのポインタを持っていません。", __FILE__, __LINE__);
			return m_enumerator->operator*();
		}
		constexpr bool operator==(const EnumerableIterator<T>& rhs) const noexcept { return this->Done() == rhs.Done(); }
		constexpr EnumerableIterator<T>& operator=(const EnumerableIterator&) noexcept = default;
	};
}

//IEnumerable
export namespace System {
	/// <summary>
	/// ICollectionの持つ各メソッドの戻り値型。
	/// メソッドチェーンにより、Linqライクの機能を提供する。
	/// begin, end関数を使用して、イテレータを取得できる
	/// </summary>
	/// <typeparam name="T">要素型</typeparam>
	template<class T>
	class IEnumerable {
		IEnumeratorBase<T>* m_last = nullptr;	//メソッドチェーンの最後のEnumeratorへのポインタ
	public:
		IEnumerable(IEnumerable<T>&& arg) noexcept : m_last(arg.m_last) { arg.m_last = nullptr; }
		IEnumerable(IEnumeratorBase<T>* enumerator) noexcept : m_last(enumerator) {}
		~IEnumerable() noexcept { delete m_last; m_last = nullptr; }
	public:
		/// <summary>
		/// 最初の要素を指すイテレータを取得する
		/// </summary>
		EnumerableIterator<T> begin() const noexcept { return m_last ? m_last->begin() : EnumerableIterator<T>(); }
		/// <summary>
		/// 終了を表すイテレータを取得する
		/// </summary>
		EnumerableIterator<T> end() const noexcept { return m_last ? m_last->end() : EnumerableIterator<T>(); }
	private:
		template<class Dst>
		IEnumerable<Dst> To(IEnumeratorBase<Dst>* ptr) noexcept {
			m_last = nullptr;
			return IEnumerable<Dst>(ptr);
		}
	public:
		/// <summary>
		/// 最初の要素を取得する
		/// </summary>
		/// <exception>
		/// 要素が存在しない場合、System::InvalidOperationExceptionを送出する
		/// </exception>
		T& First() const {
			EnumerableIterator<T> ite = begin(), e = end();
			if (!(ite != e)) throw System::InvalidOperationException("IEnumerable::First(): 要素が存在しません。");
			T& ret = *ite;
			for (; ite != e; ++ite);
			return ret;
		}
		/// <summary>
		/// 最初の要素へのポインタを取得する
		/// </summary>
		/// <returns>
		/// 要素が存在しない場合、nullptrを返す
		/// <returns>
		System::Traits::remove_cvref_t<T>* TryFirst() const noexcept {
			EnumerableIterator<T> ite = begin(), e = end();
			if (!(ite != e)) return nullptr;
			T& ret = *ite;
			for (; ite != e; ++ite);
			return &ret;
		}
		System::Traits::remove_cvref_t<T>* TryIndex(size_t index) const noexcept {
			EnumerableIterator<T> ite = begin(), e = end();
			for (size_t i = 0; i < index && ite != e; ++i, ++ite);
			if (!(ite != e)) return nullptr;
			T& ret = *ite;
			for (; ite != e; ++ite);
			return &ret;
		}
		/// <summary>
		/// 要素数を取得する
		/// </summary>
		size_t EnumeratorCount() const noexcept {
			size_t ret = 0;
			for (auto b = begin(), e = end(); b != e; ++b, ++ret);
			return ret;
		}
	public:
		IEnumerable<T> Clone() const noexcept { return IEnumerable<T>(m_last->Clone()); }
	public:
		/// <summary>
		/// 現在と同じIEnumerableを取得する
		/// </summary>
		IEnumerable<T> Chain() && noexcept { return To(m_last); }
		IEnumerable<T> Chain() & noexcept { return IEnumerable<T>(m_last->Clone()); }
		/// <summary>
		/// 指定した関数を使用して、別の要素型を持つIEnumerableを取得する
		/// </summary>
		/// <typeparam name="Dst">変換先の型</typeparam>
		/// <param name="func">このIEnumerableが持つ要素型への参照を引数とし、変換先の型を返す関数オブジェクト</param>
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) && noexcept {
			return To(new SelectEnumerator<Dst, T>(m_last, func));
		}
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) & noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T>(m_last->Clone(), func));
		}
		/// <summary>
		/// 条件式を満たす要素を持つIEnumerableを取得する
		/// </summary>
		/// <param name="func">このIEnumerableの要素型への参照を引数とし、bool型を返す関数オブジェクト</param>
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) && noexcept {
			return To(new WhereEnumerator<T>(m_last, func));
		}
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) & noexcept {
			return IEnumerable<T>(new WhereEnumerator<T>(m_last->Clone(), func));
		}
		/// <summary>
		/// 指定したメンバ変数ポインタを使用して、メンバ変数への参照型を持つIEnumerableを取得する
		/// </summary>
		/// <typeparam name="Dst">メンバ変数型</typeparam>
		/// <typeparam name="M">メンバ変数ポインタ型</typeparam>
		/// <param name="member">メンバ変数ポインタ</param>
		template<class Dst, class R>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<R, T> member) && noexcept {
			return To(new MemberSelectEnumerator<Dst, T>(m_last, member));
		}
		template<class Dst, class R>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<R, T> member) & noexcept {
			return IEnumerable<Dst>(new MemberSelectEnumerator<Dst, T>(m_last->Clone(), member));
		}
		/// <summary>
		/// 要素の順序を反転したIEnumerableを取得する
		/// </summary>
		IEnumerable<T> Reverse() && noexcept {
			m_last->Reverse();
			return To(m_last);
		}
		IEnumerable<T> Reverse() & noexcept {
			IEnumeratorBase<T>* tmp = m_last->Clone();
			tmp->Reverse();
			return IEnumerable<T>(tmp);
		}
	};
}

//ICollection
export namespace System {
	/// <summary>
	/// Linqライクの機能を提供したいコンテナクラスに仮想継承する基底クラス。
	/// GetEnumerator関数とGetReverseEnumerator関数がオーバーライド必須である(const/非const両方)
	/// </summary>
	/// <typeparam name="T">要素型</typeparam>
	template<class T>
	class ICollection {
	public:
		constexpr ICollection() noexcept = default;
	public:
		/// <summary>
		/// このコンテナが持つ要素を列挙する
		/// </summary>
		virtual IEnumerator<T> GetEnumerator() noexcept = 0;
		/// <summary>
		/// このコンテナが持つ要素を列挙する(const版)
		/// </summary>
		virtual IEnumerator<T const> GetEnumerator() const noexcept = 0;
		/// <summary>
		/// このコンテナが持つ要素を逆順で列挙する
		/// </summary>
		virtual IEnumerator<T> GetReverseEnumerator() noexcept = 0;
		/// <summary>
		/// このコンテナが持つ要素を逆順で列挙する(const版)
		/// </summary>
		virtual IEnumerator<T const> GetReverseEnumerator() const noexcept = 0;
	private:
		IEnumerator<T>* GetPtr(bool reverse = false) noexcept {
			return new IEnumerator<T>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, reverse);
		}
		IEnumerator<T const>* GetPtr(bool reverse = false) const noexcept {
			return new IEnumerator<T const>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, reverse);
		}
	public:
		/// <summary>
		/// このコンテナが持つ要素を加工せず列挙するIEnumerableを取得する
		/// </summary>
		IEnumerable<T> Chain() noexcept { return IEnumerable<T>(GetPtr()); }
		IEnumerable<T const> Chain() const noexcept { return IEnumerable<T const>(GetPtr()); }
		/// <summary>
		/// 指定した関数の引数にこのコンテナが持つ要素を適用し、戻り値を列挙するIEnumerableを取得する。
		/// 要素型の持つメンバ変数への参照を列挙する場合は、この関数の代わりにMemberSelect()関数を使用する
		/// </summary>
		/// <typeparam name="Dst">戻り値の型</typeparam>
		/// <param name="func">要素型の参照を引数にとり、任意の型を返す変換関数</param>
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T>(GetPtr(), func));
		}
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T const&)>& func) const noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T const>(GetPtr(), func));
		}
		/// <summary>
		/// このコンテナが持つ要素の中から指定した関数がtrueを返すものを列挙するIEnumerableを取得する
		/// </summary>
		/// <param name="func">要素型へのconst参照を引数にとり、bool型を返す条件関数</param>
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) noexcept { return IEnumerable<T>(new WhereEnumerator<T>(GetPtr(), func)); }
		IEnumerable<T const> Where(const System::Function<bool(T const&)>& func) const noexcept { return IEnumerable<T const>(new WhereEnumerator<T const>(GetPtr(), func)); }
		/// <summary>
		/// このコンテナが持つ要素の指定したメンバ変数を列挙するIEnumerableを取得する。
		/// この関数によって取得したIEnumerableのイテレータは指定したメンバ変数への参照を表す
		/// </summary>
		/// <typeparam name="Dst">メンバ変数型</typeparam>
		/// <param name="member">要素型が持つメンバ変数ポインタ</param>
		template<class Dst>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<Dst, T> member) noexcept requires(System::Traits::is_class_v<T> || System::Traits::is_union_v<T>) {
			return IEnumerable<Dst>(new MemberSelectEnumerator<Dst, T>(GetPtr(), member));
		}
		template<class Dst>
		IEnumerable<const Dst> MemberSelect(System::Traits::MemberPtr<Dst, T> member) const noexcept requires(System::Traits::is_class_v<T> || System::Traits::is_union_v<T>) {
			return IEnumerable<const Dst>(new SelectEnumerator<const Dst, T const>(GetPtr(), member));
		}
		/// <summary>
		/// このコンテナが持つ要素を逆順に列挙するIEnumerableを取得する
		/// </summary>
		IEnumerable<T> Reverse() noexcept { return IEnumerable<T>(GetPtr(true)); }
		IEnumerable<T const> Reverse() const noexcept { return IEnumerable<T const>(GetPtr(true)); }
	};
}