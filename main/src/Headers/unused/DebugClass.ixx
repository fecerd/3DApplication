export module DebugClass;

export namespace DebugClass {
	using size_t = unsigned long long;

	template<class T>
	struct DebugIterator {
		using iterator_type = DebugIterator<T>;
		using value_type = T;
	protected:
		value_type* m_current = nullptr;
	public:
		constexpr DebugIterator() noexcept : m_current(nullptr) {}
		constexpr DebugIterator(value_type* node) noexcept : m_current(node) {}
		constexpr ~DebugIterator() noexcept { m_current = nullptr; }
	public:
		constexpr iterator_type& operator++() noexcept { return *this; }
		constexpr iterator_type operator++(int) noexcept { return iterator_type(m_current); }
		constexpr iterator_type& operator--() noexcept { return *this; }
		constexpr iterator_type operator--(int) noexcept { return iterator_type(m_current); }
		constexpr value_type& operator*() const noexcept { return *m_current; }
		constexpr long long operator-(const iterator_type& rhs) const { return 0; }
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return m_current == rhs.m_current; }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
	};

	template<class T>
	struct DebugEnumerator {
	public:
		DebugIterator<T> begin() const noexcept { return DebugIterator<T>(); }
		DebugIterator<T> end() const noexcept { return DebugIterator<T>(); }
		DebugEnumerator<T>& operator++() noexcept { return *this; }
		explicit operator bool() const noexcept { return false; }
	};

	template<class Key, class Value, class ...Args>
	struct DebugNode {
		using key_type = Key;
		using value_type = Value;
	public:
		DebugNode() noexcept {}
		template<class K, class V, class ...A>
		DebugNode(K&& k, V&& v, A&& ...a) noexcept {}
	public:
		Key& GetKey() const noexcept { return *static_cast<Key*>(nullptr); }
		template<size_t i = 0>
		Value& GetValue() const noexcept { return *static_cast<Value*>(nullptr); }
		template<size_t i = 0>
		Value& GetValuePtr() const noexcept { return static_cast<Value*>(nullptr); }
	public:
		DebugNode* GetFirst() const noexcept { return static_cast<DebugNode*>(nullptr); }
		DebugNode* GetLast() const noexcept { return static_cast<DebugNode*>(nullptr); }
		void SetPrev(DebugNode*) const noexcept {}
		void SetNext(DebugNode) const noexcept {}
		DebugNode* GetPrev() const noexcept { return static_cast<DebugNode*>(nullptr); }
		DebugNode* GetNext() const noexcept { return static_cast<DebugNode*>(nullptr); }
	};

	template<class Key, class ...Args>
	class HashMap {
		using iterator_type = DebugIterator<DebugNode<Key, Args...>>;
		unsigned char m_unused = 0;
		DebugNode<Key, Args...> m_node = {};
	public:
		HashMap() noexcept : HashMap(10) {}
		HashMap(const HashMap&) noexcept = delete;
		HashMap(HashMap&& arg) noexcept = default;
		HashMap(unsigned char n) noexcept : m_unused(n) {}
		~HashMap() noexcept {}
	public:
		constexpr iterator_type begin() const noexcept { return iterator_type(); }
		constexpr iterator_type end() const noexcept { return iterator_type(); }
	public:
		constexpr bool Contains(Key const& key) const noexcept { return false; }
		constexpr bool Any() const noexcept { return begin() != end(); }
		constexpr size_t Count() const noexcept { return 0; }
	public:
		template<class K, class ...A>
		bool Insert(K&& key, A&& ...args) noexcept { return false; }
		bool Remove(Key const& key) noexcept { return false; }
		void Clear() noexcept {}
	public:
		template<size_t i = 0>
		auto& At(Key const& key) const { return m_node.template GetValue<i>(); }
		template<size_t i = 0>
		auto& At(Key&& key) const { return m_node.template GetValue<i>(); }
		template<size_t i = 0>
		auto AtPtr(Key const& key) const noexcept { return m_node.template GetValuePtr<i>(); }
	public:
		template<class F> void SetDestructor(F const&) noexcept {}
		template<class F> void SetDestructor(F&&) noexcept {}
		bool IsInitializedDestructor() const noexcept { return false; }
	public:
		auto GetEnumerator(bool reverse = false) noexcept {
			return DebugEnumerator<DebugNode<Key, Args...>>();
		}
		auto GetEnumerator(bool reverse = false) const noexcept {
			return DebugEnumerator<DebugNode<Key, Args...> const>();
		}
	public:
		template<size_t i = 0>
		auto Values() noexcept {
			return DebugEnumerator<typename DebugNode<Key, Args...>::value_type>();
		}
		template<size_t i = 0>
		auto Values() const noexcept {
			return DebugEnumerator<typename DebugNode<Key, Args...>::value_type const>();
		}
	public:
		HashMap& operator=(const HashMap&) noexcept = delete;
		HashMap& operator=(HashMap&& rhs) noexcept { return *this; }
	};

}
