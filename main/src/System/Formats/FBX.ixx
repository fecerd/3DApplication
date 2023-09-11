export module FBX;
import CSTDINT;
import Objects;
import CString;
import Vector;
import IO;
import Compression;

export namespace System::Formats {
	struct FBXArray {
		//要素数
		uint32_t m_count = 0;
		//エンコーディング。0: 非圧縮、1: zlib圧縮(zlibヘッダ含む)
		uint32_t m_encoding = 0;
		//続くデータのファイル上でのバイト数
		uint32_t m_byteSize = 0;
		union {
			bool* m_bool;
			int32_t* m_i32;
			int64_t* m_i64;
			float* m_f32;
			double* m_f64;
			uint8_t* m_read;
		};
	public:
		FBXArray() noexcept : m_bool(nullptr) {}
	public:
		bool Load(IO::FileStream& file, uint8_t typeCode) noexcept {
			file.ReadLE(m_count);
			file.ReadLE(m_encoding);
			file.ReadLE(m_byteSize);
			if (!m_count || !m_byteSize) return false;
			size_t typeSize = 0;
			switch (typeCode) {
			case 'b':
				typeSize = sizeof(*m_bool);
				break;
			case 'i':
				typeSize = sizeof(*m_i32);
				break;
			case 'l':
				typeSize = sizeof(*m_i64);
				break;
			case 'f':
				typeSize = sizeof(*m_f32);
				break;
			case 'd':
				typeSize = sizeof(*m_f64);
				break;
			default:
				typeSize = 0;
			}
			if (m_encoding && typeSize) {
				uint8_t* binary = new uint8_t[m_byteSize];
				file.Read(binary, m_byteSize);
				Compression::ZlibView zlib(binary, m_byteSize);
				uint8_t* data = zlib.GetCompressedData();
				if (!data) {
					delete[] binary;
					return false;
				}
				Vector<uint8_t> result = Compression::HuffmanDecoder::Decode(data, zlib.GetCompressedDataSize(), typeSize * m_count);
				m_read = result.Release();
				delete[] binary;
			}
			else {
				m_read = new uint8_t[m_byteSize];
				file.Read(m_read, m_byteSize);
			}
			return static_cast<bool>(file);
		}
		void Clear() noexcept {
			delete[] m_read;
			m_read = nullptr;
			m_count = 0;
			m_encoding = 0;
			m_byteSize = 0;
		}
	};

	struct FBXSpecialData {
		//続くデータのバイト数
		uint32_t m_length = 0;
		union {
			//utf-8文字列。(基本的に)ヌル終端しない
			char8_t* m_string;
			//バイナリデータ
			uint8_t* m_byte;
		};
	public:
		FBXSpecialData() noexcept : m_string(nullptr) {}
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(m_length);
			m_byte = new uint8_t[m_length];
			file.Read(m_byte, m_length);
			return static_cast<bool>(file);
		}
		String GetString() const noexcept {
			return String(m_string, m_length);
		}
		void Clear() noexcept {
			delete[] m_byte;
			m_byte = nullptr;
			m_length = 0;
		}
	};

	struct FBXProperty {
		uint8_t m_typeCode = 0;
		union {
			bool m_bool;
			int16_t m_i16;
			int32_t m_i32;
			int64_t m_i64;
			float m_f32;
			double m_f64;
			FBXArray m_array;
			FBXSpecialData m_special;
		};
	public:
		FBXProperty() noexcept : m_array() {}
		~FBXProperty() noexcept {
			switch (m_typeCode) {
			case 'C':
			case 'Y':
			case 'I':
			case 'L':
			case 'F':
			case 'D':
				break;
			case 'R':
			case 'S':
				m_special.Clear();
				break;
			default:
				m_array.Clear();
				break;
			}
			m_typeCode = 0;
		}
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(m_typeCode);
			switch (m_typeCode) {
			case 'C': {
				uint8_t tmp;
				file.ReadLE(tmp);
				//'Y'0x59がtrue、'T'0x54がfalseだが、0x01と0x00の場合もあるため、
				//奇数のときはtrueとする
				m_bool = tmp & 1;
				break;
			}
			case 'Y': 
				file.ReadLE(m_i16);
				break;
			case 'I':
				file.ReadLE(m_i32);
				break;
			case 'L':
				file.ReadLE(m_i64);
				break;
			case 'F':
				file.ReadLE(m_f32);
				break;
			case 'D':
				file.ReadLE(m_f64);
				break;
			case 'R':
			case 'S':
				return m_special.Load(file);
			default:
				return m_array.Load(file, m_typeCode);
			}
			return static_cast<bool>(file);
		}
	};

	struct FBXNode {
		//ファイル先頭からこのノードの次のノードの先頭へのオフセット
		uint64_t m_endOffset = 0;
		//属性数
		uint64_t m_numProperties = 0;
		//全属性の合計バイト数
		uint64_t m_propertyListLength = 0;
		//ノード名
		String m_name;
		//属性リスト[m_numProperties]
		FBXProperty* m_properties = nullptr;
		//子ノード
		Vector<FBXNode*> m_children;
	public:
		FBXNode() noexcept = default;
		~FBXNode() noexcept {
			delete[] m_properties;
			m_properties = nullptr;
			m_propertyListLength = 0;
			m_numProperties = 0;
			m_endOffset = 0;
			m_children.DeleteAll();
		}
	public:
		bool Load(IO::FileStream& file, uint32_t version, IO::StreamPos beg) noexcept {
			if (!file) return false;
			if (version <= 7400) {
				uint32_t tmp = 0;
				file.ReadLE(tmp);
				m_endOffset = tmp;
				file.ReadLE(tmp);
				m_numProperties = tmp;
				file.ReadLE(tmp);
				m_propertyListLength = tmp;
			}
			else {
				file.ReadLE(m_endOffset);
				file.ReadLE(m_numProperties);
				file.ReadLE(m_propertyListLength);
			}
			uint8_t nameLength = 0;
			file.ReadLE(nameLength);
			if (IsEndMarker()) return static_cast<bool>(file);
			char8_t* name = new char8_t[nameLength + 1];
			file.Read(name, nameLength);
			name[nameLength] = u8'\0';
			m_name = String(name, nameLength + 1);
			delete[] name;
			m_properties = new FBXProperty[m_numProperties];
			for (uint64_t i = 0; i < m_numProperties; ++i) {
				if (!m_properties[i].Load(file)) return false;
			}
			IO::StreamPos end = beg + static_cast<IO::StreamOff>(m_endOffset);
			while (end != file.TellPos()) {
				FBXNode* node = new FBXNode();
				if (!node->Load(file, version, beg)) break;
				m_children.Add(node);
				if (!file) break;
			}
			return static_cast<bool>(file);
		}
		bool IsEndMarker() const noexcept {
			return !m_endOffset && !m_numProperties && !m_propertyListLength && m_name.IsNullOrEmpty();
		}
		const FBXNode* GetChildByName(const String& name) const noexcept {
			for (FBXNode* node : m_children) {
				if (node->m_name == name) return node;
			}
			return nullptr;
		}
	};
}

export namespace System::Formats {
	struct FBXNodes {
		static constexpr CString FBXHeaderExtension = u"FBXHeaderExtension";
		static constexpr CString FileId = u"FileId";
		static constexpr CString CreationTime = u"CreationTime";
		static constexpr CString Creator = u"Creator";
		static constexpr CString GlobalSettings = u"GlobalSettings";
		static constexpr CString Documents = u"Documents";
		static constexpr CString References = u"References";
		static constexpr CString Definitions = u"Definitions";
		static constexpr CString Objects = u"Objects";
		static constexpr CString Connections = u"Connections";
		static constexpr CString Version = u"Version";
		static constexpr CString Properties70 = u"Properties70";
		static constexpr CString P = u"P";
	};
}

export namespace System::Formats {
	struct FBXGlobalSettings {
		int32_t UPAxis = -1;
		int32_t FrontAxis = -1;
		int32_t CoordAxis = -1;
		int32_t UpAxisSign = 0;
		int32_t FrontAxisSign = 0;
		int32_t CoordAxisSign = 0;
	};
}

export namespace System::Formats {
	class FBX {
		static constexpr char8_t MagicNumber[23] = u8"Kaydara FBX Binary  \x00\x1a";
	protected:
		uint32_t m_version = 0;
		Vector<FBXNode*> m_nodes;
	public:
		FBX() noexcept {}
		~FBX() noexcept {
			m_nodes.DeleteAll();
		}
	public:
		bool Load(const String& filePath) noexcept {
			IO::FileStream file(filePath, IO::OpenMode::IN_BINARY);
			if (!file) return false;
			IO::StreamPos beg = file.TellPos();
			char8_t magic[23]{};
			file.Read(magic, 23);
			for (size_t i = 0; i < sizeof(MagicNumber); ++i) {
				if (magic[i] != MagicNumber[i]) return false;
			}
			file.ReadLE(m_version);
			do {
				FBXNode* node = new FBXNode();
				if (!node->Load(file, m_version, beg) || node->IsEndMarker()) {
					delete node;
					break;
				}
				m_nodes.Add(node);
			} while (file);
			return m_nodes.Count();
		}
	public:
		const FBXNode* GetTopLevelNode(const String& name) const noexcept {
			for (FBXNode* node : m_nodes) {
				if (node->m_name == name) return node;
			}
			return nullptr;
		}
		FBXGlobalSettings GetGlobalSettings() const noexcept {
			FBXGlobalSettings ret;
			const FBXNode* node = GetTopLevelNode(FBXNodes::GlobalSettings);
			if (!node) return ret;
			const FBXNode* properties70 = node->GetChildByName(FBXNodes::Properties70);
			if (!properties70) return ret;
			for (const FBXNode* child : properties70->m_children) {
				if (child->m_numProperties < 4) continue;
				if (child->m_properties[0].m_special.GetString() == u"UpAxis") {
					ret.UPAxis = child->m_properties[child->m_numProperties - 1].m_i32;
				}
				else if (child->m_properties[0].m_special.GetString() == u"UpAxisSign") {
					ret.UpAxisSign = child->m_properties[child->m_numProperties - 1].m_i32;
				}
				else if (child->m_properties[0].m_special.GetString() == u"FrontAxis") {
					ret.FrontAxis = child->m_properties[child->m_numProperties - 1].m_i32;
				}
				else if (child->m_properties[0].m_special.GetString() == u"FrontAxisSign") {
					ret.FrontAxisSign = child->m_properties[child->m_numProperties - 1].m_i32;
				}
				else if (child->m_properties[0].m_special.GetString() == u"CoordAxis") {
					ret.CoordAxis = child->m_properties[child->m_numProperties - 1].m_i32;
				}
				else if (child->m_properties[0].m_special.GetString() == u"CoordAxisSign") {
					ret.CoordAxisSign = child->m_properties[child->m_numProperties - 1].m_i32;
				}
			}
			return ret;
		}
	};
}
