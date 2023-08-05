export module ExclusiveObject;
import <mutex>;	//unique_lock, mutex

export namespace System {
	template<class T>
	class ExclusiveObject {
		T* m_object;
		std::unique_lock<std::mutex> m_lock;
	public:
		ExclusiveObject() noexcept = delete;
		ExclusiveObject(T* object, std::mutex& mtx) noexcept
			: m_object(object), m_lock(mtx) {}
		ExclusiveObject(const ExclusiveObject&) noexcept = delete;
		ExclusiveObject(ExclusiveObject&&) noexcept = default;
		~ExclusiveObject() noexcept = default;
	public:
		T* operator->() noexcept { return m_object; }
		const T& operator->() const noexcept { return m_object; }
		T& operator*() noexcept { return *m_object; }
		T& operator*() const noexcept { return *m_object; }
		ExclusiveObject& operator=(const ExclusiveObject&) noexcept = delete;
		ExclusiveObject& operator=(ExclusiveObject&&) noexcept = default;
	public:
		bool OwnsLock() const noexcept { return m_lock.owns_lock(); }
		void Lock() noexcept { if (!m_lock.owns_lock()) m_lock.lock(); }
		void Unlock() noexcept { if (m_lock.owns_lock()) m_lock.unlock(); }
	};
}
