export module ApplicationUtility;
import System;

export namespace System::Application {
	class IPaintContext {
	public:
		IPaintContext() noexcept = default;
		virtual ~IPaintContext() noexcept = default;
	public:
		virtual void SetBackColor(System::Drawing::Color color) noexcept = 0;
		virtual void DrawText(int32_t x, int32_t y, const String& str) noexcept = 0;
	};
}

export namespace System::Application {
	enum class WindowType : uint8_t {
		Error = 0,
		Windows,
		SDL
	};

	template<class T>
	class AutoPtr {
		T* m_ptr = nullptr;
		Function<void(T*)> m_destructor;
	public:
		AutoPtr() noexcept = delete;
		AutoPtr(T* ptr, const Function<void(T*)>& destructor) noexcept
			: m_ptr(ptr), m_destructor(destructor) {}
		AutoPtr(const AutoPtr&) noexcept = delete;
		AutoPtr(AutoPtr&& arg) noexcept : m_ptr(arg.m_ptr), m_destructor(static_cast<Function<void(T*)>&&>(arg.m_destructor)) {
			arg.m_ptr = nullptr;
		}
		~AutoPtr() noexcept {
			m_destructor(m_ptr);
			m_ptr = nullptr;
		}
	public:
		T* GetData() const noexcept { return m_ptr; }
	public:
		AutoPtr& operator=(const AutoPtr&) noexcept = delete;
		AutoPtr& operator=(AutoPtr&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_destructor(m_ptr);
			m_ptr = rhs.m_ptr;
			rhs.m_ptr = nullptr;
			m_destructor = static_cast<Function<void(T*)>&&>(rhs.m_destructor);
			return *this;
		}
	};

	struct WindowPlatformData {
		WindowType Type;
		AutoPtr<void> Data;
	public:
		WindowPlatformData() noexcept = delete;
		WindowPlatformData(WindowType type, void* ptr, const Function<void(void*)>& destructor) noexcept
			: Type(type), Data(ptr, destructor) {}
		WindowPlatformData(const WindowPlatformData&) noexcept = delete;
		WindowPlatformData(WindowPlatformData&&) noexcept = default;
	public:
		WindowPlatformData& operator=(const WindowPlatformData&) noexcept = delete;
		WindowPlatformData& operator=(WindowPlatformData&&) noexcept = default;
	};
}
