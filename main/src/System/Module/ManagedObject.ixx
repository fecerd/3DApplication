export module ManagedObject;
import CSTDINT;
import Objects;
import HashMap;
import Thread;
import <type_traits>;

namespace System::Internal {
	template<class T>
	concept CHasManagedObjectDeleteType = requires {
		typename T::managedObject_delete_type;
	};
}

//IManagedBase
export namespace System {
	template<class T> class ManagedObject;

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
		IManagedBase() noexcept {}
		IManagedBase(T* ptr) noexcept : m_ptr(ptr), m_refCount(new size_t(1)) {}
		IManagedBase(const IManagedBase& arg) noexcept {
			LockGuard lock{ arg.GetMutex() };
			m_ptr = arg.m_ptr;
			m_refCount = arg.m_refCount;
			if (m_refCount) ++(*m_refCount);
		}
		IManagedBase(IManagedBase&& arg) noexcept {
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
				if constexpr (Internal::CHasManagedObjectDeleteType<T>) {
					using delete_type = T::managedObject_delete_type;
					delete_type* tmp = m_ptr;
					delete tmp;
				} else {
					delete m_ptr;
				}
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
		IManagedBase& operator=(IManagedBase&& rhs) noexcept {
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
		using maptype = HashMap<String, IManagedBase<T>>;
		using namemaptype = HashMap<T*, String>;
		using thistype = ManagedObject<T>;
		using basetype = IManagedBase<T>;
	private:
		static maptype*& GetHashMapPtr() noexcept {
			static maptype* ret = new maptype{ 2 };
			return ret;
		}
		static namemaptype*& GetNamesPtr() noexcept {
			static namemaptype* ret = new namemaptype{ 2 };
			return ret;
		}
	private:
		static maptype& GetHashMap() noexcept {
			maptype*& ptr = GetHashMapPtr();
			if (!ptr) {
				ptr = new maptype{ 2 };
			}
			return *ptr;
		}
		static namemaptype& GetNames() noexcept {
			namemaptype*& ptr = GetNamesPtr();
			if (!ptr) {
				ptr = new namemaptype{ 2 };
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
		static thistype GetObject(const String& name) noexcept {
			LockGuard lock{ m_hashmap_mtx };
			basetype* tmp = GetHashMap().AtPtr(name);
			if (!tmp) return ManagedObject();
			else return ManagedObject(*tmp);
		}
	protected:
		using basetype::m_ptr;
		using basetype::m_refCount;
	public:
		ManagedObject() noexcept : basetype() {}
		ManagedObject(T* ptr) noexcept : basetype(ptr) {}
		ManagedObject(const String& name, T* ptr) noexcept : basetype() {
			if (name.IsNullOrEmpty()) {
				m_ptr = ptr;
				m_refCount = new size_t(1);
			}
			else {
				LockGuard hashmap_lock{ m_hashmap_mtx };
				maptype& hashmap = GetHashMap();
				basetype* tmp = hashmap.AtPtr(name);
				if (tmp) {
					m_ptr = tmp->m_ptr;
					m_refCount = tmp->m_refCount;
					if constexpr (Internal::CHasManagedObjectDeleteType<T>) {
						using delete_type = T::managedObject_delete_type;
						delete_type* tmp = ptr;
						delete tmp;
					} else {
						delete ptr;
					}
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
		ManagedObject(const ManagedObject& arg) noexcept : basetype(arg) {}
		ManagedObject(ManagedObject&& arg) noexcept : basetype(System::move(arg)) {}
		ManagedObject(const basetype& arg) noexcept : basetype(arg) {}
		ManagedObject(basetype&& arg) noexcept : basetype(System::move(arg)) {}
		~ManagedObject() noexcept { Release(); }
	private:
		void ReleaseInternal() noexcept {
			LockGuard lock{ basetype::m_mtx };
			if (!*this) return;
			if (*this->m_refCount <= 2) {
				LockGuard hashmap_lock{ m_hashmap_mtx };
				LockGuard names_lock{ m_names_mtx };
				maptype*& pHashmap = GetHashMapPtr();
				namemaptype*& pNames = GetNamesPtr();
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
			namemaptype& names = GetNames();
			String* ret = names.AtPtr(this->m_ptr);
			return ret ? *ret : GetEmptyName();
		}
		void Release() noexcept {
			LockGuard lock{ basetype::m_mtx };
			ReleaseInternal();
			basetype::Release();
		}
	public:
		ManagedObject& operator=(const ManagedObject& rhs) noexcept {
			if (this == &rhs) return *this;
			ReleaseInternal();
			basetype::operator=(rhs);
			return *this;
		}
		ManagedObject& operator=(ManagedObject&& rhs) noexcept {
			if (this == &rhs) return *this;
			ReleaseInternal();
			basetype::operator=(System::move(rhs));
			return *this;
		}
	};
}
