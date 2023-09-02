export module TypeID;
import CSTDINT;
import XOR128;

export namespace System {
	XOR128& GetRNG() noexcept {
		static XOR128 ret;
		return ret;
	}
}

export namespace System {
	template<class T>
	uint32_t GetID() noexcept {
		static uint32_t ret = GetRNG().Get();
		return ret;
	}
}