export module CSTDINT;
export import <cstdint>;
export import <initializer_list>;
export import <compare>;
import <limits>;
#if !defined(__GNUC__) || defined(__clang__)
import <memory>;	//std::addressof
#endif

inline constexpr size_t SIZEMAX = SIZE_MAX;

#undef UINT8_MAX
#undef UINT16_MAX
#undef UINT32_MAX
#undef UINT64_MAX
#undef INT8_MAX
#undef INT8_MIN
#undef INT16_MAX
#undef INT16_MIN
#undef INT32_MAX
#undef INT32_MIN
#undef INT64_MAX
#undef INT64_MIN
#undef SIZE_MAX
#undef INFINITY
#undef NAN

//std整数型
export namespace System {
	using uint8_t = std::uint8_t;
	using uint16_t = std::uint16_t;
	using uint32_t = std::uint32_t;
	using uint64_t = std::uint64_t;
	using int8_t = std::int8_t;
	using int16_t = std::int16_t;
	using int32_t = std::int32_t;
	using int64_t = std::int64_t;
	using uintptr_t = std::uintptr_t;
	using intptr_t = std::intptr_t;
	using size_t = size_t;
	using nullptr_t = std::nullptr_t;
	using ptrdiff_t = ::ptrdiff_t;
}

//整数型の最大値・最小値
export namespace System {
	inline constexpr uint8_t UINT8_MAX = 0xff;
	inline constexpr uint8_t UINT8_MIN = 0x00;
	inline constexpr uint16_t UINT16_MAX = 0xffff;
	inline constexpr uint16_t UINT16_MIN = 0x0000;
	inline constexpr uint32_t UINT32_MAX = 0xffffffff;
	inline constexpr uint32_t UINT32_MIN = 0x00000000;
	inline constexpr uint64_t UINT64_MAX = 0xffffffffffffffff;
	inline constexpr uint64_t UINT64_MIN = 0x0000000000000000;
	inline constexpr int8_t INT8_MAX = static_cast<int8_t>(0x7f);
	inline constexpr int8_t INT8_MIN = static_cast<int8_t>(0x80);
	inline constexpr int16_t INT16_MAX = static_cast<int16_t>(0x7fff);
	inline constexpr int16_t INT16_MIN = static_cast<int16_t>(0x8000);
	inline constexpr int32_t INT32_MAX = 0x7fffffff;
	inline constexpr int32_t INT32_MIN = 0x80000000;
	inline constexpr int64_t INT64_MAX = 0x7fffffffffffffff;
	inline constexpr int64_t INT64_MIN = 0x8000000000000000;
	inline constexpr size_t SIZE_MAX = SIZEMAX;
}

namespace System {
	template<class T> struct MAX;
	template<>
	struct MAX<uint8_t> {
		static constexpr uint8_t value = UINT8_MAX;
	};
	template<>
	struct MAX<uint16_t> {
		static constexpr uint16_t value = UINT16_MAX;
	};
	template<>
	struct MAX<uint32_t> {
		static constexpr uint32_t value = UINT32_MAX;
	};
	template<>
	struct MAX<uint64_t> {
		static constexpr uint64_t value = UINT64_MAX;
	};
	template<>
	struct MAX<int8_t> {
		static constexpr int8_t value = INT8_MAX;
	};
	template<>
	struct MAX<int16_t> {
		static constexpr int16_t value = INT16_MAX;
	};
	template<>
	struct MAX<int32_t> {
		static constexpr int32_t value = INT32_MAX;
	};
	template<>
	struct MAX<int64_t> {
		static constexpr int64_t value = INT64_MAX;
	};

	template<class T> struct MIN;
	template<>
	struct MIN<uint8_t> {
		static constexpr uint8_t value = UINT8_MIN;
	};
	template<>
	struct MIN<uint16_t> {
		static constexpr uint16_t value = UINT16_MIN;
	};
	template<>
	struct MIN<uint32_t> {
		static constexpr uint32_t value = UINT32_MIN;
	};
	template<>
	struct MIN<uint64_t> {
		static constexpr uint64_t value = UINT64_MIN;
	};
	template<>
	struct MIN<int8_t> {
		static constexpr int8_t value = INT8_MIN;
	};
	template<>
	struct MIN<int16_t> {
		static constexpr int16_t value = INT16_MIN;
	};
	template<>
	struct MIN<int32_t> {
		static constexpr int32_t value = INT32_MIN;
	};
	template<>
	struct MIN<int64_t> {
		static constexpr int64_t value = INT64_MIN;
	};
}

//整数型の最大値・最小値(テンプレート)
export namespace System {
	template<class T> inline constexpr T MAX_VALUE = MAX<T>::value;
	template<class T> inline constexpr T MIN_VALUE = MIN<T>::value;
}

//浮動小数点数の定数
export namespace System {
	inline constexpr float FLOAT_NAN = std::numeric_limits<float>::quiet_NaN();
	inline constexpr double DOUBLE_NAN = std::numeric_limits<double>::quiet_NaN();
	inline constexpr long double LONGDOUBLE_NAN = std::numeric_limits<long double>::quiet_NaN();
	inline constexpr float FLOAT_INF = std::numeric_limits<float>::infinity();
	inline constexpr double DOUBLE_INF = std::numeric_limits<double>::infinity();
	inline constexpr long double LONGDOUBLE_INF = std::numeric_limits<long double>::infinity();
}

//特殊な数値型
export namespace System {
	//3バイト整数
	class uint24_c {
		uint8_t m_data[3] = { 0, 0, 0 };
	public:
		constexpr uint24_c() noexcept = default;
		constexpr uint24_c(const uint24_c& val) noexcept : m_data{ val.m_data[0], val.m_data[1], val.m_data[2] } {}
		constexpr uint24_c(const uint32_t& val) noexcept : m_data{ static_cast<uint8_t>(val & 0xff), static_cast<uint8_t>((val & 0xff00) >> 8), static_cast<uint8_t>((val & 0xff0000) >> 16) } {}
	public:
		constexpr operator uint32_t() const noexcept { return static_cast<uint32_t>((m_data[2] << 16) | (m_data[1] << 8) | m_data[0]); }
		explicit constexpr operator float() const noexcept { return static_cast<float>(operator uint32_t()); }
		explicit constexpr operator bool() const noexcept { return operator uint32_t() != 0; }
		uint24_c& operator=(const uint24_c&) = default;
		uint24_c& operator=(const uint32_t& rhs) noexcept {
			m_data[0] = static_cast<uint8_t>(rhs & 0xff);
			m_data[1] = static_cast<uint8_t>((rhs & 0xff00) >> 8);
			m_data[2] = static_cast<uint8_t>((rhs & 0xff0000) >> 16);
			return *this;
		}
		uint24_c& operator&=(const uint24_c& val) noexcept { return *this = *this & val; }
		uint24_c& operator|=(const uint24_c& val) noexcept { return *this = *this | val; }
		constexpr bool operator==(const uint24_c& val) const noexcept = default;
		constexpr uint24_c operator&(const uint24_c& val) const noexcept { return uint24_c(static_cast<uint32_t>(*this) & static_cast<uint32_t>(val)); }
		constexpr uint24_c operator|(const uint24_c& val) const noexcept { return uint24_c(static_cast<uint32_t>(*this) | static_cast<uint32_t>(val)); }
	};

	//F2DOT14固定小数点数型
	class f2dot14_c {
		uint16_t m_data = 0x0000;
	public:
		constexpr f2dot14_c() noexcept = default;
		constexpr f2dot14_c(const f2dot14_c& val) noexcept = default;
		constexpr f2dot14_c(const int32_t& val) noexcept {
			if (val >= 2) m_data = 0x4000 | 0x3fff;
			else if (val <= -2) m_data = 0x8000;
			else if (val == 1) m_data = 0x4000;
			else if (val == -1) m_data = 0xc000;
		}
		constexpr f2dot14_c(const float& val) noexcept {
			if (val >= 2.0f) m_data = 0x4000 | 0x3fff;
			else if (val >= 1.0f) m_data = 0x4000 | (0x3fff & static_cast<uint16_t>((val - 1.0f) * static_cast<float>(1 << 14)));
			else if (val >= 0.0f) m_data = 0x0000 | (0x3fff & static_cast<uint16_t>(val * static_cast<float>(1 << 14)));
			else if (val >= -1.0f) m_data = 0xc000 | (0x3fff & static_cast<uint16_t>((val + 1.0f) * static_cast<float>(1 << 14)));
			else if (val >= -2.0f) m_data = 0x8000 | (0x3fff & static_cast<uint16_t>((val + 2.0f) * static_cast<float>(1 << 14)));
			else m_data = 0x8000;
		}
		constexpr ~f2dot14_c() = default;
	public:
		explicit constexpr operator float() const noexcept {
			if ((0xc000 & m_data) == 0xc000) return -1.0f + (static_cast<float>(m_data & 0x3fff) / static_cast<float>(1 << 14));
			else if (0x8000 & m_data) return -2.0f + (static_cast<float>(m_data & 0x3fff) / static_cast<float>(1 << 14));
			else if (0x4000 & m_data) return 1.0f + (static_cast<float>(m_data & 0x3fff) / static_cast<float>(1 << 14));
			else return static_cast<float>(m_data & 0x3fff) / static_cast<float>(1 << 14);
		}
	public:
		constexpr f2dot14_c operator>>(const int32_t& val) const noexcept {
			f2dot14_c result;
			result.m_data = m_data >> val;
			return result;
		}
		constexpr f2dot14_c operator<<(const int32_t& val) const noexcept {
			f2dot14_c result;
			result.m_data = m_data << val;
			return result;
		}
		constexpr f2dot14_c operator&(const uint16_t& val) const noexcept {
			f2dot14_c result;
			result.m_data = m_data & val;
			return result;
		}
		constexpr f2dot14_c operator|(const uint16_t& val) const noexcept {
			f2dot14_c result;
			result.m_data = m_data | val;
			return result;
		}
		f2dot14_c& operator=(const f2dot14_c& rhs) noexcept = default;
		f2dot14_c& operator&=(const uint16_t& val) noexcept { m_data &= val; return *this; }
		f2dot14_c& operator|=(const uint16_t& val) noexcept { m_data |= val; return *this; }
		f2dot14_c& operator&=(const f2dot14_c& val) noexcept { m_data &= val.m_data; return *this; }
		f2dot14_c& operator|=(const f2dot14_c& val) noexcept { m_data |= val.m_data; return *this; }
	};

	//4バイト固定小数点数型(上位2バイトが整数部、下位2バイトが小数部)
	class fixed32_c {
		int16_t integer = 0;
		uint16_t fraction = 0;
	public:
		constexpr fixed32_c() noexcept = default;
		constexpr fixed32_c(const fixed32_c& val) noexcept : integer(val.integer), fraction(val.fraction) {}
		constexpr ~fixed32_c() = default;
	public:
		constexpr bool IsInteger() const noexcept { return fraction == 0; }
	public:
		explicit constexpr operator int16_t() const noexcept { return integer; }
		explicit constexpr operator float() const noexcept {
			float frac = static_cast<float>(fraction);
			while (frac > 0) frac /= 10.0f;
			return static_cast<float>(integer) + frac;
		}
		fixed32_c& operator=(const fixed32_c& val) noexcept {
			integer = val.integer;
			fraction = val.fraction;
			return *this;
		}
		fixed32_c& operator=(const int16_t& val) noexcept {
			integer = val;
			fraction = 0;
			return *this;
		}
		//uint8_t[4]をfixed32_cとして解釈してコピーする
		fixed32_c& operator=(const uint8_t* data) noexcept {
			integer = (data[0] << 8) | data[1];
			fraction = (data[2] << 8) | data[3];
			return *this;
		}
	};
}

//initializer_list
export namespace System {
	template<class T>
	using initializer_list = std::initializer_list<T>;
}

//ordering
export namespace System {
	using strong_ordering = std::strong_ordering;
	using partial_ordering = std::partial_ordering;
	using weak_ordering = std::weak_ordering;
}

//cl.exeでは***_ordering構造体内のstaticメンバ変数が未定義のシンボルとなるため、定義しておく。
//g++ではあってもなくてもよく、clangでは定義が重複する。不要なため、隠しておく
#if defined(_MSC_VER)
constexpr std::partial_ordering std::partial_ordering::less;
constexpr std::partial_ordering std::partial_ordering::equivalent;
constexpr std::partial_ordering std::partial_ordering::greater;
constexpr std::partial_ordering std::partial_ordering::unordered;
constexpr std::weak_ordering std::weak_ordering::less;
constexpr std::weak_ordering std::weak_ordering::equivalent;
constexpr std::weak_ordering std::weak_ordering::greater;
constexpr std::strong_ordering std::strong_ordering::less;
constexpr std::strong_ordering std::strong_ordering::equal;
constexpr std::strong_ordering std::strong_ordering::equivalent;
constexpr std::strong_ordering std::strong_ordering::greater;
#endif

//remove_reference
namespace System::Internal {
	template<typename T> struct remove_reference { using type = T; };
	template<typename T> struct remove_reference<T&> { using type = T; };
	template<typename T> struct remove_reference<T&&> { using type = T; };
}

//move
export namespace System {
	template<class T>
	constexpr typename Internal::remove_reference<T>::type&& move(T&& t) noexcept {
		return static_cast<typename Internal::remove_reference<T>::type&&>(t);
	}
}

//SGR(Select Graphics Rendition)コマンド
export namespace System {
	inline constexpr const char cout_red[] = "\x1b[31m";
	inline constexpr const char cout_green[] = "\x1b[32m";
	inline constexpr const char cout_yellow[] = "\x1b[33m";
	inline constexpr const char cout_blue[] = "\x1b[34m";
	inline constexpr const char cout_magenta[] = "\x1b[35m";
	inline constexpr const char cout_cyan[] = "\x1b[36m";
	inline constexpr const char cout_white[] = "\x1b[37m";
	inline constexpr const char cout_reset[] = "\x1b[m";
	inline constexpr const char cout_bold[] = "\x1b[1m";
	inline constexpr const char cout_light[] = "\x1b[2m";
	inline constexpr const char cout_italic[] = "\x1b[3m";
	inline constexpr const char cout_underline[] = "\x1b[4m";
	inline constexpr const char cout_flashing[] = "\x1b[5m";
	inline constexpr const char cout_flashing_violently[] = "\x1b[6m";
	inline constexpr const char cout_toggle_color[] = "\x1b[7m";
	inline constexpr const char cout_hide[] = "\x1b[8m";

	inline constexpr const wchar_t wcout_red[] = L"\x1b[31m";
	inline constexpr const wchar_t wcout_green[] = L"\x1b[32m";
	inline constexpr const wchar_t wcout_yellow[] = L"\x1b[33m";
	inline constexpr const wchar_t wcout_blue[] = L"\x1b[34m";
	inline constexpr const wchar_t wcout_magenta[] = L"\x1b[35m";
	inline constexpr const wchar_t wcout_cyan[] = L"\x1b[36m";
	inline constexpr const wchar_t wcout_white[] = L"\x1b[37m";
	inline constexpr const wchar_t wcout_reset[] = L"\x1b[m";
	inline constexpr const wchar_t wcout_bold[] = L"\x1b[1m";
	inline constexpr const wchar_t wcout_light[] = L"\x1b[2m";
	inline constexpr const wchar_t wcout_italic[] = L"\x1b[3m";
	inline constexpr const wchar_t wcout_underline[] = L"\x1b[4m";
	inline constexpr const wchar_t wcout_flashing[] = L"\x1b[5m";
	inline constexpr const wchar_t wcout_flashing_violently[] = L"\x1b[6m";
	inline constexpr const wchar_t wcout_toggle_color[] = L"\x1b[7m";
	inline constexpr const wchar_t wcout_hide[] = L"\x1b[8m";
}

//addressof
export namespace System {
	template<class T>
	constexpr T* addressof(T& arg) noexcept {
#if !defined(__GNUC__) || defined(__clang__)
	return std::addressof(arg);
#else
	return __builtin_addressof(arg);
#endif
	}
	template<class T> const T* addressof(const T&&) = delete;
}
