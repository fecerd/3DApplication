module;
#include <memory.h>
#include <cstring>
module Memory;
import CSTDINT;

namespace System::Memory {
	inline constexpr bool MemcpyMode = 1;
	inline constexpr bool UseMemcpy = true;

	void Copy(void* dst, const void* src, size_t byteCount, bool unused) noexcept {
		if constexpr (MemcpyMode == 0) memcpy_s(dst, byteCount, src, byteCount);
		else if constexpr (MemcpyMode == 1) std::memcpy(dst, src, byteCount);
		else {
			const size_t count8 = byteCount >> 3;
			const size_t count1 = byteCount - (count8 << 3);
			if (count8) {
				const uint64_t* srcPtr = reinterpret_cast<const uint64_t*>(src);
				uint64_t* dstPtr = reinterpret_cast<uint64_t*>(dst);
				for (size_t i = 0; i < count8; ++i, ++srcPtr, ++dstPtr) *dstPtr = *srcPtr;
			}
			const uint8_t* srcPtr = reinterpret_cast<const uint8_t*>(src) + count8 * 8;
			uint8_t* dstPtr = reinterpret_cast<uint8_t*>(dst) + count8 * 8;
			for (size_t i = 0; i < count1; ++i, ++srcPtr, ++dstPtr) *dstPtr = *srcPtr;
		}
	}
}
