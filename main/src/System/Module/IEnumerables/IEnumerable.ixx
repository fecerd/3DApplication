export module IEnumerable;
export import IEnumeratorBase;
export import IEnumerator;
export import Enumerators;
import Traits;
import Iterators;
import Exception;
export import Function;
export import Coroutine;
export import <new>;

//IMemberSelect
export namespace System::Internal {
	template<class T, class E>
	class IMemberSelect {
	public:
		constexpr IMemberSelect() noexcept {}
		constexpr ~IMemberSelect() noexcept {}
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
	class IEnumerable : public Internal::IMemberSelect<T, IEnumerable<T>> {
		IEnumeratorBase<T>* m_last = nullptr;	//メソッドチェーンの最後のEnumeratorへのポインタ
	public:
		constexpr IEnumerable() noexcept = delete;
		constexpr IEnumerable(const IEnumerable&) noexcept = delete;
		constexpr IEnumerable(IEnumerable&& arg) noexcept : m_last(arg.m_last) { arg.m_last = nullptr; }
		constexpr IEnumerable(IEnumeratorBase<T>* enumerator) noexcept : m_last(enumerator) {}
		constexpr ~IEnumerable() noexcept { delete m_last; m_last = nullptr; }
	public:/* 呼び出し側でIEnumeratorの作成を省略してIEnumerableを作成するコンストラクタ */
		constexpr IEnumerable(const Function<IEnumerator<T>(bool)>& getEnumFunc, bool reverse) noexcept {
			m_last = new IEnumerator<T>(getEnumFunc, reverse);
		}
		constexpr IEnumerable(Function<IEnumerator<T>(bool)>&& getEnumFunc, bool reverse) noexcept {
			m_last = new IEnumerator<T>(System::move(getEnumFunc), reverse);
		}
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

//IMemberSelect特殊化
export namespace System::Internal {
	template<Traits::Concepts::CHasMember T, class E>
	class IMemberSelect<T, E> {
	public:
		constexpr IMemberSelect() noexcept {}
		constexpr ~IMemberSelect() noexcept {}
	public:
		/// <summary>
		/// 指定したメンバ変数ポインタを使用して、メンバ変数への参照型を持つIEnumerableを取得する
		/// </summary>
		/// <typeparam name="Dst">メンバ変数型</typeparam>
		/// <typeparam name="M">メンバ変数ポインタ型</typeparam>
		/// <param name="member">メンバ変数ポインタ</param>
		template<class Dst>
		IEnumerable<Dst> MemberSelect(Dst T::*member) && noexcept {
			E* e = static_cast<E*>(this);
			return e->To(new MemberSelectEnumerator<Dst, T>(e->m_last, member));
		}
		template<class Dst>
		IEnumerable<Dst> MemberSelect(Dst T::*member) & noexcept {
			E* e = static_cast<E*>(this);
			return IEnumerable<Dst>(new MemberSelectEnumerator<Dst, T>(e->m_last->Clone(), member));
		}
	};
}

//CreateIEnumerable
export namespace System {
	/// <summary>
	/// コルーチン関数とその引数からIEnumerableを作成する
	/// </summary>
	/// <param name="func">コルーチン関数へのポインタ</param>
	/// <param name="args">
	/// コルーチン関数に渡される引数。
	/// コルーチン関数の引数が左辺値参照の場合、argsの値が左辺値か右辺値かで挙動が異なる。
	/// argsに左辺値が渡されたとき、IEnumerableはその参照を保持し、呼び出し元に存在する変数の値にも影響する。
	/// argsに右辺値が渡されたとき、IEnumerableはその値で内部変数を初期化し、呼び出し元には影響しない。
	/// ただし、コルーチン関数は内部変数の左辺値参照を引数にとるため、呼び出しごとの変化は累積する。
	/// </param>
	template<class T, class... Args>
	IEnumerable<T> CreateIEnumerable(IEnumerator<T>(*func)(Args...), auto&&... args) noexcept {
		return IEnumerable<T>(
			[func, &args...](bool) mutable -> IEnumerator<T> {
				return func(static_cast<Args&&>(args)...);
			},
			false
		);
	}
	/// <summary>
	/// コルーチン関数とその引数から反転可能なIEnumerableを作成する
	/// </summary>
	/// <param name="func">反転可能なコルーチン関数へのポインタ</param>
	/// <param name="reverse">コルーチンの進行方向の初期値。trueのとき、反転する</param>
	/// <param name="args">
	/// コルーチン関数に渡される引数。
	/// コルーチン関数の引数が左辺値参照の場合、argsの値が左辺値か右辺値かで挙動が異なる。
	/// argsに左辺値が渡されたとき、IEnumerableはその参照を保持し、呼び出し元に存在する変数の値にも影響する。
	/// argsに右辺値が渡されたとき、IEnumerableはその値で内部変数を初期化し、呼び出し元には影響しない。
	/// ただし、コルーチン関数は内部変数の左辺値参照を引数にとるため、呼び出しごとの変化は累積する。
	/// </param>
	template<class T, class... Args>
	IEnumerable<T> CreateIEnumerableReversible(IEnumerator<T>(*func)(bool, Args...), bool reverse, auto&&... args) noexcept {
		return IEnumerable<T>(
			[func, &args...](bool r) mutable -> IEnumerator<T> {
				return func(r, static_cast<Args&&>(args)...);
			},
			reverse
		);
	}
}

//使用例
/*
	IEnumerable<int> TestCoroutine(int x, double& y) noexcept {
		//bool値reverseで反転できるコルーチン関数を定義する。
		//ラムダ式を使ってもよいし、外部で定義してもよいが、引数を取るならラムダ式一択。
		//戻り値はIEnumerator<T>で、引数をWriteするならmutableの指定が必要。
		auto method = [&](bool reverse) mutable -> IEnumerator<int> {
			if (reverse) {
				for (int i = 5; i-- > 0;) {
					std::cout << "x: " << x-- << std::endl;
					std::cout << "y: " << y << std::endl;
					y -= 1.0;
					co_yield i;
				}
			} else {
				for (int i = 0; i < 5; ++i) {
					std::cout << "x: " << x++ << std::endl;
					std::cout << "y: " << y << std::endl;
					y += 1.0;
					co_yield i;
				}
			}
		};
		//関数ポインタとreverseの初期値を渡してIEnumerableを作成する。
		return IEnumerable<int>(method, false);
	}
*/
