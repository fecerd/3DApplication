export module ResourcePath;
import Objects;
import Path;
using namespace System;

export namespace ResourcePaths {
	System::IO::Path RootPath;
}

export namespace ResourcePaths {
	String GetPathName(const String& relativePath) noexcept {
		return RootPath.CreatePath(relativePath).ToString();
	}
}