export module UniquePtr;
import CSTDINT;
import Traits;
import Allocator;
//import <iostream>;	//overload operator<<
import <functional>;	//specialization hash<>

namespace System::Internal {
	template<class T, class D>
	struct UniquePtrPointerType {
		using type = T*;
	};
	template<class T, class D>
	requires requires() { typename D::pointer; }
	struct UniquePtrPointerType<T, D> {
		using type = typename D::pointer;
	};
}

//UniquePtr
export namespace System {
	template<class T, class D = DefaultDelete<T>>
	class UniquePtr {
	public:
		using element_type = T;
		using deleter_type = Traits::remove_cvref_t<D>;
		using pointer = typename Internal::UniquePtrPointerType<element_type, deleter_type>::type;
	private:
		static constexpr bool NonReferenceDeleter = !Traits::Concepts::CReference<D>;
		static constexpr bool LValueReferenceDeleter = Traits::Concepts::CLValueReference<D>;
		static constexpr bool RValueReferenceDeleter = Traits::Concepts::CRValueReference<D>;
		static constexpr bool EnabledMoveAssign = NonReferenceDeleter ? Traits::Concepts::CMoveAssignable<deleter_type> : Traits::Concepts::CCopyAssignable<deleter_type>;
	protected:
		pointer m_ptr = nullptr;
		D m_deleter;
	public:
		constexpr UniquePtr() noexcept : m_deleter() {}
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr(UniquePtr&& arg) noexcept : m_ptr(arg.m_ptr), m_deleter(System::move(arg.m_deleter)) {
			arg.m_ptr = nullptr;
		}
		constexpr UniquePtr(nullptr_t) noexcept : m_deleter() {}
		constexpr UniquePtr(pointer ptr) noexcept : m_ptr(ptr), m_deleter() {}
		template<Traits::Concepts::CNotArray U, class E>
		constexpr UniquePtr(UniquePtr<U, E>&& arg) noexcept : m_ptr(arg.m_ptr), m_deleter(System::move(arg.m_deleter)) {
			arg.m_ptr = nullptr;
		}
		constexpr ~UniquePtr() noexcept { Reset(); }
	public:
		constexpr UniquePtr(pointer ptr, deleter_type const& deleter) noexcept requires(NonReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(NonReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
	public:
		constexpr UniquePtr(pointer ptr, deleter_type& deleter) noexcept requires(LValueReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(LValueReferenceDeleter) = delete;
	public:
		constexpr UniquePtr(pointer ptr, deleter_type const& deleter) noexcept requires(RValueReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type const&& deleter) noexcept requires(RValueReferenceDeleter) = delete;
	public:
		constexpr pointer Release() noexcept {
			pointer ret = m_ptr;
			m_ptr = nullptr;
			return ret;
		}
		constexpr void Reset(pointer ptr = pointer()) noexcept {
			pointer old = m_ptr;
			m_ptr = ptr;
			if (old) m_deleter(old);
		}
		constexpr void Swap(UniquePtr& other) noexcept {
			pointer tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
			deleter_type del = System::move(other.m_deleter);
			other.m_deleter = System::move(m_deleter);
			m_deleter = System::move(del);
		}
		constexpr pointer Get() const noexcept { return m_ptr; }
		constexpr deleter_type& GetDeleter() noexcept {
			return m_deleter;
		}
		constexpr deleter_type const& GetDeleter() const noexcept {
			return m_deleter;
		}
	public:
		constexpr Traits::add_lvalue_reference_t<T> operator*() const noexcept {
			return *m_ptr;
		}
		constexpr pointer operator->() const noexcept {
			return m_ptr;
		}
		explicit constexpr operator bool() const noexcept { return m_ptr; }
		constexpr UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr& operator=(UniquePtr&& rhs) noexcept requires(EnabledMoveAssign) {
			if (this == &rhs) return *this;
			Reset(rhs.Release());
			GetDeleter() = System::move(rhs.GetDeleter());
			return *this;
		}
		constexpr UniquePtr& operator=(nullptr_t) noexcept {
			Reset();
			return *this;
		}
		template<Traits::Concepts::CNotArray U, Traits::Concepts::CMoveAssignableTo<deleter_type> E>
		constexpr UniquePtr& operator=(UniquePtr<U, E>&& rhs) noexcept {
			Reset(rhs.Release());
			GetDeleter() = System::move(rhs.GetDeleter());
			return *this;
		}
	};

	template<class T, class D>
	class UniquePtr<T[], D> {
	public:
		using element_type = T;
		using deleter_type = Traits::remove_cvref_t<D>;
		using pointer = typename Internal::UniquePtrPointerType<element_type, deleter_type>::type;
	private:
		static constexpr bool NonReferenceDeleter = !Traits::Concepts::CReference<D>;
		static constexpr bool LValueReferenceDeleter = Traits::Concepts::CLValueReference<D>;
		static constexpr bool RValueReferenceDeleter = Traits::Concepts::CRValueReference<D>;
		static constexpr bool EnabledMoveAssign = NonReferenceDeleter ? Traits::Concepts::CMoveAssignable<deleter_type> : Traits::Concepts::CCopyAssignable<deleter_type>;
	protected:
		pointer m_ptr = nullptr;
		D m_deleter;
	public:
		constexpr UniquePtr() noexcept : m_deleter() {}
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr(UniquePtr&& arg) noexcept : m_ptr(arg.m_ptr), m_deleter(System::move(arg.m_deleter)) {
			arg.m_ptr = nullptr;
		}
		template<class U, class E>
		constexpr UniquePtr(UniquePtr<U, E>&& arg) noexcept : m_ptr(arg.m_ptr), m_deleter(System::move(arg.m_deleter)) {
			arg.m_ptr = nullptr;
		}
		constexpr UniquePtr(nullptr_t) noexcept : m_deleter() {}
		constexpr UniquePtr(pointer ptr) noexcept : m_ptr(ptr), m_deleter() {}
		constexpr ~UniquePtr() noexcept { Reset(); }
	public:
		constexpr UniquePtr(pointer ptr, deleter_type const& deleter) noexcept requires(NonReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(NonReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
	public:
		constexpr UniquePtr(pointer ptr, deleter_type& deleter) noexcept requires(LValueReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(LValueReferenceDeleter) = delete;
	public:
		constexpr UniquePtr(pointer ptr, deleter_type const& deleter) noexcept requires(RValueReferenceDeleter)
			: m_ptr(ptr), m_deleter(System::move(deleter)) {}
		constexpr UniquePtr(pointer ptr, deleter_type const&& deleter) noexcept requires(RValueReferenceDeleter) = delete;
	public:
		constexpr pointer Release() noexcept {
			pointer ret = m_ptr;
			m_ptr = nullptr;
			return ret;
		}
		template<class U>
		constexpr void Reset(U ptr) noexcept {
			pointer old = m_ptr;
			m_ptr = ptr;
			if (old) GetDeleter()(old);
		}
		constexpr void Reset(nullptr_t = nullptr) noexcept {
			pointer old = m_ptr;
			m_ptr = nullptr;
			if (old) GetDeleter()(old);
		}
		constexpr void Swap(UniquePtr& other) noexcept {
			pointer tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
			deleter_type del = System::move(other.m_deleter);
			other.m_deleter = System::move(m_deleter);
			m_deleter = System::move(del);
		}
		constexpr pointer Get() const noexcept { return m_ptr; }
		constexpr deleter_type& GetDeleter() noexcept {
			return m_deleter;
		}
		constexpr deleter_type const& GetDeleter() const noexcept {
			return m_deleter;
		}
	public:
		constexpr T& operator[](size_t index) const {
			return m_ptr[index];
		}
		explicit constexpr operator bool() const noexcept { return m_ptr; }
		constexpr UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr& operator=(UniquePtr&& rhs) noexcept requires(EnabledMoveAssign) {
			if (this == &rhs) return *this;
			Reset(rhs.Release());
			GetDeleter() = System::move(rhs.GetDeleter());
			return *this;
		}
		constexpr UniquePtr& operator=(nullptr_t) noexcept {
			Reset();
			return *this;
		}
		template<Traits::Concepts::CArray U, Traits::Concepts::CMoveAssignableTo<deleter_type> E>
		constexpr UniquePtr& operator=(UniquePtr<U, E>&& rhs) noexcept {
			Reset(rhs.Release());
			GetDeleter() = System::move(rhs.GetDeleter());
			return *this;
		}
	};

	template<class T, CDefaultDeleter<T> D>
	class UniquePtr<T, D> {
	public:
		using element_type = T;
		using deleter_type = D;
		using pointer = typename Internal::UniquePtrPointerType<element_type, deleter_type>::type;
	protected:
		pointer m_ptr = nullptr;
	public:
		constexpr UniquePtr() noexcept {}
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr(UniquePtr&& arg) noexcept : m_ptr(arg.m_ptr) {
			arg.m_ptr = nullptr;
		}
		constexpr UniquePtr(nullptr_t) noexcept {}
		constexpr UniquePtr(pointer ptr) noexcept : m_ptr(ptr) {}
		constexpr ~UniquePtr() noexcept { Reset(); }
	public:
		constexpr pointer Release() noexcept {
			pointer ret = m_ptr;
			m_ptr = nullptr;
			return ret;
		}
		constexpr void Reset(pointer ptr = pointer()) noexcept {
			pointer old = m_ptr;
			m_ptr = ptr;
			if (old) deleter_type()(old);
		}
		constexpr void Swap(UniquePtr& other) noexcept {
			pointer tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
		}
		constexpr pointer Get() const noexcept { return m_ptr; }
	public:
		constexpr Traits::add_lvalue_reference_t<T> operator*() const noexcept {
			return *m_ptr;
		}
		constexpr pointer operator->() const noexcept {
			return m_ptr;
		}
		explicit constexpr operator bool() const noexcept { return m_ptr; }
		constexpr UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr& operator=(UniquePtr&& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset(rhs.Release());
			return *this;
		}
		constexpr UniquePtr& operator=(nullptr_t) noexcept {
			Reset();
			return *this;
		}
	};

	template<class T, CDefaultDeleter<T[]> D>
	class UniquePtr<T[], D> {
	public:
		using element_type = T;
		using deleter_type = D;
		using pointer = typename Internal::UniquePtrPointerType<element_type, deleter_type>::type;
	protected:
		pointer m_ptr = nullptr;
	public:
		constexpr UniquePtr() noexcept {}
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr(UniquePtr&& arg) noexcept : m_ptr(arg.m_ptr) {
			arg.m_ptr = nullptr;
		}
		template<class U>
		constexpr UniquePtr(UniquePtr<U>&& arg) noexcept : m_ptr(arg.m_ptr) {
			arg.m_ptr = nullptr;
		}
		constexpr UniquePtr(nullptr_t) noexcept {}
		template<class U>
		constexpr UniquePtr(U ptr) noexcept : m_ptr(ptr) {}
		constexpr ~UniquePtr() noexcept { Reset(); }
	public:
		constexpr pointer Release() noexcept {
			pointer ret = m_ptr;
			m_ptr = nullptr;
			return ret;
		}
		template<class U>
		constexpr void Reset(U ptr) noexcept {
			pointer old = m_ptr;
			m_ptr = ptr;
			if (old) deleter_type()(old);
		}
		constexpr void Reset(nullptr_t = nullptr) noexcept {
			pointer old = m_ptr;
			m_ptr = nullptr;
			if (old) deleter_type()(old);
		}
		constexpr void Swap(UniquePtr& other) noexcept {
			pointer tmp = other.m_ptr;
			other.m_ptr = m_ptr;
			m_ptr = tmp;
		}
		constexpr pointer Get() const noexcept { return m_ptr; }
	public:
		constexpr T& operator[](size_t index) const {
			return m_ptr[index];
		}
		constexpr operator bool() const noexcept { return m_ptr; }
		constexpr UniquePtr& operator=(const UniquePtr&) noexcept = delete;
		constexpr UniquePtr& operator=(UniquePtr&& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset(rhs.Release());
			return *this;
		}
		constexpr UniquePtr& operator=(nullptr_t) noexcept {
			Reset();
			return *this;
		}
	};
}

//MakeUnique
export namespace System {
	template<Traits::Concepts::CNotArray T, class ...Args>
	constexpr UniquePtr<T> MakeUnique(Args&& ...args) {
		T* ptr = DefaultNew<T>()(System::move(args)...);
		return UniquePtr<T>(ptr);
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	requires Traits::Concepts::CDefaultConstructible<Traits::remove_all_extents_t<T>>
	constexpr UniquePtr<T> MakeUnique(size_t N) {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T>()(N);
		return UniquePtr<T>(ptr);
	}
	template<Traits::Concepts::CKnownBoundArray T, class ...Args>
	constexpr auto MakeUnique(Args&& ...args) = delete;

	template<Traits::Concepts::CNotArray T>
	constexpr UniquePtr<T> MakeUniqueForOverwrite() {
		T* ptr = DefaultNew<T, true>()();
		return UniquePtr<T>(ptr);
	}
	template<Traits::Concepts::CUnknownBoundArray T>
	constexpr UniquePtr<T> MakeUniqueForOverwrite(size_t N) {
		using type = Traits::remove_all_extents_t<T>;
		type* ptr = DefaultNew<T, true>()(N);
		return UniquePtr<T>(ptr);
	}
	template<Traits::Concepts::CKnownBoundArray T, class ...Args>
	constexpr auto MakeUniqueForOverwrite(Args&& ...args) = delete;
}

using namespace System;
//overload global operator
export {
	template<class T1, class D1, class T2, class D2>
	constexpr strong_ordering operator<=>(const UniquePtr<T1, D1>& lhs, const UniquePtr<T2, D2>& rhs) noexcept {
		return lhs.Get() <=> rhs.Get();
	}
	template<class T, class D>
	constexpr strong_ordering operator<=>(const UniquePtr<T, D>& lhs, nullptr_t) noexcept {
		return lhs.Get() <=> static_cast<UniquePtr<T, D>::element_type*>(nullptr);
	}
	// template<class CharT, class Traits, class Y, class D>
	// std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const UniquePtr<Y, D>& ptr) noexcept {
	// 	return (os << ptr.Get());
	// }
}

//std overload and specialization
export namespace std {
	template<class T, class D>
	constexpr void swap(UniquePtr<T, D>& lhs, UniquePtr<T, D>& rhs) noexcept {
		lhs.Swap(rhs);
	}
	template<class T, class D>
	struct hash<UniquePtr<T, D>> {
		size_t operator()(const UniquePtr<T, D>& arg) const noexcept {
			return hash<typename UniquePtr<T, D>::element_type*>()(arg.Get());
		}
	};
}
