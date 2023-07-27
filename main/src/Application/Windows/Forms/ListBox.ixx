export module ListBox;
import Control;
using namespace WinAPI;

//ListBox
export namespace System::Application::Windows::Forms {
	class ListBox : public Control {
	public:
		ListBox() noexcept : Control(WS_CHILD | WS_VISIBLE | WS_BORDER, u"Listbox") {}
	};
}
