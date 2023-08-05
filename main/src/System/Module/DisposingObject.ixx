module;
#include "FUNCSIG.hpp"
#include <functional>
#include <memory>
#include <mutex>
export module DisposingObject;
import CSTDINT;
import Traits;
import TaskWorker;
import Exception;
//import Function;
//import List;
//import Thread;

//import <functional>;
//import <memory>;
//import <mutex>;

using namespace System;

namespace System{
	template <class T>
	struct ListNode
	{
		std::unique_ptr<T> value = nullptr;
		ListNode<T> *prev = nullptr;
		ListNode<T> *next = nullptr;
	public:
		ListNode() noexcept = default;
		ListNode(const ListNode<T> &) noexcept = delete;
		ListNode(ListNode<T> &&arg) noexcept
			: value(std::move(arg.value)), prev(arg.prev), next(arg.next)
		{
			arg.prev = nullptr;
			arg.next = nullptr;
		}
		ListNode(T const &obj) noexcept : value(std::make_unique<T>(obj)) {}
		ListNode(T &&obj) noexcept : value(std::make_unique<T>(std::move(obj))) {}
		~ListNode() noexcept = default;
	};

	template<class T> class List;
	template <class T>
	class ListIterator
	{
	public:
		friend class List<T>;
		using value_type = T;
		using iterator_concept = System::Traits::bidirectional_iterator_tag;

	private:
		ListNode<T> *m_node = nullptr;
		ListNode<T> *m_prev = nullptr;
		ListNode<T> *m_next = nullptr;
		const List<T> *m_list = nullptr;

	public:
		constexpr ListIterator(const List<T> *list = nullptr, ListNode<T> *node = nullptr) noexcept
			: m_list(list), m_node(node), m_prev(node ? node->prev : nullptr), m_next(node ? node->next : nullptr) {}
		constexpr ListIterator(const ListIterator<T> &) noexcept = default;
		constexpr ListIterator(ListIterator<T> &&) noexcept = default;
		constexpr ~ListIterator() noexcept = default;

	public:
		constexpr ListIterator<T> &operator++() noexcept
		{
			m_node = m_next;
			if (m_node)
			{
				m_prev = m_node->prev;
				m_next = m_node->next;
			}
			return *this;
		}
		constexpr ListIterator<T> operator++(int) noexcept
		{
			ListIterator<T> ret = *this;
			++(*this);
			return ret;
		}
		constexpr ListIterator<T> &operator--() noexcept
		{
			m_node = m_prev;
			if (m_node)
			{
				m_prev = m_node->prev;
				m_next = m_node->next;
			}
			return *this;
		}
		constexpr ListIterator<T> operator--(int) noexcept
		{
			ListIterator<T> ret = *this;
			--(*this);
			return ret;
		}
		constexpr ListIterator<T> &operator=(const ListIterator<T> &rhs) noexcept
		{
			if (this == &rhs)
				return *this;
			m_node = rhs.m_node;
			m_prev = rhs.m_prev;
			m_next = rhs.m_next;
			m_list = rhs.m_list;
			return *this;
		}
		constexpr T &operator*() const noexcept
		{
			return *m_node->value;
		}
		constexpr ptrdiff_t operator-(const ListIterator<T> &rhs) const noexcept
		{
			if (m_list != rhs.m_list)
				return System::MIN_VALUE<ptrdiff_t>;
			ptrdiff_t ret = 0;
			ListIterator<T> ite = *this;
			while (ite.m_node)
			{
				if (ite == rhs)
					return ret;
				++ite;
				--ret;
			}
			ret = 0;
			ite = *this;
			while (ite.m_node)
			{
				if (ite == rhs)
					return ret;
				--ite;
				++ret;
			}
			return System::MIN_VALUE<ptrdiff_t>;
		}
		constexpr operator bool() const noexcept;
		constexpr bool operator==(const ListIterator<T> &rhs) const noexcept
		{
			return m_node == rhs.m_node || (m_prev == rhs.m_prev && m_next == rhs.m_next);
		}
	};

	template <class T>
	class List
	{
		ListNode<T> *m_first = nullptr;
		ListNode<T> *m_last = nullptr;
	public:
		List() noexcept
		{
			m_last = new ListNode<T>();
			m_first = m_last;
		}
		List(const List<T> &arg) noexcept
			requires(Traits::Concepts::CCopyConstructible<T>)
		{
			m_last = new ListNode<T>();
			m_first = m_last;
			if (!arg.Any())
				return;
			ListNode<T> *argNode = arg.m_last->prev;
			do
			{
				ListNode<T> *tmp = argNode->value ? new ListNode<T>(*argNode->value) : new ListNode<T>();
				PushFrontNode(tmp);
				argNode = argNode->prev;
			} while (argNode);
		}
		List(List<T> &&arg) noexcept : m_first(arg.m_first), m_last(arg.m_last)
		{
			arg.m_first = nullptr;
			arg.m_last = nullptr;
		}
		~List() noexcept
		{
			Clear();
			delete m_last;
			m_last = nullptr;
			m_first = nullptr;
		}
	public:
		bool Any() const noexcept { return m_first != m_last; }
		T &Front() const
		{
			if (m_first != m_last)
				return *m_first->value;
			throw LogicException(
				__FUNCSIG__,
				u"空のリストの要素を取得しようとしました。",
				__FILE__, __LINE__);
		}
		T &Back() const
		{
			if (m_first != m_last)
				return *m_last->prev->value;
			throw LogicException(
				__FUNCSIG__,
				u"空のリストの要素を取得しようとしました。",
				__FILE__, __LINE__);
		}
		T *TryFront() const noexcept
		{
			if (m_first != m_last)
				return m_first->value.get();
			else
				return nullptr;
		}
		T *TryBack() const noexcept
		{
			if (m_first != m_last)
				return m_last->prev->value.get();
			else
				return nullptr;
		}
		template <System::Traits::Concepts::CConditionFunction<T> Cond>
		T *TryFirst(Cond const &cond) const noexcept
		{
			for (auto b = begin(), e = end(); b != e; ++b)
			{
				if (cond(*b))
					return b.m_node->value.get();
			}
			return nullptr;
		}
		void Clear() noexcept
		{
			ListNode<T> *node = m_first;
			while (node != m_last)
			{
				ListNode<T> *tmp = node;
				node = node->next;
				delete tmp;
			}
			m_first = m_last;
		}
	public:
		ListIterator<T> begin() const noexcept
		{
			return ListIterator<T>(this, m_first);
		}
		ListIterator<T> end() const noexcept
		{
			return ListIterator<T>(this, m_last);
		}
	private:
		void PushBackNode(ListNode<T> *node) noexcept
		{
			node->next = m_last;
			if (m_first == m_last)
			{
				m_first = node;
			}
			else
			{
				ListNode<T> *prev = m_last->prev;
				node->prev = prev;
				prev->next = node;
			}
			m_last->prev = node;
		}
		void PushFrontNode(ListNode<T> *node) noexcept
		{
			ListNode<T> *next = m_first;
			node->next = next;
			next->prev = node;
			m_first = node;
		}
		void InsertNode(const ListIterator<T> &ite, ListNode<T> *node) noexcept
		{
			if (ite == begin())
			{
				PushFrontNode(node);
			}
			else
			{
				ListNode<T> *prev = ite.m_prev;
				// ite.m_nodeはRemove()で無効になっている可能性がある。
				// 基本的にRemove()後のイテレータは使用しないが、事故防止のためにprevから取得しておく。
				ListNode<T> *next = prev->next;
				prev->next = node;
				node->prev = prev;
				node->next = next;
				next->prev = node;
			}
		}
	public:
		void PushBack(T const &obj) noexcept
		{
			ListNode<T> *current = new ListNode<T>(obj);
			PushBackNode(current);
		}
		void PushBack(T &&obj) noexcept
		{
			ListNode<T> *current = new ListNode<T>(System::move(obj));
			PushBackNode(current);
		}
		void PushFront(T const &obj) noexcept
		{
			ListNode<T> *current = new ListNode<T>(obj);
			PushFrontNode(current);
		}
		void PushFront(T &&obj) noexcept
		{
			ListNode<T> *current = new ListNode<T>(System::move(obj));
			PushFrontNode(current);
		}
		ListIterator<T> Insert(const ListIterator<T> &ite, T const &obj) noexcept
		{
			if (ite.m_list != this)
				return end();
			ListNode<T> *current = new ListNode<T>(obj);
			InsertNode(ite, current);
			return ListIterator<T>(this, current);
		}
		ListIterator<T> Insert(const ListIterator<T> &ite, T &&obj) noexcept
		{
			if (ite.m_list != this)
				return end();
			ListNode<T> *current = new ListNode<T>(System::move(obj));
			InsertNode(ite, current);
			return ListIterator<T>(this, current);
		}
		std::unique_ptr<T> PopFront() noexcept
		{
			if (m_first == m_last)
				return std::unique_ptr<T>();
			ListNode<T> *node = m_first;
			m_first = m_first->next;
			std::unique_ptr<T> ret = System::move(node->value);
			delete node;
			return ret;
		}
		std::unique_ptr<T> PopBack() noexcept
		{
			if (m_first == m_last)
				return std::unique_ptr<T>();
			ListNode<T> *node = m_last->prev;
			ListNode<T> *prev = node->prev;
			m_last->prev = prev;
			if (prev)
				prev->next = m_last;
			else
			{
				m_first = m_last;
			}
			std::unique_ptr<T> ret = System::move(node->value);
			delete node;
			return ret;
		}
		ListIterator<T> FindFront(T const &value) const noexcept
		{
			ListIterator<T> ret = begin();
			for (ListIterator<T> e = end(); ret != e; ++ret)
			{
				if (*ret == value)
					return ret;
			}
			return ret;
		}
		ListIterator<T> FindBack(T const &value) const noexcept
		{
			ListIterator<T> ret = end();
			if (m_first == m_last)
				return ret;
			const ListIterator<T> b = begin();
			while (--ret != b)
			{
				if (*ret == value)
					return ret;
			}
			if (*ret == value)
				return ret;
			return end();
		}
		bool Contains(T const &value) const noexcept
		{
			return FindFront(value) != end();
		}
		void Remove(const ListIterator<T> &ite) noexcept
		{
			if (ite.m_list != this || ite.m_node == m_last)
				return;
			ListNode<T> *current = ite.m_node;
			delete current;
			ListNode<T> *prev = ite.m_prev;
			ListNode<T> *next = ite.m_next;
			if (!prev)
			{
				m_first = next;
				m_first->prev = nullptr;
			}
			else
			{
				prev->next = next;
				if (next)
					next->prev = prev;
			}
		}
	public:
		List<T> &operator=(const List<T> &rhs) noexcept
		{
			if (this == &rhs)
				return *this;
			Clear();
			if (!rhs.Any())
				return *this;
			ListNode<T> *rhsNode = rhs.m_last->prev;
			do
			{
				ListNode<T> *tmp = rhsNode->value ? new ListNode<T>(*rhsNode->value) : new ListNode<T>();
				PushFrontNode(tmp);
				rhsNode = rhsNode->prev;
			} while (rhsNode);
			return *this;
		}
		List<T> &operator=(List<T> &&rhs) noexcept
		{
			if (this == &rhs)
				return *this;
			Clear();
			ListNode<T> *empty = m_last;
			m_first = rhs.m_first;
			rhs.m_first = empty;
			m_last = rhs.m_last;
			rhs.m_last = empty;
			return *this;
		}
	};
}

//DisposingObject
export namespace System {
	/// <summary>
	/// 指定した回数Update()が呼び出されたとき、自動的に破棄されるオブジェクトを
	/// 表すクラス
	/// </summary>
	class DisposingObject {
		union {
			void* m_ptr = nullptr;
			uint8_t m_buf[sizeof(void*)];
		};
		std::function<void(DisposingObject&)> m_destructor;
		uint32_t m_remainCount = 1;
	private:
		template<class T>
		static void DisposePlacement(DisposingObject& obj) {
			reinterpret_cast<T*>(obj.m_buf)->~T();
			obj.m_ptr = nullptr;
		}
		template<class T>
		static void DisposePointer(DisposingObject& obj) {
			delete static_cast<T*>(obj.m_ptr);
			obj.m_ptr = nullptr;
		}
	public:
		template<class T>
		DisposingObject(T const& object, uint32_t remainCount) noexcept
			: m_remainCount(remainCount)
		{
			if constexpr (sizeof(T) < sizeof(m_buf)) {
				new (m_buf) T(object);
				m_destructor = DisposePlacement<T>;
			}
			else {
				m_ptr = new T(object);
				m_destructor = DisposePointer<T>;
			}
		}
		template<class T>
		DisposingObject(T&& object, uint32_t remainCount) noexcept
			: m_remainCount(remainCount)
		{
			if constexpr (sizeof(T) < sizeof(m_buf)) {
				new (m_buf) T(System::move(object));
				m_destructor = DisposePlacement<T>;
			}
			else {
				m_ptr = new T(System::move(object));
				m_destructor = DisposePointer<T>;
			}
		}
		template<class T>
		DisposingObject(T const& object, uint32_t remainCount, const std::function<void(T&)>& preDispose) noexcept
			: m_remainCount(remainCount)
		{
			std::function<void(T&)> capturePreDispose = preDispose;
			if constexpr (sizeof(T) < sizeof(m_buf)) {
				new (m_buf) T(object);
				m_destructor = [capturePreDispose](DisposingObject& obj) mutable {
					capturePreDispose(*reinterpret_cast<T*>(obj.m_buf));
					DisposePlacement<T>(obj);
				};
			}
			else {
				m_ptr = new T(object);
				m_destructor = [capturePreDispose](DisposingObject& obj) mutable {
					capturePreDispose(*static_cast<T*>(obj.m_ptr));
					DisposePointer<T>(obj);
				};
			}
		}
		template<class T>
		DisposingObject(T&& object, uint32_t remainCount, const std::function<void(T&)>& preDispose) noexcept
			: m_remainCount(remainCount)
		{
			std::function<void(T&)> capturePreDispose = preDispose;
			if constexpr (sizeof(T) < sizeof(m_buf)) {
				new (m_buf) T(System::move(object));
				m_destructor = [capturePreDispose](DisposingObject& obj) mutable {
					capturePreDispose(*reinterpret_cast<T*>(obj.m_buf));
					DisposePlacement<T>(obj);
				};
			}
			else {
				m_ptr = new T(System::move(object));
				m_destructor = [capturePreDispose](DisposingObject& obj) mutable {
					capturePreDispose(*static_cast<T*>(obj.m_ptr));
					DisposePointer<T>(obj);
				};
			}
		}
		DisposingObject(const DisposingObject&) noexcept = delete;
		DisposingObject(DisposingObject&& obj) noexcept
			: m_ptr(obj.m_ptr), m_destructor(System::move(obj.m_destructor)), m_remainCount(obj.m_remainCount)
		{
			obj.m_ptr = nullptr;
			obj.m_remainCount = 0;
		}
		~DisposingObject() noexcept { Dispose(); }
	public:
		bool Update() noexcept {
			if (!m_remainCount) return true;
			--m_remainCount;
			if (m_remainCount) return false;
			m_destructor(*this);
			return true;
		}
		void Dispose() noexcept {
			if (!m_remainCount) return;
			m_remainCount = 0;
			m_destructor(*this);
		}
		uint32_t GetRemainCount() const noexcept { return m_remainCount; }
	public:
		DisposingObject& operator=(const DisposingObject&) noexcept = delete;
		DisposingObject& operator=(DisposingObject&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_ptr = rhs.m_ptr;
			rhs.m_ptr = nullptr;
			m_destructor = System::move(rhs.m_destructor);
			m_remainCount = rhs.m_remainCount;
			rhs.m_remainCount = 0;
			return *this;
		}
	};
}

//DisposingHeap
namespace System {
	/// <summary>
	/// DisposingObjectを保持するクラス。
	/// 探索機能などは持たない
	/// </summary>
	class DisposingHeap {
		List<DisposingObject> m_list;
	public:
		DisposingHeap() noexcept = default;
		~DisposingHeap() noexcept {
			for (DisposingObject& obj : m_list) obj.Dispose();
		}
	public:
		void Push(DisposingObject&& obj) noexcept { m_list.PushFront(System::move(obj)); }
		void Update() noexcept {
			for (auto ite = m_list.begin(), e = m_list.end(); ite != e; ++ite) {
				if (static_cast<DisposingObject&>(*ite).Update()) m_list.Remove(ite);
			}
		}
	};
}

//Disposer
export namespace System {
	/// <summary>
	/// DisposingObjectを破棄するワーカークラス。
	/// スレッドセーフでDisposingObjectを渡すことができ、
	/// 破棄自体も別スレッドで行われる。
	/// </summary>
	class Disposer {
		TaskWorker m_worker = TaskWorker(1);
		TaskFuture<bool, bool> m_fence;
		DisposingHeap* m_heap = nullptr;
		std::mutex m_mtx;
	public:
		Disposer() noexcept {
			m_heap = new DisposingHeap();
			m_fence = m_worker.PushFence(1, false);
			TaskFuture<bool, bool> b = m_worker.Push<bool, bool>(
				[this](TaskPromise<bool, bool>& p) -> void {
					std::lock_guard lock{ m_mtx };
					if (m_heap) m_heap->Update();
				},
				2, false
			);
		}
		~Disposer() noexcept {
			std::lock_guard lock{ m_mtx };
			delete m_heap;
			m_heap = nullptr;
		}
	public:
		bool Push(DisposingObject&& obj) noexcept {
			m_fence.Wait(TaskState::Suspended);
			std::lock_guard lock{ m_mtx };
			if (!m_heap) return false;
			m_heap->Push(System::move(obj));
			return true;
		}
		template<class T>
		bool Push(T&& object, uint32_t remainCount) noexcept {
			return Push(DisposingObject{ System::move(object), remainCount });
		}
		template<class T>
		bool Push(T&& object, uint32_t remainCount, const std::function<void(T&)>& preDispose) noexcept {
			return Push(DisposingObject{ System::move(object), remainCount, preDispose });
		}
		void Update() noexcept {
			m_fence.Wait(TaskState::Suspended);
			m_fence.Run();
		}
		/// <summary>
		/// すべてのDisposingObjectを強制的に破棄する。
		/// これは呼び出したスレッドで行われる。
		/// あるAPIに依存したオブジェクトを保持しているとき、
		/// そのAPIの終了前にこの関数を呼び出すことで、
		/// APIに依存したオブジェクトが解放済みであることを保証できる
		/// </summary>
		void Reset() noexcept {
			std::lock_guard lock{ m_mtx };
			delete m_heap;
			m_heap = new DisposingHeap();
		}
	};
}
