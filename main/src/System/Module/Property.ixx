export module Property;
export import Function;
import Traits;

//DefaultGS
namespace System {
	/// <summary>
	/// Propertyクラスが使用するデフォルトのゲッタ/セッタ
	/// </summary>
	template<class T>
	struct DefaultGS {
		static T& Get(T& p, void* obj = nullptr) noexcept { return p; }
		static void Set(T& p, const T& value, void* obj = nullptr) noexcept { p = value; }
	};
}

//Property
export namespace System {
	template<class T, class C = void, class GS = DefaultGS<T>>
	class Property {
		static constexpr bool HasGetter = requires(T& p, C* obj) { GS::Get(p, obj); };
		static constexpr bool HasGetterRef = requires(T & p, C* obj) { { GS::Get(p, obj) } -> Traits::Concepts::convertible_to<T&>; };
		static constexpr bool HasSetterCopyable = requires(T & p, const T & value, C * obj) { GS::Set(p, value, obj); };
		static constexpr bool HasSetterMovable = requires(T & p, T && value, C* obj) { GS::Set(p, static_cast<T&&>(value), obj); };
		static constexpr bool HasSetter = HasSetterCopyable || HasSetterMovable;
		static constexpr bool CopyableType = requires(T & p, const T & value) { p = value; };
		static constexpr bool MovableType = requires(T & p, T && value) { p = static_cast<T&&>(value); };
	private:
		mutable T m_value;
		C* const m_object;
	public:
		Property() noexcept = delete;
		Property(C* object) noexcept : m_object(object) {}
		Property(C* object, const T& value) noexcept : m_value(value), m_object(object) {}
		Property(C* object, T&& value) noexcept : m_value(static_cast<T&&>(value)), m_object(object) {}
		Property(const Property& arg) noexcept : m_value(arg.m_value), m_object(arg.m_object) {}
		Property(Property&& arg) noexcept : m_value(static_cast<T&&>(arg.m_value)), m_object(arg.m_object) {}
	public:
		T* operator->() noexcept { return &m_value; }
		const T* operator->() const noexcept { return &m_value; }
		/// <summary>
		/// 非constゲッタ(参照版)
		/// </summary>
		operator T& () noexcept requires(HasGetterRef || !HasGetter) {
			if constexpr (HasGetterRef) return GS::Get(m_value, m_object);
			else return m_value;
		}
		/// <summary>
		/// constゲッタ(ポインタor値版)
		/// </summary>
		operator T() const noexcept requires(Traits::Concepts::CPointer<T> || !HasGetterRef) {
			if constexpr (HasGetter) return GS::Get(m_value, m_object);
			else return m_value;
		}
		/// <summary>
		/// constゲッタ(参照版)
		/// </summary>
		operator T const& () const noexcept requires(!Traits::Concepts::CPointer<T> && (HasGetterRef || !HasGetter)) {
			if constexpr (HasGetterRef) return GS::Get(m_value, m_object);
			else return m_value;
		}
		Property& operator=(const T& value) noexcept requires(CopyableType) {
			if constexpr (HasSetter) GS::Set(m_value, value, m_object);
			else m_value = value;
			return *this;
		}
		Property& operator=(T&& value) noexcept requires(MovableType) {
			if constexpr (HasSetter) GS::Set(m_value, static_cast<T&&>(value), m_object);
			else m_value = static_cast<T&&>(value);
			return *this;
		}
		const Property& operator=(const T&) const noexcept = delete;
		const Property& operator=(T&&) const noexcept = delete;
	public:
		Property& operator=(const Property<T, C, GS>& rhs) noexcept {
			if (this == &rhs) return *this;
			m_value = rhs.m_value;
			m_object = rhs.m_object;
			return *this;
		}
		Property& operator=(Property<T, C, GS>&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_value = static_cast<T&&>(rhs.m_value);
			m_object = rhs.m_object;
			return *this;
		}
	};

	template<class T, class GS>
	class Property<T, void, GS> {
		static constexpr bool HasGetter = requires(T & p) { GS::Get(p); };
		static constexpr bool HasGetterRef = requires(T & p) { { GS::Get(p) } -> Traits::Concepts::convertible_to<T&>; };
		static constexpr bool HasSetterCopyable = requires(T & p, const T & value) { GS::Set(p, value); };
		static constexpr bool HasSetterMovable = requires(T & p, T && value) { GS::Set(p, static_cast<T&&>(value)); };
		static constexpr bool HasSetter = HasSetterCopyable || HasSetterMovable;
		static constexpr bool CopyableType = requires(T & p, const T & value) { p = value; };
		static constexpr bool MovableType = requires(T & p, T && value) { p = static_cast<T&&>(value); };
	private:
		mutable T m_value;
	public:
		Property() noexcept = default;
		Property(const T& value) noexcept : m_value(value) {}
		Property(T&& value) noexcept : m_value(static_cast<T&&>(value)) {}
		Property(const Property& arg) noexcept : m_value(arg.m_value) {}
		Property(Property&& arg) noexcept : m_value(static_cast<T&&>(arg.m_value)) {}
	public:
		T* operator->() noexcept { return &m_value; }
		const T* operator->() const noexcept { return &m_value; }
		/// <summary>
		/// 非constゲッタ(参照版)
		/// </summary>
		operator T& () noexcept requires(HasGetterRef || !HasGetter) {
			if constexpr (HasGetterRef) return GS::Get(m_value);
			else return m_value;
		}
		/// <summary>
		/// constゲッタ(ポインタor値版)
		/// </summary>
		operator T() const noexcept requires(Traits::Concepts::CPointer<T> || !HasGetterRef) {
			if constexpr (HasGetter) return GS::Get(m_value);
			else return m_value;
		}
		/// <summary>
		/// constゲッタ(参照版)
		/// </summary>
		operator T const& () const noexcept requires(!Traits::Concepts::CPointer<T> && (HasGetterRef || !HasGetter)) {
			if constexpr (HasGetterRef) return GS::Get(m_value);
			else return m_value;
		}
		Property& operator=(const T& value) noexcept requires(CopyableType) {
			if constexpr (HasSetter) GS::Set(m_value, value);
			else m_value = value;
			return *this;
		}
		Property& operator=(T&& value) noexcept requires(MovableType) {
			if constexpr (HasSetter) GS::Set(m_value, static_cast<T&&>(value));
			else m_value = static_cast<T&&>(value);
			return *this;
		}
		const Property& operator=(const T&) const noexcept = delete;
		const Property& operator=(T&&) const noexcept = delete;
	public:
		Property& operator=(const Property<T, void, GS>& rhs) noexcept {
			if (this == &rhs) return *this;
			m_value = rhs.m_value;
			return *this;
		}
		Property& operator=(Property<T, void, GS>&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_value = static_cast<T&&>(rhs.m_value);
			return *this;
		}
	};
}
