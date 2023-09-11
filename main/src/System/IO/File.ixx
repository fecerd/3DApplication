export module File;
export import Objects;
export import Memory;
export import Path;
export import <fstream>;

//OpenMode
export namespace System::IO {
#undef IN
#undef OUT
	enum class OpenMode {
		BINARY = std::ios_base::binary,
		IN = std::ios_base::in,
		OUT = std::ios_base::out,
		ATE = std::ios_base::ate,
		APP = std::ios_base::app,
		TRUNC = std::ios_base::trunc,
		IN_BINARY = std::ios_base::in | std::ios_base::binary,
		OUT_BINARY = std::ios_base::out | std::ios_base::binary,
		IN_OUT = std::ios_base::in | std::ios_base::out,
		IN_OUT_BINARY = std::ios_base::in | std::ios_base::out | std::ios_base::binary
	};
	OpenMode operator&(OpenMode lhs, OpenMode rhs) noexcept { return static_cast<OpenMode>(static_cast<int>(lhs) & static_cast<int>(rhs)); }
}

//StreamPos, StreamOff, SeekDir, StreamState
export namespace System::IO {
	using StreamPos = std::streampos;
	using StreamOff = std::streamoff;
	using SeekDir = std::ios_base::seekdir;
	using StreamState = std::ios_base::iostate;
	namespace SeekDirs {
		SeekDir Begin = std::ios_base::beg;
		SeekDir Current = std::ios_base::cur;
		SeekDir End = std::ios_base::end;
	}
	namespace StreamStates {
		StreamState Good = std::ios_base::goodbit;
		StreamState Bad = std::ios_base::badbit;
		StreamState Eof = std::ios_base::eofbit;
		StreamState Fail = std::ios_base::failbit;
	}
}

//FileStream
export namespace System::IO {
	class FileStream : public Object {
		std::fstream m_file;
		OpenMode m_mode;
	public:
		FileStream() = delete;
		FileStream(const Path& path, OpenMode mode = OpenMode::IN_OUT) : m_mode(mode) {
			m_file.open(path.PathName().Convert<wchar_t>().c_str(), static_cast<std::ios_base::openmode>(m_mode));
		}
	public:
		FileStream(FileStream&& arg) noexcept : m_file(static_cast<std::fstream&&>(arg.m_file)), m_mode(arg.m_mode) {}
		~FileStream() { m_file.close(); }
	public:
		void Read(void* buffer, size_t count) noexcept { if (m_file.is_open()) m_file.read(reinterpret_cast<char*>(buffer), count); }
		template<class T> void ReadBE(T* dst, size_t count) noexcept { Memory::ReadBE(m_file, dst, count); }
		template<class T, size_t N> void ReadBE(T(&dst)[N]) noexcept { Memory::ReadBE(m_file, dst); }
		template<class T> void ReadBE(T& dst) noexcept { Memory::ReadBE(m_file, dst); }
		template<class T> void ReadLE(T* dst, size_t count) noexcept { Memory::ReadLE(m_file, dst, count); }
		template<class T, size_t N> void ReadLE(T(&dst)[N]) noexcept { Memory::ReadLE(m_file, dst); }
		template<class T> void ReadLE(T& dst) noexcept { Memory::ReadLE(m_file, dst); }
	public:
		void Write(const void* buffer, size_t count) noexcept { if (m_file.is_open()) m_file.write(reinterpret_cast<const char*>(buffer), count); }
		template<typename T> void WriteBE(const T& src) noexcept { Memory::WriteBE(m_file, src); }
		template<typename T, size_t N> void WriteBE(const T(&src)[N]) noexcept { Memory::WriteBE(m_file, src); }
		template<typename T> void WriteBE(const T* src, const size_t N) noexcept { Memory::WriteBE(m_file, src, N); }
		template<typename T> void WriteLE(const T& src) noexcept { Memory::WriteLE(m_file, src); }
		template<typename T, size_t N> void WriteLE(const T(&src)[N]) noexcept { Memory::WriteLE(m_file, src); }
		template<typename T> void WriteLE(const T* src, const size_t N) noexcept { Memory::WriteLE(m_file, src, N); }
	public:
		inline void Close() noexcept { m_file.close(); }
	public:
		void Seek(StreamPos pos) noexcept {
			if ((m_mode & OpenMode::IN) == OpenMode::IN) m_file.seekg(pos);
			else m_file.seekp(pos);
		}
		void Seek(StreamOff off, SeekDir dir) noexcept {
			m_file.clear();
			if ((m_mode & OpenMode::IN) == OpenMode::IN) m_file.seekg(off, dir);
			else m_file.seekp(off, dir);
		}
		StreamPos TellPos() noexcept {
			if ((m_mode & OpenMode::IN) == OpenMode::IN) return m_file.tellg();
			else return m_file.tellp();
		}
		/// <summary>
		/// 現在位置から指定した位置までのオフセットを取得する
		/// </summary>
		/// <param name="target">オフセット計算の移動先</param>
		StreamOff TellOff(StreamPos target) noexcept {
			if (!m_file.is_open()) return StreamOff(0);
			if (!m_file) Clear();
			StreamPos cur = TellPos();
			return target - cur;
		}
		/// <summary>
		/// 現在開いているファイルのバイトサイズを取得する
		/// </summary>
		size_t Size() noexcept {
			if (!m_file.is_open()) return 0;
			if (!m_file) Clear();
			StreamPos cur = TellPos();
			Seek(0, SeekDirs::End);
			StreamPos end = TellPos();
			if (!m_file) Clear();
			Seek(0, SeekDirs::Begin);
			StreamPos beg = TellPos();
			Seek(cur);
			return static_cast<size_t>(static_cast<std::streamsize>(end - beg));
		}
	public:
		inline void Clear(StreamState state = StreamStates::Good) noexcept { m_file.clear(state); }
		inline bool IsEOF() noexcept { return m_file.eof(); }
	public:
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const FileStream&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<FileStream>(); }
		String ToString() const noexcept override { return String(u"FileStream Object"); }
		uint32_t GetTypeID() const noexcept override { return GetID<FileStream>(); }
	public:
		inline explicit operator bool() const noexcept { return static_cast<bool>(m_file); }
		inline bool operator!() const noexcept { return !m_file; }
	};
}
