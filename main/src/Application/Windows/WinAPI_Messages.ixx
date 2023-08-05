module;
#define NOMINMAX
#include<Windows.h>
export module WinAPI_Messages;
import CSTDINT;
using namespace System;

namespace {
#undef WM_NULL
#undef WM_CREATE
#undef WM_DESTROY
#undef WM_MOVE
#undef WM_SIZE
#undef WM_ACTIVATE
#undef WA_INACTIVE
#undef WA_ACTIVE
#undef WA_CLICKACTIVE
#undef WM_SETFOCUS
#undef WM_KILLFOCUS
#undef WM_ENABLE
#undef WM_SETREDRAW
#undef WM_SETTEXT
#undef WM_GETTEXT
#undef WM_GETTEXTLENGTH
#undef WM_PAINT
#undef WM_CLOSE
#undef WM_QUERYENDSESSION
#undef WM_QUERYOPEN
#undef WM_ENDSESSION
#undef WM_QUIT
#undef WM_ERASEBKGND
#undef WM_SYSCOLORCHANGE
#undef WM_SHOWWINDOW
#undef WM_WININICHANGE
#undef WM_SETTINGCHANGE
#undef WM_DEVMODECHANGE
#undef WM_ACTIVATEAPP
#undef WM_FONTCHANGE
#undef WM_TIMECHANGE
#undef WM_CANCELMODE
#undef WM_SETCURSOR
#undef WM_MOUSEACTIVATE
#undef WM_CHILDACTIVATE
#undef WM_QUEUESYNC
#undef WM_GETMINMAXINFO
#undef WM_PAINTICON
#undef WM_ICONERASEBKGND
#undef WM_NEXTDLGCTL
#undef WM_SPOOLERSTATUS
#undef WM_DRAWITEM
#undef WM_MEASUREITEM
#undef WM_DELETEITEM
#undef WM_VKEYTOITEM
#undef WM_CHARTOITEM
#undef WM_SETFONT
#undef WM_GETFONT
#undef WM_SETHOTKEY
#undef WM_GETHOTKEY
#undef WM_QUERYDRAGICON
#undef WM_COMPAREITEM
#undef WM_GETOBJECT
#undef WM_COMPACTING
#undef WM_COMMNOTIFY
#undef WM_WINDOWPOSCHANGING
#undef WM_WINDOWPOSCHANGED
#undef WM_POWER
#undef PWR_OK
#undef PWR_FAIL
#undef PWR_SUSPENDREQUEST
#undef PWR_SUSPENDRESUME
#undef PWR_CRITICALRESUME
#undef WM_COPYDATA
#undef WM_CANCELJOURNAL
#undef WM_NOTIFY
#undef WM_INPUTLANGCHANGEREQUEST
#undef WM_INPUTLANGCHANGE
#undef WM_TCARD
#undef WM_HELP
#undef WM_USERCHANGED
#undef WM_NOTIFYFORMAT
#undef NFR_ANSI
#undef NFR_UNICODE
#undef NF_QUERY
#undef NF_REQUERY
#undef WM_CONTEXTMENU
#undef WM_STYLECHANGING
#undef WM_STYLECHANGED
#undef WM_DISPLAYCHANGE
#undef WM_GETICON
#undef WM_SETICON
#undef WM_NCCREATE
#undef WM_NCDESTROY
#undef WM_NCCALCSIZE
#undef WM_NCHITTEST
#undef WM_NCPAINT
#undef WM_NCACTIVATE
#undef WM_GETDLGCODE
#undef WM_SYNCPAINT
#undef WM_NCMOUSEMOVE
#undef WM_NCLBUTTONDOWN
#undef WM_NCLBUTTONUP
#undef WM_NCLBUTTONDBLCLK
#undef WM_NCRBUTTONDOWN
#undef WM_NCRBUTTONUP
#undef WM_NCRBUTTONDBLCLK
#undef WM_NCMBUTTONDOWN
#undef WM_NCMBUTTONUP
#undef WM_NCMBUTTONDBLCLK
#undef WM_NCXBUTTONDOWN
#undef WM_NCXBUTTONUP
#undef WM_NCXBUTTONDBLCLK
#undef WM_INPUT_DEVICE_CHANGE
#undef WM_INPUT
#undef WM_KEYFIRST
#undef WM_KEYDOWN
#undef WM_KEYUP
#undef WM_CHAR
#undef WM_DEADCHAR
#undef WM_SYSKEYDOWN
#undef WM_SYSKEYUP
#undef WM_SYSCHAR
#undef WM_SYSDEADCHAR
#undef WM_UNICHAR
#undef WM_KEYLAST
#undef UNICODE_NOCHAR
#undef WM_KEYLAST
#undef WM_IME_STARTCOMPOSITION
#undef WM_IME_ENDCOMPOSITION
#undef WM_IME_COMPOSITION
#undef WM_IME_KEYLAST
#undef WM_INITDIALOG
#undef WM_COMMAND
#undef WM_SYSCOMMAND
#undef WM_TIMER
#undef WM_HSCROLL
#undef WM_VSCROLL
#undef WM_INITMENU
#undef WM_INITMENUPOPUP
#undef WM_GESTURE
#undef WM_GESTURENOTIFY
#undef WM_MENUSELECT
#undef WM_MENUCHAR
#undef WM_ENTERIDLE
#undef WM_MENURBUTTONUP
#undef WM_MENUDRAG
#undef WM_MENUGETOBJECT
#undef WM_UNINITMENUPOPUP
#undef WM_MENUCOMMAND
#undef WM_CHANGEUISTATE
#undef WM_UPDATEUISTATE
#undef WM_QUERYUISTATE
#undef UIS_SET
#undef UIS_CLEAR
#undef UIS_INITIALIZE
#undef UISF_HIDEFOCUS
#undef UISF_HIDEACCEL
#undef UISF_ACTIVE
#undef WM_CTLCOLORMSGBOX
#undef WM_CTLCOLOREDIT
#undef WM_CTLCOLORLISTBOX
#undef WM_CTLCOLORBTN
#undef WM_CTLCOLORDLG
#undef WM_CTLCOLORSCROLLBAR
#undef WM_CTLCOLORSTATIC
#undef MN_GETHMENU
#undef WM_MOUSEFIRST
#undef WM_MOUSEMOVE
#undef WM_LBUTTONDOWN
#undef WM_LBUTTONUP
#undef WM_LBUTTONDBLCLK
#undef WM_RBUTTONDOWN
#undef WM_RBUTTONUP
#undef WM_RBUTTONDBLCLK
#undef WM_MBUTTONDOWN
#undef WM_MBUTTONUP
#undef WM_MBUTTONDBLCLK
#undef WM_MOUSEWHEEL
#undef WM_XBUTTONDOWN
#undef WM_XBUTTONUP
#undef WM_XBUTTONDBLCLK
#undef WM_MOUSEHWHEEL
#undef WM_MOUSELAST
#undef WM_MOUSELAST
#undef WM_MOUSELAST
#undef WM_MOUSELAST
#undef WHEEL_DELTA
#undef WHEEL_PAGESCROLL
#undef XBUTTON1
#undef XBUTTON2
#undef WM_PARENTNOTIFY
#undef WM_ENTERMENULOOP
#undef WM_EXITMENULOOP
#undef WM_NEXTMENU
#undef WM_SIZING
#undef WM_CAPTURECHANGED
#undef WM_MOVING
#undef WM_POWERBROADCAST
#undef WM_DEVICECHANGE
#undef WM_MDICREATE
#undef WM_MDIDESTROY
#undef WM_MDIACTIVATE
#undef WM_MDIRESTORE
#undef WM_MDINEXT
#undef WM_MDIMAXIMIZE
#undef WM_MDITILE
#undef WM_MDICASCADE
#undef WM_MDIICONARRANGE
#undef WM_MDIGETACTIVE
#undef WM_MDISETMENU
#undef WM_ENTERSIZEMOVE
#undef WM_EXITSIZEMOVE
#undef WM_DROPFILES
#undef WM_MDIREFRESHMENU
#undef WM_POINTERDEVICECHANGE
#undef WM_POINTERDEVICEINRANGE
#undef WM_POINTERDEVICEOUTOFRANGE
#undef WM_TOUCH
#undef WM_NCPOINTERUPDATE
#undef WM_NCPOINTERDOWN
#undef WM_NCPOINTERUP
#undef WM_POINTERUPDATE
#undef WM_POINTERDOWN
#undef WM_POINTERUP
#undef WM_POINTERENTER
#undef WM_POINTERLEAVE
#undef WM_POINTERACTIVATE
#undef WM_POINTERCAPTURECHANGED
#undef WM_TOUCHHITTESTING
#undef WM_POINTERWHEEL
#undef WM_POINTERHWHEEL
#undef DM_POINTERHITTEST
#undef WM_POINTERROUTEDTO
#undef WM_POINTERROUTEDAWAY
#undef WM_POINTERROUTEDRELEASED
#undef WM_IME_SETCONTEXT
#undef WM_IME_NOTIFY
#undef WM_IME_CONTROL
#undef WM_IME_COMPOSITIONFULL
#undef WM_IME_SELECT
#undef WM_IME_CHAR
#undef WM_IME_REQUEST
#undef WM_IME_KEYDOWN
#undef WM_IME_KEYUP
#undef WM_MOUSEHOVER
#undef WM_MOUSELEAVE
#undef WM_NCMOUSEHOVER
#undef WM_NCMOUSELEAVE
#undef WM_WTSSESSION_CHANGE
#undef WM_TABLET_FIRST
#undef WM_TABLET_LAST
#undef WM_DPICHANGED
#undef WM_DPICHANGED_BEFOREPARENT
#undef WM_DPICHANGED_AFTERPARENT
#undef WM_GETDPISCALEDSIZE
#undef WM_CUT
#undef WM_COPY
#undef WM_PASTE
#undef WM_CLEAR
#undef WM_UNDO
#undef WM_RENDERFORMAT
#undef WM_RENDERALLFORMATS
#undef WM_DESTROYCLIPBOARD
#undef WM_DRAWCLIPBOARD
#undef WM_PAINTCLIPBOARD
#undef WM_VSCROLLCLIPBOARD
#undef WM_SIZECLIPBOARD
#undef WM_ASKCBFORMATNAME
#undef WM_CHANGECBCHAIN
#undef WM_HSCROLLCLIPBOARD
#undef WM_QUERYNEWPALETTE
#undef WM_PALETTEISCHANGING
#undef WM_PALETTECHANGED
#undef WM_HOTKEY
#undef WM_PRINT
#undef WM_PRINTCLIENT
#undef WM_APPCOMMAND
#undef WM_THEMECHANGED
#undef WM_CLIPBOARDUPDATE
#undef WM_DWMCOMPOSITIONCHANGED
#undef WM_DWMNCRENDERINGCHANGED
#undef WM_DWMCOLORIZATIONCOLORCHANGED
#undef WM_DWMWINDOWMAXIMIZEDCHANGE
#undef WM_DWMSENDICONICTHUMBNAIL
#undef WM_DWMSENDICONICLIVEPREVIEWBITMAP
#undef WM_GETTITLEBARINFOEX
#undef WM_HANDHELDFIRST
#undef WM_HANDHELDLAST
#undef WM_AFXFIRST
#undef WM_AFXLAST
#undef WM_PENWINFIRST
#undef WM_PENWINLAST
#undef WM_APP
#undef WM_USER
}

namespace {
#undef WMSZ_LEFT
#undef WMSZ_RIGHT
#undef WMSZ_TOP
#undef WMSZ_TOPLEFT
#undef WMSZ_TOPRIGHT
#undef WMSZ_BOTTOM
#undef WMSZ_BOTTOMLEFT
#undef WMSZ_BOTTOMRIGHT
#undef HTERROR
#undef HTTRANSPARENT
#undef HTNOWHERE
#undef HTCLIENT
#undef HTCAPTION
#undef HTSYSMENU
#undef HTGROWBOX
#undef HTSIZE
#undef HTMENU
#undef HTHSCROLL
#undef HTVSCROLL
#undef HTMINBUTTON
#undef HTMAXBUTTON
#undef HTLEFT
#undef HTRIGHT
#undef HTTOP
#undef HTTOPLEFT
#undef HTTOPRIGHT
#undef HTBOTTOM
#undef HTBOTTOMLEFT
#undef HTBOTTOMRIGHT
#undef HTBORDER
#undef HTREDUCE
#undef HTZOOM
#undef HTSIZEFIRST
#undef HTSIZELAST
#undef HTOBJECT
#undef HTCLOSE
#undef HTHELP
#undef SMTO_NORMAL
#undef SMTO_BLOCK
#undef SMTO_ABORTIFHUNG
#undef SMTO_NOTIMEOUTIFNOTHUNG
#undef SMTO_ERRORONEXIT
#undef MA_ACTIVATE
#undef MA_ACTIVATEANDEAT
#undef MA_NOACTIVATE
#undef MA_NOACTIVATEANDEAT
#undef ICON_SMALL
#undef ICON_BIG
#undef ICON_SMALL2
#undef SIZE_RESTORED
#undef SIZE_MINIMIZED
#undef SIZE_MAXIMIZED
#undef SIZE_MAXSHOW
#undef SIZE_MAXHIDE
#undef SIZENORMAL
#undef SIZEICONIC
#undef SIZEFULLSCREEN
#undef SIZEZOOMSHOW
#undef SIZEZOOMHIDE
#undef WVR_ALIGNTOP
#undef WVR_ALIGNLEFT
#undef WVR_ALIGNBOTTOM
#undef WVR_ALIGNRIGHT
#undef WVR_HREDRAW
#undef WVR_VREDRAW
#undef WVR_REDRAW
#undef WVR_VALIDRECTS
#undef MK_LBUTTON
#undef MK_RBUTTON
#undef MK_SHIFT
#undef MK_CONTROL
#undef MK_MBUTTON
#undef MK_XBUTTON1
#undef MK_XBUTTON2
#undef TME_HOVER
#undef TME_LEAVE
#undef TME_NONCLIENT
#undef TME_QUERY
#undef TME_CANCEL
#undef HOVER_DEFAULT
}

namespace {
#undef GET_WHEEL_DELTA_WPARAM
#undef GET_KEYSTATE_WPARAM
#undef GET_NCHITTEST_WPARAM
#undef GET_XBUTTON_WPARAM
}

//WM_ACTIVE state
export namespace WinAPI {
	inline constexpr uint16_t WA_INACTIVE = 0;
	inline constexpr uint16_t WA_ACTIVE = 1;
	inline constexpr uint16_t WA_CLICKACTIVE = 2;
}

//WM_POWER wParam
export namespace WinAPI {
	inline constexpr int16_t PWR_OK = 1;
	inline constexpr int16_t PWR_FAIL = -1;
	inline constexpr int16_t PWR_SUSPENDREQUEST = 1;
	inline constexpr int16_t PWR_SUSPENDRESUME = 2;
	inline constexpr int16_t PWR_CRITICALRESUME = 3;
}

//WM_UISTATE wParam
export namespace WinAPI {
	//Low Word
	inline constexpr uint16_t UIS_SET = 1;
	inline constexpr uint16_t UIS_CLEAR = 2;
	inline constexpr uint16_t UIS_INITIALIZE = 3;
	//High Word
	inline constexpr uint16_t UISF_HIDEFOCUS = 0x1;
	inline constexpr uint16_t UISF_HIDEACCEL = 0x2;
	inline constexpr uint16_t UISF_ACTIVE = 0x4;
}

//Wheel etc...
export namespace WinAPI {
	inline constexpr uint16_t WHEEL_DELTA = 120;	// Value for rolling one detent
	inline constexpr uint16_t GET_WHEEL_DELTA_WPARAM(WPARAM wParam) noexcept { return static_cast<uint16_t>(wParam >> 16); }
	/* Setting to scroll one page for SPI_GET/SETWHEELSCROLLLINES */
	inline constexpr uint32_t WHEEL_PAGESCROLL = UINT32_MAX;
	inline constexpr uint16_t GET_KEYSTATE_WPARAM(WPARAM wParam) noexcept { return static_cast<uint16_t>(wParam); }
	inline constexpr uint16_t GET_NCHITTEST_WPARAM(WPARAM wParam) noexcept { return static_cast<uint16_t>(wParam); }
	inline constexpr uint16_t GET_XBUTTON_WPARAM(WPARAM wParam) noexcept { return static_cast<uint16_t>(wParam >> 16); }
	/* XButton values are WORD flags */
	inline constexpr uint16_t XBUTTON1 = 0x0001;
	inline constexpr uint16_t XBUTTON2 = 0x0002;
	/* Were there to be an XBUTTON3, its value would be 0x0004 */
}

//Window Messages
export namespace WinAPI {
	inline constexpr uint16_t WM_NULL = 0x0000;
	inline constexpr uint16_t WM_CREATE = 0x0001;
	inline constexpr uint16_t WM_DESTROY = 0x0002;
	inline constexpr uint16_t WM_MOVE = 0x0003;
	inline constexpr uint16_t WM_SIZE = 0x0005;
	inline constexpr uint16_t WM_ACTIVATE = 0x0006;
	inline constexpr uint16_t WM_SETFOCUS = 0x0007;
	inline constexpr uint16_t WM_KILLFOCUS = 0x0008;
	inline constexpr uint16_t WM_ENABLE = 0x000A;
	inline constexpr uint16_t WM_SETREDRAW = 0x000B;
	inline constexpr uint16_t WM_SETTEXT = 0x000C;
	inline constexpr uint16_t WM_GETTEXT = 0x000D;
	inline constexpr uint16_t WM_GETTEXTLENGTH = 0x000E;
	inline constexpr uint16_t WM_PAINT = 0x000F;
	inline constexpr uint16_t WM_CLOSE = 0x0010;
	inline constexpr uint16_t WM_QUERYENDSESSION = 0x0011;
	inline constexpr uint16_t WM_QUERYOPEN = 0x0013;
	inline constexpr uint16_t WM_ENDSESSION = 0x0016;
	inline constexpr uint16_t WM_QUIT = 0x0012;
	inline constexpr uint16_t WM_ERASEBKGND = 0x0014;
	inline constexpr uint16_t WM_SYSCOLORCHANGE = 0x0015;
	inline constexpr uint16_t WM_SHOWWINDOW = 0x0018;
	inline constexpr uint16_t WM_WININICHANGE = 0x001A;
	inline constexpr uint16_t WM_SETTINGCHANGE = WM_WININICHANGE;
	inline constexpr uint16_t WM_DEVMODECHANGE = 0x001B;
	inline constexpr uint16_t WM_ACTIVATEAPP = 0x001C;
	inline constexpr uint16_t WM_FONTCHANGE = 0x001D;
	inline constexpr uint16_t WM_TIMECHANGE = 0x001E;
	inline constexpr uint16_t WM_CANCELMODE = 0x001F;
	inline constexpr uint16_t WM_SETCURSOR = 0x0020;
	inline constexpr uint16_t WM_MOUSEACTIVATE = 0x0021;
	inline constexpr uint16_t WM_CHILDACTIVATE = 0x0022;
	inline constexpr uint16_t WM_QUEUESYNC = 0x0023;
	inline constexpr uint16_t WM_GETMINMAXINFO = 0x0024;
	inline constexpr uint16_t WM_PAINTICON = 0x0026;
	inline constexpr uint16_t WM_ICONERASEBKGND = 0x0027;
	inline constexpr uint16_t WM_NEXTDLGCTL = 0x0028;
	inline constexpr uint16_t WM_SPOOLERSTATUS = 0x002A;
	inline constexpr uint16_t WM_DRAWITEM = 0x002B;
	inline constexpr uint16_t WM_MEASUREITEM = 0x002C;
	inline constexpr uint16_t WM_DELETEITEM = 0x002D;
	inline constexpr uint16_t WM_VKEYTOITEM = 0x002E;
	inline constexpr uint16_t WM_CHARTOITEM = 0x002F;
	inline constexpr uint16_t WM_SETFONT = 0x0030;
	inline constexpr uint16_t WM_GETFONT = 0x0031;
	inline constexpr uint16_t WM_SETHOTKEY = 0x0032;
	inline constexpr uint16_t WM_GETHOTKEY = 0x0033;
	inline constexpr uint16_t WM_QUERYDRAGICON = 0x0037;
	inline constexpr uint16_t WM_COMPAREITEM = 0x0039;
	inline constexpr uint16_t WM_GETOBJECT = 0x003D;
	inline constexpr uint16_t WM_COMPACTING = 0x0041;
	inline constexpr uint16_t WM_COMMNOTIFY = 0x0044;
	inline constexpr uint16_t WM_WINDOWPOSCHANGING = 0x0046;
	inline constexpr uint16_t WM_WINDOWPOSCHANGED = 0x0047;
	inline constexpr uint16_t WM_POWER = 0x0048;
	inline constexpr uint16_t WM_COPYDATA = 0x004A;
	inline constexpr uint16_t WM_CANCELJOURNAL = 0x004B;
	inline constexpr uint16_t WM_NOTIFY = 0x004E;
	inline constexpr uint16_t WM_INPUTLANGCHANGEREQUEST = 0x0050;
	inline constexpr uint16_t WM_INPUTLANGCHANGE = 0x0051;
	inline constexpr uint16_t WM_TCARD = 0x0052;
	inline constexpr uint16_t WM_HELP = 0x0053;
	inline constexpr uint16_t WM_USERCHANGED = 0x0054;
	inline constexpr uint16_t WM_NOTIFYFORMAT = 0x0055;
	inline constexpr uint16_t NFR_ANSI = 1;
	inline constexpr uint16_t NFR_UNICODE = 2;
	inline constexpr uint16_t NF_QUERY = 3;
	inline constexpr uint16_t NF_REQUERY = 4;
	inline constexpr uint16_t WM_CONTEXTMENU = 0x007B;
	inline constexpr uint16_t WM_STYLECHANGING = 0x007C;
	inline constexpr uint16_t WM_STYLECHANGED = 0x007D;
	inline constexpr uint16_t WM_DISPLAYCHANGE = 0x007E;
	inline constexpr uint16_t WM_GETICON = 0x007F;
	inline constexpr uint16_t WM_SETICON = 0x0080;
	inline constexpr uint16_t WM_NCCREATE = 0x0081;
	inline constexpr uint16_t WM_NCDESTROY = 0x0082;
	inline constexpr uint16_t WM_NCCALCSIZE = 0x0083;
	inline constexpr uint16_t WM_NCHITTEST = 0x0084;
	inline constexpr uint16_t WM_NCPAINT = 0x0085;
	inline constexpr uint16_t WM_NCACTIVATE = 0x0086;
	inline constexpr uint16_t WM_GETDLGCODE = 0x0087;
	inline constexpr uint16_t WM_SYNCPAINT = 0x0088;
	inline constexpr uint16_t WM_NCMOUSEMOVE = 0x00A0;
	inline constexpr uint16_t WM_NCLBUTTONDOWN = 0x00A1;
	inline constexpr uint16_t WM_NCLBUTTONUP = 0x00A2;
	inline constexpr uint16_t WM_NCLBUTTONDBLCLK = 0x00A3;
	inline constexpr uint16_t WM_NCRBUTTONDOWN = 0x00A4;
	inline constexpr uint16_t WM_NCRBUTTONUP = 0x00A5;
	inline constexpr uint16_t WM_NCRBUTTONDBLCLK = 0x00A6;
	inline constexpr uint16_t WM_NCMBUTTONDOWN = 0x00A7;
	inline constexpr uint16_t WM_NCMBUTTONUP = 0x00A8;
	inline constexpr uint16_t WM_NCMBUTTONDBLCLK = 0x00A9;
	inline constexpr uint16_t WM_NCXBUTTONDOWN = 0x00AB;
	inline constexpr uint16_t WM_NCXBUTTONUP = 0x00AC;
	inline constexpr uint16_t WM_NCXBUTTONDBLCLK = 0x00AD;
	inline constexpr uint16_t WM_INPUT_DEVICE_CHANGE = 0x00FE;
	inline constexpr uint16_t WM_INPUT = 0x00FF;
	inline constexpr uint16_t WM_KEYFIRST = 0x0100;
	inline constexpr uint16_t WM_KEYDOWN = 0x0100;
	inline constexpr uint16_t WM_KEYUP = 0x0101;
	inline constexpr uint16_t WM_CHAR = 0x0102;
	inline constexpr uint16_t WM_DEADCHAR = 0x0103;
	inline constexpr uint16_t WM_SYSKEYDOWN = 0x0104;
	inline constexpr uint16_t WM_SYSKEYUP = 0x0105;
	inline constexpr uint16_t WM_SYSCHAR = 0x0106;
	inline constexpr uint16_t WM_SYSDEADCHAR = 0x0107;
#if(_WIN32_WINNT >= 0x0501)
	inline constexpr uint16_t WM_UNICHAR = 0x0109;
	inline constexpr uint16_t WM_KEYLAST = 0x0109;
	inline constexpr uint16_t UNICODE_NOCHAR = 0xFFFF;
#else
	inline constexpr uint16_t WM_KEYLAST = 0x0108;
#endif /* _WIN32_WINNT >= 0x0501 */
	inline constexpr uint16_t WM_IME_STARTCOMPOSITION = 0x010D;
	inline constexpr uint16_t WM_IME_ENDCOMPOSITION = 0x010E;
	inline constexpr uint16_t WM_IME_COMPOSITION = 0x010F;
	inline constexpr uint16_t WM_IME_KEYLAST = 0x010F;
	inline constexpr uint16_t WM_INITDIALOG = 0x0110;
	inline constexpr uint16_t WM_COMMAND = 0x0111;
	inline constexpr uint16_t WM_SYSCOMMAND = 0x0112;
	inline constexpr uint16_t WM_TIMER = 0x0113;
	inline constexpr uint16_t WM_HSCROLL = 0x0114;
	inline constexpr uint16_t WM_VSCROLL = 0x0115;
	inline constexpr uint16_t WM_INITMENU = 0x0116;
	inline constexpr uint16_t WM_INITMENUPOPUP = 0x0117;
	inline constexpr uint16_t WM_GESTURE = 0x0119;
	inline constexpr uint16_t WM_GESTURENOTIFY = 0x011A;
	inline constexpr uint16_t WM_MENUSELECT = 0x011F;
	inline constexpr uint16_t WM_MENUCHAR = 0x0120;
	inline constexpr uint16_t WM_ENTERIDLE = 0x0121;
	inline constexpr uint16_t WM_MENURBUTTONUP = 0x0122;
	inline constexpr uint16_t WM_MENUDRAG = 0x0123;
	inline constexpr uint16_t WM_MENUGETOBJECT = 0x0124;
	inline constexpr uint16_t WM_UNINITMENUPOPUP = 0x0125;
	inline constexpr uint16_t WM_MENUCOMMAND = 0x0126;
	inline constexpr uint16_t WM_CHANGEUISTATE = 0x0127;
	inline constexpr uint16_t WM_UPDATEUISTATE = 0x0128;
	inline constexpr uint16_t WM_QUERYUISTATE = 0x0129;
	inline constexpr uint16_t WM_CTLCOLORMSGBOX = 0x0132;
	inline constexpr uint16_t WM_CTLCOLOREDIT = 0x0133;
	inline constexpr uint16_t WM_CTLCOLORLISTBOX = 0x0134;
	inline constexpr uint16_t WM_CTLCOLORBTN = 0x0135;
	inline constexpr uint16_t WM_CTLCOLORDLG = 0x0136;
	inline constexpr uint16_t WM_CTLCOLORSCROLLBAR = 0x0137;
	inline constexpr uint16_t WM_CTLCOLORSTATIC = 0x0138;
	inline constexpr uint16_t MN_GETHMENU = 0x01E1;
	inline constexpr uint16_t WM_MOUSEFIRST = 0x0200;
	inline constexpr uint16_t WM_MOUSEMOVE = 0x0200;
	inline constexpr uint16_t WM_LBUTTONDOWN = 0x0201;
	inline constexpr uint16_t WM_LBUTTONUP = 0x0202;
	inline constexpr uint16_t WM_LBUTTONDBLCLK = 0x0203;
	inline constexpr uint16_t WM_RBUTTONDOWN = 0x0204;
	inline constexpr uint16_t WM_RBUTTONUP = 0x0205;
	inline constexpr uint16_t WM_RBUTTONDBLCLK = 0x0206;
	inline constexpr uint16_t WM_MBUTTONDOWN = 0x0207;
	inline constexpr uint16_t WM_MBUTTONUP = 0x0208;
	inline constexpr uint16_t WM_MBUTTONDBLCLK = 0x0209;
	inline constexpr uint16_t WM_MOUSEWHEEL = 0x020A;
	inline constexpr uint16_t WM_XBUTTONDOWN = 0x020B;
	inline constexpr uint16_t WM_XBUTTONUP = 0x020C;
	inline constexpr uint16_t WM_XBUTTONDBLCLK = 0x020D;
	inline constexpr uint16_t WM_MOUSEHWHEEL = 0x020E;
#if (_WIN32_WINNT >= 0x0600)
	inline constexpr uint16_t WM_MOUSELAST = 0x020E;
#elif (_WIN32_WINNT >= 0x0500)
	inline constexpr uint16_t WM_MOUSELAST = 0x020D;
#elif (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	inline constexpr uint16_t WM_MOUSELAST = 0x020A;
#else
	inline constexpr uint16_t WM_MOUSELAST = 0x0209;
#endif /* (_WIN32_WINNT >= 0x0600) */
	inline constexpr uint16_t WM_PARENTNOTIFY = 0x0210;
	inline constexpr uint16_t WM_ENTERMENULOOP = 0x0211;
	inline constexpr uint16_t WM_EXITMENULOOP = 0x0212;
	inline constexpr uint16_t WM_NEXTMENU = 0x0213;
	inline constexpr uint16_t WM_SIZING = 0x0214;
	inline constexpr uint16_t WM_CAPTURECHANGED = 0x0215;
	inline constexpr uint16_t WM_MOVING = 0x0216;
	inline constexpr uint16_t WM_POWERBROADCAST = 0x0218;
	inline constexpr uint16_t WM_DEVICECHANGE = 0x0219;
	inline constexpr uint16_t WM_MDICREATE = 0x0220;
	inline constexpr uint16_t WM_MDIDESTROY = 0x0221;
	inline constexpr uint16_t WM_MDIACTIVATE = 0x0222;
	inline constexpr uint16_t WM_MDIRESTORE = 0x0223;
	inline constexpr uint16_t WM_MDINEXT = 0x0224;
	inline constexpr uint16_t WM_MDIMAXIMIZE = 0x0225;
	inline constexpr uint16_t WM_MDITILE = 0x0226;
	inline constexpr uint16_t WM_MDICASCADE = 0x0227;
	inline constexpr uint16_t WM_MDIICONARRANGE = 0x0228;
	inline constexpr uint16_t WM_MDIGETACTIVE = 0x0229;
	inline constexpr uint16_t WM_MDISETMENU = 0x0230;
	inline constexpr uint16_t WM_ENTERSIZEMOVE = 0x0231;
	inline constexpr uint16_t WM_EXITSIZEMOVE = 0x0232;
	inline constexpr uint16_t WM_DROPFILES = 0x0233;
	inline constexpr uint16_t WM_MDIREFRESHMENU = 0x0234;
	inline constexpr uint16_t WM_POINTERDEVICECHANGE = 0x238;
	inline constexpr uint16_t WM_POINTERDEVICEINRANGE = 0x239;
	inline constexpr uint16_t WM_POINTERDEVICEOUTOFRANGE = 0x23A;
	inline constexpr uint16_t WM_TOUCH = 0x0240;
	inline constexpr uint16_t WM_NCPOINTERUPDATE = 0x0241;
	inline constexpr uint16_t WM_NCPOINTERDOWN = 0x0242;
	inline constexpr uint16_t WM_NCPOINTERUP = 0x0243;
	inline constexpr uint16_t WM_POINTERUPDATE = 0x0245;
	inline constexpr uint16_t WM_POINTERDOWN = 0x0246;
	inline constexpr uint16_t WM_POINTERUP = 0x0247;
	inline constexpr uint16_t WM_POINTERENTER = 0x0249;
	inline constexpr uint16_t WM_POINTERLEAVE = 0x024A;
	inline constexpr uint16_t WM_POINTERACTIVATE = 0x024B;
	inline constexpr uint16_t WM_POINTERCAPTURECHANGED = 0x024C;
	inline constexpr uint16_t WM_TOUCHHITTESTING = 0x024D;
	inline constexpr uint16_t WM_POINTERWHEEL = 0x024E;
	inline constexpr uint16_t WM_POINTERHWHEEL = 0x024F;
	inline constexpr uint16_t DM_POINTERHITTEST = 0x0250;
	inline constexpr uint16_t WM_POINTERROUTEDTO = 0x0251;
	inline constexpr uint16_t WM_POINTERROUTEDAWAY = 0x0252;
	inline constexpr uint16_t WM_POINTERROUTEDRELEASED = 0x0253;
	inline constexpr uint16_t WM_IME_SETCONTEXT = 0x0281;
	inline constexpr uint16_t WM_IME_NOTIFY = 0x0282;
	inline constexpr uint16_t WM_IME_CONTROL = 0x0283;
	inline constexpr uint16_t WM_IME_COMPOSITIONFULL = 0x0284;
	inline constexpr uint16_t WM_IME_SELECT = 0x0285;
	inline constexpr uint16_t WM_IME_CHAR = 0x0286;
	inline constexpr uint16_t WM_IME_REQUEST = 0x0288;
	inline constexpr uint16_t WM_IME_KEYDOWN = 0x0290;
	inline constexpr uint16_t WM_IME_KEYUP = 0x0291;
	inline constexpr uint16_t WM_MOUSEHOVER = 0x02A1;
	inline constexpr uint16_t WM_MOUSELEAVE = 0x02A3;
	inline constexpr uint16_t WM_NCMOUSEHOVER = 0x02A0;
	inline constexpr uint16_t WM_NCMOUSELEAVE = 0x02A2;
	inline constexpr uint16_t WM_WTSSESSION_CHANGE = 0x02B1;
	inline constexpr uint16_t WM_TABLET_FIRST = 0x02c0;
	inline constexpr uint16_t WM_TABLET_LAST = 0x02df;
	inline constexpr uint16_t WM_DPICHANGED = 0x02E0;
	inline constexpr uint16_t WM_DPICHANGED_BEFOREPARENT = 0x02E2;
	inline constexpr uint16_t WM_DPICHANGED_AFTERPARENT = 0x02E3;
	inline constexpr uint16_t WM_GETDPISCALEDSIZE = 0x02E4;
	inline constexpr uint16_t WM_CUT = 0x0300;
	inline constexpr uint16_t WM_COPY = 0x0301;
	inline constexpr uint16_t WM_PASTE = 0x0302;
	inline constexpr uint16_t WM_CLEAR = 0x0303;
	inline constexpr uint16_t WM_UNDO = 0x0304;
	inline constexpr uint16_t WM_RENDERFORMAT = 0x0305;
	inline constexpr uint16_t WM_RENDERALLFORMATS = 0x0306;
	inline constexpr uint16_t WM_DESTROYCLIPBOARD = 0x0307;
	inline constexpr uint16_t WM_DRAWCLIPBOARD = 0x0308;
	inline constexpr uint16_t WM_PAINTCLIPBOARD = 0x0309;
	inline constexpr uint16_t WM_VSCROLLCLIPBOARD = 0x030A;
	inline constexpr uint16_t WM_SIZECLIPBOARD = 0x030B;
	inline constexpr uint16_t WM_ASKCBFORMATNAME = 0x030C;
	inline constexpr uint16_t WM_CHANGECBCHAIN = 0x030D;
	inline constexpr uint16_t WM_HSCROLLCLIPBOARD = 0x030E;
	inline constexpr uint16_t WM_QUERYNEWPALETTE = 0x030F;
	inline constexpr uint16_t WM_PALETTEISCHANGING = 0x0310;
	inline constexpr uint16_t WM_PALETTECHANGED = 0x0311;
	inline constexpr uint16_t WM_HOTKEY = 0x0312;
	inline constexpr uint16_t WM_PRINT = 0x0317;
	inline constexpr uint16_t WM_PRINTCLIENT = 0x0318;
	inline constexpr uint16_t WM_APPCOMMAND = 0x0319;
	inline constexpr uint16_t WM_THEMECHANGED = 0x031A;
	inline constexpr uint16_t WM_CLIPBOARDUPDATE = 0x031D;
	inline constexpr uint16_t WM_DWMCOMPOSITIONCHANGED = 0x031E;
	inline constexpr uint16_t WM_DWMNCRENDERINGCHANGED = 0x031F;
	inline constexpr uint16_t WM_DWMCOLORIZATIONCOLORCHANGED = 0x0320;
	inline constexpr uint16_t WM_DWMWINDOWMAXIMIZEDCHANGE = 0x0321;
	inline constexpr uint16_t WM_DWMSENDICONICTHUMBNAIL = 0x0323;
	inline constexpr uint16_t WM_DWMSENDICONICLIVEPREVIEWBITMAP = 0x0326;
	inline constexpr uint16_t WM_GETTITLEBARINFOEX = 0x033F;
	inline constexpr uint16_t WM_HANDHELDFIRST = 0x0358;
	inline constexpr uint16_t WM_HANDHELDLAST = 0x035F;
	inline constexpr uint16_t WM_AFXFIRST = 0x0360;
	inline constexpr uint16_t WM_AFXLAST = 0x037F;
	inline constexpr uint16_t WM_PENWINFIRST = 0x0380;
	inline constexpr uint16_t WM_PENWINLAST = 0x038F;
	inline constexpr uint16_t WM_APP = 0x8000;
	inline constexpr uint16_t WM_USER = 0x0400;
}

//Window Message Values
export namespace WinAPI {
	//WM_SIZE
	inline constexpr int16_t WMSZ_LEFT = 1;
	inline constexpr int16_t WMSZ_RIGHT = 2;
	inline constexpr int16_t WMSZ_TOP = 3;
	inline constexpr int16_t WMSZ_TOPLEFT = 4;
	inline constexpr int16_t WMSZ_TOPRIGHT = 5;
	inline constexpr int16_t WMSZ_BOTTOM = 6;
	inline constexpr int16_t WMSZ_BOTTOMLEFT = 7;
	inline constexpr int16_t WMSZ_BOTTOMRIGHT = 8;
	//WM_NCHITTEST/MOUSEHOOKSTRUCT Mouse Position
	inline constexpr int16_t HTERROR = (-2);
	inline constexpr int16_t HTTRANSPARENT = (-1);
	inline constexpr int16_t HTNOWHERE = 0;
	inline constexpr int16_t HTCLIENT = 1;
	inline constexpr int16_t HTCAPTION = 2;
	inline constexpr int16_t HTSYSMENU = 3;
	inline constexpr int16_t HTGROWBOX = 4;
	inline constexpr int16_t HTSIZE = HTGROWBOX;
	inline constexpr int16_t HTMENU = 5;
	inline constexpr int16_t HTHSCROLL = 6;
	inline constexpr int16_t HTVSCROLL = 7;
	inline constexpr int16_t HTMINBUTTON = 8;
	inline constexpr int16_t HTMAXBUTTON = 9;
	inline constexpr int16_t HTLEFT = 10;
	inline constexpr int16_t HTRIGHT = 11;
	inline constexpr int16_t HTTOP = 12;
	inline constexpr int16_t HTTOPLEFT = 13;
	inline constexpr int16_t HTTOPRIGHT = 14;
	inline constexpr int16_t HTBOTTOM = 15;
	inline constexpr int16_t HTBOTTOMLEFT = 16;
	inline constexpr int16_t HTBOTTOMRIGHT = 17;
	inline constexpr int16_t HTBORDER = 18;
	inline constexpr int16_t HTREDUCE = HTMINBUTTON;
	inline constexpr int16_t HTZOOM = HTMAXBUTTON;
	inline constexpr int16_t HTSIZEFIRST = HTLEFT;
	inline constexpr int16_t HTSIZELAST = HTBOTTOMRIGHT;
	inline constexpr int16_t HTOBJECT = 19;
	inline constexpr int16_t HTCLOSE = 20;
	inline constexpr int16_t HTHELP = 21;
	//SendMessageTimeout value
	inline constexpr int16_t SMTO_NORMAL = 0x0000;
	inline constexpr int16_t SMTO_BLOCK = 0x0001;
	inline constexpr int16_t SMTO_ABORTIFHUNG = 0x0002;
	inline constexpr int16_t SMTO_NOTIMEOUTIFNOTHUNG = 0x0008;
	inline constexpr int16_t SMTO_ERRORONEXIT = 0x0020;
	//WM_MOUSEACTIVATE
	inline constexpr int16_t MA_ACTIVATE = 1;
	inline constexpr int16_t MA_ACTIVATEANDEAT = 2;
	inline constexpr int16_t MA_NOACTIVATE = 3;
	inline constexpr int16_t MA_NOACTIVATEANDEAT = 4;
	//WM_SETICON/ WM_GETICON type code
	inline constexpr int16_t ICON_SMALL = 0;
	inline constexpr int16_t ICON_BIG = 1;
	inline constexpr int16_t ICON_SMALL2 = 2;
	//WM_SIZE wParam
	inline constexpr int16_t SIZE_RESTORED = 0;
	inline constexpr int16_t SIZE_MINIMIZED = 1;
	inline constexpr int16_t SIZE_MAXIMIZED = 2;
	inline constexpr int16_t SIZE_MAXSHOW = 3;
	inline constexpr int16_t SIZE_MAXHIDE = 4;
	inline constexpr int16_t SIZENORMAL = SIZE_RESTORED;
	inline constexpr int16_t SIZEICONIC = SIZE_MINIMIZED;
	inline constexpr int16_t SIZEFULLSCREEN = SIZE_MAXIMIZED;
	inline constexpr int16_t SIZEZOOMSHOW = SIZE_MAXSHOW;
	inline constexpr int16_t SIZEZOOMHIDE = SIZE_MAXHIDE;
	//WM_NCCALCSIZE window valid rect
	inline constexpr int16_t WVR_ALIGNTOP = 0x0010;
	inline constexpr int16_t WVR_ALIGNLEFT = 0x0020;
	inline constexpr int16_t WVR_ALIGNBOTTOM = 0x0040;
	inline constexpr int16_t WVR_ALIGNRIGHT = 0x0080;
	inline constexpr int16_t WVR_HREDRAW = 0x0100;
	inline constexpr int16_t WVR_VREDRAW = 0x0200;
	inline constexpr int16_t WVR_REDRAW = (WVR_HREDRAW | WVR_VREDRAW);
	inline constexpr int16_t WVR_VALIDRECTS = 0x0400;
	//Mouse Message Mask
	inline constexpr int16_t MK_LBUTTON = 0x0001;
	inline constexpr int16_t MK_RBUTTON = 0x0002;
	inline constexpr int16_t MK_SHIFT = 0x0004;
	inline constexpr int16_t MK_CONTROL = 0x0008;
	inline constexpr int16_t MK_MBUTTON = 0x0010;
	inline constexpr int16_t MK_XBUTTON1 = 0x0020;
	inline constexpr int16_t MK_XBUTTON2 = 0x0040;
	//Track Mouse Event
	inline constexpr int32_t TME_HOVER = 0x00000001;
	inline constexpr int32_t TME_LEAVE = 0x00000002;
	inline constexpr int32_t TME_NONCLIENT = 0x00000010;
	inline constexpr int32_t TME_QUERY = 0x40000000;
	inline constexpr int32_t TME_CANCEL = 0x80000000;
	inline constexpr int32_t HOVER_DEFAULT = 0xFFFFFFFF;
}
