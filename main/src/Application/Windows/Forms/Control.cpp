module Control;
import Log;
using namespace System;
using namespace WinAPI;
using namespace System::Application::Windows;
using Control = System::Application::Windows::Forms::Control;

CString<30> GetWindowMessageName(uint32_t msg) {
	switch (msg) {
	case WM_NULL: return CString<30>(u"WM_NULL", 0);
	case WM_CREATE: return CString<30>(u"WM_CREATE", 0);
	case WM_DESTROY: return CString<30>(u"WM_DESTROY", 0);
	case WM_MOVE: return CString<30>(u"WM_MOVE", 0);
	case WM_SIZE: return CString<30>(u"WM_SIZE", 0);
	case WM_ACTIVATE: return CString<30>(u"WM_ACTIVATE", 0);
	case WM_SETFOCUS: return CString<30>(u"WM_SETFOCUS", 0);
	case WM_KILLFOCUS: return CString<30>(u"WM_KILLFOCUS", 0);
	case WM_ENABLE: return CString<30>(u"WM_ENABLE", 0);
	case WM_SETREDRAW: return CString<30>(u"WM_SETREDRAW", 0);
	case WM_SETTEXT: return CString<30>(u"WM_SETTEXT", 0);
	case WM_GETTEXT: return CString<30>(u"WM_GETTEXT", 0);
	case WM_GETTEXTLENGTH: return CString<30>(u"WM_GETTEXTLENGTH", 0);
	case WM_PAINT: return CString<30>(u"WM_PAINT", 0);
	case WM_CLOSE: return CString<30>(u"WM_CLOSE", 0);
	case WM_QUERYENDSESSION: return CString<30>(u"WM_QUERYENDSESSION", 0);
	case WM_QUERYOPEN: return CString<30>(u"WM_QUERYOPEN", 0);
	case WM_ENDSESSION: return CString<30>(u"WM_ENDSESSION", 0);
	case WM_QUIT: return CString<30>(u"WM_QUIT", 0);
	case WM_ERASEBKGND: return CString<30>(u"WM_ERASEBKGND", 0);
	case WM_SYSCOLORCHANGE: return CString<30>(u"WM_SYSCOLORCHANGE", 0);
	case WM_SHOWWINDOW: return CString<30>(u"WM_SHOWWINDOW", 0);
	case WM_WININICHANGE: return CString<30>(u"WM_WININICHANGE", 0);
	case WM_DEVMODECHANGE: return CString<30>(u"WM_DEVMODECHANGE", 0);
	case WM_ACTIVATEAPP: return CString<30>(u"WM_ACTIVATEAPP", 0);
	case WM_FONTCHANGE: return CString<30>(u"WM_FONTCHANGE", 0);
	case WM_TIMECHANGE: return CString<30>(u"WM_TIMECHANGE", 0);
	case WM_CANCELMODE: return CString<30>(u"WM_CANCELMODE", 0);
	case WM_SETCURSOR: return CString<30>(u"WM_SETCURSOR", 0);
	case WM_MOUSEACTIVATE: return CString<30>(u"WM_MOUSEACTIVATE", 0);
	case WM_CHILDACTIVATE: return CString<30>(u"WM_CHILDACTIVATE", 0);
	case WM_QUEUESYNC: return CString<30>(u"WM_QUEUESYNC", 0);
	case WM_GETMINMAXINFO: return CString<30>(u"WM_GETMINMAXINFO", 0);
	case WM_PAINTICON: return CString<30>(u"WM_PAINTICON", 0);
	case WM_ICONERASEBKGND: return CString<30>(u"WM_ICONERASEBKGND", 0);
	case WM_NEXTDLGCTL: return CString<30>(u"WM_NEXTDLGCTL", 0);
	case WM_SPOOLERSTATUS: return CString<30>(u"WM_SPOOLERSTATUS", 0);
	case WM_DRAWITEM: return CString<30>(u"WM_DRAWITEM", 0);
	case WM_MEASUREITEM: return CString<30>(u"WM_MEASUREITEM", 0);
	case WM_DELETEITEM: return CString<30>(u"WM_DELETEITEM", 0);
	case WM_VKEYTOITEM: return CString<30>(u"WM_VKEYTOITEM", 0);
	case WM_CHARTOITEM: return CString<30>(u"WM_CHARTOITEM", 0);
	case WM_SETFONT: return CString<30>(u"WM_SETFONT", 0);
	case WM_GETFONT: return CString<30>(u"WM_GETFONT", 0);
	case WM_SETHOTKEY: return CString<30>(u"WM_SETHOTKEY", 0);
	case WM_GETHOTKEY: return CString<30>(u"WM_GETHOTKEY", 0);
	case WM_QUERYDRAGICON: return CString<30>(u"WM_QUERYDRAGICON", 0);
	case WM_COMPAREITEM: return CString<30>(u"WM_COMPAREITEM", 0);
	case WM_GETOBJECT: return CString<30>(u"WM_GETOBJECT", 0);
	case WM_COMPACTING: return CString<30>(u"WM_COMPACTING", 0);
	case WM_COMMNOTIFY: return CString<30>(u"WM_COMMNOTIFY", 0);
	case WM_WINDOWPOSCHANGING: return CString<30>(u"WM_WINDOWPOSCHANGING", 0);
	case WM_WINDOWPOSCHANGED: return CString<30>(u"WM_WINDOWPOSCHANGED", 0);
	case WM_POWER: return CString<30>(u"WM_POWER", 0);
	case WM_COPYDATA: return CString<30>(u"WM_COPYDATA", 0);
	case WM_CANCELJOURNAL: return CString<30>(u"WM_CANCELJOURNAL", 0);
	case WM_NOTIFY: return CString<30>(u"WM_NOTIFY", 0);
	case WM_INPUTLANGCHANGEREQUEST: return CString<30>(u"WM_INPUTLANGCHANGEREQUEST", 0);
	case WM_INPUTLANGCHANGE: return CString<30>(u"WM_INPUTLANGCHANGE", 0);
	case WM_TCARD: return CString<30>(u"WM_TCARD", 0);
	case WM_HELP: return CString<30>(u"WM_HELP", 0);
	case WM_USERCHANGED: return CString<30>(u"WM_USERCHANGED", 0);
	case WM_NOTIFYFORMAT: return CString<30>(u"WM_NOTIFYFORMAT", 0);
	case WM_CONTEXTMENU: return CString<30>(u"WM_CONTEXTMENU", 0);
	case WM_STYLECHANGING: return CString<30>(u"WM_STYLECHANGING", 0);
	case WM_STYLECHANGED: return CString<30>(u"WM_STYLECHANGED", 0);
	case WM_DISPLAYCHANGE: return CString<30>(u"WM_DISPLAYCHANGE", 0);
	case WM_GETICON: return CString<30>(u"WM_GETICON", 0);
	case WM_SETICON: return CString<30>(u"WM_SETICON", 0);
	case WM_NCCREATE: return CString<30>(u"WM_NCCREATE", 0);
	case WM_NCDESTROY: return CString<30>(u"WM_NCDESTROY", 0);
	case WM_NCCALCSIZE: return CString<30>(u"WM_NCCALCSIZE", 0);
	case WM_NCHITTEST: return CString<30>(u"WM_NCHITTEST", 0);
	case WM_NCPAINT: return CString<30>(u"WM_NCPAINT", 0);
	case WM_NCACTIVATE: return CString<30>(u"WM_NCACTIVATE", 0);
	case WM_GETDLGCODE: return CString<30>(u"WM_GETDLGCODE", 0);
	case WM_SYNCPAINT: return CString<30>(u"WM_SYNCPAINT", 0);
	case WM_NCMOUSEMOVE: return CString<30>(u"WM_NCMOUSEMOVE", 0);
	case WM_NCLBUTTONDOWN: return CString<30>(u"WM_NCLBUTTONDOWN", 0);
	case WM_NCLBUTTONUP: return CString<30>(u"WM_NCLBUTTONUP", 0);
	case WM_NCLBUTTONDBLCLK: return CString<30>(u"WM_NCLBUTTONDBLCLK", 0);
	case WM_NCRBUTTONDOWN: return CString<30>(u"WM_NCRBUTTONDOWN", 0);
	case WM_NCRBUTTONUP: return CString<30>(u"WM_NCRBUTTONUP", 0);
	case WM_NCRBUTTONDBLCLK: return CString<30>(u"WM_NCRBUTTONDBLCLK", 0);
	case WM_NCMBUTTONDOWN: return CString<30>(u"WM_NCMBUTTONDOWN", 0);
	case WM_NCMBUTTONUP: return CString<30>(u"WM_NCMBUTTONUP", 0);
	case WM_NCMBUTTONDBLCLK: return CString<30>(u"WM_NCMBUTTONDBLCLK", 0);
	case WM_NCXBUTTONDOWN: return CString<30>(u"WM_NCXBUTTONDOWN", 0);
	case WM_NCXBUTTONUP: return CString<30>(u"WM_NCXBUTTONUP", 0);
	case WM_NCXBUTTONDBLCLK: return CString<30>(u"WM_NCXBUTTONDBLCLK", 0);
	case WM_INPUT_DEVICE_CHANGE: return CString<30>(u"WM_INPUT_DEVICE_CHANGE", 0);
	case WM_INPUT: return CString<30>(u"WM_INPUT", 0);
	case WM_KEYDOWN: return CString<30>(u"WM_KEYDOWN", 0);
	case WM_KEYUP: return CString<30>(u"WM_KEYUP", 0);
	case WM_CHAR: return CString<30>(u"WM_CHAR", 0);
	case WM_DEADCHAR: return CString<30>(u"WM_DEADCHAR", 0);
	case WM_SYSKEYDOWN: return CString<30>(u"WM_SYSKEYDOWN", 0);
	case WM_SYSKEYUP: return CString<30>(u"WM_SYSKEYUP", 0);
	case WM_SYSCHAR: return CString<30>(u"WM_SYSCHAR", 0);
	case WM_SYSDEADCHAR: return CString<30>(u"WM_SYSDEADCHAR", 0);
	case WM_KEYLAST: return CString<30>(u"WM_KEYLAST", 0);
	case WM_IME_STARTCOMPOSITION: return CString<30>(u"WM_IME_STARTCOMPOSITION", 0);
	case WM_IME_ENDCOMPOSITION: return CString<30>(u"WM_IME_ENDCOMPOSITION", 0);
	case WM_IME_COMPOSITION: return CString<30>(u"WM_IME_COMPOSITION", 0);
	case WM_INITDIALOG: return CString<30>(u"WM_INITDIALOG", 0);
	case WM_COMMAND: return CString<30>(u"WM_COMMAND", 0);
	case WM_SYSCOMMAND: return CString<30>(u"WM_SYSCOMMAND", 0);
	case WM_TIMER: return CString<30>(u"WM_TIMER", 0);
	case WM_HSCROLL: return CString<30>(u"WM_HSCROLL", 0);
	case WM_VSCROLL: return CString<30>(u"WM_VSCROLL", 0);
	case WM_INITMENU: return CString<30>(u"WM_INITMENU", 0);
	case WM_INITMENUPOPUP: return CString<30>(u"WM_INITMENUPOPUP", 0);
	case WM_GESTURE: return CString<30>(u"WM_GESTURE", 0);
	case WM_GESTURENOTIFY: return CString<30>(u"WM_GESTURENOTIFY", 0);
	case WM_MENUSELECT: return CString<30>(u"WM_MENUSELECT", 0);
	case WM_MENUCHAR: return CString<30>(u"WM_MENUCHAR", 0);
	case WM_ENTERIDLE: return CString<30>(u"WM_ENTERIDLE", 0);
	case WM_MENURBUTTONUP: return CString<30>(u"WM_MENURBUTTONUP", 0);
	case WM_MENUDRAG: return CString<30>(u"WM_MENUDRAG", 0);
	case WM_MENUGETOBJECT: return CString<30>(u"WM_MENUGETOBJECT", 0);
	case WM_UNINITMENUPOPUP: return CString<30>(u"WM_UNINITMENUPOPUP", 0);
	case WM_MENUCOMMAND: return CString<30>(u"WM_MENUCOMMAND", 0);
	case WM_CHANGEUISTATE: return CString<30>(u"WM_CHANGEUISTATE", 0);
	case WM_UPDATEUISTATE: return CString<30>(u"WM_UPDATEUISTATE", 0);
	case WM_QUERYUISTATE: return CString<30>(u"WM_QUERYUISTATE", 0);
	case WM_CTLCOLORMSGBOX: return CString<30>(u"WM_CTLCOLORMSGBOX", 0);
	case WM_CTLCOLOREDIT: return CString<30>(u"WM_CTLCOLOREDIT", 0);
	case WM_CTLCOLORLISTBOX: return CString<30>(u"WM_CTLCOLORLISTBOX", 0);
	case WM_CTLCOLORBTN: return CString<30>(u"WM_CTLCOLORBTN", 0);
	case WM_CTLCOLORDLG: return CString<30>(u"WM_CTLCOLORDLG", 0);
	case WM_CTLCOLORSCROLLBAR: return CString<30>(u"WM_CTLCOLORSCROLLBAR", 0);
	case WM_CTLCOLORSTATIC: return CString<30>(u"WM_CTLCOLORSTATIC", 0);
	case WM_MOUSEMOVE: return CString<30>(u"WM_MOUSEMOVE", 0);
	case WM_LBUTTONDOWN: return CString<30>(u"WM_LBUTTONDOWN", 0);
	case WM_LBUTTONUP: return CString<30>(u"WM_LBUTTONUP", 0);
	case WM_LBUTTONDBLCLK: return CString<30>(u"WM_LBUTTONDBLCLK", 0);
	case WM_RBUTTONDOWN: return CString<30>(u"WM_RBUTTONDOWN", 0);
	case WM_RBUTTONUP: return CString<30>(u"WM_RBUTTONUP", 0);
	case WM_RBUTTONDBLCLK: return CString<30>(u"WM_RBUTTONDBLCLK", 0);
	case WM_MBUTTONDOWN: return CString<30>(u"WM_MBUTTONDOWN", 0);
	case WM_MBUTTONUP: return CString<30>(u"WM_MBUTTONUP", 0);
	case WM_MBUTTONDBLCLK: return CString<30>(u"WM_MBUTTONDBLCLK", 0);
	case WM_MOUSEWHEEL: return CString<30>(u"WM_MOUSEWHEEL", 0);
	case WM_XBUTTONDOWN: return CString<30>(u"WM_XBUTTONDOWN", 0);
	case WM_XBUTTONUP: return CString<30>(u"WM_XBUTTONUP", 0);
	case WM_XBUTTONDBLCLK: return CString<30>(u"WM_XBUTTONDBLCLK", 0);
	case WM_MOUSEHWHEEL: return CString<30>(u"WM_MOUSEHWHEEL", 0);
	case WM_PARENTNOTIFY: return CString<30>(u"WM_PARENTNOTIFY", 0);
	case WM_ENTERMENULOOP: return CString<30>(u"WM_ENTERMENULOOP", 0);
	case WM_EXITMENULOOP: return CString<30>(u"WM_EXITMENULOOP", 0);
	case WM_NEXTMENU: return CString<30>(u"WM_NEXTMENU", 0);
	case WM_SIZING: return CString<30>(u"WM_SIZING", 0);
	case WM_CAPTURECHANGED: return CString<30>(u"WM_CAPTURECHANGED", 0);
	case WM_MOVING: return CString<30>(u"WM_MOVING", 0);
	case WM_POWERBROADCAST: return CString<30>(u"WM_POWERBROADCAST", 0);
	case WM_DEVICECHANGE: return CString<30>(u"WM_DEVICECHANGE", 0);
	case WM_MDICREATE: return CString<30>(u"WM_MDICREATE", 0);
	case WM_MDIDESTROY: return CString<30>(u"WM_MDIDESTROY", 0);
	case WM_MDIACTIVATE: return CString<30>(u"WM_MDIACTIVATE", 0);
	case WM_MDIRESTORE: return CString<30>(u"WM_MDIRESTORE", 0);
	case WM_MDINEXT: return CString<30>(u"WM_MDINEXT", 0);
	case WM_MDIMAXIMIZE: return CString<30>(u"WM_MDIMAXIMIZE", 0);
	case WM_MDITILE: return CString<30>(u"WM_MDITILE", 0);
	case WM_MDICASCADE: return CString<30>(u"WM_MDICASCADE", 0);
	case WM_MDIICONARRANGE: return CString<30>(u"WM_MDIICONARRANGE", 0);
	case WM_MDIGETACTIVE: return CString<30>(u"WM_MDIGETACTIVE", 0);
	case WM_MDISETMENU: return CString<30>(u"WM_MDISETMENU", 0);
	case WM_ENTERSIZEMOVE: return CString<30>(u"WM_ENTERSIZEMOVE", 0);
	case WM_EXITSIZEMOVE: return CString<30>(u"WM_EXITSIZEMOVE", 0);
	case WM_DROPFILES: return CString<30>(u"WM_DROPFILES", 0);
	case WM_MDIREFRESHMENU: return CString<30>(u"WM_MDIREFRESHMENU", 0);
	case WM_POINTERDEVICECHANGE: return CString<30>(u"WM_POINTERDEVICECHANGE", 0);
	case WM_POINTERDEVICEINRANGE: return CString<30>(u"WM_POINTERDEVICEINRANGE", 0);
	case WM_POINTERDEVICEOUTOFRANGE: return CString<30>(u"WM_POINTERDEVICEOUTOFRANGE", 0);
	case WM_TOUCH: return CString<30>(u"WM_TOUCH", 0);
	case WM_NCPOINTERUPDATE: return CString<30>(u"WM_NCPOINTERUPDATE", 0);
	case WM_NCPOINTERDOWN: return CString<30>(u"WM_NCPOINTERDOWN", 0);
	case WM_NCPOINTERUP: return CString<30>(u"WM_NCPOINTERUP", 0);
	case WM_POINTERUPDATE: return CString<30>(u"WM_POINTERUPDATE", 0);
	case WM_POINTERDOWN: return CString<30>(u"WM_POINTERDOWN", 0);
	case WM_POINTERUP: return CString<30>(u"WM_POINTERUP", 0);
	case WM_POINTERENTER: return CString<30>(u"WM_POINTERENTER", 0);
	case WM_POINTERLEAVE: return CString<30>(u"WM_POINTERLEAVE", 0);
	case WM_POINTERACTIVATE: return CString<30>(u"WM_POINTERACTIVATE", 0);
	case WM_POINTERCAPTURECHANGED: return CString<30>(u"WM_POINTERCAPTURECHANGED", 0);
	case WM_TOUCHHITTESTING: return CString<30>(u"WM_TOUCHHITTESTING", 0);
	case WM_POINTERWHEEL: return CString<30>(u"WM_POINTERWHEEL", 0);
	case WM_POINTERHWHEEL: return CString<30>(u"WM_POINTERHWHEEL", 0);
	case WM_POINTERROUTEDTO: return CString<30>(u"WM_POINTERROUTEDTO", 0);
	case WM_POINTERROUTEDAWAY: return CString<30>(u"WM_POINTERROUTEDAWAY", 0);
	case WM_POINTERROUTEDRELEASED: return CString<30>(u"WM_POINTERROUTEDRELEASED", 0);
	case WM_IME_SETCONTEXT: return CString<30>(u"WM_IME_SETCONTEXT", 0);
	case WM_IME_NOTIFY: return CString<30>(u"WM_IME_NOTIFY", 0);
	case WM_IME_CONTROL: return CString<30>(u"WM_IME_CONTROL", 0);
	case WM_IME_COMPOSITIONFULL: return CString<30>(u"WM_IME_COMPOSITIONFULL", 0);
	case WM_IME_SELECT: return CString<30>(u"WM_IME_SELECT", 0);
	case WM_IME_CHAR: return CString<30>(u"WM_IME_CHAR", 0);
	case WM_IME_REQUEST: return CString<30>(u"WM_IME_REQUEST", 0);
	case WM_IME_KEYDOWN: return CString<30>(u"WM_IME_KEYDOWN", 0);
	case WM_IME_KEYUP: return CString<30>(u"WM_IME_KEYUP", 0);
	case WM_MOUSEHOVER: return CString<30>(u"WM_MOUSEHOVER", 0);
	case WM_MOUSELEAVE: return CString<30>(u"WM_MOUSELEAVE", 0);
	case WM_NCMOUSEHOVER: return CString<30>(u"WM_NCMOUSEHOVER", 0);
	case WM_NCMOUSELEAVE: return CString<30>(u"WM_NCMOUSELEAVE", 0);
	case WM_WTSSESSION_CHANGE: return CString<30>(u"WM_WTSSESSION_CHANGE", 0);
	case WM_TABLET_FIRST: return CString<30>(u"WM_TABLET_FIRST", 0);
	case WM_TABLET_LAST: return CString<30>(u"WM_TABLET_LAST", 0);
	case WM_DPICHANGED: return CString<30>(u"WM_DPICHANGED", 0);
	case WM_DPICHANGED_BEFOREPARENT: return CString<30>(u"WM_DPICHANGED_BEFOREPARENT", 0);
	case WM_DPICHANGED_AFTERPARENT: return CString<30>(u"WM_DPICHANGED_AFTERPARENT", 0);
	case WM_GETDPISCALEDSIZE: return CString<30>(u"WM_GETDPISCALEDSIZE", 0);
	case WM_CUT: return CString<30>(u"WM_CUT", 0);
	case WM_COPY: return CString<30>(u"WM_COPY", 0);
	case WM_PASTE: return CString<30>(u"WM_PASTE", 0);
	case WM_CLEAR: return CString<30>(u"WM_CLEAR", 0);
	case WM_UNDO: return CString<30>(u"WM_UNDO", 0);
	case WM_RENDERFORMAT: return CString<30>(u"WM_RENDERFORMAT", 0);
	case WM_RENDERALLFORMATS: return CString<30>(u"WM_RENDERALLFORMATS", 0);
	case WM_DESTROYCLIPBOARD: return CString<30>(u"WM_DESTROYCLIPBOARD", 0);
	case WM_DRAWCLIPBOARD: return CString<30>(u"WM_DRAWCLIPBOARD", 0);
	case WM_PAINTCLIPBOARD: return CString<30>(u"WM_PAINTCLIPBOARD", 0);
	case WM_VSCROLLCLIPBOARD: return CString<30>(u"WM_VSCROLLCLIPBOARD", 0);
	case WM_SIZECLIPBOARD: return CString<30>(u"WM_SIZECLIPBOARD", 0);
	case WM_ASKCBFORMATNAME: return CString<30>(u"WM_ASKCBFORMATNAME", 0);
	case WM_CHANGECBCHAIN: return CString<30>(u"WM_CHANGECBCHAIN", 0);
	case WM_HSCROLLCLIPBOARD: return CString<30>(u"WM_HSCROLLCLIPBOARD", 0);
	case WM_QUERYNEWPALETTE: return CString<30>(u"WM_QUERYNEWPALETTE", 0);
	case WM_PALETTEISCHANGING: return CString<30>(u"WM_PALETTEISCHANGING", 0);
	case WM_PALETTECHANGED: return CString<30>(u"WM_PALETTECHANGED", 0);
	case WM_HOTKEY: return CString<30>(u"WM_HOTKEY", 0);
	case WM_PRINT: return CString<30>(u"WM_PRINT", 0);
	case WM_PRINTCLIENT: return CString<30>(u"WM_PRINTCLIENT", 0);
	case WM_APPCOMMAND: return CString<30>(u"WM_APPCOMMAND", 0);
	case WM_THEMECHANGED: return CString<30>(u"WM_THEMECHANGED", 0);
	case WM_CLIPBOARDUPDATE: return CString<30>(u"WM_CLIPBOARDUPDATE", 0);
	case WM_DWMCOMPOSITIONCHANGED: return CString<30>(u"WM_DWMCOMPOSITIONCHANGED", 0);
	case WM_DWMNCRENDERINGCHANGED: return CString<30>(u"WM_DWMNCRENDERINGCHANGED", 0);
	case WM_DWMCOLORIZATIONCOLORCHANGED: return CString<30>(u"WM_DWMCOLORIZATIONCOLORCHANGED", 0);
	case WM_DWMWINDOWMAXIMIZEDCHANGE: return CString<30>(u"WM_DWMWINDOWMAXIMIZEDCHANGE", 0);
	case WM_DWMSENDICONICTHUMBNAIL: return CString<30>(u"WM_DWMSENDICONICTHUMBNAIL", 0);
	case WM_DWMSENDICONICLIVEPREVIEWBITMAP: return CString<30>(u"WM_DWMSENDICONICLIVEPREVIEWBITMAP", 0);
	case WM_GETTITLEBARINFOEX: return CString<30>(u"WM_GETTITLEBARINFOEX", 0);
	case WM_HANDHELDFIRST: return CString<30>(u"WM_HANDHELDFIRST", 0);
	case WM_HANDHELDLAST: return CString<30>(u"WM_HANDHELDLAST", 0);
	case WM_AFXFIRST: return CString<30>(u"WM_AFXFIRST", 0);
	case WM_AFXLAST: return CString<30>(u"WM_AFXLAST", 0);
	case WM_PENWINFIRST: return CString<30>(u"WM_PENWINFIRST", 0);
	case WM_PENWINLAST: return CString<30>(u"WM_PENWINLAST", 0);
	case WM_APP: return CString<30>(u"WM_APP", 0);
	case WM_USER: return CString<30>(u"WM_USER", 0);
	default: return CString<30>(u"NODEFINITION", 0);
	}
}

namespace WinAPI::Extension {
	inline constexpr uint16_t WM_USER_AWAKE = 0x8000;
}

void Control::Init_Internal() {
	if (m_hWnd) return;
	InitWindowClass();
	const Point<int32_t> pos = this->Pos;
	const System::Size<int32_t> size = this->Size;
	m_hWnd = CreateControl(m_wc.GetClassName(), pos.x, pos.y, size.width, size.height, this->Name, Parent ? Parent->GetHWND() : nullptr, hMenu, WindowStyle, WindowStyleEx);
	if (!m_hWnd) throw System::Exception("ウィンドウの生成に失敗しました。");
	if (m_wc.GetWNDCLASSEXW()->lpfnWndProc != ControlProc) {
		m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLong(m_hWnd, GWL_WNDPROC, reinterpret_cast<int64_t>(ControlProc)));
		if (!m_prevWndProc) throw System::Exception("ウィンドウプロシージャの設定に失敗しました。");
	}
	GetAllControls().Insert(m_hWnd, this);

	//WinAPIのCreateWindowはウィンドウサイズ指定なので、
	//ここでクライアントサイズが指定したサイズになるようにリサイズ
	this->Size = size;

	CREATESTRUCTW cs;
	cs.dwExStyle = WindowStyleEx;
	cs.lpszClass = m_wc.GetClassName();
	cs.lpszName = Name->w_str();
	cs.style = WindowStyle;
	cs.x = pos.x;
	cs.y = pos.y;
	cs.cx = size.width;
	cs.cy = size.height;
	cs.hwndParent = Parent ? Parent->GetHWND() : nullptr;
	cs.hMenu = hMenu;
	cs.hInstance = GetHInstance();
	cs.lpCreateParams = nullptr;
	SendMessage(m_hWnd, WM_CREATE, 0, reinterpret_cast<LPARAM>(&cs));
	PostMessage(m_hWnd, WinAPI::Extension::WM_USER_AWAKE, 0, 0);
}

LRESULT Control::EventProc(uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept {
	//WinAPI::OutputDebugString(GetWindowMessageName(msg).w_str(), true);
	switch (msg) {
	case WM_CREATE: {
		OnLoad(EventArgs::Empty);
		return 0;
	}
	case WinAPI::Extension::WM_USER_AWAKE: {
		OnAwake(EventArgs::Empty);
		return 0;
	}
	case WM_CLOSE: {
		CloseEventArgs e;
		OnClosing(e);
		if (e.Closing) {
			OnClosed(EventArgs::Empty);
			return 1;
		}
		else return 0;
	}
	case WM_MOVE: {
		return 0;
	}
	case WM_LBUTTONDOWN: {
		OnMouseDown(ToMouseEventArgs(wParam, lParam));
		return 0;
	}
	case WM_LBUTTONUP: {
		OnClick(EventArgs::Empty);
		OnMouseUp(ToMouseEventArgs(wParam, lParam));
		return 0;
	}
	case WM_KEYDOWN: {
		OnKeyDown(ToKeyEventArgs(wParam, lParam));
		if (wParam == VK_ESCAPE) PostMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return 0;
	}
	case WM_KEYUP: {
		OnKeyUp(ToKeyEventArgs(wParam, lParam));
		return 0;
	}
	case WM_MOUSEMOVE: {
		OnMouseMove(ToMouseEventArgs(wParam, lParam));
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		DeviceContext dc = DeviceContext(BeginPaint(m_hWnd, &ps));
		OnPaint(PaintEventArgs(dc, ToRectangle(ps.rcPaint)));
		EndPaint(m_hWnd, &ps);
		return 0;
	}
	case WM_SIZE: {
		//このサイズはクライアントサイズ
		OnResized(ToResizeEventArgs(wParam, lParam));
		return 0;
	}
	case WM_SYSKEYDOWN:
	case WM_STYLECHANGED:
	case WM_SYSCOMMAND:
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED: {
		return 1;
	}
	default: return 1;
	}
}

void Control::Destroy() noexcept {
	if (m_hWnd) WinAPI::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}
