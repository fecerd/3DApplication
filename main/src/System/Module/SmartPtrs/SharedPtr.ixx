export module SharedPtr;
import CSTDINT;
import Traits;
import Allocator;
import <atomic>;
import <typeinfo>;
import <functional>;	//specialization std::hash<>, std::equal_to

//前方宣言
export namespace System {
	template<class T> class SharedPtr;
	template<class T> class WeakPtr;
	template<class T> class EnableSharedFromThis;
}

//Functions
namespace System::Internal {
	inline int32_t AtomicIncrement(std::atomic<int32_t>& x) noexcept {
		return x.fetch_add(1, std::memory_order_relaxed);
	}
	inline int32_t AtomicDecrement(std::atomic<int32_t>& x) noexcept {
		return x.fetch_sub(1, std::memory_order_acq_rel);
	}
	inline int32_t AtomicConditionalIncrement(std::atomic<int32_t>& x) noexcept {
		int32_t r = x.load(std::memory_order_relaxed);
		do {
			if (r == 0) return r;
			if (x.compare_exchange_weak(r, r + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
				return r;
			}
		} while(true);
	}
	inline int32_t AtomicLoad(std::atomic<int32_t> const& x) noexcept {
		return x.load(std::memory_order_acquire);
	}

	template<class T>
	size_t HashPointer(T* p) noexcept {
		size_t v = reinterpret_cast<size_t>(p);
		return static_cast<size_t>(v + (v >> 3));
	}
}

//PtrCounter
namespace System::Internal {
	class IPtrCounter {
		std::atomic<int32_t> m_shared;
		std::atomic<int32_t> m_weak;
	public:
		constexpr IPtrCounter() noexcept : m_shared(1), m_weak(1) {}
		constexpr IPtrCounter(const IPtrCounter&) = delete;
		virtual constexpr ~IPtrCounter() noexcept {}
	public:
		int32_t IncrementShared() noexcept {
			return AtomicIncrement(m_shared);
		}
		int32_t DecrementShared() noexcept {
			return AtomicDecrement(m_shared);
		}
		int32_t IncrementWeak() noexcept {
			return AtomicIncrement(m_weak);
		}
		int32_t DecrementWeak() noexcept {
			return AtomicDecrement(m_weak);
		}
		int32_t UseCountShared() const noexcept {
			return AtomicLoad(m_shared);
		}
		int32_t UseCountWeak() const noexcept {
			return AtomicLoad(m_weak);
		}
		bool IncrementConditional() noexcept {
			return AtomicConditionalIncrement(m_shared) != 0;
		}
	public:
		/// @brief 共有データptrをdeleteする。TとDeleterに応じてoverrideする。
		/// @param ptr SharedPtrによって管理されるポインタ。
		/// @details 
		virtual constexpr void Dispose(void* ptr) noexcept = 0;
		/// @brief sharedとweakがともに参照0になったとき、自身を破壊するために呼び出す。Allocatorに応じてoverrideする。
		virtual constexpr void Destroy() noexcept { delete this; }
	};

	template<class T>
	class PtrCounter : public IPtrCounter {
		using element_type = Traits::remove_all_extents_t<T>;
	public:
		constexpr void Dispose(void* ptr) noexcept override {
			element_type* p = static_cast<element_type*>(ptr);
			DefaultDelete<T>()(p);
		}
		constexpr void Destroy() noexcept override {
			delete this;
		}
	};

	template<class T, class Deleter>
	class PtrCounterD : public IPtrCounter {
		using element_type = Traits::remove_all_extents_t<T>;
	private:
		Deleter m_deleter;
	public:
		PtrCounterD() noexcept = delete;
		PtrCounterD(const PtrCounterD&) noexcept = default;
		PtrCounterD(Deleter const& deleter) noexcept : m_deleter(deleter) {}
		PtrCounterD(Deleter&& deleter) noexcept : m_deleter(System::move(deleter)) {}
		~PtrCounterD() noexcept = default;
	public:
		constexpr void Dispose(void* ptr) noexcept override {
			element_type* p = static_cast<element_type*>(ptr);
			m_deleter(p);
		}
		constexpr void Destroy() noexcept override { delete this; }
	};
	template<class T, CDefaultDeleter<T> Deleter>
	class PtrCounterD<T, Deleter> : public IPtrCounter {
		using element_type = Traits::remove_all_extents_t<T>;
	public:
		constexpr void Dispose(void* ptr) noexcept override {
			element_type* p = static_cast<element_type*>(ptr);
			Deleter()(p);
		}
		constexpr void Destroy() noexcept override { delete this; }
	};

	template<class T, class Deleter, class Allocator>
	class PtrCounterDA : public IPtrCounter {
		using element_type = Traits::remove_all_extents_t<T>;
	private:
		Deleter m_deleter;
		Allocator m_allocator;
	public:
		PtrCounterDA() noexcept = delete;
		PtrCounterDA(const PtrCounterDA&) noexcept = default;
		template<class D, class A>
		PtrCounterDA(D&& deleter, A&& allocator) noexcept
			: m_deleter(System::move(deleter)), m_allocator(System::move(allocator)) {}
		~PtrCounterDA() noexcept = default;
	public:
		constexpr void Dispose(void* ptr) noexcept override {
			element_type* p = static_cast<element_type*>(ptr);
			m_deleter(p);
		}
		constexpr void Destroy() noexcept override {
			AllocatorTraits<Allocator>::destroy(m_allocator, this);
			m_allocator.deallocate(this, 1);
		}
	};
}

//SharedPtr
export namespace System {
	template<class T>
	class SharedPtr {
		template<class U> friend class SharedPtr;
		template<class U> friend class WeakPtr;
	public:
		using element_type = Traits::remove_all_extents_t<T>;
		using weak_type = WeakPtr<T>;
	private:
		element_type* m_ptr;
		Internal::IPtrCounter* m_counter;
	public:
		constexpr SharedPtr() noexcept : m_ptr(nullptr), m_counter(nullptr) {}
		constexpr SharedPtr(const SharedPtr& arg) noexcept : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			if (m_counter) m_counter->IncrementShared();
		}
		constexpr SharedPtr(SharedPtr&& arg) noexcept : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr SharedPtr(const SharedPtr<Y>& arg) noexcept requires(Traits::Concepts::CNotArray<T>)
			: m_ptr(static_cast<element_type*>(arg.m_ptr)), m_counter(arg.m_counter)
		{
			if (m_counter) m_counter->IncrementShared();
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr SharedPtr(SharedPtr<Y>&& arg) noexcept requires(Traits::Concepts::CNotArray<T>)
			: m_ptr(static_cast<element_type*>(arg.m_ptr)), m_counter(arg.m_counter)
		{
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		template<class Y>
		constexpr SharedPtr(const SharedPtr<Y>& arg) noexcept requires(Traits::Concepts::CArray<T>) : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			if (m_counter) m_counter->IncrementShared();
		}
		template<class Y>
		constexpr SharedPtr(SharedPtr<Y>&& arg) noexcept requires(Traits::Concepts::CArray<T>) : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		constexpr ~SharedPtr() noexcept {
			if (m_counter) {
				if (m_counter->DecrementShared() == 1) {
					m_counter->Dispose(m_ptr);
					if (m_counter->DecrementWeak() == 1) {
						m_counter->Destroy();
					}
				}
			}
			m_ptr = nullptr;
			m_counter = nullptr;
		}
	public:
		constexpr SharedPtr(nullptr_t) noexcept : SharedPtr() {}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		explicit constexpr SharedPtr(Y* ptr) requires(Traits::Concepts::CNotArray<T>) : m_ptr(static_cast<element_type*>(ptr)) {
			m_counter = new Internal::PtrCounter<Y>();
			//非配列かつEnableSharedFromThisを継承しているとき、
			if constexpr (Traits::Concepts::CAssignableFrom<EnableSharedFromThis<Y> const*, Y const*>) {
				if (ptr) ptr->InternalAcceptOwner(this, ptr);
			}
		}
		template<class Y>
		explicit constexpr SharedPtr(Y* ptr) requires(Traits::Concepts::CArray<T>) : m_ptr(static_cast<element_type*>(ptr)) {
			m_counter = new Internal::PtrCounter<T>();
		}
		template<class Y, class Deleter>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr SharedPtr(Y* ptr, Deleter d) noexcept requires(Traits::Concepts::CNotArray<T>) : m_ptr(static_cast<element_type*>(ptr)) {
			if constexpr (CDefaultDeleter<Deleter, Y>) {
				m_counter = new Internal::PtrCounterD<Y, Deleter>();
			} else {
				m_counter = new Internal::PtrCounterD<Y, Deleter>(d);
			}
			if constexpr (Traits::Concepts::CAssignableFrom<System::EnableSharedFromThis<Y> const*, Y const*>) {
				if (ptr) ptr->InternalAcceptOwner(this, ptr);
			}
		}
		template<class Y, class Deleter>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr SharedPtr(Y* ptr, Deleter d) noexcept requires(Traits::Concepts::CArray<T>) : m_ptr(static_cast<element_type*>(ptr)) {
			if constexpr (CDefaultDeleter<Deleter, Y>) {
				m_counter = new Internal::PtrCounterD<Y, Deleter>();
			} else {
				m_counter = new Internal::PtrCounterD<Y, Deleter>(d);
			}
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr SharedPtr(const WeakPtr<Y>& arg) noexcept : m_counter(arg.m_counter) {
			if (!m_counter || !m_counter->IncrementConditional()) {
				m_counter = nullptr;
			}
			else {
				m_ptr = static_cast<element_type*>(arg.m_ptr);
			}
		}
	public:
		void Reset() noexcept {
			SharedPtr().Swap(*this);
		}
		template<class Y>
		void Reset(Y* ptr) noexcept {
			if (!ptr || ptr != m_ptr) SharedPtr(ptr).Swap(*this);
		}
		element_type* Get() const noexcept { return m_ptr; }
		int32_t UseCount() const noexcept { return m_counter ? m_counter->UseCountShared() : 0; }
		void Swap(SharedPtr& other) noexcept {
			element_type* t_ptr = other.m_ptr;
			Internal::IPtrCounter* t_counter = other.m_counter;
			other.m_ptr = m_ptr;
			other.m_counter = m_counter;
			m_ptr = t_ptr;
			m_counter = t_counter;
		}
	public:
		void reset() noexcept { Reset(); }
		template<class Y> void reset(Y* ptr) noexcept { Reset<Y>(ptr); }
		element_type* get() const noexcept { return m_ptr; }
		int32_t use_count() const noexcept { return UseCount(); }
		void swap(SharedPtr& other) noexcept { Swap(other); }
	public:
		explicit operator bool() const noexcept { return m_ptr; }
		bool operator!() const noexcept { return !m_ptr; }
	public:
		SharedPtr& operator=(const SharedPtr& rhs) noexcept {
			SharedPtr(rhs).Swap(*this);
			return *this;
		}
		SharedPtr& operator=(SharedPtr&& rhs) noexcept {
			SharedPtr(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		SharedPtr& operator=(const SharedPtr<Y>& rhs) noexcept {
			SharedPtr(rhs).Swap(*this);
			return *this;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		SharedPtr& operator=(SharedPtr&& rhs) noexcept {
			SharedPtr(System::move(rhs)).Swap(*this);
			return *this;
		}
	public:
		element_type& operator*() const noexcept requires(Traits::Concepts::CNotArray<T> && Traits::Concepts::CNotVoid<T>) {
			return *m_ptr;
		}
		element_type* operator->() const noexcept requires(Traits::Concepts::CNotArray<T>) {
			return m_ptr;
		}
		Traits::array_access_return_t<T> operator[](size_t index) const noexcept requires(Traits::Concepts::CArray<T>) {
			return static_cast<Traits::array_access_return_t<T>>(m_ptr[index]);
		}
	};
}

//MakeShared
export namespace System {
	template<Traits::Concepts::CNotArray T, class ...Args>
	SharedPtr<T> MakeShared(Args&& ...args) noexcept {
		T* ptr = DefaultNew<T>()(System::move(args)...);
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	requires Traits::Concepts::CDefaultConstructible<Traits::remove_all_extents_t<T>>
	SharedPtr<T> MakeShared(size_t N) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T>()(N);
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeShared() noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T>()();
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}

	template<Traits::Concepts::CUnknownBoundArray T>
	SharedPtr<T> MakeShared(size_t N, const Traits::remove_extent_t<T>& u) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = CopyNew<T>(N)(u);
		return SharedPtr<T>(ptr, CopyDelete<T>(N));
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeShared(const Traits::remove_extent_t<T>& u) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = CopyNew<T>()(u);
		return SharedPtr<T>(ptr, CopyDelete<T>());
	}

	template<Traits::Concepts::CNotArray T>
	SharedPtr<T> MakeSharedForOverwrite() noexcept {
		T* ptr = DefaultNew<T, true>()();
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	SharedPtr<T> MakeSharedForOverwrite(size_t N) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T, true>()(N);
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeSharedForOverwrite() noexcept {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T, true>()();
		return SharedPtr<T>(ptr, DefaultDelete<T>());
	}

}

//WeakPtr
export namespace System {
	template<class T>
	class WeakPtr {
		template<class Y> friend class SharedPtr;
		template<class Y> friend class WeakPtr;
		using this_type = WeakPtr<T>;		
	public:
		using element_type = Traits::remove_extent_t<T>;
	private:
		element_type* m_ptr;
		Internal::IPtrCounter* m_counter;
	public:
		constexpr WeakPtr() noexcept : m_ptr(nullptr), m_counter(nullptr) {}
		constexpr WeakPtr(const WeakPtr& arg) noexcept : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			if (m_counter) m_counter->IncrementWeak();
		}
		constexpr WeakPtr(WeakPtr&& arg) noexcept : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr WeakPtr(const WeakPtr<Y>& arg) noexcept requires(Traits::Concepts::CNotArray<T>)
			: m_ptr(static_cast<element_type*>(arg.m_ptr)), m_counter(arg.m_counter)
		{
			if (m_counter) m_counter->IncrementWeak();
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr WeakPtr(WeakPtr<Y>&& arg) noexcept requires(Traits::Concepts::CNotArray<T>)
			: m_ptr(static_cast<element_type*>(arg.m_ptr)), m_counter(arg.m_counter)
		{
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		template<class Y>
		constexpr WeakPtr(const WeakPtr<Y>& arg) noexcept requires(Traits::Concepts::CArray<T>) : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			if (m_counter) m_counter->IncrementWeak();
		}
		template<class Y>
		constexpr WeakPtr(WeakPtr<Y>&& arg) noexcept requires(Traits::Concepts::CArray<T>) : m_ptr(arg.m_ptr), m_counter(arg.m_counter) {
			arg.m_ptr = nullptr;
			arg.m_counter = nullptr;
		}
		constexpr ~WeakPtr() noexcept {
			if (m_counter) {
				if (m_counter->DecrementWeak() == 1) {
					m_counter->Destroy();
				}
			}
			m_ptr = nullptr;
			m_counter = nullptr;
		}
	public:
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		constexpr WeakPtr(const SharedPtr<Y>& arg) noexcept : m_ptr(static_cast<element_type*>(arg.m_ptr)), m_counter(arg.m_counter) {
			if (m_counter) m_counter->IncrementWeak();
		}
	public:
		SharedPtr<T> Lock() const noexcept { return SharedPtr<T>(*this); }
		int32_t UseCount() const noexcept { return m_counter ? m_counter->UseCountShared() : 0; }
		bool Expired() const noexcept { return !UseCount(); }
		void Reset() noexcept { WeakPtr<T>().Swap(*this); }
		void Swap(WeakPtr& other) noexcept {
			element_type* t_ptr = other.m_ptr;
			Internal::IPtrCounter* t_counter = other.m_counter;
			other.m_ptr = m_ptr;
			other.m_counter = m_counter;
			m_ptr = t_ptr;
			m_counter = t_counter;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		bool OwnerEquals(const WeakPtr<Y>& rhs) const noexcept {
			return m_counter == rhs.m_counter;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		bool OwnerEquals(const SharedPtr<Y>& rhs) const noexcept {
			return m_counter == rhs.m_counter;
		}
		size_t OwnerHashValue() const noexcept {
			return Internal::HashPointer(m_counter);
		}
	public:
		SharedPtr<T> lock() const noexcept { return Lock(); }
		int32_t use_count() const noexcept { return UseCount(); }
		bool expired() const noexcept { return Expired(); }
		void reset() noexcept { Reset(); }
		void swap(WeakPtr& other) noexcept { Swap(other); }
	public:
		WeakPtr& operator=(const WeakPtr& rhs) noexcept {
			WeakPtr(rhs).Swap(*this);
			return *this;
		}
		WeakPtr& operator=(WeakPtr&& rhs) noexcept {
			WeakPtr(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		WeakPtr& operator=(const WeakPtr<Y>& rhs) noexcept {
			WeakPtr(rhs).Swap(*this);
			return *this;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		WeakPtr& operator=(WeakPtr&& rhs) noexcept {
			WeakPtr(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y>
		requires Traits::Concepts::CConvertibleTo<Y*, element_type*>
		WeakPtr& operator=(const SharedPtr<Y>& rhs) noexcept {
			WeakPtr(rhs).Swap(*this);
			return *this;
		}
	};
	template<class T>
	WeakPtr(SharedPtr<T>)->WeakPtr<T>;
}

//STL overload and specialization
export namespace std {
	template<class T>
	void swap(System::SharedPtr<T>& lhs, System::SharedPtr<T>& rhs) noexcept {
		lhs.Swap(rhs);
	}

	template<class T>
	struct hash<System::SharedPtr<T>> {
		size_t operator()(const System::SharedPtr<T>& arg) const noexcept {
			return hash<typename System::SharedPtr<T>::element_type*>()(arg.Get());
		}
	};
	template<class T>
	struct hash<System::WeakPtr<T>> {
		size_t operator()(const System::WeakPtr<T>& arg) const noexcept {
			return arg.OwnerHashValue();
		}
	};

	template<class T>
	struct equal_to<System::WeakPtr<T>> {
		bool operator()(const System::WeakPtr<T>& lhs, const System::WeakPtr<T>& rhs) const noexcept {
			return lhs.OwnerEquals(rhs);
		}
	};
}

//EnableSharedFromThis
export namespace System {
	template<class T>
	class EnableSharedFromThis {
		mutable WeakPtr<T> m_weak;
	public:
		constexpr EnableSharedFromThis() noexcept : m_weak() {}
		constexpr EnableSharedFromThis(const EnableSharedFromThis&) noexcept = default;
		constexpr ~EnableSharedFromThis() noexcept = default;
	public:
		SharedPtr<T> SharedFromThis() noexcept {
			return SharedPtr<T>(m_weak);
		}
		SharedPtr<T const> SharedFromThis() const noexcept {
			return SharedPtr<T const>(m_weak);
		}
		WeakPtr<T> WeakFromThis() noexcept {
			return m_weak;
		}
		WeakPtr<T const> WeakFromThis() const noexcept {
			return m_weak;
		}
	public:
		template<class X, class Y>
		void InternalAcceptOwner(const SharedPtr<X>* shared, Y* ptr) const noexcept {
			if (m_weak.Expired()) m_weak = SharedPtr<T>(*shared, ptr);	
		}
	public:
		EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept {
			return *this;
		}
	};
}
