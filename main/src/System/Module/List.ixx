module;
#include "FUNCSIG.hpp"
export module List;
import CSTDINT;
import SmartPtr;
import Traits;
import Sorts;
import Exception;
import IEnumerable;

//ListNode
namespace System::Internal {
	template<class T>
	struct ListNode {
		System::UniquePtr<T> value = nullptr;
		ListNode<T>* prev = nullptr;
		ListNode<T>* next = nullptr;
	public:
		ListNode() noexcept = default;
		ListNode(const ListNode<T>&) noexcept = delete;
		ListNode(ListNode<T>&& arg) noexcept
			: value(System::move(arg.value)), prev(arg.prev), next(arg.next)
		{
			arg.prev = nullptr;
			arg.next = nullptr;
		}
		ListNode(T const& obj) noexcept : value(System::Unique<T>(obj)) {}
		ListNode(T&& obj) noexcept : value(System::Unique<T>(System::move(obj))) {}
		~ListNode() noexcept = default;
	};
}
using namespace System::Internal;

//ListIterator, List
export namespace System {
	template<class T> class List;

	template<class T>
	class ListIterator {
	public:
		friend class List<T>;
		using value_type = T;
		using iterator_concept = System::Traits::bidirectional_iterator_tag;
	private:
		ListNode<T>* m_node = nullptr;
		ListNode<T>* m_prev = nullptr;
		ListNode<T>* m_next = nullptr;
		const List<T>* m_list = nullptr;
	public:
		constexpr ListIterator(const List<T>* list = nullptr, ListNode<T>* node = nullptr) noexcept
			: m_list(list), m_node(node), m_prev(node ? node->prev : nullptr), m_next(node ? node->next : nullptr) {}
		constexpr ListIterator(const ListIterator<T>&) noexcept = default;
		constexpr ListIterator(ListIterator<T>&&) noexcept = default;
		constexpr ~ListIterator() noexcept = default;
	public:
		constexpr ListIterator<T>& operator++() noexcept {
			m_node = m_next;
			if (m_node) {
				m_prev = m_node->prev;
				m_next = m_node->next;
			}
			return *this;
		}
		constexpr ListIterator<T> operator++(int) noexcept {
			ListIterator<T> ret = *this;
			++(*this);
			return ret;
		}
		constexpr ListIterator<T>& operator--() noexcept {
			m_node = m_prev;
			if (m_node) {
				m_prev = m_node->prev;
				m_next = m_node->next;
			}
			return *this;
		}
		constexpr ListIterator<T> operator--(int) noexcept {
			ListIterator<T> ret = *this;
			--(*this);
			return ret;
		}
		constexpr ListIterator<T>& operator=(const ListIterator<T>& rhs) noexcept {
			if (this == &rhs) return *this;
			m_node = rhs.m_node;
			m_prev = rhs.m_prev;
			m_next = rhs.m_next;
			m_list = rhs.m_list;
			return *this;
		}
		constexpr T& operator*() const noexcept {
			return *m_node->value;
		}
		constexpr ptrdiff_t operator-(const ListIterator<T>& rhs) const noexcept {
			if (m_list != rhs.m_list) return System::MIN_VALUE<ptrdiff_t>;
			ptrdiff_t ret = 0;
			ListIterator<T> ite = *this;
			while (ite.m_node) {
				if (ite == rhs) return ret;
				++ite;
				--ret;
			}
			ret = 0;
			ite = *this;
			while (ite.m_node) {
				if (ite == rhs) return ret;
				--ite;
				++ret;
			}
			return System::MIN_VALUE<ptrdiff_t>;
		}
		constexpr operator bool() const noexcept;
		constexpr bool operator==(const ListIterator<T>& rhs) const noexcept {
			return m_node == rhs.m_node || (m_prev == rhs.m_prev && m_next == rhs.m_next);
		}
	};

	template<class T>
	class List : public ICollection<T> {
		ListNode<T>* m_first = nullptr;
		ListNode<T>* m_last = nullptr;
	public:
		List() noexcept {
			m_last = new ListNode<T>();
			m_first = m_last;
		}
		List(const List<T>& arg) noexcept requires(Traits::Concepts::CCopyConstructible<T>) {
			m_last = new ListNode<T>();
			m_first = m_last;
			if (!arg.Any()) return;
			ListNode<T>* argNode = arg.m_last->prev;
			do {
				ListNode<T>* tmp = argNode->value ? new ListNode<T>(*argNode->value) : new ListNode<T>();
				PushFrontNode(tmp);
				argNode = argNode->prev;
			} while (argNode);
		}
		List(List<T>&& arg) noexcept : m_first(arg.m_first), m_last(arg.m_last) {
			arg.m_first = nullptr;
			arg.m_last = nullptr;
		}
		~List() noexcept {
			Clear();
			delete m_last;
			m_last = nullptr;
			m_first = nullptr;
		}
	public:
		bool Any() const noexcept { return m_first != m_last; }
		T& Front() const {
			if (m_first != m_last) return *m_first->value;
			throw LogicException(
				__FUNCSIG__,
				u"空のリストの要素を取得しようとしました。",
				__FILE__, __LINE__
			);
		}
		T& Back() const {
			if (m_first != m_last) return *m_last->prev->value;
			throw LogicException(
				__FUNCSIG__,
				u"空のリストの要素を取得しようとしました。",
				__FILE__, __LINE__
			);
		}
		T* TryFront() const noexcept {
			if (m_first != m_last) return m_first->value.get();
			else return nullptr;
		}
		T* TryBack() const noexcept {
			if (m_first != m_last) return m_last->prev->value.get();
			else return nullptr;
		}
		template<System::Traits::Concepts::CConditionFunction<T> Cond>
		T* TryFirst(Cond const& cond) const noexcept {
			for (auto b = begin(), e = end(); b != e; ++b) {
				if (cond(*b)) return b.m_node->value.get();
			}
			return nullptr;
		}
		void Clear() noexcept {
			ListNode<T>* node = m_first;
			while (node != m_last) {
				ListNode<T>* tmp = node;
				node = node->next;
				delete tmp;
			}
			m_first = m_last;
		}
	public:
		ListIterator<T> begin() const noexcept {
			return ListIterator<T>(this, m_first);
		}
		ListIterator<T> end() const noexcept {
			return ListIterator<T>(this, m_last);
		}
	private:
		void PushBackNode(ListNode<T>* node) noexcept {
			node->next = m_last;
			if (m_first == m_last) {
				m_first = node;
			}
			else {
				ListNode<T>* prev = m_last->prev;
				node->prev = prev;
				prev->next = node;
			}
			m_last->prev = node;
		}
		void PushFrontNode(ListNode<T>* node) noexcept {
			ListNode<T>* next = m_first;
			node->next = next;
			next->prev = node;
			m_first = node;
		}
		void InsertNode(const ListIterator<T>& ite, ListNode<T>* node) noexcept {
			if (ite == begin()) {
				PushFrontNode(node);
			}
			else {
				ListNode<T>* prev = ite.m_prev;
				//ite.m_nodeはRemove()で無効になっている可能性がある。
				//基本的にRemove()後のイテレータは使用しないが、事故防止のためにprevから取得しておく。
				ListNode<T>* next = prev->next;
				prev->next = node;
				node->prev = prev;
				node->next = next;
				next->prev = node;
			}
		}
	public:
		void PushBack(T const& obj) noexcept {
			ListNode<T>* current = new ListNode<T>(obj);
			PushBackNode(current);
		}
		void PushBack(T&& obj) noexcept {
			ListNode<T>* current = new ListNode<T>(System::move(obj));
			PushBackNode(current);
		}
		void PushFront(T const& obj) noexcept {
			ListNode<T>* current = new ListNode<T>(obj);
			PushFrontNode(current);
		}
		void PushFront(T&& obj) noexcept {
			ListNode<T>* current = new ListNode<T>(System::move(obj));
			PushFrontNode(current);
		}
		ListIterator<T> Insert(const ListIterator<T>& ite, T const& obj) noexcept {
			if (ite.m_list != this) return end();
			ListNode<T>* current = new ListNode<T>(obj);
			InsertNode(ite, current);
			return ListIterator<T>(this, current);
		}
		ListIterator<T> Insert(const ListIterator<T>& ite, T&& obj) noexcept {
			if (ite.m_list != this) return end();
			ListNode<T>* current = new ListNode<T>(System::move(obj));
			InsertNode(ite, current);
			return ListIterator<T>(this, current);
		}
		UniquePtr<T> PopFront() noexcept {
			if (m_first == m_last) return UniquePtr<T>();
			ListNode<T>* node = m_first;
			m_first = m_first->next;
			UniquePtr<T> ret = System::move(node->value);
			delete node;
			return ret;
		}
		UniquePtr<T> PopBack() noexcept {
			if (m_first == m_last) return UniquePtr<T>();
			ListNode<T>* node = m_last->prev;
			ListNode<T>* prev = node->prev;
			m_last->prev = prev;
			if (prev) prev->next = m_last;
			else {
				m_first = m_last;
			}
			UniquePtr<T> ret = System::move(node->value);
			delete node;
			return ret;
		}
		ListIterator<T> FindFront(T const& value) const noexcept {
			ListIterator<T> ret = begin();
			for (ListIterator<T> e = end(); ret != e; ++ret) {
				if (*ret == value) return ret;
			}
			return ret;
		}
		ListIterator<T> FindBack(T const& value) const noexcept {
			ListIterator<T> ret = end();
			if (m_first == m_last) return ret;
			const ListIterator<T> b = begin();
			while (--ret != b) {
				if (*ret == value) return ret;
			}
			if (*ret == value) return ret;
			return end();
		}
		bool Contains(T const& value) const noexcept {
			return FindFront(value) != end();
		}
		void Remove(const ListIterator<T>& ite) noexcept {
			if (ite.m_list != this || ite.m_node == m_last) return;
			ListNode<T>* current = ite.m_node;
			delete current;
			ListNode<T>* prev = ite.m_prev;
			ListNode<T>* next = ite.m_next;
			if (!prev) {
				m_first = next;
				m_first->prev = nullptr;
			}
			else {
				prev->next = next;
				if (next) next->prev = prev;
			}
		}
		template<System::Traits::Concepts::CCompareFunction<Traits::remove_cvref_t<T>> CompareFunc>
		void Sort(bool ascendingOrder, CompareFunc const& compare) noexcept {
			System::Sorts::QuickSort(begin(), end(), ascendingOrder, compare);
		}
	public:
		List<T>& operator=(const List<T>& rhs) noexcept {
			if (this == &rhs) return *this;
			Clear();
			if (!rhs.Any()) return *this;
			ListNode<T>* rhsNode = rhs.m_last->prev;
			do {
				ListNode<T>* tmp = rhsNode->value ? new ListNode<T>(*rhsNode->value) : new ListNode<T>();
				PushFrontNode(tmp);
				rhsNode = rhsNode->prev;
			} while (rhsNode);
			return *this;
		}
		List<T>& operator=(List<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			Clear();
			ListNode<T>* empty = m_last;
			m_first = rhs.m_first;
			rhs.m_first = empty;
			m_last = rhs.m_last;
			rhs.m_last = empty;
			return *this;
		}
	public:/* ICollection override */
		IEnumerator<T> GetEnumerator() noexcept override {
			for (T& x : *this)
				co_yield x;

			// if constexpr (Traits::Concepts::CCopyConstructible<T>){
			// 	for (T &x : *this) co_yield x;
			// }
			// else {
			// 	return IEnumerator<T>(System::coroutine_handle<System::promise_type<T, IEnumerator<T>>>{});
			// }
		}
		IEnumerator<T const> GetEnumerator() const noexcept override {
			for (T& x : *this)
				co_yield x;

			// if constexpr (Traits::Concepts::CCopyConstructible<T>) {
			// 	for (T &x : *this) co_yield x;
			// }
			// else{
			// 	return IEnumerator<T const>(System::coroutine_handle<System::promise_type<T const, IEnumerator<T const>>>{});
			// }
		}
		IEnumerator<T> GetReverseEnumerator() noexcept override {
			if (m_first == m_last) co_return;
			for (ListIterator<T> ite = --end(), b = begin(); ite != b; --ite) {
				T& x = *ite;
				co_yield x;
			}
			co_yield *m_first->value;

			// if constexpr (Traits::Concepts::CCopyConstructible<T>) {
			// 	if (m_first == m_last) co_return;
			// 	for (ListIterator<T> ite = --end(), b = begin(); ite != b; --ite) {
			// 		T &x = *ite;
			// 		co_yield x;
			// 	}
			// 	co_yield *m_first->value;
			// }
			// else {
			// 	return IEnumerator<T>(System::coroutine_handle<System::promise_type<T, IEnumerator<T>>>{});
			// }
		}
		IEnumerator<T const> GetReverseEnumerator() const noexcept override {
			if (m_first == m_last) co_return;
			for (ListIterator<T> ite = --end(), b = begin(); ite != b; --ite) {
				T& x = *ite;
				co_yield x;
			}
			co_yield *m_first->value;

			// if constexpr (Traits::Concepts::CCopyConstructible<T>) {
			// 	if (m_first == m_last) co_return;
			// 	for (ListIterator<T> ite = --end(), b = begin(); ite != b; --ite) {
			// 		T &x = *ite;
			// 		co_yield x;
			// 	}
			// 	co_yield *m_first->value;
			// }
			// else {
			// 	return IEnumerator<T const>(System::coroutine_handle<System::promise_type<T const, IEnumerator<T const>>>{});
			// }
		}
	};
}

//Implementation
namespace System {
	template<class T>
	constexpr ListIterator<T>::operator bool() const noexcept { return m_list && m_node && m_next; }
}

//import CSTDINT;
//import Traits;
//import IEnumerable;
//import Iterators;
//import SmartPtr;
//import Thread;
//import Exception;
//import Sorts;
////BidirectionalNode
//export namespace System {
//	template<class T>
//	struct BidirectionalNode : IContainerNode {
//		//番兵の場合、nullptr
//		UniquePtr<T> m_value = nullptr;
//		//先頭の場合、nullptr
//		BidirectionalNode<T>* m_prev = nullptr;
//		//番兵の場合、nullptr
//		BidirectionalNode<T>* m_next = nullptr;
//	public:
//		BidirectionalNode() noexcept = default;
//	public:
//		T* operator->() const noexcept { return m_value.get(); }
//		T& operator*() const noexcept { return *m_value; }
//		BidirectionalNode<T>* operator+(size_t rhs) const noexcept {
//			BidirectionalNode<T>* ret = const_cast<BidirectionalNode<T>*>(this);
//			for (size_t i = 0; i < rhs; ++i) {
//				if (!ret->m_value) break;
//				ret = ret->m_next;
//			}
//			return ret;
//		}
//		BidirectionalNode<T>* operator-(size_t rhs) const noexcept {
//			BidirectionalNode<T>* ret = const_cast<BidirectionalNode<T>*>(this);
//			for (size_t i = 0; i < rhs; ++i) {
//				if (!ret->m_prev) break;
//				ret = ret->m_prev;
//			}
//			return ret;
//		}
//		constexpr bool operator==(const BidirectionalNode<T>& rhs) const noexcept {
//			return m_value == rhs.m_value;
//		}
//		ptrdiff_t operator-(const BidirectionalNode<T>& rhs) const noexcept {
//			const BidirectionalNode<T>* cur = this;
//			ptrdiff_t ret = 0;
//			while (cur) {
//				if (*cur == rhs) return ret;
//				cur = cur->m_prev;
//				++ret;
//			}
//			ret = 0;
//			cur = this;
//			while (cur) {
//				if (*cur == rhs) return ret;
//				cur = cur->m_next;
//				--ret;
//			}
//			return MIN_VALUE<ptrdiff_t>;
//		}
//		explicit constexpr operator bool() const noexcept { return !m_value; }
//	};
//}
//
////List
//export namespace System {
//	template<class T>
//	class List : public ICollection<T> {
//		BidirectionalNode<T>* m_first = nullptr;
//		BidirectionalNode<T>* m_last = nullptr;
//		mutable RecursiveMutex m_mtx;
//	public:
//		List() noexcept {
//			m_first = new BidirectionalNode<T>();
//			m_last = m_first;
//		}
//		~List() noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* node = m_first;
//			while (node) {
//				BidirectionalNode<T>* next = node->m_next;
//				delete node;
//				node = next;
//			}
//			m_first = nullptr;
//			m_last = nullptr;
//		}
//	private:
//		void Insert_Internal(const T& arg, BidirectionalNode<T>* prev, BidirectionalNode<T>* next) noexcept {
//			BidirectionalNode<T>* tmp = new BidirectionalNode<T>();
//			tmp->m_value = Unique<T>(arg);
//			if (prev) prev->m_next = tmp;
//			else m_first = tmp;
//			tmp->m_prev = prev;
//			tmp->m_next = next;
//			if (next) next->m_prev = tmp;
//		}
//		void Insert_Internal(T&& arg, BidirectionalNode<T>* prev, BidirectionalNode<T>* next) noexcept {
//			BidirectionalNode<T>* tmp = new BidirectionalNode<T>();
//			tmp->m_value = Unique<T>(static_cast<T&&>(arg));
//			if (prev) prev->m_next = tmp;
//			else m_first = tmp;
//			tmp->m_prev = prev;
//			tmp->m_next = next;
//			if (next) next->m_prev = tmp;
//		}
//		void Remove_Internal(BidirectionalNode<T>* current) noexcept {
//			if (!Any() || !current) return;
//			BidirectionalNode<T>* prev = current->m_prev;
//			BidirectionalNode<T>* next = current->m_next;
//			if (prev) prev->m_next = next;
//			else m_first = next;
//			next->m_prev = prev;
//			delete current;
//		}
//	public:
//		void PushFront(const T& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			return Insert_Internal(arg, nullptr, m_first);
//		}
//		void PushFront(T&& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			return Insert_Internal(static_cast<T&&>(arg), nullptr, m_first);
//		}
//		void PushBack(const T& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			return Insert_Internal(arg, m_last->m_prev, m_last);
//		}
//		void PushBack(T&& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			return Insert_Internal(static_cast<T&&>(arg), m_last->m_prev, m_last);
//		}
//		void Insert(const T& arg, const BidirectionalIterator<BidirectionalNode<T>>& ite) noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* const& node = ite.GetNode();
//			return Insert_Internal(arg, node->m_prev, node);
//		}
//		void Insert(T&& arg, const BidirectionalIterator<BidirectionalNode<T>>& ite) noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* const& node = ite.GetNode();
//			return Insert_Internal(static_cast<T&&>(arg), node->m_prev, node);
//		}
//		UniquePtr<T> PopFront() noexcept {
//			LockGuard lock{ m_mtx };
//			if (!Any()) return UniquePtr<T>();
//			UniquePtr<T> ret = static_cast<UniquePtr<T>&&>(m_first->m_value);
//			Remove_Internal(m_first);
//			return static_cast<UniquePtr<T>&&>(ret);
//		}
//		UniquePtr<T> PopBack() noexcept {
//			LockGuard lock{ m_mtx };
//			if (!Any()) return UniquePtr<T>();
//			UniquePtr<T> ret = static_cast<UniquePtr<T>&&>(m_last->m_prev->m_value);
//			Remove_Internal(m_last->m_prev);
//			return static_cast<UniquePtr<T>&&>(ret);
//		}
//		void Remove(const T& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			for (BidirectionalNode<T>* node = m_first; node != m_last; node = node->m_next) {
//				if (*node->m_value == arg) {
//					Remove_Internal(node);
//					break;
//				}
//			}
//		}
//		void Remove(const BidirectionalIterator<BidirectionalNode<T>>& ite) noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* const& node = ite.GetNode();
//			if (!node->m_next) return;
//			return Remove_Internal(node);
//		}
//		UniquePtr<T> Release(const BidirectionalIterator<BidirectionalNode<T>>& ite) noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* const& node = ite.GetNode();
//			if (!node->m_next) return UniquePtr<T>();
//			UniquePtr<T> ret = static_cast<UniquePtr<T>&&>(node->m_value);
//			Remove_Internal(node);
//			return static_cast<UniquePtr<T>&&>(ret);
//		}
//		void Clear() noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* node = m_first;
//			while (node != m_last) {
//				BidirectionalNode<T>* cur = node;
//				node = node->m_next;
//				delete cur;
//			}
//			m_last->m_prev = nullptr;
//			m_first = m_last;
//		}
//		void DeleteAll() noexcept requires(Traits::is_pointer_v<T>) {
//			LockGuard lock{ m_mtx };
//			BidirectionalNode<T>* node = m_first;
//			while (node != m_last) {
//				BidirectionalNode<T>* cur = node;
//				node = node->m_next;
//				T& p = *(*cur);
//				delete p;
//				delete cur;
//			}
//			m_last->m_prev = nullptr;
//			m_first = m_last;
//		}
//	public:
//		constexpr bool Any() const noexcept {
//			LockGuard lock{ m_mtx };
//			return m_first != m_last;
//		}
//		template<class Functor>
//		bool Any(Functor&& condition) noexcept {
//			LockGuard lock{ m_mtx };
//			for (BidirectionalNode* node = m_first; node != m_last; node = node->m_next) {
//				if (condition(*(*node))) return true;
//			}
//			return false;
//		}
//		T& Front() {
//			LockGuard lock{ m_mtx };
//			if (Any()) return *(*m_first);
//			throw InvalidOperationException(__FUNCSIG__, u"空のリストの要素を取得しようとしました。", __FILE__, __LINE__);
//		}
//		const T& Front() const {
//			LockGuard lock{ m_mtx };
//			if (Any()) return *(*m_first);
//			throw InvalidOperationException(__FUNCSIG__, u"空のリストの要素を取得しようとしました。", __FILE__, __LINE__);
//		}
//		T& Back() {
//			LockGuard lock{ m_mtx };
//			if (m_last->m_prev) return *(*m_last->m_prev);
//			throw InvalidOperationException(__FUNCSIG__, u"空のリストの要素を取得しようとしました。", __FILE__, __LINE__);
//		}
//		const T& Back() const {
//			LockGuard lock{ m_mtx };
//			if (m_last->m_prev) return *(*m_last->m_prev);
//			throw InvalidOperationException(__FUNCSIG__, u"空のリストの要素を取得しようとしました。", __FILE__, __LINE__);
//		}
//		T* TryFront() noexcept {
//			LockGuard lock{ m_mtx };
//			if (Any()) return m_first->m_value.get();
//			else return nullptr;
//		}
//		const T* TryFront() const noexcept {
//			LockGuard lock{ m_mtx };
//			if (Any()) return m_first->m_value.get();
//			else return nullptr;
//		}
//		const T* TryBack() const noexcept {
//			LockGuard lock{ m_mtx };
//			if (Any()) return m_last->m_prev->m_value.get();
//			else return nullptr;
//		}
//		template<class Functor>
//		T* TryFirst(Functor&& condition) const noexcept {
//			LockGuard lock{ m_mtx };
//			for (BidirectionalNode<T>* node = m_first; node != m_last; node = node->m_next) {
//				if (condition(*(*node))) return node->m_value.get();
//			}
//			return nullptr;
//		}
//	public:
//		BidirectionalIterator<BidirectionalNode<T>> begin() noexcept {
//			LockGuard lock{ m_mtx };
//			return BidirectionalIterator<BidirectionalNode<T>>(m_first);
//		}
//		BidirectionalIterator<BidirectionalNode<T>> end() noexcept {
//			return BidirectionalIterator<BidirectionalNode<T>>(m_last);
//		}
//		BidirectionalIterator<const BidirectionalNode<T>> begin() const noexcept {
//			LockGuard lock{ m_mtx };
//			return BidirectionalIterator<const BidirectionalNode<T>>(m_first);
//		}
//		BidirectionalIterator<const BidirectionalNode<T>> end() const noexcept {
//			return BidirectionalIterator<const BidirectionalNode<T>>(m_last);
//		}
//	public:
//		BidirectionalIterator<BidirectionalNode<T>> IndexOf(size_t index) noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalIterator<BidirectionalNode<T>> ret = begin();
//			for (size_t i = 0; i < index; ++i) ++ret;
//			return ret;
//		}
//		BidirectionalIterator<const BidirectionalNode<T>> IndexOf(size_t index) const noexcept {
//			LockGuard lock{ m_mtx };
//			BidirectionalIterator<const BidirectionalNode<T>> ret = begin();
//			for (size_t i = 0; i < index; ++i) ++ret;
//			return ret;
//		}
//		size_t Count() const noexcept {
//			LockGuard lock{ m_mtx };
//			size_t ret = 0;
//			for (auto b = begin(), e = end(); b != e; ++b) ++ret;
//			return ret;
//		}
//		bool Contains(const T& arg) noexcept {
//			LockGuard lock{ m_mtx };
//			for (BidirectionalNode<T>* node = m_first; node != m_last; node = node->m_next) {
//				if (*node->m_value == arg) return true;
//			}
//			return false;
//		}
//		template<CCompareFunction<BidirectionalIterator<BidirectionalNode<T>>> CompareFunc>
//		void Sort(bool ascendingOrder, const CompareFunc& compare) noexcept {
//			System::Sorts::QuickSort(begin(), end(), ascendingOrder, compare);
//		}
//	public:
//		RecursiveMutex& GetMutex() const noexcept { return m_mtx; }
//	public:
//		IEnumerator<T> GetEnumerator() noexcept override {
//			for (T& x : *this) co_yield x;
//		}
//		IEnumerator<T const> GetEnumerator() const noexcept override {
//			for (T& x : *this) co_yield x;
//		}
//		IEnumerator<T> GetReverseEnumerator() noexcept override {
//			auto b = begin();
//			for (auto ite = --end(); ite != b; --ite) co_yield *ite;
//			if (b != end()) co_yield *b;
//		}
//		IEnumerator<T const> GetReverseEnumerator() const noexcept override {
//			auto b = begin();
//			for (auto ite = --end(); ite != b; --ite) co_yield *ite;
//			if (b != end()) co_yield *b;
//		}
//	};
//}
