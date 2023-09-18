export module WindowsEventArgs;
import System;
export import EventArgs;
import WinAPI;
using namespace WinAPI;

namespace System::Application::Windows {
	constexpr MouseButton ToMouseButton(WPARAM wParam) noexcept {
		MouseButton ret = MouseButton::None;
		if (wParam & MK_CONTROL) ret |= MouseButton::Ctrl;
		if (wParam & MK_LBUTTON) ret |= MouseButton::Left;
		if (wParam & MK_MBUTTON) ret |= MouseButton::Middle;
		if (wParam & MK_RBUTTON) ret |= MouseButton::Right;
		if (wParam & MK_SHIFT) ret |= MouseButton::Shift;
		if (wParam & MK_XBUTTON1) ret |= MouseButton::XButton1;
		if (wParam & MK_XBUTTON2) ret |= MouseButton::XButton2;
		return ret;
	}
	constexpr ResizeType ToResizeType(WPARAM wParam) noexcept {
		switch (wParam) {
		case SIZE_RESTORED: return ResizeType::RESTORED;
		case SIZE_MINIMIZED: return ResizeType::MINIMIZED;
		case SIZE_MAXIMIZED: return ResizeType::MAXIMIZED;
		case SIZE_MAXSHOW: return ResizeType::MAXSHOW;
		case SIZE_MAXHIDE: return ResizeType::MAXHIDE;
		default: return ResizeType::UNDEFINED;
		}
	}
}

export namespace System::Application::Windows {
	constexpr MouseEventArgs ToMouseEventArgs(WPARAM wParam, LPARAM lParam, int32_t clicks = 1, int32_t delta = 0) noexcept {
		return MouseEventArgs{ ToMouseButton(wParam), clicks, LOWORD(lParam), HIWORD(lParam), delta };
	}
	constexpr System::Drawing::Rectangle ToRectangle(const RECT& rect) noexcept {
		return System::Drawing::Rectangle(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}
	constexpr KeyCode ToKeyCode(WPARAM wp) noexcept {
		switch (static_cast<uint32_t>(wp)) {
		case VK_SPACE: return KeyCode::Space;
		case VK_ESCAPE: return KeyCode::Esc;
		case VK_SHIFT: return KeyCode::Shift;
		case VK_LEFT: return KeyCode::Left;
		case VK_RIGHT: return KeyCode::Right;
		case VK_UP: return KeyCode::Up;
		case VK_DOWN: return KeyCode::Down;
		default:
			if ('A' <= wp && wp <= 'Z') return static_cast<KeyCode>(wp);
			else return KeyCode::None;
		}
	}
	constexpr KeyEventArgs ToKeyEventArgs(WPARAM wParam, LPARAM lParam) noexcept {
		return KeyEventArgs{
			ToKeyCode(wParam),
			static_cast<uint16_t>(static_cast<uint32_t>(lParam) & 0xffffu),
			static_cast<bool>(static_cast<uint32_t>(lParam) & 0x20000000u),
			static_cast<bool>(static_cast<uint32_t>(lParam) & 0x80000000u)
		};
	}
	constexpr ResizeEventArgs ToResizeEventArgs(WPARAM wParam, LPARAM lParam) noexcept {
		return ResizeEventArgs{
			ToResizeType(wParam),
			LOWORD(lParam),
			HIWORD(lParam)
		};
	}
}

