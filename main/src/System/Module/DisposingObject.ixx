export module DisposingObject;
import CSTDINT;
import Function;
import List;
import TaskWorker;
using namespace System;

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
		Function<void(DisposingObject&)> m_destructor;
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
		DisposingObject(T const& object, uint32_t remainCount, const Function<void(T&)>& preDispose) noexcept
			: m_remainCount(remainCount)
		{
			Function<void(T&)> capturePreDispose = preDispose;
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
		DisposingObject(T&& object, uint32_t remainCount, const Function<void(T&)>& preDispose) noexcept
			: m_remainCount(remainCount)
		{
			Function<void(T&)> capturePreDispose = preDispose;
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
		Mutex m_mtx;
	public:
		Disposer() noexcept {
			m_heap = new DisposingHeap();
			m_fence = m_worker.PushFence(1, false);
			m_worker.Push<bool, void>(
				[this](TaskPromise<bool, void>& p) {
					LockGuard lock{ m_mtx };
					if (m_heap) m_heap->Update();
				},
				2, false
			);
		}
		~Disposer() noexcept {
			LockGuard lock{ m_mtx };
			delete m_heap;
			m_heap = nullptr;
		}
	public:
		bool Push(DisposingObject&& obj) noexcept {
			m_fence.Wait(TaskState::Suspended);
			LockGuard lock{ m_mtx };
			if (!m_heap) return false;
			m_heap->Push(System::move(obj));
			return true;
		}
		template<class T>
		bool Push(T&& object, uint32_t remainCount) noexcept {
			return Push(DisposingObject{ System::move(object), remainCount });
		}
		template<class T>
		bool Push(T&& object, uint32_t remainCount, const Function<void(T&)>& preDispose) noexcept {
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
			LockGuard lock{ m_mtx };
			delete m_heap;
			m_heap = new DisposingHeap();
		}
	};
}
