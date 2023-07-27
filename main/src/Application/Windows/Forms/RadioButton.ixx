export module RadioButton;
import Control;
using namespace WinAPI;

//RadioButton
export namespace System::Application::Windows::Forms {
	class RadioButton : public Control {
	public:
		RadioButton() noexcept : Control(WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, u"Button") {}
	};
}
