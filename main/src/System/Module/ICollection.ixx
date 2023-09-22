export module ICollection;
import Traits;
import Function;
export import IEnumerable;
export import BoostCoroutine;

//no_mangling
#if defined(__GNUC__) && !defined(__clang__)
//g++では3つ以上のラムダ式をテンプレートクラスのメンバ関数内に持つとname manglingが正常に設定されないエラーが起きることがある。
//このクラスを使用してラムダ式の数を減らすことができる。
export namespace System {
	template<class T>
	struct no_mangling {
		Function<void(Boost::push_type<T&>&)> m_internal;
		Function<void(Boost::push_type<T&>&)> m_internal_r;
	public:
		template<class F, class Fr>
		no_mangling(F&& f, Fr&& fr) noexcept : m_internal(System::forward<F>(f)), m_internal_r(System::forward<Fr>(fr)) {}
		no_mangling(const no_mangling& arg) noexcept : m_internal(arg.m_internal), m_internal_r(arg.m_internal_r) {}
		no_mangling(no_mangling&& arg) noexcept : m_internal(System::move(arg.m_internal)), m_internal_r(System::move(arg.m_internal_r)) {}
		~no_mangling() noexcept {}
	public:
		no_mangling& operator=(const no_mangling& rhs) noexcept {
			if (this == &rhs) return *this;
			m_internal = rhs.m_internal;
			m_internal_r = rhs.m_internal_r;
			return *this;
		}
		no_mangling& operator=(no_mangling&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_internal = System::move(rhs.m_internal);
			m_internal_r = System::move(rhs.m_internal_r);
			return *this;
		}
	public:
		IEnumerator<T> operator()(bool r) const {
			return IEnumerator<T>(r
				? m_internal_r
				: m_internal
			);
		}
	};
}
#endif

//ICollectionMemberSelect
export namespace System::Internal {
	template<class T, class C>
	class ICollectionMemberSelect {
	public:
		constexpr ICollectionMemberSelect() noexcept {}
		constexpr ~ICollectionMemberSelect() noexcept {}
	};
	template<Traits::Concepts::CHasMember T, class C>
	class ICollectionMemberSelect<T, C> {
	public:
		constexpr ICollectionMemberSelect() noexcept {}
		constexpr ~ICollectionMemberSelect() noexcept {}
	public:
		/// <summary>
		/// このコンテナが持つ要素の指定したメンバ変数を列挙するIEnumerableを取得する。
		/// この関数によって取得したIEnumerableのイテレータは指定したメンバ変数への参照を表す
		/// </summary>
		/// <typeparam name="Dst">メンバ変数型</typeparam>
		/// <param name="member">要素型が持つメンバ変数ポインタ</param>
		template<class Dst>
		IEnumerable<Dst> MemberSelect(Dst T::* member) noexcept {
			return IEnumerable<Dst>(
				new MemberSelectEnumerator<Dst, T>(static_cast<C*>(this)->GetPtr(), member)
			);
		}
		template<class Dst>
		IEnumerable<Dst const> MemberSelect(Dst T::* member) const noexcept {
			return IEnumerable<Dst const>(
				new SelectEnumerator<Dst const, T const>(static_cast<C const*>(this)->GetPtr(), member)
			);
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
	class ICollection : public Internal::ICollectionMemberSelect<T, ICollection<T>> {
		friend class Internal::ICollectionMemberSelect<T, ICollection<T>>;
	public:
		constexpr ICollection() noexcept {}
		virtual ~ICollection() noexcept {}
	public:
		virtual IEnumerator<T> GetEnumerator(bool reverse = false) noexcept = 0;
		virtual IEnumerator<T const> GetEnumerator(bool reverse = false) const noexcept = 0;
	private:
		IEnumerator<T>* GetPtr(bool reverse = false) noexcept {
			return new IEnumerator<T>([this](bool r) { return GetEnumerator(r); }, reverse);
		}
		IEnumerator<T const>* GetPtr(bool reverse = false) const noexcept {
			return new IEnumerator<T const>([this](bool r) { return GetEnumerator(r); }, reverse);
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
		IEnumerable<Dst> Select(const Function<Dst(T&)>& func) noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T>(GetPtr(), func));
		}
		template<class Dst>
		IEnumerable<Dst> Select(const Function<Dst(T const&)>& func) const noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T const>(GetPtr(), func));
		}
		/// <summary>
		/// このコンテナが持つ要素の中から指定した関数がtrueを返すものを列挙するIEnumerableを取得する
		/// </summary>
		/// <param name="func">要素型へのconst参照を引数にとり、bool型を返す条件関数</param>
		IEnumerable<T> Where(const Function<bool(T const&)>& func) noexcept { return IEnumerable<T>(new WhereEnumerator<T>(GetPtr(), func)); }
		IEnumerable<T const> Where(const Function<bool(T const&)>& func) const noexcept { return IEnumerable<T const>(new WhereEnumerator<T const>(GetPtr(), func)); }
		/// <summary>
		/// このコンテナが持つ要素を逆順に列挙するIEnumerableを取得する
		/// </summary>
		IEnumerable<T> Reverse() noexcept { return IEnumerable<T>(GetPtr(true)); }
		IEnumerable<T const> Reverse() const noexcept { return IEnumerable<T const>(GetPtr(true)); }
	};
}
