export module TaskWorker;
import CSTDINT;
import Traits;
//import Thread;
//import Function;
//import SmartPtr;
import <memory>;	//std::shared_ptr
import <condition_variable>;	//std::condition_variable
import <mutex>;	//std::mutex, std::lock_guard
import <thread>;	//std::thread
import <functional>;	//std::function


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
		mutable std::mutex m_state_mtx;
		mutable std::condition_variable m_state_cv;
		std::shared_ptr<TaskState> m_state = std::make_shared<TaskState>(TaskState::Ready);
		std::shared_ptr<bool> m_worker_running;
		bool m_once;
	public:
		TaskStateBase() noexcept : m_worker_running(std::make_shared<bool>(true)), m_once(true) {}
		TaskStateBase(const std::shared_ptr<bool>& worker_running, bool once) noexcept
			: m_worker_running(worker_running), m_once(once) {}
		TaskStateBase(const TaskStateBase&) noexcept = delete;
		TaskStateBase(TaskStateBase&&) noexcept = delete;
		~TaskStateBase() noexcept {
			std::lock_guard lock{ m_state_mtx };
			*m_state = TaskState::Empty;
		}
	public:/* タスク状態取得 */
		TaskState GetState() const noexcept {
			std::lock_guard lock{ m_state_mtx };
			return *m_state;
		}
		bool IsReady() const noexcept {
			return GetState() == TaskState::Ready;
		}
		bool IsFinished() const noexcept {
			return GetState() == TaskState::Finished;
		}
		bool Once() const noexcept {
			std::lock_guard lock{ m_state_mtx };
			return m_once;
		}
		void Once(bool once) noexcept {
			std::lock_guard lock{ m_state_mtx };
			m_once = once;
		}
	public:/* Worker側からの操作 */
		void Notify() noexcept {
			std::lock_guard lock{ m_state_mtx };
			m_state_cv.notify_all();
		}
	public:/* Promise側の操作 */
		/// <summary>
		/// Finished -> Readyの遷移(通知しない)
		/// </summary>
		void Ready() noexcept {
			std::lock_guard state_lock{ m_state_mtx };
			if (*m_state != TaskState::Finished) return;
			*m_state = TaskState::Ready;
		}
		/// <summary>
		/// Running -> Suspendの遷移
		/// この関数を呼び出したスレッドは別スレッドからRun()が呼び出されるまでブロックされる
		/// </summary>
		void Suspend() noexcept {
			std::unique_lock state_lock{ m_state_mtx };
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
			std::lock_guard lock{ m_state_mtx };
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
			std::lock_guard state_lock{ m_state_mtx };
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
					std::unique_lock state_lock{ m_state_mtx };
					m_state_cv.wait(
						state_lock,
						[this, state] {
							if (*m_state == state || *m_state == TaskState::Empty) return true;
							//SuspendはFinish時にも抜けられる(現在は機能削除)
							//if (state == TaskState::Suspended) return *m_state == TaskState::Finished;

							//ForceはSuspend, Finishにかかわらず、停止状態なら抜ける
							if (state == TaskState::Force) return *m_state == TaskState::Suspended || *m_state == TaskState::Finished;
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
	public:
		TaskStateBase& operator=(const TaskStateBase&) noexcept = delete;
		TaskStateBase& operator=(TaskStateBase&& rhs) noexcept = delete;
	};

	template<class R>
	class TaskValueBase {
	public:
		using r_t = Traits::remove_cv_t<R>;
		using const_r_t = r_t const;
	protected:
		mutable std::mutex m_value_mtx;
		std::unique_ptr<r_t> m_value = nullptr;
		//前回のGetValue()以降にSetValue()された場合、true
		mutable bool m_isChangedValue = false;
	public:
		TaskValueBase() noexcept = default;
		TaskValueBase(const TaskValueBase&) noexcept = delete;
		TaskValueBase(TaskValueBase&& arg) noexcept = delete;
		~TaskValueBase() noexcept = default;
	public:/* 値操作 */
		template<Traits::Concepts::CMoveConstructibleTo<r_t> T>
		void SetValue(T&& value) noexcept {
			std::lock_guard lock{ m_value_mtx };
			if (m_value) *m_value = System::move(value);
			else m_value = std::make_unique<r_t>(System::move(value));
			m_isChangedValue = true;
		}
		bool HasValue() const noexcept {
			std::lock_guard lock{ m_value_mtx };
			return static_cast<bool>(m_value);
		}
		r_t& GetValue() noexcept {
			std::lock_guard lock{ m_value_mtx };
			m_isChangedValue = false;
			return *m_value;
		}
		const_r_t& GetValue() const noexcept {
			std::lock_guard lock{ m_value_mtx };
			m_isChangedValue = false;
			return *m_value;
		}
		bool IsValueChanged() const noexcept {
			std::lock_guard lock{ m_value_mtx };
			return m_isChangedValue;
		}
	public:
		TaskValueBase& operator=(const TaskValueBase&) noexcept = delete;
		TaskValueBase& operator=(TaskValueBase&& rhs) noexcept = delete;
	};

	template<class S>
	class TaskSuspendBase {
	public:
		static constexpr bool IsNotVoid = !Traits::is_void_v<Traits::remove_cv_t<S>>;
		using s_t = Traits::conditional_t<IsNotVoid, Traits::remove_cv_t<S>, bool>;
		using const_s_t = s_t const;
	private:
		mutable std::mutex m_suspend_mtx;
		std::unique_ptr<s_t> m_suspendValue = nullptr;
		//前回のGetSuspendValue()以降にSetSuspendValue()された場合、true
		mutable bool m_isChangedSuspendValue = false;
	public:
		TaskSuspendBase() noexcept = default;
		TaskSuspendBase(const TaskSuspendBase&) noexcept = delete;
		TaskSuspendBase(TaskSuspendBase&&) noexcept = delete;
		~TaskSuspendBase() noexcept = default;
	public:
		template <Traits::Concepts::CMoveConstructibleTo<s_t> T>
		void SetSuspendValue(T&& value) noexcept requires(IsNotVoid) {
			std::lock_guard lock{m_suspend_mtx};
			if (m_suspendValue) *m_suspendValue = System::move(value);
			else m_suspendValue = std::make_unique<s_t>(System::move(value));
			m_isChangedSuspendValue = true;
		}
		bool HasSuspendValue() const noexcept requires(IsNotVoid) {
			std::lock_guard lock{m_suspend_mtx};
			return static_cast<bool>(m_suspendValue);
		}
		auto& GetSuspendValue() noexcept requires(IsNotVoid) {
			std::lock_guard lock{m_suspend_mtx};
			m_isChangedSuspendValue = false;
			return *m_suspendValue;
		}
		auto& GetSuspendValue() const noexcept requires(IsNotVoid) {
			std::lock_guard lock{m_suspend_mtx};
			m_isChangedSuspendValue = false;
			return *m_suspendValue;
		}
		bool IsSuspendValueChanged() const noexcept requires(IsNotVoid) {
			std::lock_guard lock{m_suspend_mtx};
			return m_isChangedSuspendValue;
		}
	public:
		TaskSuspendBase<S>& operator=(const TaskSuspendBase<S>&) noexcept = delete;
		TaskSuspendBase<S>& operator=(TaskSuspendBase<S>&&) noexcept = delete;
	};

	template<class R, class S>
	class TaskData : public TaskStateBase, public TaskValueBase<R>, public TaskSuspendBase<S> {
	public:
		using TaskValueBase<R>::r_t;
		using TaskValueBase<R>::const_r_t;
		using TaskSuspendBase<S>::s_t;
		using TaskSuspendBase<S>::const_s_t;
		static constexpr bool Suspendable = TaskSuspendBase<S>::IsNotVoid;
	public:
		TaskData() noexcept = default;
		TaskData(const std::shared_ptr<bool>& worker_running, bool once) noexcept
			: TaskStateBase(worker_running, once), TaskValueBase<R>(), TaskSuspendBase<S>() {}
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
	public:
		static constexpr bool Suspendable = TaskData<R, S>::Suspendable;
	private:
		std::shared_ptr<TaskData<R, S>> m_ptr;
	public:
		TaskFuture() noexcept : m_ptr(nullptr) {}
		TaskFuture(const TaskFuture<R, S>&) noexcept = delete;
		TaskFuture(TaskFuture<R, S>&& arg) noexcept : m_ptr(System::move(arg.m_ptr)) {}
		~TaskFuture() noexcept = default;
	private:
		friend class TaskPromise<R, S>;
		TaskFuture(const std::shared_ptr<TaskData<R, S>>& arg) noexcept : m_ptr(arg) {}
	public:
		void Wait(TaskState state) noexcept { m_ptr->Wait(state); }
		void Run() noexcept { m_ptr->Run(); }
	public:
		bool IsFinished() const noexcept { return m_ptr->IsFinished(); }
		template<Traits::Concepts::CMoveConstructibleTo<r_t> T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(System::move(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
		template<Traits::Concepts::CMoveConstructibleTo<s_t> T>
		void SetSuspendValue(T&& value) noexcept requires(Suspendable) { m_ptr->SetSuspendValue(System::move(value)); }
		auto& GetSuspendValue() noexcept requires(Suspendable) { return m_ptr->GetSuspendValue(); }
		auto& GetSuspendValue() const noexcept requires(Suspendable) { return m_ptr->GetSuspendValue(); }
		bool HasSuspendValue() const noexcept requires(Suspendable) { return m_ptr->HasSuspendValue(); }
	public:
		TaskFuture<R, S>& operator=(const TaskFuture<R, S>&) noexcept = delete;
		TaskFuture<R, S>& operator=(TaskFuture<R, S>&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_ptr = System::move(rhs.m_ptr);
			return *this;
		}
	};

	template<class R, class S>
	class TaskPromise {
		using r_t = TaskData<R, S>::r_t;
		using s_t = TaskData<R, S>::s_t;
		using const_r_t = TaskData<R, S>::const_r_t;
		using const_s_t = TaskData<R, S>::const_s_t;
	public:
		static constexpr bool Suspendable = TaskData<R, S>::Suspendable;
	private:
		friend struct TaskNode;
		std::shared_ptr<TaskData<R, S>> m_ptr;
	public:
		TaskPromise() noexcept : m_ptr(std::make_shared<TaskData<R, S>>()) {}
		TaskPromise(const std::shared_ptr<bool>& worker_running, bool once) noexcept : m_ptr(std::make_shared<TaskData<R, S>>(worker_running, once)) {}
		TaskPromise(const TaskPromise<R, S>& arg) noexcept : m_ptr(arg.m_ptr) {}
		TaskPromise(TaskPromise<R, S>&& arg) noexcept : m_ptr(System::move(arg.m_ptr)) {}
		~TaskPromise() noexcept = default;
	public:
		TaskFuture<R, S> GetFuture() noexcept { return TaskFuture<R, S>(m_ptr); }
	public:
		void RemoveTask() noexcept { m_ptr->Once(true); }
		void Run() noexcept { m_ptr->Run(); }
		void Suspend() noexcept requires(Suspendable) { m_ptr->Suspend(); }
		void Finish() noexcept { m_ptr->Finish(); }
	public:
		template<Traits::Concepts::CMoveConstructibleTo<r_t> T>
		void SetValue(T&& value) noexcept { m_ptr->SetValue(System::move(value)); }
		r_t& GetValue() noexcept { return m_ptr->GetValue(); }
		const_r_t& GetValue() const noexcept { return m_ptr->GetValue(); }
		bool HasValue() const noexcept { return m_ptr->HasValue(); }
		template<Traits::Concepts::CMoveConstructibleTo<s_t> T>
		void SetSuspendValue(T&& value) noexcept requires(Suspendable) { m_ptr->SetSuspendValue(System::move(value)); }
		auto& GetSuspendValue() noexcept requires(Suspendable) { return m_ptr->GetSuspendValue(); }
		auto& GetSuspendValue() const noexcept requires(Suspendable) { return m_ptr->GetSuspendValue(); }
		bool HasSuspendValue() const noexcept requires(Suspendable) { return m_ptr->HasSuspendValue(); }
	public:
		TaskPromise<R, S>& operator=(const TaskPromise<R, S>& rhs) noexcept = default;
		TaskPromise<R, S>& operator=(TaskPromise<R, S>&& rhs) noexcept = default;
	};
}

//TaskNode
namespace System {
	struct TaskNode {
		std::shared_ptr<TaskStateBase> m_state;
		std::function<void(void)> m_task;
		TaskNode* m_prev = nullptr;
		TaskNode* m_next = nullptr;
		uint32_t m_level = MAX_VALUE<uint32_t>;
	public:
		TaskNode() noexcept = default;
		template<class R, class S>
		TaskNode(TaskPromise<R, S>&& p, const std::function<void(TaskPromise<R, S>&)>& task, uint32_t level) noexcept
			: m_state(p.m_ptr), m_level(level)
		{			
			m_task = [task = task, p = System::move(p)]() mutable {
				task(p);
				p.Finish();
			};
		}
		TaskNode(const TaskNode&) noexcept  = delete;
		TaskNode(TaskNode&&) noexcept = delete;
		~TaskNode() noexcept = default;
	public:
		TaskNode& operator=(const TaskNode&) noexcept = delete;
		TaskNode& operator=(TaskNode&&) noexcept = delete;
	};
}

//TaskWorker
export namespace System {
	class TaskWorker {
	private:
		mutable std::mutex m_main_mtx;
		std::condition_variable m_main_cv;
		std::shared_ptr<bool> m_running = std::make_shared<bool>(true);
		TaskNode* m_begin = nullptr;
		TaskNode* m_end = nullptr;
		uint32_t m_currentLevel = 0;
	private:
		std::unique_ptr<std::thread[]> m_threads;
		const uint32_t m_threadCount;
	private:
		void ThreadRunloop() noexcept {
			while (true) {
				TaskNode* node = nullptr;
				std::function<void(void)>* task = nullptr;
				{
					std::unique_lock main_lock{ m_main_mtx };
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
			m_threads = std::make_unique<std::thread[]>(m_threadCount);
			for (uint32_t i = 0; i < m_threadCount; ++i) {
				m_threads[i] = std::thread([this] { this->ThreadRunloop(); });
			}
		}
		TaskWorker(const TaskWorker&) noexcept = delete;
		TaskWorker(TaskWorker&&) noexcept = delete;
		~TaskWorker() noexcept {
			{
				std::lock_guard main_lock{ m_main_mtx };
				*m_running = false;
				//スレッド終了を通知
				m_main_cv.notify_all();
				//
				for (TaskNode* node = m_begin; node != m_end; node = node->m_next) {
					node->m_state->Notify();
				}
			}
			//スレッドが終わるまで待機
			for (uint32_t i = 0; i < m_threadCount; ++i) m_threads[i].join();
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
			std::lock_guard main_lock{ m_main_mtx };
			TaskPromise<R, S> p(m_running, once);
			TaskFuture<R, S> ret = p.GetFuture();
			if (!*m_running) {
				p.Finish();
				return ret;
			}
			//タスクをノードリストに追加
			TaskNode* tmp = new TaskNode(System::move(p), std::function<void(TaskPromise<R, S>&)>(System::move(task)), level);
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
	public:
		TaskWorker& operator=(const TaskWorker&) noexcept = delete;
		TaskWorker& operator=(TaskWorker&&) noexcept = delete;
	};
}
