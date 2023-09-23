module XOR128;
import <chrono>;

namespace System {
	inline const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	
	XOR128 XOR128::GetRNG() noexcept {
		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::nanoseconds elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time);
		std::chrono::nanoseconds epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(start_time.time_since_epoch());

		unsigned long long count = static_cast<unsigned long long>(elapsed.count());
		unsigned long long start = static_cast<unsigned long long>(epoch.count());
		
		return XOR128 {
			static_cast<unsigned int>((start >> 32) & 0xffffffffu),
			static_cast<unsigned int>(start & 0xffffffffu),
			static_cast<unsigned int>((count >> 32) & 0xffffffffu),
			static_cast<unsigned int>(count & 0xffffffffu)
		};
	}

}
