export module IWindow;
import System;
export import EventArgs;
export import ApplicationUtility;
using namespace System;

export namespace System::Application {
	enum class CursorType : uint8_t {
		Default = 0,
		Arrow,
		IBeam,
		Wait,
		Cross,
		UpArrow,
		Size,
		Icon,
		SizeNWSE,
		SizeNESW,
		SizeWE,
		SizeNS,
		SizeAll,
		No,
		Hand,
		AppStarting,
		Help
	};

	inline constexpr int32_t DEFAULT_WINDOWPOS = -1;
	inline constexpr int32_t DEFAULT_WINDOWSIZE = -1;

	struct WindowDesc {
		String Name;
		int32_t X = DEFAULT_WINDOWPOS;
		int32_t Y = DEFAULT_WINDOWPOS;
		int32_t Width = DEFAULT_WINDOWSIZE;
		int32_t Height = DEFAULT_WINDOWSIZE;
		CursorType Cursor = CursorType::Default;
		uint64_t FixedTimeStep = 20;	//ƒ~ƒŠ•b
	};
}

//IWindow
export namespace System::Application {
	class IWindow {
		const String m_name;
	public:
		IWindow() noexcept : IWindow(WindowDesc()) {}
		IWindow(const WindowDesc& desc) noexcept;
		virtual ~IWindow() noexcept = default;
	public:
		void Show() noexcept;
		bool Close() noexcept;
		const String& GetName() const noexcept { return m_name; }
		Point<int32_t> GetPos() const noexcept;
		Size<int32_t> GetSize() const noexcept;
		void SetTitle(const String& str) noexcept;
		WindowPlatformData GetPlatformData() const noexcept;
		void SetInputFocus() noexcept;
	public:
		virtual void OnAwake() noexcept {}
		virtual void OnUpdate() noexcept {}
		virtual void OnFixedUpdate() noexcept {}
		virtual void OnInput(const InputEventArgs& e) noexcept {}
		virtual void OnKeyDown(const KeyEventArgs& e) noexcept {}
		virtual void OnKeyUp(const KeyEventArgs& e) noexcept {}
		virtual void OnMouseMove(const MouseEventArgs& e) noexcept {}
		virtual void OnResized(const ResizeEventArgs& e) noexcept {}
		virtual void OnClosing(CloseEventArgs& e) noexcept {}
		virtual void OnClosed() noexcept {}
	};
}