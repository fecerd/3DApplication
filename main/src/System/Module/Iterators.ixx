export module Iterators;
import CSTDINT;
import Traits;

export namespace System {
	struct IContainerNode {};

	template<class N>
	concept CContainerNode = Traits::is_base_of_v<IContainerNode, N> && requires(N n) {
		{ n + size_t(0) } -> Traits::Concepts::convertible_to<N*>;
		{ n - size_t(0) } -> Traits::Concepts::convertible_to<N*>;
		*n;
		{ n - n } -> Traits::Concepts::convertible_to<ptrdiff_t>;
	};
}

namespace System::Iterator_Internal {
	template<class ptr_type>
	void Increment(ptr_type& ptr) noexcept {
		using node_type = Traits::remove_cvref_t<decltype(*ptr)>;
		if constexpr (CContainerNode<node_type>) ptr = *ptr + 1;
		else ++ptr;
	}
	template<class ptr_type> void Decrement(ptr_type&) noexcept;
	template<class ptr_type> void Decrement(ptr_type&, ptrdiff_t) noexcept;
	template<class ptr_type>
	void Increment(ptr_type& ptr, ptrdiff_t count) noexcept {
		using node_type = Traits::remove_cvref_t<decltype(*ptr)>;
		if (count >= 0) {
			if constexpr (CContainerNode<node_type>) ptr = *ptr + count;
			else ptr += count;
		}
		else {
			if (count == MIN_VALUE<ptrdiff_t>) {
				Decrement(ptr);
				Decrement(ptr, -(count - 1));
			}
			else Decrement(ptr, -count);
		}
	}
	template<class ptr_type>
	void Decrement(ptr_type& ptr) noexcept {
		using node_type = Traits::remove_cvref_t<decltype(*ptr)>;
		if constexpr (CContainerNode<node_type>) ptr = *ptr - 1;
		else --ptr;
	}
	template<class ptr_type> void Decrement(ptr_type& ptr, ptrdiff_t count) noexcept {
		using node_type = Traits::remove_cvref_t<decltype(*ptr)>;
		if (count >= 0) {
			if constexpr (CContainerNode<node_type>) ptr = *ptr - count;
			else ptr -= count;
		}
		else {
			if (count == MIN_VALUE<ptrdiff_t>) {
				Increment(ptr);
				Increment(ptr, -(count - 1));
			}
			else Increment(ptr, -count);
		}
	}
	template<class ptr_type>
	auto& Get(ptr_type& ptr) noexcept {
		using node_type = Traits::remove_cvref_t<decltype(*ptr)>;
		if constexpr (CContainerNode<node_type>) return *ptr;
		else return ptr;
	}
}
using namespace System::Iterator_Internal;

//Iterators
export namespace System {
	template<class Node>
	struct InputIterator {
	public:
		using iterator_concept = Traits::input_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = InputIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr InputIterator() noexcept = default;
		constexpr InputIterator(const InputIterator&) noexcept = default;
		constexpr InputIterator(InputIterator&&) noexcept = default;
		constexpr InputIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~InputIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};

	template<class Node>
	struct OutputIterator {
	public:
		using iterator_concept = Traits::output_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = OutputIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr OutputIterator() noexcept = default;
		constexpr OutputIterator(const OutputIterator&) noexcept = default;
		constexpr OutputIterator(OutputIterator&&) noexcept = default;
		constexpr OutputIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~OutputIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};

	template<class Node>
	struct ForwardIterator {
	public:
		using iterator_concept = Traits::forward_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = ForwardIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr ForwardIterator() noexcept = default;
		constexpr ForwardIterator(const ForwardIterator&) noexcept = default;
		constexpr ForwardIterator(ForwardIterator&&) noexcept = default;
		constexpr ForwardIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~ForwardIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public: /* forward_iteratorに必須 */
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return Get(ptr) == Get(rhs.ptr); }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};

	template<class Node>
	struct BidirectionalIterator {
	public:
		using iterator_concept = Traits::bidirectional_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = BidirectionalIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr BidirectionalIterator() noexcept = default;
		constexpr BidirectionalIterator(const BidirectionalIterator&) noexcept = default;
		constexpr BidirectionalIterator(BidirectionalIterator&&) noexcept = default;
		constexpr BidirectionalIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~BidirectionalIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public:/* bidirectional_iteratorに必須 */
		constexpr iterator_type& operator--() noexcept { Decrement(ptr); return *this; }
		constexpr iterator_type operator--(int) noexcept { auto ret = *this; Decrement(ptr); return ret; }
	public: /* forward_iteratorに必須 */
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return Get(ptr) == Get(rhs.ptr); }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};

	template<class Node>
	struct RandomAccessIterator {
	public:
		using iterator_concept = Traits::random_access_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = RandomAccessIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr RandomAccessIterator() noexcept = default;
		constexpr RandomAccessIterator(const RandomAccessIterator&) noexcept = default;
		constexpr RandomAccessIterator(RandomAccessIterator&&) noexcept = default;
		constexpr RandomAccessIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~RandomAccessIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public: /* random_access_iteratorに必須 */
		constexpr iterator_type& operator+=(ptrdiff_t n) noexcept { Increment(ptr, n); return *this; }
		constexpr iterator_type operator+(ptrdiff_t n) const noexcept { auto ret = *this; Increment(ret.ptr, n); return ret; }
		friend constexpr iterator_type operator+(ptrdiff_t n, const iterator_type& rhs) noexcept { return rhs + n; }
		constexpr iterator_type& operator-=(ptrdiff_t n) noexcept { Decrement(ptr, n); return *this; }
		constexpr iterator_type operator-(ptrdiff_t n) const noexcept { auto ret = *this; Decrement(ret.ptr, n); return ret; }
		constexpr value_type& operator[](ptrdiff_t n) const noexcept { return *(*this + n); }
		constexpr auto operator<=>(const iterator_type& rhs) const noexcept { return Get(ptr) <=> Get(rhs.ptr); }
	public:/* bidirectional_iteratorに必須 */
		constexpr iterator_type& operator--() noexcept { Decrement(ptr); return *this; }
		constexpr iterator_type operator--(int) noexcept { auto ret = *this; Decrement(ptr); return ret; }
	public: /* forward_iteratorに必須 */
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return Get(ptr) == Get(rhs.ptr); }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};

	template<class Node>
	struct ContiguousIterator {
	public:
		using iterator_concept = Traits::contiguous_iterator_tag;
		using node_type = Traits::remove_cvref_t<Node>;
		using value_type = Traits::conditional_t<CContainerNode<node_type>, Traits::dereference_t<node_type>, node_type>;
		using iterator_type = ContiguousIterator<Node>;
	protected:
		node_type* ptr = nullptr;
	public:
		constexpr ContiguousIterator() noexcept = default;
		constexpr ContiguousIterator(const ContiguousIterator&) noexcept = default;
		constexpr ContiguousIterator(ContiguousIterator&&) noexcept = default;
		constexpr ContiguousIterator(node_type* p) noexcept : ptr(p) {}
		constexpr ~ContiguousIterator() noexcept = default;
	public:
		constexpr node_type*& GetNode() noexcept { return ptr; }
		constexpr node_type* const& GetNode() const noexcept { return ptr; }
	public:/* contiguous_iteratorに必須 */
		constexpr value_type* operator->() const noexcept {
			if constexpr (Traits::is_same_v<node_type, value_type>) return ptr;
			else return &(*(*ptr));
		}
	public: /* random_access_iteratorに必須 */
		constexpr iterator_type& operator+=(ptrdiff_t n) noexcept { Increment(ptr, n); return *this; }
		constexpr iterator_type operator+(ptrdiff_t n) const noexcept { auto ret = *this; Increment(ret.ptr, n); return ret; }
		friend constexpr iterator_type operator+(ptrdiff_t n, const iterator_type& rhs) noexcept { return rhs + n; }
		constexpr iterator_type& operator-=(ptrdiff_t n) noexcept { Decrement(ptr, n); return *this; }
		constexpr iterator_type operator-(ptrdiff_t n) const noexcept { auto ret = *this; Decrement(ret.ptr, n); return ret; }
		constexpr value_type& operator[](ptrdiff_t n) const noexcept { return *(*this + n); }
		constexpr auto operator<=>(const iterator_type& rhs) const noexcept { return Get(ptr) <=> Get(rhs.ptr); }
	public:/* bidirectional_iteratorに必須 */
		constexpr iterator_type& operator--() noexcept { Decrement(ptr); return *this; }
		constexpr iterator_type operator--(int) noexcept { auto ret = *this; Decrement(ptr); return ret; }
	public: /* forward_iteratorに必須 */
		constexpr bool operator==(const iterator_type& rhs) const noexcept { return Get(ptr) == Get(rhs.ptr); }
	public: /* input/output_iteratorに必須 */
		constexpr iterator_type& operator++() noexcept { Increment(ptr); return *this; }
		constexpr iterator_type operator++(int) noexcept { auto ret = *this; Increment(ptr); return ret; }
		constexpr value_type& operator*() const noexcept { return *Get(ptr); }
		constexpr iterator_type& operator=(const iterator_type&) noexcept = default;
		constexpr ptrdiff_t operator-(const iterator_type& rhs) const noexcept { return Get(ptr) - Get(rhs.ptr); }
	};
}
