export module IApplicationManager;
import System;
export import IWindow;
export import ApplicationEnum;
using namespace System;

export namespace System::Application {
	class IApplicationManager {
	protected:
		Timer m_timer;
		nanoseconds m_frameNs = static_cast<nanoseconds>(milliseconds(1000)) / 60;
	public:
		IApplicationManager() noexcept = default;
		IApplicationManager(const IApplicationManager&) noexcept = delete;
		IApplicationManager(IApplicationManager&&) noexcept = delete;
		virtual ~IApplicationManager() noexcept = default;
	public:
		Timer& GetTimer() noexcept { return m_timer; }
		void SetFPS(uint32_t fps) noexcept {
			if (fps == 0) m_frameNs = nanoseconds(0);
			else m_frameNs = static_cast<nanoseconds>(milliseconds(1000)) / fps;
		}
		uint32_t GetFPS() const noexcept {
			if (m_frameNs == nanoseconds(0)) return 0;
			else return static_cast<uint32_t>(static_cast<nanoseconds>(milliseconds(1000)) / m_frameNs);
		}
		virtual bool CreateWindow(IWindow* window, const WindowDesc& desc) noexcept = 0;
		virtual bool ShowWindow(IWindow* window) noexcept = 0;
		virtual WindowPlatformData GetWindowData(const IWindow* arg) noexcept = 0;
		virtual void UpdateWindows() noexcept = 0;
		virtual bool CloseWindow(IWindow* window) noexcept = 0;
		virtual int RunLoop() noexcept = 0;
		virtual MessageBoxResult MessageBox(IWindow* parent, const String& caption, const String& text, MessageBoxType type) noexcept = 0;
	public:
		virtual Point<int32_t> GetWindowPos(const IWindow* window) noexcept = 0;
		virtual Size<int32_t> GetWindowSize(const IWindow* window) noexcept = 0;
		virtual String GetWindowTitle(const IWindow* window) noexcept = 0;
	public:
		virtual void SetWindowTitle(IWindow* window, const String& title) noexcept = 0;
	};

	IApplicationManager& GetManager() noexcept;
}
