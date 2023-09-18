module Log;
import Thread;
import Objects;
#if !defined(SDL)

#if defined(_WIN32) || defined(_WIN64)
import WinAPI;
#else	//_WIN32 or _WIN64
#endif	//_WIN32 or _WIN64

#else	//SDL
#endif	//SDL

namespace System::Application {
	static Mutex& GetLogMutex() noexcept {
		static Mutex ret;
		return ret;
	}
}

#if !defined(SDL)

#if defined(_WIN32) || defined(_WIN64)
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
#else	//WindowsでもSDLでもない場合
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
