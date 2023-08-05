export module Label;
import Control;
using namespace WinAPI;

//Label
export namespace System::Application::Windows::Forms {
	class Label : public Control {
	public:
		Label() noexcept : Control(WS_CHILD | WS_VISIBLE, u"Static") {}
	};
}
