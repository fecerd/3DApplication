module MeshFilter;
import Component;
import System;
import Bone;
import Common3D;
import EngineUtilities;	//***MeshName
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

bool MeshFilter_NoObj::LoadPlane(Drawing::Color diffuseColor) noexcept {
	m_meshResource = Common3D::GetMeshResource(Constants::PlaneMeshName);
	if (!m_meshResource) {
		Common3D::MeshDesc desc{};
		desc.vertices.Reserve(4);
		Vertex v{};
		v.pos = Vector3(-1, -1, 0);
		v.normal = Vector3(0, 0, -1);
		v.tangent = Vector3(1, 0, 0);
		v.binormal = Vector3(0, 1, 0);
		v.SetUV(0, 1);
		v.SetBoneNo(0, 0);
		v.boneWeight = 1;
		desc.vertices.Add(v);
		v.pos.y = 1;
		v.SetUV(0, 0);
		desc.vertices.Add(v);
		v.pos.x = 1;
		v.SetUV(1, 0);
		desc.vertices.Add(v);
		v.pos.y = -1;
		v.SetUV(1, 1);
		desc.vertices.Add(v);
		const uint32_t i[6] = { 0, 1, 2, 2, 3, 0 };
		desc.indices.AddRange(i, 6);
		desc.indexCountsPerMaterial.Add(6);
		m_meshResource = Common3D::CreateMeshResource(Constants::PlaneMeshName, desc);
	}
	m_materials.Clear();
	Material m;
	ReflectionsResourceDesc desc{};
	desc.SetDiffuseColor(diffuseColor);
	//reflections
	m.AddResource(CreateResource(String::Empty(), desc.GetData(), desc.GetCount()));
	//texture
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereM
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereA
	m.AddResource(GetResource(Common3D::DefaultBlackTextureName));
	//toon
	m.AddResource(GetResource(Common3D::DefaultToonTextureName));
	m_materials.Add(System::move(m));
	UpdateMaterialHeap();
	return true;
}
bool MeshFilter_NoObj::LoadTriangle(Drawing::Color diffuseColor) noexcept {
	m_meshResource = Common3D::GetMeshResource(Constants::TriangleMeshName);
	if (!m_meshResource) {
		Common3D::MeshDesc desc{};
		desc.vertices.Reserve(3);
		Vertex v{};
		v.pos = Vector3(-1, -1, 0);
		v.normal = Vector3(0, 0, -1);
		v.tangent = Vector3(1, 0, 0);
		v.binormal = Vector3(0, 1, 0);
		v.SetUV(0, 1);
		v.SetBoneNo(0, 0);
		v.boneWeight = 1;
		desc.vertices.Add(v);
		v.pos = Vector3(0, 1, 0);
		v.SetUV(0.5f, 0.f);
		desc.vertices.Add(v);
		v.pos = Vector3(1, -1, 0);
		v.SetUV(1, 1);
		desc.vertices.Add(v);
		const uint32_t i[3] = { 0, 1, 2 };
		desc.indices.AddRange(i, 3);
		desc.indexCountsPerMaterial.Add(3);
		m_meshResource = Common3D::CreateMeshResource(Constants::TriangleMeshName, desc);
		if (!m_meshResource) return false;
	}
	m_materials.Clear();
	Material m;
	ReflectionsResourceDesc desc{};
	desc.SetDiffuseColor(diffuseColor);
	//reflections
	m.AddResource(CreateResource(String::Empty(), desc.GetData(), desc.GetCount()));
	//texture
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereM
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereA
	m.AddResource(GetResource(Common3D::DefaultBlackTextureName));
	//toon
	m.AddResource(GetResource(Common3D::DefaultToonTextureName));
	m_materials.Add(System::move(m));
	UpdateMaterialHeap();
	return true;
}
bool MeshFilter_NoObj::LoadCube(Drawing::Color diffuseColor) noexcept {
	m_meshResource = Common3D::GetMeshResource(Constants::CubeMeshName);
	if (!m_meshResource) {
		Common3D::MeshDesc desc{};
		desc.vertices.Reserve(24);
		desc.indices.Reserve(36);
		uint32_t indices[6] = { 0, 1, 2, 2, 1, 3 };
		Vertex vert[4];
		vert[0].pos = Vector3(-1, 1, -1);
		vert[0].normal = Vector3(0, 0, -1);
		vert[0].binormal = Vector3(1, 0, 0);
		vert[0].tangent = Vector3(0, -1, 0);
		vert[0].SetBoneNo(0, 0);
		vert[0].boneWeight = 255;
		vert[0].SetUV(0, 0);
		vert[1].pos = Vector3(1, 1, -1);
		vert[1].normal = vert[0].normal;
		vert[1].binormal = vert[0].binormal;
		vert[1].tangent = vert[0].tangent;
		vert[1].SetBoneNo(0, 0);
		vert[1].boneWeight = 255;
		vert[1].SetUV(1, 0);
		vert[2].pos = Vector3(-1, -1, -1);
		vert[2].normal = vert[0].normal;
		vert[2].binormal = vert[0].normal;
		vert[2].tangent = vert[0].tangent;
		vert[2].SetBoneNo(0, 0);
		vert[2].boneWeight = 255;
		vert[2].SetUV(0, 1);
		vert[3].pos = Vector3(1, -1, -1);
		vert[3].normal = vert[0].normal;
		vert[3].binormal = vert[0].binormal;
		vert[3].tangent = vert[0].tangent;
		vert[3].SetBoneNo(0, 0);
		vert[3].boneWeight = 255;
		vert[3].SetUV(1, 1);
		//Z軸負方向
		desc.vertices.AddRange(vert, 4);
		desc.indices.AddRange(indices, 6);
		//Z軸正方向
		vert[0].pos = Vector3(1, 1, 1);
		vert[0].normal.z = 1;
		vert[0].binormal.x = -1;
		vert[1].pos = Vector3(-1, 1, 1);
		vert[1].normal.z = 1;
		vert[1].binormal.x = -1;
		vert[2].pos = Vector3(1, -1, 1);
		vert[2].normal.z = 1;
		vert[2].binormal.x = -1;
		vert[3].pos = Vector3(-1, -1, 1);
		vert[3].normal.z = 1;
		vert[3].binormal.x = -1;
		desc.vertices.AddRange(vert, 4);
		for (int i = 0; i < 6; ++i) indices[i] += 4;
		desc.indices.AddRange(indices, 6);
		//X軸負方向
		vert[0].pos = Vector3(-1, 1, 1);
		vert[0].normal = Vector3(-1, 0, 0);
		vert[0].binormal = Vector3(0, 0, -1);
		vert[1].pos = Vector3(-1, 1, -1);
		vert[1].normal = vert[0].normal;
		vert[1].binormal = vert[0].binormal;
		vert[2].pos = Vector3(-1, -1, 1);
		vert[2].normal = vert[0].normal;
		vert[2].binormal = vert[0].binormal;
		vert[3].pos = Vector3(-1, -1, -1);
		vert[3].normal = vert[0].normal;
		vert[3].binormal = vert[0].binormal;
		desc.vertices.AddRange(vert, 4);
		for (int i = 0; i < 6; ++i) indices[i] += 4;
		desc.indices.AddRange(indices, 6);
		//X軸正方向
		vert[0].pos = Vector3(1, 1, -1);
		vert[0].normal.x = 1;
		vert[0].binormal.z = 1;
		vert[1].pos = Vector3(1, 1, 1);
		vert[1].normal.x = 1;
		vert[1].binormal.z = 1;
		vert[2].pos = Vector3(1, -1, -1);
		vert[2].normal.x = 1;
		vert[2].binormal.z = 1;
		vert[3].pos = Vector3(1, -1, 1);
		vert[3].normal.x = 1;
		vert[3].binormal.z = 1;
		desc.vertices.AddRange(vert, 4);
		for (int i = 0; i < 6; ++i) indices[i] += 4;
		desc.indices.AddRange(indices, 6);
		//Y軸負方向
		vert[0].pos = Vector3(-1, -1, -1);
		vert[0].normal = Vector3(0, -1, 0);
		vert[0].binormal = Vector3(1, 0, 0);
		vert[0].tangent = Vector3(0, 0, 1);
		vert[1].pos = Vector3(1, -1, -1);
		vert[1].normal = vert[0].normal;
		vert[1].binormal = vert[0].binormal;
		vert[1].tangent = vert[0].tangent;
		vert[2].pos = Vector3(-1, -1, 1);
		vert[2].normal = vert[0].normal;
		vert[2].binormal = vert[0].binormal;
		vert[2].tangent = vert[0].tangent;
		vert[3].pos = Vector3(1, -1, 1);
		vert[3].normal = vert[0].normal;
		vert[3].binormal = vert[0].binormal;
		vert[3].tangent = vert[0].tangent;
		desc.vertices.AddRange(vert, 4);
		for (int i = 0; i < 6; ++i) indices[i] += 4;
		desc.indices.AddRange(indices, 6);
		//Y軸正方向
		vert[0].pos = Vector3(-1, 1, 1);
		vert[0].normal.y = 1;
		vert[0].tangent.z = -1;
		vert[1].pos = Vector3(1, 1, 1);
		vert[1].normal.y = 1;
		vert[1].tangent.z = -1;
		vert[2].pos = Vector3(-1, 1, -1);
		vert[2].normal.y = 1;
		vert[2].tangent.z = -1;
		vert[3].pos = Vector3(1, 1, -1);
		vert[3].normal.y = 1;
		vert[3].tangent.z = -1;
		desc.vertices.AddRange(vert, 4);
		for (int i = 0; i < 6; ++i) indices[i] += 4;
		desc.indices.AddRange(indices, 6);
		desc.indexCountsPerMaterial.Reserve(6);
		for (int i = 0; i < 6; ++i) desc.indexCountsPerMaterial.Add(6);
		m_meshResource = Common3D::CreateMeshResource(Constants::CubeMeshName, desc);
		if (!m_meshResource) return false;
	}
	m_materials.Clear();

	Material m;
	ReflectionsResourceDesc desc{};
	desc.SetDiffuseColor(diffuseColor);
	//reflections
	m.AddResource(CreateResource(String::Empty(), desc.GetData(), desc.GetCount()));
	//texture
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereM
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereA
	m.AddResource(GetResource(Common3D::DefaultBlackTextureName));
	//toon
	m.AddResource(GetResource(Common3D::DefaultToonTextureName));
	for (int i = 0; i < 6; ++i) m_materials.Add(m);
	UpdateMaterialHeap();
	return true;
}
bool MeshFilter_NoObj::LoadSphere(Drawing::Color diffuseColor) noexcept {
	m_meshResource = Common3D::GetMeshResource(Constants::SphereMeshName);
	if (!m_meshResource) {
		MeshDesc desc{};
		const uint16_t tessellation = 32;
		const uint16_t verticalSeg = tessellation;
		const uint16_t horizontalSeg = tessellation * 2;
		const float radius = 1;
		Vertex vert{};
		vert.SetBoneNo(0, 0);
		vert.boneWeight = 255;
		desc.vertices.Reserve(static_cast<size_t>(verticalSeg + 1) * (horizontalSeg + 1));
		for (uint16_t i = 0; i <= verticalSeg; ++i) {
			const float tmp = static_cast<float>(i) / verticalSeg;
			const float v = 1.f - tmp;
			// latitude(rad) = tmp * PI - PI/2 = PI * (tmp - 1/2)
			const float latitude = Math::PI<float> * (tmp - 0.5f);
			float dy, dxz;
			Math::SinCos(latitude, dy, dxz);
			for (uint16_t j = 0; j <= horizontalSeg; ++j) {
				const float u = static_cast<float>(j) / horizontalSeg;
				// longitude(rad) = u * 2PI
				const float longitude = u * Math::PIConstant<float, 2, 1>;
				float dx, dz;
				Math::SinCos(longitude, dx, dz);
				dx *= dxz;
				dz *= dxz;
				//計算誤差を考慮しなければ、normalは正規化済み
				vert.normal = Vector3(dx, dy, dz);
				vert.pos = vert.normal * radius;
				vert.binormal = Vector3(0, 0, 0);
				vert.tangent = Vector3(0, 0, 0);
				vert.SetUV(u, v);
				desc.vertices.Add(vert);
			}
		}
		const uint16_t stride = horizontalSeg + 1;
		desc.indices.Reserve(6ull * verticalSeg * stride);
		for (uint16_t i = 0; i < verticalSeg; ++i) {
			for (uint16_t j = 0; j <= horizontalSeg; ++j) {
				const uint16_t nextI = i + 1;
				const uint16_t nextJ = (j + 1) % stride;
				desc.indices.Add(i * stride + j);
				desc.indices.Add(nextI * stride + j);
				desc.indices.Add(i * stride + nextJ);
				desc.indices.Add(i * stride + nextJ);
				desc.indices.Add(nextI * stride + j);
				desc.indices.Add(nextI * stride + nextJ);
			}
		}
		desc.indexCountsPerMaterial.Add(static_cast<uint32_t>(desc.indices.Count()));
		m_meshResource = Common3D::CreateMeshResource(Constants::SphereMeshName, desc);
		if (!m_meshResource) return false;
	}
	m_materials.Clear();
	Material m;
	ReflectionsResourceDesc desc{};
	desc.SetDiffuseColor(diffuseColor);
	//reflections
	m.AddResource(CreateResource(String::Empty(), desc.GetData(), desc.GetCount()));
	//texture
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereM
	m.AddResource(GetResource(Common3D::DefaultWhiteTextureName));
	//sphereA
	m.AddResource(GetResource(Common3D::DefaultBlackTextureName));
	//toon
	m.AddResource(GetResource(Common3D::DefaultToonTextureName));
	m_materials.Add(System::move(m));
	UpdateMaterialHeap();
	return true;
}

bool MeshFilter_NoObj::LoadModelForPMD(const String& filename, bool calcTangentSpace) noexcept {
	Formats::PMD pmd{ filename };
	if (!pmd) return false;
	//メッシュデータを作成
	m_meshResource = Common3D::GetMeshResource(filename);
	if (!m_meshResource) {
		Common3D::MeshDesc desc{};
		if (calcTangentSpace) {
			const Formats::PMDVertex* vertices = pmd.vertices.Items();
			Vertex v{};
			uint8_t mod = 0;
			uint32_t i = 0;
			desc.vertices.Reserve(pmd.indices.Count());
			desc.indices.Reserve(pmd.indices.Count());
			for (uint16_t index : pmd.indices) {
				const Formats::PMDVertex& pv = vertices[index];
				v.pos = pv.pos;
				v.normal = pv.normal;
				v.SetUV(pv.uv);
				v.SetBoneNo(pv.boneNo);
				v.boneWeight = pv.boneWeight;
				desc.vertices.Add(v);
				desc.indices.Add(i++);
				if (mod >= 2) mod = 0;
				else ++mod;
				//接ベクトル空間を計算して設定する
				if (!mod) {
					Vertex& v2 = desc.vertices[i - 1];
					Vertex& v1 = desc.vertices[i - 2];
					Vertex& v0 = desc.vertices[i - 3];
					//XUV
					{
						Vector3 xuv0 = Vector3(v0.pos.x, v0.uv[0], v0.uv[1]);
						Vector3 xuv1 = Vector3(v1.pos.x, v1.uv[0], v1.uv[1]);
						Vector3 xuv2 = Vector3(v2.pos.x, v2.uv[0], v2.uv[1]);
						Vector3 xuvNormal = Vector3::Cross(xuv1 - xuv0, xuv2 - xuv1);
						if (xuvNormal.x != 0.f) {
							v0.tangent.x = -xuvNormal.y / xuvNormal.x;
							v0.binormal.x = -xuvNormal.z / xuvNormal.x;
							v1.tangent.x = v0.tangent.x;
							v1.binormal.x = v0.binormal.x;
							v2.tangent.x = v0.tangent.x;
							v2.binormal.x = v0.binormal.x;
						}
					}
					//YUV
					{
						Vector3 yuv0 = Vector3(v0.pos.y, v0.uv[0], v0.uv[1]);
						Vector3 yuv1 = Vector3(v1.pos.y, v1.uv[0], v1.uv[1]);
						Vector3 yuv2 = Vector3(v2.pos.y, v2.uv[0], v2.uv[1]);
						Vector3 yuvNormal = Vector3::Cross(yuv1 - yuv0, yuv2 - yuv1);
						if (yuvNormal.x != 0.f) {
							v0.tangent.y = -yuvNormal.y / yuvNormal.x;
							v0.binormal.y = -yuvNormal.z / yuvNormal.x;
							v1.tangent.y = v0.tangent.y;
							v1.binormal.y = v0.binormal.y;
							v2.tangent.y = v0.tangent.y;
							v2.binormal.y = v0.binormal.y;
						}
					}
					//ZUV
					{
						Vector3 zuv0 = Vector3(v0.pos.z, v0.uv[0], v0.uv[1]);
						Vector3 zuv1 = Vector3(v1.pos.z, v1.uv[0], v1.uv[1]);
						Vector3 zuv2 = Vector3(v2.pos.z, v2.uv[0], v2.uv[1]);
						Vector3 zuvNormal = Vector3::Cross(zuv1 - zuv0, zuv2 - zuv1);
						if (zuvNormal.x != 0.f) {
							v0.tangent.z = -zuvNormal.y / zuvNormal.x;
							v0.binormal.z = -zuvNormal.z / zuvNormal.x;
							v1.tangent.z = v0.tangent.z;
							v1.binormal.z = v0.binormal.z;
							v2.tangent.z = v0.tangent.z;
							v2.binormal.z = v0.binormal.z;
						}
					}
				}
			}
		}
		else {
			desc.vertices.Reserve(pmd.vertices.Count());
			Vertex tmp{};
			for (Formats::PMDVertex& v : pmd.vertices) {
				tmp.pos = v.pos;
				tmp.normal = v.normal;
				tmp.SetUV(v.uv);
				tmp.SetBoneNo(v.boneNo);
				tmp.boneWeight = v.boneWeight;
				desc.vertices.Add(tmp);
			}
			desc.indices.Reserve(pmd.indices.Count());
			for (uint16_t i : pmd.indices) desc.indices.Add(i);
		}
		desc.indexCountsPerMaterial.Reserve(pmd.materials.Count());
		for (Formats::PMDMaterial& material : pmd.materials) {
			desc.indexCountsPerMaterial.Add(material.indicesNum);
		}
		m_meshResource = Common3D::CreateMeshResource(filename, desc);
		if (!m_meshResource) return false;
	}
	//マテリアル情報からリソースを作成(設定)
	m_materials.Reserve(pmd.materials.Count());
	const IO::Path modelPath = IO::Path(filename).GetDirectory();
	Common3D::Renderer mmdRenderer = Common3D::GetRenderer(Common3D::DefaultMMDRendererName);
	Common3D::Resource whiteTexture = Common3D::GetResource(Common3D::DefaultWhiteTextureName);
	Common3D::Resource blackTexture = Common3D::GetResource(Common3D::DefaultBlackTextureName);
	Common3D::Resource toonTexture = Common3D::GetResource(Common3D::DefaultToonTextureName);
	for (Formats::PMDMaterial& material : pmd.materials) {
		Material m;
		//各種反射用定数リソース
		ReflectionsResourceDesc desc{};
		desc.diffuse[0] = material.diffuse.x;
		desc.diffuse[1] = material.diffuse.y;
		desc.diffuse[2] = material.diffuse.z;
		desc.diffuse[3] = material.alpha;
		desc.specular[0] = material.specular.x;
		desc.specular[1] = material.specular.y;
		desc.specular[2] = material.specular.z;
		desc.specularity = material.specularity;
		desc.ambient[0] = material.ambient.x;
		desc.ambient[1] = material.ambient.y;
		desc.ambient[2] = material.ambient.z;
		m.AddResource(Common3D::CreateResource(String::Empty(), desc.GetData(), desc.GetCount()));
		//各種テクスチャリソース
		bool exists[3]{};	//texture, sph, spa
		Resource texture;
		Resource sphereM;
		Resource sphereA;
		for (const String& path : material.GetTextureFilePaths()) {
			const IO::Path fullPath = modelPath.CreatePath(path);
			const String ext = fullPath.Extension();
			static const String bmp = u".bmp";
			static const String png = u".png";
			static const String tga = u".tga";
			static const String sph = u".sph";
			static const String spa = u".spa";
			if (!exists[1] && ext == sph) {
				sphereM = Common3D::GetResource(fullPath.PathName());
				if (!sphereM) {
					Drawing::Image image{ fullPath.PathName() };
					if (image) sphereM = Common3D::CreateResource(fullPath.PathName(), image);
					else sphereM = Common3D::GetResource(Common3D::DefaultWhiteTextureName);
				}
				exists[1] = true;
			}
			else if (!exists[2] && ext == spa) {
				sphereA = Common3D::GetResource(fullPath.PathName());
				if (!sphereA) {
					Drawing::Image image{ fullPath.PathName() };
					if (image) sphereA = Common3D::CreateResource(fullPath.PathName(), image);
					else sphereA = Common3D::GetResource(Common3D::DefaultBlackTextureName);
				}
				exists[2] = true;
			}
			else if (!exists[0] && (ext == bmp || ext == png || ext == tga)) {
				texture = Common3D::GetResource(fullPath.PathName());
				if (!texture) {
					Drawing::Image image{ fullPath.PathName() };
					if (image) texture = Common3D::CreateResource(fullPath.PathName(), image);
					else texture = Common3D::GetResource(Common3D::DefaultWhiteTextureName);
				}
				exists[0] = true;
			}
		}
		m.AddResource(exists[0] ? texture : whiteTexture);
		m.AddResource(exists[1] ? sphereM : whiteTexture);
		m.AddResource(exists[2] ? sphereA : blackTexture);
		//トゥーンテクスチャリソース
		const IO::Path toonFullPath = modelPath.CreatePath(material.GetToonTexturePath());
		Resource toon = Common3D::GetResource(toonFullPath.PathName());
		if (!toon) {
			Drawing::Image image{ toonFullPath.PathName() };
			if (image) toon = Common3D::CreateResource(toonFullPath.PathName(), image);
			else toon = toonTexture;
		}
		m.AddResource(toon);
		m.SetRenderer(mmdRenderer);
		m_materials.Add(System::move(m));
	}
	UpdateMaterialHeap();
	//ボーン情報を変換する
	m_boneInfo = ManagedObject<BoneInfo>(filename, new BoneInfo());
	Vector<Bone>& bones = m_boneInfo->bones;
	HashMap<String, size_t>& boneIDs = m_boneInfo->boneIDs;
	bones.Reserve(pmd.bones.Count());
	for (size_t i = 0, count = pmd.bones.Count(); i < count; ++i) {
		Formats::PMDBone& bone = pmd.bones[i];
		Bone tmp;
		tmp.name = bone.boneName;
		tmp.id = i;
		tmp.position = bone.pos;
		tmp.rotation = Quaternion::Identity();
		tmp.scale = Vector3::One();
		tmp.parent = bone.parentNo;
		tmp.isIKBone = bone.IsIKBone();
		bones.Add(System::move(tmp));
		boneIDs.Insert(bone.boneName, i);
	}
	//IKボーン情報を変換する
	Vector<IK>& iks = m_boneInfo->iks;
	iks.Reserve(pmd.iks.Count());
	for (Formats::PMDIK& ik : pmd.iks) {
		IK tmp;
		tmp.id = ik.boneIdx;
		tmp.targetID = ik.targetIdx;
		tmp.interations = ik.iterations;
		tmp.limit = ik.limit;
		tmp.chainID.Reserve(ik.chainLen);
		for (uint8_t i = 0; i < ik.chainLen; ++i) tmp.chainID.Add(ik.nodeIdxes[i]);
		iks.Add(System::move(tmp));
	}
	return true;
}
