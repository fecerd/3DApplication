export module FlagSet;
import CSTDINT;
import Traits;
using namespace System;

export namespace System {
	template<Traits::Concepts::CBitFlag E>
	class FlagSet {
		using value_type = System::Traits::int_t<false, sizeof(E)>;
		E m_value;
	public:
		constexpr FlagSet() noexcept : m_value(static_cast<E>(0)) {}
		constexpr FlagSet(E f) noexcept : m_value(f) {}
		template<Traits::Concepts::CSame<E> ...Args>
		constexpr FlagSet(E f, Args ...args) noexcept
			: m_value(static_cast<E>(
				static_cast<value_type>(f)
				| ((static_cast<value_type>(args)) | ...)
				)) {}
		constexpr FlagSet(const FlagSet&) noexcept = default;
	public:
		constexpr bool Includes(const FlagSet<E>& flags) const noexcept {
			return (*this & flags) == flags;
		}
		constexpr bool Excludes(const FlagSet<E>& flags) const noexcept {
			return !(*this & flags);
		}
		constexpr void Toggle() noexcept {
			m_value = static_cast<E>(~static_cast<value_type>(m_value));
		}
	public:
		static constexpr FlagSet<E> FromBitMask(uint8_t high, uint8_t low) noexcept {
			return FlagSet<E>(static_cast<E>(((1ull << (high + 1)) - 1) ^ ((1ull << low) - 1)));
		}
		static constexpr FlagSet<E> GetBitMaskAll() noexcept {
			return FlagSet<E>(~static_cast<value_type>(0));
		}
		static constexpr uint8_t GetNumBits() noexcept { return sizeof(value_type) * 8; }
	public:
		constexpr operator E() const noexcept { return m_value; }
		constexpr operator E& () noexcept { return m_value; }
		explicit constexpr operator bool() const noexcept { return static_cast<value_type>(m_value); }
	public:
		constexpr FlagSet<E>& operator=(const FlagSet<E>& rhs) noexcept = default;
		constexpr FlagSet<E> operator&(const FlagSet<E>& rhs) const noexcept {
			value_type v = static_cast<value_type>(m_value) & static_cast<value_type>(rhs.m_value);
			return FlagSet<E>(static_cast<E>(v));
		}
		constexpr FlagSet<E>& operator&=(const FlagSet<E>& rhs) noexcept {
			return *this = *this & rhs;
		}
		constexpr FlagSet<E> operator|(const FlagSet<E>& rhs) const noexcept {
			return FlagSet<E>(static_cast<E>(
				static_cast<value_type>(m_value) | static_cast<value_type>(rhs.m_value)
				));
		}
		constexpr FlagSet<E>& operator|=(const FlagSet<E>& rhs) noexcept {
			return *this = *this | rhs;
		}
		constexpr FlagSet<E> operator^(const FlagSet<E>& rhs) const noexcept {
			return FlagSet<E>(
				static_cast<E>(static_cast<value_type>(m_value) ^ static_cast<value_type>(rhs.m_value))
			);
		}
		constexpr FlagSet<E>& operator^=(const FlagSet<E>& rhs) noexcept {
			return *this = *this ^ rhs;
		}
		constexpr FlagSet<E> operator~() const noexcept {
			return FlagSet<E>(static_cast<E>(~static_cast<value_type>(m_value)));
		}
		constexpr bool operator==(const FlagSet<E>& rhs) const noexcept {
			return m_value == rhs.m_value;
		}
		constexpr bool operator==(E rhs) const noexcept { return m_value == rhs; }
	};
}
