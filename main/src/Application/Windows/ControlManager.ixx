export module ControlManager;
import System;
import IApplicationManager;
import Forms;
import Log;
using namespace System;
using Control = System::Application::Windows::Forms::Control;
using Form = System::Application::Windows::Forms::Form;
using namespace WinAPI;

export namespace System::Application::Windows {
	class ControlManager : public System::Application::IApplicationManager {
		HashMap<IWindow*, Form*> m_forms;
		Vector<Form*> m_closed;
	private:
		ControlManager() noexcept = default;
	public:
		~ControlManager() noexcept = default;
	private:
		static ControlManager*& GetInterfacePtr() noexcept {
			static ControlManager* ret = new ControlManager();
			if (!ret) ret = new ControlManager();
			return ret;
		}
	public:
		static ControlManager& GetInterface() noexcept {
			return *GetInterfacePtr();
		}
		static void ReleaseInterface() noexcept {
			ControlManager*& ptr = GetInterfacePtr();
			delete ptr;
			ptr = nullptr;
		}
	public:
		static uint16_t ConvertCursorType(CursorType type) noexcept {
			switch (type) {
			case CursorType::Default: return WinAPI::IDC_ARROW;
			case CursorType::Arrow: return WinAPI::IDC_ARROW;
			case CursorType::IBeam: return WinAPI::IDC_IBEAM;
			case CursorType::Wait: return WinAPI::IDC_WAIT;
			case CursorType::Cross: return WinAPI::IDC_CROSS;
			case CursorType::UpArrow: return WinAPI::IDC_UPARROW;
			case CursorType::Size: return WinAPI::IDC_SIZE;
			case CursorType::Icon: return WinAPI::IDC_ICON;
			case CursorType::SizeNWSE: return WinAPI::IDC_SIZENWSE;
			case CursorType::SizeNESW: return WinAPI::IDC_SIZENESW;
			case CursorType::SizeWE: return WinAPI::IDC_SIZEWE;
			case CursorType::SizeNS: return WinAPI::IDC_SIZENS;
			case CursorType::SizeAll: return WinAPI::IDC_SIZEALL;
			case CursorType::No: return WinAPI::IDC_NO;
			case CursorType::Hand: return WinAPI::IDC_HAND;
			case CursorType::AppStarting: return WinAPI::IDC_APPSTARTING;
			case CursorType::Help: return WinAPI::IDC_HELP;
			default: return WinAPI::IDC_ARROW;
			}
		}
		static uint32_t ConvertMessageBoxType(MessageBoxType type) noexcept {
			switch (type) {
			case MessageBoxType::OK: return MB_OK;
			case MessageBoxType::OKCancel: return MB_OKCANCEL;
			case MessageBoxType::AbortRetryIgnore: return MB_ABORTRETRYIGNORE;
			case MessageBoxType::YesNoCancel: return MB_YESNOCANCEL;
			case MessageBoxType::YesNo: return MB_YESNO;
			case MessageBoxType::RetryCancel: return MB_RETRYCANCEL;
			case MessageBoxType::CancelTryContinue: return MB_CANCELTRYCONTINUE;
			default: return MB_OK;
			}
		}
		static MessageBoxResult ConvertWin32MessageBoxResult(int result) noexcept {
			switch (result) {
			case IDOK: return MessageBoxResult::OK;
			case IDCANCEL: return MessageBoxResult::Cancel;
			case IDABORT: return MessageBoxResult::Abort;
			case IDRETRY: return MessageBoxResult::Retry;
			case IDIGNORE: return MessageBoxResult::Ignore;
			case IDYES: return MessageBoxResult::Yes;
			case IDNO: return MessageBoxResult::No;
			case IDTRYAGAIN: return MessageBoxResult::TryAgain;
			case IDCONTINUE: return MessageBoxResult::Continue;
			default: return MessageBoxResult::OK;
			}
		}
	public:
		bool CreateWindow(IWindow* window, const WindowDesc& desc) noexcept override {
			Form* form = new Form();
			if (!m_forms.Insert(window, form)) {
				delete form;
				return false;
			}
			form->Pos = Point<int32_t>{
				desc.X == DEFAULT_WINDOWPOS ? WinAPI::CW_USEDEFAULT : desc.X,
				desc.Y == DEFAULT_WINDOWPOS ? WinAPI::CW_USEDEFAULT : desc.Y
			};
			form->Size = Size<int32_t>{
				desc.Width == DEFAULT_WINDOWSIZE ? WinAPI::CW_USEDEFAULT : desc.Width,
				desc.Height == DEFAULT_WINDOWSIZE ? WinAPI::CW_USEDEFAULT : desc.Height
			};
			form->classStyle = WindowClassStyle::Redraw;
			form->hCursor = WinAPI::LoadCursor(nullptr, ConvertCursorType(desc.Cursor));
			form->Name = desc.Name;
			form->Awake += [window](Control&, const EventArgs&) {
				window->OnAwake();
			};
			form->Update += [window](Control&, const EventArgs&) {
				window->OnUpdate();
			};
			form->FixedUpdate += [window](Control&, const EventArgs&) {
				window->OnFixedUpdate();
			};
			form->KeyDown += [window](Control&, const KeyEventArgs& e) {
				window->OnKeyDown(e);
			};
			form->KeyUp += [window](Control&, const KeyEventArgs& e) {
				window->OnKeyUp(e);
			};
			form->MouseMove += [window](Control&, const MouseEventArgs& e) {
				window->OnMouseMove(e);
			};
			form->Resized += [window](Control&, const ResizeEventArgs& e) {
				window->OnResized(e);
			};
			form->Closing += [window](Control&, CloseEventArgs& e) {
				window->OnClosing(e);
			};
			form->Closed += [this](Control& control, const EventArgs&) {
				for (auto& pair : m_forms) {
					Form* form = pair.GetValue();
					if (form == &control) {
						m_closed.Add(form);
						IWindow* window = pair.GetKey();
						if (window) window->OnClosed();
						m_forms.Remove(window);
						break;
					}
				}
			};
			form->Init();
			form->EnableTimer<milliseconds>(desc.FixedTimeStep);
			return true;
		}
		bool ShowWindow(IWindow* window) noexcept override {
			Form* form = m_forms.At(window);
			if (!form) return false;
			form->Show();
			return true;
		}
		WindowPlatformData GetWindowData(const IWindow* arg) noexcept override {
			Form* form = m_forms.At(arg);
			if (!form) return WindowPlatformData{ WindowType::Error, nullptr, [](void*) {} };
			else return WindowPlatformData{ WindowType::Windows, form->GetHWND(), [](void*) {} };
		}
		void UpdateWindows() noexcept override {
			for (Form* form : m_forms.Values()) form->RunLoopProc();
		}
		bool CloseWindow(IWindow* window) noexcept override {
			Form* form = m_forms.At(window);
			if (!form) return false;
			form->Destroy();
			return true;
		}
		int RunLoop() noexcept override {
			if (!m_forms.Count()) return 1;
			MSG msg;
			this->m_timer.Start();
			do {
				if (PeekMessage(&msg, nullptr, 0, 0, 1)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (m_closed.Count()) m_closed.DeleteAll();
				UpdateWindows();
				this->m_timer.Update();
				nanoseconds deltaNs = nanoseconds(this->m_timer.DeltaTime<nanoseconds>());
				if (this->m_frameNs > deltaNs) {
					nanoseconds sleepTime = this->m_frameNs - deltaNs;
					//System::Application::Log(String::Joint(u"Sleep: ", sleepTime.count(), u"ns"));
					Timer::Sleep(sleepTime);
				}
			} while (msg.message != WM_QUIT);
			return static_cast<int>(msg.wParam);
		}
		MessageBoxResult MessageBox(IWindow* parent, const String& caption, const String& text, MessageBoxType type) noexcept override {
			Form* form = m_forms.At(parent);
			int ret = WinAPI::MessageBox(form ? form->GetHWND() : nullptr, text.w_str(), caption.w_str(), ConvertMessageBoxType(type));
			return ConvertWin32MessageBoxResult(ret);
		}
	public:
		Point<int32_t> GetWindowPos(const IWindow* window) noexcept override {
			Form* form = m_forms.At(window);
			if (!form) return Point<int32_t>(-1, -1);
			else {
				Point<int32_t> ret{ -1, -1 };
				form->GetPos(ret.x, ret.y);
				return ret;
			}
		}
		Size<int32_t> GetWindowSize(const IWindow* window) noexcept override {
			Form* form = m_forms.At(window);
			if (!form) return Size<int32_t>(-1, -1);
			else {
				Size<int32_t> ret{ -1, -1 };
				form->GetSize(ret.width, ret.height);
				return ret;
			}
		}
		String GetWindowTitle(const IWindow* window) noexcept override {
			Form* form = m_forms.At(window);
			if (!form) return String();
			else return form->GetText();
		}
	public:
		void SetWindowTitle(IWindow* window, const String& title) noexcept override {
			Form* form = m_forms.At(window);
			if (form) form->SetText(title);
		}
	};
}