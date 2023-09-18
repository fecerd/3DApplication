module;
#define NOMINMAX
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#include <cstdio>	//snprintf()
#include<Windows.h>
#undef CreateWindow
export module WinAPI;
export import WinAPI_Messages;
export import WinAPI_Constants;
import CSTDINT;
using namespace System;

//using class
export namespace WinAPI {
	using HINSTANCE = HINSTANCE;
	using HWND = HWND;
	using HMENU = HMENU;
	using HICON = HICON;
	using HCURSOR = HCURSOR;
	using HDC = HDC;
	using HGDIOBJ = HGDIOBJ;
	using HBRUSH = HBRUSH;
	using HPEN = HPEN;
	using HRGN = HRGN;
	using HMODULE = HMODULE;
	using HRSRC = HRSRC;
	using HGLOBAL = HGLOBAL;
	using LRESULT = LRESULT;
	using WPARAM = WPARAM;
	using LPARAM = LPARAM;
	using MSG = MSG;
	using WNDPROC = WNDPROC;
	using ATOM = ATOM;
	using WNDCLASSA = WNDCLASSA;
	using WNDCLASSW = WNDCLASSW;
	using WNDCLASSEXA = WNDCLASSEXA;
	using WNDCLASSEXW = WNDCLASSEXW;
	using PAINTSTRUCT = PAINTSTRUCT;
	using TEXTMETRICA = TEXTMETRICA;
	using TEXTMETRICW = TEXTMETRICW;
	using RECT = RECT;
	using POINT = POINT;
	using LOGPEN = LOGPEN;
	using LOGBRUSH = LOGBRUSH;
	using SYSTEM_INFO = SYSTEM_INFO;
	using OSVERSIONINFOEXA = OSVERSIONINFOEXA;
	using OSVERSIONINFOEXW = OSVERSIONINFOEXW;
	using TIMERPROC = TIMERPROC;
	using SYSTEMTIME = SYSTEMTIME;
	using SCROLLINFO = SCROLLINFO;
	using CREATESTRUCTA = CREATESTRUCTA;
	using CREATESTRUCTW = CREATESTRUCTW;
	using MEASUREITEMSTRUCT = MEASUREITEMSTRUCT;
	using DRAWITEMSTRUCT = DRAWITEMSTRUCT;
}

//Class Style
export namespace WinAPI {
#undef CS_VREDRAW
#undef CS_HREDRAW
#undef CS_DBLCLKS
#undef CS_OWNDC
#undef CS_CLASSDC
#undef CS_PARENTDC
#undef CS_NOCLOSE
#undef CS_SAVEBITS
#undef CS_BYTEALIGNCLIENT
#undef CS_BYTEALIGNWINDOW
#undef CS_GLOBALCLASS
#undef CS_IME
#undef CS_DROPSHADOW
	inline constexpr uint32_t CS_VREDRAW = 0x0001;
	inline constexpr uint32_t CS_HREDRAW = 0x0002;
	inline constexpr uint32_t CS_DBLCLKS = 0x0008;
	inline constexpr uint32_t CS_OWNDC = 0x0020;
	inline constexpr uint32_t CS_CLASSDC = 0x0040;
	inline constexpr uint32_t CS_PARENTDC = 0x0080;
	inline constexpr uint32_t CS_NOCLOSE = 0x0200;
	inline constexpr uint32_t CS_SAVEBITS = 0x0800;
	inline constexpr uint32_t CS_BYTEALIGNCLIENT = 0x1000;
	inline constexpr uint32_t CS_BYTEALIGNWINDOW = 0x2000;
	inline constexpr uint32_t CS_GLOBALCLASS = 0x4000;
	inline constexpr uint32_t CS_IME = 0x10000;
	inline constexpr uint32_t CS_DROPSHADOW = 0x20000;
}

//Stock Logical Object
export namespace WinAPI {
#undef WHITE_BRUSH
#undef LTGRAY_BRUSH
#undef GRAY_BRUSH
#undef DKGRAY_BRUSH
#undef BLACK_BRUSH
#undef NULL_BRUSH
#undef HOLLOW_BRUSH
#undef WHITE_PEN
#undef BLACK_PEN
#undef NULL_PEN
#undef OEM_FIXED_FONT
#undef ANSI_FIXED_FONT
#undef ANSI_VAR_FONT
#undef SYSTEM_FONT
#undef DEVICE_DEFAULT_FONT
#undef DEFAULT_PALETTE
#undef SYSTEM_FIXED_FONT
	inline constexpr uint16_t WHITE_BRUSH = 0;
	inline constexpr uint8_t LTGRAY_BRUSH = 1;
	inline constexpr uint8_t GRAY_BRUSH = 2;
	inline constexpr uint8_t DKGRAY_BRUSH = 3;
	inline constexpr uint8_t BLACK_BRUSH = 4;
	inline constexpr uint8_t NULL_BRUSH = 5;
	inline constexpr uint8_t HOLLOW_BRUSH = NULL_BRUSH;
	inline constexpr uint8_t WHITE_PEN = 6;
	inline constexpr uint8_t BLACK_PEN = 7;
	inline constexpr uint8_t NULL_PEN = 8;
	inline constexpr uint8_t OEM_FIXED_FONT = 10;
	inline constexpr uint8_t ANSI_FIXED_FONT = 11;
	inline constexpr uint8_t ANSI_VAR_FONT = 12;
	inline constexpr uint8_t SYSTEM_FONT = 13;
	inline constexpr uint8_t DEVICE_DEFAULT_FONT = 14;
	inline constexpr uint8_t DEFAULT_PALETTE = 15;
	inline constexpr uint8_t SYSTEM_FIXED_FONT = 16;
}

//Icon IDs
export namespace WinAPI {
#undef IDI_APPLICATION
#undef IDI_HAND
#undef IDI_QUESTION
#undef IDI_EXCLAMATION
#undef IDI_ASTERISK
#undef IDI_WINLOGO
#undef IDI_SHIELD
#undef IDI_APPLICATION
#undef IDI_HAND
#undef IDI_QUESTION
#undef IDI_EXCLAMATION
#undef IDI_ASTERISK
#undef IDI_WINLOGO
#undef IDI_SHIELD
#undef IDI_WARNING
#undef IDI_ERROR
#undef IDI_INFORMATION
	inline constexpr uint16_t IDI_APPLICATION = 32512;
	inline constexpr uint16_t IDI_HAND = 32513;
	inline constexpr uint16_t IDI_QUESTION = 32514;
	inline constexpr uint16_t IDI_EXCLAMATION = 32515;
	inline constexpr uint16_t IDI_ASTERISK = 32516;
	inline constexpr uint16_t IDI_WINLOGO = 32517;
	inline constexpr uint16_t IDI_SHIELD = 32518;
	inline constexpr uint16_t IDI_WARNING = IDI_EXCLAMATION;
	inline constexpr uint16_t IDI_ERROR = IDI_HAND;
	inline constexpr uint16_t IDI_INFORMATION = IDI_ASTERISK;
}

//Cursor IDs
export namespace WinAPI {
#undef IDC_ARROW
#undef IDC_IBEAM
#undef IDC_WAIT
#undef IDC_CROSS
#undef IDC_UPARROW
#undef IDC_SIZE
#undef IDC_ICON
#undef IDC_SIZENWSE
#undef IDC_SIZENESW
#undef IDC_SIZEWE
#undef IDC_SIZENS
#undef IDC_SIZEALL
#undef IDC_NO
#undef IDC_HAND
#undef IDC_APPSTARTING
#undef IDC_HELP
#undef IDC_PIN
#undef IDC_PERSON
	inline constexpr uint16_t IDC_ARROW = 32512;
	inline constexpr uint16_t IDC_IBEAM = 32513;
	inline constexpr uint16_t IDC_WAIT = 32514;
	inline constexpr uint16_t IDC_CROSS = 32515;
	inline constexpr uint16_t IDC_UPARROW = 32516;
	inline constexpr uint16_t IDC_SIZE = 32640;
	inline constexpr uint16_t IDC_ICON = 32641;
	inline constexpr uint16_t IDC_SIZENWSE = 32642;
	inline constexpr uint16_t IDC_SIZENESW = 32643;
	inline constexpr uint16_t IDC_SIZEWE = 32644;
	inline constexpr uint16_t IDC_SIZENS = 32645;
	inline constexpr uint16_t IDC_SIZEALL = 32646;
	inline constexpr uint16_t IDC_NO = 32648;
	inline constexpr uint16_t IDC_HAND = 32649;
	inline constexpr uint16_t IDC_APPSTARTING = 32650;
	inline constexpr uint16_t IDC_HELP = 32651;
}

//Window Style
export namespace WinAPI {
#undef WS_OVERLAPPED
#undef WS_POPUP
#undef WS_CHILD
#undef WS_MINIMIZE
#undef WS_VISIBLE
#undef WS_DISABLED
#undef WS_CLIPSIBLINGS
#undef WS_CLIPCHILDREN
#undef WS_MAXIMIZE
#undef WS_CAPTION
#undef WS_BORDER
#undef WS_DLGFRAME
#undef WS_VSCROLL
#undef WS_HSCROLL
#undef WS_SYSMENU
#undef WS_THICKFRAME
#undef WS_GROUP
#undef WS_TABSTOP
#undef WS_MINIMIZEBOX
#undef WS_MAXIMIZEBOX
#undef WS_TILED
#undef WS_ICONIC
#undef WS_SIZEBOX
#undef WS_TILEDWINDOW
#undef WS_OVERLAPPEDWINDOW
#undef WS_POPUPWINDOW
#undef WS_CHILDWINDOW
	inline constexpr uint32_t WS_OVERLAPPED = 0x00000000L;
	inline constexpr uint32_t WS_POPUP = 0x80000000L;
	inline constexpr uint32_t WS_CHILD = 0x40000000L;
	inline constexpr uint32_t WS_MINIMIZE = 0x20000000L;
	inline constexpr uint32_t WS_VISIBLE = 0x10000000L;
	inline constexpr uint32_t WS_DISABLED = 0x08000000L;
	inline constexpr uint32_t WS_CLIPSIBLINGS = 0x04000000L;
	inline constexpr uint32_t WS_CLIPCHILDREN = 0x02000000L;
	inline constexpr uint32_t WS_MAXIMIZE = 0x01000000L;
	inline constexpr uint32_t WS_CAPTION = 0x00C00000L;
	inline constexpr uint32_t WS_BORDER = 0x00800000L;
	inline constexpr uint32_t WS_DLGFRAME = 0x00400000L;
	inline constexpr uint32_t WS_VSCROLL = 0x00200000L;
	inline constexpr uint32_t WS_HSCROLL = 0x00100000L;
	inline constexpr uint32_t WS_SYSMENU = 0x00080000L;
	inline constexpr uint32_t WS_THICKFRAME = 0x00040000L;
	inline constexpr uint32_t WS_GROUP = 0x00020000L;
	inline constexpr uint32_t WS_TABSTOP = 0x00010000L;
	inline constexpr uint32_t WS_MINIMIZEBOX = 0x00020000L;
	inline constexpr uint32_t WS_MAXIMIZEBOX = 0x00010000L;
	inline constexpr uint32_t WS_TILED = WS_OVERLAPPED;
	inline constexpr uint32_t WS_ICONIC = WS_MINIMIZE;
	inline constexpr uint32_t WS_SIZEBOX = WS_THICKFRAME;
	inline constexpr uint32_t WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	inline constexpr uint32_t WS_TILEDWINDOW = WS_OVERLAPPEDWINDOW;
	inline constexpr uint32_t WS_POPUPWINDOW = WS_POPUP | WS_BORDER | WS_SYSMENU;
	inline constexpr uint32_t WS_CHILDWINDOW = WS_CHILD;
}

//Window StyleEx
export namespace WinAPI {
#undef WS_EX_DLGMODALFRAME
#undef WS_EX_NOPARENTNOTIFY
#undef WS_EX_TOPMOST
#undef WS_EX_ACCEPTFILES
#undef WS_EX_TRANSPARENT
#undef WS_EX_MDICHILD
#undef WS_EX_TOOLWINDOW
#undef WS_EX_WINDOWEDGE
#undef WS_EX_CLIENTEDGE
#undef WS_EX_CONTEXTHELP
#undef WS_EX_RIGHT
#undef WS_EX_LEFT
#undef WS_EX_RTLREADING
#undef WS_EX_LTRREADING
#undef WS_EX_LEFTSCROLLBAR
#undef WS_EX_RIGHTSCROLLBAR
#undef WS_EX_CONTROLPARENT
#undef WS_EX_STATICEDGE
#undef WS_EX_APPWINDOW
#undef WS_EX_OVERLAPPEDWINDOW
#undef WS_EX_PALETTEWINDOW
#undef WS_EX_LAYERED
#undef WS_EX_NOINHERITLAYOUT
#undef WS_EX_NOREDIRECTIONBITMAP
#undef WS_EX_LAYOUTRTL
#undef WS_EX_COMPOSITED
#undef WS_EX_NOACTIVATE
	inline constexpr uint32_t WS_EX_DLGMODALFRAME = 0x00000001L;
	inline constexpr uint32_t WS_EX_NOPARENTNOTIFY = 0x00000004L;
	inline constexpr uint32_t WS_EX_TOPMOST = 0x00000008L;
	inline constexpr uint32_t WS_EX_ACCEPTFILES = 0x00000010L;
	inline constexpr uint32_t WS_EX_TRANSPARENT = 0x00000020L;
	inline constexpr uint32_t WS_EX_MDICHILD = 0x00000040L;
	inline constexpr uint32_t WS_EX_TOOLWINDOW = 0x00000080L;
	inline constexpr uint32_t WS_EX_WINDOWEDGE = 0x00000100L;
	inline constexpr uint32_t WS_EX_CLIENTEDGE = 0x00000200L;
	inline constexpr uint32_t WS_EX_CONTEXTHELP = 0x00000400L;
	inline constexpr uint32_t WS_EX_RIGHT = 0x00001000L;
	inline constexpr uint32_t WS_EX_LEFT = 0x00000000L;
	inline constexpr uint32_t WS_EX_RTLREADING = 0x00002000L;
	inline constexpr uint32_t WS_EX_LTRREADING = 0x00000000L;
	inline constexpr uint32_t WS_EX_LEFTSCROLLBAR = 0x00004000L;
	inline constexpr uint32_t WS_EX_RIGHTSCROLLBAR = 0x00000000L;
	inline constexpr uint32_t WS_EX_CONTROLPARENT = 0x00010000L;
	inline constexpr uint32_t WS_EX_STATICEDGE = 0x00020000L;
	inline constexpr uint32_t WS_EX_APPWINDOW = 0x00040000L;
	inline constexpr uint32_t WS_EX_OVERLAPPEDWINDOW = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;
	inline constexpr uint32_t WS_EX_PALETTEWINDOW = WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	inline constexpr uint32_t WS_EX_LAYERED = 0x00080000;
	inline constexpr uint32_t WS_EX_NOINHERITLAYOUT = 0x00100000L;
	inline constexpr uint32_t WS_EX_NOREDIRECTIONBITMAP = 0x00200000L;
	inline constexpr uint32_t WS_EX_LAYOUTRTL = 0x00400000L;
	inline constexpr uint32_t WS_EX_COMPOSITED = 0x02000000L;
	inline constexpr uint32_t WS_EX_NOACTIVATE = 0x08000000L;
}

//CreateWindow() constant
export namespace WinAPI {
#undef CW_USEDEFAULT
	inline constexpr int CW_USEDEFAULT = static_cast<int>(0x80000000);
}

//ShowWindow() Commands
export namespace WinAPI {
#undef SW_HIDE
#undef SW_SHOWNORMAL
#undef SW_NORMAL
#undef SW_SHOWMINIMIZED
#undef SW_SHOWMAXIMIZED
#undef SW_MAXIMIZE
#undef SW_SHOWNOACTIVATE
#undef SW_SHOW
#undef SW_MINIMIZE
#undef SW_SHOWMINNOACTIVE
#undef SW_SHOWNA
#undef SW_RESTORE
#undef SW_SHOWDEFAULT
#undef SW_FORCEMINIMIZE
#undef SW_MAX
	inline constexpr uint8_t SW_HIDE = 0;
	inline constexpr uint8_t SW_SHOWNORMAL = 1;
	inline constexpr uint8_t SW_NORMAL = 1;
	inline constexpr uint8_t SW_SHOWMINIMIZED = 2;
	inline constexpr uint8_t SW_SHOWMAXIMIZED = 3;
	inline constexpr uint8_t SW_MAXIMIZE = 3;
	inline constexpr uint8_t SW_SHOWNOACTIVATE = 4;
	inline constexpr uint8_t SW_SHOW = 5;
	inline constexpr uint8_t SW_MINIMIZE = 6;
	inline constexpr uint8_t SW_SHOWMINNOACTIVE = 7;
	inline constexpr uint8_t SW_SHOWNA = 8;
	inline constexpr uint8_t SW_RESTORE = 9;
	inline constexpr uint8_t SW_SHOWDEFAULT = 10;
	inline constexpr uint8_t SW_FORCEMINIMIZE = 11;
	inline constexpr uint8_t SW_MAX = 11;
}

//MessageBox() Type
export namespace WinAPI {
#undef MB_OK
#undef MB_OKCANCEL
#undef MB_ABORTRETRYIGNORE
#undef MB_YESNOCANCEL
#undef MB_YESNO
#undef MB_RETRYCANCEL
#undef MB_CANCELTRYCONTINUE
#undef MB_ICONHAND
#undef MB_ICONQUESTION
#undef MB_ICONEXCLAMATION
#undef MB_ICONASTERISK
#undef MB_USERICON
#undef MB_ICONWARNING
#undef MB_ICONERROR
#undef MB_ICONINFORMATION
#undef MB_ICONSTOP
#undef MB_DEFBUTTON1
#undef MB_DEFBUTTON2
#undef MB_DEFBUTTON3
#undef MB_DEFBUTTON4
#undef MB_APPLMODAL
#undef MB_SYSTEMMODAL
#undef MB_TASKMODAL
#undef MB_HELP
#undef MB_NOFOCUS
#undef MB_SETFOREGROUND
#undef MB_DEFAULT_DESKTOP_ONLY
#undef MB_TOPMOST
#undef MB_RIGHT
#undef MB_RTLREADING
#undef MB_SERVICE_NOTIFICATION
#undef MB_SERVICE_NOTIFICATION
#undef MB_SERVICE_NOTIFICATION_NT3X
#undef MB_TYPEMASK
#undef MB_ICONMASK
#undef MB_DEFMASK
#undef MB_MODEMASK
#undef MB_MISCMASK
	inline constexpr uint32_t MB_OK = 0x00000000L;
	inline constexpr uint32_t MB_OKCANCEL = 0x00000001L;
	inline constexpr uint32_t MB_ABORTRETRYIGNORE = 0x00000002L;
	inline constexpr uint32_t MB_YESNOCANCEL = 0x00000003L;
	inline constexpr uint32_t MB_YESNO = 0x00000004L;
	inline constexpr uint32_t MB_RETRYCANCEL = 0x00000005L;
	inline constexpr uint32_t MB_CANCELTRYCONTINUE = 0x00000006L;
	inline constexpr uint32_t MB_ICONHAND = 0x00000010L;
	inline constexpr uint32_t MB_ICONQUESTION = 0x00000020L;
	inline constexpr uint32_t MB_ICONEXCLAMATION = 0x00000030L;
	inline constexpr uint32_t MB_ICONASTERISK = 0x00000040L;
	inline constexpr uint32_t MB_USERICON = 0x00000080L;
	inline constexpr uint32_t MB_ICONWARNING = MB_ICONEXCLAMATION;
	inline constexpr uint32_t MB_ICONERROR = MB_ICONHAND;
	inline constexpr uint32_t MB_ICONINFORMATION = MB_ICONASTERISK;
	inline constexpr uint32_t MB_ICONSTOP = MB_ICONHAND;
	inline constexpr uint32_t MB_DEFBUTTON1 = 0x00000000L;
	inline constexpr uint32_t MB_DEFBUTTON2 = 0x00000100L;
	inline constexpr uint32_t MB_DEFBUTTON3 = 0x00000200L;
	inline constexpr uint32_t MB_DEFBUTTON4 = 0x00000300L;
	inline constexpr uint32_t MB_APPLMODAL = 0x00000000L;
	inline constexpr uint32_t MB_SYSTEMMODAL = 0x00001000L;
	inline constexpr uint32_t MB_TASKMODAL = 0x00002000L;
	inline constexpr uint32_t MB_HELP = 0x00004000L;
	inline constexpr uint32_t MB_NOFOCUS = 0x00008000L;
	inline constexpr uint32_t MB_SETFOREGROUND = 0x00010000L;
	inline constexpr uint32_t MB_DEFAULT_DESKTOP_ONLY = 0x00020000L;
	inline constexpr uint32_t MB_TOPMOST = 0x00040000L;
	inline constexpr uint32_t MB_RIGHT = 0x00080000L;
	inline constexpr uint32_t MB_RTLREADING = 0x00100000L;
	inline constexpr uint32_t MB_SERVICE_NOTIFICATION = 0x00200000L;
	inline constexpr uint32_t MB_SERVICE_NOTIFICATION_NT3X = 0x00040000L;
	inline constexpr uint32_t MB_TYPEMASK = 0x0000000FL;
	inline constexpr uint32_t MB_ICONMASK = 0x000000F0L;
	inline constexpr uint32_t MB_DEFMASK = 0x00000F00L;
	inline constexpr uint32_t MB_MODEMASK = 0x00003000L;
	inline constexpr uint32_t MB_MISCMASK = 0x0000C000L;
}

//DrawText() Format
export namespace WinAPI {
#undef DT_TOP
#undef DT_LEFT
#undef DT_CENTER
#undef DT_RIGHT
#undef DT_VCENTER
#undef DT_BOTTOM
#undef DT_WORDBREAK
#undef DT_SINGLELINE
#undef DT_EXPANDTABS
#undef DT_TABSTOP
#undef DT_NOCLIP
#undef DT_EXTERNALLEADING
#undef DT_CALCRECT
#undef DT_NOPREFIX
#undef DT_INTERNAL
#undef DT_EDITCONTROL
#undef DT_PATH_ELLIPSIS
#undef DT_END_ELLIPSIS
#undef DT_MODIFYSTRING
#undef DT_RTLREADING
#undef DT_WORD_ELLIPSIS
#undef DT_NOFULLWIDTHCHARBREAK
#undef DT_HIDEPREFIX
#undef DT_PREFIXONLY
	inline constexpr uint32_t DT_TOP = 0x00000000;
	inline constexpr uint32_t DT_LEFT = 0x00000000;
	inline constexpr uint32_t DT_CENTER = 0x00000001;
	inline constexpr uint32_t DT_RIGHT = 0x00000002;
	inline constexpr uint32_t DT_VCENTER = 0x00000004;
	inline constexpr uint32_t DT_BOTTOM = 0x00000008;
	inline constexpr uint32_t DT_WORDBREAK = 0x00000010;
	inline constexpr uint32_t DT_SINGLELINE = 0x00000020;
	inline constexpr uint32_t DT_EXPANDTABS = 0x00000040;
	inline constexpr uint32_t DT_TABSTOP = 0x00000080;
	inline constexpr uint32_t DT_NOCLIP = 0x00000100;
	inline constexpr uint32_t DT_EXTERNALLEADING = 0x00000200;
	inline constexpr uint32_t DT_CALCRECT = 0x00000400;
	inline constexpr uint32_t DT_NOPREFIX = 0x00000800;
	inline constexpr uint32_t DT_INTERNAL = 0x00001000;
	inline constexpr uint32_t DT_EDITCONTROL = 0x00002000;
	inline constexpr uint32_t DT_PATH_ELLIPSIS = 0x00004000;
	inline constexpr uint32_t DT_END_ELLIPSIS = 0x00008000;
	inline constexpr uint32_t DT_MODIFYSTRING = 0x00010000;
	inline constexpr uint32_t DT_RTLREADING = 0x00020000;
	inline constexpr uint32_t DT_WORD_ELLIPSIS = 0x00040000;
	inline constexpr uint32_t DT_NOFULLWIDTHCHARBREAK = 0x00080000;
	inline constexpr uint32_t DT_HIDEPREFIX = 0x00100000;
	inline constexpr uint32_t DT_PREFIXONLY = 0x00200000;
}

//Device Context Background Modes
export namespace WinAPI {
#undef TRANSPARENT
#undef OPAQUE
#undef BKMODE_LAST
	inline constexpr int32_t TRANSPARENT = 1;
	inline constexpr int32_t OPAQUE = 2;
	inline constexpr int32_t BKMODE_LAST = 2;
}

//CreatePen() Styles
export namespace WinAPI {
#undef PS_SOLID
#undef PS_DASH
#undef PS_DOT
#undef PS_DASHDOT
#undef PS_DASHDOTDOT
#undef PS_NULL
#undef PS_INSIDEFRAME
#undef PS_USERSTYLE
#undef PS_ALTERNATE
#undef PS_STYLE_MASK
#undef PS_ENDCAP_ROUND
#undef PS_ENDCAP_SQUARE
#undef PS_ENDCAP_FLAT
#undef PS_ENDCAP_MASK
#undef PS_JOIN_ROUND
#undef PS_JOIN_BEVEL
#undef PS_JOIN_MITER
#undef PS_JOIN_MASK
#undef PS_COSMETIC
#undef PS_GEOMETRIC
#undef PS_TYPE_MASK
	inline constexpr int32_t PS_SOLID = 0;
	inline constexpr int32_t PS_DASH = 1;
	inline constexpr int32_t PS_DOT = 2;
	inline constexpr int32_t PS_DASHDOT = 3;
	inline constexpr int32_t PS_DASHDOTDOT = 4;
	inline constexpr int32_t PS_NULL = 5;
	inline constexpr int32_t PS_INSIDEFRAME = 6;
	inline constexpr int32_t PS_USERSTYLE = 7;
	inline constexpr int32_t PS_ALTERNATE = 8;
	inline constexpr int32_t PS_STYLE_MASK = 0x0000000F;
	inline constexpr int32_t PS_ENDCAP_ROUND = 0x00000000;
	inline constexpr int32_t PS_ENDCAP_SQUARE = 0x00000100;
	inline constexpr int32_t PS_ENDCAP_FLAT = 0x00000200;
	inline constexpr int32_t PS_ENDCAP_MASK = 0x00000F00;
	inline constexpr int32_t PS_JOIN_ROUND = 0x00000000;
	inline constexpr int32_t PS_JOIN_BEVEL = 0x00001000;
	inline constexpr int32_t PS_JOIN_MITER = 0x00002000;
	inline constexpr int32_t PS_JOIN_MASK = 0x0000F000;
	inline constexpr int32_t PS_COSMETIC = 0x00000000;
	inline constexpr int32_t PS_GEOMETRIC = 0x00010000;
	inline constexpr int32_t PS_TYPE_MASK = 0x000F0000;
}

//LOGBRUSH Styles
export namespace WinAPI {
#undef BS_SOLID
#undef BS_NULL
#undef BS_HOLLOW
#undef BS_HATCHED
#undef BS_PATTERN
#undef BS_INDEXED
#undef BS_DIBPATTERN
#undef BS_DIBPATTERNPT
#undef BS_PATTERN8X8
#undef BS_DIBPATTERN8X8
#undef BS_MONOPATTERN
	inline constexpr int32_t BS_SOLID = 0;
	inline constexpr int32_t BS_NULL = 1;
	inline constexpr int32_t BS_HOLLOW = BS_NULL;
	inline constexpr int32_t BS_HATCHED = 2;
	inline constexpr int32_t BS_PATTERN = 3;
	inline constexpr int32_t BS_INDEXED = 4;
	inline constexpr int32_t BS_DIBPATTERN = 5;
	inline constexpr int32_t BS_DIBPATTERNPT = 6;
	inline constexpr int32_t BS_PATTERN8X8 = 7;
	inline constexpr int32_t BS_DIBPATTERN8X8 = 8;
	inline constexpr int32_t BS_MONOPATTERN = 9;
}

//CreateHatchBrush() Styles
export namespace WinAPI {
#undef HS_HORIZONTAL
#undef HS_VERTICAL
#undef HS_FDIAGONAL
#undef HS_BDIAGONAL
#undef HS_CROSS
#undef HS_DIAGCROSS
#undef HS_API_MAX
	inline constexpr int32_t HS_HORIZONTAL = 0;	/* ----- */
	inline constexpr int32_t HS_VERTICAL = 1;	/* ||||| */
	inline constexpr int32_t HS_FDIAGONAL = 2;	/* \\\\\ */
	inline constexpr int32_t HS_BDIAGONAL = 3;	/* ///// */
	inline constexpr int32_t HS_CROSS = 4;	/* +++++ */
	inline constexpr int32_t HS_DIAGCROSS = 5;	/* xxxxx */
	inline constexpr int32_t HS_API_MAX = 12;
}

//Binary Raster Operation Modes
export namespace WinAPI {
#undef R2_BLACK
#undef R2_NOTMERGEPEN
#undef R2_MASKNOTPEN
#undef R2_NOTCOPYPEN
#undef R2_MASKPENNOT
#undef R2_NOT
#undef R2_XORPEN
#undef R2_NOTMASKPEN
#undef R2_MASKPEN
#undef R2_NOTXORPEN
#undef R2_NOP
#undef R2_MERGENOTPEN
#undef R2_COPYPEN
#undef R2_MERGEPENNOT
#undef R2_MERGEPEN
#undef R2_WHITE
#undef R2_LAST
	inline constexpr int32_t R2_BLACK = 1;	/*  0       */
	inline constexpr int32_t R2_NOTMERGEPEN = 2;	/* DPon     */
	inline constexpr int32_t R2_MASKNOTPEN = 3;	/* DPna     */
	inline constexpr int32_t R2_NOTCOPYPEN = 4;	/* PN       */
	inline constexpr int32_t R2_MASKPENNOT = 5;	/* PDna     */
	inline constexpr int32_t R2_NOT = 6;	/* Dn       */
	inline constexpr int32_t R2_XORPEN = 7;	/* DPx      */
	inline constexpr int32_t R2_NOTMASKPEN = 8;	/* DPan     */
	inline constexpr int32_t R2_MASKPEN = 9;	/* DPa      */
	inline constexpr int32_t R2_NOTXORPEN = 10;	/* DPxn     */
	inline constexpr int32_t R2_NOP = 11;	/* D        */
	inline constexpr int32_t R2_MERGENOTPEN = 12;	/* DPno     */
	inline constexpr int32_t R2_COPYPEN = 13;	/* P        */
	inline constexpr int32_t R2_MERGEPENNOT = 14;	/* PDno     */
	inline constexpr int32_t R2_MERGEPEN = 15;	/* DPo      */
	inline constexpr int32_t R2_WHITE = 16;	/*  1       */
	inline constexpr int32_t R2_LAST = 16;
}

//PolyFill() Modes
export namespace WinAPI {
#undef ALTERNATE
#undef WINDING
#undef POLYFILL_LAST
	inline constexpr int32_t ALTERNATE = 1;
	inline constexpr int32_t WINDING = 2;
	inline constexpr int32_t POLYFILL_LAST = 2;
}

//Function
export namespace WinAPI {
#undef LoadIcon
	HICON LoadIcon(HINSTANCE hInstance, const char* iconName) noexcept { return LoadIconA(hInstance, iconName); }
	HICON LoadIcon(HINSTANCE hInstance, const wchar_t* iconName) noexcept { return LoadIconW(hInstance, iconName); }
	HICON LoadIcon(HINSTANCE hInstance, uint16_t iconID) noexcept { return LoadIconW(hInstance, reinterpret_cast<wchar_t*>(iconID)); }
#undef LoadCursor
	HCURSOR LoadCursor(HINSTANCE hInstance, const char* cursorName) noexcept { return LoadCursorA(hInstance, cursorName); }
	HCURSOR LoadCursor(HINSTANCE hInstance, const wchar_t* cursorName) noexcept { return LoadCursorW(hInstance, cursorName); }
	HCURSOR LoadCursor(HINSTANCE hInstance, uint16_t cursorID) noexcept { return LoadCursorW(hInstance, reinterpret_cast<wchar_t*>(cursorID)); }
	int16_t GetKeyState(int32_t virtualKey) noexcept { return ::GetKeyState(virtualKey); }
	int32_t GetSystemMetrics(int32_t index) noexcept { return ::GetSystemMetrics(index); }
	void GetSystemInfo(SYSTEM_INFO* info) noexcept { return ::GetSystemInfo(info); }
#undef GetVersionEx
	bool VerifyVersionInfo(OSVERSIONINFOEXA* versionInfo, uint32_t typeMask, uint64_t conditionMask) noexcept { return ::VerifyVersionInfoA(versionInfo, typeMask, conditionMask); }
	bool VerifyVersionInfo(OSVERSIONINFOEXW* versionInfo, uint32_t typeMask, uint64_t conditionMask) noexcept { return ::VerifyVersionInfoW(versionInfo, typeMask, conditionMask); }
#undef LOWORD
	uint16_t LOWORD(LPARAM val) noexcept { return static_cast<uint16_t>(val & 0xffff); }
#undef HIWORD
	uint16_t HIWORD(LPARAM val) noexcept { return static_cast<uint16_t>(val >> 16); }
	uint64_t SetTimer(HWND hWnd, uint32_t timerID, uint32_t msElapse, TIMERPROC timerProc) noexcept { return ::SetTimer(hWnd, timerID, msElapse, timerProc); }
	uint64_t SetTimer(HWND hWnd, uint32_t timerID, uint32_t msElapse, void(*timerProc)(HWND, uint32_t, uint64_t, uint32_t)) noexcept { return ::SetTimer(hWnd, timerID, msElapse, reinterpret_cast<TIMERPROC>(timerProc)); }
	uint64_t SetTimer(HWND hWnd, uint32_t timerID, uint32_t msElapse, nullptr_t timerProc) noexcept { return ::SetTimer(hWnd, timerID, msElapse, nullptr); }
	bool KillTimer(HWND hWnd, uint64_t timerID) noexcept { return ::KillTimer(hWnd, timerID); }
	void GetLocalTime(SYSTEMTIME* systemTime) noexcept { return ::GetLocalTime(systemTime); }
	void GetSystemTime(SYSTEMTIME* systemTime) noexcept { return ::GetSystemTime(systemTime); }
	uint32_t GetSysColor(int32_t index) noexcept { return ::GetSysColor(index); }
	bool SetSysColors(int32_t counts, const int32_t* elements, const uint32_t* colors) noexcept { return ::SetSysColors(counts, reinterpret_cast<const INT*>(elements), reinterpret_cast<const COLORREF*>(colors)); }
#undef LoadString
	int32_t LoadString(HINSTANCE hInstance, uint32_t id, char* str, int32_t bufferCount) noexcept { return ::LoadStringA(hInstance, id, str, bufferCount); }
	int32_t LoadString(HINSTANCE hInstance, uint32_t id, wchar_t* str, int32_t bufferCount) noexcept { return ::LoadStringW(hInstance, id, str, bufferCount); }
	HGLOBAL LoadResource(HMODULE hModule, HRSRC hResInfo) noexcept { return ::LoadResource(hModule, hResInfo); }
#undef FindResource
#undef FindResourceEx
	HRSRC FindResource(HMODULE hModule, const char* name, const char* type, uint16_t language) noexcept { return ::FindResourceExA(hModule, type, name, language); }
	HRSRC FindResource(HMODULE hModule, const wchar_t* name, const wchar_t* type, uint16_t language) noexcept { return ::FindResourceExW(hModule, type, name, language); }
	void* LockResource(HGLOBAL hResourceData) noexcept { return ::LockResource(hResourceData); }
#undef OutputDebugString
	void OutputDebugString(const char* str, bool cr = false) noexcept { ::OutputDebugStringA(str); if (cr) ::OutputDebugStringA("\n"); }
	void OutputDebugString(const wchar_t* str, bool cr = false) noexcept { ::OutputDebugStringW(str); if (cr) ::OutputDebugStringW(L"\n"); }
	void OutputDebugString(const char16_t* str, bool cr = false) noexcept { ::OutputDebugStringW(reinterpret_cast<const wchar_t*>(str)); if (cr) ::OutputDebugStringW(L"\n"); }
	void OutputDebugStringI(int64_t i, bool cr = false) noexcept {
		char buf[256]{};
		snprintf(buf, 256, "%jd", i);
		OutputDebugString(buf);
		if (cr) ::OutputDebugStringA("\n");
	}
	void OutputDebugStringI(uint64_t i, bool cr = false) noexcept {
		char buf[256]{};
		snprintf(buf, 256, "%zu", i);
		OutputDebugString(buf);
		if (cr) ::OutputDebugStringA("\n");
	}
#undef GetModuleHandle
	HMODULE GetModuleHandle(nullptr_t) noexcept {
		return ::GetModuleHandleW(nullptr);
	}
	HMODULE GetModuleHandle(const char* moduleName) noexcept {
		return ::GetModuleHandleA(moduleName);
	}
	HMODULE GetModuleHandle(const wchar_t* moduleName) noexcept {
		return ::GetModuleHandleW(moduleName);
	}
}

//Function(Mouse)
export namespace WinAPI {
	bool GetCursorPos(POINT* point) noexcept { return ::GetCursorPos(point); }
	bool SetCursorPos(int32_t x, int32_t y) noexcept { return ::SetCursorPos(x, y); }
	int32_t ShowCursor(bool show) noexcept { return ::ShowCursor(show); }
	HWND SetCapture(HWND hWnd) noexcept { return ::SetCapture(hWnd); }
	bool ReleaseCapture() noexcept { return ::ReleaseCapture(); }
	HCURSOR SetCursor(HCURSOR hCursor) noexcept { return ::SetCursor(hCursor); }
}

//Function(Window Message)
export namespace WinAPI {
#undef DefWindowProc
	LRESULT DefWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept { return ::DefWindowProcW(hWnd, msg, wParam, lParam); }
#undef GetMessage
	bool GetMessage(MSG* msg, HWND hWnd, uint32_t msgFilterMin, uint32_t msgFilterMax) noexcept {
		return ::GetMessageW(msg, hWnd, msgFilterMin, msgFilterMax);
	}
#undef DispatchMessage
	bool DispatchMessage(const MSG* msg) noexcept { return ::DispatchMessageW(msg); }
	bool TranslateMessage(const MSG* msg) noexcept { return ::TranslateMessage(msg); }
	void PostQuitMessage(int exitCode) noexcept { ::PostQuitMessage(exitCode); }
#undef SendMessage
	LRESULT SendMessage(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept { return ::SendMessageW(hWnd, msg, wParam, lParam); }
#undef PostMessage
	bool PostMessage(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept { return ::PostMessageW(hWnd, msg, wParam, lParam); }
#undef PeekMessage
	bool PeekMessage(MSG* msg, HWND hWnd, uint32_t msgFilterMin, uint32_t msgFilterMax, uint32_t removeMsg) noexcept { return ::PeekMessageW(msg, hWnd, msgFilterMin, msgFilterMax, removeMsg); }
}

//Function(Window)
export namespace WinAPI {
#undef RegisterClass	
	ATOM RegisterClass(const WNDCLASSA* wndClassA) noexcept { return ::RegisterClassA(wndClassA); }
	ATOM RegisterClass(const WNDCLASSW* wndClassW) noexcept { return ::RegisterClassW(wndClassW); }
	ATOM RegisterClass(const WNDCLASSEXA* wndClassExA) noexcept { return ::RegisterClassExA(wndClassExA); }
	ATOM RegisterClass(const WNDCLASSEXW* wndClassExW) noexcept { return ::RegisterClassExW(wndClassExW); }
#undef CreateWindow
	HWND CreateWindow(
		uint32_t exStyle, const char* className, const char* windowName, uint32_t windowStyle,
		int32_t x, int32_t y, int32_t width, int32_t height,
		HWND parent, HMENU hMenu, HINSTANCE hInstance, void* param
	) noexcept {
		return ::CreateWindowExA(exStyle, className, windowName, windowStyle, x, y, width, height, parent, hMenu, hInstance, param);
	}
	HWND CreateWindow(
		uint32_t exStyle, const wchar_t* className, const wchar_t* windowName, uint32_t windowStyle,
		int32_t x, int32_t y, int32_t width, int32_t height,
		HWND parent, HMENU hMenu, HINSTANCE hInstance, void* param
	) noexcept {
		return ::CreateWindowExW(exStyle, className, windowName, windowStyle, x, y, width, height, parent, hMenu, hInstance, param);
	}

	bool ShowWindow(HWND hWnd, int32_t command) noexcept { return ::ShowWindow(hWnd, command); }
#undef MessageBox
	int32_t MessageBox(HWND hWnd, const char* text, const char* caption, uint32_t type) noexcept { return ::MessageBoxA(hWnd, text, caption, type); }
	int32_t MessageBox(HWND hWnd, const wchar_t* text, const wchar_t* caption, uint32_t type) noexcept { return ::MessageBoxW(hWnd, text, caption, type); }
	bool DestroyWindow(HWND hWnd) noexcept { return ::DestroyWindow(hWnd); }
#undef GetWindowLong
#undef GetWindowLongPtr
	int64_t GetWindowLong(HWND hWnd, int32_t index) noexcept { return ::GetWindowLongPtrW(hWnd, index); }
#undef SetWindowLong
#undef SetWindowLongPtr
	int64_t SetWindowLong(HWND hWnd, int32_t index, int64_t data) noexcept { return ::SetWindowLongPtrW(hWnd, index, data); }
#undef SetWindowText
	bool SetWindowText(HWND hWnd, const char* str) noexcept { return ::SetWindowTextA(hWnd, str); }
	bool SetWindowText(HWND hWnd, const wchar_t* str) noexcept { return ::SetWindowTextW(hWnd, str); }
#undef GetWindowText
	int32_t GetWindowText(HWND hWnd, char* str, int32_t count) noexcept { return ::GetWindowTextA(hWnd, str, count); }
	int32_t GetWindowText(HWND hWnd, wchar_t* str, int32_t count) noexcept { return ::GetWindowTextW(hWnd, str, count); }
#undef GetWindowTextLength
	int32_t GetWindowTextLength(HWND hWnd) noexcept { return ::GetWindowTextLengthW(hWnd); }
	bool IsWindowVisible(HWND hWnd) noexcept { return ::IsWindowVisible(hWnd); }
	bool EnableWindow(HWND hWnd, bool enable) noexcept { return ::EnableWindow(hWnd, enable); }
	bool IsWindowEnabled(HWND hWnd) noexcept { return ::IsWindowEnabled(hWnd); }
#undef CallWindowProc
	LRESULT CallWindowProc(WNDPROC wndProc, HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam) noexcept { return ::CallWindowProcW(wndProc, hWnd, msg, wParam, lParam); }
#undef SetClassLong
#undef SetClassLongPtr
	int64_t SetClassLong(HWND hWnd, int32_t index, int64_t data) noexcept { return ::SetClassLongPtrW(hWnd, index, data); }
#undef GetClassLong
#undef GetClassLongPtr
	int64_t GetClassLong(HWND hWnd, int32_t index) noexcept { return ::GetClassLongPtrW(hWnd, index); }
	bool MoveWindow(HWND hWnd, int32_t x, int32_t y, int32_t width, int32_t height, bool repaint) noexcept {
		return ::MoveWindow(hWnd, x, y, width, height, repaint);
	}
	bool GetWindowRect(HWND hWnd, RECT* rc) noexcept { return ::GetWindowRect(hWnd, rc); }
	bool SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int32_t X, int32_t Y, int32_t cx, int32_t cy, uint32_t uFlags) noexcept {
		return ::SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
	}
	HWND GetFocus() noexcept { return ::GetFocus(); }
	HWND GetTopWindow(HWND hWnd = nullptr) noexcept { return ::GetTopWindow(hWnd); }
}

//Function(ScrollBar)
export namespace WinAPI {
	int32_t SetScrollInfo(HWND hWnd, int32_t scrollBarType, SCROLLINFO* info, bool redraw) noexcept { return ::SetScrollInfo(hWnd, scrollBarType, info, redraw); }
	bool GetScrollInfo(HWND hWnd, int32_t scrollBarType, SCROLLINFO* info) noexcept { return ::GetScrollInfo(hWnd, scrollBarType, info); }
}

//Function(Device Context)
export namespace WinAPI {
	HDC GetDC(HWND hWnd) noexcept { return ::GetDC(hWnd); }
	bool ReleaseDC(HWND hWnd, HDC hdc) noexcept { return ::ReleaseDC(hWnd, hdc); }
	HDC BeginPaint(HWND hWnd, PAINTSTRUCT* paintStruct) noexcept { return ::BeginPaint(hWnd, paintStruct); }
	bool EndPaint(HWND hWnd, const PAINTSTRUCT* paintStruct) noexcept { return ::EndPaint(hWnd, paintStruct); }
	uint32_t SetTextColor(HDC hdc, uint32_t color) noexcept { return ::SetTextColor(hdc, color); }
	int32_t SaveDC(HDC hdc) noexcept { return ::SaveDC(hdc); }
	bool RestoreDC(HDC hdc, int32_t savedDC) noexcept { return ::RestoreDC(hdc, savedDC); }
	uint32_t SetBkColor(HDC hdc, uint32_t color) noexcept { return ::SetBkColor(hdc, color); }
	int32_t SetBkMode(HDC hdc, int32_t mode) noexcept { return ::SetBkMode(hdc, mode); }
	uint32_t GetBkColor(HDC hdc) noexcept { return ::GetBkColor(hdc); }
	int32_t GetBkMode(HDC hdc) noexcept { return ::GetBkMode(hdc); }
	bool MoveToEx(HDC hdc, int32_t x, int32_t y, POINT* point) noexcept { return ::MoveToEx(hdc, x, y, point); }
	int32_t SetROP2(HDC hdc, int32_t drawMode) noexcept { return ::SetROP2(hdc, drawMode); }
	int32_t GetROP2(HDC hdc) noexcept { return ::GetROP2(hdc); }
	int32_t SetPolyFillMode(HDC hdc, int32_t fillMode) noexcept { return ::SetPolyFillMode(hdc, fillMode); }
	int32_t GetPolyFillMode(HDC hdc) noexcept { return ::GetPolyFillMode(hdc); }
	int32_t GetDeviceCaps(HDC hdc, int32_t index) noexcept { return ::GetDeviceCaps(hdc, index); }
}

//Function(GDI Object)
export namespace WinAPI {
	HGDIOBJ GetStockObject(int i) noexcept { return ::GetStockObject(i); }
	bool DeleteObject(HGDIOBJ obj) noexcept { return ::DeleteObject(obj); }
	HGDIOBJ SelectObject(HDC hdc, HGDIOBJ obj) noexcept { return ::SelectObject(hdc, obj); }
	HBRUSH CreateSolidBrush(uint32_t color) noexcept { return ::CreateSolidBrush(color); }
	HBRUSH CreateHatchBrush(int32_t style, uint32_t color) noexcept { return ::CreateHatchBrush(style, color); }
	HBRUSH CreateBrushIndirect(LOGBRUSH* logicalBrush) noexcept { return ::CreateBrushIndirect(logicalBrush); }
	HPEN CreatePen(int32_t style, int32_t width, uint32_t color) noexcept { return ::CreatePen(style, width, color); }
	HPEN CreatePenIndirect(const LOGPEN* logicalPen) noexcept { return ::CreatePenIndirect(logicalPen); }
	HRGN CreateRectRgn(int32_t left, int32_t top, int32_t right, int32_t bottom) noexcept { return ::CreateRectRgn(left, top, right, bottom); }
	HRGN CreateRectRgnIndirect(const RECT* rect) noexcept { return ::CreateRectRgnIndirect(rect); }
	HRGN CreateEllipticRgn(int32_t left, int32_t top, int32_t right, int32_t bottom) noexcept { return ::CreateEllipticRgn(left, top, right, bottom); }
	HRGN CreateEllipticRgnIndirect(const RECT* rect) noexcept { return ::CreateEllipticRgnIndirect(rect); }
	HRGN CreateRoundRectRgn(int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t widthEllipse, int32_t heightEllipse) noexcept { return ::CreateRoundRectRgn(left, top, right, bottom, widthEllipse, heightEllipse); }
	HRGN CreatePolygonRgn(const POINT* points, int32_t count, int32_t fillMode) noexcept { return ::CreatePolygonRgn(points, count, fillMode); }
	HRGN CreatePolyPolygonRgn(const POINT* points, const int32_t* counts, int32_t polyCount, int32_t fillMode) noexcept { return ::CreatePolyPolygonRgn(points, reinterpret_cast<const int*>(counts), polyCount, fillMode); }
	int32_t CombineRgn(HRGN dst, HRGN src1, HRGN src2, int32_t combineMode) noexcept { return ::CombineRgn(dst, src1, src2, combineMode); }
}

//Function(Drawing)
export namespace WinAPI {
#undef TextOut
	bool TextOut(HDC hdc, int32_t x, int32_t y, const char* str, int32_t length) noexcept {
		return ::TextOutA(hdc, x, y, str, length);
	}
	bool TextOut(HDC hdc, int32_t x, int32_t y, const wchar_t* str, int32_t length) noexcept {
		return ::TextOutW(hdc, x, y, str, length);
	}
#undef GetTextMetrics
	bool GetTextMetrics(HDC hdc, TEXTMETRICA* textMetrics) noexcept { return ::GetTextMetricsA(hdc, textMetrics); }
	bool GetTextMetrics(HDC hdc, TEXTMETRICW* textMetrics) noexcept { return ::GetTextMetricsW(hdc, textMetrics); }
#undef DrawText
	int32_t DrawText(HDC hdc, const char* text, int32_t length, RECT* rect, uint32_t format) noexcept {
		return ::DrawTextA(hdc, text, length, rect, format);
	}
	int32_t DrawText(HDC hdc, const wchar_t* text, int32_t length, RECT* rect, uint32_t format) noexcept {
		return ::DrawTextW(hdc, text, length, rect, format);
	}
	uint32_t SetPixel(HDC hdc, int32_t x, int32_t y, uint32_t color) noexcept { return ::SetPixel(hdc, x, y, color); }
	uint32_t GetPixel(HDC hdc, int32_t x, int32_t y) noexcept { return ::GetPixel(hdc, x, y); }
	bool LineTo(HDC hdc, int32_t xEnd, int32_t yEnd) noexcept { return ::LineTo(hdc, xEnd, yEnd); }
	bool PolylineTo(HDC hdc, const POINT* points, uint32_t count) noexcept { return ::PolylineTo(hdc, points, count); }
	bool Polyline(HDC hdc, const POINT* points, int32_t count) noexcept { return ::Polyline(hdc, points, count); }
	bool PolyPolyline(HDC hdc, const POINT* points, const uint32_t* counts, uint32_t countSum) noexcept { return ::PolyPolyline(hdc, points, reinterpret_cast<const unsigned long*>(counts), countSum); }
	bool Rectangle(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom) noexcept { return ::Rectangle(hdc, left, top, right, bottom); }
	bool Ellipse(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom) noexcept { return ::Ellipse(hdc, left, top, right, bottom); }
	bool RoundRect(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t roundWidth, int32_t roundHeight) noexcept {
		return ::RoundRect(hdc, left, top, right, bottom, roundWidth, roundHeight);
	}
	bool Arc(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd) noexcept {
		return ::Arc(hdc, left, top, right, bottom, xStart, yStart, xEnd, yEnd);
	}
	bool Pie(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd) noexcept {
		return ::Pie(hdc, left, top, right, bottom, xStart, yStart, xEnd, yEnd);
	}
	bool Chord(HDC hdc, int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd) noexcept {
		return ::Chord(hdc, left, top, right, bottom, xStart, yStart, xEnd, yEnd);
	}
	bool PolyBezier(HDC hdc, const POINT* points, uint32_t count) noexcept { return ::PolyBezier(hdc, points, count); }
	bool PolyBezierTo(HDC hdc, const POINT* points, uint32_t count) noexcept { return ::PolyBezierTo(hdc, points, count); }
	bool Polygon(HDC hdc, const POINT* points, int32_t count) noexcept { return ::Polygon(hdc, points, count); }
	bool FillRgn(HDC hdc, HRGN hRegion, HBRUSH hBrush) noexcept { return ::FillRgn(hdc, hRegion, hBrush); }
	bool FrameRgn(HDC hdc, HRGN hRegion, HBRUSH hBrush, int32_t lineWidth, int32_t lineHeight) noexcept { return ::FrameRgn(hdc, hRegion, hBrush, lineWidth, lineHeight); }
	bool PaintRgn(HDC hdc, HRGN hRegion) noexcept { return ::PaintRgn(hdc, hRegion); }
}

//Function(Client Area)
export namespace WinAPI {
	bool GetClientRect(HWND hWnd, RECT* rect) noexcept { return ::GetClientRect(hWnd, rect); }
	bool InvalidateRect(HWND hWnd, const RECT* rect, bool erase) noexcept { return ::InvalidateRect(hWnd, rect, erase); }
	bool InvalidateRgn(HWND hWnd, HRGN hRegion, bool erase) noexcept { return ::InvalidateRgn(hWnd, hRegion, erase); }
	bool ValidateRect(HWND hWnd, const RECT* rect) noexcept { return ::ValidateRect(hWnd, rect); }
	bool ValidateRgn(HWND hWnd, HRGN hRegion) noexcept { return ::ValidateRgn(hWnd, hRegion); }
	bool ClientToScreen(HWND hWnd, POINT* lpPoint) noexcept { return ::ClientToScreen(hWnd, lpPoint); }
	bool ScreenToClient(HWND hWnd, POINT* lpPoint) noexcept { return ::ScreenToClient(hWnd, lpPoint); }
}