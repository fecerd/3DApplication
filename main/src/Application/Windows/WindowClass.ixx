export module WindowClass;
import CSTDINT;
import WinAPI;
import Objects;
using namespace System;
using namespace WinAPI;

//WindowClassStyle
export namespace System::Application::Windows {
	enum class WindowClassStyle {
		VRedraw = CS_VREDRAW,
		HRedraw = CS_HREDRAW,
		DoubleClicks = CS_DBLCLKS,
		NoClose = CS_NOCLOSE,
		SharedDeviceContext = CS_CLASSDC,
		UniqueDeviceContext = CS_OWNDC,
		ParentDeviceContext = CS_PARENTDC,
		GlobalClass = CS_GLOBALCLASS,
		Redraw = CS_HREDRAW | CS_VREDRAW
	};
	WindowClassStyle operator|(WindowClassStyle lhs, WindowClassStyle rhs) noexcept {
		return static_cast<WindowClassStyle>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
}

//WindowClass
export namespace System::Application::Windows {
	class WindowClass {
		bool m_isRegistered = false;
		WNDCLASSEXW m_wc;
		String m_className;
		String m_menuName;
	public:
		WindowClass() noexcept = default;
		WindowClass(WindowClass&& arg) noexcept 
			: m_isRegistered(arg.m_isRegistered), m_wc(arg.m_wc), m_className(static_cast<String&&>(arg.m_className)), m_menuName(static_cast<String&&>(arg.m_menuName))
		{
			arg.m_isRegistered = false;
			arg.m_wc.lpszClassName = nullptr;
			arg.m_wc.lpszMenuName = nullptr;
			arg.m_wc.lpfnWndProc = nullptr;
			arg.m_wc.hCursor = nullptr;
			arg.m_wc.hIcon = nullptr;
			arg.m_wc.hIconSm = nullptr;
			arg.m_wc.hbrBackground = nullptr;
			arg.m_wc.hInstance = nullptr;
		}
	public:
		static WindowClass CreateWindowClass(
			const String& className, WNDPROC wndProc, HINSTANCE hInstance, HCURSOR hCursor, HICON hIcon, HICON hIconSm, HBRUSH hBackGroundBrush,
			const String& menuName, WindowClassStyle style, int classEx = 0, int windowEx = 0
		) {
			WindowClass ret;
			ret.m_className = className;
			ret.m_menuName = menuName;
			ret.m_wc.cbClsExtra = classEx;
			ret.m_wc.cbWndExtra = windowEx;
			ret.m_wc.cbSize = sizeof(WNDCLASSEXW);
			ret.m_wc.hbrBackground = hBackGroundBrush;
			ret.m_wc.hCursor = hCursor;
			ret.m_wc.hIcon = hIcon;
			ret.m_wc.hIconSm = hIconSm;
			ret.m_wc.hInstance = hInstance;
			ret.m_wc.lpfnWndProc = wndProc;
			ret.m_wc.lpszClassName = ret.m_className.w_str();
			ret.m_wc.lpszMenuName = ret.m_menuName.w_str();
			ret.m_wc.style = static_cast<uint32_t>(style);
			ret.m_isRegistered = RegisterClass(&ret.m_wc);
			return ret;
		}
		static WindowClass CreateWindowClass(const String& className) noexcept {
			WindowClass ret;
			ret.m_className = className;
			ret.m_isRegistered = true;
			return ret;
		}
	public:
		const WNDCLASSEXW* GetWNDCLASSEXW() const noexcept { return &m_wc; }
		bool IsRegistered() const noexcept { return m_isRegistered; }
		const wchar_t* GetClassName() const noexcept { return m_className.w_str(); }
		const wchar_t* GetMenuName() const noexcept { return m_menuName.w_str(); }
		bool HasNullOrEmptyClassName() const noexcept { return m_className.IsNullOrEmpty(); }
	public:
		WindowClass& operator=(const WindowClass& rhs) noexcept {
			if (this == &rhs) return *this;
			m_isRegistered = rhs.m_isRegistered;
			m_className = rhs.m_className;
			m_menuName = rhs.m_menuName;
			m_wc = rhs.m_wc;
			m_wc.lpszClassName = m_className.w_str();
			m_wc.lpszMenuName = m_menuName.w_str();
			return *this;
		}
		WindowClass& operator=(WindowClass&& rhs) noexcept {
			if (this == &rhs) return *this;
			m_isRegistered = rhs.m_isRegistered;
			m_wc = rhs.m_wc;
			m_className = static_cast<String&&>(rhs.m_className);
			m_menuName = static_cast<String&&>(rhs.m_menuName);
			rhs.m_wc.lpszClassName = nullptr;
			rhs.m_wc.lpszMenuName = nullptr;
			rhs.m_wc.lpfnWndProc = nullptr;
			rhs.m_wc.hCursor = nullptr;
			rhs.m_wc.hIcon = nullptr;
			rhs.m_wc.hIconSm = nullptr;
			rhs.m_wc.hbrBackground = nullptr;
			rhs.m_wc.hInstance = nullptr;
			return *this;
		}
	};
}
