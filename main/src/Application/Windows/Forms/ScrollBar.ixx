export module ScrollBar;
import Control;
using namespace WinAPI;

//ScrollBar
export namespace System::Application::Windows::Forms {
	class ScrollBar : public Control {
	protected:
		SCROLLINFO info;
	public:
		ScrollBar() noexcept : Control(WS_CHILD | WS_VISIBLE, u"ScrollBar") {}
	protected:
		LRESULT WndProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept override {
			return CallDefaultProc(msg, wParam, lParam);
		}
	protected:
		void Init() override {
			Control::Init();
			info.cbSize = sizeof(SCROLLINFO);
			info.fMask = SIF_PAGE | SIF_RANGE;
			info.nMin = 0;
			info.nMax = 100;
			info.nPage = 10;
			SetScrollInfo(m_hWnd, SB_CTL, &info, true);
			info.fMask = SIF_POS;
		}
	};
}
