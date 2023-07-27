module Log;
import Thread;

namespace System::Application {
	static Mutex& GetLogMutex() noexcept {
		static Mutex ret;
		return ret;
	}
}

#ifndef SDL
#if defined(_WIN32) || defined(_WIN64)
import WinAPI;
namespace System::Application {
	void Log(const String& str) noexcept {
		LockGuard lock{ GetLogMutex() };
		WinAPI::OutputDebugString(str.w_str(), true);
	}
	void Log(size_t n) noexcept {
		LockGuard lock{ GetLogMutex() };
		WinAPI::OutputDebugStringI(n, true);
	}
}
#else	//WindowsÇ≈Ç‡SDLÇ≈Ç‡Ç»Ç¢èÍçá
namespace System::Application {
	void Log(const String& str) noexcept {
		LockGuard lock{ GetLogMutex() };
	}
	void Log(size_t n) noexcept {
		LockGuard lock{ GetLogMutex() };
	}
}
#endif
#else	//SDL
namespace System::Application {
	void Log(const String& str) noexcept {
		LockGuard lock{ GetLogMutex() };
	}
	void Log(size_t n) noexcept {
		LockGuard lock{ GetLogMutex() };
	}
}
#endif
