export module Exception;
import Traits;
import StringBlock;
import <exception>;//std::exception, std::terminate()
import <stdexcept>;//std::runtime_error
import <source_location>;//std::source_location

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

//例外メッセージ生成関数
namespace System {
	template<Traits::Concepts::CCharType T>
	StringBlock<char> CreateExceptionMessage(const char* title, const char* funcsig, T const* message, const char* filename, const StringBlock<char>& lineNumberStr) noexcept {
		StringBlock<char> strs[6] = {
			StringBlock<char>{ title },
			StringBlock<char>{ StringBlock<T>(message).ToMultiByteStringBlock() },
			StringBlock<char>{ "[関数名]" },
			StringBlock<char>{ funcsig },
			filename ? StringBlock<char>{ "[ファイル名]" } + StringBlock<char>{ filename } : StringBlock<char>{},
			lineNumberStr
		};
		size_t length[6];
		size_t sumLength = 0;
		for (int i = 0; i < 6; ++i){
			length[i] = strs[i].Length();
			sumLength += length[i];
		}
		StringBlock<char> ret = StringBlock<char>::CreateStringBlock(sumLength + 1);
		size_t current = 0;
		for (int i = 0; i < 6; ++i){
			ret.strcpy(strs[i].c_str(), length[i], current);
			current += length[i];
		}
		return ret;
	}
	template<Traits::Concepts::CCharType T>
	StringBlock<char> CreateExceptionMessage(const char* title, const char* funcsig, T const* message, const char* filename, int lineNumber) noexcept{
		return CreateExceptionMessage<T>(
			title, funcsig, message, filename,
			lineNumber >= 0
			? StringBlock<char>{ " " } + StringBlock<char>{ GetCStringBlock<char>(lineNumber) } + StringBlock<char>{ "行目" }
			: StringBlock<char>{}
		);
	}
	template<Traits::Concepts::CCharType T>
	StringBlock<char> CreateExceptionMessage(const char* title, const char* funcsig, T const* message, const std::source_location& loc) noexcept {
		auto lineNumber = loc.line();
		return CreateExceptionMessage<T>(
			title, funcsig, message, loc.file_name(),
			lineNumber >= 0
			? StringBlock<char>{ " " } + StringBlock<char>{ GetCStringBlock<char>(lineNumber) } + StringBlock<char>{ "行目(呼び出し元)" }
			: StringBlock<char>{}
		);
	}
}

//Exception
export namespace System {
	class Exception : public std::runtime_error {
	protected:
		using str = StringBlock<char>;
	public:
		Exception() noexcept
		    : Exception("") {}
		Exception(const Exception& arg) noexcept
		    : std::runtime_error(static_cast<std::runtime_error const&>(arg)) {}
		Exception(Exception&&) noexcept = delete;
		virtual ~Exception() noexcept {}
	public: /* runtime_errorコンストラクタ */
		explicit Exception(const char* message) noexcept
		    : std::runtime_error(message) {}
		explicit Exception(const std::string& message) noexcept
		    : std::runtime_error(message) {}
		Exception(const std::runtime_error& error) noexcept
			: std::runtime_error(error) {}
	public: /* __FUNCSIG__、例外メッセージ、__FILE__、__LINE__を受け取るコンストラクタ群  */
		template<Traits::Concepts::CCharType T>
		Exception(const char* funcsig, T const* message, const char* filename = nullptr, int lineNumber = -1) noexcept
		    : std::runtime_error(CreateExceptionMessage("例外メッセージ：", funcsig, message, filename, lineNumber).c_str()) {}
		template<Traits::Concepts::CCharType T>
		Exception(const char* funcsig, T const* message, const std::source_location& loc) noexcept
		    : std::runtime_error(CreateExceptionMessage("例外メッセージ：", funcsig, message, loc).c_str()) {}
	public: /* whatはそのまま使用する */
		const char* what() const noexcept override {
			return std::runtime_error::what();
		}
	public:
		Exception& operator=(const Exception& rhs) noexcept {
			if (this == &rhs) return *this;
			std::runtime_error::operator=(static_cast<const std::runtime_error&>(rhs));
			return *this;
		}
		Exception& operator=(Exception&&) noexcept = delete;
	};
}

//LogicException
export namespace System {
	class LogicException : public std::logic_error {
	protected:
		using str = StringBlock<char>;
	public:
		LogicException() noexcept : std::logic_error("未定義の論理例外メッセージです。") {}
		LogicException(const LogicException&) noexcept = default;
		~LogicException() noexcept = default;
	public:
		LogicException(const char* message) noexcept
		    : std::logic_error(message) {}
		LogicException(const std::string& message) noexcept
		    : std::logic_error(message) {}
	public:
		template<Traits::Concepts::CCharType T>
		LogicException(const char* funcsig, T const* message, const char* filename = nullptr, int lineNumber = -1) noexcept
		    : std::logic_error(CreateExceptionMessage("論理例外メッセージ：", funcsig, message, filename, lineNumber).c_str()) {}
		template<Traits::Concepts::CCharType T>
		LogicException(const char* funcsig, T const* message, const std::source_location& loc) noexcept
		    : std::logic_error(CreateExceptionMessage("論理例外メッセージ：", funcsig, message, loc).c_str()) {}
	public:
		using std::logic_error::what;
	};
}

//InvalidOperationException
export namespace System {
	class InvalidOperationException : public Exception {
	public:
		InvalidOperationException() noexcept : Exception("") {}
		InvalidOperationException(const InvalidOperationException& arg) noexcept
			: Exception(static_cast<const Exception&>(arg)) {}
		InvalidOperationException(InvalidOperationException&&) noexcept = delete;
		virtual ~InvalidOperationException() noexcept {}
	public: /* runtime_errorコンストラクタ */
		explicit InvalidOperationException(const char* message) noexcept
		    : Exception(message) {}
		explicit InvalidOperationException(const std::string& message) noexcept
		    : Exception(message) {}
		InvalidOperationException(const std::runtime_error& error) noexcept
		    : Exception(error) {}
	public: /* __FUNCSIG__、例外メッセージ、__FILE__、__LINE__を受け取るコンストラクタ群  */
		template<Traits::Concepts::CCharType T>
		InvalidOperationException(const char* funcsig, T const* message, const char* filename = nullptr, int lineNumber = -1) noexcept
		    : Exception(CreateExceptionMessage("無効操作例外メッセージ：", funcsig, message, filename, lineNumber).c_str()) {}
		template<Traits::Concepts::CCharType T>
		InvalidOperationException(const char* funcsig, T const* message, const std::source_location& loc) noexcept
		    : Exception(CreateExceptionMessage("無効操作例外メッセージ：", funcsig, message, loc).c_str()) {}
	public:
		const char* what() const noexcept override {
			return Exception::what();
		}
	public:
		InvalidOperationException& operator=(const InvalidOperationException& rhs) noexcept {
			if (this == &rhs) return *this;
			Exception::operator=(static_cast<const Exception&>(rhs));
			return *this;
		}
		InvalidOperationException& operator=(InvalidOperationException&&) noexcept = delete;
	};
}

//Terminate Functions
export namespace System {
	void Terminate(const LogicException& ex = LogicException("メッセージは未設定です。")) noexcept;
	void Terminate(const Exception& ex) noexcept;
}
