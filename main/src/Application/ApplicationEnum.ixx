export module ApplicationEnum;
import System;

export namespace System::Application {
	enum class MessageBoxType : uint8_t {
		OK = 0,
		OKCancel,
		AbortRetryIgnore,
		YesNoCancel,
		YesNo,
		RetryCancel,
		CancelTryContinue
	};
	enum class MessageBoxResult : uint8_t {
		OK = 1,
		Cancel,
		Abort,
		Retry,
		Ignore,
		Yes,
		No,
		TryAgain,
		Continue
	};
}