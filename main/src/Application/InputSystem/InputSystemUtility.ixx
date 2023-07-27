export module InputSystemUtility;
import System;
using namespace System;

export namespace System::Application {
	enum class ActionType : uint8_t {
		Value = 0,
		Button,
		PassThrough
	};
	enum class InteractionType : uint8_t {
		Press = 0,
		Hold,
		Tap,
		SlowTap,
		MultiTap
	};
	enum class TriggerBehaviour : uint8_t {
		PressOnly = 0,
		ReleaseOnly,
		PressAndRelease
	};
}

export namespace System::Application {
	enum class ControllerButtons : uint32_t {
		LMouse = 1,
		RMouse,
		KeyCancel,
		MMouse,
		KeyBack = 8,
		KeyTab,
		KeyClear = 0x0c,
		KeyReturn,
		KeyShift = 0x10,
		KeyControl,
		KeyMenu,
		KeyAlt = KeyMenu,
		KeyPause,
		KeyCapital,
		KeyEscape = 0x1b,
		KeySpace = 0x20,
		KeyPageUp,
		KeyPageDown,
		KeyEnd,
		KeyHome,
		KeyLeft,
		KeyUp,
		KeyRight,
		KeyDown,
		KeySelect,
		KeyPrint,
		KeyExecute,
		KeySnapshot,
		KeyInsert,
		KeyDelete,
		KeyHelp = 0x2f,
		Key0 = 0x30,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9 = 0x39,
		KeyA = 0x41,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ = 0x5A,
		KeyLCmd = 0x5B,
		KeyRCmd,
		KeyApp = 0x5D,
		KeyNum0 = 0x60,
		KeyNum1,
		KeyNum2,
		KeyNum3,
		KeyNum4,
		KeyNum5,
		KeyNum6,
		KeyNum7,
		KeyNum8,
		KeyNum9 = 0x69,
		KeyMultiply = 0x6A,
		KeyAdd,
		KeySeparator,	//ÉeÉìÉLÅ[Enter
		KeySubstract,
		KeyDecimal,
		KeyDivide = 0x6f,
		KeyF1 = 0x70,
		KeyF2,
		KeyF3,
		KeyF4,
		KeyF5,
		KeyF6,
		KeyF7,
		KeyF8,
		KeyF9,
		KeyF10,
		KeyF11,
		KeyF12,
		KeyF13,
		KeyF14,
		KeyF15,
		KeyF16,
		KeyF17,
		KeyF18,
		KeyF19,
		KeyF20,
		KeyF21,
		KeyF22,
		KeyF23,
		KeyF24 = 0x87,
		KeyNumlock = 0x90,
		KeyLShift = 0xA0,
		KeyRShift,
		KeyLControl,
		KeyRControl,
		KeyLAlt,
		KeyRAlt = 0xA5,
		KeyOEM1 = 0xBA,
		KeyColonUS = KeyOEM1,
		KeyOEMPlus = 0xBB,
		KeyOEMComma,
		KeyOEMMinus,
		KeyOEMPeriod,
		KeyOEM2,
		KeySlashUS = KeyOEM2,
		KeyOEM3 = 0xC0,
		KeyTildeUS = KeyOEM3,
		KeyOEM4 = 0xDB,
		KeyOpenBracketUS = KeyOEM4,
		KeyOEM5,
		KeyBackSlashUS = KeyOEM5,
		KeyOEM6,
		KeyCloseBracketUS = KeyOEM6,
		KeyOEM7,
		KeyQuoteUS = KeyOEM7,
		KeyOEM8 = 0xDF,
		KeyOEM102 = 0xE2,
		KeyAngleBracketsUS = KeyOEM102,
		RUp = 0x0100 | 1,	//B or Circle
		RLeft,						//A or Cross
		RRight,						//X or Square
		RDown,					//Y or Triangle
		LUp,
		LLeft,
		LRight,
		LDown,
		LShoulder,		//L1
		RShoulder,		//R1
		LTrigger,			//L2
		RTrigger,			//L2
		LStickButton,	//L3
		RStickButton,	//R3
		LStick,	//(Vector2)
		RStick,	//(Vector2)
		LMenu,		//Back or Share
		MMenu,	//Guide or PS
		RMenu,	//Start or Options
		Wheel = 0x1000 | 1,
		MouseWheel,
		MouseMove,
		MouseXYZ
	};
	String ToString(ControllerButtons button) noexcept {
		switch (button) {
		case ControllerButtons::LMouse: return String(u"LMouse");
		case ControllerButtons::RMouse: return String(u"RMouse");
		case ControllerButtons::KeyCancel: return String(u"KeyCancel");
		case ControllerButtons::MMouse: return String(u"MMouse");
		case ControllerButtons::KeyBack: return String(u"KeyBack");
		case ControllerButtons::KeyTab: return String(u"KeyTab");
		case ControllerButtons::KeyClear: return String(u"KeyClear");
		case ControllerButtons::KeyReturn: return String(u"KeyReturn");
		case ControllerButtons::KeyShift: return String(u"KeyShift");
		case ControllerButtons::KeyControl: return String(u"KeyControl");
		case ControllerButtons::KeyAlt: return String(u"KeyAlt");
		case ControllerButtons::KeyPause: return String(u"KeyPause");
		case ControllerButtons::KeyCapital: return String(u"KeyCapital");
		case ControllerButtons::KeyEscape: return String(u"KeyEscape");
		case ControllerButtons::KeySpace: return String(u"KeySpace");
		case ControllerButtons::KeyPageUp: return String(u"KeyPageUp");
		case ControllerButtons::KeyPageDown: return String(u"KeyPageDown");
		case ControllerButtons::KeyEnd: return String(u"KeyEnd");
		case ControllerButtons::KeyHome: return String(u"KeyHome");
		case ControllerButtons::KeyLeft: return String(u"KeyLeft");
		case ControllerButtons::KeyUp: return String(u"KeyUp");
		case ControllerButtons::KeyRight: return String(u"KeyRight");
		case ControllerButtons::KeyDown: return String(u"KeyDown");
		case ControllerButtons::KeySelect: return String(u"KeySelect");
		case ControllerButtons::KeyPrint: return String(u"KeyPrint");
		case ControllerButtons::KeyExecute: return String(u"KeyExecute");
		case ControllerButtons::KeySnapshot: return String(u"KeySnapshot");
		case ControllerButtons::KeyInsert: return String(u"KeyInsert");
		case ControllerButtons::KeyDelete: return String(u"KeyDelete");
		case ControllerButtons::KeyHelp: return String(u"KeyHelp");
		case ControllerButtons::Key0: return String(u"Key0");
		case ControllerButtons::Key1: return String(u"Key1");
		case ControllerButtons::Key2: return String(u"Key2");
		case ControllerButtons::Key3: return String(u"Key3");
		case ControllerButtons::Key4: return String(u"Key4");
		case ControllerButtons::Key5: return String(u"Key5");
		case ControllerButtons::Key6: return String(u"Key6");
		case ControllerButtons::Key7: return String(u"Key7");
		case ControllerButtons::Key8: return String(u"Key8");
		case ControllerButtons::Key9: return String(u"Key9");
		case ControllerButtons::KeyA: return String(u"KeyA");
		case ControllerButtons::KeyB: return String(u"KeyB");
		case ControllerButtons::KeyC: return String(u"KeyC");
		case ControllerButtons::KeyD: return String(u"KeyD");
		case ControllerButtons::KeyE: return String(u"KeyE");
		case ControllerButtons::KeyF: return String(u"KeyF");
		case ControllerButtons::KeyG: return String(u"KeyG");
		case ControllerButtons::KeyH: return String(u"KeyH");
		case ControllerButtons::KeyI: return String(u"KeyI");
		case ControllerButtons::KeyJ: return String(u"KeyJ");
		case ControllerButtons::KeyK: return String(u"KeyK");
		case ControllerButtons::KeyL: return String(u"KeyL");
		case ControllerButtons::KeyM: return String(u"KeyM");
		case ControllerButtons::KeyN: return String(u"KeyN");
		case ControllerButtons::KeyO: return String(u"KeyO");
		case ControllerButtons::KeyP: return String(u"KeyP");
		case ControllerButtons::KeyQ: return String(u"KeyQ");
		case ControllerButtons::KeyR: return String(u"KeyR");
		case ControllerButtons::KeyS: return String(u"KeyS");
		case ControllerButtons::KeyT: return String(u"KeyT");
		case ControllerButtons::KeyU: return String(u"KeyU");
		case ControllerButtons::KeyV: return String(u"KeyV");
		case ControllerButtons::KeyW: return String(u"KeyW");
		case ControllerButtons::KeyX: return String(u"KeyX");
		case ControllerButtons::KeyY: return String(u"KeyY");
		case ControllerButtons::KeyZ: return String(u"KeyZ");
		case ControllerButtons::KeyLCmd: return String(u"KeyLCmd");
		case ControllerButtons::KeyRCmd: return String(u"KeyRCmd");
		case ControllerButtons::KeyApp: return String(u"KeyApp");
		case ControllerButtons::KeyNum0: return String(u"KeyNum0");
		case ControllerButtons::KeyNum1: return String(u"KeyNum1");
		case ControllerButtons::KeyNum2: return String(u"KeyNum2");
		case ControllerButtons::KeyNum3: return String(u"KeyNum3");
		case ControllerButtons::KeyNum4: return String(u"KeyNum4");
		case ControllerButtons::KeyNum5: return String(u"KeyNum5");
		case ControllerButtons::KeyNum6: return String(u"KeyNum6");
		case ControllerButtons::KeyNum7: return String(u"KeyNum7");
		case ControllerButtons::KeyNum8: return String(u"KeyNum8");
		case ControllerButtons::KeyNum9: return String(u"KeyNum9");
		case ControllerButtons::KeyMultiply: return String(u"KeyMultiply");
		case ControllerButtons::KeyAdd: return String(u"KeyAdd");
		case ControllerButtons::KeySeparator: return String(u"KeySeparator");
		case ControllerButtons::KeySubstract: return String(u"KeySubstract");
		case ControllerButtons::KeyDecimal: return String(u"KeyDecimal");
		case ControllerButtons::KeyDivide: return String(u"KeyDivide");
		case ControllerButtons::KeyF1: return String(u"KeyF1");
		case ControllerButtons::KeyF2: return String(u"KeyF2");
		case ControllerButtons::KeyF3: return String(u"KeyF3");
		case ControllerButtons::KeyF4: return String(u"KeyF4");
		case ControllerButtons::KeyF5: return String(u"KeyF5");
		case ControllerButtons::KeyF6: return String(u"KeyF6");
		case ControllerButtons::KeyF7: return String(u"KeyF7");
		case ControllerButtons::KeyF8: return String(u"KeyF8");
		case ControllerButtons::KeyF9: return String(u"KeyF9");
		case ControllerButtons::KeyF10: return String(u"KeyF10");
		case ControllerButtons::KeyF11: return String(u"KeyF11");
		case ControllerButtons::KeyF12: return String(u"KeyF12");
		case ControllerButtons::KeyF13: return String(u"KeyF13");
		case ControllerButtons::KeyF14: return String(u"KeyF14");
		case ControllerButtons::KeyF15: return String(u"KeyF15");
		case ControllerButtons::KeyF16: return String(u"KeyF16");
		case ControllerButtons::KeyF17: return String(u"KeyF17");
		case ControllerButtons::KeyF18: return String(u"KeyF18");
		case ControllerButtons::KeyF19: return String(u"KeyF19");
		case ControllerButtons::KeyF20: return String(u"KeyF20");
		case ControllerButtons::KeyF21: return String(u"KeyF21");
		case ControllerButtons::KeyF22: return String(u"KeyF22");
		case ControllerButtons::KeyF23: return String(u"KeyF23");
		case ControllerButtons::KeyF24: return String(u"KeyF24");
		case ControllerButtons::KeyNumlock: return String(u"KeyNumlock");
		case ControllerButtons::KeyLShift: return String(u"KeyLShift");
		case ControllerButtons::KeyRShift: return String(u"KeyRShift");
		case ControllerButtons::KeyLControl: return String(u"KeyLControl");
		case ControllerButtons::KeyRControl: return String(u"KeyRControl");
		case ControllerButtons::KeyLAlt: return String(u"KeyLAlt");
		case ControllerButtons::KeyRAlt: return String(u"KeyRAlt");
		case ControllerButtons::KeyOEM1: return String(u"KeyOEM1");
		case ControllerButtons::KeyOEMPlus: return String(u"KeyOEMPlus");
		case ControllerButtons::KeyOEMComma: return String(u"KeyOEMComma");
		case ControllerButtons::KeyOEMMinus: return String(u"KeyOEMMinus");
		case ControllerButtons::KeyOEMPeriod: return String(u"KeyOEMPeriod");
		case ControllerButtons::KeyOEM2: return String(u"KeyOEM2");
		case ControllerButtons::KeyOEM3: return String(u"KeyOEM3");
		case ControllerButtons::KeyOEM4: return String(u"KeyOEM4");
		case ControllerButtons::KeyOEM5: return String(u"KeyOEM5");
		case ControllerButtons::KeyOEM6: return String(u"KeyOEM6");
		case ControllerButtons::KeyOEM7: return String(u"KeyOEM7");
		case ControllerButtons::KeyOEM8: return String(u"KeyOEM8");
		case ControllerButtons::KeyOEM102: return String(u"KeyOEM102");
		case ControllerButtons::RUp: return String(u"RUp");
		case ControllerButtons::RLeft: return String(u"RLeft");
		case ControllerButtons::RRight: return String(u"RRight");
		case ControllerButtons::RDown: return String(u"RDown");
		case ControllerButtons::LUp: return String(u"LUp");
		case ControllerButtons::LLeft: return String(u"LLeft");
		case ControllerButtons::LRight: return String(u"LRight");
		case ControllerButtons::LDown: return String(u"LDown");
		case ControllerButtons::LShoulder: return String(u"LShoulder");
		case ControllerButtons::RShoulder: return String(u"RShoulder");
		case ControllerButtons::LTrigger: return String(u"LTrigger");
		case ControllerButtons::RTrigger: return String(u"RTrigger");
		case ControllerButtons::LStickButton: return String(u"LStickButton");
		case ControllerButtons::RStickButton: return String(u"RStickButton");
		case ControllerButtons::LStick: return String(u"LStick");
		case ControllerButtons::RStick: return String(u"RStick");
		case ControllerButtons::LMenu: return String(u"LMenu");
		case ControllerButtons::MMenu: return String(u"MMenu");
		case ControllerButtons::RMenu: return String(u"RMenu");
		case ControllerButtons::Wheel: return String(u"Wheel");
		case ControllerButtons::MouseWheel: return String(u"MouseWheel");
		case ControllerButtons::MouseMove: return String(u"MouseMove");
		case ControllerButtons::MouseXYZ: return String(u"MouseXYZ");
		default: return String(u"Undefined");
		}
	}
	enum class InputValueType : uint8_t {
		Button = 0,
		Vector2,
		Vector3
	};
	enum class InputBindingTarget : uint8_t {
		Button = 0,
		Up = Button,
		Down,
		Left,
		Right,
		Forward,
		Backward
	};
	enum class InputBindingType : uint8_t {
		Simple = 0,
		Composite
	};
	enum class InputSpecialBindingTarget : uint8_t {
		WASD = 0,
		Arrow
	};
}

export namespace System::Application {
	struct InputBinding {
		InputBindingType Type = InputBindingType::Simple;
		union {
			ControllerButtons Button;
			ControllerButtons Up;
		};
		ControllerButtons Down;
		ControllerButtons Left;
		ControllerButtons Right;
		ControllerButtons Forward;
		ControllerButtons Backward;
	public:
		InputBinding& With(ControllerButtons button, InputBindingTarget target = InputBindingTarget::Button) noexcept {
			switch (target) {
			case InputBindingTarget::Up:
				Up = button;
				break;
			case InputBindingTarget::Down:
				Down = button;
				break;
			case InputBindingTarget::Left:
				Left = button;
				break;
			case InputBindingTarget::Right:
				Right = button;
				break;
			case InputBindingTarget::Forward:
				Forward = button;
				break;
			case InputBindingTarget::Backward:
				Backward = button;
				break;
			default:
				break;
			}
			return *this;
		}
		InputBinding& With(InputSpecialBindingTarget target) noexcept {
			switch (target) {
			case InputSpecialBindingTarget::WASD:
				Up = ControllerButtons::KeyW;
				Down = ControllerButtons::KeyS;
				Left = ControllerButtons::KeyD;
				Right = ControllerButtons::KeyA;
				break;
			case InputSpecialBindingTarget::Arrow:
				Up = ControllerButtons::KeyUp;
				Down = ControllerButtons::KeyDown;
				Left = ControllerButtons::KeyLeft;
				Right = ControllerButtons::KeyRight;
				break;
			default:
				return *this;
			}
			Type = InputBindingType::Composite;
			return *this;
		}
	};

	struct InputState {
		InputValueType ValueType = InputValueType::Button;
		bool Started = false;
		bool Performed = false;
		bool Canceled = false;
		union {
			Vector3 XYZ = Vector3();
			Vector2 XY;
			float Power;
			bool Pressed;
		};
	};
}