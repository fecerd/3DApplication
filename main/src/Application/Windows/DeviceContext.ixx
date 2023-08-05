export module DeviceContext;
import System;
import ApplicationUtility;
import WinAPI;
using namespace System;
using namespace WinAPI;

export namespace System::Application::Windows {
	class DeviceContext : public System::Application::IPaintContext {
	public:
		HDC m_hdc = nullptr;
	public:
		DeviceContext() noexcept = default;
		DeviceContext(DeviceContext&& arg) noexcept : m_hdc(arg.m_hdc) {
			arg.m_hdc = nullptr;
		}
		DeviceContext(HDC hdc) noexcept : m_hdc(hdc) {}
		~DeviceContext() noexcept { m_hdc = nullptr; }
	public:
		virtual void SetBackColor(System::Drawing::Color color) noexcept { SetBkColor(m_hdc, color); }
	public:
		virtual void DrawText(int32_t x, int32_t y, const String& str) noexcept { TextOut(m_hdc, x, y, str.w_str(), static_cast<int32_t>(str.Length())); }
	};
}