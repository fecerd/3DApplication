export module CheckBox;
import Control;
using namespace WinAPI;

//CheckBox
export namespace System::Application::Windows::Forms {
	class CheckBox : public Control {
	public:
		CheckBox() noexcept : Control(WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, u"Button") {}
	};
}
