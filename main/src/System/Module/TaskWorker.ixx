export module TaskWorker;
import CSTDINT;
import Traits;
import Thread;
import Function;
export import SmartPtr;

//TaskState
export namespace System {
	enum class TaskState : uint8_t {
		Empty = 0,	//破棄されたタスク
		Ready,	//実行可能タスク
		Running,	//実行中タスク
		Suspended,	//一時停止中タスク
		Finished,	//終了タスク
		Force	//強制タスク(一時停止命令を無視)
	};
}

//TaskData
namespace System {
	class TaskStateBase {
		mutable Mutex m_state_mtx;
		ConditionVariable m_state_cv;
		SharedPtr<TaskState> m_state = std::make_shared<TaskState>(TaskState::Ready);
		SharedPtr<bool> m_worker_running;
		bool m_once;
	public:
		TaskStateBase() noexcept : m_worker_running(std::make_shared<bool>(true)), m_once(true) {}
		TaskStateBase(const SharedPtr<bool>& worker_running, bool once) noexcept
			: m_worker_running(worker_running), m_once(once) {}
		~TaskStateBase() noexcept {
			LockGuard lock{ m_state_mtx };
			*m_state = TaskState::Empty;
		}
	public:/* タスク状態取得 */
		TaskState GetState() const noexcept {
			LockGuard lock{ m_state_mtx };
			return *m_state;
		}
		bool IsReady() const noexcept {
			return GetState() == TaskState::Ready;
		}
		bool IsFinished() const noexcept {
			return GetState() == TaskState::Finished;
		}
		bool Once() const noexcept {
			LockGuard lock{ m_state_mtx };
			return m_once;
		}
		void Once(bool once) noexcept {
			LockGuard lock{ m_state_mtx };
			m_once = once;
		}
	public:/* Worker側からの操作 */
		void Notify() noexcept {
			LockGuard lock{ m_state_mtx };
			m_state_cv.notify_all();
		}
	public:/* Promise側の操作 */
		/// <summary>
		/// Finished -> Readyの遷移(通知しない)
		/// </summary>
		void Ready() noexcept {
			LockGuard state_lock{ m_state_mtx };
			if (*m_state != TaskState::Finished) return;
			*m_state = TaskState::Ready;
		}
		/// <summary>
		/// Running -> Suspendの遷移
		/// この関数を呼び出したスレッドは別スレッドからRun()が呼び出されるまでブロックされる
		/// </summary>
		void Suspend() noexcept {
			UniqueLock state_lock{ m_state_mtx };
			if (*m_state != TaskState::Running) return;
			*m_state = TaskState::Suspended;
			m_state_cv.notify_all();
			m_state_cv.wait(state_lock, [this] { return *m_state == TaskState::Running || !*m_worker_running; });
		}
		/// <summary>
		/// Running -> Finishedの遷移
		/// タスクの最後に呼び出す
		/// </summary>
		void Finish() noexcept {
			LockGuard lock{ m_state_mtx };
			if (*m_state != TaskState::Running) return;
			//Running -> Finishedの移行を通知
			*m_state = TaskState::Finished;
			m_state_cv.notify_all();
		}
	public: /* Promise, Future双方から呼べる操作 */
		/// <summary>
		/// Promise: Ready -> Runningの遷移(通知しない)
		/// Future: Suspend -> Runningの遷移(通知する)
		/// </summary>
		void Run() noexcept {
			LockGuard state_lock{ m_state_mtx };
			if (*m_state == TaskState::Ready) *m_state = TaskState::Running;
			else if (*m_state == TaskState::Suspended) {
				*m_state = TaskState::Running;
				m_state_cv.notify_all();
			}
		}
	public: /* Future側の操作 */
		/// <summary>
		/// タスク関数が指定した状態になるまで待機する
		/// </summary>
		/// <param name="state">Suspend, Finished, Forceのいずれか</param>
		void Wait(TaskState state) noexcept {
			if (state != TaskState::Suspended && state != TaskState::Force && state != TaskState::Finished) return;
			do {
				{
					//コンストラクタでロック取得
					UniqueLock state_lock{ m_state_mtx };
					m_state_cv.wait(
						state_lock,
						[this, state] {
							if (*m_state == state || *m_state == TaskState::Empty) return true;
							//SuspendはFinish時にも抜けられる
							//if (state == TaskState::Suspended) return *m_state == TaskState::Finished;

							//ForceはSuspend, Finishにかかわらず、停止状態なら抜ける
							else if (state == TaskState::Force) return *m_state == TaskState::Suspended || *m_state == TaskState::Finished;
							else return false;
						}
					);
					//Force以外を待機している場合、もしくはForceを待機していて現在の状態がFinishedの場合、Wait終了
					if (state != TaskState::Force || *m_state == TaskState::Finished) break;
				}
				//Forceを待機していて現在の状態がSuspendedのとき、ロックを解除してSuspend -> Running
				Run();
			} while (true);
		}
	};
	template<class R>
	class TaskValueBase {
	public:
		using r_t = Traits::remove_cv_t<R>;
		using const_r_t = const r_t;
	protected: /* 値 */
		mutable Mutex m_value_mtx;
		UniquePtr<r_t> m_value = nullptr;
		//前回のGetValue以降にSetValueされた場合、true
		mutable bool m_isChangedValue = false;
	public:
		TaskValueBase() noexcept = default;
		~TaskValueBase() noexcept = default;
	public:/* 値操作 */
		template<class T>
		void SetValue(T&& value) noexcept {
			LockGuard lock{ m_value_mtx };
			if (m_value) *m_value = static_cast<T&&>(value);
			else m_value = Unique<r_t>(static_cast<T&&>(value));
			m_isChangedValue = true;
		}
		bool HasValue() const noexcept {
			LockGuard lock{ m_value_mtx };
			return static_cast<bool>(m_value);
		}
		r_t& GetValue() noexcept {
			LockGuard lock{ m_value_mtx };
			m_isChangedValue = false;
			return *m_value;
		}
		const_r_t& GetValue() const noexcept {
			LockGuard lock{ m_value_mtx };
			m_isChangedValue = false;
			return *m_value;
		}
		bool IsValueChanged() const noexcept {
			LockGuard lock{ m_value_mtx };
			return m_isChangedValue;
		}
	};

	template<class R, class S>
	class TaskData : public TaskStateBase, public TaskValueBase<R> {
	public:
		using s_t = Traits::remove_cv_t<S>;
		using const_s_t = const s_t;
	private: /* 一時停止時の値 */
		using TaskValueBase<R>::m_value_mtx;
		UniquePtr<s_t> m_suspendValue = nullptr;
		mutable bool m_isChangedSuspendValue = false;
	public:
		TaskData() noexcept = default;
		TaskData(const SharedPtr<bool>& worker_running, bool once) noexcept : TaskStateBase(worker_running, once) {}
		~TaskData() noexcept = default;
	public:
		template<class T>
		void SetSuspendValue(T&& value) noexcept {
			LockGuard lock{ m_value_mtx };
			if (m_suspendValue) *m_suspendValue = static_cast<T&&>(value);
			else m_suspendValue = Unique<s_t>(static_cast<T&&>(value));
			m_isChangedSuspendValue = true;
		}
		bool HasSuspendValue() const noexcept {
			LockGuard lock{ m_value_mtx };
			return static_cast<bool>(m_suspendValue);
		}
		s_t& GetSuspendValue() noexcept {
			LockGuard lock{ m_value_mtx };
			m_isChangedSuspendValue = false;
			return *m_suspendValue;
		}
		const_s_t& GetSuspendValue() const noexcept {
			LockGuard lock{ m_value_mtx };
			m_isChangedSuspendValue = false;
			return *m_suspendValue;
		}
		bool IsSuspendValueChanged() const noexcept {
			LockGuard lock{ m_value_mtx };
			return m_isChangedSuspendValue;
		}
	};
	template<class R>
	class TaskData<R, void> : public TaskStateBase, public TaskValueBase<R> {
	public:
		TaskData() noexcept = default;
		TaskData(const SharedPtr<bool>& worker_running, bool once) noexcept : TaskStateBase(worker_running, once) {}
		~TaskData() noexcept = default;
	};
}

//TaskFuture, TaskPromise
export namespace System {
	template<class R, class S> class TaskPromise;
	template<class R, class S>
	class TaskFuture {
		using r_t = TaskData<R, S>::r_t;
		using s_t = TaskData<R, S>::s_t;
		using const_r_t = TaskData<R, S>::const_r_t;
		using const_s_t = TaskData<R, S>::const_s_t;
	private:
		SharedPtr<TaskData<R, S>> m_ptr;
	public:
		TaskFuture() noexcept = default;
		TaskFuture(const TaskFuture&) noexcept = delete;
		TaskFuture(TaskFuture&&) noexcept = default;
		~TaskFuture() noexcept = default;
	private:
		friend class TaskPromise<R, S>;
		TaskFuture(const SharedPtr<TaskData<R, S>>& arg) noexcept : m_ptr(arg) {}
	public:
		void Wait(TaskState state) noexcept { m_ptr->Wait(state); }
		void Run() noexcept { m_ptr->Run(); }
	public:
		bool IsFinished() const noexcept { return m_ptr->IsFinished(); }
		template<class T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(static_cast<T&&>(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
		template<class T>
		void SetSuspendValue(T&& value) noexcept { m_ptr->SetSuspendValue(static_cast<T&&>(value)); }
		s_t& GetSuspendValue() noexcept { return m_ptr->GetSuspendValue(); }
		const_s_t& GetSuspendValue() const noexcept { return m_ptr->GetSuspendValue(); }
		bool HasSuspendValue() const noexcept { return m_ptr->HasSuspendValue(); }
	public:
		TaskFuture<R, S>& operator=(TaskFuture<R, S>&& rhs) noexcept = default;
	};
	template<class R>
	class TaskFuture<R, void> {
		using r_t = TaskData<R, void>::r_t;
		using const_r_t = TaskData<R, void>::const_r_t;
	private:
		SharedPtr<TaskData<R, void>> m_ptr;
	public:
		TaskFuture() noexcept = default;
		TaskFuture(const TaskFuture&) noexcept = delete;
		TaskFuture(TaskFuture&&) noexcept = default;
		~TaskFuture() noexcept = default;
	private:
		friend class TaskPromise<R, void>;
		TaskFuture(const SharedPtr<TaskData<R, void>>& arg) noexcept : m_ptr(arg) {}
	public:
		void Wait(TaskState state) noexcept { m_ptr->Wait(state); }
		void Run() noexcept { m_ptr->Run(); }
	public:
		bool IsFinished() const noexcept { return m_ptr->IsFinished(); }
		template<class T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(static_cast<T&&>(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
	};

	template<class R, class S>
	class TaskPromise {
		using r_t = TaskData<R, S>::r_t;
		using s_t = TaskData<R, S>::s_t;
		using const_r_t = TaskData<R, S>::const_r_t;
		using const_s_t = TaskData<R, S>::const_s_t;
	private:
		friend struct TaskNode;
		SharedPtr<TaskData<R, S>> m_ptr;
	public:
		TaskPromise() noexcept : m_ptr(std::make_shared<TaskData<R, S>>()) {}
		TaskPromise(const SharedPtr<bool>& worker_running, bool once) noexcept : m_ptr(std::make_shared<TaskData<R, S>>(worker_running, once)) {}
		TaskPromise(const TaskPromise&) noexcept = default;
		TaskPromise(TaskPromise&&) noexcept = default;
		~TaskPromise() noexcept = default;
	public:
		TaskFuture<R, S> GetFuture() noexcept { return TaskFuture<R, S>(m_ptr); }
	public:
		void RemoveTask() noexcept { m_ptr->Once(true); }
		void Run() noexcept { m_ptr->Run(); }
		void Suspend() noexcept { m_ptr->Suspend(); }
		void Finish() noexcept { m_ptr->Finish(); }
	public:
		template<class T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(static_cast<T&&>(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
		template<class T>
		void SetSuspendValue(T&& value) noexcept { m_ptr->SetSuspendValue(static_cast<T&&>(value)); }
		s_t& GetSuspendValue() noexcept { return m_ptr->GetSuspendValue(); }
		const_s_t& GetSuspendValue() const noexcept { return m_ptr->GetSuspendValue(); }
		bool HasSuspendValue() const noexcept { return m_ptr->HasSuspendValue(); }
	};
	template<class R>
	class TaskPromise<R, void> {
		using r_t = TaskData<R, void>::r_t;
		using const_r_t = TaskData<R, void>::const_r_t;
	private:
		friend struct TaskNode;
		SharedPtr<TaskData<R, void>> m_ptr;
	public:
		TaskPromise() noexcept : m_ptr(std::make_shared<TaskData<R, void>>()) {}
		TaskPromise(const SharedPtr<bool>& worker_running, bool once) noexcept : m_ptr(std::make_shared<TaskData<R, void>>(worker_running, once)) {}
		TaskPromise(const TaskPromise&) noexcept = default;
		TaskPromise(TaskPromise&&) noexcept = default;
		~TaskPromise() noexcept = default;
	public:
		TaskFuture<R, void> GetFuture() noexcept { return TaskFuture<R, void>(m_ptr); }
	public:
		void RemoveTask() noexcept { m_ptr->Once(true); }
		void Run() noexcept { m_ptr->Run(); }
		void Finish() noexcept { m_ptr->Finish(); }
	public:
		template<class T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(static_cast<T&&>(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
	};
}

//TaskNode
namespace System {
	struct TaskNode {
		SharedPtr<TaskStateBase> m_state;
		Function<void(void)> m_task;
		TaskNode* m_prev = nullptr;
		TaskNode* m_next = nullptr;
		uint32_t m_level = MAX_VALUE<uint32_t>;
	public:
		TaskNode() noexcept = default;
		template<class R, class S>
		TaskNode(TaskPromise<R, S>& p, const Function<void(TaskPromise<R, S>&)>& task, uint32_t level) noexcept
			: m_level(level)
		{
			m_state = p.m_ptr;
			m_task = [task = task, p = static_cast<TaskPromise<R, S>&&>(p)]() mutable {
				task(p);
				p.Finish();
			};
		}
	};
}

//TaskWorker
export namespace System {
	class TaskWorker {
	private:
		mutable Mutex m_main_mtx;
		ConditionVariable m_main_cv;
		SharedPtr<bool> m_running = std::make_shared<bool>(true);
		TaskNode* m_begin = nullptr;
		TaskNode* m_end = nullptr;
		uint32_t m_currentLevel = 0;
	private:
		UniquePtr<Thread[]> m_threads;
		const uint32_t m_threadCount;
	private:
		void ThreadRunloop() noexcept {
			while (true) {
				TaskNode* node = nullptr;
				Function<void(void)>* task = nullptr;
				{
					UniqueLock main_lock{ m_main_mtx };
					//デストラクタからの終了、もしくはタスクの追加(0個でなくなる)を待機
					m_main_cv.wait(
						main_lock,
						[this] {
							return !*m_running || m_begin != m_end;
						}
					);
					//デストラクタからの終了の場合、スレッド終了
					if (!*m_running) return;
					//先頭から次のReady状態の処理まですべてのタスクがFinished状態ならtrue
					bool finished = true;
					node = m_begin;
					while (node != m_end) {
						if (node->m_state->IsReady()) break;
						finished = finished && node->m_state->IsFinished();
						node = node->m_next;
					}
					if (node->m_level > m_currentLevel) {
						if (finished) {
							if (node != m_end) m_currentLevel = node->m_level;
							else {
								TaskNode* tmp = m_begin;
								while (tmp != m_end) {
									//一度しか呼び出されないタスクをノードリストから削除する
									if (tmp->m_state->Once()) {
										TaskNode* prev = tmp->m_prev;
										TaskNode* next = tmp->m_next;
										if (prev) prev->m_next = next;
										else m_begin = next;
										next->m_prev = prev;
										delete tmp;
										tmp = next;
									}
									else {
										tmp->m_state->Ready();
										tmp = tmp->m_next;
									}
								}
								m_currentLevel = 0;
								continue;
							}
						}
						else {
							//何かしらのタスクが終了を通知する、もしくはデストラクタがスレッド終了を通知するまで待機
							m_main_cv.wait(main_lock);
							if (!*m_running) return;
							continue;
						}
					}
					//タスクの状態をReady -> Runに遷移
					node->m_state->Run();
					//タスク関数へのポインタを取得
					task = &node->m_task;
				}
				//タスク内でFinish()が呼ばれRunning -> Finish遷移
				(*task)();
				//タスクの終了を通知
				m_main_cv.notify_all();
			}
		}
	public:
		TaskWorker() noexcept : TaskWorker(4) {}
		TaskWorker(uint32_t threadCount) noexcept : m_threadCount(threadCount) {
			m_begin = new TaskNode{};
			m_end = m_begin;
			m_threads = Unique<Thread[]>(m_threadCount);
			for (uint32_t i = 0; i < m_threadCount; ++i) {
				m_threads[i] = Thread([this] { this->ThreadRunloop(); });
			}
		}
		~TaskWorker() noexcept {
			{
				LockGuard main_lock{ m_main_mtx };
				*m_running = false;
				//スレッド終了を通知
				m_main_cv.notify_all();
				//
				for (TaskNode* node = m_begin; node != m_end; node = node->m_next) {
					node->m_state->Notify();
				}
			}
			//スレッドが終わるまで待機
			for (uint32_t i = 0; i < m_threadCount; ++i) m_threads[i].Join();
			//ノードリスト破棄(この時点でサブスレッドは終了しているため排他制御不要)
			TaskNode* next = m_begin;
			while (next) {
				TaskNode* cur = next;
				next = next->m_next;
				delete cur;
			}
			m_begin = nullptr;
			m_end = nullptr;
		}
	public:
		template<class R, class S, class Functor>
		TaskFuture<R, S> Push(Functor&& task, uint32_t level, bool once) noexcept {
			LockGuard main_lock{ m_main_mtx };
			TaskPromise<R, S> p(m_running, once);
			TaskFuture<R, S> ret = p.GetFuture();
			if (!*m_running) {
				p.Finish();
				return ret;
			}
			//タスクをノードリストに追加
			TaskNode* tmp = new TaskNode(p, Function<void(TaskPromise<R, S>&)>(task), level);
			//先頭から検索して初めて追加するタスクよりレベルが高くなるタスク
			TaskNode* next = m_begin;
			while (next != m_end && next->m_level <= level) next = next->m_next;
			TaskNode* prev = next->m_prev;
			if (m_begin == next) m_begin = tmp;
			if (prev) prev->m_next = tmp;
			tmp->m_prev = prev;
			tmp->m_next = next;
			next->m_prev = tmp;
			//ノード追加を通知
			m_main_cv.notify_all();
			return ret;
		}
		TaskFuture<bool, bool> PushFence(uint32_t level, bool once) noexcept {
			return Push<bool, bool>(
				[](TaskPromise<bool, bool>& p) { p.Suspend(); },
				level, once
			);
		}
	};
}
