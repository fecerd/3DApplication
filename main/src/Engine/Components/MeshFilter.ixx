export module MeshFilter;
export import Component;
import System;
import Common3D;
import Bone;
import EngineUtilities;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

//Material
export namespace Engine {
	// /// @brief マテリアル情報構造体
	// struct Material {
	// 	/// @brief 反射係数用リソース
	// 	Resource reflections;
	// 	/// @brief メインテクスチャ用リソース
	// 	Resource texture;
	// 	/// @brief 乗算スフィア用リソース
	// 	Resource sphereM;
	// 	/// @brief 加算スフィア用リソース
	// 	Resource sphereA;
	// 	/// @brief トゥーン用リソース
	// 	Resource toon;
	// 	/// @brief レンダラー
	// 	Renderer renderer = GetRenderer(DefaultRendererName);
	// };

	/// @brief マテリアル情報クラス
	class Material {
	private:
		VectorBase<Resource> m_resources;
		Renderer m_renderer = Common3D::GetRenderer(DefaultRendererName);
	public:
		Material() noexcept = default;
		Material(const Material&) noexcept = default;
		Material(Material&&) noexcept = default;
		~Material() noexcept = default;
	public:
		uint32_t GetResourceCount() const noexcept { return static_cast<uint32_t>(m_resources.Count()); }
		Resource& GetResource(uint32_t index) noexcept { return m_resources[index]; }
		const Resource& GetResource(uint32_t index) const noexcept { return m_resources[index]; }
		uint32_t AddResource(const Resource& resource) noexcept {
			m_resources.Add(resource);
			return static_cast<uint32_t>(m_resources.Count() - 1);
		}
		Renderer& GetRenderer() noexcept { return m_renderer; }		
		const Renderer& GetRenderer() const noexcept { return m_renderer; }
		void SetRenderer(const Renderer& renderer) noexcept { m_renderer = renderer; }
	public:
		Material& operator=(const Material&) noexcept = default;
		Material& operator=(Material&&) noexcept = default;
	};
}

//BoneInfo
namespace Engine {
	/// @brief ボーン情報構造体
	struct BoneInfo {
		/// @brief ボーン配列
		Vector<Bone> bones;
		/// @brief ボーン名とインデックスのマップ
		HashMap<String, size_t> boneIDs = 3;
		/// @brief IKボーン配列
		Vector<IK> iks;
	};
}

//MeshFilter_NoObj
export namespace Engine {
	class MeshFilter_NoObj {
		/// @brief メッシュリソース
		MeshResource m_meshResource;
		/// @brief マテリアルリソースへのDescriptorを配置するHeap
		Heap m_materialHeap;
		/// @brief マテリアル配列
		Vector<Material> m_materials;
		/// @brief モデルのパスによって共有されるボーン情報
		ManagedObject<BoneInfo> m_boneInfo;
	public:
		MeshFilter_NoObj() noexcept {}
		/// @brief すべてのコピーはManagedObjectのコピーに帰結するため安全
		MeshFilter_NoObj(const MeshFilter_NoObj&) noexcept = default;
		MeshFilter_NoObj(MeshFilter_NoObj&&) noexcept = default;
		~MeshFilter_NoObj() noexcept {}
	public:
	public:/* マテリアル更新関連(現状はPMDマテリアルのみに対応) */
		/// @brief マテリアルの変更を確定する。Materialの各メンバを直接変更した後にこの関数を呼び出さないと、表示に反映されない。
		bool UpdateMaterialHeap() noexcept {
			if (!m_materialHeap || m_materialHeap.GetViewsSetCount() != GetMaterialCount()) {
				m_materialHeap = CreateHeap(String::Empty(), HeapType::Material, GetMaterialCount());
			}
			if (!m_materialHeap) return false;
			uint32_t materialIndex = 0;
			for (Material& material : m_materials) {
				const uint32_t resourceCount = material.GetResourceCount();
				for (uint32_t i = 0; i < resourceCount; ++i) {
					const ViewFormat format = i == 0 ? ViewFormat::CBV : ViewFormat::SRV;
					m_materialHeap.SetView(materialIndex * 5 + i, format, material.GetResource(i));
				}
				++materialIndex;
			}
			return true;
		}
		bool UpdateReflections(uint32_t materialIndex, const ReflectionsResourceDesc& desc) noexcept {
			if (m_materials.Count() <= materialIndex) return false;
			Material& material = m_materials[materialIndex];
			Resource& reflections = material.GetResource(0);
			if (!reflections) {
				reflections = CreateResource(String::Empty(), desc.GetData(), desc.GetCount());
				if (!m_materialHeap) {
					m_materialHeap = CreateHeap(String::Empty(), HeapType::Material, GetMaterialCount());
					if (!m_materialHeap) return false;
				}
				m_materialHeap.SetView(materialIndex * 5 + 0, ViewFormat::CBV, reflections);
			}
			else reflections.UpdateResource(desc.GetData(), desc.GetCount());
			return true;
		}
		bool UpdateTexture(uint32_t materialIndex, const Resource& texture, uint32_t resourceIndex) noexcept {
			if (m_materials.Count() <= materialIndex) return false;
			Material& material = m_materials[materialIndex];
			uint32_t resourceCount = material.GetResourceCount();
			Resource whiteTexture = GetResource(Common3D::DefaultWhiteTextureName);
			for (uint32_t i = resourceCount; i < resourceIndex; ++i) {
				material.AddResource(whiteTexture);
			}
			resourceCount = material.GetResourceCount();
			if (resourceCount > resourceIndex) material.GetResource(resourceIndex) = texture;
			else material.AddResource(texture);
			Resource& tex = material.GetResource(resourceIndex);
			if (!m_materialHeap) {
				m_materialHeap = CreateHeap(String::Empty(), HeapType::Material, GetMaterialCount());
				if (!m_materialHeap) return false;
			}
			m_materialHeap.SetView(materialIndex * 5 + resourceIndex, ViewFormat::SRV, tex);
			return true;
		}
		bool UpdateTexture(uint32_t materialIndex, const Resource& texture) noexcept {
			return UpdateTexture(materialIndex, texture, 1);
		}
		bool UpdateSphereM(uint32_t materialIndex, const Resource& sphereM) noexcept {
			return UpdateTexture(materialIndex, sphereM, 2);
		}
		bool UpdateSphereA(uint32_t materialIndex, const Resource& sphereA) noexcept {
			return UpdateTexture(materialIndex, sphereA, 3);
		}
		bool UpdateToon(uint32_t materialIndex, const Resource& toon) noexcept {
			return UpdateTexture(materialIndex, toon, 4);
		}
		bool UpdateRenderer(uint32_t materialIndex, const Renderer& renderer) noexcept {
			if (m_materials.Count() <= materialIndex) return false;
			Material& material = m_materials[materialIndex];
			material.SetRenderer(renderer);
			return true;
		}
		ReflectionsResourceDesc GetReflections(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return ReflectionsResourceDesc();
			const Material& material = m_materials[materialIndex];
			if (material.GetResourceCount() < 1) return ReflectionsResourceDesc();
			const Resource& reflections = material.GetResource(0);
			ReflectionsResourceDesc desc{};
			reflections.GetData(desc.GetData(), desc.GetCount());
			return desc;
		}
		Resource GetTexture(uint32_t materialIndex, uint32_t resourceIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Resource();
			else {
				const Material& material = m_materials[materialIndex];
				if (material.GetResourceCount() <= resourceIndex) return Resource();
				else return material.GetResource(resourceIndex);
			}
		}
		Resource GetTexture(uint32_t materialIndex) const noexcept {
			return GetTexture(materialIndex, 1);
		}
		Resource GetSphereM(uint32_t materialIndex) const noexcept {
			return GetTexture(materialIndex, 2);
		}
		Resource GetSphereA(uint32_t materialIndex) const noexcept {
			return GetTexture(materialIndex, 3);
		}
		Resource GetToon(uint32_t materialIndex) const noexcept {
			return GetTexture(materialIndex, 4);
		}
		Renderer GetRenderer(uint32_t materialIndex) const noexcept {
			if (m_materials.Count() <= materialIndex) return Renderer();
			else return m_materials[materialIndex].GetRenderer();
		}
	public:/* モデルデータ読み込み関連 */
		bool LoadPlane(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadTriangle(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadCube(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadSphere(Drawing::Color diffuseColor = Drawing::Colors::White) noexcept;
		bool LoadModelForPMD(const String& filename, bool calcTangentSpace = true) noexcept;
	public:/* Material取得関連 */
		Vector<Material>& GetMaterials() noexcept { return m_materials; }
		const Vector<Material>& GetMaterials() const noexcept { return m_materials; }
		uint32_t GetMaterialCount() const noexcept { return static_cast<uint32_t>(m_materials.Count()); }
	public:/* ボーン情報取得関連 */
		Vector<Bone>& GetBones() noexcept { return m_boneInfo->bones; }
		const Vector<Bone>& GetBones() const noexcept { return m_boneInfo->bones; }
		size_t GetBoneID(const String& boneName) const noexcept {
			size_t* ret = m_boneInfo->boneIDs.AtPtr(boneName);
			return ret ? *ret : MAX_VALUE<size_t>;
		}
		Vector<IK>& GetIKs() noexcept { return m_boneInfo->iks; }
	public:/* Common3D関連 */
		MeshResource GetMeshResource() const noexcept { return m_meshResource; }
		Heap GetHeap() const noexcept { return m_materialHeap; }
	public:
		MeshFilter_NoObj& operator=(const MeshFilter_NoObj&) noexcept = default;
		MeshFilter_NoObj& operator=(MeshFilter_NoObj&&) noexcept = default;		
	};
}

//MeshFilter
export namespace Engine {
	template<class GameObject>
	class MeshFilter_Impl : public Component_Impl<GameObject>, public MeshFilter_NoObj {
	public:
		MeshFilter_Impl() noexcept {}
		MeshFilter_Impl(const MeshFilter_Impl&) noexcept = default;
		MeshFilter_Impl(MeshFilter_Impl&&) noexcept = default;
		MeshFilter_Impl(GameObject* gObj) noexcept : Component_Impl<GameObject>(gObj), MeshFilter_NoObj() {}
		virtual ~MeshFilter_Impl() noexcept {}
	public:
		bool UpdateRenderer(uint32_t materialIndex, const Renderer& renderer) noexcept {
			bool ret = MeshFilter_NoObj::UpdateRenderer(materialIndex, renderer);
//上手く動かないため、コメントアウト
//			if (ret) Component_Impl<GameObject>::GetObject().UpdateCommandList();
			return ret;
		}
	public:/* Component override */
		Type GetType() const noexcept override { return Type::CreateType<MeshFilter_Impl<GameObject>>(); }
		String ToString() const noexcept override { return String(u"MeshFilter Component"); }
		uint32_t GetTypeID() const noexcept override { return GetID<MeshFilter_Impl<GameObject>>(); }
		MeshFilter_Impl<GameObject>* Clone(GameObject* object) noexcept override {
			MeshFilter_Impl<GameObject>* ret = new MeshFilter_Impl<GameObject>(object);
			static_cast<MeshFilter_NoObj*>(ret)->operator=(*this);
			ret->UpdateMaterialHeap();
			return ret;
		}
	public:
		MeshFilter_Impl& operator=(const MeshFilter_Impl&) noexcept = default;
		MeshFilter_Impl& operator=(MeshFilter_Impl&&) noexcept = default;
	};
}
