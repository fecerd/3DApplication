export module Button;
import Control;
using namespace WinAPI;

//Button
export namespace System::Application::Windows::Forms {
	class Button : public Control {
	public:
		Button() noexcept : Control(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, u"Button") {}
	};
}
