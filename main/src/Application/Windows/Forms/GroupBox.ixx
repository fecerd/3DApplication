export module GroupBox;
import Control;
using namespace WinAPI;

//GroupBox
export namespace System::Application::Windows::Forms {
	class GroupBox : public Control {
	public:
		GroupBox() noexcept : Control(WS_CHILD | WS_VISIBLE | BS_GROUPBOX, u"Button") {}
	};
}
