export module LayerMask;
import Module;
using namespace System;

export namespace Engine {
	enum class LayerMask : uint32_t {
		All = 0,
		Default = 0x00000001,
		Character = 0x00000002,
		UI = 0x00000004,
		Layer0 = 0,
		Layer1 = 1u << 0,
		Layer2 = 1u << 1,
		Layer3 = 1u << 2,
		Layer4 = 1u << 3,
		Layer5 = 1u << 4
	};
}
export namespace System::Traits {
	template<> struct enabling_bitwise_for_enum<Engine::LayerMask> : true_type {};
}
