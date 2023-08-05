export module TextBox;
import Control;
using namespace WinAPI;

//TextBox
export namespace System::Application::Windows::Forms {
	class TextBox : public Control {
	public:
		TextBox() noexcept : Control(WS_CHILD | WS_VISIBLE | WS_BORDER, u"Edit") {}
	};
}
