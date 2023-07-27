export module Log;
import Objects;

export namespace System::Application {
	void Log(const String& str) noexcept;
	void Log(size_t n) noexcept;
}
