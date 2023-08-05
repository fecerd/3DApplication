module Path;
using namespace System;

namespace System::IO {
	String Path::Extension() const noexcept {
		return m_data.substr(m_data.rfind(String(u'.')));
	}
	bool Path::Exists() const noexcept {
		try
		{
			std::fstream fs(m_data.Convert<wchar_t>().c_str());
			return fs.is_open();
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
	const String& Path::PathName() const noexcept { return m_data; }
	String Path::GetDirectory() const noexcept {
		const char16_t* cstr = m_data.c_str();
		const size_t length = m_data.Length();
		if (length > 0 && cstr[length - 1] == Constant::Separator()[0]) return m_data;
		bool ext = false;
		for (size_t i = length; i-- > 0;) {
			if (cstr[i] == u'.') ext = true;
			else if (cstr[i] == Constant::Separator()[0]) {
				if (ext) return m_data.substr(0, i + 1);
				else return String::Joint(m_data, Constant::Separator());
			}
		}
		return String();
	}
	Path Path::CreatePath(const String& relativePath) const noexcept {
		if (m_data.IsNullOrEmpty()) return Path();	//currentはパスでない
		if (relativePath.IsNullOrEmpty()) return *this;
		const String directory = GetDirectory();
		const String relative = relativePath.Replace(Constant::UnixSeparator(), Constant::Separator());
		size_t rpos = 0;
		size_t dpos = directory.Length();
		do {
			size_t tmp = relative.find(Constant::RelativeSeparator(), rpos);
			if (tmp == String::npos) break;
			if (tmp != rpos) dpos = directory.rfind(Constant::Separator(), dpos - 1);
			rpos = tmp + 2;
			if (dpos == String::npos) return Path();
		} while (true);
		return directory.substr(0, dpos + 1) + relative.substr(rpos, String::npos);
	}
}