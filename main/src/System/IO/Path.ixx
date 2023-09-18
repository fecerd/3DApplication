export module Path;
import Objects;
import Traits;
import <fstream>;
using namespace System::Traits;

export namespace System::IO {
	template<class T>
	concept ConvertibleToString = requires() {
		String(declval<T>());
		requires !Traits::Concepts::CSame<Traits::remove_cvref_t<T>, String>;
	};

	class Path : public Object {
		struct Constant {
			static const String& UnixSeparator() noexcept {
				static const String ret = u"/";
				return ret;
			}
			static const String& WindowsSeparator() noexcept {
				static const String ret = u"\\";
				return ret;
			}			
			static const String& Separator() noexcept;
			static const String& RelativeUnixSeparator() noexcept {
				static const String ret = u"./";
				return ret;
			}
			static const String& RelativeWindowsSeparator() noexcept {
				static const String ret = u".\\";
				return ret;
			}
			static const String& RelativeSeparator() noexcept;
		};
	private:
		String m_data;
	public:
		Path() noexcept : m_data() {}
		Path(const String& arg) noexcept;
		template<ConvertibleToString T>
		Path(T const& arg) noexcept : Path(String(arg)) {}
		~Path() noexcept {}
	public:
		String Extension() const noexcept;
		bool Exists() const noexcept;
		const String& PathName() const noexcept;
		Path GetDirectory() const noexcept;
	public:
		Path CreatePath(const String& relativePath) const noexcept;
	public:
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const Path&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Path>(); }
		String ToString() const noexcept override { return m_data; }
		uint32_t GetTypeID() const noexcept override { return GetID<Path>(); }
	};
}