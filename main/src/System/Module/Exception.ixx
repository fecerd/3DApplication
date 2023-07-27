module;
#include<exception>
#include<stdexcept>
#include<source_location>
export module Exception;
import StringBlock;

//source_location
export namespace System {
	using source_location = std::source_location;
	using srcloc = source_location;
}

//std_exception
export namespace System {
	using std_exception = std::exception;
	using std_logic_error = std::logic_error;
	using std_runtime_error = std::runtime_error;
}

//Exception
export namespace System {
	class Exception : public std::runtime_error {
	protected:
		using str = StringBlock<char>;
	public:
		Exception() noexcept = default;
		Exception(const Exception&) noexcept = default;
		~Exception() noexcept = default;
	public:
		Exception(const char* message) noexcept : std::runtime_error(message) {}
		Exception(const std::string& message) noexcept : std::runtime_error(message) {}
	public:
		using std::runtime_error::what;
	public:
		Exception(const char* funcsig, const char* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: std::runtime_error(
				(str("例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		Exception(const char* funcsig, const char16_t* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: std::runtime_error(
				(str("例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		Exception(const char* funcsig, const char* message, const std::source_location& loc) noexcept
			: std::runtime_error(
				(str("例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
		Exception(const char* funcsig, const char16_t* message, const std::source_location& loc) noexcept
			: std::runtime_error(
				(str("例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
	};

}

//LogicException
export namespace System {
	class LogicException : public std::logic_error {
	protected:
		using str = StringBlock<char>;
	public:
		LogicException() noexcept = default;
		LogicException(const LogicException&) noexcept = default;
		~LogicException() noexcept = default;
	public:
		LogicException(const char* message) noexcept : std::logic_error(message) {}
		LogicException(const std::string& message) noexcept : std::logic_error(message) {}
	public:
		using std::logic_error::what;
	public:
		LogicException(const char* funcsig, const char* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: std::logic_error(
				(str("論理例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		LogicException(const char* funcsig, const char16_t* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: std::logic_error(
				(str("論理例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		LogicException(const char* funcsig, const char* message, const std::source_location& loc) noexcept
			: std::logic_error(
				(str("論理例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
		LogicException(const char* funcsig, const char16_t* message, const std::source_location& loc) noexcept
			: std::logic_error(
				(str("論理例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
	};

}

//InvalidOperationException
export namespace System {
	class InvalidOperationException : public Exception {
	public:
		InvalidOperationException(const char* funcsig, const char* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: Exception(
				(str("無効操作例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		InvalidOperationException(const char* funcsig, const char16_t* message, const char* filename = nullptr, int lineNumber = -1) noexcept
			: Exception(
				(str("無効操作例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ (filename ? str(" [ファイル名]") + str(filename) : str())
					+ (filename && lineNumber >= 0 ? str(" ") + str(GetCStringBlock<char>(lineNumber)) + str("行目") : str())
					).c_str()
			) {}
		InvalidOperationException(const char* funcsig, const char* message, const std::source_location& loc) noexcept
			: Exception(
				(str("無効操作例外メッセージ：")
					+ str(message)
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
		InvalidOperationException(const char* funcsig, const char16_t* message, const std::source_location& loc) noexcept
			: Exception(
				(str("無効操作例外メッセージ：")
					+ str(StringBlock<char16_t>(message).ToMultiByteStringBlock())
					+ str(" [関数名]")
					+ str(funcsig)
					+ str(" [ファイル名]") + str(loc.file_name())
					+ str(" ") + str(GetCStringBlock<char>(loc.line())) + str("行目(呼び出し元)")
					).c_str()
			) {}
	};
}

export namespace System {
	void Terminate(const LogicException& ex = LogicException("メッセージは未設定です。")) noexcept;
	void Terminate(const Exception& ex) noexcept;
}