export module IPtrCounter;
import CSTDINT;
import Traits;
import <atomic>;	//std::atomic<>
import <typeinfo>;	//std::type_info
import <memory>;	//std::allocator
import <new>;	//overload new/delete

//Functions
export namespace System::Internal {
	template<class T, class RemoveT>
	constexpr void DefaultDelete(RemoveT* ptr) {
		if constexpr (Traits::is_array_v<T>){
			delete[] ptr;
		}
		else {
			delete ptr;
		}
	}

	inline void AtomicIncrement(std::atomic<int32_t>* p) noexcept {
		p->fetch_add(1, std::memory_order_relaxed);
	}
	inline int32_t AtomicDecrement(std::atomic<int32_t>* p) noexcept {
		return p->fetch_sub(1, std::memory_order_acq_rel);
	}
	inline int32_t AtomicConditionalIncrement(std::atomic<int32_t>* p) noexcept {
		int32_t r = p->load(std::memory_order_relaxed);
		do {
			if (r == 0) return r;
			if (p->compare_exchange_weak(r, r + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
				return r;
			}
		} while(true);
	}

	template<class T>
	inline constexpr void CheckedDelete(T* ptr) noexcept { delete ptr; }
	template<class T>
	inline constexpr void CheckedArrayDelete(T* ptr) noexcept { delete[] ptr; }

	template<class T>
	struct CheckedDeleter {
		using result_type = void;
		using argument_type = T*;
		void operator()(T* ptr) const { delete ptr; }
	};
	template<class T>
	struct CheckedArrayDeleter {
		using result_type = void;
		using argument_type = T*;
		void operator()(T* ptr) const { delete[] ptr; }
	};

	template<class D> class LocalDeleter;

	template<class D>
	D* GetLocalDeleter_Internal(D*) noexcept { return nullptr; }
	template<class D> D* GetLocalDeleter_Internal(LocalDeleter<D>*) noexcept;

	template<class T>
	class AddrOf_Ref {
		T& m_ref;
	public:
		inline AddrOf_Ref(T& t) noexcept : m_ref(t) {}
		inline operator T&() const noexcept { return m_ref; }
		AddrOf_Ref& operator=(const AddrOf_Ref&) noexcept = delete;
	};

	template<class T>
	struct AddrOf {
		static inline T* Get(T& t, long) noexcept {
			return reinterpret_cast<T*>(
				const_cast<char&>(reinterpret_cast<const volatile char&>(t))
			);
		}
		static inline T* Get(T* p, int) noexcept {
			return p;
		}
	};
	template<>
	struct AddrOf<nullptr_t> {
		using type = nullptr_t;
		static inline type* get(type& t, int) noexcept {
			return &t;
		}
	};
	template<>
	struct AddrOf<const nullptr_t> {
		using type = const nullptr_t;
		static inline type* get(type& t, int) noexcept {
			return &t;
		}
	};
	template<>
	struct AddrOf<volatile nullptr_t> {
		using type = volatile nullptr_t;
		static inline type* get(type& t, int) noexcept {
			return &t;
		}
	};
	template<>
	struct AddrOf<const volatile nullptr_t> {
		using type = const volatile nullptr_t;
		static inline type* get(type& t, int) noexcept {
			return &t;
		}
	};

	template<class T>
	inline constexpr T* AddressOf(T& x) noexcept {
		return AddrOf<T>::Get(AddrOf_Ref<T>(x), 0);
	}
}

//PtrCounter
export namespace System::Internal {
	class IPtrCounter {
		std::atomic<int32_t> m_use;
		std::atomic<int32_t> m_weak;
	public:
		constexpr IPtrCounter() noexcept : m_use(1), m_weak(1) {}
		constexpr IPtrCounter(const IPtrCounter&) = delete;
		virtual constexpr ~IPtrCounter() noexcept {}
	public:
		virtual constexpr void Dispose() noexcept = 0;
		virtual constexpr void Destroy() noexcept { delete this; }
		virtual constexpr void* GetDeleter(const std::type_info&) = 0;
		virtual constexpr void* GetLocalDeleter(const std::type_info&) = 0;
		virtual constexpr void* GetUntypedDeleter() noexcept = 0;
	public:
		void AddRefCopy() noexcept {
			AtomicIncrement(&m_use);
		}
		bool AddRefLock() noexcept {
			return AtomicConditionalIncrement(&m_use) != 0;
		}
		void Release() noexcept {
			if (AtomicDecrement(&m_use) == 1) {
				Dispose();
				WeakRelease();
			}
		}
		void WeakAddRef() noexcept {
			AtomicIncrement(&m_weak);
		}
		void WeakRelease() noexcept {
			if (AtomicDecrement(&m_weak) == 1) Destroy();
		}
		int32_t UseCount() const noexcept {
			return m_use.load(std::memory_order_acquire);
		}
	public:
		constexpr IPtrCounter& operator=(const IPtrCounter&) noexcept = delete;
	};

	template<class X>
	class PtrCounter : public IPtrCounter {
		X* m_ptr;
		using this_type = PtrCounter<X>;
	public:
		explicit constexpr PtrCounter(X* ptr) noexcept : m_ptr(ptr) {}
		constexpr PtrCounter(const PtrCounter&) noexcept = delete;
	public:
		constexpr void Dispose() noexcept override {
			CheckedDelete(m_ptr);
		}
		constexpr void* GetDeleter(const std::type_info&) noexcept override {
			return nullptr;
		}
		constexpr void* GetLocalDeleter(const std::type_info&) noexcept override {
			return nullptr;
		}
		constexpr void* GetUntypedDeleter() noexcept override {
			return nullptr;
		}
		constexpr void* operator new(std::size_t) {
			return std::allocator<this_type>().allocate(1);
		}
		constexpr void operator delete(void* p) noexcept {
			std::allocator<this_type>().deallocate(static_cast<this_type*>(p), 1);
		}
	public:
		constexpr PtrCounter& operator=(const PtrCounter&) noexcept = delete;
	};

	template<class P, class D>
	class PtrCounterDel : public IPtrCounter {
		P m_ptr;
		D m_deleter;
		using this_type = PtrCounterDel<P, D>;
	public:
		constexpr PtrCounterDel(P ptr) noexcept : m_ptr(ptr), m_deleter() {}
		constexpr PtrCounterDel(P ptr, D&& deleter) noexcept
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr PtrCounterDel(const PtrCounterDel&) noexcept = delete;
	public:
		constexpr void Dispose() noexcept override { m_deleter(m_ptr); }
		void* GetDeleter(const std::type_info& info) noexcept override {
			return info == typeid(D) ? &reinterpret_cast<char&>(m_deleter) : nullptr;
		}
		void* GetLocalDeleter(const std::type_info& info) noexcept override {
			return info == typeid(D) ? GetLocalDeleter_Internal(AddressOf(m_deleter)) : nullptr;
		}
		void* GetUntypedDeleter() noexcept override {
			return &reinterpret_cast<char&>(m_deleter);
		}
		void* operator new(size_t) noexcept {
			return std::allocator<this_type>().allocate(1);
		}
		void operator delete(void* p) noexcept {
			std::allocator<this_type>().deallocate(static_cast<this_type*>(p), 1);
		}
	public:
		PtrCounterDel& operator=(const PtrCounterDel&) noexcept = delete;
	};
	template<class P, class D>
	PtrCounterDel(P, D&)->PtrCounterDel<P, D>;

	template<class P, class D, class A>
	class PtrCounterDelAlloc : public IPtrCounter {
		P m_ptr;
		D m_deleter;
		A m_allocator;
		using this_type = PtrCounterDelAlloc<P, D, A>;
	public:
		constexpr PtrCounterDelAlloc(P ptr, A allocator) noexcept
		    : m_ptr(ptr), m_deleter(allocator), m_allocator(allocator) {}
		constexpr PtrCounterDelAlloc(P ptr, D&& deleter, A allocator) noexcept
		    : m_ptr(ptr), m_deleter(System::move(deleter)), m_allocator(allocator) {}
		constexpr PtrCounterDelAlloc(const PtrCounterDelAlloc&) noexcept = delete;
	public:
		constexpr void Dispose() noexcept override { m_deleter(m_ptr); }
		constexpr void Destroy() noexcept override {
			using alloc_type = typename std::allocator_traits<A>::template rebind_alloc<this_type>;
			alloc_type alloc(m_allocator);
			this->~this_type();
			alloc.deallocate(this, 1);
		}
		void* GetDeleter(const std::type_info& info) noexcept override {
			return info == typeid(D) ? &reinterpret_cast<char&>(m_deleter) : nullptr;
		}
		void* GetLocalDeleter(const std::type_info& info) noexcept override {
			return info == typeid(D) ? GetLocalDeleter_Internal(AddressOf(m_deleter)) : nullptr;
		}
		void* GetUntypedDeleter() noexcept override {
			return &reinterpret_cast<char&>(m_deleter);
		}
	public:
		PtrCounterDelAlloc& operator=(const PtrCounterDelAlloc&) noexcept = delete;
	};

	template<class D> struct InplaceTag {};

	template<class T>
	class ReferenceWrapper {
		T* m_ptr;
	public:
		explicit ReferenceWrapper(T& t) noexcept
		    : m_ptr(AddressOf(t)) {}
		template<class Y>
		void operator()(Y* ptr) const {
			(*m_ptr)(ptr);
		} 
	};

	template<class D>
	struct ConvertReference {
		using type = D;
	};
	template<class D>
	struct ConvertReference<D&> {
		using type = ReferenceWrapper<D>;
	};

	template<class T>
	size_t HashPointer(T* p) noexcept {
		size_t v = reinterpret_cast<size_t>(p);
		return static_cast<size_t>(v + (v >> 3));
	}

	template<class T>
	inline constexpr bool Less(T lhs, T rhs) noexcept {
		if constexpr (Traits::Concepts::CPointer<T>) {
			return reinterpret_cast<size_t>(lhs) < reinterpret_cast<size_t>(rhs);
		}
		else return lhs < rhs;
	}
}
