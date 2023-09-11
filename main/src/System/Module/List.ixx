module;
#include "FUNCSIG.hpp"
#include "EnableVirtual.hpp"
export module List;
import CSTDINT;
import Traits;
import SmartPtrs;
import Sorts;
import Exception;
export import ICollection;

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
		ListNode(T const& obj) noexcept : value(System::MakeUnique<T>(obj)) {}
		ListNode(T&& obj) noexcept : value(System::MakeUnique<T>(System::move(obj))) {}
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
#if defined(__GNUC__) && !defined(__clang__)
		IEnumerator<T> GetEnumerator(bool reverse) noexcept override {
			auto internal = [this](Boost::push_type<T&>& sink) {
				for (T& x : *this) sink(x);
			};
			auto internal_r = [this](Boost::push_type<T&>& sink) {
				if (m_first == m_last) return;
				for (auto ite = --end(), b = begin(); ite != b; --ite) {
					T& x = *ite;
					sink(x);
				}
				sink(*m_first->value);
			};
			return IEnumerator<T>(
				[internal, internal_r](bool r) mutable {
					return IEnumerator<T>(r
						? Function<void(Boost::push_type<T&>&)>(internal_r)
						: Function<void(Boost::push_type<T&>&)>(internal)
					);
				},
				reverse
			);
		}
		IEnumerator<T const> GetEnumerator(bool reverse) const noexcept override {
			auto internal = [this](Boost::push_type<T const&>& sink) {
				for (T const& x : *this) sink(x);
			};
			auto internal_r = [this](Boost::push_type<T const&>& sink) {
				if (m_first == m_last) return;
				for (auto ite = --end(), b = begin(); ite != b; --ite) {
					T const& x = *ite;
					sink(x);
				}
				sink(*m_first->value);
			};
			return IEnumerator<T const>(
				[internal, internal_r](bool r) mutable {
					return IEnumerator<T const>(r
						? Function<void(Boost::push_type<T const&>&)>(internal_r)
						: Function<void(Boost::push_type<T const&>&)>(internal)
					);
				},
				reverse
			);
		}
#else
	private:
		IEnumerator<T> Internal(bool reverse) noexcept {
			if (reverse) {
				if (m_first == m_last) co_return;
				for (ListIterator<T> ite = --end(), b = begin(); ite != b; --ite) {
					T& x = *ite;
					co_yield x;
				}
				co_yield *m_first->value;
			} else {
				for (T& x : *this) co_yield x;
			}
		}
		IEnumerator<T const> Internal(bool reverse) const noexcept {
			if (reverse) {
				if (m_first == m_last) co_return;
				for (auto ite = --end(), b = begin(); ite != b; --ite) {
					T const& x = *ite;
					co_yield x;
				}
				co_yield *m_first->value;
			} else {
				for (T const& x : *this) co_yield x;
			}
		}
	public:
		IEnumerator<T> GetEnumerator(bool reverse) noexcept override {
			return IEnumerator<T>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
		IEnumerator<T const> GetEnumerator(bool reverse) const noexcept override {
			return IEnumerator<T const>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
#endif
	};
}

//Implementation
namespace System {
	template<class T>
	constexpr ListIterator<T>::operator bool() const noexcept { return m_list && m_node && m_next; }
}
