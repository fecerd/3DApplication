module;
#if defined(__GNUC__) && !defined(__clang__)
#include <exception>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <memory>
#include <tuple>
#include <new>
#include <boost/coroutine2/detail/config.hpp>
#include <boost/coroutine2/detail/disable_overload.hpp>
#include <boost/context/fiber.hpp>
#include <boost/coroutine2/detail/state.hpp>
#include <boost/coroutine2/detail/create_control_block.ipp>
#include <boost/context/fixedsize_stack.hpp>
#include <boost/context/segmented_stack.hpp>
#else
#endif
export module BoostCoroutine;

//Boostライブラリは現状g++のみ使用可能
#if defined(__GNUC__) && !defined(__clang__)

//boost/coroutine2/fixedsize_stack.hpp
export namespace boost {
namespace coroutines2 {

using fixedsize_stack = boost::context::fixedsize_stack;
#if !defined(BOOST_USE_SEGMENTED_STACKS)
using default_stack = boost::context::default_stack;
#endif

}}

//boost/coroutine2/segmented_stack.hpp
export namespace boost {
namespace coroutines2 {

#if defined(BOOST_USE_SEGMENTED_STACKS)
# if ! defined(BOOST_WINDOWS)
using segmented_stack = boost::context::segmented_stack;
using default_stack = boost::context::default_stack;
# endif
#endif

}}

//coroutine2/detail/coroutine.hpp
export namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
class pull_coroutine;

template< typename T >
class push_coroutine;

}}}

//coroutine2/detail/pull_coroutine.hpp
export namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
class pull_coroutine {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit pull_coroutine( control_block *) noexcept;

    bool has_result_() const noexcept;

public:
    template< typename Fn,
              typename = detail::disable_overload< pull_coroutine, Fn >
    >
    explicit pull_coroutine( Fn &&);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator &&, Fn &&);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&) noexcept;

    pull_coroutine & operator=( pull_coroutine && other) noexcept {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    T get() noexcept;

    class iterator {
    private:
        pull_coroutine< T > *   c_{ nullptr };

        void fetch_() noexcept {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_) ) {
                c_ = nullptr;
                return;
            }
        }

        void increment_() {
            BOOST_ASSERT( nullptr != c_);
            BOOST_ASSERT( * c_);
            ( * c_)();
            fetch_();
        }

    public:
        typedef std::input_iterator_tag iterator_category;
        typedef typename std::remove_reference< T >::type value_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type * pointer;
        typedef value_type & reference;

        typedef pointer   pointer_t;
        typedef reference reference_t;

        iterator() noexcept = default;

        explicit iterator( pull_coroutine< T > * c) noexcept :
            c_{ c } {
            fetch_();
        }

        bool operator==( iterator const& other) const noexcept {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const noexcept {
            return other.c_ != c_;
        }

        iterator & operator++() {
            increment_();
            return * this;
        }

        void operator++( int) {
            increment_();
        }

        reference_t operator*() const noexcept {
            return c_->cb_->get();
        }

        pointer_t operator->() const noexcept {
            return std::addressof( c_->cb_->get() );
        }
    };

    friend class iterator;

    iterator begin() { return iterator (this); }
    iterator end() { return iterator(); }
};

template< typename T >
class pull_coroutine< T & > {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit pull_coroutine( control_block *) noexcept;

    bool has_result_() const noexcept;

public:
    template< typename Fn,
              typename = detail::disable_overload< pull_coroutine, Fn >
    >
    explicit pull_coroutine( Fn &&);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator &&, Fn &&);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&) noexcept;

    pull_coroutine & operator=( pull_coroutine && other) noexcept {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    T & get() noexcept;

    class iterator {
    private:
        pull_coroutine< T & > *   c_{ nullptr };

        void fetch_() noexcept {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_) ) {
                c_ = nullptr;
                return;
            }
        }

        void increment_() {
            BOOST_ASSERT( nullptr != c_);
            BOOST_ASSERT( * c_);
            ( * c_)();
            fetch_();
        }

    public:
        typedef std::input_iterator_tag iterator_category;
        typedef typename std::remove_reference< T >::type value_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type * pointer;
        typedef value_type & reference;

        typedef pointer   pointer_t;
        typedef reference reference_t;

        iterator() noexcept = default;

        explicit iterator( pull_coroutine< T & > * c) noexcept :
            c_{ c } {
            fetch_();
        }

        bool operator==( iterator const& other) const noexcept {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const noexcept {
            return other.c_ != c_;
        }

        iterator & operator++() {
            increment_();
            return * this;
        }

        void operator++( int) {
            increment_();
        }

        reference_t operator*() const noexcept {
            return c_->cb_->get();
        }

        pointer_t operator->() const noexcept {
            return std::addressof( c_->cb_->get() );
        }
    };

    friend class iterator;

    iterator begin() { return iterator (this); }
    iterator end() { return iterator(); }
};

template<>
class pull_coroutine< void > {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit pull_coroutine( control_block *) noexcept;

public:
    template< typename Fn,
              typename = detail::disable_overload< pull_coroutine, Fn >
    >
    explicit pull_coroutine( Fn &&);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator &&, Fn &&);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&) noexcept;

    pull_coroutine & operator=( pull_coroutine && other) noexcept {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;


};

template< typename T >
typename pull_coroutine< T >::iterator
begin( pull_coroutine< T > & c) {
    return typename pull_coroutine< T >::iterator( & c);
}

template< typename T >
typename pull_coroutine< T >::iterator
end( pull_coroutine< T > &) {
    return typename pull_coroutine< T >::iterator();
}

}}}

//coroutine2/detail/push_coroutine.hpp
export namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
class push_coroutine {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit push_coroutine( control_block * cb) noexcept : cb_{ cb } {}

public:
    template< typename Fn,
              typename = detail::disable_overload< push_coroutine, Fn >
    >
    explicit push_coroutine( Fn && fn) :
    push_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
}

    ~push_coroutine(){
    if ( nullptr != cb_) {
        cb_->deallocate();
    }
}

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine && other) noexcept : cb_{ nullptr } {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
	}

    push_coroutine & operator=( push_coroutine && other) noexcept {
        if ( this == & other) return * this;
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }

    push_coroutine & operator()( T const& t)  {
    cb_->resume( t);
    return * this;
}

    push_coroutine & operator()( T && t) {
    cb_->resume( std::forward< T >( t) );
    return * this;
}

    explicit operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

    bool operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

    class iterator {
    private:
        push_coroutine< T > *   c_{ nullptr };

    public:
        typedef std::output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;

        iterator() noexcept = default;

        explicit iterator( push_coroutine< T > * c) noexcept :
            c_{ c } {
        }

        iterator & operator=( T t) {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_)( t) ) {
                c_ = nullptr;
            }
            return * this;
        }

        bool operator==( iterator const& other) const noexcept {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const noexcept {
            return other.c_ != c_;
        }

        iterator & operator*() noexcept {
            return * this;
        }

        iterator & operator++() noexcept {
            return * this;
        }
    };
};

template< typename T >
class push_coroutine< T & > {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit push_coroutine( control_block * cb) noexcept  :
    cb_{ cb } {
}

public:
    template< typename Fn,
              typename = detail::disable_overload< push_coroutine, Fn >
    >
    explicit push_coroutine( Fn && fn) :
    push_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
}

    ~push_coroutine() {
    if ( nullptr != cb_) {
        cb_->deallocate();
    }
}

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine && other) noexcept : cb_{ nullptr } {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
	}

    push_coroutine & operator=( push_coroutine && other) noexcept {
        if ( this == & other) return * this;
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }

    push_coroutine & operator()( T & t) {
    cb_->resume( t);
    return * this;
}

    explicit operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

    bool operator!() const noexcept  {
    return nullptr == cb_ || ! cb_->valid();
}

    class iterator {
    private:
        push_coroutine< T & >   *   c_{ nullptr };

    public:
        typedef std::output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;

        iterator() noexcept = default;

        explicit iterator( push_coroutine< T & > * c) noexcept :
            c_{ c } {
        }

        iterator & operator=( T & t) {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_)( t) ) {
                c_ = nullptr;
            }
            return * this;
        }

        bool operator==( iterator const& other) const noexcept {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const noexcept {
            return other.c_ != c_;
        }

        iterator & operator*() noexcept {
            return * this;
        }

        iterator & operator++() noexcept {
            return * this;
        }
    };
};

template<>
class push_coroutine< void > {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    control_block   *   cb_;

    explicit push_coroutine( control_block * cb) noexcept :
    cb_{ cb } {
}

public:
    template< typename Fn,
              typename = detail::disable_overload< push_coroutine, Fn >
    >
    explicit push_coroutine( Fn && fn) :
    push_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
}

    ~push_coroutine();

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine && other) noexcept : cb_{ nullptr } {
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
	}

    push_coroutine & operator=( push_coroutine && other) noexcept;

    push_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;
};

template< typename T >
typename push_coroutine< T >::iterator
begin( push_coroutine< T > & c) {
    return typename push_coroutine< T >::iterator( & c);
}

template< typename T >
typename push_coroutine< T >::iterator
end( push_coroutine< T > &) {
    return typename push_coroutine< T >::iterator();
}

}}}

//coroutine2/detail/pull_control_block_cc.hpp
export namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
struct pull_coroutine< T >::control_block {
    boost::context::fiber                                           c;
    typename push_coroutine< T >::control_block                 *   other;
    state_t                                                         state;
    std::exception_ptr                                              except;
    bool                                                            bvalid;
	alignas(T) std::byte storage[sizeof(T)];

    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( typename push_coroutine< T >::control_block *, boost::context::fiber &) noexcept;

    ~control_block();

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume();

    void set( T const&);
    void set( T &&);

    T & get() noexcept;

    bool valid() const noexcept;
};

template< typename T >
struct pull_coroutine< T & >::control_block {
    struct holder {
        T   &   t;

        holder( T & t_) :
            t{ t_ } {
        }
    };

    boost::context::fiber                                                       c;
    typename push_coroutine< T & >::control_block                           *   other;
    state_t                                                                     state;
    std::exception_ptr                                                          except;
    bool                                                                        bvalid;
	alignas(holder) std::byte storage[sizeof(holder)];


    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( typename push_coroutine< T & >::control_block *, boost::context::fiber &) noexcept;

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume();

    void set( T &);

    T & get() noexcept;

    bool valid() const noexcept;
};

struct pull_coroutine< void >::control_block {
    boost::context::fiber                       c;
    push_coroutine< void >::control_block  *    other;
    state_t                                     state;
    std::exception_ptr                          except;

    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( push_coroutine< void >::control_block *, boost::context::fiber &) noexcept;

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume();

    bool valid() const noexcept;
};

}}}

//coroutine2/detail/push_control_block_cc.hpp
export namespace boost {
namespace coroutines2 {
namespace detail {

template< typename T >
struct push_coroutine< T >::control_block {
    boost::context::fiber                           c;
    typename pull_coroutine< T >::control_block *   other;
    state_t                                         state;
    std::exception_ptr                              except;

    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( typename pull_coroutine< T >::control_block *, boost::context::fiber &) noexcept;

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume( T const&);

    void resume( T &&);

    bool valid() const noexcept;
};

template< typename T >
struct push_coroutine< T & >::control_block {
    boost::context::fiber                               c;
    typename pull_coroutine< T & >::control_block   *   other;
    state_t                                             state;
    std::exception_ptr                                  except;

    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( typename pull_coroutine< T & >::control_block *, boost::context::fiber &) noexcept;

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume( T &);

    bool valid() const noexcept;
};

struct push_coroutine< void >::control_block {
    boost::context::fiber                       c;
    pull_coroutine< void >::control_block  *    other;
    state_t                                     state;
    std::exception_ptr                          except;

    static void destroy( control_block * cb) noexcept;

    template< typename StackAllocator, typename Fn >
    control_block( context::preallocated, StackAllocator &&, Fn &&);

    control_block( pull_coroutine< void >::control_block *, boost::context::fiber &) noexcept;

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void deallocate() noexcept;

    void resume();

    bool valid() const noexcept;
};

}}}


export namespace boost::coroutines2::detail {
	inline push_coroutine<void>::~push_coroutine() {
		if ( nullptr != cb_) {
        	cb_->deallocate();
		}
	}
	inline push_coroutine<void> & push_coroutine<void>::operator=( push_coroutine<void> && other) noexcept {
        if ( this == & other) return * this;
		auto tmp = other.cb_;
		other.cb_ = cb_;
		cb_ = tmp;
        return * this;
    }
	inline push_coroutine<void> & push_coroutine<void>::operator()()  {
		cb_->resume();
		return * this;
	}

	inline push_coroutine<void>::operator bool() const noexcept {
		return nullptr != cb_ && cb_->valid();
	}

	inline bool push_coroutine<void>::operator!() const noexcept  {
		return nullptr == cb_ || ! cb_->valid();
	}
}


//coroutine2/detail/pull_coroutine.ipp
export namespace boost {
namespace coroutines2 {
namespace detail {

// pull_coroutine< T >

template< typename T >
pull_coroutine< T >::pull_coroutine( control_block * cb) noexcept :
    cb_{ cb } {
}

template< typename T >
bool
pull_coroutine< T >::has_result_() const noexcept {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn,
          typename
>
pull_coroutine< T >::pull_coroutine( Fn && fn) :
    pull_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::pull_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
    if ( ! cb_->valid() ) {
        cb_->deallocate();
        cb_ = nullptr;
    }
}

template< typename T >
pull_coroutine< T >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->deallocate();
    }
}

template< typename T >
pull_coroutine< T >::pull_coroutine( pull_coroutine && other) noexcept : cb_{ nullptr } {
	auto tmp = other.cb_;
	other.cb_ = cb_;
	cb_ = tmp;
}

template< typename T >
pull_coroutine< T > & 
pull_coroutine< T >::operator()() {
    cb_->resume();
    return * this;
}

template< typename T >
pull_coroutine< T >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T
pull_coroutine< T >::get() noexcept {
    return std::move( cb_->get() );
}


// pull_coroutine< T & >

template< typename T >
pull_coroutine< T & >::pull_coroutine( control_block * cb) noexcept :
    cb_{ cb } {
}

template< typename T >
bool
pull_coroutine< T & >::has_result_() const noexcept {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn,
          typename
>
pull_coroutine< T & >::pull_coroutine( Fn && fn) :
    pull_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::pull_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
    if ( ! cb_->valid() ) {
        cb_->deallocate();
        cb_ = nullptr;
    }
}

template< typename T >
pull_coroutine< T & >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->deallocate();
    }
}

template< typename T >
pull_coroutine< T & >::pull_coroutine( pull_coroutine && other) noexcept : cb_{ nullptr } {
	auto tmp = other.cb_;
	other.cb_ = cb_;
	cb_ = tmp;
}

template< typename T >
pull_coroutine< T & > &
pull_coroutine< T & >::operator()() {
    cb_->resume();
    return * this;
}

template< typename T >
pull_coroutine< T & >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T & >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T &
pull_coroutine< T & >::get() noexcept {
    return cb_->get();
}


// pull_coroutine< void >

inline
pull_coroutine< void >::pull_coroutine( control_block * cb) noexcept :
    cb_{ cb } {
}

template< typename Fn,
          typename
>
pull_coroutine< void >::pull_coroutine( Fn && fn) :
    pull_coroutine{ default_stack(), std::forward< Fn >( fn) } {
}

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::pull_coroutine( StackAllocator && salloc, Fn && fn) :
    cb_{ create_control_block< control_block >( std::forward< StackAllocator >( salloc), std::forward< Fn >( fn) ) } {
    if ( ! cb_->valid() ) {
        cb_->deallocate();
        cb_ = nullptr;
    }
}

inline
pull_coroutine< void >::~pull_coroutine() {
    if ( nullptr != cb_) {
        cb_->deallocate();
    }
}

inline
pull_coroutine< void >::pull_coroutine( pull_coroutine && other) noexcept : cb_{ nullptr } {
	auto tmp = other.cb_;
	other.cb_ = cb_;
	cb_ = tmp;
}

inline
pull_coroutine< void > &
pull_coroutine< void >::operator()() {
    cb_->resume();
    return * this;
}

inline
pull_coroutine< void >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

inline
bool
pull_coroutine< void >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

}}}

//coroutine2/detail/pull_control_block_cc.ipp
namespace boost {
namespace coroutines2 {
namespace detail {

// pull_coroutine< T >

template< typename T >
void
pull_coroutine< T >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc,
                                                   Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized push_coroutine< T >
               typename push_coroutine< T >::control_block synthesized_cb{ this, c };
               push_coroutine< T > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized push_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
               return std::move( other->c).resume();
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)](boost::context::fiber && c) mutable {
          // create synthesized push_coroutine< T >
          typename push_coroutine< T >::control_block synthesized_cb{ this, c };
          push_coroutine< T > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized push_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back
          return std::move( other->c).resume();
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{},
    bvalid{ false },
    storage{} {
        c = std::move( c).resume();
        if ( except) {
            std::rethrow_exception( except);
        }
}

template< typename T >
pull_coroutine< T >::control_block::control_block( typename push_coroutine< T >::control_block * cb,
                                                   boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{},
    bvalid{ false },
    storage{} {
}

template< typename T >
pull_coroutine< T >::control_block::~control_block() {
    // destroy data if set
    if ( bvalid) {
        reinterpret_cast< T * >( std::addressof( storage) )->~T();
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::resume() {
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::set( T const& t) {
    // destroy data if set
    if ( bvalid) {
        reinterpret_cast< T * >( std::addressof( storage) )->~T();
    }
    ::new ( static_cast< void * >( std::addressof( storage) ) ) T( t);
    bvalid = true;
}

template< typename T >
void
pull_coroutine< T >::control_block::set( T && t) {
    // destroy data if set
    if ( bvalid) {
        reinterpret_cast< T * >( std::addressof( storage) )->~T();
    }
    ::new ( static_cast< void * >( std::addressof( storage) ) ) T( std::move( t) );
    bvalid = true;
}

template< typename T >
T &
pull_coroutine< T >::control_block::get() noexcept {
    return * reinterpret_cast< T * >( std::addressof( storage) );
}

template< typename T >
bool
pull_coroutine< T >::control_block::valid() const noexcept {
    return nullptr != other && state_t::none == ( state & state_t::complete) && bvalid;
}


// pull_coroutine< T & >

template< typename T >
void
pull_coroutine< T & >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc,
                                                     Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized push_coroutine< T & >
               typename push_coroutine< T & >::control_block synthesized_cb{ this, c };
               push_coroutine< T & > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized push_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
               return std::move( other->c).resume();
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)](boost::context::fiber && c) mutable {
          // create synthesized push_coroutine< T & >
          typename push_coroutine< T & >::control_block synthesized_cb{ this, c };
          push_coroutine< T & > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized push_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back
          return std::move( other->c).resume();
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{},
    bvalid{ false },
    storage{} {
        c = std::move( c).resume();
        if ( except) {
            std::rethrow_exception( except);
        }
}

template< typename T >
pull_coroutine< T & >::control_block::control_block( typename push_coroutine< T & >::control_block * cb,
                                                     boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{},
    bvalid{ false },
    storage{} {
}

template< typename T >
void
pull_coroutine< T & >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

template< typename T >
void
pull_coroutine< T & >::control_block::resume() {
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
void
pull_coroutine< T & >::control_block::set( T & t) {
    ::new (static_cast<void*>(std::addressof(storage))) holder{ t };
    bvalid = true;
}

template< typename T >
T &
pull_coroutine< T & >::control_block::get() noexcept {
    return reinterpret_cast< holder * >( std::addressof( storage) )->t;
}

template< typename T >
bool
pull_coroutine< T & >::control_block::valid() const noexcept {
    return nullptr != other && state_t::none == ( state & state_t::complete) && bvalid;
}


// pull_coroutine< void >

inline
void
pull_coroutine< void >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc,
                                                      Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized push_coroutine< void >
               typename push_coroutine< void >::control_block synthesized_cb{ this, c };
               push_coroutine< void > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized push_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
               return std::move( other->c).resume();
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)]( boost::context::fiber && c) mutable {
          // create synthesized push_coroutine< void >
          typename push_coroutine< void >::control_block synthesized_cb{ this, c };
          push_coroutine< void > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized push_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back to ctx
          return std::move( other->c).resume();
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{} {
        c = std::move( c).resume();
        if ( except) {
            std::rethrow_exception( except);
        }
}

inline
pull_coroutine< void >::control_block::control_block( push_coroutine< void >::control_block * cb,
                                                      boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{} {
}

inline
void
pull_coroutine< void >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

inline
void
pull_coroutine< void >::control_block::resume() {
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

inline
bool
pull_coroutine< void >::control_block::valid() const noexcept {
    return nullptr != other && state_t::none == ( state & state_t::complete);
}

}}}

//coroutine2/detail/push_control_block_cc.ipp
namespace boost {
namespace coroutines2 {
namespace detail {

// push_coroutine< T >

template< typename T >
void
push_coroutine< T >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc,
                                                   Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized pull_coroutine< T >
               typename pull_coroutine< T >::control_block synthesized_cb{ this, c };
               pull_coroutine< T > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               other->c = std::move( other->c).resume();
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized pull_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
               other->c = std::move( other->c).resume();
               return std::move( other->c);
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)](boost::context::fiber && c) mutable {
          // create synthesized pull_coroutine< T >
          typename pull_coroutine< T >::control_block synthesized_cb{ this, c };
          pull_coroutine< T > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          other->c = std::move( other->c).resume();
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized pull_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back
          return std::move( other->c).resume();
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{} {
        c = std::move( c).resume();
}

template< typename T >
push_coroutine< T >::control_block::control_block( typename pull_coroutine< T >::control_block * cb,
                                                   boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{} {
}

template< typename T >
void
push_coroutine< T >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

template< typename T >
void
push_coroutine< T >::control_block::resume( T const& data) {
    // pass data to other context
    other->set( data);
    // resume other context
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
void
push_coroutine< T >::control_block::resume( T && data) {
    // pass data to other context
    other->set( std::move( data) );
    // resume other context
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
bool
push_coroutine< T >::control_block::valid() const noexcept {
    return state_t::none == ( state & state_t::complete );
}


// push_coroutine< T & >

template< typename T >
void
push_coroutine< T & >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T & >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc,
                                                     Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized pull_coroutine< T & >
               typename pull_coroutine< T & >::control_block synthesized_cb{ this, c };
               pull_coroutine< T & > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               other->c = std::move( other->c).resume();
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized pull_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
              other->c = std::move( other->c).resume();
              return std::move( other->c);
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)](boost::context::fiber && c) mutable {
          // create synthesized pull_coroutine< T & >
          typename pull_coroutine< T & >::control_block synthesized_cb{ this, c };
          pull_coroutine< T & > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          other->c = std::move( other->c).resume();
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized pull_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back
          other->c = std::move( other->c).resume();
          return std::move( other->c);
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{} {
        c = std::move( c).resume();
}

template< typename T >
push_coroutine< T & >::control_block::control_block( typename pull_coroutine< T & >::control_block * cb,
                                                     boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{} {
}

template< typename T >
void
push_coroutine< T & >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

template< typename T >
void
push_coroutine< T & >::control_block::resume( T & data) {
    // pass data to other context
    other->set( data);
    // resume other context
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

template< typename T >
bool
push_coroutine< T & >::control_block::valid() const noexcept {
    return state_t::none == ( state & state_t::complete );
}


// push_coroutine< void >

inline
void
push_coroutine< void >::control_block::destroy( control_block * cb) noexcept {
    boost::context::fiber c = std::move( cb->c);
    // destroy control structure
    cb->~control_block();
    // destroy coroutine's stack
    cb->state |= state_t::destroy;
}

template< typename StackAllocator, typename Fn >
push_coroutine< void >::control_block::control_block( context::preallocated palloc, StackAllocator && salloc, Fn && fn) :
#if defined(BOOST_NO_CXX14_GENERIC_LAMBDAS)
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       wrap( [this](typename std::decay< Fn >::type & fn_,boost::context::fiber && c) mutable {
               // create synthesized pull_coroutine< void >
               typename pull_coroutine< void >::control_block synthesized_cb{ this, c };
               pull_coroutine< void > synthesized{ & synthesized_cb };
               other = & synthesized_cb;
               other->c = std::move( other->c).resume();
               if ( state_t::none == ( state & state_t::destroy) ) {
                   try {
                       auto fn = std::move( fn_);
                       // call coroutine-fn with synthesized pull_coroutine as argument
                       fn( synthesized);
                   } catch ( boost::context::detail::forced_unwind const&) {
                       throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
                   } catch ( abi::__forced_unwind const&) {
                       throw;
#endif
                   } catch (...) {
                       // store other exceptions in exception-pointer
                       except = std::current_exception();
                   }
               }
               // set termination flags
               state |= state_t::complete;
               // jump back
               other->c = std::move( other->c).resume();
               return std::move( other->c);
            },
            std::forward< Fn >( fn) ) },
#else
    c{ std::allocator_arg, palloc, std::forward< StackAllocator >( salloc),
       [this,fn_=std::forward< Fn >( fn)](boost::context::fiber && c) mutable {
          // create synthesized pull_coroutine< void >
          typename pull_coroutine< void >::control_block synthesized_cb{ this, c};
          pull_coroutine< void > synthesized{ & synthesized_cb };
          other = & synthesized_cb;
          other->c = std::move( other->c).resume();
          if ( state_t::none == ( state & state_t::destroy) ) {
              try {
                  auto fn = std::move( fn_);
                  // call coroutine-fn with synthesized pull_coroutine as argument
                  fn( synthesized);
              } catch ( boost::context::detail::forced_unwind const&) {
                  throw;
#if defined( BOOST_CONTEXT_HAS_CXXABI_H )
              } catch ( abi::__forced_unwind const&) {
                  throw;
#endif
              } catch (...) {
                  // store other exceptions in exception-pointer
                  except = std::current_exception();
              }
          }
          // set termination flags
          state |= state_t::complete;
          // jump back
          other->c = std::move( other->c).resume();
          return std::move( other->c);
       } },
#endif
    other{ nullptr },
    state{ state_t::unwind },
    except{} {
        c = std::move( c).resume();
}

inline
push_coroutine< void >::control_block::control_block( pull_coroutine< void >::control_block * cb,
                                                      boost::context::fiber & c_) noexcept :
    c{ std::move( c_) },
    other{ cb },
    state{ state_t::none },
    except{} {
}

inline
void
push_coroutine< void >::control_block::deallocate() noexcept {
    if ( state_t::none != ( state & state_t::unwind) ) {
        destroy( this);
    }
}

inline
void
push_coroutine< void >::control_block::resume() {
    c = std::move( c).resume();
    if ( except) {
        std::rethrow_exception( except);
    }
}

inline
bool
push_coroutine< void >::control_block::valid() const noexcept {
    return state_t::none == ( state & state_t::complete );
}

}}}

//coroutine.hpp
export namespace boost {
namespace coroutines2 {

template< typename T >
struct coroutine {
    using pull_type = detail::pull_coroutine< T >;
    using push_type = detail::push_coroutine< T >;
};

template< typename T >
using asymmetric_coroutine = coroutine< T >;

}}

#else
#endif

//System::Boost
#if defined(__GNUC__) && !defined(__clang__)
export namespace System::Boost {
	template<class T>
	using push_type = typename boost::coroutines2::coroutine<T>::push_type;
	template<class T>
	using pull_type = typename boost::coroutines2::coroutine<T>::pull_type;
}
#else
#endif
