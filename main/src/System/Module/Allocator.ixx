export module Allocator;
import CSTDINT;
import Traits;
export import <new>;	//std::align_val_t

//construct_at/destroy_at
export namespace System {
	template<class T, class ...Args>
	requires requires(T* p, Args&& ...args) {
//		::new (static_cast<void*>(p)) T(System::move(args)...);
		Traits::true_v<T, Args...>;
	}
	constexpr T* construct_at(T* p, Args&& ...args) {
		return ::new (static_cast<void*>(p)) T(System::move(args)...);
	}
	template<class T>
	constexpr void destroy_at(T* p) requires (!Traits::Concepts::CArray<T>) {
		p->~T();
	}
	template<Traits::Concepts::CForwardIterator Ite>
	constexpr void destroy(Ite first, Ite last) {
		for (; first != last; ++first) System::destroy_at(System::addressof(*first));
	}
	template<class U, size_t N>
	constexpr void destroy_at(U(*p)[N]) {
		System::destroy(static_cast<U*>(p + 0), static_cast<U*>(p + N));
	}

	template<class T>
	constexpr T* construct_default(T* p) {
		return ::new (static_cast<void*>(p)) T;
	}
}

//Allocator
export namespace System {
	template<class T>
	class Allocator {
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = Traits::true_type;
	public:
		constexpr Allocator() noexcept {}
		constexpr Allocator(const Allocator& arg) noexcept {}
		template<class U>
		constexpr Allocator(const Allocator<U>& arg) noexcept {}
		constexpr ~Allocator() noexcept {}
	public:
		[[nodiscard]]
		constexpr T* Allocate(size_t n) const {
			void* ret = ::operator new(n * sizeof(T), static_cast<std::align_val_t>(alignof(T)));
			return static_cast<T*>(ret);
		}
		constexpr void Deallocate(T* p, size_t n) const {
			//clangでは直接呼ぶことはできない
			//::operator delete(static_cast<void*>(p), n * sizeof(T), static_cast<std::align_val_t>(alignof(T)));
			::operator delete(static_cast<void*>(p), static_cast<std::align_val_t>(alignof(T)));
		}
	public: /* std::allocator互換 */
		[[nodiscard]]
		constexpr T* allocate(size_t n) const { return Allocate(n); }
		constexpr void deallocate(T* p, size_t n) const { return Deallocate(p, n); }
	public: /* オリジナル機能 */
		constexpr void DeallocateUnknownSize(T* p) const {
			::operator delete(static_cast<void*>(p), static_cast<std::align_val_t>(alignof(T)));
		}
	};
}

namespace System::Internal {
	template<class T, class V>
	struct PointerType { using type = V*; };
	template<class T, class V>
	requires requires() { typename T::pointer; }
	struct PointerType<T, V> { using type = T::pointer; };

	template<class T, class P, class V>
	struct ConstPointerType { using type = Traits::pointer_traits<P>::template rebind<const V>; };
	template<class T, class P, class V>
	requires requires() { typename T::const_pointer; }
	struct ConstPointerType<T, P, V> { using type = T::const_pointer; };

	template<class T, class P>
	struct VoidPointerType { using type = Traits::pointer_traits<P>::template rebind<void>; };
	template<class T, class P>
	requires requires() { typename T::void_pointer; }
	struct VoidPointerType<T, P> { using type = T::void_pointer; };

	template<class T, class P>
	struct ConstVoidPointerType { using type = Traits::pointer_traits<P>::template rebind<const void>; };
	template<class T, class P>
	requires requires() { typename T::const_void_pointer; }
	struct ConstVoidPointerType<T, P> { using type = T::const_void_pointer; };

	template<class T, class P>
	struct DifferenceType { using type = Traits::pointer_traits<P>::difference_type; };
	template<class T, class P>
	requires requires() { typename T::difference_type; }
	struct DifferenceType<T, P> { using type = T::difference_type; };

	template<class T, class D>
	struct SizeType { using type = Traits::make_unsigned_t<D>; };
	template<class T, class D>
	requires requires() { typename T::size_type; }
	struct SizeType<T, D> { using type = T::size_type; };

	template<class T>
	struct PropagateOnContainerCopyAssignType { using type = Traits::false_type; };
	template<class T>
	requires requires() { typename T::propagate_on_container_copy_assignment; }
	struct PropagateOnContainerCopyAssignType<T> { using type = T::propagate_on_container_copy_assignment; };

	template<class T>
	struct PropagateOnContainerMoveAssignType { using type = Traits::false_type; };
	template<class T>
	requires requires() { typename T::propagate_on_container_move_assignment; }
	struct PropagateOnContainerMoveAssignType<T> { using type = T::propagate_on_container_move_assignment; };

	template<class T>
	struct PropagateOnContainerSwapType { using type = Traits::false_type; };
	template<class T>
	requires requires() { typename T::propagate_on_container_swap; }
	struct PropagateOnContainerSwapType<T> { using type = T::propagate_on_container_swap; };

	template<class T>
	struct IsAlwaysEqualType { using type = typename Traits::is_empty<T>::type; };
	template<class T>
	requires requires() { typename T::is_always_equal; }
	struct IsAlwaysEqualType<T> { using type = T::is_always_equal; };

	template<class Alloc, class T> struct RebindAllocType;
	template<class Alloc, class T>
	requires requires() { typename Alloc::template rebind<T>::other; }
	struct RebindAllocType<Alloc, T> {
		using type = Alloc::template rebind<T>::ohter;
	};
	template<template<class U, class ...Args> class Alloc, class T, class U, class ...Args>
	struct RebindAllocType<Alloc<U, Args...>, T> {
		using type = Alloc<T, Args...>;
	};

	template<class Alloc, class T, class ...Args>
	concept CHasConstruct = requires(Alloc& alloc, T* p, Args&& ...args) {
		alloc.construct(p, System::move(args)...);
	};
	template<class Alloc, class T>
	concept CHasDestroy = requires(Alloc& alloc, T* p) {
		alloc.destroy(p);
	};
	template<class Alloc>
	concept CHasMaxSize = requires(Alloc& alloc) {
		alloc.max_size();
	};
	template<class Alloc>
	concept CHasSelectOnContainerCopyConstruction = requires(const Alloc& alloc) {
		alloc.select_on_container_copy_construction();
	};

	template<class Alloc>
	concept CHasDeallocateUnknownSize = requires(Alloc& alloc) {
		alloc.DeallocateUnknownSize();
	};

}

//AllocatorTraits
export namespace System {
	template<class Alloc>
	struct AllocatorTraits {
		using allocator_type = Alloc;
		using value_type = Alloc::value_type;
		using pointer = typename Internal::PointerType<Alloc, value_type>::type;
		using const_pointer = typename Internal::ConstPointerType<Alloc, pointer, value_type>::type;
		using void_pointer = typename Internal::VoidPointerType<Alloc, pointer>::type;
		using const_void_pointer = typename Internal::ConstVoidPointerType<Alloc, pointer>::type;
		using difference_type = typename Internal::DifferenceType<Alloc, pointer>::type;
		using size_type = typename Internal::SizeType<Alloc, difference_type>::type;
		using propagate_on_container_copy_assignment = typename Internal::PropagateOnContainerCopyAssignType<Alloc>::type;
		using propagate_on_container_move_assignment = typename Internal::PropagateOnContainerMoveAssignType<Alloc>::type;
		using propagate_on_container_swap = typename Internal::PropagateOnContainerSwapType<Alloc>::type;
		using is_always_equal = typename Internal::IsAlwaysEqualType<Alloc>::type;
	public:
		template<class T>
		using rebind_alloc = typename Internal::RebindAllocType<Alloc, T>::type;
		template<class T>
		using rebind_traits = AllocatorTraits<rebind_alloc<T>>;
	public:
		[[nodiscard]]
		static constexpr pointer Allocate(Alloc& alloc, size_type n) {
			return alloc.allocate(n);
		}
		[[nodiscard]]
		static constexpr pointer Allocate(Alloc& alloc, size_type n, const_void_pointer hint) {
			return alloc.allocate(n);
		}
		static constexpr void Deallocate(Alloc& alloc, pointer p, size_type n) {
			alloc.deallocate(p, n);
		}
		template<class T, class ...Args>
		static constexpr void Construct(Alloc& alloc, T* p, Args&& ...args) requires (Internal::CHasConstruct<Alloc, T, Args...>) {
			alloc.construct(p, System::move(args)...);
		}
		template<class T, class ...Args>
		static constexpr void Construct(Alloc& alloc, T* p, Args&& ...args) requires (!Internal::CHasConstruct<Alloc, T, Args...>) {
			System::construct_at(p, System::move(args)...);
		}
		template<class T>
		static constexpr void Destroy(Alloc& alloc, T* p) requires(Internal::CHasDestroy<Alloc, T>) {
			alloc.destroy(p);
		}
		template<class T>
		static constexpr void Destroy(Alloc& alloc, T* p) requires(!Internal::CHasDestroy<Alloc, T>) {
			System::destroy_at(p);
		}
		static constexpr size_type MaxSize(const Alloc& alloc) noexcept requires(Internal::CHasMaxSize<Alloc>) {
			return alloc.max_size();
		}
		static constexpr size_type MaxSize(const Alloc& alloc) noexcept requires(!Internal::CHasMaxSize<Alloc>) {
			return System::MAX_VALUE<size_type> / sizeof(value_type);
		}
		static constexpr Alloc SelectOnContainerCopyConstruction(const Alloc& alloc) requires(Internal::CHasSelectOnContainerCopyConstruction<Alloc>) {
			return alloc.select_on_container_copy_construction();
		}
		static constexpr Alloc SelectOnContainerCopyConstruction(const Alloc& alloc) requires(!Internal::CHasSelectOnContainerCopyConstruction<Alloc>) {
			return alloc;
		}
	public: /* std::allocator_traits互換 */
		[[nodiscard]]
		static constexpr pointer allocate(Alloc& alloc, size_type n) { return Allocate(alloc, n); }
		[[nodiscard]]
		static constexpr pointer allocate(Alloc& alloc, size_type n, const_void_pointer hint) { return Allocate(alloc, n, hint); }
		static constexpr void deallocate(Alloc& alloc, pointer p, size_type n) { Deallocate(alloc, p, n); }
		template<class T, class ...Args>
		static constexpr void construct(Alloc& alloc, T* p, Args&& ...args) { Construct(alloc, p, System::move(args)...); }
		template<class T>
		static constexpr void destroy(Alloc& alloc, T* p) { Destroy(alloc, p); }
		static constexpr size_type max_size(const Alloc& alloc) noexcept { return MaxSize(alloc); }
		static constexpr Alloc select_on_container_copy_construction(const Alloc& alloc) { return SelectOnContainerCopyConstruction(alloc); }
	public: /* オリジナル機能 */
		static constexpr void DeallocateUnknownSize(Alloc& alloc, pointer p) requires(Internal::CHasDeallocateUnknownSize<Alloc>) {
			alloc.DeallocateUnknownSize(p);
		}
		template<class T>
		static constexpr void ConstructDefault(Alloc& alloc, T* p) {
			System::construct_default(p);
		}
	};
}

//DefaultNew/DefaultDelete
export namespace System {
	template<class T>
	struct DefaultDelete {
		constexpr DefaultDelete() noexcept = default;
		template<class U>
		constexpr DefaultDelete(const DefaultDelete<U>&) noexcept {}
	public:
		constexpr void operator()(T* ptr) const { delete ptr; }
	};
	template<class T>
	struct DefaultDelete<T[]> {
		using element_type = Traits::remove_all_extents_t<T[]>;
	public:
		constexpr DefaultDelete() noexcept = default;
		template<class U>
		constexpr DefaultDelete(const DefaultDelete<U[]>&) noexcept {}
	public:
		template<class U>
		requires Traits::Concepts::CAssignableTo<U*, element_type*>
		constexpr void operator()(U* ptr) const { delete[] static_cast<element_type*>(ptr); }
	};
	template<class T, size_t N>
	struct DefaultDelete<T[N]> {
		using element_type = Traits::remove_all_extents_t<T[N]>;
	public:
		constexpr DefaultDelete() noexcept = default;
		template<class U>
		constexpr DefaultDelete(const DefaultDelete<U[N]>&) noexcept {}
	public:
		template<class U>
		requires Traits::Concepts::CAssignableTo<U*, element_type*>
		constexpr void operator()(U* ptr) const { delete[] static_cast<element_type*>(ptr); }
	};

	template<class T, bool DefaultInitialize = false>
	struct DefaultNew {
		template<class ...Args>
		constexpr T* operator()(Args&& ...args) const {
			return new T(System::move(args)...);
		}
	};
	template<class T>
	struct DefaultNew<T, true> {
		constexpr T* operator()() const {
			return new T;
		}
	};
	template<class T>
	struct DefaultNew<T[], false> {
		using element_type = Traits::remove_all_extents_t<T[]>;
		static constexpr size_t element_count = Traits::element_count_v<T[]>;
	public:
		constexpr element_type* operator()(size_t n) const {
			return new element_type[n * element_count]();
		}
	};
	template<class T>
	struct DefaultNew<T[], true> {
		using element_type = Traits::remove_all_extents_t<T[]>;
		static constexpr size_t element_count = Traits::element_count_v<T[]>;
	public:
		constexpr element_type* operator()(size_t n) const {
			return new element_type[n * element_count];
		}
	};
	template<class T, size_t N>
	struct DefaultNew<T[N], false> {
		using element_type = Traits::remove_all_extents_t<T[N]>;
		static constexpr size_t element_count = Traits::element_count_v<T[N]>;
	public:
		constexpr element_type* operator()() const {
			return new element_type[element_count]();
		}
	};
	template<class T, size_t N>
	struct DefaultNew<T[N], true> {
		using element_type = Traits::remove_all_extents_t<T[N]>;
		static constexpr size_t element_count = Traits::element_count_v<T[N]>;
	public:
		constexpr element_type* operator()() const {
			return new element_type[element_count];
		}
	};

	template<class T> struct CopyDelete;
	template<class T>
	struct CopyDelete<T[]> {
		using element_type = Traits::remove_all_extents_t<T[]>;
		static constexpr size_t element_count = Traits::element_count_v<T[]>;
	private:
		size_t m_n;
	public:
		constexpr CopyDelete() noexcept = delete;
		constexpr CopyDelete(const CopyDelete&) noexcept = default;
		constexpr CopyDelete(size_t n) noexcept : m_n(n) {}
		constexpr ~CopyDelete() noexcept = default;
	public:
		template<class U>
		requires Traits::Concepts::CAssignableTo<U*, element_type*>
		void operator()(U* ptr) const {
			Allocator<element_type> alloc;
			using traits = AllocatorTraits<Allocator<element_type>>;
			const size_t count = m_n * element_count;
			for (size_t i = count; i-- > 0;) {
				traits::destroy(alloc, static_cast<element_type*>(ptr) + i);
			}
			traits::deallocate(alloc, static_cast<element_type*>(ptr), count);
		}
	};
	template<class T, size_t N>
	struct CopyDelete<T[N]> {
		using element_type = Traits::remove_all_extents_t<T[N]>;
		static constexpr size_t element_count = Traits::element_count_v<T[N]>;
	public:
		template<class U>
		requires Traits::Concepts::CAssignableTo<U*, element_type*>
		void operator()(U* ptr) const {
			Allocator<element_type> alloc;
			using traits = AllocatorTraits<Allocator<element_type>>;
			for (size_t i = element_count; i-- > 0;) {
				traits::destroy(alloc, static_cast<element_type*>(ptr) + i);
			}
			traits::deallocate(alloc, static_cast<element_type*>(ptr), element_count);
		}
	};

	template<class T> struct CopyNew;
	template<class T>
	struct CopyNew<T[]> {
		using element_type = Traits::remove_all_extents_t<T[]>;
		static constexpr size_t element_count = Traits::element_count_v<T[]>;
	private:
		size_t m_n;
	public:
		constexpr CopyNew() noexcept = delete;
		constexpr CopyNew(const CopyNew&) noexcept = default;
		constexpr CopyNew(size_t n) noexcept : m_n(n) {}
		constexpr ~CopyNew() noexcept = default;
	public:
		element_type* operator()(const Traits::remove_extent_t<T[]>& u) const {
			Allocator<element_type> alloc;
			using traits = AllocatorTraits<Allocator<element_type>>;
			element_type* ptr = traits::allocate(alloc, m_n * element_count);
			element_type* p = ptr;
			const element_type* init = nullptr;
			if constexpr (Traits::Concepts::CArray<Traits::remove_extent_t<T[]>>) init = reinterpret_cast<const element_type*>(&u[0]);
			else init = &u;
			for (size_t i = 0; i < m_n; ++i) {
				for (size_t j = 0; j < element_count; ++j) {
					traits::construct(alloc, p++, init[j]);
				}
			}
			return ptr;
		}
	};
	template<class T, size_t N>
	struct CopyNew<T[N]> {
		using element_type = Traits::remove_all_extents_t<T[N]>;
		static constexpr size_t element_count = Traits::element_count_v<T[N]>;
	public:
		element_type* operator()(const Traits::remove_extent_t<T[N]>& u) const {
			Allocator<element_type> alloc;
			using traits = AllocatorTraits<Allocator<element_type>>;
			const size_t down_count = Traits::element_count_v<T>;
			element_type* ptr = traits::allocate(alloc, element_count);
			element_type* p = ptr;
			const element_type* init = nullptr;
			if constexpr (Traits::Concepts::CArray<Traits::remove_extent_t<T[N]>>) init = reinterpret_cast<const element_type*>(&u[0]);
			else init = &u;
			for (size_t i = 0; i < N; ++i) {
				for (size_t j = 0; j < down_count; ++j) {
					traits::construct(alloc, p++, init[j]);
				}
			}
			return ptr;
		}
	};

	template<class D, class T>
	concept CDefaultDeleter = requires(D const& deleter, Traits::remove_all_extents_t<T>* ptr) {
		requires Traits::Concepts::CDefaultConstructible<D>;
		deleter(ptr);
	};
}
