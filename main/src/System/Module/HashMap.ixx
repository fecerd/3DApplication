module;
#include<crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module HashMap;
import CSTDINT;
import Exception;
import IEnumerable;
import Traits;
import Vector;
import Function;
using IEnumerable = System::IEnumerable;

//Hash関数オブジェクト型
export namespace System {
	template<class T> class Hash {};
	template<System::Traits::Arithmetic T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<class T> requires(System::Traits::is_same_v<System::Traits::remove_cv_t<T>, bool>)
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<System::Traits::Pointer T>
	class Hash<T> {
	public:
		inline size_t operator()(const Traits::remove_pointer_t<Traits::remove_cvref_t<T>>* arg) const noexcept { return reinterpret_cast<size_t>(arg); }
	};
	template<class T> requires requires(const T& x) { x.GetHashCode(); }
	class Hash<T> {
	public:
		inline size_t operator()(const T& arg) const noexcept {
			return static_cast<size_t>(arg.GetHashCode());
		}
	};
	template<Traits::Concepts::CEnum T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept {
			return static_cast<size_t>(static_cast<System::Traits::underlying_type_t<T>>(arg));
		}
	};
}

//HashNodeBase, HashNode
export namespace System {
	/// <summary>
	/// HashNodeの基底クラス
	/// </summary>
	/// <typeparam name="Key">キー型</typeparam>
	template<class Key>
	struct HashNodeBase {
	protected:
		HashNodeBase* m_prev = nullptr;	//HashMap内で直前に追加されたノード
		HashNodeBase* m_next = nullptr;	//HashMap内で直後に追加されたノード
		Key m_key;	//キー
	public:
		template<Traits::Constructible<Key> K>
		HashNodeBase(K&& key) noexcept : m_key(static_cast<K&&>(key)) {}
	public:
		/// <summary>
		/// キーの参照を取得する
		/// </summary>
		Key& GetKey() noexcept { return m_key; }
		const Key& GetKey() const noexcept { return m_key; }
	public:
		/// <summary>
		/// このノードが存在するHashMap内で一番最初に追加されたノードを取得する
		/// </summary>
		HashNodeBase* GetFirst() noexcept { return m_prev ? m_prev->GetFirst() : this; }
		/// <summary>
		/// このノードが存在するHashMap内で一番最後に追加されたノードを取得する
		/// </summary>
		HashNodeBase* GetLast() noexcept { return m_next ? m_next->GetLast() : this; }
		/// <summary>
		/// 直前のノードを設定する
		/// </summary>
		/// <param name="prev">設定するノード</param>
		void SetPrev(HashNodeBase* prev) noexcept { m_prev = prev; }
		/// <summary>
		/// 直前のノードを取得する
		/// </summary>
		HashNodeBase* GetPrev() const noexcept { return m_prev; }
		/// <summary>
		/// 直後のノードを設定する
		/// </summary>
		/// <param name="prev">設定するノード</param>
		void SetNext(HashNodeBase* next) noexcept { m_next = next; }
		/// <summary>
		/// 直後のノードを取得する
		/// </summary>
		HashNodeBase* GetNext() const noexcept { return m_next; }
	};

	/// <summary>
	/// HashNodeのひな型
	/// </summary>
	/// <typeparam name="Key"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	template<class Key, class ...Args>
	struct HashNode : public HashNodeBase<Key> {
		template<size_t i>
		using value_type = System::Traits::one_of_t<i, Args...>;
	public:
		using HashNodeBase<Key>::GetKey;
		template<size_t i = 0> value_type<i>& GetValue() noexcept;
		template<size_t i = 0> value_type<i>* GetValuePtr() noexcept;
	};
}

//HashNodes
export namespace System {
	/// <summary>
	/// 値を一つ持つHashMap用ノード
	/// </summary>
	/// <typeparam name="Key">キー型</typeparam>
	/// <typeparam name="Value">値型</typeparam>
	template<class Key, class Value>
	struct HashNode<Key, Value> : public HashNodeBase<Key> {
		using HashNodeBase<Key>::m_next;
		using HashNodeBase<Key>::m_key;
		Value m_value;
	public:
		template<Traits::Constructible<Key> K, Traits::Constructible<Value> V>
		HashNode<Key, Value>(K&& key, V&& value) noexcept
			: HashNodeBase<Key>(static_cast<K&&>(key)), m_value(static_cast<V&&>(value)) {}
	public:
		using HashNodeBase<Key>::GetKey;
		/// <summary>
		/// 値を取得する
		/// </summary>
		template<size_t i = 0> requires(i == 0)
		Value& GetValue() noexcept { return m_value; }
		/// <summary>
		/// 値へのポインタを取得する
		/// </summary>
		template<size_t i = 0> requires(i == 0)
		Value* GetValuePtr() noexcept { return &m_value; }
	};
}

namespace System {
	template<class T>
	struct is_node : System::Traits::false_type {};
	template<template<class, class...> class Node, class Key, class ...Args>
	struct is_node<Node<Key, Args...>> : System::Traits::true_type {};
	template<template<class, class...> class Node, class Key, class ...Args>
	struct is_node<const Node<Key, Args...>> : System::Traits::true_type {};

	template<class T>
	concept NodeType = is_node<T>::value;

	template<class T>
	struct toggle_const {
		using type = const T;
	};
	template<class T>
	struct toggle_const<const T> {
		using type = System::Traits::remove_const_t<T>;
	};
}

//HashMapIterator
export namespace System {
	/// <summary>
	/// HashMap::begin(), end()が返す双方向イテレータ
	/// </summary>
	/// <typeparam name="Key">キー型</typeparam>
	/// <typeparam name="...Args">値型</typeparam>
	template<NodeType T>
	struct HashMapIterator {
		using iterator_concept = System::Traits::bidirectional_iterator_tag;
		using nodetype = T;
		using iterator_type = HashMapIterator<T>;
		using toggle_iterator_type = HashMapIterator<typename toggle_const<T>::type>;
		friend struct HashMapIterator<typename toggle_const<T>::type>;
	protected:
		//現在指しているノード。nullptrの場合、最終ノードの後ろを表す
		nodetype* m_current = nullptr;
	public:
		constexpr HashMapIterator() noexcept : m_current(nullptr) {}
		constexpr HashMapIterator(nodetype* node) noexcept : m_current(node) {}
		constexpr ~HashMapIterator() noexcept { m_current = nullptr; }
	public:
		constexpr iterator_type& operator++() noexcept {
			if (!m_current) return *this;
			m_current = static_cast<nodetype*>(m_current->GetNext());
			return *this;
		}
		constexpr iterator_type operator++(int) noexcept {
			if (!m_current) return iterator_type(m_current);
			iterator_type ret(m_current);
			m_current = static_cast<nodetype*>(m_current->GetNext());
			return ret;
		}
		constexpr iterator_type& operator--() noexcept {
			if (!m_current) return *this;
			m_current = static_cast<nodetype*>(m_current->GetPrev());
			return *this;
		}
		constexpr iterator_type operator--(int) noexcept {
			if (!m_current) return iterator_type(m_current);
			iterator_type ret(m_current);
			m_current = static_cast<nodetype*>(m_current->GetPrev());
			return ret;
		}
		constexpr nodetype& operator*() const noexcept { return *m_current; }
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const {
			if (!m_current && !rhs.m_current) return 0;
			ptrdiff_t ret = 0;
			nodetype* tmp = nullptr;
			if (!m_current || !rhs.m_current) {	//thisとrhsの片方が最終ノードの次(nullptr)を指している場合
				if (!m_current) {
					tmp = rhs.m_current;
					while (tmp) {
						++ret;
						tmp = tmp->GetNext();
					}
					return ret;
				}
				else {
					tmp = m_current;
					while (tmp) {
						--ret;
						tmp = tmp->GetNext();
					}
					return ret;
				}
			}
			else {	//thisとrhsがともにノードを指している場合
				tmp = m_current;
				while (tmp) {
					if (tmp == rhs.m_current) return ret;
					++ret;
					tmp = tmp->GetNext();
				}
				ret = -1;
				tmp = m_current->GetPrev();
				while (tmp) {
					if (tmp == rhs.m_current) return ret;
					--ret;
					tmp = tmp->GetPrev();
				}
			}
			throw InvalidOperationException(__FUNCSIG__, "異なるHashMapのイテレータ間で計算しようとしました。", __FILE__, __LINE__);
		}
		constexpr ptrdiff_t operator-(const toggle_iterator_type& rhs) const {
			return operator-(iterator_type{ rhs.m_current });
		}
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr bool operator==(const toggle_iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
	};
}

//argtypes
//one_of_t<i, Args>は範囲外でvoid型を返してコンパイルエラーを引き起こすため、
//ダミー型を利用して抑制する。
//無効なインデックスiはstatic_assertで知らせる
namespace System {
	struct ArgDummy {};
	template<size_t i, class T, class ...Args>
	struct argtypes : argtypes<i - 1, Args...> {};
	template<class T, class ...Args>
	struct argtypes<0, T, Args...> { using type = T; };
	template<size_t i, class T>
	struct argtypes<i, T> { using type = ArgDummy; };
}

//HashMap
export namespace System {
	/// <summary>
	/// キーと値の組み合わせを管理するコンテナクラス。
	/// キーの重複は認めないが、ノードには複数の値を格納できる
	/// </summary>
	/// <typeparam name="Key">キー型</typeparam>
	/// <typeparam name="...Args">値型</typeparam>
	template<class Key, class ...Args>
	class HashMap : public ICollection<HashNode<Key, Args...>> {
		using nodetype = HashNode<Key, Args...>;	//使用されるノード型
		template<size_t i> using argtype = argtypes<i, Args...>::type;	//i番目の値型
		template<size_t i> static constexpr bool enable_argtype_v = i < sizeof...(Args);	//i番目の値が存在するか
		using maptype = HashMap<Key, Args...>;	//このHashMapの型
		using ckeytype = System::Traits::conditional_t<Traits::is_pointer_v<Key>, const Traits::remove_pointer_t<Traits::remove_cvref_t<Key>>*, const Traits::remove_cvref_t<Key>&>;
	private:
		Vector<nodetype*>* m_nodes = nullptr;	//ノードの動的配列(Vector<HashNode<Key, Args...>>)の配列
		nodetype* m_first = nullptr;	//存在するノードの中で一番最初に追加されたノード
		size_t m_nodesSize = 0;	//m_nodesの要素数
		System::Function<void(HashMap<Key, Args...>&)> m_destructor;	//キーや値のデストラクタ
		bool m_destructorInitialized = false;	//m_destructorが初期化されているか
	public:
		HashMap() noexcept : HashMap(10) {}
		HashMap(const HashMap&) noexcept = delete;
		HashMap(HashMap&& arg) noexcept : m_nodes(arg.m_nodes), m_nodesSize(arg.m_nodesSize), m_first(arg.m_first) {
			arg.m_nodes = nullptr;
			arg.m_nodesSize = 0;
			arg.m_first = nullptr;
		}
		/// <summary>
		/// 2の乗数でハッシュ値の法を指定して、初期化する
		/// </summary>
		/// <param name="n">2の乗数。3の場合、8(2^3)をハッシュ値の法として使用する</param>
		HashMap(uint8_t n) noexcept : m_nodesSize(1ull << (n > 63 ? 63 : n)) {
			m_nodes = new Vector<nodetype*>[m_nodesSize];
		}
		~HashMap() noexcept {
			if (m_destructorInitialized) m_destructor(*this);
			if (m_nodes) {
				for (size_t i = 0; i < m_nodesSize; ++i) m_nodes[i].DeleteAll();
				delete[] m_nodes;
				m_nodes = nullptr;
			}
			m_first = nullptr;
			m_nodesSize = 0;
		}
	private:
		constexpr size_t GetCode(ckeytype key) const noexcept { return Hash<Key>()(key) & (m_nodesSize - 1); }
		constexpr nodetype* FindNode(ckeytype key) const noexcept {
			size_t index = GetCode(key);
			if (index >= m_nodesSize) return nullptr;
			Vector<nodetype*>& nodes = m_nodes[index];
			for (nodetype* node : nodes) if (node->GetKey() == key) return node;
			return nullptr;
		}
	public:
		constexpr HashMapIterator<nodetype> begin() noexcept {
			return HashMapIterator<nodetype>(m_first);
		}
		constexpr HashMapIterator<const nodetype> begin() const noexcept {
			return HashMapIterator<const nodetype>(m_first);
		}
		constexpr HashMapIterator<nodetype> end() noexcept {
			return HashMapIterator<nodetype>();
		}
		constexpr HashMapIterator<const nodetype> end() const noexcept {
			return HashMapIterator<const nodetype>();
		}
	public:
		/// <summary>
		/// キーが含まれているか調べる
		/// </summary>
		/// <param name="key">キー</param>
		constexpr bool Contains(ckeytype key) const noexcept { return FindNode(key); }
		constexpr bool Any() const noexcept { return begin() != end(); }
		constexpr size_t Count() const noexcept {
			size_t ret = 0;
			for (size_t i = 0; i < m_nodesSize; ++i) {
				Vector<nodetype*>& nodes = m_nodes[i];
				ret += nodes.Count();
			}
			return ret;
		}
	public:
		/// <summary>
		/// キーと値を指定してHashMapに追加する
		/// </summary>
		/// <param name="key">キー。Key型もしくはKey型コンストラクタに渡せる型</param>
		/// <param name="...args">
		/// 値のパラメータパック。Args型もしくはArgs型コンストラクタに渡せる型。
		/// 並び順はArgsと一致する
		/// </param>
		/// <returns>追加に成功したとき、true。すでに存在するキーを指定した場合、false</returns>
		template<Traits::Constructible<Key> K, Traits::Constructible<Args...> ...A>
		bool Insert(K&& key, A&& ...args) noexcept {
			Key tmpkey = Key(static_cast<K&&>(key));
			size_t index = GetCode(tmpkey);
			if (index >= m_nodesSize) return false;	//普通はありえない
			Vector<nodetype*>& nodes = m_nodes[index];
			for (nodetype* node : nodes) if (node->GetKey() == tmpkey) return false;
			nodes.Add(new HashNode<Key, Args...>(static_cast<Key&&>(tmpkey), static_cast<A&&>(args)...));
			HashNodeBase<Key>* tmp = nodes.Last();
			HashNodeBase<Key>* last = m_first ? m_first->GetLast() : nullptr;
			if (last) {
				last->SetNext(tmp);
				tmp->SetPrev(last);
			}
			else m_first = static_cast<nodetype*>(tmp);
			return true;
		}
		/// <summary>
		/// キーを指定してHashMapからデータを削除する
		/// </summary>
		/// <param name="key">キー</param>
		/// <returns>削除に成功した場合、true。存在しないキーを指定した場合、false</returns>
		bool Remove(ckeytype key) noexcept {
			size_t index = GetCode(key);
			if (index >= m_nodesSize) return false;
			Vector<nodetype*>& nodes = m_nodes[index];
			for (size_t i = 0, count = nodes.Count(); i < count; ++i) {
				nodetype* node = nodes[i];
				if (node->GetKey() == key) {
					HashNodeBase<Key>* prev = node->GetPrev();
					HashNodeBase<Key>* next = node->GetNext();
					if (prev || next) {	//最後の一つではない
						if (prev) {
							prev->SetNext(next);
							if (next) next->SetPrev(prev);
						}
						else {
							next->SetPrev(nullptr);
							m_first = static_cast<nodetype*>(next);
						}
					}
					else m_first = nullptr;
					delete node;
					nodes.RemoveByIndex(i);
					return true;
				}
			}
			return false;
		}
		/// <summary>
		/// すべてのノードを解放する。関数呼び出し後はコンストラクト直後の状態となる
		/// </summary>
		void Clear() noexcept {
			for (size_t i = 0; i < m_nodesSize; ++i) {
				Vector<nodetype*>& nodes = m_nodes[i];
				nodes.DeleteAll();
			}
			m_first = nullptr;
		}
	public:
		/// <summary>
		/// キーと値のインデックスを指定して値への参照を取得する。
		/// この関数は値が非ポインタ型の場合のみ定義される
		/// </summary>
		/// <param name="key">キー</param>
		/// <exception>存在しないキーを指定した場合、InvalidOperationExceptionを送出する</exception>
		template<size_t i = 0>
		argtype<i>& At(ckeytype key, srcloc loc = srcloc::current()) requires(!System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) throw InvalidOperationException(__FUNCSIG__, "存在しないキーが指定されました。", loc);
			return node->GetValue<i>();
		}
		/// <summary>
		/// キーと値のインデックスを指定して値を取得する。
		/// この関数は値がポインタ型の場合のみ定義される
		/// </summary>
		/// <param name="key">キー</param>
		template<size_t i = 0>
		argtype<i> At(ckeytype key) const noexcept requires(System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) return nullptr;
			return node->GetValue<i>();
		}
		/// <summary>
		/// キーと値のインデックスを指定して値へのポインタを取得する
		/// </summary>
		/// <param name="key">キー</param>
		template<size_t i = 0>
		argtype<i>* AtPtr(ckeytype key) const noexcept requires(!System::Traits::is_pointer_v<argtype<i>>) {
			static_assert(enable_argtype_v<i>, "Invalid index specified for HashMap::AtPtr().");
			nodetype* node = FindNode(key);
			if (!node) return nullptr;
			return node->GetValuePtr<i>();
		}
	public:
		/// <summary>
		/// デストラクタの最初に呼び出される関数を設定する。
		/// KeyやValueが解放必須のポインタの場合に使用する
		/// </summary>
		/// <param name="destructor">このインスタンスへの参照を引数にとる関数</param>
		void SetDestructor(const System::Function<void(HashMap<Key, Args...>&)>& destructor) noexcept {
			m_destructor = destructor;
			m_destructorInitialized = true;
		}
		void SetDestructor(System::Function<void(HashMap<Key, Args...>&)>&& destructor) noexcept {
			m_destructor = static_cast<System::Function<void(HashMap<Key, Args...>&)>&&>(destructor);
			m_destructorInitialized = true;
		}
		/// <summary>
		/// デストラクタの最初に呼び出される関数が設定されているか調べる
		/// </summary>
		bool IsInitializedDestructor() const noexcept { return m_destructorInitialized; }
	public://ICollectionオーバーライド
		IEnumerator<nodetype> GetEnumerator() noexcept override {
			nodetype* current = m_first;
			while (current) {
				co_yield *current;
				current = static_cast<nodetype*>(current->GetNext());
			}
		}
		IEnumerator<nodetype const> GetEnumerator() const noexcept override {
			nodetype* current = m_first;
			while (current) {
				co_yield *current;
				current = static_cast<nodetype*>(current->GetNext());
			}
		}
		IEnumerator<nodetype> GetReverseEnumerator() noexcept override {
			HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
			while (current) {
				co_yield *static_cast<nodetype*>(current);
				current = current->GetPrev();
			}
		}
		IEnumerator<nodetype const> GetReverseEnumerator() const noexcept override {
			HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
			while (current) {
				co_yield *static_cast<nodetype*>(current);
				current = current->GetPrev();
			}
		}
	public:
		/// <summary>
		/// 指定したインデックスの値(参照)を列挙する
		/// </summary>
		template<size_t i = 0>
		IEnumerable<argtype<i>> Values() noexcept {
			if constexpr (i == 0)
				return IEnumerable<argtype<i>>(
					new MemberSelectEnumerator<argtype<i>, nodetype>(
						new IEnumerator<nodetype>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, false),
						&nodetype::m_value
						)
					);
			else {
				static_assert(i <= 0, "Invalid index specified for HashMap::Values().");
			}
		}
		template<size_t i = 0>
		IEnumerable<argtype<i> const> Values() const noexcept {
			if constexpr (i == 0)
				return IEnumerable<argtype<i> const>(
					new MemberSelectEnumerator<argtype<i> const, nodetype const>(
						new IEnumerator<nodetype const>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, false),
						&nodetype::m_value
						)
					);
			else {
				static_assert(i <= 0, "Invalid index specified for HashMap::Values().");
			}
		}
	public:
		HashMap& operator=(HashMap&& rhs) noexcept {
			if (this == &rhs) return *this;
			Clear();
			delete[] m_nodes;
			m_nodes = rhs.m_nodes;
			m_first = rhs.m_first;
			m_nodesSize = rhs.m_nodesSize;
			rhs.m_nodes = nullptr;
			rhs.m_first = nullptr;
			rhs.m_nodesSize = 0;
			return *this;
		}
	};
}