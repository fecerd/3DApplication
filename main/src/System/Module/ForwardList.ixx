module;
#include "FUNCSIG.hpp"
export module ForwardList;
import Traits;
import Exception;

namespace System {
	template<class T>
	struct ForwardListNode {
		T value;
		ForwardListNode<T>* next = nullptr;
	public:
		ForwardListNode() noexcept = default;
		ForwardListNode(const T& val, ForwardListNode<T>* n) noexcept : value(val), next(n) {}
		ForwardListNode(T&& val, ForwardListNode<T>* n) noexcept : value(static_cast<T&&>(val)), next(n) {}
	public:
		T& operator*() const noexcept { return value; }
	};
}

export namespace System {
	template<class T>
	class ForwardList;

	template<class T>
	class ForwardListIterator {
		using value_type = Traits::remove_cv_t<T>;
		friend class ForwardList<value_type>;
		ForwardListNode<value_type>* m_current = nullptr;
		ForwardListNode<value_type>* m_next = nullptr;
	public:
		ForwardListIterator() noexcept = default;
		ForwardListIterator(ForwardListNode<value_type>* current) noexcept
			: m_current(current), m_next(current ? current->next : nullptr) {}
		ForwardListIterator(ForwardListNode<value_type>* current, ForwardListNode<value_type>* next) noexcept
			: m_current(current), m_next(next) {}
		~ForwardListIterator() noexcept = default;
	public:
		T& operator*() noexcept {
			return m_current->value;
		}
		ForwardListIterator<T>& operator++() noexcept {
			m_current = m_next;
			m_next = m_current ? m_current->next : nullptr;
			return *this;
		}
		ForwardListIterator<T> operator++(int) noexcept {
			ForwardListIterator<T> ret = *this;
			this->operator++();
			return ret;
		}
		constexpr bool operator==(const ForwardListIterator<value_type>& rhs) const noexcept {
			return m_current == rhs.m_current;
		}
		constexpr bool operator==(const ForwardListIterator<const value_type>& rhs) const noexcept {
			return m_current == rhs.m_current;
		}
		constexpr operator bool() const noexcept {
			return m_current;
		}
	};

	template<class T>
	class ForwardList {
		ForwardListNode<T>* m_first = nullptr;
	public:
		ForwardList() noexcept = default;
		~ForwardList() noexcept { Clear(); }
	public:
		ForwardListIterator<T> begin() noexcept {
			return ForwardListIterator<T>(m_first);
		}
		ForwardListIterator<const T> cbegin() const noexcept {
			return ForwardListIterator<const T>(m_first);
		}
		ForwardListIterator<T> end() const noexcept {
			return ForwardListIterator<T>(nullptr);
		}
		ForwardListIterator<const T> cend() const noexcept {
			return ForwardListIterator<T>(nullptr);
		}
	public:
		void Push(const T& value) noexcept {
			m_first = new ForwardListNode<T>(value, m_first);
		}
		void Push(T&& value) noexcept {
			m_first = new ForwardListNode<T>(static_cast<T&&>(value), m_first);
		}
		void Pop() noexcept {
			if (!m_first) return;
			ForwardListNode<T>* tmp = m_first;
			m_first = m_first->next;
			delete tmp;
		}
		bool Empty() const noexcept {
			return !m_first;
		}
		T& Front() {
			if (!m_first) throw System::InvalidOperationException(__FUNCSIG__, u"要素が存在しません。", __FILE__, __LINE__);
			return m_first->value;
		}
		const T& Front() const {
			if (!m_first) throw System::InvalidOperationException(__FUNCSIG__, u"要素が存在しません。", __FILE__, __LINE__);
			return m_first->value;
		}
		void Clear() noexcept {
			ForwardListNode<T>* node = m_first;
			while (node) {
				ForwardListNode<T>* next = node->next;
				delete node;
				node = next;
			}
			m_first = nullptr;
		}
		void Remove(const ForwardListIterator<T>& ite) noexcept {
			if (!ite) return;
			ForwardListIterator<T> prev{ nullptr, m_first };
			for (auto b = begin(), e = end(); b != e; ++b, ++prev) {
				if (b == ite) {
					if (prev) prev.m_current->next = b.m_next;
					else m_first = b.m_next;
					delete b.m_current;
					break;
				}
			}
		}
	};
}
