export module ManagedObject;
import CSTDINT;
import Objects;
import HashMap;
import Thread;

//IManagedBase
namespace System {
	export template<class T> class ManagedObject;

	/// <summary>
	/// ManagedObjectの基底型。単純なスマートポインタ機能を提供する。
	/// ManagedObjectの持つHashMapの型をこちらにすることで、削除の循環が起きないようにする
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class IManagedBase {
		friend class ManagedObject<T>;
	protected:
		T* m_ptr = nullptr;
		size_t* m_refCount = nullptr;
		mutable RecursiveMutex m_mtx;
	public:
		IManagedBase() noexcept = default;
		IManagedBase(T* ptr) noexcept : m_ptr(ptr), m_refCount(new size_t(1)) {}
		IManagedBase(const IManagedBase& arg) noexcept {
			LockGuard lock{ arg.GetMutex() };
			m_ptr = arg.m_ptr;
			m_refCount = arg.m_refCount;
			if (m_refCount) ++(*m_refCount);
		}
		IManagedBase(IManagedBase<T>&& arg) noexcept {
			LockGuard lock{ arg.GetMutex() };
			m_ptr = arg.m_ptr;
			m_refCount = arg.m_refCount;
			arg.m_ptr = nullptr;
			arg.m_refCount = nullptr;
		}
		~IManagedBase() noexcept { Release(); }
	public:
		RecursiveMutex& GetMutex() const noexcept { return m_mtx; }
		void Release() noexcept {
			LockGuard lock{ m_mtx };
			if (!m_refCount) return;
			--(*m_refCount);
			if (!*m_refCount) {
				delete m_refCount;
				delete m_ptr;
			}
			m_refCount = nullptr;
			m_ptr = nullptr;
		}
	public:
		constexpr T* operator->() const noexcept {
			LockGuard lock{ m_mtx };
			return m_ptr;
		}
		constexpr T& operator*() const noexcept {
			LockGuard lock{ m_mtx };
			return *m_ptr;
		}
		explicit constexpr operator bool() const noexcept { return m_ptr && m_refCount; }
		constexpr bool operator==(const IManagedBase& rhs) noexcept {
			LockGuard lock{ m_mtx };
			LockGuard rhs_lock{ rhs.GetMutex() };
			return m_ptr == rhs.m_ptr;
		}
		IManagedBase& operator=(const IManagedBase& rhs) noexcept {
			if (this == &rhs) return *this;
			LockGuard rhs_lock{ rhs.GetMutex() };
			Release();
			LockGuard lock{ m_mtx };
			m_ptr = rhs.m_ptr;
			m_refCount = rhs.m_refCount;
			if (m_refCount) ++(*m_refCount);
			return *this;
		}
		IManagedBase& operator=(IManagedBase<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			LockGuard rhs_lock{ rhs.GetMutex() };
			Release();
			LockGuard lock{ m_mtx };
			m_ptr = rhs.m_ptr;
			rhs.m_ptr = nullptr;
			m_refCount = rhs.m_refCount;
			rhs.m_refCount = nullptr;
			return *this;
		}
	};
}

//ManagedObject
export namespace System {
	/// <summary>
	/// リソースに対応する一意の文字列を指定できる共有ポインタ。
	/// そのリソースを指すManagedObjectが一つ以上存在している間、文字列を指定してリソースを取得できる
	/// </summary>
	/// <typeparam name="T">管理するクラス。T*型ポインタを内部に持つ</typeparam>
	template<class T>
	class ManagedObject : public IManagedBase<T> {
		static HashMap<String, IManagedBase<T>>*& GetHashMapPtr() noexcept {
			static HashMap<String, IManagedBase<T>>* ret = new HashMap<String, IManagedBase<T>>{ 2 };
			return ret;
		}
		static HashMap<T*, String>*& GetNamesPtr() noexcept {
			static HashMap<T*, String>* ret = new HashMap<T*, String>{ 2 };
			return ret;
		}
	private:
		static HashMap<String, IManagedBase<T>>& GetHashMap() noexcept {
			HashMap<String, IManagedBase<T>>*& ptr = GetHashMapPtr();
			if (!ptr) {
				ptr = new HashMap<String, IManagedBase<T>>{ 2 };
			}
			return *ptr;
		}
		static HashMap<T*, String>& GetNames() noexcept {
			HashMap<T*, String>*& ptr = GetNamesPtr();
			if (!ptr) {
				ptr = new HashMap<T*, String>{ 2 };
			}
			return *ptr;
		}
		static const String& GetEmptyName() noexcept {
			static const String ret{};
			return ret;
		}
		inline static Mutex m_hashmap_mtx;
		inline static Mutex m_names_mtx;
	public:
		static ManagedObject<T> GetObject(const String& name) noexcept {
			LockGuard lock{ m_hashmap_mtx };
			IManagedBase<T>* tmp = GetHashMap().AtPtr(name);
			if (!tmp) return ManagedObject();
			else return ManagedObject(*tmp);
		}
	protected:
		using IManagedBase<T>::m_ptr;
		using IManagedBase<T>::m_refCount;
	public:
		ManagedObject() noexcept = default;
		ManagedObject(T* ptr) noexcept {
			m_ptr = ptr;
			m_refCount = new size_t(1);
		}
		ManagedObject(const String& name, T* ptr) noexcept {
			if (name.IsNullOrEmpty()) {
				m_ptr = ptr;
				m_refCount = new size_t(1);
			}
			else {
				LockGuard hashmap_lock{ m_hashmap_mtx };
				HashMap<String, IManagedBase<T>>& hashmap = GetHashMap();
				IManagedBase<T>* tmp = hashmap.AtPtr(name);
				if (tmp) {
					m_ptr = tmp->m_ptr;
					m_refCount = tmp->m_refCount;
					delete ptr;
				}
				else {
					m_ptr = ptr;
					m_refCount = new size_t(1);
					hashmap.Insert(name, *this);
					LockGuard names_lock{ m_names_mtx };
					GetNames().Insert(this->m_ptr, name);
				}
			}
		}
		ManagedObject(const ManagedObject<T>& arg) noexcept = default;
		ManagedObject(ManagedObject<T>&& arg) noexcept = default;
		ManagedObject(const IManagedBase<T>& arg) noexcept : IManagedBase<T>(arg) {}
		ManagedObject(IManagedBase<T>&& arg) noexcept : IManagedBase<T>(static_cast<IManagedBase<T>&&>(arg)) {}
		~ManagedObject() noexcept { Release(); }
	private:
		void ReleaseInternal() noexcept {
			LockGuard lock{ IManagedBase<T>::m_mtx };
			if (!*this) return;
			if (*this->m_refCount <= 2) {
				LockGuard hashmap_lock{ m_hashmap_mtx };
				LockGuard names_lock{ m_names_mtx };
				HashMap<String, IManagedBase<T>>*& pHashmap = GetHashMapPtr();
				HashMap<T*, String>*& pNames = GetNamesPtr();
				if (pHashmap && pNames) {
					const String* tmp = pNames->AtPtr(this->m_ptr);
					if (tmp) {
						const String name = *tmp;
						pNames->Remove(this->m_ptr);
						pHashmap->Remove(name);
					}
					if (!pNames->Any()) {
						delete pNames;
						pNames = nullptr;
					}
					if (!pHashmap->Any()) {
						delete pHashmap;
						pHashmap = nullptr;
					}
				}
			}
		}
	public:
		const String& GetName() const noexcept {
			LockGuard names_lock{ m_names_mtx };
			HashMap<T*, String>& names = GetNames();
			String* ret = names.AtPtr(this->m_ptr);
			return ret ? *ret : GetEmptyName();
		}
		void Release() noexcept {
			LockGuard lock{ IManagedBase<T>::m_mtx };
			ReleaseInternal();
			IManagedBase<T>::Release();
		}
	public:
		ManagedObject& operator=(const ManagedObject<T>& rhs) noexcept {
			if (this == &rhs) return *this;
			ReleaseInternal();
			IManagedBase<T>::operator=(rhs);
			return *this;
		}
		ManagedObject& operator=(ManagedObject<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			ReleaseInternal();
			IManagedBase<T>::operator=(static_cast<ManagedObject<T>&&>(rhs));
			return *this;
		}
	};
}
