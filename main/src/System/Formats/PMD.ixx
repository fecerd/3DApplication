export module PMD;
import CSTDINT;
import Vector3;
import Vector;
import Objects;
import IEnumerable;
import BoostCoroutine;
import IO;
import Memory;

export namespace System::Formats {
	struct PMDHeader {
		float version;
		char model_name[20]{};
		char comment[256]{};
	};
	struct PMDVertex {
		Vector3 pos;
		Vector3 normal;
		float uv[2]{};
		uint16_t boneNo[2]{};
		uint8_t boneWeight = 0;
		uint8_t edgeFlag = 0;
	};
	struct PMDMaterial {
		Vector3 diffuse;
		float alpha;
		float specularity;
		Vector3 specular;
		Vector3 ambient;
		uint8_t toonIdx;
		uint8_t edgeFlag;
		//uint16_t padding;
		uint32_t indicesNum;
		char texFilePath[20];
	private:
#if defined(__GNUC__) && !defined(__clang__)
		IEnumerator<String> GetTextureFilePathEnumerator(bool reverse) const noexcept {
			auto internal_0 = [this](Boost::push_type<String&>& sink) {
				if (texFilePath[0] == '\0') return;
				const String rawPath{ texFilePath, 20 };
				const String separator = u"*";
				size_t length = rawPath.find(separator);
				size_t pos = 0;
				do {
					String ret = rawPath.substr(pos, length);
					sink(ret);
					pos = length + 1;
					length = rawPath.find(separator, pos);
				} while (length != String::npos);				
			};
			auto internal = [internal_0](bool) -> IEnumerator<String> {
				return IEnumerator<String>(internal_0);
			};
			return IEnumerator<String>(internal, reverse);
		}
#else
		IEnumerator<String> GetTextureFilePathEnumerator(bool reverse) const noexcept {
			auto internal = [this](bool) -> IEnumerator<String> {
				if (texFilePath[0] == '\0') co_return;
				const String rawPath{ texFilePath, 20 };
				const String separator = u"*";
				size_t length = rawPath.find(separator);
				size_t pos = 0;
				do {
					String ret = rawPath.substr(pos, length);
					co_yield ret;
					pos = length + 1;
					length = rawPath.find(separator, pos);
				} while (length != String::npos);
			};
			return IEnumerator<String>(internal, reverse);
		}	
#endif
	public:
		IEnumerable<String> GetTextureFilePaths() const noexcept {
			return IEnumerable<String>(
				new IEnumerator<String>(GetTextureFilePathEnumerator(false))
			);
		}
		String GetToonTexturePath() const noexcept {
			//トゥーンテクスチャ名はtoon(toonIdx + 1).bmpで、一桁の場合、toon01.bmpのようにゼロ埋めする
			uint8_t toonIndex = toonIdx + 1;
			return String::Joint(u".\\toon", toonIndex < 10 ? u"0" : u"", toonIndex, u".bmp");
		}
	};
	enum class BoneType : uint8_t {
		Rotation = 0,
		RotationAndTranslate,
		IK,
		Unknown,
		IKChild,
		RotationChild,
		IKDestination,
		Invisible
	};
	struct PMDBone {
		char boneName[20];
		uint16_t parentNo;
		uint16_t nextNo;
		BoneType type;
		//uint8_t padding[3];
		uint16_t ikBoneNo;
		Vector3 pos;
	public:
		constexpr bool IsIKBone() const noexcept {
			return type == BoneType::IK;
		}
	};
	struct PMDIK {
		uint16_t boneIdx;
		uint16_t targetIdx;
		uint8_t chainLen;
		uint16_t iterations;
		float limit;
		uint16_t* nodeIdxes = nullptr;
	public:
		~PMDIK() noexcept {
			delete nodeIdxes;
			nodeIdxes = nullptr;
		}
	};
}

export namespace System::Formats {
	class PMD {
	public:
		//(親がいない)センターボーンの親ボーンNoに使用する値
		static constexpr uint16_t CenterBoneParentNo = MAX_VALUE<uint16_t>;
	private:
		PMDHeader header;
	public:
		const String filePathName;
		Vector<PMDVertex> vertices;
		Vector<uint16_t> indices;
		Vector<PMDMaterial> materials;
		Vector<PMDBone> bones;
		Vector<PMDIK> iks;
	public:
		PMD() noexcept = delete;
		PMD(const PMD&) noexcept = delete;
		PMD(const String& filePath) noexcept : filePathName(filePath) {
			IO::FileStream file(filePath, IO::OpenMode::IN_BINARY);
			if (!file) return;
			char sign[3]{};
			file.Read(sign, 3);
			if (!Memory::Compare(sign, "Pmd", 3)) return;
			file.Read(&header, sizeof(header));
			uint32_t vertexCount = 0;
			file.ReadLE(vertexCount);
			vertices.AddRange(file, vertexCount, 38);
			uint32_t indexCount = 0;
			file.ReadLE(indexCount);
			indices.AddRange(file, indexCount);
			auto LoadMaterial = [](IO::FileStream& f, PMDMaterial* dst) {
				f.Read(dst, 46);
				f.Read(&dst->indicesNum, 24);
				//PMX -> PMDに変換するとtexFilePathの末尾が\0で埋められてしまうため処理
				if (dst->texFilePath[16] == u'.' && dst->texFilePath[19] == u'\0') {
					if (dst->texFilePath[17] == u'p' && dst->texFilePath[18] == u'n') {
						dst->texFilePath[19] = u'g';
					}
					else if (dst->texFilePath[17] == u'b' && dst->texFilePath[18] == u'm') {
						dst->texFilePath[19] = u'p';
					}
				}
			};
			uint32_t materialCount = 0;
			file.ReadLE(materialCount);
			materials.AddRange(file, materialCount, LoadMaterial);
			uint16_t boneCount = 0;
			file.ReadLE(boneCount);
			auto LoadBone = [](IO::FileStream& f, PMDBone* dst) {
				f.Read(dst, 25);
				f.Read(&dst->ikBoneNo, 14);
			};
			bones.AddRange(file, boneCount, LoadBone);
			uint16_t ikCount = 0;
			file.ReadLE(ikCount);
			auto LoadIK = [](IO::FileStream& f, PMDIK* dst) {
				f.Read(dst, 5);
				f.Read(&dst->iterations, 6);
				if (dst->nodeIdxes) delete dst->nodeIdxes;
				dst->nodeIdxes = new uint16_t[dst->chainLen];
				f.Read(dst->nodeIdxes, sizeof(uint16_t) * dst->chainLen);
			};
			iks.AddRange(file, ikCount, LoadIK);
		}
		~PMD() noexcept = default;
	public:
		explicit operator bool() const noexcept {
			return vertices.Count() && indices.Count();
		}
	};
}
