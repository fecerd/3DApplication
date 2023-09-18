module;
#define DIRECTINPUT_VERSION 0x0800
//マクロ再定義抑制
#undef __SPECSTRINGS_STRICT_LEVEL
#define __SPECSTRINGS_STRICT_LEVEL 0
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#include <dinput.h>
#undef __nullnullterminated
#undef interface
#undef OutputDebugString
module DirectInput;
import CSTDINT;
import InputSystemUtility;
import NamespaceHelper;

namespace System::Application::Windows {
	uint8_t ToDirectInputKeys(ControllerButtons key) noexcept {
		switch (key) {
		case ControllerButtons::Key0: return DIK_0;
		case ControllerButtons::Key1: return DIK_1;
		case ControllerButtons::Key2: return DIK_2;
		case ControllerButtons::Key3: return DIK_3;
		case ControllerButtons::Key4: return DIK_4;
		case ControllerButtons::Key5: return DIK_5;
		case ControllerButtons::Key6: return DIK_6;
		case ControllerButtons::Key7: return DIK_7;
		case ControllerButtons::Key8: return DIK_8;
		case ControllerButtons::Key9: return DIK_9;
		case ControllerButtons::KeyA: return DIK_A;
		case ControllerButtons::KeyB: return DIK_B;
		case ControllerButtons::KeyC: return DIK_C;
		case ControllerButtons::KeyD: return DIK_D;
		case ControllerButtons::KeyE: return DIK_E;
		case ControllerButtons::KeyF: return DIK_F;
		case ControllerButtons::KeyG: return DIK_G;
		case ControllerButtons::KeyH: return DIK_H;
		case ControllerButtons::KeyI: return DIK_I;
		case ControllerButtons::KeyJ: return DIK_J;
		case ControllerButtons::KeyK: return DIK_K;
		case ControllerButtons::KeyL: return DIK_L;
		case ControllerButtons::KeyM: return DIK_M;
		case ControllerButtons::KeyN: return DIK_N;
		case ControllerButtons::KeyO: return DIK_O;
		case ControllerButtons::KeyP: return DIK_P;
		case ControllerButtons::KeyQ: return DIK_Q;
		case ControllerButtons::KeyR: return DIK_R;
		case ControllerButtons::KeyS: return DIK_S;
		case ControllerButtons::KeyT: return DIK_T;
		case ControllerButtons::KeyU: return DIK_U;
		case ControllerButtons::KeyV: return DIK_V;
		case ControllerButtons::KeyW: return DIK_W;
		case ControllerButtons::KeyX: return DIK_X;
		case ControllerButtons::KeyY: return DIK_Y;
		case ControllerButtons::KeyZ: return DIK_Z;
		case ControllerButtons::KeyEscape: return DIK_ESCAPE;
		case ControllerButtons::KeyF1: return DIK_F1;
		case ControllerButtons::KeyF2: return DIK_F2;
		case ControllerButtons::KeyF3: return DIK_F3;
		case ControllerButtons::KeyF4: return DIK_F4;
		case ControllerButtons::KeyF5: return DIK_F5;
		case ControllerButtons::KeyF6: return DIK_F6;
		case ControllerButtons::KeyF7: return DIK_F7;
		case ControllerButtons::KeyF8: return DIK_F8;
		case ControllerButtons::KeyF9: return DIK_F9;
		case ControllerButtons::KeyF10: return DIK_F10;
		case ControllerButtons::KeyF11: return DIK_F11;
		case ControllerButtons::KeyF12: return DIK_F12;
		case ControllerButtons::KeyF13: return DIK_F13;
		case ControllerButtons::KeyF14: return DIK_F14;
		case ControllerButtons::KeyF15: return DIK_F15;
		case ControllerButtons::KeyDelete: return DIK_DELETE;
		case ControllerButtons::KeyTildeUS: return DIK_GRAVE;
		case ControllerButtons::KeyOEMMinus: return DIK_MINUS;
		case ControllerButtons::KeyOEMPlus: return DIK_EQUALS;
		case ControllerButtons::KeyBack: return DIK_BACKSPACE;
		case ControllerButtons::KeyTab: return DIK_TAB;
		case ControllerButtons::KeyOpenBracketUS: return DIK_LBRACKET;
		case ControllerButtons::KeyCloseBracketUS: return DIK_RBRACKET;
		case ControllerButtons::KeyBackSlashUS: return DIK_BACKSLASH;
		case ControllerButtons::KeyCapital: return DIK_CAPITAL;
		case ControllerButtons::KeyColonUS: return DIK_SEMICOLON;
		case ControllerButtons::KeyReturn: return DIK_RETURN;
		case ControllerButtons::KeyLShift: return DIK_LSHIFT;
		case ControllerButtons::KeyRShift: return DIK_RSHIFT;
		case ControllerButtons::KeyOEMComma: return DIK_COMMA;
		case ControllerButtons::KeyOEMPeriod: return DIK_PERIOD;
		case ControllerButtons::KeySlashUS: return DIK_SLASH;
		case ControllerButtons::KeyLControl: return DIK_LCONTROL;
		case ControllerButtons::KeyLCmd: return DIK_LWIN;
		case ControllerButtons::KeyLAlt: return DIK_LMENU;
		case ControllerButtons::KeySpace: return DIK_SPACE;
		case ControllerButtons::KeyRAlt: return DIK_RMENU;
		case ControllerButtons::KeyRCmd: return DIK_RWIN;
		case ControllerButtons::KeyUp: return DIK_UP;
		case ControllerButtons::KeyRight: return DIK_RIGHT;
		case ControllerButtons::KeyDown: return DIK_DOWN;
		case ControllerButtons::KeyLeft: return DIK_LEFT;
		default: return 0;
		}
	}
}

