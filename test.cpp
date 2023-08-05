
#include <memory>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>
using namespace std;

enum class State
{
	Empty,
	Ready,
	Running,
	Suspended,
	Finished,
	Force
};

class INonCopyAndMove
{
public:
	INonCopyAndMove() noexcept = default;
	INonCopyAndMove(const INonCopyAndMove &) noexcept = delete;
	INonCopyAndMove(INonCopyAndMove &&) noexcept = delete;
	INonCopyAndMove &operator=(const INonCopyAndMove &) noexcept = delete;
	INonCopyAndMove &operator=(INonCopyAndMove &&) noexcept = delete;
};

class StateBase : public INonCopyAndMove
{
	mutable mutex m_state_mtx;
	mutable condition_variable m_state_cv;
	shared_ptr<State> m_state = make_shared<State>(State::Ready);
	shared_ptr<bool> m_worker_running;
	bool m_once;

public:
	StateBase() noexcept : StateBase(make_shared<bool>(true), true) {}
	StateBase(const shared_ptr<bool> &worker_running, bool once) noexcept : m_worker_running(worker_running), m_once(once) {}
	~StateBase() noexcept
	{
		lock_guard lock{m_state_mtx};
		*m_state = State::Empty;
		m_once = true;
	}

public:
	State GetState() const noexcept
	{
		lock_guard lock{m_state_mtx};
		return *m_state;
	}
	bool IsReady() const noexcept { return GetState() == State::Ready; }
	bool IsFinished() const noexcept { return GetState() == State::Finished; }
	bool Once() const noexcept
	{
		lock_guard lock{m_state_mtx};
		return m_once;
	}
	void Once(bool once) noexcept
	{
		lock_guard lock{m_state_mtx};
		m_once = once;
	}
	void Ready() noexcept
	{
		lock_guard lock{m_state_mtx};
		if (*m_state != State::Finished)
			return;
		*m_state = State::Ready;
	}
	void Suspend() noexcept
	{
		unique_lock lock{m_state_mtx};
		if (*m_state != State::Running)
			return;
		*m_state = State::Suspended;
		m_state_cv.notify_all();
		m_state_cv.wait(lock, [this]
						{ return *m_state == State::Running || !*m_worker_running; });
	}
	void Run() noexcept
	{
		lock_guard lock{m_state_mtx};
		if (*m_state == State::Ready)
			*m_state = State::Running;
		else if (*m_state == State::Suspended)
		{
			*m_state = State::Running;
			m_state_cv.notify_all();
		}
	}
	void Finish() noexcept
	{
		lock_guard lock{m_state_mtx};
		if (*m_state != State::Running)
			return;
		*m_state = State::Finished;
		m_state_cv.notify_all();
	}
	void Wait(State state) noexcept
	{
		if (state != State::Suspended && state != State::Force && state != State::Finished)
			return;
		do
		{
			{
				unique_lock lock{m_state_mtx};
				m_state_cv.wait(
					lock,
					[this, state]
					{
						if (*m_state == state || *m_state == State::Empty)
							return true;
						if (state == State::Force)
							return *m_state == State::Suspended || *m_state == State::Finished;
						else
							return false;
					});
				if (state != State::Force || *m_state == State::Finished)
					break;
			}
			Run();
		} while (true);
	}
	void Notify() noexcept
	{
		lock_guard lock{m_state_mtx};
		m_state_cv.notify_all();
	}
};
template <class R>
class ValueBase : public INonCopyAndMove
{
	mutable mutex m_value_mtx;
	unique_ptr<R> m_value = nullptr;
	mutable bool m_isChangedValue = false;

public:
	ValueBase() noexcept = default;
	~ValueBase() noexcept = default;

public:
	template <class T>
	void SetValue(T &&value) noexcept
	{
		lock_guard lock{m_value_mtx};
		if (m_value)
			*m_value = std::move(value);
		else
			m_value = make_unique<R>(std::move(value));
		m_isChangedValue = true;
	}
};
template <class S>
class SuspendBase : public INonCopyAndMove
{
	mutable mutex m_suspend_mtx;
	unique_ptr<S> m_suspend_value = nullptr;
	mutable bool m_isChangedSuspendValue = false;

public:
	SuspendBase() noexcept = default;
	~SuspendBase() noexcept = default;
};

template <class R, class S>
class Data : public StateBase, public ValueBase<R>, public SuspendBase<S>
{
public:
	Data() noexcept = default;
	Data(const shared_ptr<bool> &worker_running, bool once) noexcept : StateBase(worker_running, once), ValueBase<R>(), SuspendBase<S>() {}
	~Data() noexcept = default;
};

template <class R, class S>
class Promise;
template <class R, class S>
class Future
{
	shared_ptr<Data<R, S>> m_ptr;

public:
	Future() noexcept : m_ptr(nullptr) {}
	Future(const Future<R, S> &) noexcept = delete;
	Future(Future<R, S> &&arg) noexcept : m_ptr(std::move(arg.m_ptr)) {}
	~Future() noexcept = default;

private:
	friend class Promise<R, S>;
	Future(const shared_ptr<Data<R, S>> &arg) noexcept : m_ptr(arg) {}

public:
	Future<R, S> &operator=(const Future<R, S> &) noexcept = delete;
	Future<R, S> &operator=(Future<R, S> &&) noexcept = default;

public:
	void Wait(State state) noexcept { m_ptr->Wait(state); }
	void Run() noexcept { m_ptr->Run(); }
};

template <class R, class S>
class Promise
{
	friend struct Node;
	shared_ptr<Data<R, S>> m_ptr;

public:
	Promise() noexcept : m_ptr(make_shared<Data<R, S>>()) {}
	Promise(const shared_ptr<bool> &worker_running, bool once) noexcept : m_ptr(make_shared<Data<R, S>>(worker_running, once)) {}
	Promise(const Promise<R, S> &) noexcept = default;
	Promise(Promise<R, S> &&) noexcept = default;
	~Promise() noexcept = default;

public:
	Future<R, S> GetFuture() noexcept { return Future<R, S>(m_ptr); }

public:
	Promise<R, S> &operator=(const Promise<R, S> &) noexcept = default;
	Promise<R, S> &operator=(Promise<R, S> &&) noexcept = default;

public:
	template <class T>
	void SetValue(T &&value) noexcept
	{
		m_ptr->SetValue(std::move(value));
	}
	void Finish() noexcept
	{
		m_ptr->Finish();
	}
	void RemoveTask() noexcept { m_ptr->Once(true); }
	void Run() noexcept { m_ptr->Run(); }
	void Suspend() noexcept { m_ptr->Suspend(); }
};

struct Node
{
	shared_ptr<StateBase> m_state;
	function<void(void)> m_task;
	Node *m_prev = nullptr;
	Node *m_next = nullptr;
	unsigned int m_level = 0xffffffffu;

public:
	Node() noexcept = default;
	template <class R, class S>
	Node(Promise<R, S> &p, const function<void(Promise<R, S> &)> &task, unsigned int level) noexcept : m_level(level)
	{
		m_state = p.m_ptr;
		m_task = [task = task, p = std::move(p)]() mutable
		{
			task(p);
		};
	}
	Node(const Node &) noexcept = delete;
	Node(Node &&) noexcept = delete;
	~Node() noexcept = default;

public:
	Node &operator=(const Node &) noexcept = delete;
	Node &operator=(Node &&) noexcept = delete;
};

class Worker
{
	mutable mutex m_main_mtx;
	condition_variable m_main_cv;
	shared_ptr<bool> m_running = make_shared<bool>(true);
	Node *m_begin = nullptr;
	Node *m_end = nullptr;
	unsigned int m_currentLevel = 0;

private:
	unique_ptr<thread[]> m_threads;
	const unsigned int m_threadCount;

private:
	void Runloop() noexcept
	{
		while (true)
		{
			Node *node = nullptr;
			function<void(void)> *task = nullptr;
			{
				unique_lock main_lock{m_main_mtx};
				m_main_cv.wait(
					main_lock,
					[this]
					{ return !*m_running || m_begin != m_end; });
				if (!*m_running)
					return;
				bool finished = true;
				node = m_begin;
				while (node != m_end)
				{
					if (node->m_state->IsReady())
						break;
					finished = finished && node->m_state->IsFinished();
					node = node->m_next;
				}
				if (node->m_level > m_currentLevel)
				{
					if (finished)
					{
						if (node != m_end)
							m_currentLevel = node->m_level;
						else
						{
							Node *tmp = m_begin;
							while (tmp != m_end)
							{
								// 一度しか呼び出されないタスクをノードリストから削除する
								if (tmp->m_state->Once())
								{
									Node *prev = tmp->m_prev;
									Node *next = tmp->m_next;
									if (prev)
										prev->m_next = next;
									else
										m_begin = next;
									next->m_prev = prev;
									delete tmp;
									tmp = next;
								}
								else
								{
									tmp->m_state->Ready();
									tmp = tmp->m_next;
								}
							}
							m_currentLevel = 0;
							continue;
						}
					}
					else
					{
						// 何かしらのタスクが終了を通知する、もしくはデストラクタがスレッド終了を通知するまで待機
						m_main_cv.wait(main_lock);
						if (!*m_running)
							return;
						continue;
					}
				}
				node->m_state->Run();
				task = &node->m_task;
			}
			(*task)();
			m_main_cv.notify_all();
		}
	}

public:
	Worker() noexcept : Worker(4) {}
	Worker(unsigned int count) noexcept : m_threadCount(count)
	{
		m_begin = new Node();
		m_end = m_begin;
		m_threads = make_unique<thread[]>(m_threadCount);
		for (auto i = 0u; i < m_threadCount; ++i)
		{
			m_threads[i] = thread([this]
								  { this->Runloop(); });
		}
	}
	Worker(const Worker &) noexcept = delete;
	Worker(Worker &&) noexcept = delete;
	~Worker() noexcept
	{
		{
			lock_guard lock{m_main_mtx};
			*m_running = false;
			m_main_cv.notify_all();
			for (Node *node = m_begin; node != m_end; node = node->m_next)
			{
				node->m_state->Notify();
			}
		}
		for (auto i = 0u; i < m_threadCount; ++i)
			m_threads[i].join();
		Node *next = m_begin;
		while (next)
		{
			Node *cur = next;
			next = next->m_next;
			delete cur;
		}
		m_begin = nullptr;
		m_end = nullptr;
	}

public:
	template <class R, class S>
	Future<R, S> Push(function<void(Promise<R, S> &)> task, unsigned int level, bool once) noexcept
	{
		lock_guard main_lock{m_main_mtx};
		Promise<R, S> p(m_running, once);
		Future<R, S> ret = p.GetFuture();
		if (!*m_running)
		{
			p.Finish();
			return ret;
		}
		Node *tmp = new Node(p, std::move(task), level);
		Node *next = m_begin;
		while (next != m_end && next->m_level <= level)
			next = next->m_next;
		Node *prev = next->m_prev;
		if (m_begin == next)
			m_begin = tmp;
		if (prev)
			prev = prev->m_next = tmp;
		tmp->m_prev = prev;
		tmp->m_next = next;
		next->m_prev = tmp;
		m_main_cv.notify_all();
		return ret;
	}
};

int main()
{
	Worker *worker = new Worker();
	Future<int, bool> f = worker->Push(
		function<void(Promise<int, bool> &)>([](Promise<int, bool>& p){ p.Suspend(); int tmp = 0; p.SetValue(tmp); }),
		0, false
	);
	f.Wait(State::Suspended);
	f.Run();

	f.Wait(State::Suspended);
	delete worker;
	return 0;
}
