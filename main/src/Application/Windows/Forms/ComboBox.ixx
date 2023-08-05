export module ComboBox;
import Control;
using namespace WinAPI;

//ComboBox
export namespace System::Application::Windows::Forms {
	class ComboBox : public Control {
	public:
		ComboBox() noexcept : Control(WS_CHILD | WS_VISIBLE | CBS_SIMPLE, u"Combobox") {}
	};
}