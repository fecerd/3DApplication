module;
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module Control;
export import System;
export import WinAPI;
export import Brush;
export import Color;
export import WindowClass;
export import DeviceContext;
export import WindowsEventArgs;
using namespace System;
using namespace System::Drawing;
using namespace WinAPI;

//Control
export namespace System::Application::Windows::Forms {
	class Control {
		struct PosGS {
			static Point<int32_t>& Get(Point<int32_t>& p, Control* obj) noexcept {
				HWND hWnd = obj->GetHWND();
				if (hWnd) {
					RECT rect;
					GetWindowRect(hWnd, &rect);
					p.x = rect.left;
					p.y = rect.top;
				}
				return p;
			}
			static void Set(Point<int32_t>& p, const Point<int32_t>& value, Control* obj) noexcept {
				p = value;
				HWND hWnd = obj->GetHWND();
				if (hWnd) {
					SetWindowPos(hWnd, nullptr, p.x, p.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				}
			}
		};
		struct SizeGS {
			static Size<int32_t>& Get(Size<int32_t>& p, Control* obj) noexcept {
				HWND hWnd = obj->GetHWND();
				if (hWnd) {
					RECT cr;
					GetClientRect(hWnd, &cr);
					p.width = cr.right - cr.left;
					p.height = cr.bottom - cr.top;
				}
				return p;
			}
			static void Set(Size<int32_t>& p, const Size<int32_t>& value, Control* obj) noexcept {
				p = value;
				HWND hWnd = obj->GetHWND();
				if (hWnd) {
					RECT wr, cr;
					GetWindowRect(hWnd, &wr);
					GetClientRect(hWnd, &cr);
					int32_t dw = (wr.right - wr.left) - (cr.right - cr.left);
					int32_t dh = (wr.bottom - wr.top) - (cr.bottom - cr.top);
					MoveWindow(hWnd, wr.left, wr.top, p.width + dw, p.height + dh, true);
				}
			}
		};
		struct NameGS {
			static void Set(String& p, const String& value, Control* obj) noexcept {
				p = value;
				HWND hWnd = obj->GetHWND();
				if (hWnd) SetWindowText(hWnd, p.w_str());
			}
		};
	public:
		Property<Point<int32_t>, Control, PosGS> Pos = { this, Point<int32_t>{ CW_USEDEFAULT, CW_USEDEFAULT } };
		Property<System::Size<int32_t>, Control, SizeGS> Size = { this, System::Size<int32_t>{ CW_USEDEFAULT, CW_USEDEFAULT } };
		Property<String, Control, NameGS> Name = { this };
		//Property<int32_t> X = {
		//	CW_USEDEFAULT,
		//	nullptr,
		//	[this](const int32_t value) {
		//		this->X.SetRawData(value);
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			SetWindowPos(hWnd, nullptr, this->X.GetRawData(), this->Y.GetRawData(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//		}
		//	}
		//};
		//Property<int32_t> Y = {
		//	CW_USEDEFAULT,
		//	nullptr,
		//	[this](const int32_t value) {
		//		this->Y.SetRawData(value);
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			SetWindowPos(hWnd, nullptr, this->X.GetRawData(), this->Y.GetRawData(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//		}
		//	}
		//};
		//Property<int32_t> Width = {
		//	CW_USEDEFAULT,
		//	[this]() -> int32_t& {
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			this->Width.SetRawData(rect.right - rect.left);
		//		}
		//		return this->Width.GetRawData();
		//	},
		//	[this](const int32_t value) {
		//		this->Width.SetRawData(value);
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			MoveWindow(hWnd, rect.left, rect.top, this->Width.GetRawData(), rect.bottom - rect.top, true);
		//		}
		//	}
		//};
		//Property<int32_t> Height = {
		//	CW_USEDEFAULT,
		//	[this]() -> int32_t& {
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			this->Height.SetRawData(rect.bottom - rect.top);
		//		}
		//		return this->Height.GetRawData();
		//	},
		//	[this](const int32_t value) {
		//		this->Height.SetRawData(value);
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) {
		//			RECT rect;
		//			GetWindowRect(hWnd, &rect);
		//			MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, this->Height.GetRawData(), true);
		//		}
		//	}
		//};
		//Property<String> Name = {
		//	String(),
		//	nullptr,
		//	[this](const String& value) {
		//		this->Name.SetRawData(value);
		//		HWND hWnd = this->GetHWND();
		//		if (hWnd) SetWindowText(hWnd, this->Name.GetRawData().w_str());
		//	}
		//};
	public:
		Control* Parent = nullptr;
	public:
		uint32_t WindowStyle = 0;
		uint32_t WindowStyleEx = 0;
	public:
		HICON hIcon = nullptr;
		HICON hIconSm = nullptr;
		HCURSOR hCursor = nullptr;
		HMENU hMenu = nullptr;
		Brush BackBrush;
		String menuName;
		WindowClassStyle classStyle;
	protected:
		WindowClass m_wc;
		WNDPROC m_prevWndProc = nullptr;
		size_t m_classID;
		inline static size_t nextClassID = 0;
	protected:
		HWND m_hWnd = nullptr;
	protected:
		Timer* m_timer = nullptr;
	public:
		Vector<Control*> Controls;
	public:
		EventHandler<Control&, EventArgs> Load;
		EventHandler<Control&, EventArgs> Awake;
		EventHandler<Control&, EventArgs> Click;
		EventHandler<Control&, MouseEventArgs> MouseDown;
		EventHandler<Control&, MouseEventArgs> MouseUp;
		EventHandler<Control&, MouseEventArgs> MouseMove;
		EventHandler<Control&, KeyEventArgs> KeyDown;
		EventHandler<Control&, KeyEventArgs> KeyUp;
		EventHandler<Control&, PaintEventArgs> Paint;
		EventHandler<Control&, EventArgs> Update;
		EventHandler<Control&, EventArgs> FixedUpdate;
		EventHandler<Control&, ResizeEventArgs> Resized;
		EventHandler<Control&, CloseEventArgs&> Closing;
		EventHandler<Control&, EventArgs> Closed;
	public:
		/// <summary>
		/// �V�X�e���N���X���ƃE�B���h�E�X�^�C�����w�肵��Control�I�u�W�F�N�g�𐶐�����
		/// </summary>
		/// <param name="windowStyle">�E�B���h�E�X�^�C��</param>
		/// <param name="windowClassName">�V�X�e���N���X��</param>
		Control(uint32_t windowStyle, const String& windowClassName) noexcept : WindowStyle(windowStyle) {
			if (!windowClassName.IsNullOrEmpty()) m_wc = WindowClass::CreateWindowClass(windowClassName);
		}
		Control(Control&& arg) noexcept
			: Pos(System::move(arg.Pos)), Size(System::move(arg.Size))
			, Name(System::move(arg.Name)), Parent(arg.Parent)
			, WindowStyle(arg.WindowStyle), WindowStyleEx(arg.WindowStyleEx)
			, hIcon(arg.hIcon), hIconSm(arg.hIconSm), hCursor(arg.hCursor), hMenu(arg.hMenu)
			, BackBrush(System::move(arg.BackBrush)), menuName(System::move(arg.menuName))
			, classStyle(arg.classStyle), m_wc(System::move(arg.m_wc))
			, m_prevWndProc(arg.m_prevWndProc), m_classID(arg.m_classID)
			, m_hWnd(arg.m_hWnd), Controls(System::move(arg.Controls))
		{
			arg.m_hWnd = nullptr;
		}
		virtual ~Control() {
			delete m_timer;
			m_timer = nullptr;
			for (size_t i = 0, end = Controls.Count(); i < end; ++i) delete Controls[i];
			Controls.Clear();
			if (m_hWnd) CloseControl(m_hWnd, WM_DESTROY);
		}
	protected:/* �v���Z�X���̂��ׂĂ�Control���Ǘ�����HashMap */
		static HashMap<HWND, Control*>& GetAllControls() noexcept {
			static HashMap<HWND, Control*> ret;
			return ret;
		}
		/// <summary>
		/// �E�B���h�E�n���h������Control�I�u�W�F�N�g���擾����
		/// </summary>
		/// <param name="hWnd">�E�B���h�E�n���h��</param>
		/// <returns>�w�肵���E�B���h�E�n���h��������Control�I�u�W�F�N�g�B���݂��Ȃ��ꍇ�Anullptr��Ԃ�</returns>
		static Control* GetControl(HWND hWnd) noexcept {
			return GetAllControls().At<0>(hWnd);
		}
	private:
		static HINSTANCE GetHInstance() noexcept {
			static HINSTANCE ret = GetModuleHandle(nullptr);
			return ret;
		}
		static HWND CreateControl(const String& className, int32_t x, int32_t y, int32_t width, int32_t height, const String& caption, HWND parent = nullptr, HMENU hMenu = nullptr, uint32_t windowStyle = WS_OVERLAPPEDWINDOW, uint32_t windowStyleEx = 0) noexcept {
			HWND hWnd = WinAPI::CreateWindow(windowStyleEx, className.w_str(), caption.w_str(), windowStyle, x, y, width, height, parent, hMenu, GetHInstance(), nullptr);
			return hWnd;
		}
		static void CloseControl(HWND hWnd, uint32_t msg) noexcept {
			if (msg != WM_DESTROY) DestroyWindow(hWnd);
			HashMap<HWND, Control*>& controls = GetAllControls();
			controls.Remove(hWnd);
			if (controls.Count() == 0) PostQuitMessage(0);
		}
	private:
		/// <summary>
		/// �E�B���h�E�N���X�ɓo�^�����v���V�[�W��
		/// </summary>
		static LRESULT ControlProc(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept {
			if (msg == WM_DESTROY) {
				CloseControl(hWnd, msg);
				return 0;
			}
			Control* tmp = GetControl(hWnd);
			if (!tmp) return DefWindowProc(hWnd, msg, wParam, lParam);
			else return tmp->WndProc(msg, wParam, lParam);
		}
	protected:
		/// <summary>
		/// �V�X�e���N���X�̃f�t�H���g�v���V�[�W�����Ăяo��
		/// </summary>
		LRESULT CallDefaultProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept {
			if (m_prevWndProc) return CallWindowProc(m_prevWndProc, m_hWnd, msg, wParam, lParam);
			else return DefWindowProc(m_hWnd, msg, wParam, lParam);
		}
		/// <summary>
		/// �C�x���g�֐��Ăяo���p�֐�
		/// </summary>
		/// <returns>�C�x���g�֐����Ăяo�����Ƃ��A0�B����ȊO�̏ꍇ�A1</returns>
		LRESULT EventProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept;
		/// <summary>
		/// �R���g���[�����L�̃v���V�[�W������������B
		/// �������郁�b�Z�[�W�ɑ΂��Ă�CallDefaultProc(msg, wParam, lParam)���Ăяo���Ȃ���΂Ȃ�Ȃ�
		/// </summary>
		virtual LRESULT WndProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept {
			LRESULT r = EventProc(msg, wParam, lParam);
			if (r != 0) r = CallDefaultProc(msg, wParam, lParam);
			return r;
		}
	private:
		/// <summary>
		/// �E�B���h�E�N���X��o�^����B���łɓo�^����Ă���ꍇ�A�Ȃɂ����Ȃ�
		/// </summary>
		void InitWindowClass() {
			if (m_wc.IsRegistered()) return;
			m_classID = nextClassID++;
			m_wc = WindowClass::CreateWindowClass(
				String(m_classID), Control::ControlProc, GetHInstance(),
				hCursor, hIcon, hIconSm, BackBrush.GetHBrush(), menuName, classStyle
			);
			if (!m_wc.IsRegistered()) throw System::Exception("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B");
		}
		/// <summary>
		/// �E�B���h�E�𐶐����A�E�B���h�E�v���V�[�W����ݒ肷��
		/// </summary>
		void Init_Internal();
	public:
		void RunLoopProc() {
			if (m_timer) m_timer->Update();
		}
	public:
		/// <summary>
		/// �E�B���h�E�𐶐�����B���̊֐����Ăяo���܂ŃE�B���h�E�n���h���͎擾�ł��Ȃ��B
		/// �E�B���h�E�����O�̓����ύX����ꍇ�A���̊֐����I�[�o�[���C�h����B
		/// ���̊֐����I�[�o�[���C�h����ꍇ�A���^�̂��̊֐�������ŌĂяo���Ȃ���΂Ȃ�Ȃ�
		/// </summary>
		virtual void Init() { Init_Internal(); }
	public:
		/// <summary>
		/// �q�R���g���[����ǉ�����
		/// </summary>
		/// <param name="control">
		/// ����Control�I�u�W�F�N�g�ɒǉ�����q�R���g���[���B
		/// �֐�����Init()���Ăяo����邽�߁A�ǉ��O��Init()���Ăяo���K�v�͂Ȃ�
		/// </param>
		void AddControl(Control* control) noexcept {
			if (!control) return;
			control->Parent = this;
			control->WindowStyle |= WS_CHILD;
			Controls.Add(control);
			control->Init();
		}
	public:
		/// <summary>
		/// �R���g���[����\������
		/// </summary>
		void Show() noexcept { if (m_hWnd) WinAPI::ShowWindow(m_hWnd, WinAPI::SW_SHOW); }
		void Destroy() noexcept;
	public:
		template<System::Duration T = System::milliseconds>
		void EnableTimer(uint64_t fixedInvokeStep) noexcept {
			delete m_timer;
			m_timer = new Timer();
			m_timer->FixedTimeStep<T>(fixedInvokeStep);
			m_timer->Invoke += [this] {
				this->OnUpdate(EventArgs::Empty);
			};
			m_timer->FixedInvoke += [this] {
				this->OnFixedUpdate(EventArgs::Empty);
			};
			m_timer->Start();
		}
		const System::Timer& GetTimer() const noexcept {
			return *m_timer;
		}
		void DisableTimer() noexcept {
			delete m_timer;
			m_timer = nullptr;
		}
		void SetBackBrush(Color color) { BackBrush = SolidBrush(color); }
		HMENU GetHMenu() const noexcept { return hMenu; }
		/// <summary>
		/// �E�B���h�E�n���h�����擾����
		/// </summary>
		/// <returns>����Control�I�u�W�F�N�g���w���E�B���h�E�n���h���BInit()�֐����Ăяo�����܂ł�nullptr��Ԃ�</returns>
		HWND GetHWND() const noexcept { return m_hWnd; }
	public:
		/// <summary>
		/// ���݂̃J�[�\���ʒu���擾����
		/// </summary>
		/// <param name="x">x���W��Ԃ��Q�ƕϐ�</param>
		/// <param name="y">y���W��Ԃ��Q�ƕϐ�</param>
		void GetCursorPos(int32_t& x, int32_t& y) const noexcept {
			WinAPI::POINT p;
			WinAPI::GetCursorPos(&p);
			x = p.x;
			y = p.y;
		}
		/// <summary>
		/// ���݂̃E�B���h�E�ʒu(����)���擾����
		/// </summary>
		/// <param name="x">�������W��Ԃ��Q�ƕϐ�</param>
		/// <param name="y">�������W��Ԃ��Q�ƕϐ�</param>
		void GetPos(int32_t& x, int32_t& y) const noexcept {
			const Point<int32_t>& result = Pos;
			x = result.x;
			y = result.y;
		}
		/// <summary>
		/// ���݂̃E�B���h�E�T�C�Y���擾����
		/// </summary>
		/// <param name="w">����Ԃ��Q�ƕϐ�</param>
		/// <param name="h">������Ԃ��Q�ƕϐ�</param>
		void GetSize(int32_t& width, int32_t& height) noexcept {
			const System::Size<int32_t>& result = Size;
			width = result.width;
			height = result.height;
		}
	public:
		/// <summary>
		/// Control�̖��O���擾����
		/// </summary>
		String GetText() noexcept {
			String str = String::CreateString(GetWindowTextLength(m_hWnd) + 1);	//null�I�[�������܂ޒ���
			GetWindowText(m_hWnd, str.w_str(), (int)str.Capacity());
			return str;
		}
		void SetText(const String& str) noexcept { SetWindowText(m_hWnd, str.w_str()); }
	public:
		/// �C�x���g�֐��B�I�[�o�[���C�h����ꍇ�A���N���X�̂��̊֐�������ŌĂяo���Ȃ���΂Ȃ�Ȃ�
		virtual void OnLoad(EventArgs e) noexcept { Load(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
		virtual void OnAwake(EventArgs e) noexcept { Awake(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
		virtual void OnClick(EventArgs e) noexcept { Click(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
		virtual void OnMouseDown(MouseEventArgs e) noexcept { MouseDown(static_cast<Control&>(*this), static_cast<MouseEventArgs&&>(e)); }
		virtual void OnMouseUp(MouseEventArgs e) noexcept { MouseUp(static_cast<Control&>(*this), static_cast<MouseEventArgs&&>(e)); }
		virtual void OnMouseMove(MouseEventArgs e) noexcept { MouseMove(static_cast<Control&>(*this), static_cast<MouseEventArgs&&>(e)); }
		virtual void OnKeyDown(KeyEventArgs e) noexcept { KeyDown(static_cast<Control&>(*this), static_cast<KeyEventArgs&&>(e)); }
		virtual void OnKeyUp(KeyEventArgs e) noexcept { KeyUp(static_cast<Control&>(*this), static_cast<KeyEventArgs&&>(e)); }
		virtual void OnPaint(PaintEventArgs e) noexcept { Paint(static_cast<Control&>(*this), static_cast<PaintEventArgs&&>(e)); }		
		virtual void OnUpdate(EventArgs e) noexcept { Update(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
		virtual void OnFixedUpdate(EventArgs e) noexcept { FixedUpdate(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
		virtual void OnResized(ResizeEventArgs e) noexcept { Resized(static_cast<Control&>(*this), static_cast<ResizeEventArgs&&>(e)); }
		virtual void OnClosing(CloseEventArgs& e) noexcept { Closing(static_cast<Control&>(*this), e); }
		virtual void OnClosed(EventArgs e) noexcept { Closed(static_cast<Control&>(*this), static_cast<EventArgs&&>(e)); }
	};
}