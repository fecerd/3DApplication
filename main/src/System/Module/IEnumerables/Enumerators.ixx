export module Enumerators;
import CSTDINT;
export import IEnumeratorBase;
export import IEnumerator;
export import Function;
export import BoostCoroutine;

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
	template<class T>
	IEnumerator<T>* CreateEnumerator(IEnumerator<T>&& arg) noexcept {
		return new IEnumerator<T>(System::move(arg));
	}
	template<class T>
	void DeleteEnumerator(IEnumerator<T>*& arg) noexcept {
		IEnumeratorBase<T>* ptr = arg;
		delete ptr;
		arg = nullptr;
	}


	/// <summary>
	/// Select関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="Dst">変換後の要素型</typeparam>
	/// <typeparam name="Src">変換前の要素型</typeparam>
	template<class Dst, class Src>
	struct SelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//GetEnumerator(bool)が生成するコルーチンを持つEnumeratorへのポインタ
		Function<Dst(Src&)> m_func;	//変換用関数
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr SelectEnumerator(IEnumeratorBase<Src>* prev, const Function<Dst(Src&)>& func) noexcept : CommonEnumerator<Dst, Src>(prev), m_func(func) {
			m_converter = CreateEnumerator<Dst>(GetEnumerator(false));
		}
		constexpr ~SelectEnumerator() noexcept {
			DeleteEnumerator<Dst>(m_converter);
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			//m_conveter内でm_prevが使用されるため、m_prev->begin()の後に呼び出す
			return m_converter->begin();
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
#if defined(__GNUC__) && !defined(__clang__)
		void Select(Boost::push_type<Dst&>& sink) {
			if (!m_prev) return;
			for (auto& x : *m_prev) {
				Dst ret = m_func(x);
				sink(ret);
			}
		}
		IEnumerator<Dst> GetEnumerator(bool reverse) noexcept {
			auto internal = [this](bool) -> IEnumerator<Dst> {
				return IEnumerator<Dst>(Function([this](Boost::push_type<Dst&>& sink){ Select(sink); }));
			};
			return IEnumerator<Dst>(internal, reverse);
		}
#else
		IEnumerator<Dst> Select() {
			if (!m_prev) co_return;
			for (auto& x : *m_prev) {
				Dst ret = m_func(x);
				co_yield ret;
			}			
		}
		IEnumerator<Dst> GetEnumerator(bool reverse) noexcept {
			auto internal = [this](bool) {
				return Select();
			};
			return IEnumerator<Dst>(internal, reverse);
		}
#endif
	};

	/// <summary>
	/// MemberSelect関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="Dst">メンバ変数の型。イテレータはDst&を返す</typeparam>
	/// <typeparam name="Src">直前のEnumeratorの型。メンバ変数を持つ</typeparam>
	template<class Dst, class Src>
	struct MemberSelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//GetEnumerator(bool)が生成するコルーチンを持つEnumeratorへのポインタ
		Dst Src::* m_member = nullptr;	//メンバ変数ポインタ
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr MemberSelectEnumerator(IEnumeratorBase<Src>* prev, Dst Src::* member) noexcept : CommonEnumerator<Dst, Src>(prev), m_member(member) {
			m_converter = CreateEnumerator<Dst>(GetEnumerator(false));
		}
		virtual constexpr ~MemberSelectEnumerator() noexcept {
			DeleteEnumerator<Dst>(m_converter);
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			return m_converter->begin();
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
	private:
		/// <summary>
		/// 直前のEnumeratorの値の持つメンバ変数(参照)を列挙する
		/// </summary>
#if defined(__GNUC__) && !defined(__clang__)
		void Select(Boost::push_type<Dst&>& sink) {
			if (!m_prev) return;
			for (auto& x : *m_prev) {
				Dst& ret = x.*m_member;
				sink(ret);
			}
		}
		IEnumerator<Dst> GetEnumerator(bool reverse) noexcept {
			auto internal = [this](bool) -> IEnumerator<Dst> {
				return IEnumerator<Dst>(Function([this](Boost::push_type<Dst&>& sink){ Select(sink); }));
			};
			return IEnumerator<Dst>(internal, reverse);
		}
#else
		IEnumerator<Dst> GetEnumerator(bool reverse) noexcept {
			auto internal = [this](bool) -> IEnumerator<Dst> {
				while (m_prev && !m_prev->Done()) {
					co_yield m_prev->operator*().*m_member;
					m_prev->operator++();
				}
			};
			return IEnumerator<Dst>(Function<IEnumerator<Dst>(bool)>(internal), reverse);
		}
#endif
	};

	/// <summary>
	/// Where関数が生成するEnumerator
	/// </summary>
	/// <typeparam name="T">要素型</typeparam>
	template<class T>
	struct WhereEnumerator : public CommonEnumerator<T, T> {
		Function<bool(T const&)> m_func;	//条件式
		using CommonEnumerator<T, T>::m_prev;
	public:
		constexpr WhereEnumerator(IEnumeratorBase<T>* prev, const Function<bool(T const&)>& func) noexcept
			: CommonEnumerator<T, T>(prev), m_func(func) {}
	public:
		constexpr EnumerableIterator<T> begin() noexcept override {
			if (!m_prev) return EnumerableIterator<T>();
			//m_prev->begin()で初期化される
			for (EnumerableIterator<T> ite = m_prev->begin(), e = m_prev->end(); ite != e; ++ite) {
				T& x = *ite;
				if (m_func(x)) return EnumerableIterator<T>(this);
			}
			this->m_done = m_prev->Done();
			return EnumerableIterator<T>();
		}
	public:
		constexpr WhereEnumerator<T>& operator++() noexcept override {
			if (!m_prev) return *this;
			do {
				m_prev->operator++();
				this->m_done = m_prev->Done();
				if (this->m_done) break;
				T& x = m_prev->operator*();
				if (m_func(x)) break;
			} while (!this->m_done);
			return *this;
		}
		constexpr T& operator*() noexcept override { return m_prev->operator*(); }
	public:
		constexpr WhereEnumerator<T>* Clone() const noexcept override {
			return new WhereEnumerator<T>(m_prev->Clone(), m_func);
		}
	};
}
