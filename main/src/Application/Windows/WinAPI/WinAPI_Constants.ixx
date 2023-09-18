module;
#define NOMINMAX
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#include<Windows.h>
export module WinAPI_Constants;
import CSTDINT;
using namespace System;

//HWND_CONSTANT impl
export namespace WinAPI {
	union HWND_CONSTANT {
		int64_t value;
		void* ptr;
	public:
		constexpr HWND_CONSTANT() noexcept = default;
		constexpr HWND_CONSTANT(int64_t val) noexcept : value(val) {}
		operator HWND() const noexcept { return static_cast<HWND>(ptr); }
	};

	template<class T>
	union PTR_CONSTANT {
		int64_t value;
		void* ptr;
	public:
		constexpr PTR_CONSTANT() noexcept = default;
		constexpr PTR_CONSTANT(int64_t val) noexcept : value(val) {}
		constexpr PTR_CONSTANT(uint64_t val) noexcept : value(val) {}
		constexpr PTR_CONSTANT(int32_t val) noexcept : value(val) {}
		constexpr PTR_CONSTANT(uint32_t val) noexcept : value(val) {}
		constexpr PTR_CONSTANT(int16_t val) noexcept : value(val) {}
		constexpr PTR_CONSTANT(uint16_t val) noexcept : value(val) {}
	public:
		explicit constexpr operator int64_t() const noexcept { return static_cast<int64_t>(value); }
		explicit constexpr operator uint64_t() const noexcept { return static_cast<uint64_t>(value); }
		explicit constexpr operator int32_t() const noexcept { return static_cast<int32_t>(value); }
		explicit constexpr operator uint32_t() const noexcept { return static_cast<uint32_t>(value); }
		explicit constexpr operator int16_t() const noexcept { return static_cast<int16_t>(value); }
		explicit constexpr operator uint16_t() const noexcept { return static_cast<uint16_t>(value); }
	public:
		operator T() const noexcept { return static_cast<T>(ptr); }
	};
}

//Virtual Keys
export namespace WinAPI {
#undef VK_LBUTTON
#undef VK_RBUTTON
#undef VK_CANCEL
#undef VK_MBUTTON
#undef VK_XBUTTON1
#undef VK_XBUTTON2
//0x07 : reserved
#undef VK_BACK
#undef VK_TAB
//0x0A - 0x0B : reserved
#undef VK_CLEAR
#undef VK_RETURN
//0x0E - 0x0F : unassigned
#undef VK_SHIFT
#undef VK_CONTROL
#undef VK_MENU
#undef VK_PAUSE
#undef VK_CAPITAL
#undef VK_KANA
#undef VK_HANGEUL
#undef VK_HANGUL
#undef VK_IME_ON
#undef VK_JUNJA
#undef VK_FINAL
#undef VK_HANJA
#undef VK_KANJI
#undef VK_IME_OFF
#undef VK_ESCAPE
#undef VK_CONVERT
#undef VK_NONCONVERT
#undef VK_ACCEPT
#undef VK_MODECHANGE
#undef VK_SPACE
#undef VK_PRIOR
#undef VK_NEXT
#undef VK_END
#undef VK_HOME
#undef VK_LEFT
#undef VK_UP
#undef VK_RIGHT
#undef VK_DOWN
#undef VK_SELECT
#undef VK_PRINT
#undef VK_EXECUTE
#undef VK_SNAPSHOT
#undef VK_INSERT
#undef VK_DELETE
#undef VK_HELP
//VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
//0x3A - 0x40 : unassigned
//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
#undef VK_LWIN
#undef VK_RWIN
#undef VK_APPS
//0x5E : reserved
#undef VK_SLEEP
#undef VK_NUMPAD0
#undef VK_NUMPAD1
#undef VK_NUMPAD2
#undef VK_NUMPAD3
#undef VK_NUMPAD4
#undef VK_NUMPAD5
#undef VK_NUMPAD6
#undef VK_NUMPAD7
#undef VK_NUMPAD8
#undef VK_NUMPAD9
#undef VK_MULTIPLY
#undef VK_ADD
#undef VK_SEPARATOR
#undef VK_SUBTRACT
#undef VK_DECIMAL
#undef VK_DIVIDE
#undef VK_F1
#undef VK_F2
#undef VK_F3
#undef VK_F4
#undef VK_F5
#undef VK_F6
#undef VK_F7
#undef VK_F8
#undef VK_F9
#undef VK_F10
#undef VK_F11
#undef VK_F12
#undef VK_F13
#undef VK_F14
#undef VK_F15
#undef VK_F16
#undef VK_F17
#undef VK_F18
#undef VK_F19
#undef VK_F20
#undef VK_F21
#undef VK_F22
#undef VK_F23
#undef VK_F24
#undef VK_NAVIGATION_VIEW
#undef VK_NAVIGATION_MENU
#undef VK_NAVIGATION_UP
#undef VK_NAVIGATION_DOWN
#undef VK_NAVIGATION_LEFT
#undef VK_NAVIGATION_RIGHT
#undef VK_NAVIGATION_ACCEPT
#undef VK_NAVIGATION_CANCEL
#undef VK_NUMLOCK
#undef VK_SCROLL
#undef VK_OEM_NEC_EQUAL
#undef VK_OEM_FJ_JISHO
#undef VK_OEM_FJ_MASSHOU	// 'Unregister word' key
#undef VK_OEM_FJ_TOUROKU	// 'Register word' key
#undef VK_OEM_FJ_LOYA
#undef VK_OEM_FJ_ROYA
//0x97 - 0x9F : unassigned
#undef VK_LSHIFT
#undef VK_RSHIFT
#undef VK_LCONTROL
#undef VK_RCONTROL
#undef VK_LMENU
#undef VK_RMENU
#undef VK_BROWSER_BACK
#undef VK_BROWSER_FORWARD
#undef VK_BROWSER_REFRESH
#undef VK_BROWSER_STOP
#undef VK_BROWSER_SEARCH
#undef VK_BROWSER_FAVORITES
#undef VK_BROWSER_HOME
#undef VK_VOLUME_MUTE
#undef VK_VOLUME_DOWN
#undef VK_VOLUME_UP
#undef VK_MEDIA_NEXT_TRACK
#undef VK_MEDIA_PREV_TRACK
#undef VK_MEDIA_STOP
#undef VK_MEDIA_PLAY_PAUSE
#undef VK_LAUNCH_MAIL
#undef VK_LAUNCH_MEDIA_SELECT
#undef VK_LAUNCH_APP1
#undef VK_LAUNCH_APP2
//0xB8 - 0xB9 : reserved
#undef VK_OEM_1
#undef VK_OEM_PLUS
#undef VK_OEM_COMMA
#undef VK_OEM_MINUS
#undef VK_OEM_PERIOD
#undef VK_OEM_2
#undef VK_OEM_3
//0xC1 - 0xC2 : reserved
#undef VK_GAMEPAD_A
#undef VK_GAMEPAD_B
#undef VK_GAMEPAD_X
#undef VK_GAMEPAD_Y
#undef VK_GAMEPAD_RIGHT_SHOULDER
#undef VK_GAMEPAD_LEFT_SHOULDER
#undef VK_GAMEPAD_LEFT_TRIGGER
#undef VK_GAMEPAD_RIGHT_TRIGGER
#undef VK_GAMEPAD_DPAD_UP
#undef VK_GAMEPAD_DPAD_DOWN
#undef VK_GAMEPAD_DPAD_LEFT
#undef VK_GAMEPAD_DPAD_RIGHT
#undef VK_GAMEPAD_MENU
#undef VK_GAMEPAD_VIEW
#undef VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON
#undef VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON
#undef VK_GAMEPAD_LEFT_THUMBSTICK_UP
#undef VK_GAMEPAD_LEFT_THUMBSTICK_DOWN
#undef VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT
#undef VK_GAMEPAD_LEFT_THUMBSTICK_LEFT
#undef VK_GAMEPAD_RIGHT_THUMBSTICK_UP
#undef VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN
#undef VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT
#undef VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT
#undef VK_OEM_4
#undef VK_OEM_5
#undef VK_OEM_6
#undef VK_OEM_7
#undef VK_OEM_8
//0xE0 : reserved
#undef VK_OEM_AX
#undef VK_OEM_102
#undef VK_ICO_HELP
#undef VK_ICO_00
#undef VK_PROCESSKEY
#undef VK_ICO_CLEAR
#undef VK_PACKET
//0xE8 : unassigned
#undef VK_OEM_RESET
#undef VK_OEM_JUMP
#undef VK_OEM_PA1
#undef VK_OEM_PA2
#undef VK_OEM_PA3
#undef VK_OEM_WSCTRL
#undef VK_OEM_CUSEL
#undef VK_OEM_ATTN
#undef VK_OEM_FINISH
#undef VK_OEM_COPY
#undef VK_OEM_AUTO
#undef VK_OEM_ENLW
#undef VK_OEM_BACKTAB
#undef VK_ATTN
#undef VK_CRSEL
#undef VK_EXSEL
#undef VK_EREOF
#undef VK_PLAY
#undef VK_ZOOM
#undef VK_NONAME
#undef VK_PA1
#undef VK_OEM_CLEAR

//0xFF : reserved

	inline constexpr int32_t VK_LBUTTON = 0x01;
	inline constexpr int32_t VK_RBUTTON = 0x02;
	inline constexpr int32_t VK_CANCEL = 0x03;
	inline constexpr int32_t VK_MBUTTON = 0x04;
	inline constexpr int32_t VK_XBUTTON1 = 0x05;
	inline constexpr int32_t VK_XBUTTON2 = 0x06;
//0x07 : reserved
	inline constexpr int32_t VK_BACK = 0x08;
	inline constexpr int32_t VK_TAB = 0x09;
//0x0A - 0x0B : reserved
	inline constexpr int32_t VK_CLEAR = 0x0C;
	inline constexpr int32_t VK_RETURN = 0x0D;
//0x0E - 0x0F : unassigned
	inline constexpr int32_t VK_SHIFT = 0x10;
	inline constexpr int32_t VK_CONTROL = 0x11;
	inline constexpr int32_t VK_MENU = 0x12;
	inline constexpr int32_t VK_PAUSE = 0x13;
	inline constexpr int32_t VK_CAPITAL = 0x14;
	inline constexpr int32_t VK_KANA = 0x15;
	inline constexpr int32_t VK_HANGEUL = 0x15;
	inline constexpr int32_t VK_HANGUL = 0x15;
	inline constexpr int32_t VK_IME_ON = 0x16;
	inline constexpr int32_t VK_JUNJA = 0x17;
	inline constexpr int32_t VK_FINAL = 0x18;
	inline constexpr int32_t VK_HANJA = 0x19;
	inline constexpr int32_t VK_KANJI = 0x19;
	inline constexpr int32_t VK_IME_OFF = 0x1A;
	inline constexpr int32_t VK_ESCAPE = 0x1B;
	inline constexpr int32_t VK_CONVERT = 0x1C;
	inline constexpr int32_t VK_NONCONVERT = 0x1D;
	inline constexpr int32_t VK_ACCEPT = 0x1E;
	inline constexpr int32_t VK_MODECHANGE = 0x1F;
	inline constexpr int32_t VK_SPACE = 0x20;
	inline constexpr int32_t VK_PRIOR = 0x21;
	inline constexpr int32_t VK_NEXT = 0x22;
	inline constexpr int32_t VK_END = 0x23;
	inline constexpr int32_t VK_HOME = 0x24;
	inline constexpr int32_t VK_LEFT = 0x25;
	inline constexpr int32_t VK_UP = 0x26;
	inline constexpr int32_t VK_RIGHT = 0x27;
	inline constexpr int32_t VK_DOWN = 0x28;
	inline constexpr int32_t VK_SELECT = 0x29;
	inline constexpr int32_t VK_PRINT = 0x2A;
	inline constexpr int32_t VK_EXECUTE = 0x2B;
	inline constexpr int32_t VK_SNAPSHOT = 0x2C;
	inline constexpr int32_t VK_INSERT = 0x2D;
	inline constexpr int32_t VK_DELETE = 0x2E;
	inline constexpr int32_t VK_HELP = 0x2F;
//VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
//0x3A - 0x40 : unassigned
//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	inline constexpr int32_t VK_LWIN = 0x5B;
	inline constexpr int32_t VK_RWIN = 0x5C;
	inline constexpr int32_t VK_APPS = 0x5D;
//0x5E : reserved
	inline constexpr int32_t VK_SLEEP = 0x5F;
	inline constexpr int32_t VK_NUMPAD0 = 0x60;
	inline constexpr int32_t VK_NUMPAD1 = 0x61;
	inline constexpr int32_t VK_NUMPAD2 = 0x62;
	inline constexpr int32_t VK_NUMPAD3 = 0x63;
	inline constexpr int32_t VK_NUMPAD4 = 0x64;
	inline constexpr int32_t VK_NUMPAD5 = 0x65;
	inline constexpr int32_t VK_NUMPAD6 = 0x66;
	inline constexpr int32_t VK_NUMPAD7 = 0x67;
	inline constexpr int32_t VK_NUMPAD8 = 0x68;
	inline constexpr int32_t VK_NUMPAD9 = 0x69;
	inline constexpr int32_t VK_MULTIPLY = 0x6A;
	inline constexpr int32_t VK_ADD = 0x6B;
	inline constexpr int32_t VK_SEPARATOR = 0x6C;
	inline constexpr int32_t VK_SUBTRACT = 0x6D;
	inline constexpr int32_t VK_DECIMAL = 0x6E;
	inline constexpr int32_t VK_DIVIDE = 0x6F;
	inline constexpr int32_t VK_F1 = 0x70;
	inline constexpr int32_t VK_F2 = 0x71;
	inline constexpr int32_t VK_F3 = 0x72;
	inline constexpr int32_t VK_F4 = 0x73;
	inline constexpr int32_t VK_F5 = 0x74;
	inline constexpr int32_t VK_F6 = 0x75;
	inline constexpr int32_t VK_F7 = 0x76;
	inline constexpr int32_t VK_F8 = 0x77;
	inline constexpr int32_t VK_F9 = 0x78;
	inline constexpr int32_t VK_F10 = 0x79;
	inline constexpr int32_t VK_F11 = 0x7A;
	inline constexpr int32_t VK_F12 = 0x7B;
	inline constexpr int32_t VK_F13 = 0x7C;
	inline constexpr int32_t VK_F14 = 0x7D;
	inline constexpr int32_t VK_F15 = 0x7E;
	inline constexpr int32_t VK_F16 = 0x7F;
	inline constexpr int32_t VK_F17 = 0x80;
	inline constexpr int32_t VK_F18 = 0x81;
	inline constexpr int32_t VK_F19 = 0x82;
	inline constexpr int32_t VK_F20 = 0x83;
	inline constexpr int32_t VK_F21 = 0x84;
	inline constexpr int32_t VK_F22 = 0x85;
	inline constexpr int32_t VK_F23 = 0x86;
	inline constexpr int32_t VK_F24 = 0x87;
	inline constexpr int32_t VK_NAVIGATION_VIEW = 0x88;	// reserved
	inline constexpr int32_t VK_NAVIGATION_MENU = 0x89;	// reserved
	inline constexpr int32_t VK_NAVIGATION_UP = 0x8A;	// reserved
	inline constexpr int32_t VK_NAVIGATION_DOWN = 0x8B;	// reserved
	inline constexpr int32_t VK_NAVIGATION_LEFT = 0x8C;	// reserved
	inline constexpr int32_t VK_NAVIGATION_RIGHT = 0x8D;	// reserved
	inline constexpr int32_t VK_NAVIGATION_ACCEPT = 0x8E;	// reserved
	inline constexpr int32_t VK_NAVIGATION_CANCEL = 0x8F;	// reserved
	inline constexpr int32_t VK_NUMLOCK = 0x90;
	inline constexpr int32_t VK_SCROLL = 0x91;
	inline constexpr int32_t VK_OEM_NEC_EQUAL = 0x92;	// '=' key on numpad
	inline constexpr int32_t VK_OEM_FJ_JISHO = 0x92;	// 'Dictionary' key
	inline constexpr int32_t VK_OEM_FJ_MASSHOU = 0x93;	// 'Unregister word' key
	inline constexpr int32_t VK_OEM_FJ_TOUROKU = 0x94;	// 'Register word' key
	inline constexpr int32_t VK_OEM_FJ_LOYA = 0x95;	// 'Left OYAYUBI' key
	inline constexpr int32_t VK_OEM_FJ_ROYA = 0x96;	// 'Right OYAYUBI' key
//0x97 - 0x9F : unassigned
	inline constexpr int32_t VK_LSHIFT = 0xA0;
	inline constexpr int32_t VK_RSHIFT = 0xA1;
	inline constexpr int32_t VK_LCONTROL = 0xA2;
	inline constexpr int32_t VK_RCONTROL = 0xA3;
	inline constexpr int32_t VK_LMENU = 0xA4;
	inline constexpr int32_t VK_RMENU = 0xA5;
	inline constexpr int32_t VK_BROWSER_BACK = 0xA6;
	inline constexpr int32_t VK_BROWSER_FORWARD = 0xA7;
	inline constexpr int32_t VK_BROWSER_REFRESH = 0xA8;
	inline constexpr int32_t VK_BROWSER_STOP = 0xA9;
	inline constexpr int32_t VK_BROWSER_SEARCH = 0xAA;
	inline constexpr int32_t VK_BROWSER_FAVORITES = 0xAB;
	inline constexpr int32_t VK_BROWSER_HOME = 0xAC;
	inline constexpr int32_t VK_VOLUME_MUTE = 0xAD;
	inline constexpr int32_t VK_VOLUME_DOWN = 0xAE;
	inline constexpr int32_t VK_VOLUME_UP = 0xAF;
	inline constexpr int32_t VK_MEDIA_NEXT_TRACK = 0xB0;
	inline constexpr int32_t VK_MEDIA_PREV_TRACK = 0xB1;
	inline constexpr int32_t VK_MEDIA_STOP = 0xB2;
	inline constexpr int32_t VK_MEDIA_PLAY_PAUSE = 0xB3;
	inline constexpr int32_t VK_LAUNCH_MAIL = 0xB4;
	inline constexpr int32_t VK_LAUNCH_MEDIA_SELECT = 0xB5;
	inline constexpr int32_t VK_LAUNCH_APP1 = 0xB6;
	inline constexpr int32_t VK_LAUNCH_APP2 = 0xB7;
//0xB8 - 0xB9 : reserved
	inline constexpr int32_t VK_OEM_1 = 0xBA;	// ';:' for US
	inline constexpr int32_t VK_OEM_PLUS = 0xBB;	// '+' any country
	inline constexpr int32_t VK_OEM_COMMA = 0xBC;	// ',' any country
	inline constexpr int32_t VK_OEM_MINUS = 0xBD;	// '-' any country
	inline constexpr int32_t VK_OEM_PERIOD = 0xBE;	// '.' any country
	inline constexpr int32_t VK_OEM_2 = 0xBF;	// '/?' for US
	inline constexpr int32_t VK_OEM_3 = 0xC0;	// '`~' for US
//0xC1 - 0xC2 : reserved
	inline constexpr int32_t VK_GAMEPAD_A = 0xC3;	// reserved
	inline constexpr int32_t VK_GAMEPAD_B = 0xC4;	// reserved
	inline constexpr int32_t VK_GAMEPAD_X = 0xC5;	// reserved
	inline constexpr int32_t VK_GAMEPAD_Y = 0xC6;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_SHOULDER = 0xC7;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_SHOULDER = 0xC8;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_TRIGGER = 0xC9;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_TRIGGER = 0xCA;	// reserved
	inline constexpr int32_t VK_GAMEPAD_DPAD_UP = 0xCB;	// reserved
	inline constexpr int32_t VK_GAMEPAD_DPAD_DOWN = 0xCC;	// reserved
	inline constexpr int32_t VK_GAMEPAD_DPAD_LEFT = 0xCD;	// reserved
	inline constexpr int32_t VK_GAMEPAD_DPAD_RIGHT = 0xCE;	// reserved
	inline constexpr int32_t VK_GAMEPAD_MENU = 0xCF;	// reserved
	inline constexpr int32_t VK_GAMEPAD_VIEW = 0xD0;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5;	// reserved
	inline constexpr int32_t VK_GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9;	// reserved
	inline constexpr int32_t VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA;	// reserved
	inline constexpr int32_t VK_OEM_4 = 0xDB;	// '[{' for US
	inline constexpr int32_t VK_OEM_5 = 0xDC;	// '\|' for US
	inline constexpr int32_t VK_OEM_6 = 0xDD;	// ']}' for US
	inline constexpr int32_t VK_OEM_7 = 0xDE;	// ''"' for US
	inline constexpr int32_t VK_OEM_8 = 0xDF;
//0xE0 : reserved
	inline constexpr int32_t VK_OEM_AX = 0xE1;	// 'AX' key on Japanese AX kbd
	inline constexpr int32_t VK_OEM_102 = 0xE2;	// "<>" or "\|" on RT = 102-key kbd.
	inline constexpr int32_t VK_ICO_HELP = 0xE3;	// Help key on ICO
	inline constexpr int32_t VK_ICO_00 = 0xE4;	// 00 key on ICO
	inline constexpr int32_t VK_PROCESSKEY = 0xE5;
	inline constexpr int32_t VK_ICO_CLEAR = 0xE6;
	inline constexpr int32_t VK_PACKET = 0xE7;
//0xE8 : unassigned
	inline constexpr int32_t VK_OEM_RESET = 0xE9;
	inline constexpr int32_t VK_OEM_JUMP = 0xEA;
	inline constexpr int32_t VK_OEM_PA1 = 0xEB;
	inline constexpr int32_t VK_OEM_PA2 = 0xEC;
	inline constexpr int32_t VK_OEM_PA3 = 0xED;
	inline constexpr int32_t VK_OEM_WSCTRL = 0xEE;
	inline constexpr int32_t VK_OEM_CUSEL = 0xEF;
	inline constexpr int32_t VK_OEM_ATTN = 0xF0;
	inline constexpr int32_t VK_OEM_FINISH = 0xF1;
	inline constexpr int32_t VK_OEM_COPY = 0xF2;
	inline constexpr int32_t VK_OEM_AUTO = 0xF3;
	inline constexpr int32_t VK_OEM_ENLW = 0xF4;
	inline constexpr int32_t VK_OEM_BACKTAB = 0xF5;
	inline constexpr int32_t VK_ATTN = 0xF6;
	inline constexpr int32_t VK_CRSEL = 0xF7;
	inline constexpr int32_t VK_EXSEL = 0xF8;
	inline constexpr int32_t VK_EREOF = 0xF9;
	inline constexpr int32_t VK_PLAY = 0xFA;
	inline constexpr int32_t VK_ZOOM = 0xFB;
	inline constexpr int32_t VK_NONAME = 0xFC;
	inline constexpr int32_t VK_PA1 = 0xFD;
	inline constexpr int32_t VK_OEM_CLEAR = 0xFE;
//0xFF : reserved
}

//Region Flags
export namespace WinAPI {
#undef ERROR
#undef NULLREGION
#undef SIMPLEREGION
#undef COMPLEXREGION
#undef RGN_ERROR
	inline constexpr int32_t ERROR = 0;
	inline constexpr int32_t NULLREGION = 1;
	inline constexpr int32_t SIMPLEREGION = 2;
	inline constexpr int32_t COMPLEXREGION = 3;
	inline constexpr int32_t RGN_ERROR = ERROR;
}

//CombineRgn() Modes
export namespace WinAPI {
#undef RGN_AND
#undef RGN_OR
#undef RGN_XOR
#undef RGN_DIFF
#undef RGN_COPY
#undef RGN_MIN
#undef RGN_MAX
	inline constexpr int32_t RGN_AND = 1;
	inline constexpr int32_t RGN_OR = 2;
	inline constexpr int32_t RGN_XOR = 3;
	inline constexpr int32_t RGN_DIFF = 4;
	inline constexpr int32_t RGN_COPY = 5;
	inline constexpr int32_t RGN_MIN = RGN_AND;
	inline constexpr int32_t RGN_MAX = RGN_COPY;
}

//GetDeviceCaps() Parameters
export namespace WinAPI {
#undef DRIVERVERSION
#undef TECHNOLOGY
#undef HORZSIZE
#undef VERTSIZE
#undef HORZRES
#undef VERTRES
#undef BITSPIXEL
#undef PLANES
#undef NUMBRUSHES
#undef NUMPENS
#undef NUMMARKERS
#undef NUMFONTS
#undef NUMCOLORS
#undef PDEVICESIZE
#undef CURVECAPS
#undef LINECAPS
#undef POLYGONALCAPS
#undef TEXTCAPS
#undef CLIPCAPS
#undef RASTERCAPS
#undef ASPECTX
#undef ASPECTY
#undef ASPECTXY
#undef LOGPIXELSX
#undef LOGPIXELSY
#undef SIZEPALETTE
#undef NUMRESERVED
#undef COLORRES
#undef PHYSICALWIDTH
#undef PHYSICALHEIGHT
#undef PHYSICALOFFSETX
#undef PHYSICALOFFSETY
#undef SCALINGFACTORX
#undef SCALINGFACTORY
#undef VREFRESH
#undef DESKTOPVERTRES
#undef DESKTOPHORZRES
#undef BLTALIGNMENT
#undef SHADEBLENDCAPS
#undef COLORMGMTCAPS
	inline constexpr int32_t DRIVERVERSION = 0;	// Device driver version
	inline constexpr int32_t TECHNOLOGY = 2;	// Device classification
	inline constexpr int32_t HORZSIZE = 4;	// Horizontal size in millimeters
	inline constexpr int32_t VERTSIZE = 6;	// Vertical size in millimeters
	inline constexpr int32_t HORZRES = 8;	// Horizontal width in pixels
	inline constexpr int32_t VERTRES = 10;	// Vertical height in pixels
	inline constexpr int32_t BITSPIXEL = 12;	// Number of bits per pixel
	inline constexpr int32_t PLANES = 14;	// Number of planes
	inline constexpr int32_t NUMBRUSHES = 16;	// Number of brushes the device has
	inline constexpr int32_t NUMPENS = 18;	// Number of pens the device has
	inline constexpr int32_t NUMMARKERS = 20;	// Number of markers the device has
	inline constexpr int32_t NUMFONTS = 22;	// Number of fonts the device has
	inline constexpr int32_t NUMCOLORS = 24;	// Number of colors the device supports
	inline constexpr int32_t PDEVICESIZE = 26;	// Size required for device descriptor
	inline constexpr int32_t CURVECAPS = 28;	// Curve capabilities
	inline constexpr int32_t LINECAPS = 30;	// Line capabilities
	inline constexpr int32_t POLYGONALCAPS = 32;	// Polygonal capabilities
	inline constexpr int32_t TEXTCAPS = 34;	// Text capabilities
	inline constexpr int32_t CLIPCAPS = 36;	// Clipping capabilities
	inline constexpr int32_t RASTERCAPS = 38;	// Bitblt capabilities
	inline constexpr int32_t ASPECTX = 40;	// Length of the X leg
	inline constexpr int32_t ASPECTY = 42;	// Length of the Y leg
	inline constexpr int32_t ASPECTXY = 44;	// Length of the hypotenuse
	inline constexpr int32_t LOGPIXELSX = 88;	// Logical pixels/inch in X
	inline constexpr int32_t LOGPIXELSY = 90;	// Logical pixels/inch in Y
	inline constexpr int32_t SIZEPALETTE = 104;	// Number of entries in physical palette
	inline constexpr int32_t NUMRESERVED = 106;	// Number of reserved entries in palette
	inline constexpr int32_t COLORRES = 108;	// Actual color resolution
	inline constexpr int32_t PHYSICALWIDTH = 110;	// Physical Width in device units
	inline constexpr int32_t PHYSICALHEIGHT = 111;	// Physical Height in device units
	inline constexpr int32_t PHYSICALOFFSETX = 112;	// Physical Printable Area x margin
	inline constexpr int32_t PHYSICALOFFSETY = 113;	// Physical Printable Area y margin
	inline constexpr int32_t SCALINGFACTORX = 114;	// Scaling factor x
	inline constexpr int32_t SCALINGFACTORY = 115;	// Scaling factor y
	inline constexpr int32_t VREFRESH = 116;	// Current vertical refresh rate of the display device (for displays only) in Hz
	inline constexpr int32_t DESKTOPVERTRES = 117;	// Horizontal width of entire desktop in pixels
	inline constexpr int32_t DESKTOPHORZRES = 118;	// Vertical height of entire desktop in pixels
	inline constexpr int32_t BLTALIGNMENT = 119;	// Preferred blt alignment
	inline constexpr int32_t SHADEBLENDCAPS = 120;	// Shading and blending caps
	inline constexpr int32_t COLORMGMTCAPS = 121;	// Color Management caps

}

//GetSystemMetrics() Parameters
export namespace WinAPI {
#undef SM_CXSCREEN
#undef SM_CYSCREEN
#undef SM_CXVSCROLL
#undef SM_CYHSCROLL
#undef SM_CYCAPTION
#undef SM_CXBORDER
#undef SM_CYBORDER
#undef SM_CXDLGFRAME
#undef SM_CYDLGFRAME
#undef SM_CYVTHUMB
#undef SM_CXHTHUMB
#undef SM_CXICON
#undef SM_CYICON
#undef SM_CXCURSOR
#undef SM_CYCURSOR
#undef SM_CYMENU
#undef SM_CXFULLSCREEN
#undef SM_CYFULLSCREEN
#undef SM_CYKANJIWINDOW
#undef SM_MOUSEPRESENT
#undef SM_CYVSCROLL
#undef SM_CXHSCROLL
#undef SM_DEBUG
#undef SM_SWAPBUTTON
#undef SM_RESERVED1
#undef SM_RESERVED2
#undef SM_RESERVED3
#undef SM_RESERVED4
#undef SM_CXMIN
#undef SM_CYMIN
#undef SM_CXSIZE
#undef SM_CYSIZE
#undef SM_CXFRAME
#undef SM_CYFRAME
#undef SM_CXMINTRACK
#undef SM_CYMINTRACK
#undef SM_CXDOUBLECLK
#undef SM_CYDOUBLECLK
#undef SM_CXICONSPACING
#undef SM_CYICONSPACING
#undef SM_MENUDROPALIGNMENT
#undef SM_PENWINDOWS
#undef SM_DBCSENABLED
#undef SM_CMOUSEBUTTONS
#undef SM_CXFIXEDFRAME
#undef SM_CYFIXEDFRAME
#undef SM_CXSIZEFRAME
#undef SM_CYSIZEFRAME
#undef SM_SECURE
#undef SM_CXEDGE
#undef SM_CYEDGE
#undef SM_CXMINSPACING
#undef SM_CYMINSPACING
#undef SM_CXSMICON
#undef SM_CYSMICON
#undef SM_CYSMCAPTION
#undef SM_CXSMSIZE
#undef SM_CYSMSIZE
#undef SM_CXMENUSIZE
#undef SM_CYMENUSIZE
#undef SM_ARRANGE
#undef SM_CXMINIMIZED
#undef SM_CYMINIMIZED
#undef SM_CXMAXTRACK
#undef SM_CYMAXTRACK
#undef SM_CXMAXIMIZED
#undef SM_CYMAXIMIZED
#undef SM_NETWORK
#undef SM_CLEANBOOT
#undef SM_CXDRAG
#undef SM_CYDRAG
#undef SM_SHOWSOUNDS
#undef SM_CXMENUCHECK
#undef SM_CYMENUCHECK
#undef SM_SLOWMACHINE
#undef SM_MIDEASTENABLED
#undef SM_MOUSEWHEELPRESENT
#undef SM_XVIRTUALSCREEN
#undef SM_YVIRTUALSCREEN
#undef SM_CXVIRTUALSCREEN
#undef SM_CYVIRTUALSCREEN
#undef SM_CMONITORS
#undef SM_SAMEDISPLAYFORMAT
#undef SM_IMMENABLED
#undef SM_CXFOCUSBORDER
#undef SM_CYFOCUSBORDER
#undef SM_TABLETPC
#undef SM_MEDIACENTER
#undef SM_STARTER
#undef SM_SERVERR2
#undef SM_MOUSEHORIZONTALWHEELPRESENT
#undef SM_CXPADDEDBORDER
#undef SM_DIGITIZER
#undef SM_MAXIMUMTOUCHES
#undef SM_CMETRICS
#undef SM_CMETRICS
#undef SM_CMETRICS
#undef SM_CMETRICS
#undef SM_CMETRICS
#undef SM_REMOTESESSION
#undef SM_SHUTTINGDOWN
#undef SM_REMOTECONTROL
#undef SM_CARETBLINKINGENABLED
#undef SM_CONVERTIBLESLATEMODE
#undef SM_SYSTEMDOCKED
	inline constexpr int32_t SM_CXSCREEN = 0;
	inline constexpr int32_t SM_CYSCREEN = 1;
	inline constexpr int32_t SM_CXVSCROLL = 2;
	inline constexpr int32_t SM_CYHSCROLL = 3;
	inline constexpr int32_t SM_CYCAPTION = 4;
	inline constexpr int32_t SM_CXBORDER = 5;
	inline constexpr int32_t SM_CYBORDER = 6;
	inline constexpr int32_t SM_CXDLGFRAME = 7;
	inline constexpr int32_t SM_CYDLGFRAME = 8;
	inline constexpr int32_t SM_CYVTHUMB = 9;
	inline constexpr int32_t SM_CXHTHUMB = 10;
	inline constexpr int32_t SM_CXICON = 11;
	inline constexpr int32_t SM_CYICON = 12;
	inline constexpr int32_t SM_CXCURSOR = 13;
	inline constexpr int32_t SM_CYCURSOR = 14;
	inline constexpr int32_t SM_CYMENU = 15;
	inline constexpr int32_t SM_CXFULLSCREEN = 16;
	inline constexpr int32_t SM_CYFULLSCREEN = 17;
	inline constexpr int32_t SM_CYKANJIWINDOW = 18;
	inline constexpr int32_t SM_MOUSEPRESENT = 19;
	inline constexpr int32_t SM_CYVSCROLL = 20;
	inline constexpr int32_t SM_CXHSCROLL = 21;
	inline constexpr int32_t SM_DEBUG = 22;
	inline constexpr int32_t SM_SWAPBUTTON = 23;
	inline constexpr int32_t SM_RESERVED1 = 24;
	inline constexpr int32_t SM_RESERVED2 = 25;
	inline constexpr int32_t SM_RESERVED3 = 26;
	inline constexpr int32_t SM_RESERVED4 = 27;
	inline constexpr int32_t SM_CXMIN = 28;
	inline constexpr int32_t SM_CYMIN = 29;
	inline constexpr int32_t SM_CXSIZE = 30;
	inline constexpr int32_t SM_CYSIZE = 31;
	inline constexpr int32_t SM_CXFRAME = 32;
	inline constexpr int32_t SM_CYFRAME = 33;
	inline constexpr int32_t SM_CXMINTRACK = 34;
	inline constexpr int32_t SM_CYMINTRACK = 35;
	inline constexpr int32_t SM_CXDOUBLECLK = 36;
	inline constexpr int32_t SM_CYDOUBLECLK = 37;
	inline constexpr int32_t SM_CXICONSPACING = 38;
	inline constexpr int32_t SM_CYICONSPACING = 39;
	inline constexpr int32_t SM_MENUDROPALIGNMENT = 40;
	inline constexpr int32_t SM_PENWINDOWS = 41;
	inline constexpr int32_t SM_DBCSENABLED = 42;
	inline constexpr int32_t SM_CMOUSEBUTTONS = 43;
	inline constexpr int32_t SM_CXFIXEDFRAME = SM_CXDLGFRAME;	//win40 name change
	inline constexpr int32_t SM_CYFIXEDFRAME = SM_CYDLGFRAME;	//win40 name change
	inline constexpr int32_t SM_CXSIZEFRAME = SM_CXFRAME;	//win40 name change
	inline constexpr int32_t SM_CYSIZEFRAME = SM_CYFRAME;	//win40 name change
	inline constexpr int32_t SM_SECURE = 44;
	inline constexpr int32_t SM_CXEDGE = 45;
	inline constexpr int32_t SM_CYEDGE = 46;
	inline constexpr int32_t SM_CXMINSPACING = 47;
	inline constexpr int32_t SM_CYMINSPACING = 48;
	inline constexpr int32_t SM_CXSMICON = 49;
	inline constexpr int32_t SM_CYSMICON = 50;
	inline constexpr int32_t SM_CYSMCAPTION = 51;
	inline constexpr int32_t SM_CXSMSIZE = 52;
	inline constexpr int32_t SM_CYSMSIZE = 53;
	inline constexpr int32_t SM_CXMENUSIZE = 54;
	inline constexpr int32_t SM_CYMENUSIZE = 55;
	inline constexpr int32_t SM_ARRANGE = 56;
	inline constexpr int32_t SM_CXMINIMIZED = 57;
	inline constexpr int32_t SM_CYMINIMIZED = 58;
	inline constexpr int32_t SM_CXMAXTRACK = 59;
	inline constexpr int32_t SM_CYMAXTRACK = 60;
	inline constexpr int32_t SM_CXMAXIMIZED = 61;
	inline constexpr int32_t SM_CYMAXIMIZED = 62;
	inline constexpr int32_t SM_NETWORK = 63;
	inline constexpr int32_t SM_CLEANBOOT = 67;
	inline constexpr int32_t SM_CXDRAG = 68;
	inline constexpr int32_t SM_CYDRAG = 69;
	inline constexpr int32_t SM_SHOWSOUNDS = 70;
	inline constexpr int32_t SM_CXMENUCHECK = 71;	// Use instead of GetMenuCheckMarkDimensions()!
	inline constexpr int32_t SM_CYMENUCHECK = 72;
	inline constexpr int32_t SM_SLOWMACHINE = 73;
	inline constexpr int32_t SM_MIDEASTENABLED = 74;
	inline constexpr int32_t SM_MOUSEWHEELPRESENT = 75;
	inline constexpr int32_t SM_XVIRTUALSCREEN = 76;
	inline constexpr int32_t SM_YVIRTUALSCREEN = 77;
	inline constexpr int32_t SM_CXVIRTUALSCREEN = 78;
	inline constexpr int32_t SM_CYVIRTUALSCREEN = 79;
	inline constexpr int32_t SM_CMONITORS = 80;
	inline constexpr int32_t SM_SAMEDISPLAYFORMAT = 81;
	inline constexpr int32_t SM_IMMENABLED = 82;
	inline constexpr int32_t SM_CXFOCUSBORDER = 83;
	inline constexpr int32_t SM_CYFOCUSBORDER = 84;
	inline constexpr int32_t SM_TABLETPC = 86;
	inline constexpr int32_t SM_MEDIACENTER = 87;
	inline constexpr int32_t SM_STARTER = 88;
	inline constexpr int32_t SM_SERVERR2 = 89;
	inline constexpr int32_t SM_MOUSEHORIZONTALWHEELPRESENT = 91;
	inline constexpr int32_t SM_CXPADDEDBORDER = 92;
	inline constexpr int32_t SM_DIGITIZER = 94;
	inline constexpr int32_t SM_MAXIMUMTOUCHES = 95;
#if (WINVER < 0x0500) && (!defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0400))
	inline constexpr int32_t SM_CMETRICS = 76;
#elif WINVER == 0x500
	inline constexpr int32_t SM_CMETRICS = 83;
#elif WINVER == 0x501
	inline constexpr int32_t SM_CMETRICS = 91;
#elif WINVER == 0x600
	inline constexpr int32_t SM_CMETRICS = 93;
#else
	inline constexpr int32_t SM_CMETRICS = 97;
#endif
	inline constexpr int32_t SM_REMOTESESSION = 0x1000;
	inline constexpr int32_t SM_SHUTTINGDOWN = 0x2000;
	inline constexpr int32_t SM_REMOTECONTROL = 0x2001;
	inline constexpr int32_t SM_CARETBLINKINGENABLED = 0x2002;
	inline constexpr int32_t SM_CONVERTIBLESLATEMODE = 0x2003;
	inline constexpr int32_t SM_SYSTEMDOCKED = 0x2004;

}

//GetSystemMetrics(SM_ARRANGE) return values
export namespace WinAPI {
#undef ARW_BOTTOMLEFT
#undef ARW_BOTTOMRIGHT
#undef ARW_TOPLEFT
#undef ARW_TOPRIGHT
#undef ARW_STARTMASK
#undef ARW_STARTRIGHT
#undef ARW_STARTTOP
#undef ARW_LEFT
#undef ARW_RIGHT
#undef ARW_UP
#undef ARW_DOWN
#undef ARW_HIDE
	inline constexpr int32_t ARW_BOTTOMLEFT = 0x0000L;
	inline constexpr int32_t ARW_BOTTOMRIGHT = 0x0001L;
	inline constexpr int32_t ARW_TOPLEFT = 0x0002L;
	inline constexpr int32_t ARW_TOPRIGHT = 0x0003L;
	inline constexpr int32_t ARW_STARTMASK = 0x0003L;
	inline constexpr int32_t ARW_STARTRIGHT = 0x0001L;
	inline constexpr int32_t ARW_STARTTOP = 0x0002L;
	inline constexpr int32_t ARW_LEFT = 0x0000L;
	inline constexpr int32_t ARW_RIGHT = 0x0000L;
	inline constexpr int32_t ARW_UP = 0x0004L;
	inline constexpr int32_t ARW_DOWN = 0x0004L;
	inline constexpr int32_t ARW_HIDE = 0x0008L;
}

//PeekMessage() Mode
export namespace WinAPI {
#undef PM_NOREMOVE
#undef PM_REMOVE
#undef PM_NOYIELD
#undef PM_QS_INPUT
#undef PM_QS_POSTMESSAGE
#undef PM_QS_PAINT
#undef PM_QS_SENDMESSAGE
	inline constexpr uint32_t PM_NOREMOVE = 0x0000;
	inline constexpr uint32_t PM_REMOVE = 0x0001;
	inline constexpr uint32_t PM_NOYIELD = 0x0002;
	inline constexpr uint32_t PM_QS_INPUT = (QS_INPUT << 16);
	inline constexpr uint32_t PM_QS_POSTMESSAGE = ((QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16);
	inline constexpr uint32_t PM_QS_PAINT = (QS_PAINT << 16);
	inline constexpr uint32_t PM_QS_SENDMESSAGE = (QS_SENDMESSAGE << 16);
}

//ScrollBar Constants
export namespace WinAPI {
#undef SB_HORZ
#undef SB_VERT
#undef SB_CTL
#undef SB_BOTH
#undef SB_LINEUP
#undef SB_LINELEFT
#undef SB_LINEDOWN
#undef SB_LINERIGHT
#undef SB_PAGEUP
#undef SB_PAGELEFT
#undef SB_PAGEDOWN
#undef SB_PAGERIGHT
#undef SB_THUMBPOSITION
#undef SB_THUMBTRACK
#undef SB_TOP
#undef SB_LEFT
#undef SB_BOTTOM
#undef SB_RIGHT
#undef SB_ENDSCROLL
	inline constexpr int32_t SB_HORZ = 0;
	inline constexpr int32_t SB_VERT = 1;
	inline constexpr int32_t SB_CTL = 2;
	inline constexpr int32_t SB_BOTH = 3;
	inline constexpr int32_t SB_LINEUP = 0;
	inline constexpr int32_t SB_LINELEFT = 0;
	inline constexpr int32_t SB_LINEDOWN = 1;
	inline constexpr int32_t SB_LINERIGHT = 1;
	inline constexpr int32_t SB_PAGEUP = 2;
	inline constexpr int32_t SB_PAGELEFT = 2;
	inline constexpr int32_t SB_PAGEDOWN = 3;
	inline constexpr int32_t SB_PAGERIGHT = 3;
	inline constexpr int32_t SB_THUMBPOSITION = 4;
	inline constexpr int32_t SB_THUMBTRACK = 5;
	inline constexpr int32_t SB_TOP = 6;
	inline constexpr int32_t SB_LEFT = 6;
	inline constexpr int32_t SB_BOTTOM = 7;
	inline constexpr int32_t SB_RIGHT = 7;
	inline constexpr int32_t SB_ENDSCROLL = 8;

}

//ScrollBar Info Mask
export namespace WinAPI {
#undef SIF_RANGE
#undef SIF_PAGE
#undef SIF_POS
#undef SIF_DISABLENOSCROLL
#undef SIF_TRACKPOS
#undef SIF_ALL
	inline constexpr uint32_t SIF_RANGE = 0x0001;
	inline constexpr uint32_t SIF_PAGE = 0x0002;
	inline constexpr uint32_t SIF_POS = 0x0004;
	inline constexpr uint32_t SIF_DISABLENOSCROLL = 0x0008;
	inline constexpr uint32_t SIF_TRACKPOS = 0x0010;
	inline constexpr uint32_t SIF_ALL = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
}

//Button Style
export namespace WinAPI {
#undef BS_PUSHBUTTON
#undef BS_DEFPUSHBUTTON
#undef BS_CHECKBOX
#undef BS_AUTOCHECKBOX
#undef BS_RADIOBUTTON
#undef BS_3STATE
#undef BS_AUTO3STATE
#undef BS_GROUPBOX
#undef BS_USERBUTTON
#undef BS_AUTORADIOBUTTON
#undef BS_PUSHBOX
#undef BS_OWNERDRAW
#undef BS_TYPEMASK
#undef BS_LEFTTEXT
#undef BS_TEXT
#undef BS_ICON
#undef BS_BITMAP
#undef BS_LEFT
#undef BS_RIGHT
#undef BS_CENTER
#undef BS_TOP
#undef BS_BOTTOM
#undef BS_VCENTER
#undef BS_PUSHLIKE
#undef BS_MULTILINE
#undef BS_NOTIFY
#undef BS_FLAT
#undef BS_RIGHTBUTTON
	inline constexpr uint32_t BS_PUSHBUTTON = 0x00000000L;
	inline constexpr uint32_t BS_DEFPUSHBUTTON = 0x00000001L;
	inline constexpr uint32_t BS_CHECKBOX = 0x00000002L;
	inline constexpr uint32_t BS_AUTOCHECKBOX = 0x00000003L;
	inline constexpr uint32_t BS_RADIOBUTTON = 0x00000004L;
	inline constexpr uint32_t BS_3STATE = 0x00000005L;
	inline constexpr uint32_t BS_AUTO3STATE = 0x00000006L;
	inline constexpr uint32_t BS_GROUPBOX = 0x00000007L;
	inline constexpr uint32_t BS_USERBUTTON = 0x00000008L;
	inline constexpr uint32_t BS_AUTORADIOBUTTON = 0x00000009L;
	inline constexpr uint32_t BS_PUSHBOX = 0x0000000AL;
	inline constexpr uint32_t BS_OWNERDRAW = 0x0000000BL;
	inline constexpr uint32_t BS_TYPEMASK = 0x0000000FL;
	inline constexpr uint32_t BS_LEFTTEXT = 0x00000020L;
	inline constexpr uint32_t BS_TEXT = 0x00000000L;
	inline constexpr uint32_t BS_ICON = 0x00000040L;
	inline constexpr uint32_t BS_BITMAP = 0x00000080L;
	inline constexpr uint32_t BS_LEFT = 0x00000100L;
	inline constexpr uint32_t BS_RIGHT = 0x00000200L;
	inline constexpr uint32_t BS_CENTER = 0x00000300L;
	inline constexpr uint32_t BS_TOP = 0x00000400L;
	inline constexpr uint32_t BS_BOTTOM = 0x00000800L;
	inline constexpr uint32_t BS_VCENTER = 0x00000C00L;
	inline constexpr uint32_t BS_PUSHLIKE = 0x00001000L;
	inline constexpr uint32_t BS_MULTILINE = 0x00002000L;
	inline constexpr uint32_t BS_NOTIFY = 0x00004000L;
	inline constexpr uint32_t BS_FLAT = 0x00008000L;
	inline constexpr uint32_t BS_RIGHTBUTTON = BS_LEFTTEXT;
}

//Button Notifications
export namespace WinAPI {
#undef BN_CLICKED
#undef BN_PAINT
#undef BN_HILITE
#undef BN_UNHILITE
#undef BN_DISABLE
#undef BN_DOUBLECLICKED
#undef BN_PUSHED
#undef BN_UNPUSHED
#undef BN_DBLCLK
#undef BN_SETFOCUS
#undef BN_KILLFOCUS
	inline constexpr uint16_t BN_CLICKED = 0;
	inline constexpr uint16_t BN_PAINT = 1;
	inline constexpr uint16_t BN_HILITE = 2;
	inline constexpr uint16_t BN_UNHILITE = 3;
	inline constexpr uint16_t BN_DISABLE = 4;
	inline constexpr uint16_t BN_DOUBLECLICKED = 5;
	inline constexpr uint16_t BN_PUSHED = BN_HILITE;
	inline constexpr uint16_t BN_UNPUSHED = BN_UNHILITE;
	inline constexpr uint16_t BN_DBLCLK = BN_DOUBLECLICKED;
	inline constexpr uint16_t BN_SETFOCUS = 6;
	inline constexpr uint16_t BN_KILLFOCUS = 7;
}

//PostMessage(), SendMessage() Special HWND values
export namespace WinAPI {
#undef HWND_BROADCAST
#undef HWND_MESSAGE
	inline constexpr HWND_CONSTANT HWND_BROADCAST = 0xffff;
	inline constexpr HWND_CONSTANT HWND_MESSAGE = -3;
}

//Button Control Message
export namespace WinAPI {
#undef BM_GETCHECK
#undef BM_SETCHECK
#undef BM_GETSTATE
#undef BM_SETSTATE
#undef BM_SETSTYLE
#undef BM_CLICK
#undef BM_GETIMAGE
#undef BM_SETIMAGE
#undef BM_SETDONTCLICK
#undef BST_UNCHECKED
#undef BST_CHECKED
#undef BST_INDETERMINATE
#undef BST_PUSHED
#undef BST_FOCUS
	inline constexpr uint32_t BM_GETCHECK = 0x00F0;
	inline constexpr uint32_t BM_SETCHECK = 0x00F1;
	inline constexpr uint32_t BM_GETSTATE = 0x00F2;
	inline constexpr uint32_t BM_SETSTATE = 0x00F3;
	inline constexpr uint32_t BM_SETSTYLE = 0x00F4;
	inline constexpr uint32_t BM_CLICK = 0x00F5;
	inline constexpr uint32_t BM_GETIMAGE = 0x00F6;
	inline constexpr uint32_t BM_SETIMAGE = 0x00F7;
	inline constexpr uint32_t BM_SETDONTCLICK = 0x00F8;
	inline constexpr uint32_t BST_UNCHECKED = 0x0000;
	inline constexpr uint32_t BST_CHECKED = 0x0001;
	inline constexpr uint32_t BST_INDETERMINATE = 0x0002;
	inline constexpr uint32_t BST_PUSHED = 0x0004;
	inline constexpr uint32_t BST_FOCUS = 0x0008;
}

//GetWindowLong()/SetWindowLong() Constants
export namespace WinAPI {
#undef GWL_WNDPROC
#undef GWL_HINSTANCE
#undef GWL_HWNDPARENT
#undef GWL_STYLE
#undef GWL_EXSTYLE
#undef GWL_USERDATA
#undef GWL_ID
#undef GWLP_WNDPROC
#undef GWLP_HINSTANCE
#undef GWLP_HWNDPARENT
#undef GWLP_USERDATA
#undef GWLP_ID
	inline constexpr int32_t GWL_WNDPROC = -4;
	inline constexpr int32_t GWL_HINSTANCE = -6;
	inline constexpr int32_t GWL_HWNDPARENT = -8;
	inline constexpr int32_t GWL_STYLE = -16;
	inline constexpr int32_t GWL_EXSTYLE = -20;
	inline constexpr int32_t GWL_USERDATA = -21;
	inline constexpr int32_t GWL_ID = -12;
	inline constexpr int32_t GWLP_WNDPROC = -4;
	inline constexpr int32_t GWLP_HINSTANCE = -6;
	inline constexpr int32_t GWLP_HWNDPARENT = -8;
	inline constexpr int32_t GWLP_USERDATA = -21;
	inline constexpr int32_t GWLP_ID = -12;
}

//System Color Constants
export namespace WinAPI {
#undef CTLCOLOR_MSGBOX
#undef CTLCOLOR_EDIT
#undef CTLCOLOR_LISTBOX
#undef CTLCOLOR_BTN
#undef CTLCOLOR_DLG
#undef CTLCOLOR_SCROLLBAR
#undef CTLCOLOR_STATIC
#undef CTLCOLOR_MAX
#undef COLOR_SCROLLBAR
#undef COLOR_BACKGROUND
#undef COLOR_ACTIVECAPTION
#undef COLOR_INACTIVECAPTION
#undef COLOR_MENU
#undef COLOR_WINDOW
#undef COLOR_WINDOWFRAME
#undef COLOR_MENUTEXT
#undef COLOR_WINDOWTEXT
#undef COLOR_CAPTIONTEXT
#undef COLOR_ACTIVEBORDER
#undef COLOR_INACTIVEBORDER
#undef COLOR_APPWORKSPACE
#undef COLOR_HIGHLIGHT
#undef COLOR_HIGHLIGHTTEXT
#undef COLOR_BTNFACE
#undef COLOR_BTNSHADOW
#undef COLOR_GRAYTEXT
#undef COLOR_BTNTEXT
#undef COLOR_INACTIVECAPTIONTEXT
#undef COLOR_BTNHIGHLIGHT
#undef COLOR_3DDKSHADOW
#undef COLOR_3DLIGHT
#undef COLOR_INFOTEXT
#undef COLOR_INFOBK
#undef COLOR_HOTLIGHT
#undef COLOR_GRADIENTACTIVECAPTION
#undef COLOR_GRADIENTINACTIVECAPTION
#undef COLOR_MENUHILIGHT
#undef COLOR_MENUBAR
#undef COLOR_DESKTOP
#undef COLOR_3DFACE
#undef COLOR_3DSHADOW
#undef COLOR_3DHIGHLIGHT
#undef COLOR_3DHILIGHT
#undef COLOR_BTNHILIGHT
	//Control Color
	inline constexpr int32_t CTLCOLOR_MSGBOX = 0;
	inline constexpr int32_t CTLCOLOR_EDIT = 1;
	inline constexpr int32_t CTLCOLOR_LISTBOX = 2;
	inline constexpr int32_t CTLCOLOR_BTN = 3;
	inline constexpr int32_t CTLCOLOR_DLG = 4;
	inline constexpr int32_t CTLCOLOR_SCROLLBAR = 5;
	inline constexpr int32_t CTLCOLOR_STATIC = 6;
	inline constexpr int32_t CTLCOLOR_MAX = 7;
	//System Color
	inline constexpr int32_t COLOR_SCROLLBAR = 0;
	inline constexpr int32_t COLOR_BACKGROUND = 1;
	inline constexpr int32_t COLOR_ACTIVECAPTION = 2;
	inline constexpr int32_t COLOR_INACTIVECAPTION = 3;
	inline constexpr int32_t COLOR_MENU = 4;
	inline constexpr int32_t COLOR_WINDOW = 5;
	inline constexpr int32_t COLOR_WINDOWFRAME = 6;
	inline constexpr int32_t COLOR_MENUTEXT = 7;
	inline constexpr int32_t COLOR_WINDOWTEXT = 8;
	inline constexpr int32_t COLOR_CAPTIONTEXT = 9;
	inline constexpr int32_t COLOR_ACTIVEBORDER = 10;
	inline constexpr int32_t COLOR_INACTIVEBORDER = 11;
	inline constexpr int32_t COLOR_APPWORKSPACE = 12;
	inline constexpr int32_t COLOR_HIGHLIGHT = 13;
	inline constexpr int32_t COLOR_HIGHLIGHTTEXT = 14;
	inline constexpr int32_t COLOR_BTNFACE = 15;
	inline constexpr int32_t COLOR_BTNSHADOW = 16;
	inline constexpr int32_t COLOR_GRAYTEXT = 17;
	inline constexpr int32_t COLOR_BTNTEXT = 18;
	inline constexpr int32_t COLOR_INACTIVECAPTIONTEXT = 19;
	inline constexpr int32_t COLOR_BTNHIGHLIGHT = 20;
	inline constexpr int32_t COLOR_3DDKSHADOW = 21;
	inline constexpr int32_t COLOR_3DLIGHT = 22;
	inline constexpr int32_t COLOR_INFOTEXT = 23;
	inline constexpr int32_t COLOR_INFOBK = 24;
	inline constexpr int32_t COLOR_HOTLIGHT = 26;
	inline constexpr int32_t COLOR_GRADIENTACTIVECAPTION = 27;
	inline constexpr int32_t COLOR_GRADIENTINACTIVECAPTION = 28;
	inline constexpr int32_t COLOR_MENUHILIGHT = 29;
	inline constexpr int32_t COLOR_MENUBAR = 30;
	inline constexpr int32_t COLOR_DESKTOP = COLOR_BACKGROUND;
	inline constexpr int32_t COLOR_3DFACE = COLOR_BTNFACE;
	inline constexpr int32_t COLOR_3DSHADOW = COLOR_BTNSHADOW;
	inline constexpr int32_t COLOR_3DHIGHLIGHT = COLOR_BTNHIGHLIGHT;
	inline constexpr int32_t COLOR_3DHILIGHT = COLOR_BTNHIGHLIGHT;
	inline constexpr int32_t COLOR_BTNHILIGHT = COLOR_BTNHIGHLIGHT;
}

//Owner draw Constants
export namespace WinAPI {
#undef ODT_MENU
#undef ODT_LISTBOX
#undef ODT_COMBOBOX
#undef ODT_BUTTON
#undef ODT_STATIC
#undef ODA_DRAWENTIRE
#undef ODA_SELECT
#undef ODA_FOCUS
#undef ODS_SELECTED
#undef ODS_GRAYED
#undef ODS_DISABLED
#undef ODS_CHECKED
#undef ODS_FOCUS
#undef ODS_DEFAULT
#undef ODS_COMBOBOXEDIT
#undef ODS_HOTLIGHT
#undef ODS_INACTIVE
#undef ODS_NOACCEL
#undef ODS_NOFOCUSRECT
//Owner draw control types
	inline constexpr uint32_t ODT_MENU = 1;
	inline constexpr uint32_t ODT_LISTBOX = 2;
	inline constexpr uint32_t ODT_COMBOBOX = 3;
	inline constexpr uint32_t ODT_BUTTON = 4;
	inline constexpr uint32_t ODT_STATIC = 5;
//Owner draw actions
	inline constexpr uint32_t ODA_DRAWENTIRE = 0x0001;
	inline constexpr uint32_t ODA_SELECT = 0x0002;
	inline constexpr uint32_t ODA_FOCUS = 0x0004;
//Owner draw state
	inline constexpr uint32_t ODS_SELECTED = 0x0001;
	inline constexpr uint32_t ODS_GRAYED = 0x0002;
	inline constexpr uint32_t ODS_DISABLED = 0x0004;
	inline constexpr uint32_t ODS_CHECKED = 0x0008;
	inline constexpr uint32_t ODS_FOCUS = 0x0010;
	inline constexpr uint32_t ODS_DEFAULT = 0x0020;
	inline constexpr uint32_t ODS_COMBOBOXEDIT = 0x1000;
	inline constexpr uint32_t ODS_HOTLIGHT = 0x0040;
	inline constexpr uint32_t ODS_INACTIVE = 0x0080;
	inline constexpr uint32_t ODS_NOACCEL = 0x0100;
	inline constexpr uint32_t ODS_NOFOCUSRECT = 0x0200;

}

//Static Control Constants
export namespace WinAPI {
#undef SS_LEFT
#undef SS_CENTER
#undef SS_RIGHT
#undef SS_ICON
#undef SS_BLACKRECT
#undef SS_GRAYRECT
#undef SS_WHITERECT
#undef SS_BLACKFRAME
#undef SS_GRAYFRAME
#undef SS_WHITEFRAME
#undef SS_USERITEM
#undef SS_SIMPLE
#undef SS_LEFTNOWORDWRAP
#undef SS_OWNERDRAW
#undef SS_BITMAP
#undef SS_ENHMETAFILE
#undef SS_ETCHEDHORZ
#undef SS_ETCHEDVERT
#undef SS_ETCHEDFRAME
#undef SS_TYPEMASK
#undef SS_REALSIZECONTROL
#undef SS_NOPREFIX
#undef SS_NOTIFY
#undef SS_CENTERIMAGE
#undef SS_RIGHTJUST
#undef SS_REALSIZEIMAGE
#undef SS_SUNKEN
#undef SS_EDITCONTROL
#undef SS_ENDELLIPSIS
#undef SS_PATHELLIPSIS
#undef SS_WORDELLIPSIS
#undef SS_ELLIPSISMASK
	inline constexpr uint32_t SS_LEFT = 0x00000000L;
	inline constexpr uint32_t SS_CENTER = 0x00000001L;
	inline constexpr uint32_t SS_RIGHT = 0x00000002L;
	inline constexpr uint32_t SS_ICON = 0x00000003L;
	inline constexpr uint32_t SS_BLACKRECT = 0x00000004L;
	inline constexpr uint32_t SS_GRAYRECT = 0x00000005L;
	inline constexpr uint32_t SS_WHITERECT = 0x00000006L;
	inline constexpr uint32_t SS_BLACKFRAME = 0x00000007L;
	inline constexpr uint32_t SS_GRAYFRAME = 0x00000008L;
	inline constexpr uint32_t SS_WHITEFRAME = 0x00000009L;
	inline constexpr uint32_t SS_USERITEM = 0x0000000AL;
	inline constexpr uint32_t SS_SIMPLE = 0x0000000BL;
	inline constexpr uint32_t SS_LEFTNOWORDWRAP = 0x0000000CL;
	inline constexpr uint32_t SS_OWNERDRAW = 0x0000000DL;
	inline constexpr uint32_t SS_BITMAP = 0x0000000EL;
	inline constexpr uint32_t SS_ENHMETAFILE = 0x0000000FL;
	inline constexpr uint32_t SS_ETCHEDHORZ = 0x00000010L;
	inline constexpr uint32_t SS_ETCHEDVERT = 0x00000011L;
	inline constexpr uint32_t SS_ETCHEDFRAME = 0x00000012L;
	inline constexpr uint32_t SS_TYPEMASK = 0x0000001FL;
	inline constexpr uint32_t SS_REALSIZECONTROL = 0x00000040L;
	inline constexpr uint32_t SS_NOPREFIX = 0x00000080L;	//Don't do "&" character translation
	inline constexpr uint32_t SS_NOTIFY = 0x00000100L;
	inline constexpr uint32_t SS_CENTERIMAGE = 0x00000200L;
	inline constexpr uint32_t SS_RIGHTJUST = 0x00000400L;
	inline constexpr uint32_t SS_REALSIZEIMAGE = 0x00000800L;
	inline constexpr uint32_t SS_SUNKEN = 0x00001000L;
	inline constexpr uint32_t SS_EDITCONTROL = 0x00002000L;
	inline constexpr uint32_t SS_ENDELLIPSIS = 0x00004000L;
	inline constexpr uint32_t SS_PATHELLIPSIS = 0x00008000L;
	inline constexpr uint32_t SS_WORDELLIPSIS = 0x0000C000L;
	inline constexpr uint32_t SS_ELLIPSISMASK = 0x0000C000L;
}

//Static Control Messages/Notifications
export namespace WinAPI {
#undef STM_SETICON
#undef STM_GETICON
#undef STM_SETIMAGE
#undef STM_GETIMAGE
#undef STM_MSGMAX
#undef STN_CLICKED
#undef STN_DBLCLK
#undef STN_ENABLE
#undef STN_DISABLE
	inline constexpr uint32_t STM_SETICON = 0x0170;
	inline constexpr uint32_t STM_GETICON = 0x0171;
	inline constexpr uint32_t STM_SETIMAGE = 0x0172;
	inline constexpr uint32_t STM_GETIMAGE = 0x0173;
	inline constexpr uint32_t STM_MSGMAX = 0x0174;
	inline constexpr uint32_t STN_CLICKED = 0;
	inline constexpr uint32_t STN_DBLCLK = 1;
	inline constexpr uint32_t STN_ENABLE = 2;
	inline constexpr uint32_t STN_DISABLE = 3;
}

//GetClassLong()/SetClassLong() Constants
export namespace WinAPI {
#undef GCL_MENUNAME
#undef GCL_HBRBACKGROUND
#undef GCL_HCURSOR
#undef GCL_HICON
#undef GCL_HMODULE
#undef GCL_CBWNDEXTRA
#undef GCL_CBCLSEXTRA
#undef GCL_WNDPROC
#undef GCL_STYLE
#undef GCW_ATOM
#undef GCL_HICONSM
#undef GCLP_MENUNAME
#undef GCLP_HBRBACKGROUND
#undef GCLP_HCURSOR
#undef GCLP_HICON
#undef GCLP_HMODULE
#undef GCLP_WNDPROC
#undef GCLP_HICONSM
	inline constexpr int32_t GCL_MENUNAME = -8;
	inline constexpr int32_t GCL_HBRBACKGROUND = -10;
	inline constexpr int32_t GCL_HCURSOR = -12;
	inline constexpr int32_t GCL_HICON = -14;
	inline constexpr int32_t GCL_HMODULE = -16;
	inline constexpr int32_t GCL_CBWNDEXTRA = -18;
	inline constexpr int32_t GCL_CBCLSEXTRA = -20;
	inline constexpr int32_t GCL_WNDPROC = -24;
	inline constexpr int32_t GCL_STYLE = -26;
	inline constexpr int32_t GCW_ATOM = -32;
	inline constexpr int32_t GCL_HICONSM = -34;
	inline constexpr int32_t GCLP_MENUNAME = -8;
	inline constexpr int32_t GCLP_HBRBACKGROUND = -10;
	inline constexpr int32_t GCLP_HCURSOR = -12;
	inline constexpr int32_t GCLP_HICON = -14;
	inline constexpr int32_t GCLP_HMODULE = -16;
	inline constexpr int32_t GCLP_WNDPROC = -24;
	inline constexpr int32_t GCLP_HICONSM = -34;
}

//Edit Control Styles/Notifications/Messages
export namespace WinAPI {
#undef ES_LEFT
#undef ES_CENTER
#undef ES_RIGHT
#undef ES_MULTILINE
#undef ES_UPPERCASE
#undef ES_LOWERCASE
#undef ES_PASSWORD
#undef ES_AUTOVSCROLL
#undef ES_AUTOHSCROLL
#undef ES_NOHIDESEL
#undef ES_OEMCONVERT
#undef ES_READONLY
#undef ES_WANTRETURN
#undef ES_NUMBER
#undef EN_SETFOCUS
#undef EN_KILLFOCUS
#undef EN_CHANGE
#undef EN_UPDATE
#undef EN_ERRSPACE
#undef EN_MAXTEXT
#undef EN_HSCROLL
#undef EN_VSCROLL
#undef EN_ALIGN_LTR_EC
#undef EN_ALIGN_RTL_EC
#undef EN_BEFORE_PASTE
#undef EN_AFTER_PASTE
#undef EM_GETSEL
#undef EM_SETSEL
#undef EM_GETRECT
#undef EM_SETRECT
#undef EM_SETRECTNP
#undef EM_SCROLL
#undef EM_LINESCROLL
#undef EM_SCROLLCARET
#undef EM_GETMODIFY
#undef EM_SETMODIFY
#undef EM_GETLINECOUNT
#undef EM_LINEINDEX
#undef EM_SETHANDLE
#undef EM_GETHANDLE
#undef EM_GETTHUMB
#undef EM_LINELENGTH
#undef EM_REPLACESEL
#undef EM_GETLINE
#undef EM_LIMITTEXT
#undef EM_CANUNDO
#undef EM_UNDO
#undef EM_FMTLINES
#undef EM_LINEFROMCHAR
#undef EM_SETTABSTOPS
#undef EM_SETPASSWORDCHAR
#undef EM_EMPTYUNDOBUFFER
#undef EM_GETFIRSTVISIBLELINE
#undef EM_SETREADONLY
#undef EM_SETWORDBREAKPROC
#undef EM_GETWORDBREAKPROC
#undef EM_GETPASSWORDCHAR
#undef EM_SETMARGINS
#undef EM_GETMARGINS
#undef EM_SETLIMITTEXT
#undef EM_GETLIMITTEXT
#undef EM_POSFROMCHAR
#undef EM_CHARFROMPOS
#undef EM_SETIMESTATUS
#undef EM_GETIMESTATUS
#undef EM_ENABLEFEATURE
	//Styles
	inline constexpr uint32_t ES_LEFT = 0x0000L;
	inline constexpr uint32_t ES_CENTER = 0x0001L;
	inline constexpr uint32_t ES_RIGHT = 0x0002L;
	inline constexpr uint32_t ES_MULTILINE = 0x0004L;
	inline constexpr uint32_t ES_UPPERCASE = 0x0008L;
	inline constexpr uint32_t ES_LOWERCASE = 0x0010L;
	inline constexpr uint32_t ES_PASSWORD = 0x0020L;
	inline constexpr uint32_t ES_AUTOVSCROLL = 0x0040L;
	inline constexpr uint32_t ES_AUTOHSCROLL = 0x0080L;
	inline constexpr uint32_t ES_NOHIDESEL = 0x0100L;
	inline constexpr uint32_t ES_OEMCONVERT = 0x0400L;
	inline constexpr uint32_t ES_READONLY = 0x0800L;
	inline constexpr uint32_t ES_WANTRETURN = 0x1000L;
	inline constexpr uint32_t ES_NUMBER = 0x2000L;
	//Notifications
	inline constexpr uint32_t EN_SETFOCUS = 0x0100;
	inline constexpr uint32_t EN_KILLFOCUS = 0x0200;
	inline constexpr uint32_t EN_CHANGE = 0x0300;
	inline constexpr uint32_t EN_UPDATE = 0x0400;
	inline constexpr uint32_t EN_ERRSPACE = 0x0500;
	inline constexpr uint32_t EN_MAXTEXT = 0x0501;
	inline constexpr uint32_t EN_HSCROLL = 0x0601;
	inline constexpr uint32_t EN_VSCROLL = 0x0602;
	inline constexpr uint32_t EN_ALIGN_LTR_EC = 0x0700;
	inline constexpr uint32_t EN_ALIGN_RTL_EC = 0x0701;
	inline constexpr uint32_t EN_BEFORE_PASTE = 0x0800;
	inline constexpr uint32_t EN_AFTER_PASTE = 0x0801;
	//Messages
	inline constexpr uint32_t EM_GETSEL = 0x00B0;
	inline constexpr uint32_t EM_SETSEL = 0x00B1;
	inline constexpr uint32_t EM_GETRECT = 0x00B2;
	inline constexpr uint32_t EM_SETRECT = 0x00B3;
	inline constexpr uint32_t EM_SETRECTNP = 0x00B4;
	inline constexpr uint32_t EM_SCROLL = 0x00B5;
	inline constexpr uint32_t EM_LINESCROLL = 0x00B6;
	inline constexpr uint32_t EM_SCROLLCARET = 0x00B7;
	inline constexpr uint32_t EM_GETMODIFY = 0x00B8;
	inline constexpr uint32_t EM_SETMODIFY = 0x00B9;
	inline constexpr uint32_t EM_GETLINECOUNT = 0x00BA;
	inline constexpr uint32_t EM_LINEINDEX = 0x00BB;
	inline constexpr uint32_t EM_SETHANDLE = 0x00BC;
	inline constexpr uint32_t EM_GETHANDLE = 0x00BD;
	inline constexpr uint32_t EM_GETTHUMB = 0x00BE;
	inline constexpr uint32_t EM_LINELENGTH = 0x00C1;
	inline constexpr uint32_t EM_REPLACESEL = 0x00C2;
	inline constexpr uint32_t EM_GETLINE = 0x00C4;
	inline constexpr uint32_t EM_LIMITTEXT = 0x00C5;
	inline constexpr uint32_t EM_CANUNDO = 0x00C6;
	inline constexpr uint32_t EM_UNDO = 0x00C7;
	inline constexpr uint32_t EM_FMTLINES = 0x00C8;
	inline constexpr uint32_t EM_LINEFROMCHAR = 0x00C9;
	inline constexpr uint32_t EM_SETTABSTOPS = 0x00CB;
	inline constexpr uint32_t EM_SETPASSWORDCHAR = 0x00CC;
	inline constexpr uint32_t EM_EMPTYUNDOBUFFER = 0x00CD;
	inline constexpr uint32_t EM_GETFIRSTVISIBLELINE = 0x00CE;
	inline constexpr uint32_t EM_SETREADONLY = 0x00CF;
	inline constexpr uint32_t EM_SETWORDBREAKPROC = 0x00D0;
	inline constexpr uint32_t EM_GETWORDBREAKPROC = 0x00D1;
	inline constexpr uint32_t EM_GETPASSWORDCHAR = 0x00D2;
	inline constexpr uint32_t EM_SETMARGINS = 0x00D3;
	inline constexpr uint32_t EM_GETMARGINS = 0x00D4;
	inline constexpr uint32_t EM_SETLIMITTEXT = EM_LIMITTEXT;
	inline constexpr uint32_t EM_GETLIMITTEXT = 0x00D5;
	inline constexpr uint32_t EM_POSFROMCHAR = 0x00D6;
	inline constexpr uint32_t EM_CHARFROMPOS = 0x00D7;
	inline constexpr uint32_t EM_SETIMESTATUS = 0x00D8;
	inline constexpr uint32_t EM_GETIMESTATUS = 0x00D9;
	inline constexpr uint32_t EM_ENABLEFEATURE = 0x00DA;
}

//Edit Control Message Parameters
export namespace WinAPI {
#undef EC_LEFTMARGIN
#undef EC_RIGHTMARGIN
#undef EC_USEFONTINFO
#undef EMSIS_COMPOSITIONSTRING
#undef EIMES_GETCOMPSTRATONCE
#undef EIMES_CANCELCOMPSTRINFOCUS
#undef EIMES_COMPLETECOMPSTRKILLFOCUS
	//Edit control EM_SETMARGIN parameters
	inline constexpr uint16_t EC_LEFTMARGIN = 0x0001;
	inline constexpr uint16_t EC_RIGHTMARGIN = 0x0002;
	inline constexpr uint16_t EC_USEFONTINFO = 0xffff;
	//wParam of EM_GET/SETIMESTATUS
	inline constexpr uint16_t EMSIS_COMPOSITIONSTRING = 0x0001;
	//lParam for EMSIS_COMPOSITIONSTRING
	inline constexpr uint16_t EIMES_GETCOMPSTRATONCE = 0x0001;
	inline constexpr uint16_t EIMES_CANCELCOMPSTRINFOCUS = 0x0002;
	inline constexpr uint16_t EIMES_COMPLETECOMPSTRKILLFOCUS = 0x0004;
}

//ListBox Messages/Notifications/Styles
export namespace WinAPI {
#undef LB_OKAY
#undef LB_ERR
#undef LB_ERRSPACE
#undef LBN_ERRSPACE
#undef LBN_SELCHANGE
#undef LBN_DBLCLK
#undef LBN_SELCANCEL
#undef LBN_SETFOCUS
#undef LBN_KILLFOCUS
#undef LB_ADDSTRING
#undef LB_INSERTSTRING
#undef LB_DELETESTRING
#undef LB_SELITEMRANGEEX
#undef LB_RESETCONTENT
#undef LB_SETSEL
#undef LB_SETCURSEL
#undef LB_GETSEL
#undef LB_GETCURSEL
#undef LB_GETTEXT
#undef LB_GETTEXTLEN
#undef LB_GETCOUNT
#undef LB_SELECTSTRING
#undef LB_DIR
#undef LB_GETTOPINDEX
#undef LB_FINDSTRING
#undef LB_GETSELCOUNT
#undef LB_GETSELITEMS
#undef LB_SETTABSTOPS
#undef LB_GETHORIZONTALEXTENT
#undef LB_SETHORIZONTALEXTENT
#undef LB_SETCOLUMNWIDTH
#undef LB_ADDFILE
#undef LB_SETTOPINDEX
#undef LB_GETITEMRECT
#undef LB_GETITEMDATA
#undef LB_SETITEMDATA
#undef LB_SELITEMRANGE
#undef LB_SETANCHORINDEX
#undef LB_GETANCHORINDEX
#undef LB_SETCARETINDEX
#undef LB_GETCARETINDEX
#undef LB_SETITEMHEIGHT
#undef LB_GETITEMHEIGHT
#undef LB_FINDSTRINGEXACT
#undef LB_SETLOCALE
#undef LB_GETLOCALE
#undef LB_SETCOUNT
#undef LB_INITSTORAGE
#undef LB_ITEMFROMPOINT
#undef LB_MULTIPLEADDSTRING
#undef LB_GETLISTBOXINFO
#undef LB_MSGMAX
#undef LBS_NOTIFY
#undef LBS_SORT
#undef LBS_NOREDRAW
#undef LBS_MULTIPLESEL
#undef LBS_OWNERDRAWFIXED
#undef LBS_OWNERDRAWVARIABLE
#undef LBS_HASSTRINGS
#undef LBS_USETABSTOPS
#undef LBS_NOINTEGRALHEIGHT
#undef LBS_MULTICOLUMN
#undef LBS_WANTKEYBOARDINPUT
#undef LBS_EXTENDEDSEL
#undef LBS_DISABLENOSCROLL
#undef LBS_NODATA
#undef LBS_NOSEL
#undef LBS_COMBOBOX
#undef LBS_STANDARD
//Listbox Return Values
	inline constexpr int32_t LB_OKAY = 0;
	inline constexpr int32_t LB_ERR = (-1);
	inline constexpr int32_t LB_ERRSPACE = (-2);
//Listbox Notification Codes
	inline constexpr int32_t LBN_ERRSPACE = (-2);
	inline constexpr int32_t LBN_SELCHANGE = 1;
	inline constexpr int32_t LBN_DBLCLK = 2;
	inline constexpr int32_t LBN_SELCANCEL = 3;
	inline constexpr int32_t LBN_SETFOCUS = 4;
	inline constexpr int32_t LBN_KILLFOCUS = 5;
//Listbox messages
	inline constexpr int32_t LB_ADDSTRING = 0x0180;
	inline constexpr int32_t LB_INSERTSTRING = 0x0181;
	inline constexpr int32_t LB_DELETESTRING = 0x0182;
	inline constexpr int32_t LB_SELITEMRANGEEX = 0x0183;
	inline constexpr int32_t LB_RESETCONTENT = 0x0184;
	inline constexpr int32_t LB_SETSEL = 0x0185;
	inline constexpr int32_t LB_SETCURSEL = 0x0186;
	inline constexpr int32_t LB_GETSEL = 0x0187;
	inline constexpr int32_t LB_GETCURSEL = 0x0188;
	inline constexpr int32_t LB_GETTEXT = 0x0189;
	inline constexpr int32_t LB_GETTEXTLEN = 0x018A;
	inline constexpr int32_t LB_GETCOUNT = 0x018B;
	inline constexpr int32_t LB_SELECTSTRING = 0x018C;
	inline constexpr int32_t LB_DIR = 0x018D;
	inline constexpr int32_t LB_GETTOPINDEX = 0x018E;
	inline constexpr int32_t LB_FINDSTRING = 0x018F;
	inline constexpr int32_t LB_GETSELCOUNT = 0x0190;
	inline constexpr int32_t LB_GETSELITEMS = 0x0191;
	inline constexpr int32_t LB_SETTABSTOPS = 0x0192;
	inline constexpr int32_t LB_GETHORIZONTALEXTENT = 0x0193;
	inline constexpr int32_t LB_SETHORIZONTALEXTENT = 0x0194;
	inline constexpr int32_t LB_SETCOLUMNWIDTH = 0x0195;
	inline constexpr int32_t LB_ADDFILE = 0x0196;
	inline constexpr int32_t LB_SETTOPINDEX = 0x0197;
	inline constexpr int32_t LB_GETITEMRECT = 0x0198;
	inline constexpr int32_t LB_GETITEMDATA = 0x0199;
	inline constexpr int32_t LB_SETITEMDATA = 0x019A;
	inline constexpr int32_t LB_SELITEMRANGE = 0x019B;
	inline constexpr int32_t LB_SETANCHORINDEX = 0x019C;
	inline constexpr int32_t LB_GETANCHORINDEX = 0x019D;
	inline constexpr int32_t LB_SETCARETINDEX = 0x019E;
	inline constexpr int32_t LB_GETCARETINDEX = 0x019F;
	inline constexpr int32_t LB_SETITEMHEIGHT = 0x01A0;
	inline constexpr int32_t LB_GETITEMHEIGHT = 0x01A1;
	inline constexpr int32_t LB_FINDSTRINGEXACT = 0x01A2;
	inline constexpr int32_t LB_SETLOCALE = 0x01A5;
	inline constexpr int32_t LB_GETLOCALE = 0x01A6;
	inline constexpr int32_t LB_SETCOUNT = 0x01A7;
	inline constexpr int32_t LB_INITSTORAGE = 0x01A8;
	inline constexpr int32_t LB_ITEMFROMPOINT = 0x01A9;
	inline constexpr int32_t LB_MULTIPLEADDSTRING = 0x01B1;
	inline constexpr int32_t LB_GETLISTBOXINFO = 0x01B2;
#if(_WIN32_WINNT >= 0x0501)
	inline constexpr int32_t LB_MSGMAX = 0x01B3;
#elif defined(_WIN32_WCE) && (_WIN32_WCE >= 0x0400)
	inline constexpr int32_t LB_MSGMAX = 0x01B1;
#elif(WINVER >= 0x0400)
	inline constexpr int32_t LB_MSGMAX = 0x01B0;
#else
	inline constexpr int32_t LB_MSGMAX = 0x01A8;
#endif
//Listbox Styles
	inline constexpr int32_t LBS_NOTIFY = 0x0001L;
	inline constexpr int32_t LBS_SORT = 0x0002L;
	inline constexpr int32_t LBS_NOREDRAW = 0x0004L;
	inline constexpr int32_t LBS_MULTIPLESEL = 0x0008L;
	inline constexpr int32_t LBS_OWNERDRAWFIXED = 0x0010L;
	inline constexpr int32_t LBS_OWNERDRAWVARIABLE = 0x0020L;
	inline constexpr int32_t LBS_HASSTRINGS = 0x0040L;
	inline constexpr int32_t LBS_USETABSTOPS = 0x0080L;
	inline constexpr int32_t LBS_NOINTEGRALHEIGHT = 0x0100L;
	inline constexpr int32_t LBS_MULTICOLUMN = 0x0200L;
	inline constexpr int32_t LBS_WANTKEYBOARDINPUT = 0x0400L;
	inline constexpr int32_t LBS_EXTENDEDSEL = 0x0800L;
	inline constexpr int32_t LBS_DISABLENOSCROLL = 0x1000L;
	inline constexpr int32_t LBS_NODATA = 0x2000L;
	inline constexpr int32_t LBS_NOSEL = 0x4000L;
	inline constexpr int32_t LBS_COMBOBOX = 0x8000L;
	inline constexpr int32_t LBS_STANDARD = LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER;
}

//DlgDirList, DlgDirComboBox flags
export namespace WinAPI {
#undef DDL_READWRITE
#undef DDL_READONLY
#undef DDL_HIDDEN
#undef DDL_SYSTEM
#undef DDL_DIRECTORY
#undef DDL_ARCHIVE
#undef DDL_POSTMSGS
#undef DDL_DRIVES
#undef DDL_EXCLUSIVE
	inline constexpr uint32_t DDL_READWRITE = 0x0000;
	inline constexpr uint32_t DDL_READONLY = 0x0001;
	inline constexpr uint32_t DDL_HIDDEN = 0x0002;
	inline constexpr uint32_t DDL_SYSTEM = 0x0004;
	inline constexpr uint32_t DDL_DIRECTORY = 0x0010;
	inline constexpr uint32_t DDL_ARCHIVE = 0x0020;
	inline constexpr uint32_t DDL_POSTMSGS = 0x2000;
	inline constexpr uint32_t DDL_DRIVES = 0x4000;
	inline constexpr uint32_t DDL_EXCLUSIVE = 0x8000;
}

//ComboBox Messages/Notifications/Styles
export namespace WinAPI {
#undef CB_OKAY
#undef CB_ERR
#undef CB_ERRSPACE
#undef CBN_ERRSPACE
#undef CBN_SELCHANGE
#undef CBN_DBLCLK
#undef CBN_SETFOCUS
#undef CBN_KILLFOCUS
#undef CBN_EDITCHANGE
#undef CBN_EDITUPDATE
#undef CBN_DROPDOWN
#undef CBN_CLOSEUP
#undef CBN_SELENDOK
#undef CBN_SELENDCANCEL
#undef CBS_SIMPLE
#undef CBS_DROPDOWN
#undef CBS_DROPDOWNLIST
#undef CBS_OWNERDRAWFIXED
#undef CBS_OWNERDRAWVARIABLE
#undef CBS_AUTOHSCROLL
#undef CBS_OEMCONVERT
#undef CBS_SORT
#undef CBS_HASSTRINGS
#undef CBS_NOINTEGRALHEIGHT
#undef CBS_DISABLENOSCROLL
#undef CBS_UPPERCASE
#undef CBS_LOWERCASE
#undef CB_GETEDITSEL
#undef CB_LIMITTEXT
#undef CB_SETEDITSEL
#undef CB_ADDSTRING
#undef CB_DELETESTRING
#undef CB_DIR
#undef CB_GETCOUNT
#undef CB_GETCURSEL
#undef CB_GETLBTEXT
#undef CB_GETLBTEXTLEN
#undef CB_INSERTSTRING
#undef CB_RESETCONTENT
#undef CB_FINDSTRING
#undef CB_SELECTSTRING
#undef CB_SETCURSEL
#undef CB_SHOWDROPDOWN
#undef CB_GETITEMDATA
#undef CB_SETITEMDATA
#undef CB_GETDROPPEDCONTROLRECT
#undef CB_SETITEMHEIGHT
#undef CB_GETITEMHEIGHT
#undef CB_SETEXTENDEDUI
#undef CB_GETEXTENDEDUI
#undef CB_GETDROPPEDSTATE
#undef CB_FINDSTRINGEXACT
#undef CB_SETLOCALE
#undef CB_GETLOCALE
#undef CB_GETTOPINDEX
#undef CB_SETTOPINDEX
#undef CB_GETHORIZONTALEXTENT
#undef CB_SETHORIZONTALEXTENT
#undef CB_GETDROPPEDWIDTH
#undef CB_SETDROPPEDWIDTH
#undef CB_INITSTORAGE
#undef CB_MULTIPLEADDSTRING
#undef CB_GETCOMBOBOXINFO
#undef CB_MSGMAX
	//Combo Box return Values
	inline constexpr int32_t CB_OKAY = 0;
	inline constexpr int32_t CB_ERR = (-1);
	inline constexpr int32_t CB_ERRSPACE = (-2);
	//Combo Box Notification Codes
	inline constexpr int32_t CBN_ERRSPACE = (-1);
	inline constexpr int32_t CBN_SELCHANGE = 1;
	inline constexpr int32_t CBN_DBLCLK = 2;
	inline constexpr int32_t CBN_SETFOCUS = 3;
	inline constexpr int32_t CBN_KILLFOCUS = 4;
	inline constexpr int32_t CBN_EDITCHANGE = 5;
	inline constexpr int32_t CBN_EDITUPDATE = 6;
	inline constexpr int32_t CBN_DROPDOWN = 7;
	inline constexpr int32_t CBN_CLOSEUP = 8;
	inline constexpr int32_t CBN_SELENDOK = 9;
	inline constexpr int32_t CBN_SELENDCANCEL = 10;
	//Combo Box styles
	inline constexpr int32_t CBS_SIMPLE = 0x0001L;
	inline constexpr int32_t CBS_DROPDOWN = 0x0002L;
	inline constexpr int32_t CBS_DROPDOWNLIST = 0x0003L;
	inline constexpr int32_t CBS_OWNERDRAWFIXED = 0x0010L;
	inline constexpr int32_t CBS_OWNERDRAWVARIABLE = 0x0020L;
	inline constexpr int32_t CBS_AUTOHSCROLL = 0x0040L;
	inline constexpr int32_t CBS_OEMCONVERT = 0x0080L;
	inline constexpr int32_t CBS_SORT = 0x0100L;
	inline constexpr int32_t CBS_HASSTRINGS = 0x0200L;
	inline constexpr int32_t CBS_NOINTEGRALHEIGHT = 0x0400L;
	inline constexpr int32_t CBS_DISABLENOSCROLL = 0x0800L;
	inline constexpr int32_t CBS_UPPERCASE = 0x2000L;
	inline constexpr int32_t CBS_LOWERCASE = 0x4000L;
	//Combo Box messages
	inline constexpr int32_t CB_GETEDITSEL = 0x0140;
	inline constexpr int32_t CB_LIMITTEXT = 0x0141;
	inline constexpr int32_t CB_SETEDITSEL = 0x0142;
	inline constexpr int32_t CB_ADDSTRING = 0x0143;
	inline constexpr int32_t CB_DELETESTRING = 0x0144;
	inline constexpr int32_t CB_DIR = 0x0145;
	inline constexpr int32_t CB_GETCOUNT = 0x0146;
	inline constexpr int32_t CB_GETCURSEL = 0x0147;
	inline constexpr int32_t CB_GETLBTEXT = 0x0148;
	inline constexpr int32_t CB_GETLBTEXTLEN = 0x0149;
	inline constexpr int32_t CB_INSERTSTRING = 0x014A;
	inline constexpr int32_t CB_RESETCONTENT = 0x014B;
	inline constexpr int32_t CB_FINDSTRING = 0x014C;
	inline constexpr int32_t CB_SELECTSTRING = 0x014D;
	inline constexpr int32_t CB_SETCURSEL = 0x014E;
	inline constexpr int32_t CB_SHOWDROPDOWN = 0x014F;
	inline constexpr int32_t CB_GETITEMDATA = 0x0150;
	inline constexpr int32_t CB_SETITEMDATA = 0x0151;
	inline constexpr int32_t CB_GETDROPPEDCONTROLRECT = 0x0152;
	inline constexpr int32_t CB_SETITEMHEIGHT = 0x0153;
	inline constexpr int32_t CB_GETITEMHEIGHT = 0x0154;
	inline constexpr int32_t CB_SETEXTENDEDUI = 0x0155;
	inline constexpr int32_t CB_GETEXTENDEDUI = 0x0156;
	inline constexpr int32_t CB_GETDROPPEDSTATE = 0x0157;
	inline constexpr int32_t CB_FINDSTRINGEXACT = 0x0158;
	inline constexpr int32_t CB_SETLOCALE = 0x0159;
	inline constexpr int32_t CB_GETLOCALE = 0x015A;
	inline constexpr int32_t CB_GETTOPINDEX = 0x015b;
	inline constexpr int32_t CB_SETTOPINDEX = 0x015c;
	inline constexpr int32_t CB_GETHORIZONTALEXTENT = 0x015d;
	inline constexpr int32_t CB_SETHORIZONTALEXTENT = 0x015e;
	inline constexpr int32_t CB_GETDROPPEDWIDTH = 0x015f;
	inline constexpr int32_t CB_SETDROPPEDWIDTH = 0x0160;
	inline constexpr int32_t CB_INITSTORAGE = 0x0161;
	inline constexpr int32_t CB_MULTIPLEADDSTRING = 0x0163;
	inline constexpr int32_t CB_GETCOMBOBOXINFO = 0x0164;
#if(_WIN32_WINNT >= 0x0501)
	inline constexpr int32_t CB_MSGMAX = 0x0165;
#elif defined(_WIN32_WCE) && (_WIN32_WCE >= 0x0400)
	inline constexpr int32_t CB_MSGMAX = 0x0163;
#elif(WINVER >= 0x0400)
	inline constexpr int32_t CB_MSGMAX = 0x0162;
#else
	inline constexpr int32_t CB_MSGMAX = 0x015B;
#endif
}

//Resource Types
export namespace WinAPI {
#undef RT_CURSOR
#undef RT_BITMAP
#undef RT_ICON
#undef RT_MENU
#undef RT_DIALOG
#undef RT_STRING
#undef RT_FONTDIR
#undef RT_FONT
#undef RT_ACCELERATOR
#undef RT_RCDATA
#undef RT_MESSAGETABLE
#undef DIFFERENCE
#undef RT_GROUP_CURSOR
#undef RT_GROUP_ICON
#undef RT_VERSION
#undef RT_DLGINCLUDE
#undef RT_PLUGPLAY
#undef RT_VXD
#undef RT_ANICURSOR
#undef RT_ANIICON
#undef RT_HTML
#undef RT_MANIFEST
#undef CREATEPROCESS_MANIFEST_RESOURCE_ID
#undef ISOLATIONAWARE_MANIFEST_RESOURCE_ID
#undef ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID
#undef ISOLATIONPOLICY_MANIFEST_RESOURCE_ID
#undef ISOLATIONPOLICY_BROWSER_MANIFEST_RESOURCE_ID
#undef MINIMUM_RESERVED_MANIFEST_RESOURCE_ID
#undef MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID
	//Predefined Resource Types
	inline constexpr PTR_CONSTANT<wchar_t*> RT_CURSOR = static_cast<uint16_t>(1);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_BITMAP = static_cast<uint16_t>(2);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_ICON = static_cast<uint16_t>(3);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_MENU = static_cast<uint16_t>(4);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_DIALOG = static_cast<uint16_t>(5);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_STRING = static_cast<uint16_t>(6);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_FONTDIR = static_cast<uint16_t>(7);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_FONT = static_cast<uint16_t>(8);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_ACCELERATOR = static_cast<uint16_t>(9);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_RCDATA = static_cast<uint16_t>(10);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_MESSAGETABLE = static_cast<uint16_t>(11);
	inline constexpr PTR_CONSTANT<wchar_t*> DIFFERENCE = 11;
	inline constexpr PTR_CONSTANT<wchar_t*> RT_GROUP_CURSOR = static_cast<uint16_t>((ULONG_PTR)(RT_CURSOR) + (uint16_t)DIFFERENCE);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_GROUP_ICON = static_cast<uint16_t>((ULONG_PTR)(RT_ICON) +  (uint16_t)DIFFERENCE);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_VERSION = static_cast<uint16_t>(16);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_DLGINCLUDE = static_cast<uint16_t>(17);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_PLUGPLAY = static_cast<uint16_t>(19);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_VXD = static_cast<uint16_t>(20);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_ANICURSOR = static_cast<uint16_t>(21);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_ANIICON = static_cast<uint16_t>(22);
	inline constexpr PTR_CONSTANT<wchar_t*> RT_HTML = static_cast<uint16_t>(23);
#ifdef RC_INVOKED
	inline constexpr PTR_CONSTANT<wchar_t*> RT_MANIFEST = 24;
	inline constexpr PTR_CONSTANT<wchar_t*> CREATEPROCESS_MANIFEST_RESOURCE_ID = 1;
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONAWARE_MANIFEST_RESOURCE_ID = 2;
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID = 3;
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONPOLICY_MANIFEST_RESOURCE_ID = 4;
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONPOLICY_BROWSER_MANIFEST_RESOURCE_ID = 5;
	inline constexpr PTR_CONSTANT<wchar_t*> MINIMUM_RESERVED_MANIFEST_RESOURCE_ID = 1;
	inline constexpr PTR_CONSTANT<wchar_t*> MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID = 16;
#else  /* RC_INVOKED */
	inline constexpr PTR_CONSTANT<wchar_t*> RT_MANIFEST = static_cast<uint16_t>(24);
	inline constexpr PTR_CONSTANT<wchar_t*> CREATEPROCESS_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(1);
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONAWARE_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(2);
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(3);
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONPOLICY_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(4);
	inline constexpr PTR_CONSTANT<wchar_t*> ISOLATIONPOLICY_BROWSER_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(5);
	inline constexpr PTR_CONSTANT<wchar_t*> MINIMUM_RESERVED_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(1);
	inline constexpr PTR_CONSTANT<wchar_t*> MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID = static_cast<uint16_t>(16);
#endif /* RC_INVOKED */
}

export namespace WinAPI {
#undef SWP_NOSIZE
#undef SWP_NOMOVE
#undef SWP_NOZORDER
#undef SWP_NOREDRAW
#undef SWP_NOACTIVATE
#undef SWP_FRAMECHANGED
#undef SWP_SHOWWINDOW
#undef SWP_HIDEWINDOW
#undef SWP_NOCOPYBITS
#undef SWP_NOOWNERZORDER
#undef SWP_NOSENDCHANGING
#undef SWP_DRAWFRAME
#undef SWP_NOREPOSITION
#undef SWP_DEFERERASE
#undef SWP_ASYNCWINDOWPOS
#undef HWND_TOP
#undef HWND_BOTTOM
#undef HWND_TOPMOST
#undef HWND_NOTOPMOST
	inline constexpr uint32_t SWP_NOSIZE = 0x0001;
	inline constexpr uint32_t SWP_NOMOVE = 0x0002;
	inline constexpr uint32_t SWP_NOZORDER = 0x0004;
	inline constexpr uint32_t SWP_NOREDRAW = 0x0008;
	inline constexpr uint32_t SWP_NOACTIVATE = 0x0010;
	inline constexpr uint32_t SWP_FRAMECHANGED = 0x0020;	// The frame changed: send WM_NCCALCSIZE
	inline constexpr uint32_t SWP_SHOWWINDOW = 0x0040;
	inline constexpr uint32_t SWP_HIDEWINDOW = 0x0080;
	inline constexpr uint32_t SWP_NOCOPYBITS = 0x0100;
	inline constexpr uint32_t SWP_NOOWNERZORDER = 0x0200;	// Don't do owner Z ordering
	inline constexpr uint32_t SWP_NOSENDCHANGING = 0x0400;	// Don't send WM_WINDOWPOSCHANGING
	inline constexpr uint32_t SWP_DRAWFRAME = SWP_FRAMECHANGED;
	inline constexpr uint32_t SWP_NOREPOSITION = SWP_NOOWNERZORDER;
	inline constexpr uint32_t SWP_DEFERERASE = 0x2000;	// same as SWP_DEFERDRAWING
	inline constexpr uint32_t SWP_ASYNCWINDOWPOS = 0x4000;	// same as SWP_CREATESPB
	inline constexpr HWND_CONSTANT HWND_TOP = 0;
	inline constexpr HWND_CONSTANT HWND_BOTTOM = 1;
	inline constexpr HWND_CONSTANT HWND_TOPMOST = -1;
	inline constexpr HWND_CONSTANT HWND_NOTOPMOST = -2;
}

//System Commands
export namespace WinAPI {
#undef SC_CLOSE
#undef SC_MAXIMIZE
#undef SC_MINIMIZE
	inline constexpr uint32_t SC_CLOSE = 0xf060;
	inline constexpr uint32_t SC_MAXIMIZE = 0xf030;
	inline constexpr uint32_t SC_MINIMIZE = 0xf020;
}

//MessageBox() Return Value
export namespace WinAPI {
#undef IDABORT
#undef IDCANCEL
#undef IDCONTINUE
#undef IDIGNORE
#undef IDNO
#undef IDOK
#undef IDRETRY
#undef IDTRYAGAIN
#undef IDYES
	inline constexpr int IDABORT = 3;
	inline constexpr int IDCANCEL = 2;
	inline constexpr int IDCONTINUE = 11;
	inline constexpr int IDIGNORE = 5;
	inline constexpr int IDNO = 7;
	inline constexpr int IDOK = 1;
	inline constexpr int IDRETRY = 4;
	inline constexpr int IDTRYAGAIN = 10;
	inline constexpr int IDYES = 6;
}