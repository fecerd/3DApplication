export module EventArgs;
import System;
import ApplicationUtility;	//IPaintContext
using namespace System;

//EventArgs
export namespace System::Application {
	class EventArgs {
	public:
		int32_t State = 0;
	public:
		constexpr EventArgs() noexcept = default;
	public:
		static const EventArgs& Empty;
	};
}

//ClosingEventArgs
export namespace System::Application {
	class CloseEventArgs : public EventArgs {
	public:
		bool Closing = true;
	public:
		constexpr CloseEventArgs() noexcept = default;
	};
}

export namespace System::Application {
	enum class MouseButton {
		None = 0,
		Left = 1,
		Middle = 2,
		Right = 4,
		XButton1 = 8,
		XButton2 = 16,
		Ctrl = 32,
		Shift = 64
	};
}
template<> struct Traits::enabling_bitwise_for_enum<System::Application::MouseButton> : Traits::true_type {};

//MouseEventArgs
export namespace System::Application {
	class MouseEventArgs : public EventArgs {
	public:
		const int32_t X;
		const int32_t Y;
		const int32_t Delta;
		const MouseButton Button;
		const int32_t Clicks;
	public:
		constexpr MouseEventArgs(MouseButton button, int32_t clicks, int32_t x, int32_t y, int32_t delta) noexcept : Button(button), Clicks(clicks), X(x), Y(y), Delta(delta) {}
	public:
		System::Point<int32_t> Location() const noexcept {
			return System::Point<int32_t>(X, Y);
		}
	};
}

//PaintEventArgs
export namespace System::Application {
	class PaintEventArgs : public EventArgs {
	public:
		IPaintContext& Context;
		System::Drawing::Rectangle Rect;
	public:
		PaintEventArgs(IPaintContext& context, const System::Drawing::Rectangle& rect) noexcept : Context(context), Rect(rect) {}
	};
}

export namespace System::Application {
	enum class KeyCode {
		None = 0,
		Esc,
		Space = u' ',
		Shift,
		Left,
		Right,
		Up,
		Down,
		A = u'A',
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z
	};
}

//KeyEventArgs
export namespace System::Application {
	class KeyEventArgs : public EventArgs {
	public:
		KeyCode Code;
		uint16_t RepeatCount;
		bool Alt;
		bool KeyDown;
	public:
		constexpr KeyEventArgs() noexcept : KeyEventArgs(KeyCode::None, 1, false, true) {}
		constexpr KeyEventArgs(KeyCode code, uint16_t repeatCount, bool alt, bool keydown) noexcept
			: Code(code), RepeatCount(repeatCount), Alt(alt), KeyDown(keydown) {}
	};
}

//InputEventArgs
export namespace System::Application {
	class InputEventArgs {
	public:
		//const System::InputState Inputs = System::InputState();
	public:
		constexpr InputEventArgs() noexcept = default;
		//InputEventArgs(const System::InputState& inputs) noexcept : Inputs(inputs) {}
	};
}

export namespace System::Application {
	enum class ResizeType : uint8_t {
		UNDEFINED = 0,
		RESTORED,		//最小化でも最大化でもなく、サイズが変更された
		MINIMIZED,		//最小化された
		MAXIMIZED,	//最大化された
		MAXSHOW,		//他のウィンドウの最大化が解除された
		MAXHIDE			//他のウィンドウが最大化した
	};
}

//ResizeEventArgs
export namespace System::Application {
	class ResizeEventArgs : public EventArgs {
	public:
		const ResizeType Type;
		const int32_t Width;
		const int32_t Height;
	public:
		constexpr ResizeEventArgs(ResizeType type, int32_t width, int32_t height) noexcept
			: Type(type), Width(width), Height(height) {}
	};
}

//Static Member Impl
namespace System::Application {
	static constexpr EventArgs Dummy = EventArgs();
	const EventArgs& EventArgs::Empty = Dummy;
}