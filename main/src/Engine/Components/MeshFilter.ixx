export module Components:MeshFilter;
import :Component;
import System;
import Bone;
import Common3D;
using namespace System;
using namespace System::Application::Common3D;
using namespace Engine;

export namespace Engine {
	struct Material {
		Resource reflections;
		Resource texture;
		Resource sphereM;
		Resource sphereA;
		Resource toon;
		Renderer renderer = GetRenderer(DefaultRendererName);
	};
}

namespace Engine::Internal {
	struct BoneInfo {
		Vector<Bone> bones;
		HashMap<String, size_t> boneIDs = 3;
		Vector<IK> iks;
	};
}
using namespace Engine::Internal;

export namespace Engine {
	class MeshFilter : public Component {
		MeshResource m_meshResource;
		Heap m_materialHeap;
		Vector<Material> m_materials;
		ManagedObject<BoneInfo> m_boneInfo;
	public:
		using Component::Component;
	public: /* マテリアル更新関連 */
		/// <summary>
		/// マテリアルの変更を確定する。
		/// Materialの各メンバを直接変更した後にこの関数を呼び出さないと、表示に反映されない
		/// </summary>
		bool UpdateMaterialHeap() noexcept;
		bool UpdateReflections(uint32_t materialIndex, const ReflectionsResourceDesc& desc) noexcept;
		bool UpdateTexture(uint32_t materialIndex, const Resource& texture) noexcept;
		bool UpdateSphereM(uint32_t materialIndex, const Resource& sphereM) noexcept;
		bool UpdateSphereA(uint32_t materialIndex, const Resource& sphereA) noexcept;
		bool UpdateToon(uint32_t materialIndex, const Resource& toon) noexcept;
		bool UpdateRenderer(uint32_t materialIndex, const Renderer& renderer) noexcept;
		ReflectionsResourceDesc GetReflections(uint32_t materialIndex) const noexcept;
		Resource GetTexture(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Resource();
			else return m_materials[materialIndex].texture;
		}
		Resource GetSphereM(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Resource();
			else return m_materials[materialIndex].sphereM;
		}
		Resource GetSphereA(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Resource();
			else return m_materials[materialIndex].sphereA;
		}
		Resource GetToon(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Resource();
			else return m_materials[materialIndex].toon;
		}
		Renderer GetRenderer(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Renderer();
			else return m_materials[materialIndex].renderer;
		}
	public: /* モデルデータ読み込み関連 */
		bool LoadPlane(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadTriangle(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadCube(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadSphere(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadModelForPMD(const String& filename, bool calcTangentSpace = true) noexcept;
	public: /* Material取得関連 */
		Vector<Material>& GetMaterials() noexcept { return m_materials; }
		const Vector<Material>& GetMaterials() const noexcept { return m_materials; }
		uint32_t GetMaterialCount() const noexcept { return static_cast<uint32_t>(m_materials.Count()); }
	public: /* ボーン情報取得関連 */
		Vector<Bone>& GetBones() noexcept { return m_boneInfo->bones; }
		const Vector<Bone>& GetBones() const noexcept { return m_boneInfo->bones; }
		size_t GetBoneID(const String& boneName) const noexcept {
			size_t* ret = m_boneInfo->boneIDs.AtPtr(boneName);
			return ret ? *ret : MAX_VALUE<size_t>;
		}
		Vector<IK>& GetIKs() noexcept { return m_boneInfo->iks; }
	public: /* Common3D関連 */
		MeshResource GetMeshResource() const noexcept { return m_meshResource; }
		Heap GetHeap() const noexcept { return m_materialHeap; }
	public:
		Type GetType() const noexcept override { return Type::CreateType<MeshFilter>(); }
		String ToString() const noexcept override { return String(u"MeshFilter Component"); }
		uint32_t GetTypeID() const noexcept override { return System::GetID<MeshFilter>(); }
		MeshFilter* Clone(GameObject* object) noexcept override;
	public:
		MeshFilter& operator=(const MeshFilter&) noexcept = delete;
		MeshFilter& operator=(MeshFilter&&) noexcept = delete;
	};
}
