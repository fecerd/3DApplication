export module HashMap;
import CSTDINT;
import Traits;
import Exception;
import Function;
export import Vector;
export import ICollection;

//Hash関数オブジェクト型
export namespace System {
	template<class T> class Hash {};
	template<Traits::Concepts::CArithmetic T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<Traits::Concepts::CSameRemoveCV<bool> T>
	class Hash<T> {
	public:
		constexpr size_t operator()(T arg) const noexcept { return static_cast<size_t>(arg); }
	};
	template<Traits::Concepts::CPointer T>
	class Hash<T> {
		using arg_type = Traits::remove_pointer_t<Traits::remove_cvref_t<T>>;
	public:
		inline size_t operator()(arg_type const* arg) const noexcept { return reinterpret_cast<size_t>(arg); }
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
		using enum_type = Traits::remove_cvref_t<T>;
	public:
		constexpr size_t operator()(enum_type arg) const noexcept {
			return static_cast<size_t>(static_cast<Traits::underlying_type_t<enum_type>>(arg));
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
		template<Traits::Concepts::CConstructibleTo<Key> K>
		HashNodeBase(K&& key) noexcept : m_key(System::move(key)) {}
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
		using value_type = Traits::one_of_t<i, Args...>;
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
		template<Traits::Concepts::CConstructibleTo<Key> K, Traits::Concepts::CConstructibleTo<Value> V>
		HashNode(K&& key, V&& value) noexcept
			: HashNodeBase<Key>(System::move(key)), m_value(System::move(value)) {}
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

//NodeTypeコンセプト
namespace System {
	namespace Internal {
		template<class T>
		struct is_node : Traits::false_type {};
		template<template<class, class...> class Node, class Key, class... Args>
		struct is_node<Node<Key, Args...>> : Traits::true_type {};
		template<template<class, class...> class Node, class Key, class... Args>
		struct is_node<const Node<Key, Args...>> : Traits::true_type {};
	}
	//一つのキーと0個以上の値をとるクラスのコンセプト
	template<class T> concept NodeType = Internal::is_node<T>::value;
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
		using iterator_concept = Traits::bidirectional_iterator_tag;
		using nodetype = T;
		using iterator_type = HashMapIterator<nodetype>;
		using toggle_iterator_type = HashMapIterator<Traits::toggle_const_t<nodetype>>;
		friend toggle_iterator_type;
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
			throw InvalidOperationException("異なるHashMapのイテレータ間で計算しようとしました。");
		}
		constexpr ptrdiff_t operator-(const toggle_iterator_type& rhs) const {
			return operator-(iterator_type{ rhs.m_current });
		}
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr bool operator==(const toggle_iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr operator bool() const noexcept { return m_current; }
	};
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
		template<size_t i> using argtype = Traits::one_of_t<i, Args...>;	//i番目の値型
		using maptype = HashMap<Key, Args...>;	//このHashMapの型
		using ckeytype = Traits::conditional_t<Traits::is_pointer_v<Key>, Traits::remove_pointer_t<Key> const*, Traits::remove_cvref_t<Key> const&>;
		using iteratortype = HashMapIterator<nodetype>;
		using const_iteratortype = HashMapIterator<nodetype>::toggle_iterator_type;
	private:
		Vector<nodetype*>* m_nodes = nullptr;	//ノードの動的配列(Vector<HashNode<Key, Args...>>)の配列
		nodetype* m_first = nullptr;	//存在するノードの中で一番最初に追加されたノード
		size_t m_nodesSize = 0;	//m_nodesの要素数
		Function<void(maptype&)> m_destructor;	//キーや値のデストラクタ
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
		constexpr iteratortype begin() noexcept { return iteratortype(m_first); }
		constexpr const_iteratortype begin() const noexcept { return const_iteratortype(m_first); }
		constexpr iteratortype end() noexcept { return iteratortype(); }
		constexpr const_iteratortype end() const noexcept { return const_iteratortype(); }
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
		template<Traits::Concepts::CConstructibleTo<Key> K, Traits::Concepts::CConstructibleTo<Args> ...A>
		bool Insert(K&& key, A&& ...args) noexcept {
			Key tmpkey { System::move(key) };
			size_t index = GetCode(tmpkey);
			if (index >= m_nodesSize) return false;	//普通はありえない
			Vector<nodetype*>& nodes = m_nodes[index];
			for (nodetype* node : nodes) {
				if (node->GetKey() == tmpkey) return false;
			}
			nodes.Add(new HashNode<Key, Args...>(System::move(tmpkey), System::move(args)...));
			HashNodeBase<Key>* tmp = nodes.Last();
			HashNodeBase<Key>* last = m_first ? m_first->GetLast() : nullptr;
			if (last) {
				last->SetNext(tmp);
				tmp->SetPrev(last);
			}
			else {
				m_first = static_cast<nodetype*>(tmp);
			}
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
		auto& At(ckeytype key, const srcloc& loc = srcloc::current()) requires(!Traits::Concepts::CPointer<argtype<i>>) {
			if constexpr (Traits::is_void_v<argtype<i>>) {
				static_assert("HashMap::At()に無効なインデックスが指定されました。");
				return *this;
			}
			else {
				nodetype* node = FindNode(key);
				if (!node) throw InvalidOperationException("存在しないキーが指定されました。", loc);
				return node->template GetValue<i>();
			}
		}
		/// <summary>
		/// キーと値のインデックスを指定して値を取得する。
		/// この関数は値がポインタ型の場合のみ定義される
		/// </summary>
		/// <param name="key">キー</param>
		template<size_t i = 0>
		auto At(ckeytype key) const noexcept requires(Traits::Concepts::CPointer<argtype<i>>) {
			if constexpr (Traits::is_void_v<argtype<i>>) {
				static_assert("HashMap::At()に無効なインデックスが指定されました。");
				return;
			}
			else {
				nodetype* node = FindNode(key);
				if (!node) return static_cast<argtype<i>>(nullptr);
				else return node->template GetValue<i>();
			}
		}
		/// <summary>
		/// キーと値のインデックスを指定して値へのポインタを取得する
		/// </summary>
		/// <param name="key">キー</param>
		template<size_t i = 0>
		auto AtPtr(ckeytype key) const noexcept requires(!Traits::Concepts::CPointer<argtype<i>>) {
			if constexpr (Traits::is_void_v<argtype<i>>) {
				static_assert("HashMap::AtPtr()に無効なインデックスが指定されました。");
				return;
			}
			else {
				nodetype* node = FindNode(key);
				if (!node) return static_cast<argtype<i>*>(nullptr);
				else return node->template GetValuePtr<i>();
			}
		}
	public:
		/// <summary>
		/// デストラクタの最初に呼び出される関数を設定する。
		/// KeyやValueが解放必須のポインタの場合に使用する
		/// </summary>
		/// <param name="destructor">このインスタンスへの参照を引数にとる関数</param>
		void SetDestructor(const Function<void(maptype&)>& destructor) noexcept {
			m_destructor = destructor;
			m_destructorInitialized = true;
		}
		void SetDestructor(Function<void(maptype&)>&& destructor) noexcept {
			m_destructor = System::move(destructor);
			m_destructorInitialized = true;
		}
		/// <summary>
		/// デストラクタの最初に呼び出される関数が設定されているか調べる
		/// </summary>
		bool IsInitializedDestructor() const noexcept { return m_destructorInitialized; }
	public://ICollectionオーバーライド
#if defined(__GNUC__) && !defined(__clang__)
		IEnumerator<nodetype> GetEnumerator(bool reverse = false) noexcept override {
			auto internal = [this](Boost::push_type<nodetype&>& sink) {
				for (nodetype& node : *this) {
					sink(node);
				}
			};
			auto internal_r = [this](Boost::push_type<nodetype&>& sink) {
				auto ite = iteratortype(static_cast<nodetype*>(m_first ? m_first->GetLast() : nullptr));
				auto beg = begin();
				for (; ite && ite != beg; --ite) {
					nodetype& ret = *ite;
					sink(ret);
				}
				if (beg) {
					nodetype& ret = *beg;
					sink(ret);
				}
			};
			return IEnumerator<nodetype>(no_mangling<nodetype>(internal, internal_r), reverse);
		}
		IEnumerator<nodetype const> GetEnumerator(bool reverse = false) const noexcept override {
			auto internal = [this](Boost::push_type<nodetype const&>& sink) {
				nodetype* current = m_first;
				while (current) {
					nodetype const& ret = *current;
					sink(ret);
					current = static_cast<nodetype*>(current->GetNext());
				}
			};
			auto internal_r = [this](Boost::push_type<nodetype const&>& sink) {
				HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
				while (current) {
					nodetype const& ret = *static_cast<nodetype*>(current);
					sink(ret);
					current = current->GetPrev();
				}
			};
			return IEnumerator<nodetype const>(no_mangling<nodetype const>(internal, internal_r), reverse);
		}
#else
	private:
		IEnumerator<nodetype> Internal(bool r) noexcept {
			if (r) {
				HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
				while (current) {
					co_yield *static_cast<nodetype*>(current);
					current = current->GetPrev();
				}
			} else {
				nodetype* current = m_first;
				while (current) {
					co_yield *current;
					current = static_cast<nodetype*>(current->GetNext());
				}
			}
		}
		IEnumerator<nodetype const> Internal(bool r) const noexcept {
			if (r) {
				HashNodeBase<Key>* current = m_first ? m_first->GetLast() : nullptr;
				while (current) {
					co_yield *static_cast<nodetype*>(current);
					current = current->GetPrev();
				}
			} else {
				nodetype* current = m_first;
				while (current) {
					co_yield *current;
					current = static_cast<nodetype*>(current->GetNext());
				}
			}

		}
	public:
		IEnumerator<nodetype> GetEnumerator(bool reverse = false) noexcept override {
			return IEnumerator<nodetype>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
		IEnumerator<nodetype const> GetEnumerator(bool reverse = false) const noexcept override {
			return IEnumerator<nodetype const>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
#endif
	public:
		template<size_t i = 0>
		auto Values() noexcept {
			if constexpr (Traits::is_void_v<argtype<i>>) {
				static_assert("HashMap::Value()に無効なインデックスが指定されました。");
				return;
			}
			else {
				return IEnumerable<argtype<i>>(
					new MemberSelectEnumerator<argtype<i>, nodetype>(
						new IEnumerator<nodetype>(GetEnumerator(false)),
						&nodetype::m_value
					)
				);
			}
		}
		template<size_t i = 0>
		auto Values() const noexcept {
			if constexpr (Traits::is_void_v<argtype<i>>) {
				static_assert("HashMap::Value()に無効なインデックスが指定されました。");
				return;
			}
			else {
				return IEnumerable<argtype<i> const>(
					new MemberSelectEnumerator<argtype<i> const, nodetype const>(
						new IEnumerator<nodetype const>(GetEnumerator(false)),
						&nodetype::m_value
					)
				);
			}
		}
	public:
		HashMap& operator=(const HashMap&) noexcept = delete;
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
