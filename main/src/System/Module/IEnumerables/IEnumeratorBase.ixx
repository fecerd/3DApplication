export module IEnumeratorBase;
import Traits;
import Exception;

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
	public:
		//コルーチン関数の状態をここに保持する
		bool m_done = false;
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
		constexpr EnumerableIterator() noexcept = default;
		constexpr EnumerableIterator(IEnumeratorBase<T>* enumerator) noexcept : m_enumerator(enumerator) {}
		constexpr ~EnumerableIterator() noexcept { m_enumerator = nullptr; }
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
			if (!m_enumerator) throw InvalidOperationException(u"イテレータがIEnumeratorへのポインタを持っていません。");
			return m_enumerator->operator*();
		}
		constexpr bool operator==(const EnumerableIterator<T>& rhs) const noexcept { return this->Done() == rhs.Done(); }
		constexpr bool operator!=(const EnumerableIterator<T>& rhs) const noexcept { return this->Done() != rhs.Done(); }
		constexpr EnumerableIterator<T>& operator=(const EnumerableIterator&) noexcept = default;
	};
}
