export module SharedPtr;
import CSTDINT;
import Traits;
import Allocator;
import IPtrCounter;
import UniquePtr;
import <typeinfo>;	//std::type_info
import <iostream>;	//overload operator<<
import <functional>;	//specialization std::hash<>

//SharedCount, WeakCount
namespace System::Internal {
	class WeakCount;

	class SharedCount {
		friend class WeakCount;
		IPtrCounter* m_count;
	public:
		constexpr SharedCount() noexcept : m_count(nullptr) {}
		constexpr explicit SharedCount(IPtrCounter* count) noexcept : m_count(count) {}
		template<class Y>
		explicit SharedCount(Y* p) noexcept : m_count(nullptr) {
			m_count = new PtrCounter<Y>(p);
			if (!m_count) CheckedDelete(p);
		}
		template<class P, class D>
		SharedCount(P ptr, D&& deleter) noexcept : m_count(nullptr) {
			m_count = new PtrCounterDel<P, D>(ptr, System::move(deleter));
		}
		template<class P, class D>
		SharedCount(P ptr, InplaceTag<D>) noexcept : m_count(nullptr) {
			m_count = new PtrCounterDel<P, D>(ptr);
		}
		template<class P, class D, class A>
		SharedCount(P ptr, D deleter, A allocator) noexcept : m_count(nullptr) {
			using impl_type = PtrCounterDelAlloc<P, D, A>;
			using alloc_type = typename AllocatorTraits<A>::template rebind_alloc<impl_type>;
			alloc_type alloc(allocator);
			m_count = alloc.allocate(1);
			if (m_count) {
				System::construct_at(static_cast<impl_type*>(m_count), ptr, deleter, allocator);
			} else {
				deleter(ptr);
			}
		}
		template<class P, class D, class A>
		SharedCount(P ptr, InplaceTag<D>, A allocator) noexcept : m_count(nullptr) {
			using impl_type = PtrCounterDelAlloc<P, D, A>;
			using alloc_type = typename AllocatorTraits<A>::template rebind_alloc<impl_type>;
			alloc_type alloc(allocator);
			m_count = alloc.allocate(1);
			if (m_count) {
				System::construct_at(static_cast<impl_type*>(m_count), ptr, allocator);
			} else {
				D::operator_fn(ptr);
			}
		}
		template<class Y, class D>
		explicit SharedCount(System::UniquePtr<Y, D>&& unique) : m_count(nullptr) {
			using del = typename ConvertReference<D>::type;
			del deleter{ unique.GetDeleter() };
			m_count = new PtrCounterDel<typename System::UniquePtr<Y, D>::pointer, del>(unique.Get(), deleter);
			unique.Release();
		}
		SharedCount(const SharedCount& arg) noexcept : m_count(arg.m_count) {
			if (m_count) m_count->AddRefCopy();
		}
		SharedCount(SharedCount&& arg) noexcept : m_count(arg.m_count) {
			arg.m_count = nullptr;
		}
		explicit SharedCount(const WeakCount&) noexcept;
		~SharedCount() noexcept {
			if (m_count) m_count->Release();
		}
	public:
		void Swap(SharedCount& rhs) noexcept {
			IPtrCounter* tmp = rhs.m_count;
			rhs.m_count = m_count;
			m_count = tmp;
		}
		void swap(SharedCount& rhs) noexcept { Swap(rhs); }
		int32_t UseCount() const noexcept {
			return m_count ? m_count->UseCount() : 0;
		}
		bool Unique() const noexcept {
			return UseCount() == 1;
		}
		bool Empty() const noexcept {
			return !m_count;
		}
	public:
		void* GetDeleter(const std::type_info& info) const noexcept {
			return m_count ? m_count->GetDeleter(info) : nullptr;
		}
		void* GetLocalDeleter(const std::type_info& info) const noexcept {
			return m_count ? m_count->GetLocalDeleter(info) : nullptr;
		}
		void* GetUntypedDeleter() const noexcept {
			return m_count ? m_count->GetUntypedDeleter() : nullptr;
		}
		size_t HashValue() const noexcept {
			return HashPointer(m_count);
		}
	public:
		SharedCount& operator=(const SharedCount& rhs) noexcept {
			IPtrCounter* tmp = rhs.m_count;
			if (tmp != m_count){
				if (tmp) tmp->AddRefCopy();
				if (m_count) m_count->Release();
				m_count = tmp;
			}
			return *this;
		}
		bool operator==(const SharedCount& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		bool operator==(const WeakCount& rhs) const noexcept;
		bool operator<(const SharedCount& rhs) const noexcept {
			return Less<IPtrCounter*>(m_count, rhs.m_count);
		}
		bool operator<(const WeakCount&) const noexcept;
	};

	class WeakCount {
		friend class SharedCount;
		IPtrCounter* m_count;
	public:
		constexpr WeakCount() noexcept : m_count(nullptr) {}
		WeakCount(const SharedCount& arg) noexcept : m_count(arg.m_count) {
			if (m_count) m_count->WeakAddRef();
		}
		WeakCount(const WeakCount& arg) noexcept : m_count(arg.m_count) {
			if (m_count) m_count->WeakAddRef();
		}
		WeakCount(WeakCount&& arg) noexcept : m_count(arg.m_count) {
			arg.m_count = nullptr;
		}
		~WeakCount() noexcept {
			if (m_count) m_count->WeakRelease();
		}
	public:
		void Swap(WeakCount& rhs) noexcept {
			IPtrCounter* tmp = rhs.m_count;
			rhs.m_count = m_count;
			m_count = tmp;
		}
		void swap(WeakCount& rhs) noexcept { Swap(rhs); }
		int32_t UseCount() const noexcept {
			return m_count ? m_count->UseCount() : 0;
		}
		bool Empty() const noexcept {
			return !m_count;
		}
	public:
		size_t HashValue() const noexcept {
			return HashPointer(m_count);
		}
	public:
		WeakCount& operator=(const SharedCount& rhs) noexcept {
			IPtrCounter* tmp = rhs.m_count;
			if (tmp != m_count) {
				if (tmp) tmp->WeakAddRef();
				if (m_count) m_count->WeakRelease();
				m_count = tmp;
			}
			return *this;
		}
		WeakCount& operator=(const WeakCount& rhs) noexcept {
			IPtrCounter* tmp = rhs.m_count;
			if (tmp != m_count) {
				if (tmp) tmp->WeakAddRef();
				if (m_count) m_count->WeakRelease();
				m_count = tmp;
			}
			return *this;
		}
		bool operator==(const WeakCount& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		bool operator==(const SharedCount& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		bool operator<(const WeakCount& rhs) const noexcept {
			return Less<IPtrCounter*>(m_count, rhs.m_count);
		}
		bool operator<(const SharedCount& rhs) const noexcept {
			return Less<IPtrCounter*>(m_count, rhs.m_count);
		}
	};

	SharedCount::SharedCount(const WeakCount& arg) noexcept : m_count(arg.m_count) {
		if (!m_count || !m_count->AddRefLock()) {
			m_count = nullptr;
		}
	}
	bool SharedCount::operator==(const WeakCount& rhs) const noexcept {
		return m_count == rhs.m_count;
	}
	bool SharedCount::operator<(const WeakCount& rhs) const noexcept {
		return Less<IPtrCounter*>(m_count, rhs.m_count);
	}
}

//前方宣言
export namespace System {
	template<class T> class SharedPtr;
	template<class T> class WeakPtr;
	template<class T> class EnableSharedFromThis;
}

//Functions
namespace System::Internal {
	///	<summary>
	/// SharedCountの初期化方法をT型が非配列、要素数不明配列、要素数既知配列の場合で分岐する。
	/// また、T型が非配列かつEnableSharedFromThisクラスから派生している場合、内部のWeakPtrが有効になる。
	/// Y*[](Y*[N])型からT*[](T*[N])型への代入ができない場合、コンパイルエラーとなる。
	///	</summary>
	template<class T, class Y>
	inline void PointerConstruct(System::SharedPtr<T>* pshared, Y* ptr, SharedCount& count) noexcept {
		using type = Traits::remove_extent_t<T>;
		using unknown_bound_type = type[];
		using known_bound_type = type[1];

		if constexpr (Traits::Concepts::CNotArray<T>) {
			SharedCount(ptr).Swap(count);
			if constexpr (Traits::Concepts::CAssignableFrom<System::EnableSharedFromThis<Y> const*, Y const*>) {
				if (ptr) ptr->InternalAcceptOwner(pshared, ptr);
			}
		} else if constexpr (Traits::Concepts::CUnknownBoundArray<T> && Traits::Concepts::CAssignableFrom<unknown_bound_type*, Y(*)[]>) {
			SharedCount(ptr, CheckedArrayDeleter<type>()).Swap(count);
		} else if constexpr (Traits::Concepts::CKnownBoundArray<T> && Traits::Concepts::CAssignableFrom<known_bound_type*, Y(*)[1]>) {
			SharedCount(ptr, CheckedArrayDeleter<type>()).Swap(count);
		} else {
			static_assert(Traits::value_v<bool, false, T, Y>, "Y*[](Y*[N])型がT*[](T*[N])型に代入できません。");
		}
	}

	///	<summary>
	///	T型が非配列かつEnableSharedFromThisクラスから派生している場合、内部のWeakPtrが有効になる。
	/// それ以外の場合、何もしない。
	///	</summary>
	template<class T, class Y>
	inline void DeleterConstruct(System::SharedPtr<T>* pshared, Y* ptr) noexcept {
		if constexpr (Traits::Concepts::CNotArray<T>) {
			if constexpr (Traits::Concepts::CAssignableFrom<System::EnableSharedFromThis<Y> const*, Y const*>) {
				if (ptr) ptr->InternalAcceptOwner(pshared, ptr);
			}
		}
	}
}
using namespace System::Internal;

//SharedPtr
export namespace System {
	template<class T>
	class SharedPtr {
		template<class Y> friend class SharedPtr;
		template<class Y> friend class WeakPtr;
		using this_type = SharedPtr<T>;
	public:
		using element_type = Traits::remove_extent_t<T>;
		using weak_type = WeakPtr<T>;
	private:
		element_type* m_ptr;
		SharedCount m_count;
	public:
		constexpr SharedPtr() noexcept : m_ptr(nullptr), m_count() {}
		SharedPtr(const SharedPtr& arg) noexcept : m_ptr(arg.m_ptr), m_count(arg.m_count) {}
		SharedPtr(SharedPtr&& arg) noexcept : m_ptr(arg.m_ptr), m_count(System::move(arg.m_count)) {
			arg.m_ptr = nullptr;
		}
		constexpr SharedPtr(nullptr_t) noexcept : m_ptr(nullptr), m_count() {}
		template<class Y>
		explicit SharedPtr(Y* ptr) noexcept : m_ptr(ptr), m_count() {
			PointerConstruct(this, ptr, m_count);
		}
		template<class Y, class D>
		SharedPtr(Y* ptr, D deleter) noexcept : m_ptr(ptr), m_count(ptr, System::move(deleter)) {
			DeleterConstruct(this, ptr);
		}
		template<class D>
		SharedPtr(nullptr_t, D deleter) noexcept : m_ptr(nullptr), m_count(nullptr, System::move(deleter)) {}
		template<class Y, class D, class A>
		SharedPtr(Y* ptr, D deleter, A allocator) noexcept : m_ptr(ptr), m_count(ptr, System::move(deleter), allocator) {
			DeleterConstruct(this, ptr);
		}
		template<class D, class A>
		SharedPtr(nullptr_t, D deleter, A allocator) noexcept : m_ptr(nullptr), m_count(nullptr, System::move(deleter), allocator) {}
		template<class Y>
		SharedPtr(const SharedPtr<Y>& arg, element_type* ptr) noexcept : m_ptr(ptr), m_count(arg.m_count) {}
		template<class Y>
		SharedPtr(SharedPtr<Y>&& arg, element_type* ptr) noexcept : m_ptr(ptr), m_count() {
			m_count.Swap(System::move(arg.m_count));
			arg.m_ptr = nullptr;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		SharedPtr(const SharedPtr<Y>& arg) noexcept : m_ptr(arg.m_ptr), m_count(arg.m_count) {}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		SharedPtr(SharedPtr<Y>&& arg) noexcept : m_ptr(arg.m_ptr), m_count(System::move(arg.m_count)) {
			arg.m_ptr = nullptr;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		explicit SharedPtr(const WeakPtr<Y>& arg) noexcept : m_count(arg.m_count) {
			if (!m_count.Empty()) m_ptr = arg.m_ptr;
		}
		template<class Y, class D> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		SharedPtr(UniquePtr<Y, D>&& arg) noexcept : m_ptr(arg.Get()), m_count() {
			typename UniquePtr<Y, D>::pointer tmp = arg.Get();
			if (tmp) {
				m_count = SharedCount(System::move(arg));
				DeleterConstruct(this, tmp);
			}
		}
	public:
		void Reset() noexcept {
			this_type().Swap(*this);
		}
		template<class Y>
		void Reset(Y* ptr) noexcept {
			if (!ptr || ptr != m_ptr) this_type(ptr).Swap(*this);
		}
		template<class Y, class D>
		void Reset(Y* ptr, D deleter) noexcept {
			this_type(ptr, System::move(deleter)).Swap(*this);
		}
		template<class Y, class D, class A>
		void Reset(Y* ptr, D deleter, A allocator) noexcept {
			this_type(ptr, System::move(deleter), allocator).Swap(*this);
		}
		template<class Y>
		void Reset(const SharedPtr<Y>& shared, element_type* ptr) noexcept {
			this_type(shared, ptr).Swap(*this);
		}
		template<class Y>
		void Reset(SharedPtr<Y>&& shared, element_type* ptr) noexcept {
			this_type(System::move(shared), ptr).Swap(*this);
		}
	public:
		element_type* Get() const noexcept { return m_ptr; }
		int32_t UseCount() const noexcept {
			return m_count.UseCount();
		}
		void Swap(SharedPtr& other) noexcept {
			element_type* tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
			m_count.Swap(other.m_count);
		}
		void swap(SharedPtr& other) noexcept { Swap(other); }
		template<class Y>
		bool OwnerBefore(const SharedPtr<Y>& rhs) const noexcept {
			return m_count < rhs.m_count;
		}
		template<class Y>
		bool OwnerBefore(const WeakPtr<Y>& rhs) const noexcept {
			return m_count < rhs.m_count;
		}
		template<class Y>
		bool OwnerEquals(const SharedPtr<Y>& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		template<class Y>
		bool OwnerEquals(const WeakPtr<Y>& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		size_t OwnerHashValue() const noexcept {
			return m_count.HashValue();
		}
	public: /* private? */
		void* InternalGetDelter(const std::type_info& info) const noexcept {
			return m_count.GetDeleter(info);
		}
		void* InternalGetLocalDelter(const std::type_info& info) const noexcept {
			return m_count.GetLocalDeleter(info);
		}
		void* InternalGetUntypedDelter() const noexcept {
			return m_count.GetUntypedDeleter();
		}
		bool InternalEquiv(const SharedPtr& rhs) const noexcept {
			return m_ptr == rhs.m_ptr && m_count == rhs.m_count;
		}
		SharedCount InternalCount() const noexcept {
			return m_count;
		}
	public:
		explicit operator bool() const noexcept {
			return m_ptr;
		}
		bool operator!() const noexcept {
			return !m_ptr;
		}
	public:
		SharedPtr& operator=(const SharedPtr& rhs) noexcept {
			this_type(rhs).Swap(*this);
			return *this;
		}
		SharedPtr& operator=(SharedPtr&& rhs) noexcept {
			this_type(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y>
		SharedPtr& operator=(const SharedPtr<Y>& rhs) noexcept {
			this_type(rhs).Swap(*this);
			return *this;
		}
		template<class Y>
		SharedPtr& operator=(SharedPtr<Y>&& rhs) noexcept {
			this_type(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y, class D> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		SharedPtr& operator=(UniquePtr<Y, D> rhs) noexcept {
			typename UniquePtr<Y, D>::pointer ptr = rhs.Get();
			SharedPtr tmp;
			if (ptr){
				tmp.m_ptr = ptr;
				tmp.m_count = SharedCount(rhs);
				DeleterConstruct(&tmp, ptr);
			}
			tmp.Swap(*this);
			return *this;
		}
		SharedPtr& operator=(nullptr_t) noexcept {
			this_type().Swap(*this);
			return *this;
		}
		Traits::dereference_return_t<T> operator*() const noexcept requires(Traits::Concepts::CNotArray<T> && Traits::Concepts::CNotVoid<T>) {
			return *m_ptr;
		}
		Traits::member_access_return_t<T> operator->() const noexcept requires(Traits::Concepts::CNotArray<T>) {
			return m_ptr;
		}
		Traits::array_access_return_t<T> operator[](ptrdiff_t index) const noexcept requires(Traits::Concepts::CArray<T>) {
			return static_cast<Traits::array_access_return_t<T>>(m_ptr[index]);
		}
	};
	template<class T>
	SharedPtr(WeakPtr<T>)->SharedPtr<T>;
	template<class T, class D>
	SharedPtr(UniquePtr<T, D>)->SharedPtr<T>;

	template<class T, class U>
	inline strong_ordering operator<=>(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) noexcept {
		return static_cast<void*>(lhs.Get()) <=> static_cast<void*>(rhs.Get());
	}
	template<class T, class U>
	inline strong_ordering operator<=>(const SharedPtr<T>& lhs, nullptr_t) noexcept {
		return static_cast<void*>(lhs.Get()) <=> static_cast<void*>(nullptr);
	}

	template<class T>
	inline void Swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept {
		lhs.Swap(rhs);
	}
	template<class T>
	inline void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept {
		Swap(lhs, rhs);
	}
	template<class T, class U>
	SharedPtr<T> static_pointer_cast(const SharedPtr<U>& shared) noexcept {
		(void)static_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = static_cast<element_t*>(shared.Get());
		return SharedPtr<T>(shared, p);
	}
	template<class T, class U>
	SharedPtr<T> const_pointer_cast(const SharedPtr<U>& shared) noexcept {
		(void)const_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = const_cast<element_t*>(shared.Get());
		return SharedPtr<T>(shared, p);
	}
	template<class T, class U>
	SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U>& shared) noexcept {
		(void)dynamic_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = dynamic_cast<element_t*>(shared.Get());
		return p ? SharedPtr<T>(shared, p) : SharedPtr<T>();
	}
	template<class T, class U>
	SharedPtr<T> reinterpret_pointer_cast(const SharedPtr<U>& shared) noexcept {
		(void)reinterpret_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = reinterpret_cast<element_t*>(shared.Get());
		return SharedPtr<T>(shared, p);
	}
	template<class T, class U>
	SharedPtr<T> static_pointer_cast(SharedPtr<U>&& shared) noexcept {
		(void)static_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = static_cast<element_t*>(shared.Get());
		return SharedPtr<T>(System::move(shared), p);
	}
	template<class T, class U>
	SharedPtr<T> const_pointer_cast(SharedPtr<U>&& shared) noexcept {
		(void)const_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = const_cast<element_t*>(shared.Get());
		return SharedPtr<T>(System::move(shared), p);
	}
	template<class T, class U>
	SharedPtr<T> dynamic_pointer_cast(SharedPtr<U>&& shared) noexcept {
		(void)dynamic_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = dynamic_cast<element_t*>(shared.Get());
		return p ? SharedPtr<T>(System::move(shared), p) : SharedPtr<T>();
	}
	template<class T, class U>
	SharedPtr<T> reinterpret_pointer_cast(SharedPtr<U>&& shared) noexcept {
		(void)reinterpret_cast<T*>(static_cast<U*>(nullptr));
		using element_t = SharedPtr<T>::element_type;
		element_t* p = reinterpret_cast<element_t*>(shared.Get());
		return SharedPtr<T>(System::move(shared), p);
	}
	template<class T>
	inline typename SharedPtr<T>::element_type* GetPointer(const SharedPtr<T>& shared) noexcept {
		return shared.Get();
	}
	template<class T, class U, class V>
	std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os, const SharedPtr<T>& shared) noexcept {
		os << shared.Get();
		return os;
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
	protected:
		element_type* m_ptr;
		WeakCount m_count;
	public:
		constexpr WeakPtr() noexcept : m_ptr(nullptr), m_count() {}
		WeakPtr(const WeakPtr& arg) noexcept : m_ptr(arg.m_ptr), m_count(arg.m_count) {}
		WeakPtr(WeakPtr&& arg) noexcept : m_ptr(arg.m_ptr), m_count(System::move(arg.m_count)) {
			arg.m_ptr = nullptr;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr(const WeakPtr<Y>& arg) noexcept : m_ptr(arg.Lock().Get()), m_count(arg.m_count) {}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr(WeakPtr<Y>&& arg) noexcept : m_ptr(arg.Lock().Get()), m_count(System::move(arg.m_count)) {
			arg.m_ptr = nullptr;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr(const SharedPtr<Y>& arg) noexcept : m_ptr(arg.m_ptr), m_count(arg.m_count) {}
		template<class Y>
		WeakPtr(const WeakPtr<Y>& arg, element_type* ptr) noexcept : m_ptr(ptr), m_count(arg.m_count) {}
		template<class Y>
		WeakPtr(const SharedPtr<Y>& arg, element_type* ptr) noexcept : m_ptr(ptr), m_count(arg.m_count) {}
		template<class Y>
		WeakPtr(WeakPtr<Y>&& arg, element_type* ptr) noexcept : m_ptr(ptr), m_count(System::move(arg.m_count)) {}
	public:
		SharedPtr<T> Lock() const noexcept { return SharedPtr<T>(*this); }
		int32_t UseCount() const noexcept { return m_count.UseCount(); }
		bool Expired() const noexcept { return !m_count.UseCount(); }
		void Reset() noexcept { this_type().Swap(*this); }
		void Swap(this_type& other) noexcept {
			element_type* tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
			m_count.Swap(other.m_count);
		}
		void swap(this_type& other) noexcept { Swap(other); }
		template<class Y>
		bool OwnerBefore(const WeakPtr<Y>& rhs) const noexcept {
			return m_count < rhs.m_count;
		}
		template<class Y>
		bool OwnerBefore(const SharedPtr<Y>& rhs) const noexcept {
			return m_count < rhs.m_count;
		}
		template<class Y>
		bool OwnerEquals(const WeakPtr<Y>& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		template<class Y>
		bool OwnerEquals(const SharedPtr<Y>& rhs) const noexcept {
			return m_count == rhs.m_count;
		}
		size_t OwnerHashValue() const noexcept {
			return m_count.HashValue();
		}
	public:
		WeakPtr& operator=(const WeakPtr& rhs) noexcept {
			if (this == &rhs) return *this;
			m_ptr = rhs.m_ptr;
			m_count = rhs.m_count;
			return *this;
		}
		WeakPtr& operator=(WeakPtr&& rhs) noexcept {
			if (this == &rhs) return *this;
			this_type(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr& operator=(const WeakPtr<Y>& rhs) noexcept {
			m_ptr = rhs.Lock().Get();
			m_count = rhs.m_count;
			return *this;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr& operator=(WeakPtr<Y>&& rhs) noexcept {
			this_type(System::move(rhs)).Swap(*this);
			return *this;
		}
		template<class Y> requires Traits::Concepts::CAssignableFrom<T*, Y*>
		WeakPtr& operator=(const SharedPtr<Y>& rhs) noexcept {
			m_ptr = rhs.m_ptr;
			m_count = rhs.m_count;
			return *this;
		}
	};
	template<class T>
	WeakPtr(SharedPtr<T>)->WeakPtr<T>;

	template<class T, class U>
	inline strong_ordering operator<=>(const WeakPtr<T>& lhs, const WeakPtr<U>& rhs) noexcept {
		return static_cast<void*>(lhs.Get()) <=> static_cast<void*>(rhs.Get());
	}
	template<class T, class U>
	inline strong_ordering operator<=>(const WeakPtr<T>& lhs, nullptr_t) noexcept {
		return static_cast<void*>(lhs.Get()) <=> static_cast<void*>(nullptr);
	}
	template<class T>
	inline void Swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept {
		lhs.Swap(rhs);
	}
	template<class T>
	inline void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept {
		Swap(lhs, rhs);
	}
	template<class T>
	size_t HashValue(const WeakPtr<T>& weak) noexcept {
		return weak.OwnerHashValue();
	}
}

//MakeShared
export namespace System {
	template<Traits::Concepts::CNotArray T, class ...Args>
	SharedPtr<T> MakeShared(Args&& ...args) noexcept {
		Allocator<T> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		T* ptr = traits::allocate(alloc, 1);
		traits::construct(alloc, ptr, System::move(args)...);
		return SharedPtr<T>(ptr);
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	SharedPtr<T> MakeShared(size_t N) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<T>;
		const size_t all_count = multiple_count * N;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				traits::construct(alloc, p++);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeShared() noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<Traits::remove_extent_t<T>>;
		constexpr size_t all_count = Traits::element_count_v<T>;
		constexpr size_t N = all_count / multiple_count;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				traits::construct(alloc, p++);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	SharedPtr<T> MakeShared(size_t N, const Traits::remove_extent_t<T>& u) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<T>;
		const size_t all_count = multiple_count * N;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		const type* init = nullptr;
		if constexpr (Traits::Concepts::CArray<Traits::remove_extent_t<T>>) init = reinterpret_cast<const type*>(&u[0]);
		else init = &u;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				traits::construct(alloc, p++, init[j]);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeShared(const Traits::remove_extent_t<T>& u) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<Traits::remove_extent_t<T>>;
		constexpr size_t all_count = Traits::element_count_v<T>;
		constexpr size_t N = all_count / multiple_count;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		const type* init = nullptr;
		if constexpr (Traits::Concepts::CArray<Traits::remove_extent_t<T>>) init = reinterpret_cast<const type*>(&u[0]);
		else init = &u;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				traits::construct(alloc, p++, init[j]);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}

	template<Traits::Concepts::CNotArray T>
	SharedPtr<T> MakeSharedForOverwrite() noexcept {
		Allocator<T> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		T* ptr = traits::allocate(alloc, 1);
		System::construct_default(ptr);
		return SharedPtr<T>(ptr);
	}
	template<Traits::Concepts::CKnownBoundArray T>
	SharedPtr<T> MakeSharedForOverwrite() noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<Traits::remove_extent_t<T>>;
		constexpr size_t all_count = Traits::element_count_v<T>;
		constexpr size_t N = all_count / multiple_count;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				System::construct_default(p++);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	SharedPtr<T> MakeSharedForOverwrite(size_t N) noexcept {
		using type = Traits::remove_all_extents_t<T>;
		Allocator<type> alloc;
		using traits = AllocatorTraits<decltype(alloc)>;
		constexpr size_t multiple_count = Traits::element_count_v<T>;
		const size_t all_count = multiple_count * N;
		type* ptr = traits::allocate(alloc, all_count);
		type* p = ptr;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < multiple_count; ++j) {
				System::construct_default(p++);
			}
		}
		return SharedPtr<T>(reinterpret_cast<Traits::remove_extent_t<T>*>(ptr));
	}
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
