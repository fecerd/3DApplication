export module SDLWindowManager;
import System;
import IWindow;
import ApplicationEnum;
export import IApplicationManager;

export namespace System::Application::SDL {
	class SDLWindowManager : public IApplicationManager {
	private:
		SDLWindowManager() noexcept {}
		~SDLWindowManager() noexcept {}
	private:
		static SDLWindowManager*& GetInterfacePtr() noexcept {
			static SDLWindowManager* ret = new SDLWindowManager();
			if (!ret) ret = new SDLWindowManager();
			return ret;
		}
	public:
		static SDLWindowManager& GetInterface() noexcept {
			return *GetInterfacePtr();
		}
		static void ReleaseInterface() noexcept {
			SDLWindowManager*& ptr = GetInterfacePtr();
			delete ptr;
			ptr = nullptr;
		}
	public:
		bool CreateWindow(IWindow* window, const WindowDesc& desc) noexcept override {
			/*Form* form = new Form();
			if (!m_forms.Insert(window, form)) {
				delete form;
				return false;
			}
			form->X = desc.X < 0 ? WinAPI::CW_USEDEFAULT : desc.X;
			form->Y = desc.Y < 0 ? WinAPI::CW_USEDEFAULT : desc.Y;
			form->Width = desc.Width < 0 ? WinAPI::CW_USEDEFAULT : desc.Width;
			form->Height = desc.Height < 0 ? WinAPI::CW_USEDEFAULT : desc.Height;
			form->classStyle = WindowClassStyle::Redraw;
			form->hCursor = WinAPI::LoadCursor(nullptr, ConvertCursorType(desc.Cursor));
			form->Name = desc.Name;
			form->Load += [window](Control&, const EventArgs&) {
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
					if (pair.GetValue() == &control) {
						m_closed.Add(pair.GetValue());
						m_forms.Remove(pair.GetKey());
						break;
					}
				}
			};
			form->Init();
			form->EnableTimer<milliseconds>(desc.FixedTimeStep);*/
			return true;
		}
		bool ShowWindow(IWindow* window) noexcept override {
			/*Form* form = m_forms.At(window);
			if (!form) return false;
			form->Show();*/
			return true;
		}
		WindowPlatformData GetWindowData(const IWindow* arg) noexcept override {
			//Form* form = m_forms.At(arg);
			//if (!form || type != WindowType::Windows) return WindowPlatformData{ WindowType::Error, nullptr, [](void*) {} };
			//else return WindowPlatformData{ type, form->GetHWND(), [](void*) {} };
			return WindowPlatformData{ WindowType::Error, nullptr, [](void*) {} };
		}
		void UpdateWindows() noexcept override {
			//for (Form* form : m_forms.Values()) form->RunLoopProc();
		}
		bool CloseWindow(IWindow* window) noexcept override {
			/*Form* form = m_forms.At(window);
			if (!form) return false;
			m_forms.Remove(window);
			delete form;*/
			return true;
		}
		int RunLoop() noexcept override {
			/*if (!m_forms.Count()) return 1;
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
				uint64_t delta = this->m_timer.DeltaTime<milliseconds>();
				Timer::Sleep<nanoseconds>(1000000);
			} while (msg.message != WM_QUIT);
			this->m_timer.Invoke.Clear();
			return static_cast<int>(msg.wParam);*/
			return 0;
		}
		MessageBoxResult MessageBox(IWindow* parent, const String& caption, const String& text, MessageBoxType type) noexcept override {
			/*Form* form = m_forms.At(parent);
			int ret = WinAPI::MessageBox(form ? form->GetHWND() : nullptr, text.w_str(), caption.w_str(), ConvertMessageBoxType(type));
			return ConvertWin32MessageBoxResult(ret);*/
			return MessageBoxResult::OK;
		}
	public:
		Point<int32_t> GetWindowPos(const IWindow* window) noexcept override {
			/*Form* form = m_forms.At(window);
			if (!form) return Point<int32_t>(-1, -1);
			else {
				Point<int32_t> ret{ -1, -1 };
				form->GetPos(ret.x, ret.y);
				return ret;
			}*/
			return Point<int32_t>(-1, -1);
		}
		Size<int32_t> GetWindowSize(const IWindow* window) noexcept override {
			/*Form* form = m_forms.At(window);
			if (!form) return Point<int32_t>(-1, -1);
			else {
				Point<int32_t> ret{ -1, -1 };
				form->GetSize(ret.x, ret.y);
				return ret;
			}*/
			return Size<int32_t>(-1, -1);
		}
		String GetWindowTitle(const IWindow* window) noexcept override {
			/*Form* form = m_forms.At(window);
			if (!form) return String();
			else return form->GetText();*/
			return String();
		}
	public:
		void SetWindowTitle(IWindow* window, const String& title) noexcept override {
			/*Form* form = m_forms.At(window);
			if (form) form->SetText(title);*/
		}
	};
}
