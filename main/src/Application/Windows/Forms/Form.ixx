export module Form;
import Module;
import Control;
import WinAPI;
using namespace WinAPI;

//Form
export namespace System::Application::Windows::Forms {
	class Form : public Control {
	public:
		Form() noexcept : Control(WS_OVERLAPPEDWINDOW, nullptr) {
			m_forms.Add(this);
		}
		~Form() noexcept { m_forms.RemoveValue(this); }
	private:
		inline static VectorBase<Form*> m_forms;
	public:
		static Form* GetForm(HWND hWnd) noexcept {
			for (size_t i = 0, end = m_forms.Count(); i < end; ++i) {
				if (m_forms[i]->GetHWND() == hWnd) return m_forms[i];
			}
			return nullptr;
		}
	protected:
		LRESULT WndProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept override {
			if (this->EventProc(msg, wParam, lParam) == 0) return 0;
			else return this->CallDefaultProc(msg, wParam, lParam);
		}
	};
}
