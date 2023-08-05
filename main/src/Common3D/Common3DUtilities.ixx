export module Common3DUtilities;
import System;
using namespace System::Drawing;

//Constant
export namespace System::Application::Common3D {
	//シェーダで同時に処理できるボーン数
	inline constexpr uint32_t BoneMatrixCount = 256;
	inline constexpr CString DefaultCameraHeapName = u"DefaultCameraHeap";
	inline constexpr CString DefaultObjectHeapName = u"DefaultObjectHeap";
	inline constexpr CString DefaultAnimationHeapName = u"DefaultAnimationHeap";
	inline constexpr CString DefaultMaterialHeapName = u"DefaultMaterialHeap";
	inline constexpr CString DefaultSceneHeapName = u"DefaultSceneHeap";
	inline constexpr CString DefaultWhiteTextureName = u"DefaultWhiteTexture";
	inline constexpr CString DefaultBlackTextureName = u"DefaultBlackTexture";
	inline constexpr CString DefaultToonTextureName = u"DefaultToonTexture";
	inline constexpr CString DefaultRendererName = u"DefaultRenderer";
	inline constexpr CString DefaultMMDRendererName = u"DefaultMMDRenderer";
	inline constexpr CString DefaultZPrepassRendererName = u"DefaultZPrepassRenderer";
	inline constexpr CString DefaultClusteredShadingRendererName = u"DefaultClusteredShadingRenderer";
	inline constexpr CString DefaultVideoRendererName = u"DefaultVideoRenderer";
	inline constexpr CString DefaultVSName = u"DefaultVS";
	inline constexpr CString DefaultPSName = u"DefaultPS";
	inline constexpr CString DefaultVSTarget = u"vs_5_0";
	inline constexpr CString DefaultPSTarget = u"ps_5_0";
	inline constexpr CString DefaultShaderEntry = u"main";
	inline constexpr CString DefaultZPrepassVSName = u"DefaultZPrepassVS";
	inline constexpr CString DefaultClusteredShadingVSName = u"DefaultClusteredShadingVS";
	inline constexpr CString DefaultClusteredShadingPSName = u"DefaultClusteredShadingPS";
	inline constexpr CString DefaultVideoPSName = u"DefaultVideoPS";

	inline constexpr uint32_t ClusterXCount = 32;
	inline constexpr uint32_t ClusterYCount = 32;
	inline constexpr uint32_t ClusterZCount = 16;
}

//Vertex
export namespace System::Application::Common3D {
	struct Vertex {
		Vector3 pos;
		Vector3 normal;
		Vector3 tangent;
		Vector3 binormal;
		float uv[2];
		uint32_t boneNo[2];
		uint8_t boneWeight;
	public:
		constexpr void SetUV(float u, float v) noexcept {
			uv[0] = u;
			uv[1] = v;
		}
		constexpr void SetUV(const float(&uv)[2]) noexcept {
			this->uv[0] = uv[0];
			this->uv[1] = uv[1];
		}
		template<System::Traits::Integral T>
		constexpr void SetBoneNo(T bone0, T bone1) noexcept {
			this->boneNo[0] = static_cast<uint32_t>(bone0);
			this->boneNo[1] = static_cast<uint32_t>(bone1);
		}
		template<System::Traits::Integral T>
		constexpr void SetBoneNo(T(&boneNo)[2]) noexcept {
			this->boneNo[0] = static_cast<uint32_t>(boneNo[0]);
			this->boneNo[1] = static_cast<uint32_t>(boneNo[1]);
		}
	};
}

//Enum
export namespace System::Application::Common3D {
	enum class PlatformID : uint8_t {
		DirectX,
		OpenGL
	};

	enum class ResourceFormat : uint8_t {
		Error,
		Float,
		Matrix,
		Texture,
		UInt
	};

	template<class T>
	ResourceFormat ConvertResourceFormat() noexcept {
		if constexpr (Traits::is_same_v<T, Image>) return ResourceFormat::Texture;
		else if constexpr (Traits::is_same_v<T, float>) return ResourceFormat::Float;
		else if constexpr (Traits::is_same_v<T, Matrix>) return ResourceFormat::Matrix;
		else if constexpr (Traits::is_same_v<T, uint32_t>) return ResourceFormat::UInt;
		else return ResourceFormat::Error;
	}

	enum class ViewFormat : uint8_t {
		CBV,
		SRV,
		UAV,
		RTV,
		DSV
	};

	enum class HeapType : uint8_t {
		Camera = 0,
		Object,
		Animation,
		Material,
		Scene
	};

	/// <summary>
	/// レンダリングモード
	/// DirectXではRootSignatureを指定する
	/// </summary>
	enum class RenderingMode : uint8_t {
		Standard,
		Z_Prepass,
		ClusteredShading
	};

	enum class DepthFuncType : uint8_t {
		Always,
		Less,
		LessEq,
		Greater,
		GreaterEq,
		Equal,
		NotEqual,
		Never
	};

	/// <summary>
	/// 指定した向きのポリゴンを描画しない
	/// </summary>
	enum class CullingMode : uint8_t {
		None,
		Front,
		Back
	};

	enum class BlendFactors : uint8_t {
		Zero,
		One,
		SrcColor,
		SrcColorInv,
		DstColor,
		DstColorInv,
		SrcAlpha,
		SrcAlphaInv,
		DstAlpha,
		DstAlphaInv
	};

	enum class BlendOp : uint8_t {
		Add,
		Subtract,
		ReverseSubtract
	};
}

//Desc
export namespace System::Application::Common3D {
	struct ShaderDesc {
		String name;
		String filepath;
		String entry;
		String target;
	};

	struct RenderTargetDesc {
		bool blendEnabled = true;
		BlendFactors dstFactor = BlendFactors::SrcAlphaInv;
		BlendFactors srcFactor = BlendFactors::SrcAlpha;
		BlendFactors dstAlphaFactor = BlendFactors::SrcAlphaInv;
		BlendFactors srcAlphaFactor = BlendFactors::SrcAlpha;
		BlendOp colorOp = BlendOp::Add;
		BlendOp alphaOp = BlendOp::Add;
	};

	struct RendererDesc {
		RenderingMode mode = RenderingMode::Standard;
		ShaderDesc vs;
		ShaderDesc ps;
		bool depthEnabled = true;
		bool stencilEnabled = false;
		DepthFuncType depthFunc = DepthFuncType::LessEq;
		CullingMode culling = CullingMode::None;
		Vector<RenderTargetDesc> rt;
	};

	struct MeshDesc {
		Vector<Vertex> vertices;
		Vector<uint32_t> indices;
		Vector<uint32_t> indexCountsPerMaterial;
	};

	struct CameraResourceDesc {
		Matrix view = Matrix::Identity();
		Matrix projection = Matrix::Identity();
		Vector3 eye = Vector3();
	public:
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(&view); }
		static constexpr uint32_t GetCount() noexcept { return (sizeof(Matrix) * 2 + sizeof(Vector3)) / sizeof(float); }
	};

	struct ObjectResourceDesc {
		Matrix world = Matrix::Identity();
	public:
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(&world); }
		static constexpr uint32_t GetCount() noexcept { return sizeof(Matrix) / sizeof(float); }
	};

	struct AnimationResourceDesc {
		Matrix bone[BoneMatrixCount] = {};
	public:
		const Matrix* GetData() const noexcept { return bone; }
		static constexpr uint32_t GetCount() noexcept { return BoneMatrixCount; }
	};

	struct SceneResourceDesc {
		Vector3 lightColor = Vector3(1.f, 1.f, 1.f);
		float unused1 = 1.f;
		Vector3 lightDirection = Vector3(0.f, 0.f, 0.f);
		float unused2 = 1.f;
	public:
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(&lightColor); }
		static constexpr uint32_t GetCount() noexcept { return sizeof(SceneResourceDesc) / sizeof(float); }
	public:
		void SetLightColor(Color color) noexcept {
			lightColor.x = color.R / 255.f;
			lightColor.y = color.G / 255.f;
			lightColor.z = color.B / 255.f;
		}
	};

	struct SceneResourceDesc2 {
		//光の減衰率(x, y, z)
		Float4 attenuation = Float4();
		Float4 clusterCount = Float4(ClusterXCount, ClusterYCount, ClusterZCount, 0.f);
	public:
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(&attenuation); }
		static constexpr uint32_t GetCount() noexcept { return sizeof(SceneResourceDesc2) / sizeof(float); }
	};

	struct LightListResourceDesc {
		Vector<Float4> lightList;
	public:
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(lightList.Items()); }
		uint32_t GetCount() const noexcept { return static_cast<uint32_t>(lightList.Count() * 4); }
	};

	struct LightIndexListResourceDesc {
		Vector<uint32_t> lightIndexList;
	public:
		const uint32_t* GetData() const noexcept { return lightIndexList.Items(); }
		uint32_t GetCount() const noexcept { return static_cast<uint32_t>(lightIndexList.Count()); }
	};

	struct LightGridResourceDesc {
		uint32_t lightGrid[ClusterXCount * ClusterYCount * ClusterZCount] = {};
	public:
		const uint32_t* GetData() const noexcept { return lightGrid; }
		static constexpr uint32_t GetCount() noexcept { return ClusterXCount * ClusterYCount * ClusterZCount; }
	};

	struct ReflectionsResourceDesc {
		float diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		float specular[3] = { 0.f, 0.f, 0.f };
		float specularity = 1.f;
		float ambient[3] = { 0.f, 0.f, 0.f };
	public:
		float* GetData() noexcept { return reinterpret_cast<float*>(this); }
		const float* GetData() const noexcept { return reinterpret_cast<const float*>(this); }
		static constexpr uint32_t GetCount() noexcept { return sizeof(ReflectionsResourceDesc) / sizeof(float); }
	public:
		void SetDiffuseColor(Color color) noexcept {
			diffuse[0] = color.R / 255.f;
			diffuse[1] = color.G / 255.f;
			diffuse[2] = color.B / 255.f;
			diffuse[3] = color.Reserved / 255.f;
		}
		void SetSpecularColor(Color color) noexcept {
			specular[0] = color.R / 255.f;
			specular[1] = color.G / 255.f;
			specular[2] = color.B / 255.f;
		}
		void SetAmbientColor(Color color) noexcept {
			ambient[0] = color.R / 255.f;
			ambient[1] = color.G / 255.f;
			ambient[2] = color.B / 255.f;
		}
	};

	/// <summary>
	/// ビューポートのサイズを指定する構造体。
	/// レンダーターゲットのどの範囲を描画に使用するか指定する。
	/// 値はレンダーターゲットのサイズに対する倍率で、
	/// 左上が(0, 0)、右下が(1, 1)である。
	/// (OpenGLでも同じ挙動にするには内部で上下反転する必要あり)
	/// </summary>
	struct ViewportDesc {
		float x = 0.f;
		float y = 0.f;
		float w = 1.f;
		float h = 1.f;
		float zMin = 0.f;
		float zMax = 1.f;
	public:
		ViewportDesc() noexcept = default;
		ViewportDesc(float x, float y, float w, float h, float zMin = 0.f, float zMax = 1.f) noexcept
			: x(x), y(y), w(w), h(h), zMin(zMin), zMax(zMax) {}
	};

	/// <summary>
	/// シザー矩形のサイズを指定する構造体。
	/// レンダーターゲットのどの範囲への描画を有効にするか指定する。
	/// 値はレンダーターゲットのサイズに対する倍率で、
	/// 左上が(0, 0)、右下が(1, 1)である
	/// (OpenGLでも同じ挙動にするには内部で上下反転する必要あり)
	/// </summary>
	struct ScissorRectDesc {
		float x = 0.f;
		float y = 0.f;
		float w = 1.f;
		float h = 1.f;
	public:
		ScissorRectDesc() noexcept = default;
		ScissorRectDesc(float x, float y, float w, float h) noexcept
			: x(x), y(y), w(w), h(h) {}
	};
}
