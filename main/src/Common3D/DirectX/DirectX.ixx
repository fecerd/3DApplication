export module DirectX;
import System;
import Application;	//IWindow
import Common3DInterface;
import DirectXHelper;
import DirectXResource;
export import DXHeader;

using namespace System::Application::Common3D;
using namespace DX;

#undef IID_PPV_ARGS
#define IID_PPV_ARGS(ppType) DX::GetIID(ppType), DX::IID_PPV_ARGS_Helper(ppType)

export namespace System::Application::Windows::DirectX::Internal {
	inline constexpr uint32_t CameraParamID = static_cast<uint32_t>(HeapType::Camera);
	inline constexpr uint32_t ObjectParamID = static_cast<uint32_t>(HeapType::Object);
	inline constexpr uint32_t AnimationParamID = static_cast<uint32_t>(HeapType::Animation);
	inline constexpr uint32_t MaterialParamID = static_cast<uint32_t>(HeapType::Material);
	inline constexpr uint32_t SceneParamID = static_cast<uint32_t>(HeapType::Scene);
	inline constexpr uint32_t ParamCount = 5;
	inline constexpr uint32_t GetParamID(HeapType type) noexcept {
		return static_cast<uint32_t>(type);
	}
	inline constexpr uint32_t DefaultSamplerID = 0;
	inline constexpr uint32_t ToonSamplerID = 1;
	inline constexpr uint32_t SamplerCount = 2;
	ID3D12RootSignature* CreateRootSignature(ID3D12Device& device) noexcept;
	ID3D12RootSignature* CreateRootSignatureForZPrepass(ID3D12Device& device) noexcept {
		D3D12_DESCRIPTOR_RANGE ranges[3] = {};
		//カメラ用
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//オブジェクト用
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//アニメーション用
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 2;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[3] = {};
		//カメラ用
		params[CameraParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[CameraParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[CameraParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[CameraParamID].DescriptorTable.pDescriptorRanges = ranges;
		//オブジェクト用
		params[ObjectParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[ObjectParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[ObjectParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[ObjectParamID].DescriptorTable.pDescriptorRanges = ranges + 1;
		//アニメーション用
		params[AnimationParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[AnimationParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[AnimationParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[AnimationParamID].DescriptorTable.pDescriptorRanges = ranges + 2;

		D3D12_STATIC_SAMPLER_DESC samplers[1] = {};
		//デフォルトサンプラ
		samplers[DefaultSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[DefaultSamplerID].Filter = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[DefaultSamplerID].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[DefaultSamplerID].MinLOD = 0.f;
		samplers[DefaultSamplerID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[DefaultSamplerID].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		samplers[DefaultSamplerID].ShaderRegister = 0;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = 3;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = 1;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
			if (device.CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(&ret)) != S_OK) {
				ret = nullptr;
			}
		}
		if (errorBlob) errorBlob->Release();
		if (rsBlob) rsBlob->Release();
		return ret;
	}
	ID3D12RootSignature* CreateRootSignatureForCluster(ID3D12Device& device) noexcept {
		D3D12_DESCRIPTOR_RANGE ranges[6] = {};
		//カメラ用
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//オブジェクト用
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//アニメーション用
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 2;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(CBV)
		ranges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[3].NumDescriptors = 1;
		ranges[3].BaseShaderRegister = 3;
		ranges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(SRV)
		ranges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[4].NumDescriptors = 4;
		ranges[4].BaseShaderRegister = 0;
		ranges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//シーン用
		ranges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[5].NumDescriptors = 4;
		ranges[5].BaseShaderRegister = 4;
		ranges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[ParamCount] = {};
		//カメラ用
		params[CameraParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[CameraParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[CameraParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[CameraParamID].DescriptorTable.pDescriptorRanges = ranges;
		//オブジェクト用
		params[ObjectParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[ObjectParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[ObjectParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[ObjectParamID].DescriptorTable.pDescriptorRanges = ranges + 1;
		//アニメーション用
		params[AnimationParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[AnimationParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[AnimationParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[AnimationParamID].DescriptorTable.pDescriptorRanges = ranges + 2;
		//マテリアル用
		params[MaterialParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		params[MaterialParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[MaterialParamID].DescriptorTable.NumDescriptorRanges = 2;
		params[MaterialParamID].DescriptorTable.pDescriptorRanges = ranges + 3;
		//シーン用
		params[SceneParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		params[SceneParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[SceneParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[SceneParamID].DescriptorTable.pDescriptorRanges = ranges + 5;

		D3D12_STATIC_SAMPLER_DESC samplers[SamplerCount] = {};
		//デフォルトサンプラ
		samplers[DefaultSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[DefaultSamplerID].Filter = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[DefaultSamplerID].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[DefaultSamplerID].MinLOD = 0.f;
		samplers[DefaultSamplerID].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[DefaultSamplerID].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		samplers[DefaultSamplerID].ShaderRegister = 0;
		//トゥーン用サンプラ
		samplers[ToonSamplerID] = samplers[DefaultSamplerID];
		samplers[ToonSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].ShaderRegister = 1;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = ParamCount;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = SamplerCount;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
			if (device.CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(&ret)) != S_OK) {
				ret = nullptr;
			}
		}
		if (errorBlob) errorBlob->Release();
		if (rsBlob) rsBlob->Release();
		return ret;
	}
	ID3D12RootSignature* CreateRootSignatureForScreen(ID3D12Device& device) noexcept {
		D3D12_DESCRIPTOR_RANGE ranges[3] = {};
		//オブジェクト用
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(CBV)
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(SRV)
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 0;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[2] = {};
		//オブジェクト用
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[0].DescriptorTable.NumDescriptorRanges = 1;
		params[0].DescriptorTable.pDescriptorRanges = ranges;
		//マテリアル用
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable.NumDescriptorRanges = 2;
		params[1].DescriptorTable.pDescriptorRanges = ranges + 1;

		D3D12_STATIC_SAMPLER_DESC samplers[1] = {};
		//デフォルトサンプラ
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[0].Filter = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[0].MinLOD = 0.f;
		samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		samplers[0].ShaderRegister = 0;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = 2;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = 1;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
			if (device.CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(&ret)) != S_OK) {
				ret = nullptr;
			}
		}
		if (errorBlob) errorBlob->Release();
		if (rsBlob) rsBlob->Release();
		return ret;
	}

	inline constexpr D3D12_CULL_MODE GetCullingMode(CullingMode mode) noexcept {
		switch (mode) {
		case CullingMode::Front: return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		case CullingMode::Back: return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		case CullingMode::None:
		default:
			return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
		}
	}
	inline constexpr D3D12_BLEND GetBlendFactor(BlendFactors x) noexcept {
		switch (x) {
		case BlendFactors::Zero: return D3D12_BLEND::D3D12_BLEND_ZERO;
		case BlendFactors::SrcColor: return D3D12_BLEND::D3D12_BLEND_SRC_COLOR;
		case BlendFactors::SrcColorInv: return D3D12_BLEND::D3D12_BLEND_INV_SRC_COLOR;
		case BlendFactors::DstColor: return D3D12_BLEND::D3D12_BLEND_DEST_COLOR;
		case BlendFactors::DstColorInv: return D3D12_BLEND::D3D12_BLEND_INV_DEST_COLOR;
		case BlendFactors::SrcAlpha: return D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
		case BlendFactors::SrcAlphaInv: return D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
		case BlendFactors::DstAlpha: return D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
		case BlendFactors::DstAlphaInv: return D3D12_BLEND::D3D12_BLEND_INV_DEST_ALPHA;
		case BlendFactors::One:
		default:
			return D3D12_BLEND::D3D12_BLEND_ONE;
		}
	}
	inline constexpr D3D12_BLEND_OP GetBlendOp(BlendOp x) noexcept {
		switch (x) {
		case BlendOp::Subtract: return D3D12_BLEND_OP::D3D12_BLEND_OP_SUBTRACT;
		case BlendOp::ReverseSubtract: return D3D12_BLEND_OP::D3D12_BLEND_OP_REV_SUBTRACT;
		case BlendOp::Add:
		default:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
		}
	}

	inline constexpr D3D12_INPUT_ELEMENT_DESC InputElementDesc[7] = {
		{ 
			"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONENO", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHT", 0, DXGI_FORMAT::DXGI_FORMAT_R8_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
	inline constexpr uint32_t InputElementDescCount = sizeof(InputElementDesc) / sizeof(InputElementDesc[0]);
}
using namespace System::Application::Windows::DirectX::Internal;

//Resource
export namespace System::Application::Windows::DirectX {
	class D3DResource : public IResource {
		friend class D3DHeap;
	private:
		ID3D12Resource** m_resources = nullptr;
		uint32_t m_count = 0;
		ResourceFormat m_format = ResourceFormat::Error;
		D3D12_RESOURCE_STATES m_state = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		uint32_t m_current = 0;
	public:
		D3DResource(ID3D12Resource** resources, uint32_t count, ResourceFormat format, D3D12_RESOURCE_STATES state) noexcept
			: m_count(count), m_format(format), m_state(state), m_current(count ? count - 1 : 0)
		{
			m_resources = new ID3D12Resource*[m_count];
			for (size_t i = 0; i < m_count; ++i) m_resources[i] = resources[i];
		}
		~D3DResource() noexcept {
			if (m_resources) {
				for (size_t i = 0; i < m_count; ++i) {
					if (m_resources[i]) m_resources[i]->Release();
				}
			}
			delete[] m_resources;
			m_resources = nullptr;
			m_count = 0;
			m_current = 0;
			m_format = ResourceFormat::Error;
			m_state = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		}
	public:
		void SetState(D3D12_RESOURCE_STATES state) noexcept {
			m_state = state;
		}
		void SetCurrentIndex(uint32_t index) noexcept { m_current = index; }
		D3D12_RESOURCE_STATES GetState() const noexcept { return m_state; }
		ID3D12Resource* GetCurrent() const noexcept {
			return m_resources ? m_resources[m_current] : nullptr;
		}
		ID3D12Resource* GetNext() const noexcept { return m_resources ? m_resources[GetNextIndex()] : nullptr; }
		/// <summary>
		/// 現在のインデックスを取得する。
		/// 描画速度に対し十分な数のリソースを保持しているとき(ダブルバッファリング時)、
		/// このインデックスは最後に描画したリソースを指す
		/// </summary>
		uint32_t GetCurrentIndex() const noexcept { return m_current; }
		/// <summary>
		/// 次のインデックスを取得する。
		/// 描画速度に対し十分な数のリソースを保持しているとき(ダブルバッファリング時)、
		/// このインデックスは次に描画するもしくは現在描画中のリソースを指す
		/// </summary>
		uint32_t GetNextIndex() const noexcept { return m_current + 1 >= m_count ? 0 : m_current + 1; }
	public:
		ResourceFormat GetFormat() const noexcept override {
			return m_format;
		}
		Size<uint32_t> GetSize() const noexcept override {
			ID3D12Resource* resource = GetCurrent();
			if (!resource) return Size<uint32_t>{ 0, 0 };
			D3D12_RESOURCE_DESC desc = resource->GetDesc();
			Size<uint32_t> ret;
			ret.height = 1;
			while (desc.Width > 0xffffffff) {
				ret.height = ret.height << 1;
				desc.Width = desc.Width >> 1;
			}
			ret.width = static_cast<uint32_t>(desc.Width);
			ret.height = static_cast<uint32_t>(ret.height * desc.Height);
			return ret;
		}
		uint32_t GetCount() const noexcept override { return m_count; }
		PlatformID GetPlatformID() const noexcept override { return PlatformID::DirectX; }
		void SetName(const String& name) noexcept override {
			if (!m_resources) return;
			for (uint32_t i = 0; i < m_count; ++i) {
#if false
				m_resources[i]->SetName(String::Joint(name, i).w_str());
#endif
			}
		}
	public:
		void Next() noexcept override {
			if (++m_current >= m_count) m_current = 0;
		}
	public:
		bool UpdateResource(const float* data, size_t count) noexcept override;
		bool UpdateResource(const Matrix* data, size_t count) noexcept override;
		bool UpdateResource(const Drawing::Image& image) noexcept override;
		bool GetData(float* out, size_t count) const noexcept override;
		bool GetData(Matrix* out, size_t count) const noexcept override;
		bool GetData(Drawing::Image& out) const noexcept override;
	};
}

//Heap
export namespace System::Application::Windows::DirectX {
	class D3DHeap : public IHeap {
		ID3D12DescriptorHeap* m_heap = nullptr;
		uint32_t m_viewCount = 0;
		uint32_t m_viewsSetCount = 0;
	public:
		D3DHeap(ID3D12DescriptorHeap* heap, uint32_t viewCount, uint32_t viewsSetCount = 1) noexcept
			: m_heap(heap), m_viewCount(viewCount), m_viewsSetCount(viewsSetCount) {}
		~D3DHeap() noexcept {
			if (m_heap) {
				uint32_t ref = m_heap->Release();
				if (ref) {
					System::Application::Log(ref);
				}
			}
			m_heap = nullptr;
			m_viewCount = 0;
			m_viewsSetCount = 0;
		}
	public:
		ID3D12DescriptorHeap* GetHeap() const noexcept { return m_heap; }
	public:
		uint32_t GetViewCount() const noexcept override { return m_viewCount; }
		uint32_t GetViewsSetCount() const noexcept override { return m_viewsSetCount; }
		uint32_t GetAllViewCount() const noexcept override { return m_viewCount * m_viewsSetCount; }
		PlatformID GetPlatformID() const noexcept override { return PlatformID::DirectX; }
		void SetName(const String& name) noexcept override {
#if false
			if (m_heap) m_heap->SetName(name.w_str());
#endif
		}
	public:
		bool SetView(uint32_t index, ViewFormat format, ManagedObject<IResource> resource) noexcept override;
	};
}

//Bundle
export namespace System::Application::Windows::DirectX {
	class D3DBundle : public ICommandList {
		inline static size_t CBVSRVUAVIncrementSize = 0;
		inline static size_t RTVIncrementSize = 0;
		inline static size_t DSVIncrementSize = 0;
	private:
		ID3D12CommandAllocator* m_allocator = nullptr;
		ID3D12GraphicsCommandList* m_bundle = nullptr;
		Atomic<bool> m_beginning = false;
		Mutex m_mtx;
		ManagedObject<IHeap> m_descriptorHeap;
		bool m_usedParamID[ParamCount] = {};
	public:
		D3DBundle(ID3D12Device& device, uint32_t materialCount) noexcept;
		~D3DBundle() noexcept {
			SafeRelease(m_bundle, m_allocator);
		}
	public:
		void SetName(const String& name) noexcept override {
#if false
			m_allocator->SetName((name).w_str());
			m_bundle->SetName((name).w_str());
#endif
		}
	private:
		bool SetHeap_Internal(const ManagedObject<IHeap>& heap, HeapType type) noexcept;
	public:
		ID3D12GraphicsCommandList* GetBundle() const noexcept {
			return m_bundle;
		}
		ID3D12DescriptorHeap* GetHeap() const noexcept {
			return static_cast<D3DHeap&>(*m_descriptorHeap).GetHeap();
		}
	public:
		PlatformID GetPlatformID() const noexcept override {
			return PlatformID::DirectX;
		}
	public:
		bool BeginCommand() noexcept override;
		bool SetRenderer(ManagedObject<IRenderer> renderer) noexcept override;
		bool SetMesh(ManagedObject<IMeshResource> mesh) noexcept override;
		bool SetCameraHeap(ManagedObject<IHeap> heap) noexcept override {
			return SetHeap_Internal(heap, HeapType::Camera);
		}
		bool SetObjectHeap(ManagedObject<IHeap> heap) noexcept override {
			return SetHeap_Internal(heap, HeapType::Object);
		}
		bool SetAnimationHeap(ManagedObject<IHeap> heap) noexcept override {
			return SetHeap_Internal(heap, HeapType::Animation);
		}
		bool SetMaterialHeap(ManagedObject<IHeap> heap) noexcept override {
			return SetHeap_Internal(heap, HeapType::Material);
		}
		bool SetSceneHeap(ManagedObject<IHeap> heap) noexcept override {
			return SetHeap_Internal(heap, HeapType::Scene);
		}
		bool SetMaterialIndex(uint32_t index) noexcept override;
		bool DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept override;
		bool EndCommand() noexcept override;
		bool UsedHeap(HeapType type) const noexcept override {
			return m_usedParamID[GetParamID(type)];
		}
	};
}

//RenderTarget
export namespace System::Application::Windows::DirectX {
	class D3DRenderTarget : public IRenderTarget {
	protected:
		inline static size_t CBVSRVUAVIncrementSize = 0;
		inline static size_t RTVIncrementSize = 0;
		inline static size_t DSVIncrementSize = 0;
	protected:
		ManagedObject<IHeap> m_rtvHeap;
		ManagedObject<IHeap> m_dsvHeap;
		Vector<ManagedObject<IResource>> m_rtvResources;
		ManagedObject<IResource> m_dsvResources;
		ID3D12CommandAllocator* m_allocator = nullptr;
		ID3D12GraphicsCommandList* m_list = nullptr;
		ID3D12CommandQueue* m_queue = nullptr;
		ID3D12Fence* m_fence = nullptr;
		Atomic<bool> m_beginning = false;
		Mutex m_mtx = Mutex{};
		float m_defaultColor[4] = { 1.f, 1.f, 1.f, 1.f };
	protected:
		D3DRenderTarget() noexcept = default;
	public:
		D3DRenderTarget(
			ID3D12Device& device, uint32_t width, uint32_t height,
			uint32_t bufferCount, uint32_t targetCount, const Drawing::Color& defaultColor
		) noexcept;
		~D3DRenderTarget() noexcept {
			BeginCommand(false, false, false);
			EndCommand(true);
			SafeRelease(m_fence, m_queue, m_list, m_allocator);
		}
	public: /* IRenderTarget */
		bool IsSwapChain() const noexcept override { return false; }
		uint32_t GetBufferCount() const noexcept override {
			if (m_dsvHeap) return m_dsvHeap->GetViewCount();
			else return 0;
		}
		uint32_t GetTargetCount() const noexcept override {
			return static_cast<uint32_t>(m_rtvResources.Count());
		}
		Size<uint32_t> GetSize() const noexcept override {
			if (m_rtvResources.Count()) return m_rtvResources[0]->GetSize();
			else return Size<uint32_t>{ 0, 0 };
		}
		ManagedObject<IResource> GetTexture(uint32_t targetIndex) const noexcept override {
			return m_rtvResources[targetIndex];
		}
		ManagedObject<IResource> GetDepthTexture() const noexcept override {
			return m_dsvResources;
		}
		/// <summary>
		/// バックバッファクリア時の色を設定する。
		/// SwapChainでないRenderTargetに対して変更し、cmd->ClearRenderTarget()を呼び出すと、
		/// "The clear values do not match those passed to resource creation. The clear operation is typically slower as a result; but will still clear to the desired value."
		/// の警告が出され色が変更されない。(パフォーマンスの都合らしい)
		/// SwapChain以外は初期色(白)から変更すべきでない
		/// </summary>
		/// <param name="color">新しく設定する色(RGBA)</param>
		void SetDefaultClearColor(const Drawing::Color& color) noexcept override {}
	public: /* I3DBase */
		PlatformID GetPlatformID() const noexcept override {
			return PlatformID::DirectX;
		}
		void SetName(const String& name) noexcept override {
#if false
			for (uint32_t i = 0, targetCount = GetTargetCount(); i < targetCount; ++i) {
				String tmp = String::Joint(name, u": Target", i);
				m_rtvResources[i]->SetName(tmp + u": RTVResource");
			}
			m_dsvResources->SetName(String::Joint(name, u": DSVResource"));
			m_rtvHeap->SetName(String::Joint(name, u": RTVHeap"));
			m_dsvHeap->SetName(String::Joint(name, u": DSVHeap"));
			m_allocator->SetName((name).w_str());
			m_list->SetName((name).w_str());
			m_queue->SetName((name).w_str());
			m_fence->SetName((name).w_str());
#endif
		}
	private:
		void SetTransitionBarrier(ID3D12Resource& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, uint32_t subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES) {
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = &resource;
			barrier.Transition.Subresource = subResource;
			barrier.Transition.StateBefore = before;
			barrier.Transition.StateAfter = after;
			m_list->ResourceBarrier(1, &barrier);
		}
	protected:
		bool EndCommand_Internal(bool sync, D3D12_RESOURCE_STATES after) noexcept;
	public:
		bool BeginCommand(bool colorClear, bool depthClear, bool stencilClear) noexcept override;
		bool SetViewports(const ViewportDesc* descs, uint32_t count) noexcept override {
			LockGuard lock{ m_mtx };
			if (!m_beginning.load(memory_order::acquire)) return false;
			const Size<uint32_t> size = GetSize();
			D3D12_VIEWPORT* viewports = new D3D12_VIEWPORT[count];
			for (uint32_t i = 0; i < count; ++i) {
				viewports[i].Width = size.width * descs[i].w;
				viewports[i].Height = size.height * descs[i].h;
				viewports[i].TopLeftX = size.width * descs[i].x;
				viewports[i].TopLeftY = size.height * descs[i].y;
				viewports[i].MinDepth = descs[i].zMin;
				viewports[i].MaxDepth = descs[i].zMax;
			}
			m_list->RSSetViewports(count, viewports);
			delete[] viewports;
			return true;
		}
		bool SetViewport(const ViewportDesc& desc) noexcept override {
			LockGuard lock{ m_mtx };
			if (!m_beginning.load(memory_order::acquire)) return false;
			const Size<uint32_t> size = GetSize();
			D3D12_VIEWPORT viewport;
			viewport.Width = size.width * desc.w;
			viewport.Height = size.height * desc.h;
			viewport.TopLeftX = size.width * desc.x;
			viewport.TopLeftY = size.height * desc.y;
			viewport.MinDepth = desc.zMin;
			viewport.MaxDepth = desc.zMax;
			m_list->RSSetViewports(1, &viewport);
			return true;
		}
		bool SetScissorRects(const ScissorRectDesc* descs, uint32_t count) noexcept override {
			LockGuard lock{ m_mtx };
			if (!m_beginning.load(memory_order::acquire)) return false;
			const Size<uint32_t> size = GetSize();
			D3D12_RECT* rects = new D3D12_RECT[count];
			for (uint32_t i = 0; i < count; ++i) {
				rects[i].top = static_cast<LONG>(size.height * descs[i].y);
				rects[i].left = static_cast<LONG>(size.width * descs[i].x);
				rects[i].bottom = static_cast<LONG>(size.height * descs[i].h);
				rects[i].right = static_cast<LONG>(size.width * descs[i].w);
			}
			m_list->RSSetScissorRects(count, rects);
			delete[] rects;
			return true;
		}
		bool SetScissorRect(const ScissorRectDesc& desc) noexcept override {
			LockGuard lock{ m_mtx };
			if (!m_beginning.load(memory_order::acquire)) return false;
			const Size<uint32_t> size = GetSize();
			D3D12_RECT rect;
			rect.top = static_cast<LONG>(size.height * desc.y);
			rect.left = static_cast<LONG>(size.width * desc.x);
			rect.bottom = static_cast<LONG>(size.height * desc.h);
			rect.right = static_cast<LONG>(size.width * desc.w);
			m_list->RSSetScissorRects(1, &rect);
			return true;
		}
		bool SetHeap(HeapType type, const ManagedObject<IHeap>& heap, uint32_t viewsSetIndex = 0) noexcept override;
		bool SetRenderer(ManagedObject<IRenderer> renderer) noexcept override;
		bool SetMesh(ManagedObject<IMeshResource> mesh) noexcept override;
		bool DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept override;
		bool SetCommandList(ManagedObject<ICommandList> list) noexcept override;
		bool EndCommand(bool sync) noexcept override {
			return EndCommand_Internal(sync, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
		bool ResizeBuffer(uint32_t width, uint32_t height) noexcept override;
	};
}

//SwapChain
export namespace System::Application::Windows::DirectX {
	class D3DSwapChain : public D3DRenderTarget {
		IDXGISwapChain4* m_swapChain = nullptr;
	public:
		D3DSwapChain(ID3D12Device& device, IWindow& window, uint32_t bufferCount, const Drawing::Color& defaultColor) noexcept;
		~D3DSwapChain() noexcept {
			if (m_swapChain) {
				m_swapChain->SetFullscreenState(FALSE, nullptr);
			}
			SafeRelease(m_swapChain);
		}
	public: /* IRenderTarget */
		bool IsSwapChain() const noexcept override { return true; }
		Size<uint32_t> GetSize() const noexcept override {
			Size<uint32_t> ret{ 0, 0 };
			if (m_swapChain) m_swapChain->GetSourceSize(&ret.width, &ret.height);
			return ret;
		}
		/// <summary>
		/// バックバッファクリア時の色を設定する。
		/// </summary>
		/// <param name="color">新しく設定する色(RGBA)</param>
		void SetDefaultClearColor(const Drawing::Color& color) noexcept override {
			m_defaultColor[0] = color.R / 255.f;
			m_defaultColor[1] = color.G / 255.f;
			m_defaultColor[2] = color.B / 255.f;
			m_defaultColor[3] = color.Reserved / 255.f;
		}
	public:
		bool EndCommand(bool sync) noexcept override;
		bool ResizeBuffer(uint32_t, uint32_t) noexcept override;
	};
}

//Shader
export namespace System::Application::Windows::DirectX {
	class D3DShader : public IShader {
		ID3DBlob* m_binary = nullptr;
	public:
		D3DShader(ID3DBlob* binary) noexcept : m_binary(binary) {}
		~D3DShader() noexcept {
			SafeRelease(m_binary);
		}
	public:
		ID3DBlob* GetBinary() const noexcept { return m_binary; }
	public:
		PlatformID GetPlatformID() const noexcept override {
			return PlatformID::DirectX;
		}
	};
}

//PipelineState
export namespace System::Application::Windows::DirectX {
	class D3DRenderer : public IRenderer {
		ID3D12PipelineState* m_pipelineState = nullptr;
	public:
		D3DRenderer(ID3D12PipelineState* pipelineState) noexcept
			: m_pipelineState(pipelineState) {}
		~D3DRenderer() noexcept {
			SafeRelease(m_pipelineState);
		}
	public:
		ID3D12PipelineState* GetPipelineState() const noexcept {
			return m_pipelineState;
		}
	public:
		PlatformID GetPlatformID() const noexcept override {
			return PlatformID::DirectX;
		}
	};
}

//MeshResource
export namespace System::Application::Windows::DirectX {
	class D3DMeshResource : public IMeshResource {
		ID3D12Resource* m_vertexBuffer = nullptr;
		ID3D12Resource* m_indexBuffer = nullptr;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
		Vector<uint32_t> m_indexCountPerMaterial;
	public:
		D3DMeshResource(
			ID3D12Resource* vb, ID3D12Resource* ib,
			uint32_t vertexCount, uint32_t indexCount,
			const Vector<uint32_t>& indexCountsPerMaterial
		) noexcept
			: m_vertexBuffer(vb), m_indexBuffer(ib)
			, m_vertexCount(vertexCount), m_indexCount(indexCount)
			, m_indexCountPerMaterial(indexCountsPerMaterial) {}
		~D3DMeshResource() noexcept {
			SafeRelease(m_vertexBuffer, m_indexBuffer);
			m_vertexCount = 0;
			m_indexCount = 0;
		}
	private:
		static consteval DXGI_FORMAT GetFormat(size_t indexTypeSize) noexcept {
			switch (indexTypeSize) {
			case 1: return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
			case 2: return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
			case 4: return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			default: return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			}
		}
	public:
		D3D12_VERTEX_BUFFER_VIEW GetVBV() const noexcept {
			using VertexType = Traits::remove_cvref_t<Traits::remove_pointer_t<decltype(MeshDesc::vertices.Items())>>;
			D3D12_VERTEX_BUFFER_VIEW ret{};
			ret.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			ret.SizeInBytes = sizeof(VertexType) * m_vertexCount;
			ret.StrideInBytes = sizeof(VertexType);
			return ret;
		}
		D3D12_INDEX_BUFFER_VIEW GetIBV() const noexcept {
			using IndexType = Traits::remove_cvref_t<Traits::remove_pointer_t<decltype(MeshDesc::indices.Items())>>;
			D3D12_INDEX_BUFFER_VIEW ret{};
			ret.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
			ret.Format = GetFormat(sizeof(IndexType));
			ret.SizeInBytes = sizeof(IndexType) * m_indexCount;
			return ret;
		}
	public:
		uint32_t GetVertexCount() const noexcept override {
			return m_vertexCount;
		}
		uint32_t GetIndexCount() const noexcept override {
			return m_indexCount;
		}
		uint32_t GetMaterialCount() const noexcept override {
			return static_cast<uint32_t>(m_indexCountPerMaterial.Count());
		}
		uint32_t GetIndexCountsInMaterial(uint32_t materialIndex) const noexcept override {
			return m_indexCountPerMaterial[materialIndex];
		}
		PlatformID GetPlatformID()	const noexcept override {
			return PlatformID::DirectX;
		}
	};
}

//CopyCommandList
namespace System::Application::Windows::DirectX {
	struct CommandAllocator {
		ID3D12CommandAllocator* allocator = nullptr;
		uint64_t fenceVal = 0;
		List<IUnknown*> releaseList;
	public:
		CommandAllocator() noexcept = default;
		CommandAllocator(const CommandAllocator&) noexcept = delete;
		CommandAllocator(CommandAllocator&&) noexcept = default;
		~CommandAllocator() noexcept = default;
	public:
		CommandAllocator& operator=(const CommandAllocator&) noexcept = delete;
		CommandAllocator& operator=(CommandAllocator&&) noexcept = default;		
	};

	struct UploadResourceCache {
		ID3D12Resource* resource = nullptr;
		uint64_t fenceVal = 0;
		size_t typeSize = 0;
		uint64_t width = 0;
		uint32_t height = 0;
	};

	class CopyCommandList {
		List<CommandAllocator> m_allocatorList;
		ID3D12GraphicsCommandList* m_list = nullptr;
		ID3D12CommandQueue* m_queue = nullptr;
		ID3D12Fence* m_fence = nullptr;
		uint64_t m_fenceVal = 0;
		bool m_beginning = false;
		ID3D12Device& m_device;
		List<UploadResourceCache> m_uploadCache;
	private:
		void Signal(uint64_t val) noexcept {
			m_queue->Signal(m_fence, val);
		}
		bool Wait(uint64_t val, bool wait) noexcept {
			const uint64_t completedVal = m_fence->GetCompletedValue();
			//val未満の値が設定されている、もしくはvalがオーバーフローしている
			if ((completedVal < val) || (completedVal - val > 0xffffffffull)) {
				if (!wait) return false;
				HANDLE fenceEv = CreateEvent(nullptr, false, false, nullptr);
				m_fence->SetEventOnCompletion(val, fenceEv);
				WaitForSingleObject(fenceEv, INFINITE);
				CloseHandle(fenceEv);
				return true;
			}
			else return true;
		}
	public:
		CopyCommandList(ID3D12Device& device) noexcept : m_device(device) {
			m_device.AddRef();
			m_queue = Helpers::CreateCommandQueue(m_device);
			m_fence = Helpers::CreateFence(m_device);
			CommandAllocator allocator;
			allocator.allocator = Helpers::CreateCommandAllocator(m_device);
			m_list = Helpers::CreateCommandList(m_device, *allocator.allocator);
			m_list->Close();
			allocator.fenceVal = m_fenceVal + 1;
			m_allocatorList.PushBack(System::move(allocator));
		}
		~CopyCommandList() noexcept {
			if (m_beginning) EndCommand();
			Signal(++m_fenceVal);
			Wait(m_fenceVal, true);
			for (UploadResourceCache& x : m_uploadCache) {
				SafeRelease(x.resource);
			}
			for (CommandAllocator& x : m_allocatorList) {
				x.allocator->Reset();
				if (m_list) {
					m_list->Reset(x.allocator, nullptr);
					SafeRelease(m_list);
				}
				for (IUnknown*& i : x.releaseList) SafeRelease(i);
				x.releaseList.Clear();
				SafeRelease(x.allocator);
			}
			SafeRelease(m_fence, m_queue);
			m_device.Release();
		}
	public:
		bool BeginCommand() noexcept {
			if (m_beginning) return false;
			uint32_t del = 0;
			for (auto ite = m_allocatorList.begin(), e = m_allocatorList.end(); ite != e;) {
				CommandAllocator& allocator = *ite;
				if (Wait(allocator.fenceVal, false)) {
					allocator.allocator->Reset();
					for (IUnknown*& i : allocator.releaseList) SafeRelease(i);
					allocator.releaseList.Clear();
					if (!m_beginning) {
						m_list->Reset(allocator.allocator, nullptr);
						allocator.fenceVal = ++m_fenceVal;
						m_beginning = true;
					}
					else {
						++del;
						if (del > 10) {
							m_allocatorList.Remove(ite);
						}
					}
				}
				++ite;
			}
			if (m_beginning) return true;
			CommandAllocator allocator;
			allocator.allocator = Helpers::CreateCommandAllocator(m_device);
			allocator.fenceVal = ++m_fenceVal;
			m_list->Reset(allocator.allocator, nullptr);
			m_allocatorList.PushBack(System::move(allocator));
			m_beginning = true;
			return true;
		}
		bool ResourceBarrier(uint32_t count, const D3D12_RESOURCE_BARRIER* barriers) noexcept {
			if (!m_beginning) return false;
			m_list->ResourceBarrier(count, barriers);
			return true;
		}
		bool CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION* pDst, uint32_t dstX, uint32_t dstY, uint32_t dstZ, const D3D12_TEXTURE_COPY_LOCATION* pSrc, const D3D12_BOX* pSrcBox) noexcept {
			if (!m_beginning) return false;
			m_list->CopyTextureRegion(pDst, dstX, dstY, dstZ, pSrc, pSrcBox);
			return true;
		}
		bool AddReleaseObject(IUnknown* obj) noexcept {
			if (!m_beginning) return false;
			for (CommandAllocator& x : m_allocatorList) {
				if (x.fenceVal == m_fenceVal) {
					x.releaseList.PushBack(obj);
					return true;
				}
			}
			return false;
		}
		bool EndCommand() noexcept {
			if (!m_beginning) return false;
			m_list->Close();
			ID3D12CommandList* lists[] = { m_list };
			m_queue->ExecuteCommandLists(1, lists);
			Signal(m_fenceVal);
			m_beginning = false;
			return true;
		}
	public:
		ID3D12Resource* GetUploadResourceCache(size_t typeSize, uint64_t width, uint32_t height, bool alignment = true, uint32_t remainCount = 1) noexcept {
			for (UploadResourceCache& x : m_uploadCache) {
				if (Wait(x.fenceVal, false)) {
					if (x.typeSize == typeSize && x.width == width && x.height == height) {
						x.fenceVal = m_fenceVal + remainCount + 1;
						return x.resource;
					}
				}
			}
			D3D12_HEAP_PROPERTIES prop{};
			prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 0;
			prop.VisibleNodeMask = 0;
			D3D12_RESOURCE_DESC desc{};
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			//データサイズ
			desc.Width = alignment
				? Helpers::GetAlignmentedSize(width * typeSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * height
				: width * typeSize * height;
			desc.Height = 1;	//Widthに高さを含めているため一次元
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			ID3D12Resource* ret = nullptr;
			if (m_device.CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ret)) != S_OK) {
				return nullptr;
			}
			UploadResourceCache cache;
			cache.resource = ret;
			cache.typeSize = typeSize;
			cache.width = width;
			cache.height = height;
			cache.fenceVal = m_fenceVal + remainCount + 1;
			m_uploadCache.PushBack(System::move(cache));
			return ret;
		}	
	};
}

//Manager
export namespace System::Application::Windows::DirectX {
	class DirectXManager {
		DirectXManager();
		~DirectXManager() noexcept;
	private:
		inline static ID3D12DebugDevice* debugDevice = nullptr;
		static void DebugReport() noexcept;
	private:
		inline static DirectXManager* pManager = nullptr;
	public:
		static ExclusiveObject<DirectXManager> GetManager();
		static bool CloseManager();
	private:/* 一度初期化すればよいメンバ */
		IDXGIFactory4* m_factory = nullptr;
		IDXGIAdapter* m_adapter = nullptr;
		ID3D12Device* m_device = nullptr;
		size_t m_CBVSRVUAVIncrementSize = 0;
		size_t m_RTVIncrementSize = 0;
		size_t m_DSVIncrementSize = 0;
		HashMap<RenderingMode, ID3D12RootSignature*> m_rootSignatures = 2;
		ManagedObject<IResource> m_whiteTexture;
		ManagedObject<IResource> m_blackTexture;
		ManagedObject<IResource> m_toonTexture;
		UniquePtr<CopyCommandList> m_copyCommandList = nullptr;
	private:
		VectorBase<ManagedObject<IShader>> m_shaders;
		VectorBase<ManagedObject<IRenderer>> m_renderers;
	public:/* staticな操作 */
		static D3D12_RESOURCE_BARRIER CreateTransitionBarrier(
			ID3D12Resource& resource, uint32_t subResource,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after
		);
		template<class T>
		static bool UpdateUploadResource(ID3D12Resource& resource, const T* data, size_t count) noexcept {
			T* map = nullptr;
			if (resource.Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
			Memory::Copy(map, data, sizeof(T) * count, true);
			resource.Unmap(0, nullptr);
			return true;
		}
		static bool UpdateUploadResource(ID3D12Resource& resource, const Drawing::Image& image) noexcept;
		static ID3DBlob* LoadShader(const ShaderDesc& desc) noexcept;
	public:
		ID3D12RootSignature* GetRootSignature(RenderingMode mode = RenderingMode::Standard) const noexcept;
	public:/* ID3DDeviceが必要な操作 */
		ID3D12Resource* CreateUploadResource(size_t typeSize, uint64_t width, uint32_t height, bool alignment = true) noexcept;
		ID3D12Resource* CreateReadbackResource(size_t typeSize, uint64_t width, uint64_t height, bool alignment = true) noexcept;
		ID3D12Resource* CreateTextureResource(bool useRenderTarget, uint32_t width, uint32_t height) noexcept;
		ID3D12Resource* CreateDepthResource(uint32_t width, uint32_t height) noexcept;
	public:
		bool CopyTextureResource(ID3D12Resource& texture, ID3D12Resource& upload, D3D12_RESOURCE_STATES currentState) noexcept;
		bool UpdateTextureResource(ID3D12Resource& resource, D3D12_RESOURCE_STATES currentState, const Drawing::Image& image) noexcept;
	public:
		bool GetTextureResourceImage(ID3D12Resource& texture, D3D12_RESOURCE_STATES currentState, Drawing::Image& out) noexcept;
		ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, bool shaderVisible = true) noexcept;
		bool CreateView(ID3D12DescriptorHeap& heap, ID3D12Resource* resource, ViewFormat format, uint32_t index, uint32_t elementCountForUAV = 0, uint32_t structureByteStride = 0) noexcept;
		ID3D12PipelineState* CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) noexcept;
	public: /* ID3DDevice, IDXGIFactoryが必要な操作 */
		IDXGISwapChain4* CreateSwapChain(ID3D12CommandQueue& queue, HWND hWnd, uint32_t bufferCount) noexcept;
	public:/* より具体的な操作 */
		IResource* CreateResource(const float* data, size_t count) noexcept;
		IResource* CreateResource(const uint32_t* data, size_t count) noexcept;
		IResource* CreateResource(const Matrix* data, size_t count) noexcept;
		IResource* CreateResource(const Drawing::Image& image) noexcept;
		IHeap* CreateHeap(uint32_t viewCount, uint32_t viewsSetCount, bool shaderVisible) noexcept;
		IRenderTarget* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Drawing::Color& defaultColor) noexcept;
		IRenderTarget* CreateRenderTarget(IWindow& window, uint32_t bufferCount, const Drawing::Color& defaultColor);
		ManagedObject<IShader> CreateShader(const ShaderDesc& desc) noexcept;
		ManagedObject<IRenderer> CreateRenderer(const String& name, const RendererDesc& desc) noexcept;
		IMeshResource* CreateMeshResource(const MeshDesc& desc) noexcept;
		IHeap* CreateHeap(HeapType type, uint32_t setCount = 1) noexcept;
		bool CopyView(ManagedObject<IHeap>& dst, uint32_t dstIndex, const ManagedObject<IHeap>& src, uint32_t srcIndex, uint32_t count = 1) noexcept;
		ICommandList* CreateCommandList(uint32_t materialCount) noexcept;
	};
}
