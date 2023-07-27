module;
#define DIRECTINPUT_VERSION 0x0800
//マクロ再定義抑制
#undef __SPECSTRINGS_STRICT_LEVEL
#define __SPECSTRINGS_STRICT_LEVEL 0
#pragma warning(disable : 5105)
#include <dinput.h>
#undef __nullnullterminated
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#undef interface
#undef OutputDebugString
export module DirectInput;
import System;
import IInputManager;
import WinAPI;
using namespace System;
using namespace System::Application;

namespace System::Application::Windows {
	enum class DirectInputButtons : uint8_t {
		None = 0,
		SquareOrX,
		CrossOrA,
		CircleOrB,
		TriangleOrY,
		LButton,
		RButton,
		LTrigger,
		RTrigger,
		LStickButton,
		RStickButton,
		SelectOrBack,
		OptionOrStart,
		Home,
		PadDown,
		LMouse,
		RMouse,
		MMouse,
		POVRight = 0x81,
		POVDown,
		POVLeft,
		POVUp
	};
	enum class DirectInputValues : uint8_t {
		None = 0,
		LStick,
		RStick,
		MouseMove,
		MouseWheel,
		Mouse
	};
	DirectInputButtons ToDirectInputButtons(ControllerButtons button) noexcept {
		switch (button) {
		case ControllerButtons::RRight: return DirectInputButtons::CircleOrB;
		case ControllerButtons::RDown: return DirectInputButtons::CrossOrA;
		case ControllerButtons::RLeft: return DirectInputButtons::SquareOrX;
		case ControllerButtons::RUp: return DirectInputButtons::TriangleOrY;
		case ControllerButtons::LRight: return DirectInputButtons::POVRight;
		case ControllerButtons::LDown: return DirectInputButtons::POVDown;
		case ControllerButtons::LLeft: return DirectInputButtons::POVLeft;
		case ControllerButtons::LUp: return DirectInputButtons::POVUp;
		case ControllerButtons::LMenu: return DirectInputButtons::SelectOrBack;
		case ControllerButtons::MMenu: return DirectInputButtons::Home;
		case ControllerButtons::RMenu: return DirectInputButtons::OptionOrStart;
		case ControllerButtons::LShoulder: return DirectInputButtons::LButton;
		case ControllerButtons::RShoulder: return DirectInputButtons::RButton;
		case ControllerButtons::LTrigger: return DirectInputButtons::LTrigger;
		case ControllerButtons::RTrigger: return DirectInputButtons::RTrigger;
		case ControllerButtons::LStickButton: return DirectInputButtons::LStickButton;
		case ControllerButtons::RStickButton: return DirectInputButtons::RStickButton;
		case ControllerButtons::LMouse: return DirectInputButtons::LMouse;
		case ControllerButtons::MMouse: return DirectInputButtons::MMouse;
		case ControllerButtons::RMouse: return DirectInputButtons::RMouse;
		default: return DirectInputButtons::None;
		}
	}
	DirectInputValues ToDirectInputValues(ControllerButtons button) noexcept {
		switch (button) {
		case ControllerButtons::LStick: return DirectInputValues::LStick;
		case ControllerButtons::RStick: return DirectInputValues::RStick;
		case ControllerButtons::MouseMove: return DirectInputValues::MouseMove;
		case ControllerButtons::MouseWheel: return DirectInputValues::MouseWheel;
		case ControllerButtons::MouseXYZ: return DirectInputValues::Mouse;
		default: return DirectInputValues::None;
		}
	}
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
	enum class DirectInputEventState : uint8_t {
		Empty = 0,
		Started,
		Performed,
		Canceled
	};

	namespace DirectInputConstant {
		inline constexpr bool IsAbsAxis = true;
		inline constexpr int32_t XAxisAbs = 1000;
		inline constexpr int32_t YAxisAbs = 1000;
		inline constexpr int32_t ZAxisAbs = 1000;
		inline constexpr int32_t BufferCount = 64;
		inline constexpr uint8_t MaxPlayerCount = 4;
	};
	using namespace DirectInputConstant;
	struct DirectInputState {
		DIJOYSTATE joystick[2] = {};
		uint8_t keyboard[2][256] = {};
		DIMOUSESTATE mouse[2] = {};
		uint8_t currentJoystickIndex = 0;
		uint8_t currentKeyboardIndex = 0;
		uint8_t currentMouseIndex = 0;
	private:
		static bool GetButton_Internal(const DIJOYSTATE& state, DirectInputButtons diButton) noexcept {
			if (static_cast<uint8_t>(diButton) & 0x80) {
				if (state.rgdwPOV[0] == 0xffffffff) return false;
				switch (diButton) {
				case DirectInputButtons::POVUp:
					return state.rgdwPOV[0] <= 4500 || state.rgdwPOV[0] >= 31500;
				case DirectInputButtons::POVRight:
					return 4500 <= state.rgdwPOV[0] && state.rgdwPOV[0] <= 13500;
				case DirectInputButtons::POVDown:
					return 13500 <= state.rgdwPOV[0] && state.rgdwPOV[0] <= 22500;
				case DirectInputButtons::POVLeft:
					return 22500 <= state.rgdwPOV[0] && state.rgdwPOV[0] <= 31500;
				default:
					return false;
				}
			}
			else return state.rgbButtons[static_cast<uint8_t>(diButton) - 1] & 0x80;
		}
		static bool GetKey_Internal(const uint8_t(&state)[256], uint8_t diKey) noexcept {
			return state[diKey] & 0x80;
		}
		static bool GetMouseButton_Internal(const DIMOUSESTATE& state, DirectInputButtons diButton) noexcept {
			switch (diButton) {
			case DirectInputButtons::LMouse:
				return state.rgbButtons[0] & 0x80;
			case DirectInputButtons::MMouse:
				return state.rgbButtons[2] & 0x80;
			case DirectInputButtons::RMouse:
				return state.rgbButtons[1] & 0x80;
			default:
				return false;
			}
		}
		static bool IsMouseButton(DirectInputButtons diButton) noexcept {
			switch (diButton) {
			case DirectInputButtons::LMouse:
			case DirectInputButtons::MMouse:
			case DirectInputButtons::RMouse:
				return true;
			default:
				return false;
			}
		}
		static bool IsMouseValue(DirectInputValues diValue) noexcept {
			switch (diValue) {
			case DirectInputValues::MouseMove:
			case DirectInputValues::MouseWheel:
			case DirectInputValues::Mouse:
				return true;
			default:
				return false;
			}
		}
	public:
		DirectInputEventState GetButton(DirectInputButtons diButton) const noexcept {
			if (IsMouseButton(diButton)) {
				return GetMouseButton_Internal(mouse[currentMouseIndex], diButton)
					? GetMouseButton_Internal(mouse[!currentMouseIndex], diButton) ? DirectInputEventState::Performed : DirectInputEventState::Started
					: GetMouseButton_Internal(mouse[!currentMouseIndex], diButton) ? DirectInputEventState::Canceled : DirectInputEventState::Empty;
			}
			else {
				return GetButton_Internal(joystick[currentJoystickIndex], diButton)
					? GetButton_Internal(joystick[!currentJoystickIndex], diButton) ? DirectInputEventState::Performed : DirectInputEventState::Started
					: GetButton_Internal(joystick[!currentJoystickIndex], diButton) ? DirectInputEventState::Canceled : DirectInputEventState::Empty;
			}
		}
		DirectInputEventState GetValue(DirectInputValues diValue, Vector3& out) const noexcept {
			if (IsMouseValue(diValue)) {
				const DIMOUSESTATE& current = mouse[currentMouseIndex];
				const DIMOUSESTATE& prev = mouse[!currentMouseIndex];
				if (diValue != DirectInputValues::MouseWheel) {
					if (current.lX != 0 || current.lY != 0 || (diValue == DirectInputValues::Mouse && current.lZ != 0)) {
						out.x = static_cast<float>(current.lX);
						out.y = static_cast<float>(current.lY);
						if (diValue == DirectInputValues::Mouse) out.z = static_cast<float>(current.lZ);
						return prev.lX != 0 || prev.lY != 0 || (diValue == DirectInputValues::Mouse && prev.lZ != 0)
							? DirectInputEventState::Performed
							: DirectInputEventState::Started;
					}
					else {
						out.x = 0;
						out.y = 0;
						if (diValue == DirectInputValues::Mouse) out.z = 0;
						return prev.lX != 0 || prev.lY != 0 || (diValue == DirectInputValues::Mouse && prev.lZ != 0)
							? DirectInputEventState::Canceled
							: DirectInputEventState::Empty;
					}
				}
				else {
					if (current.lZ != 0) {
						out.x = static_cast<float>(current.lZ);
						return prev.lZ != 0 
							? DirectInputEventState::Performed
							: DirectInputEventState::Started;
					}
					else {
						out.x = 0;
						return prev.lZ != 0
							? DirectInputEventState::Canceled
							: DirectInputEventState::Empty;
					}
				}
			}
			else {
				const DIJOYSTATE& current = joystick[currentJoystickIndex];
				const DIJOYSTATE& prev = joystick[!currentJoystickIndex];
				LONG DIJOYSTATE::* x = nullptr;
				LONG DIJOYSTATE::* y = nullptr;
				if (diValue == DirectInputValues::LStick) {
					x = &DIJOYSTATE::lX;
					y = &DIJOYSTATE::lY;
				}
				else if (diValue == DirectInputValues::RStick) {
					x = &DIJOYSTATE::lZ;
					y = &DIJOYSTATE::lRz;
				}
				if (!x || !y) return DirectInputEventState::Empty;
				if (current.*x != 0 || current.*y != 0) {
					out.x = current.*x / static_cast<float>(XAxisAbs);
					out.y = current.*y / -static_cast<float>(YAxisAbs);
					if (prev.*x != 0 || prev.*y != 0) return DirectInputEventState::Performed;
					else return DirectInputEventState::Started;
				}
				else {
					out.x = 0;
					out.y = 0;
					if (prev.*x != 0 || prev.*y != 0) return DirectInputEventState::Canceled;
					else return DirectInputEventState::Empty;
				}
			}
			return DirectInputEventState::Empty;
		}
		DirectInputEventState GetKey(uint8_t diKey) const noexcept {
			return GetKey_Internal(keyboard[currentKeyboardIndex], diKey)
				? GetKey_Internal(keyboard[!currentKeyboardIndex], diKey) ? DirectInputEventState::Performed : DirectInputEventState::Started
				: GetKey_Internal(keyboard[!currentKeyboardIndex], diKey) ? DirectInputEventState::Canceled : DirectInputEventState::Empty;
		}
		void Clear() noexcept {
			joystick[0] = DIJOYSTATE{};
			joystick[1] = DIJOYSTATE{};
			static uint8_t tmp[256] = {};
			Memory::Copy(keyboard[0], tmp, 256, true);
			Memory::Copy(keyboard[1], tmp, 256, true);
			mouse[0] = DIMOUSESTATE{};
			mouse[1] = DIMOUSESTATE{};
			currentJoystickIndex = 0;
			currentKeyboardIndex = 0;
			currentMouseIndex = 0;
		}
		void Update() noexcept {
			currentJoystickIndex = !currentJoystickIndex;
			currentKeyboardIndex = !currentKeyboardIndex;
			currentMouseIndex = !currentMouseIndex;
			joystick[currentJoystickIndex] = joystick[!currentJoystickIndex];
			Memory::Copy(keyboard[currentKeyboardIndex], keyboard[!currentKeyboardIndex], 256, true);
			mouse[currentMouseIndex] = mouse[!currentMouseIndex];
		}
	public:
		DIJOYSTATE& GetCurrentJoystick() noexcept {
			return joystick[currentJoystickIndex];
		}
		const DIJOYSTATE& GetCurrentJoystick() const noexcept {
			return joystick[currentJoystickIndex];
		}
		auto& GetCurrentKeyboard() noexcept {
			return keyboard[currentKeyboardIndex];
		}
		const auto& GetCurrentKeyboard() const noexcept {
			return keyboard[currentKeyboardIndex];
		}
		DIMOUSESTATE& GetCurrentMouse() noexcept {
			return mouse[currentMouseIndex];
		}
		const DIMOUSESTATE& GetCurrentMouse() const noexcept {
			return mouse[currentMouseIndex];
		}
	};
}

export namespace System::Application::Windows {
	using namespace DirectInputConstant;
	class DirectInputManager : public IInputManager {
		DirectInputState m_states[MaxPlayerCount];
		bool m_useBuffer = false;
	private:
		DirectInputManager() noexcept {
			GetInterface(GetModuleHandle(nullptr));
			AttachTopLevelWindow(nullptr, false, true);
		}
		~DirectInputManager() noexcept {
			DetachTopLevelWindow();
			IDirectInput8*& diInterface = GetInterface(nullptr);
			if (diInterface) diInterface->Release();
			diInterface = nullptr;
		}
	public:
		static DirectInputManager* CreateManager() noexcept {
			return new DirectInputManager();
		}
	private:
		static IDirectInput8*& GetInterface(HINSTANCE hInstance) noexcept {
			static IDirectInput8* ret = nullptr;
			if (ret) return ret;
			HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&ret), nullptr);
			if (hr != DI_OK) ret = nullptr;
			return ret;
		}
		static Vector<IDirectInputDevice8*>& GetDevices() noexcept {
			static Vector<IDirectInputDevice8*> ret;
			return ret;
		}
		static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCEW ipddi, LPVOID pvRef) {
			Vector<DIDEVICEINSTANCE>& deviceInfo = *reinterpret_cast<Vector<DIDEVICEINSTANCE>*>(pvRef);
			deviceInfo.Add(*ipddi);
			return DIENUM_CONTINUE;
		}
		static Vector<DIDEVICEINSTANCE> EnumDeviceInstancesForGameController(IDirectInput8& diInterface) noexcept {
			Vector<DIDEVICEINSTANCE> deviceInfo;
			HRESULT hr = diInterface.EnumDevices(
				DI8DEVCLASS_GAMECTRL,
				DeviceFindCallBack,
				&deviceInfo,
				DIEDFL_ATTACHEDONLY
			);
			return deviceInfo;
		}
		static Vector<DIDEVICEINSTANCE> EnumDeviceInstancesForKeyboard(IDirectInput8& diInterface) noexcept {
			Vector<DIDEVICEINSTANCE> deviceInfo;
			HRESULT hr = diInterface.EnumDevices(
				DI8DEVCLASS_KEYBOARD,
				DeviceFindCallBack,
				&deviceInfo,
				DIEDFL_ATTACHEDONLY
			);
			return deviceInfo;
		}
		static Vector<DIDEVICEINSTANCE> EnumDeviceInstancesForMouse(IDirectInput8& diInterface) noexcept {
			Vector<DIDEVICEINSTANCE> deviceInfo;
			HRESULT hr = diInterface.EnumDevices(
				DI8DEVTYPE_MOUSE,
				DeviceFindCallBack,
				&deviceInfo,
				DIEDFL_ATTACHEDONLY
			);
			return deviceInfo;
		}
		static String GetDeviceName(IDirectInputDevice8& device, bool quate = false) noexcept {
			DIDEVICEINSTANCE instance;
			instance.dwSize = sizeof(instance);
			HRESULT hr = device.GetDeviceInfo(&instance);
			if (hr != DI_OK) return String();
			else if (quate) return String::Joint(u"\"", instance.tszInstanceName, u"\"");
			else return String(instance.tszInstanceName);
		}
		static nullptr_t SafeRelease(IDirectInputDevice8*& device, const String& log = String()) noexcept {
			if (device) device->Release();
			WinAPI::OutputDebugString(log.w_str());
			return nullptr;
		}
	private:
		static IDirectInputDevice8* CreateJoyStickDevice(IDirectInput8& diInterface, const DIDEVICEINSTANCE& deviceInstance) noexcept {
			IDirectInputDevice8* device = nullptr;
			HRESULT hr = diInterface.CreateDevice(deviceInstance.guidInstance, &device, nullptr);
			if (hr != DI_OK) return nullptr;
			//JOYSTICKとして使用
			hr = device->SetDataFormat(&c_dfDIJoystick);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"はJoystickとして使用できません。"));
			//軸モード設定
			DIPROPDWORD diprop{};
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.diph.dwObj = 0;
			diprop.dwData = IsAbsAxis ? DIPROPAXISMODE_ABS : DIPROPAXISMODE_REL;
			hr = device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"に軸モードを設定できません。"));
			//デッドゾーン設定
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.diph.dwObj = 0;
			diprop.dwData = 2000;	//20%
			hr = device->SetProperty(DIPROP_DEADZONE, &diprop.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にデッドゾーンを設定できません。"));
			//軸の値の範囲設定
			DIPROPRANGE range{};
			range.diph.dwSize = sizeof(range);
			range.diph.dwHeaderSize = sizeof(range.diph);
			range.diph.dwHow = DIPH_BYOFFSET;
			range.diph.dwObj = DIJOFS_X;
			range.lMin = -XAxisAbs;
			range.lMax = XAxisAbs;
			hr = device->SetProperty(DIPROP_RANGE, &range.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にスティック1のX軸の範囲を設定できません。"));
			range.diph.dwObj = DIJOFS_Y;
			range.lMin = -YAxisAbs;
			range.lMax = YAxisAbs;
			hr = device->SetProperty(DIPROP_RANGE, &range.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にスティック1のY軸の範囲を設定できません。"));
			//DualShock4では右スティックのX軸がZ軸、Y軸がZ回転に割り当てられている
			range.diph.dwObj = DIJOFS_Z;
			range.lMin = -XAxisAbs;
			range.lMax = XAxisAbs;
			hr = device->SetProperty(DIPROP_RANGE, &range.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にスティック2のX軸の範囲を設定できません。"));
			range.diph.dwObj = DIJOFS_RZ;
			range.lMin = -YAxisAbs;
			range.lMax = YAxisAbs;
			hr = device->SetProperty(DIPROP_RANGE, &range.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にスティック2のY軸の範囲を設定できません。"));
			//バッファサイズ設定
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = BufferCount + 1;//バッファ数は設定した数より1少なくなるため
			hr = device->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にバッファサイズを設定できません。"));
			return device;
		}
		static IDirectInputDevice8* CreateKeyboardDevice(IDirectInput8& diInterface, const DIDEVICEINSTANCE& deviceInstance) noexcept {
			IDirectInputDevice8* device = nullptr;
			HRESULT hr = diInterface.CreateDevice(deviceInstance.guidInstance, &device, nullptr);
			if (hr != DI_OK) return nullptr;
			//Keyboardとして使用
			hr = device->SetDataFormat(&c_dfDIKeyboard);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"はKeyBoardとして使用できません。"));
			//バッファサイズ設定
			DIPROPDWORD diprop{};
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = BufferCount + 1;//バッファ数は設定した数より1少なくなるため
			hr = device->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にバッファサイズを設定できません。"));
			return device;
		}
		static IDirectInputDevice8* CreateMouseDevice(IDirectInput8& diInterface, const DIDEVICEINSTANCE& deviceInstance) noexcept {
			IDirectInputDevice8* device = nullptr;
			HRESULT hr = diInterface.CreateDevice(deviceInstance.guidInstance, &device, nullptr);
			if (hr != DI_OK) return nullptr;
			//Mouseとして使用
			hr = device->SetDataFormat(&c_dfDIMouse);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"はMouseとして使用できません。"));
			//バッファサイズ設定
			DIPROPDWORD diprop{};
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = BufferCount + 1;//バッファ数は設定した数より1少なくなるため
			hr = device->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
			if (hr != DI_OK) return SafeRelease(device, String::Joint(GetDeviceName(*device, true), u"にバッファサイズを設定できません。"));
			return device;
		}
	private:
		static bool SetCooperativeLevel(IDirectInputDevice8& device, HWND hWnd, bool foregroundOnly, bool nonExclusive) noexcept {
			return device.SetCooperativeLevel(
				hWnd,
				(foregroundOnly ? DISCL_FOREGROUND : DISCL_BACKGROUND) | (nonExclusive ? DISCL_NONEXCLUSIVE : DISCL_EXCLUSIVE)
			) == DI_OK;
		}
		static HWND CurrentTopLevelWindowHandle(bool set = false, HWND hWnd = nullptr) noexcept {
			static HWND ret = nullptr;
			if (set) ret = hWnd;
			return ret;
		}
	private:
		static DirectInputEventState GetState_Internal(DirectInputState& player, ControllerButtons button, Vector3& out, bool& isVector) noexcept {
			DirectInputEventState ret = DirectInputEventState::Empty;
			out = Vector3();
			isVector = false;
			DirectInputButtons diButton = ToDirectInputButtons(button);
			if (diButton != DirectInputButtons::None) {
				ret = player.GetButton(diButton);
				out.x = (ret == DirectInputEventState::Started) || (ret == DirectInputEventState::Performed) ? 1.f : 0.f;
			}
			else {
				DirectInputValues diValue = ToDirectInputValues(button);
				if (diValue != DirectInputValues::None) {
					ret = player.GetValue(diValue, out);
					isVector = true;
				}
				else {
					uint8_t diKey = ToDirectInputKeys(button);
					if (diKey != 0) ret = player.GetKey(diKey);
					out.x = (ret == DirectInputEventState::Started) || (ret == DirectInputEventState::Performed) ? 1.f : 0.f;
				}
			}
			return ret;
		}
	private:
		void UpdateDevice(IDirectInputDevice8& device) noexcept {
			DirectInputState& state = m_states[0];
			DIDEVICEINSTANCE instance{};
			instance.dwSize = sizeof(instance);
			HRESULT hr = device.GetDeviceInfo(&instance);
			if (hr != DI_OK) return;
			auto UpdateJoystick = [](IDirectInputDevice8& device, DirectInputState& state) {
				DIJOYSTATE& joystick = state.GetCurrentJoystick();
				HRESULT hr = device.GetDeviceState(sizeof(DIJOYSTATE), &joystick);
				if (hr != DI_OK) {
					device.Acquire();
					device.Poll();
					hr = device.GetDeviceState(sizeof(DIJOYSTATE), &joystick);
					if (hr != DI_OK) joystick = DIJOYSTATE{};
				}
			};
			auto UpdateKeyboard = [](IDirectInputDevice8& device, DirectInputState& state) {
				uint8_t(&keyboard)[256] = state.GetCurrentKeyboard();
				HRESULT hr = device.GetDeviceState(256, keyboard);
				if (hr != DI_OK) {
					device.Acquire();
					device.Poll();
					hr = device.GetDeviceState(256, keyboard);
					if (hr != DI_OK) {
						uint64_t* ptr = reinterpret_cast<uint64_t*>(keyboard);
						for (int i = 0; i < 32; ++i) ptr[i] = 0;
					}
				}
			};
			auto UpdateMouse = [](IDirectInputDevice8& device, DirectInputState& state) {
				DIMOUSESTATE& mouse = state.GetCurrentMouse();
				HRESULT hr = device.GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
				if (hr != DI_OK) {
					device.Acquire();
					device.Poll();
					hr = device.GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
					if (hr != DI_OK) mouse = DIMOUSESTATE{};
				}
			};
			void(*UpdateFunc)(IDirectInputDevice8&, DirectInputState&) = nullptr;
			switch (instance.dwDevType & 0xff) {
			//DualShockのデバイスタイプは1stPersonなので必要
			case DI8DEVTYPE_1STPERSON:
			case DI8DEVTYPE_JOYSTICK:
			case DI8DEVTYPE_GAMEPAD:
				UpdateFunc = UpdateJoystick;
				break;
			case DI8DEVTYPE_KEYBOARD:
				UpdateFunc = UpdateKeyboard;
				break;
			case DI8DEVTYPE_MOUSE:
				UpdateFunc = UpdateMouse;
				break;
			default:
				break;
			}
			if (!UpdateFunc) return;
			UpdateFunc(device, state);
		}
		void UpdateDeviceByBuffer(IDirectInputDevice8& device) noexcept {
			DirectInputState& state = m_states[0];
			DIDEVICEINSTANCE instance{};
			instance.dwSize = sizeof(instance);
			HRESULT hr = device.GetDeviceInfo(&instance);
			if (hr != DI_OK) return;
			auto UpdateJoystick = [](DirectInputState& state, const DIDEVICEOBJECTDATA& data) {
				DIJOYSTATE& joystick = state.GetCurrentJoystick();
				if (offsetof(DIJOYSTATE, rgbButtons) <= data.dwOfs && data.dwOfs < offsetof(DIJOYSTATE, rgbButtons) + sizeof(DIJOYSTATE::rgbButtons)) {
					uint8_t* dst = reinterpret_cast<uint8_t*>(&joystick) + data.dwOfs;
					*dst = data.dwData & 0x80 ? 0x80 : 0x00;
				}
				else if (offsetof(DIJOYSTATE, rgdwPOV) <= data.dwOfs && data.dwOfs < offsetof(DIJOYSTATE, rgdwPOV) + sizeof(DIJOYSTATE::rgdwPOV)) {
					DWORD* dst = reinterpret_cast<DWORD*>(reinterpret_cast<uint8_t*>(&joystick) + data.dwOfs);
					*dst = data.dwData;
				}
				else if (offsetof(DIJOYSTATE, rglSlider) <= data.dwOfs && data.dwOfs < offsetof(DIJOYSTATE, rglSlider) + sizeof(DIJOYSTATE::rglSlider)) {
					LONG* dst = reinterpret_cast<LONG*>(reinterpret_cast<uint8_t*>(&joystick) + data.dwOfs);
					*dst = static_cast<LONG>(data.dwData);
				}
				else {
					switch (data.dwOfs) {
					case offsetof(DIJOYSTATE, lX):
						joystick.lX = static_cast<decltype(DIJOYSTATE::lX)>(data.dwData);
						break;
					case offsetof(DIJOYSTATE, lY):
						joystick.lY = static_cast<decltype(DIJOYSTATE::lY)>(data.dwData);
						break;
					case offsetof(DIJOYSTATE, lZ):
						joystick.lZ = static_cast<decltype(DIJOYSTATE::lZ)>(data.dwData);
						break;
					case offsetof(DIJOYSTATE, lRx):
						joystick.lRx = static_cast<decltype(DIJOYSTATE::lRx)>(data.dwData);
						break;
					case offsetof(DIJOYSTATE, lRy):
						joystick.lRy = static_cast<decltype(DIJOYSTATE::lRy)>(data.dwData);
						break;
					case offsetof(DIJOYSTATE, lRz):
						joystick.lRz = static_cast<decltype(DIJOYSTATE::lRz)>(data.dwData);
						break;
					default:
						break;
					}
				}
			};
			auto UpdateKeyboard = [](DirectInputState& state, const DIDEVICEOBJECTDATA& data) {
				uint8_t(&keyboard)[256] = state.GetCurrentKeyboard();
				if (data.dwOfs < 256) keyboard[data.dwOfs] = data.dwData & 0x80 ? 0x80 : 0x00;
			};
			auto UpdateMouse = [](DirectInputState& state, const DIDEVICEOBJECTDATA& data) {
				DIMOUSESTATE& mouse = state.GetCurrentMouse();
				if (offsetof(DIMOUSESTATE, rgbButtons) <= data.dwOfs && data.dwOfs < offsetof(DIMOUSESTATE, rgbButtons) + sizeof(DIMOUSESTATE::rgbButtons)) {
					uint8_t* dst = reinterpret_cast<uint8_t*>(&mouse) + data.dwOfs;
					*dst = data.dwData & 0x80 ? 0x80 : 0x00;
				}
				else {
					switch (data.dwOfs) {
					case offsetof(DIMOUSESTATE, lX):
						mouse.lX += static_cast<decltype(DIMOUSESTATE::lX)>(data.dwData);
						break;
					case offsetof(DIMOUSESTATE, lY):
						mouse.lY += static_cast<decltype(DIMOUSESTATE::lY)>(data.dwData);
						break;
					case offsetof(DIMOUSESTATE, lZ):
						mouse.lZ += static_cast<decltype(DIMOUSESTATE::lZ)>(data.dwData);
						break;
					default:
						break;
					}
				}
			};
			void(*UpdateFunc)(DirectInputState&, const DIDEVICEOBJECTDATA&) = nullptr;
			switch (instance.dwDevType & 0xff) {
			//DualShockのデバイスタイプは1stPersonなので必要
			case DI8DEVTYPE_1STPERSON:
			case DI8DEVTYPE_JOYSTICK:
			case DI8DEVTYPE_GAMEPAD:
				UpdateFunc = UpdateJoystick;
				break;
			case DI8DEVTYPE_KEYBOARD:
				UpdateFunc = UpdateKeyboard;
				break;
			case DI8DEVTYPE_MOUSE: {
				//DIMOUSESTATEの軸は相対運動のため、毎フレーム初期化する
				DIMOUSESTATE& mouse = state.GetCurrentMouse();
				mouse.lX = 0;
				mouse.lY = 0;
				mouse.lZ = 0;
				UpdateFunc = UpdateMouse;
				break;
			}
			default:
				break;
			}
			if (!UpdateFunc) return;
			do {
				DIDEVICEOBJECTDATA data;
				//取得する最大バッファ数(関数呼び出し後は取得したバッファ数)
				DWORD bufferCount = 1;
				//第4引数をDIGDD_PEEKにすると、バッファが削除されない(今回は0を指定して削除する)
				hr = device.GetDeviceData(sizeof(data), &data, &bufferCount, 0);
				if (FAILED(hr) || !bufferCount) break;
				if (hr == DI_BUFFEROVERFLOW) {
					WinAPI::OutputDebugString(
						String::Joint(GetDeviceName(device, true), u"がバッファオーバーフローを起こしています。\n").w_str()
					);
				}
				UpdateFunc(state, data);
			} while (true);
		}
	public:
		bool AttachTopLevelWindow(HWND hWnd, bool foregroundOnly = true, bool nonExclusive = true) noexcept {
			DetachTopLevelWindow();
			IDirectInput8* diInterface = GetInterface(nullptr);
			if (!diInterface) return false;
			Vector<IDirectInputDevice8*>& devices = GetDevices();
			Vector<DIDEVICEINSTANCE> gameControllers = EnumDeviceInstancesForGameController(*diInterface);
			Vector<DIDEVICEINSTANCE> keyboards = EnumDeviceInstancesForKeyboard(*diInterface);
			Vector<DIDEVICEINSTANCE> mouses = EnumDeviceInstancesForMouse(*diInterface);
			devices.Reserve(gameControllers.Count() + keyboards.Count() + mouses.Count());
			for (DIDEVICEINSTANCE& instance : gameControllers) {
				IDirectInputDevice8* device = CreateJoyStickDevice(*diInterface, instance);
				if (!device) continue;
				SetCooperativeLevel(*device, hWnd, foregroundOnly, nonExclusive);
				device->Poll();
				devices.Add(device);
			}
			for (DIDEVICEINSTANCE& instance : keyboards) {
				IDirectInputDevice8* device = CreateKeyboardDevice(*diInterface, instance);
				if (!device) continue;
				SetCooperativeLevel(*device, hWnd, foregroundOnly, nonExclusive);
				device->Poll();
				devices.Add(device);
			}
			for (DIDEVICEINSTANCE& instance : mouses) {
				IDirectInputDevice8* device = CreateMouseDevice(*diInterface, instance);
				if (!device) continue;
				SetCooperativeLevel(*device, hWnd, foregroundOnly, nonExclusive);
				device->Poll();
				devices.Add(device);
			}
			const size_t deviceCount = devices.Count();
			CurrentTopLevelWindowHandle(true, hWnd);
			return true;
		}
		bool DetachTopLevelWindow() noexcept {
			Vector<IDirectInputDevice8*>& devices = GetDevices();
			for (IDirectInputDevice8* device : devices) {
				device->Unacquire();
				device->Release();
			}
			devices.Clear();
			for (uint8_t i = 0; i < MaxPlayerCount; ++i) m_states[i].Clear();
			CurrentTopLevelWindowHandle(true, nullptr);
			return true;
		}
	public:
		bool Update() noexcept override {
			Vector<IDirectInputDevice8*>& devices = GetDevices();
			const size_t deviceCount = devices.Count();
			DirectInputState& state = m_states[0];
			state.Update();
			for (size_t i = 0; i < deviceCount; ++i) {
				IDirectInputDevice8*& device = devices[i];
				//ウィンドウのフォーカスが切り替わるたびに取得しなおす必要があるため、毎ループ呼び出しておく
				//(Acquire関数は呼び出し不要な場合に何もせずS_FALSEを返す)
				HRESULT hr = device->Acquire();
				if (hr != DI_OK && hr != S_FALSE) continue;
				if (m_useBuffer) UpdateDeviceByBuffer(*device);
				else UpdateDevice(*device);
			}
			return deviceCount;
		}
		InputState GetState(InputValueType type, const InputBinding& binding) noexcept override {
			DirectInputState& player = m_states[0 % MaxPlayerCount];
			InputState ret{};
			DirectInputEventState ev = DirectInputEventState::Empty;
			if (binding.Type == InputBindingType::Simple) {
				Vector3 out;
				bool isVector;
				ev = GetState_Internal(player, binding.Button, out, isVector);
				if (type == InputValueType::Button) ret.Pressed = isVector ? out.SqrMagnitude() > 0.1f : out.x != 0.f;
				else ret.XYZ = out;
			}
			else if (binding.Type == InputBindingType::Composite) {
				Vector3 out;
				bool isVector;
				if (type == InputValueType::Button) {
					ev = GetState_Internal(player, binding.Button, out, isVector);
					ret.Pressed = out.SqrMagnitude() > 0.1f;
				}
				else {
					DirectInputEventState evs[6]{};
					evs[0] = GetState_Internal(player, binding.Up, out, isVector);
					ret.XYZ.y = Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
					evs[1] = GetState_Internal(player, binding.Down, out, isVector);
					ret.XYZ.y -= Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
					evs[2] = GetState_Internal(player, binding.Left, out, isVector);
					ret.XYZ.x = Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
					evs[3] = GetState_Internal(player, binding.Right, out, isVector);
					ret.XYZ.x -= Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
					if (type == InputValueType::Vector3) {
						evs[4] = GetState_Internal(player, binding.Forward, out, isVector);
						ret.XYZ.z = Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
						evs[5] = GetState_Internal(player, binding.Backward, out, isVector);
						ret.XYZ.z -= Math::Clamp(isVector ? out.Magnitude() : Math::Abs(out.x), 0.f, 1.f);
					}
					for (size_t i = 0; i < 6; ++i) {
						switch (evs[i]) {
						case  DirectInputEventState::Started:
							ev = DirectInputEventState::Started;
							break;
						case DirectInputEventState::Performed:
							ev = DirectInputEventState::Performed;
							break;
						case DirectInputEventState::Canceled:
							if (ev == DirectInputEventState::Empty) ev = DirectInputEventState::Canceled;
							break;
						default:
							break;
						}
						if (ev == DirectInputEventState::Performed) break;
					}

				}
			}
			ret.Started = ev == DirectInputEventState::Started;
			ret.Performed = ret.Started || (ev == DirectInputEventState::Performed);
			ret.Canceled = ev == DirectInputEventState::Canceled;
			return ret;
		}
		bool SetTopLevelWindow(const WindowPlatformData& data) noexcept override {
			if (data.Type == WindowType::Windows) {
				HWND hWnd = static_cast<HWND>(data.Data.GetData());
				return AttachTopLevelWindow(hWnd, hWnd ? true : false);
			}
			else if (data.Type == WindowType::SDL) {
				return false;
			}
			else return false;
		}
		Point<int32_t> GetCursorPosOnScreen() noexcept override {
			POINT p;
			if (GetCursorPos(&p)) return Point<int32_t>{ p.x, p.y };
			else return Point<int32_t>{};
		}
		Point<int32_t> GetCursorPosOnWindow() noexcept override {
			POINT p;
			if (!GetCursorPos(&p)) return Point<int32_t>{};
			if (ScreenToClient(CurrentTopLevelWindowHandle(), &p)) return Point<int32_t>{ p.x, p.y };
			else return Point<int32_t>{};
		}
	};
}
