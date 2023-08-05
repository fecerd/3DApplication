export module Path;
import Objects;
import Traits;
import <fstream>;
using namespace System::Traits;

export namespace System::IO {
	template<class T>
	concept ConvertibleToString = requires() {
		String(declval<T>());
	};

	class Path : public Object {
		struct Constant {
			static const String& UnixSeparator() noexcept {
				static const String ret = u"/";
				return ret;
			}
			static const String& Separator() noexcept {
				static const String ret = u"\\";
				return ret;
			}
			static const String& RelativeSeparator() noexcept {
				static const String ret = u".\\";
				return ret;
			}
		};
	private:
		String m_data;
	public:
		Path() noexcept : m_data() {}
		template<ConvertibleToString T>
		Path(const T& arg) noexcept : m_data(String(arg).Replace(Constant::UnixSeparator(), Constant::Separator())) {}
		~Path() noexcept {}
	public:
		String Extension() const noexcept;
		bool Exists() const noexcept;
		const System::String& PathName() const noexcept;
		String GetDirectory() const noexcept;
	public:
		Path CreatePath(const String& relativePath) const noexcept;
	public:
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const Path&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Path>(); }
		String ToString() const noexcept override { return m_data; }
		uint32_t GetTypeID() const noexcept override { return GetID<Path>(); }
	};
}