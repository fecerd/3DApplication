export module ApplicationUtility;
import System;

export namespace System::Application {
	class IPaintContext {
	public:
		IPaintContext() noexcept {}
		virtual ~IPaintContext() noexcept {}
	public:
		virtual void SetBackColor(Drawing::Color color) noexcept = 0;
		virtual void DrawText(int32_t x, int32_t y, const String& str) noexcept = 0;
	};
}

export namespace System::Application {
	enum class WindowType : uint8_t {
		Error = 0,
		Windows,
		SDL
	};

	struct WindowPlatformData {
	private:
		WindowType m_type;
		void* m_ptr = nullptr;
		Function<void(void*)> m_destructor;
	public:
		WindowPlatformData() noexcept = delete;
		WindowPlatformData(WindowType type, void* ptr, const Function<void(void*)>& destructor) noexcept
			: m_type(type), m_ptr(ptr), m_destructor(destructor) {}
		WindowPlatformData(const WindowPlatformData&) noexcept = delete;
		WindowPlatformData(WindowPlatformData&&) noexcept = default;
		~WindowPlatformData() noexcept {
			if (m_ptr && m_destructor) m_destructor(m_ptr);
			m_ptr = nullptr;
		}
	public:
		WindowType GetWindowType() const noexcept { return m_type; }
		bool HasWindowType(WindowType type) const noexcept { return m_type == type; }
		void* GetNativePtr() const noexcept { return m_ptr; }
	public:
		WindowPlatformData& operator=(const WindowPlatformData&) noexcept = delete;
		WindowPlatformData& operator=(WindowPlatformData&&) noexcept = default;
	};
}
