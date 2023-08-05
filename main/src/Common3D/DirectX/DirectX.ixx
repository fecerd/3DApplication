module;
#include "DirectXHeader.hpp"
export module DirectX;
import System;
import Application;	//IWindow
import Common3DInterface;
import DirectXHelper;
import DirectXResource;
using namespace System;
using namespace System::Drawing;
using namespace System::Application;
using namespace System::Application::Common3D;

namespace System::Application::Windows::DirectX::Internal {
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
	ID3D12RootSignature* CreateRootSignature(ID3D12Device& device) noexcept {
		D3D12_DESCRIPTOR_RANGE ranges[6] = {};
		//カメラ用
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//オブジェクト用
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//アニメーション用
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 2;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(CBV)
		ranges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[3].NumDescriptors = 1;
		ranges[3].BaseShaderRegister = 3;
		ranges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(SRV)
		ranges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[4].NumDescriptors = 4;
		ranges[4].BaseShaderRegister = 0;
		ranges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//シーン用
		ranges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[5].NumDescriptors = 1;
		ranges[5].BaseShaderRegister = 4;
		ranges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[ParamCount] = {};
		//カメラ用
		params[CameraParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[CameraParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[CameraParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[CameraParamID].DescriptorTable.pDescriptorRanges = ranges;
		//オブジェクト用
		params[ObjectParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[ObjectParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[ObjectParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[ObjectParamID].DescriptorTable.pDescriptorRanges = ranges + 1;
		//アニメーション用
		params[AnimationParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[AnimationParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[AnimationParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[AnimationParamID].DescriptorTable.pDescriptorRanges = ranges + 2;
		//マテリアル用
		params[MaterialParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		params[MaterialParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[MaterialParamID].DescriptorTable.NumDescriptorRanges = 2;
		params[MaterialParamID].DescriptorTable.pDescriptorRanges = ranges + 3;
		//シーン用
		params[SceneParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		params[SceneParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[SceneParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[SceneParamID].DescriptorTable.pDescriptorRanges = ranges + 5;

		D3D12_STATIC_SAMPLER_DESC samplers[SamplerCount] = {};
		//デフォルトサンプラ
		samplers[DefaultSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[DefaultSamplerID].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[DefaultSamplerID].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[DefaultSamplerID].MinLOD = 0.f;
		samplers[DefaultSamplerID].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[DefaultSamplerID].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[DefaultSamplerID].ShaderRegister = 0;
		//トゥーン用サンプラ
		samplers[ToonSamplerID] = samplers[DefaultSamplerID];
		samplers[ToonSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].ShaderRegister = 1;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = ParamCount;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = SamplerCount;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
			if (device.CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(&ret)) != S_OK) {
				ret = nullptr;
			}
		}
		if (errorBlob) errorBlob->Release();
		if (rsBlob) rsBlob->Release();
		return ret;
	}

	ID3D12RootSignature* CreateRootSignatureForZPrepass(ID3D12Device& device) noexcept {
		D3D12_DESCRIPTOR_RANGE ranges[3] = {};
		//カメラ用
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//オブジェクト用
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//アニメーション用
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 2;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[3] = {};
		//カメラ用
		params[CameraParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[CameraParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[CameraParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[CameraParamID].DescriptorTable.pDescriptorRanges = ranges;
		//オブジェクト用
		params[ObjectParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[ObjectParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[ObjectParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[ObjectParamID].DescriptorTable.pDescriptorRanges = ranges + 1;
		//アニメーション用
		params[AnimationParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[AnimationParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[AnimationParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[AnimationParamID].DescriptorTable.pDescriptorRanges = ranges + 2;

		D3D12_STATIC_SAMPLER_DESC samplers[1] = {};
		//デフォルトサンプラ
		samplers[DefaultSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[DefaultSamplerID].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[DefaultSamplerID].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[DefaultSamplerID].MinLOD = 0.f;
		samplers[DefaultSamplerID].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[DefaultSamplerID].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[DefaultSamplerID].ShaderRegister = 0;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = 3;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = 1;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
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
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//オブジェクト用
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//アニメーション用
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 2;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(CBV)
		ranges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[3].NumDescriptors = 1;
		ranges[3].BaseShaderRegister = 3;
		ranges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(SRV)
		ranges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[4].NumDescriptors = 4;
		ranges[4].BaseShaderRegister = 0;
		ranges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//シーン用
		ranges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[5].NumDescriptors = 4;
		ranges[5].BaseShaderRegister = 4;
		ranges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[ParamCount] = {};
		//カメラ用
		params[CameraParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[CameraParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[CameraParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[CameraParamID].DescriptorTable.pDescriptorRanges = ranges;
		//オブジェクト用
		params[ObjectParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[ObjectParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[ObjectParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[ObjectParamID].DescriptorTable.pDescriptorRanges = ranges + 1;
		//アニメーション用
		params[AnimationParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[AnimationParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[AnimationParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[AnimationParamID].DescriptorTable.pDescriptorRanges = ranges + 2;
		//マテリアル用
		params[MaterialParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		params[MaterialParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[MaterialParamID].DescriptorTable.NumDescriptorRanges = 2;
		params[MaterialParamID].DescriptorTable.pDescriptorRanges = ranges + 3;
		//シーン用
		params[SceneParamID].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		params[SceneParamID].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[SceneParamID].DescriptorTable.NumDescriptorRanges = 1;
		params[SceneParamID].DescriptorTable.pDescriptorRanges = ranges + 5;

		D3D12_STATIC_SAMPLER_DESC samplers[SamplerCount] = {};
		//デフォルトサンプラ
		samplers[DefaultSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[DefaultSamplerID].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[DefaultSamplerID].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[DefaultSamplerID].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[DefaultSamplerID].MinLOD = 0.f;
		samplers[DefaultSamplerID].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[DefaultSamplerID].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[DefaultSamplerID].ShaderRegister = 0;
		//トゥーン用サンプラ
		samplers[ToonSamplerID] = samplers[DefaultSamplerID];
		samplers[ToonSamplerID].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[ToonSamplerID].ShaderRegister = 1;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = ParamCount;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = SamplerCount;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
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
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1;
		ranges[0].BaseShaderRegister = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(CBV)
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[1].NumDescriptors = 1;
		ranges[1].BaseShaderRegister = 1;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//マテリアル用(SRV)
		ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[2].NumDescriptors = 1;
		ranges[2].BaseShaderRegister = 0;
		ranges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER params[2] = {};
		//オブジェクト用
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[0].DescriptorTable.NumDescriptorRanges = 1;
		params[0].DescriptorTable.pDescriptorRanges = ranges;
		//マテリアル用
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable.NumDescriptorRanges = 2;
		params[1].DescriptorTable.pDescriptorRanges = ranges + 1;

		D3D12_STATIC_SAMPLER_DESC samplers[1] = {};
		//デフォルトサンプラ
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[0].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[0].MinLOD = 0.f;
		samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[0].ShaderRegister = 0;

		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = 2;
		desc.pStaticSamplers = samplers;
		desc.NumStaticSamplers = 1;
		ID3DBlob* rsBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		ID3D12RootSignature* ret = nullptr;
		if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rsBlob, &errorBlob) == S_OK) {
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
		case CullingMode::Front: return D3D12_CULL_MODE_FRONT;
		case CullingMode::Back: return D3D12_CULL_MODE_BACK;
		case CullingMode::None:
		default:
			return D3D12_CULL_MODE_NONE;
		}
	}
	inline constexpr D3D12_BLEND GetBlendFactor(BlendFactors x) noexcept {
		switch (x) {
		case BlendFactors::Zero: return D3D12_BLEND_ZERO;
		case BlendFactors::SrcColor: return D3D12_BLEND_SRC_COLOR;
		case BlendFactors::SrcColorInv: return D3D12_BLEND_INV_SRC_COLOR;
		case BlendFactors::DstColor: return D3D12_BLEND_DEST_COLOR;
		case BlendFactors::DstColorInv: return D3D12_BLEND_INV_DEST_COLOR;
		case BlendFactors::SrcAlpha: return D3D12_BLEND_SRC_ALPHA;
		case BlendFactors::SrcAlphaInv: return D3D12_BLEND_INV_SRC_ALPHA;
		case BlendFactors::DstAlpha: return D3D12_BLEND_DEST_ALPHA;
		case BlendFactors::DstAlphaInv: return D3D12_BLEND_INV_DEST_ALPHA;
		case BlendFactors::One:
		default:
			return D3D12_BLEND_ONE;
		}
	}
	inline constexpr D3D12_BLEND_OP GetBlendOp(BlendOp x) noexcept {
		switch (x) {
		case BlendOp::Subtract: return D3D12_BLEND_OP_SUBTRACT;
		case BlendOp::ReverseSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
		case BlendOp::Add:
		default:
			return D3D12_BLEND_OP_ADD;
		}
	}

	inline constexpr D3D12_INPUT_ELEMENT_DESC InputElementDesc[7] = {
		{ 
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONENO", 0, DXGI_FORMAT_R32G32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHT", 0, DXGI_FORMAT_R8_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
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
		D3D12_RESOURCE_STATES m_state = D3D12_RESOURCE_STATE_COMMON;
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
			m_state = D3D12_RESOURCE_STATE_COMMON;
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
			for (uint32_t i = 0; i < m_count; ++i) m_resources[i]->SetName(String::Joint(name, i).w_str());
		}
	public:
		void Next() noexcept override {
			if (++m_current >= m_count) m_current = 0;
		}
	public:
		bool UpdateResource(const float* data, size_t count) noexcept override;
		bool UpdateResource(const Matrix* data, size_t count) noexcept override;
		bool UpdateResource(const Image& image) noexcept override;
		bool GetData(float* out, size_t count) const noexcept override;
		bool GetData(Matrix* out, size_t count) const noexcept override;
		bool GetData(Image& out) const noexcept override;
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
			if (m_heap) m_heap->SetName(name.w_str());
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
			Helpers::SafeRelease(m_bundle, m_allocator);
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
			uint32_t bufferCount, uint32_t targetCount, const Color& defaultColor
		) noexcept;
		~D3DRenderTarget() noexcept {
			BeginCommand(false, false, false);
			EndCommand(true);
			Helpers::SafeRelease(m_fence, m_queue, m_list, m_allocator);
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
		void SetDefaultClearColor(const Color& color) noexcept override {}
	public: /* I3DBase */
		PlatformID GetPlatformID() const noexcept override {
			return PlatformID::DirectX;
		}
		void SetName(const String& name) noexcept override {
			for (uint32_t i = 0, targetCount = GetTargetCount(); i < targetCount; ++i) {
				String tmp = String::Joint(name, u": Target", i);
				m_rtvResources[i]->SetName(tmp + u": RTVResource");
			}
			m_dsvResources->SetName(String::Joint(name, u": DSVResource"));
			m_rtvHeap->SetName(String::Joint(name, u": RTVHeap"));
			m_dsvHeap->SetName(String::Joint(name, u": DSVHeap"));
		}
	private:
		void SetTransitionBarrier(ID3D12Resource& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, uint32_t subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES) {
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
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
			return EndCommand_Internal(sync, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
		bool ResizeBuffer(uint32_t width, uint32_t height) noexcept override;
	};
}

//SwapChain
export namespace System::Application::Windows::DirectX {
	class D3DSwapChain : public D3DRenderTarget {
		IDXGISwapChain4* m_swapChain = nullptr;
	public:
		D3DSwapChain(ID3D12Device& device, IWindow& window, uint32_t bufferCount, const Color& defaultColor) noexcept;
		~D3DSwapChain() noexcept {
			if (m_swapChain) {
				m_swapChain->SetFullscreenState(FALSE, nullptr);
			}
			Helpers::SafeRelease(m_swapChain);
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
		void SetDefaultClearColor(const Color& color) noexcept override {
			m_defaultColor[0] = color.R / 255.f;
			m_defaultColor[1] = color.G / 255.f;
			m_defaultColor[2] = color.B / 255.f;
			m_defaultColor[3] = color.Reserved / 255.f;
		}
	public:
		bool EndCommand(bool sync) noexcept override {
			if (EndCommand_Internal(sync, D3D12_RESOURCE_STATE_PRESENT)) {
				if (sync) return m_swapChain->Present(1, 0) == S_OK;
				else return true;
			}
			else return false;
		}
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
			Helpers::SafeRelease(m_binary);
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
			Helpers::SafeRelease(m_pipelineState);
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
			Helpers::SafeRelease(m_vertexBuffer, m_indexBuffer);
			m_vertexCount = 0;
			m_indexCount = 0;
		}
	private:
		static consteval DXGI_FORMAT GetFormat(size_t indexTypeSize) noexcept {
			switch (indexTypeSize) {
			case 1: return DXGI_FORMAT_R8_UINT;
			case 2: return DXGI_FORMAT_R16_UINT;
			case 4: return DXGI_FORMAT_R32_UINT;
			default: return DXGI_FORMAT_UNKNOWN;
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
		PlatformID GetPlatformID()	const noexcept {
			return PlatformID::DirectX;
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
		inline static Mutex manager_mtx;
		inline static DirectXManager* pManager = nullptr;
		static void DebugReport() noexcept {
			if (debugDevice) {
				debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
			}
			Helpers::SafeRelease(debugDevice);
		}
	public:
		static DirectXManager& GetManager() {
			LockGuard lock{ manager_mtx };
			if (!pManager) {
				pManager = new DirectXManager();
				atexit([] { LockGuard lock{ manager_mtx }; delete pManager; pManager = nullptr; });
			}
			return *pManager;
		}
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
	private:
		Vector<ManagedObject<IShader>> m_shaders;
		Vector<ManagedObject<IRenderer>> m_renderers;
	public:/* staticな操作 */
		static D3D12_RESOURCE_BARRIER CreateTransitionBarrier(
			ID3D12Resource& resource, uint32_t subResource,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after
		) {
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = &resource;
			barrier.Transition.Subresource = subResource;
			barrier.Transition.StateBefore = before;
			barrier.Transition.StateAfter = after;
			return barrier;
		}
		template<class T>
		static bool UpdateUploadResource(ID3D12Resource& resource, const T* data, size_t count) noexcept {
			T* map = nullptr;
			if (resource.Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
			Memory::Copy(map, data, sizeof(T) * count, true);
			resource.Unmap(0, nullptr);
			return true;
		}
		static bool UpdateUploadResource(ID3D12Resource& resource, const Image& image) noexcept {
			Pixel* map = nullptr;
			if (resource.Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
			const uint32_t width = image.Width();
			const uint32_t height = image.Height();
			const size_t rowPitch = width * sizeof(Pixel);
			const size_t alignment = Helpers::GetAlignmentedSize(rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
			const Pixel* pixels = image.Data();
			for (uint32_t y = 0; y < height; ++y) {
				System::Memory::Copy(map, pixels, rowPitch, true);
				pixels += width;
				map = reinterpret_cast<Pixel*>(reinterpret_cast<uint8_t*>(map) + alignment);
			}
			resource.Unmap(0, nullptr);
			return true;
		}
		static ID3DBlob* LoadShader(const ShaderDesc& desc) noexcept {
			if (desc.filepath.IsNullOrEmpty()) return nullptr;
			auto entry = desc.entry.Convert<char>();
			auto target = desc.target.Convert<char>();
			ID3DBlob* result = nullptr;
			ID3DBlob* error = nullptr;
			HRESULT hr = D3DCompileFromFile(
				desc.filepath.w_str(), nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entry.c_str(), target.c_str(),
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0, &result, &error
			);
			if (error) {
				String str(static_cast<const char*>(error->GetBufferPointer()), error->GetBufferSize());
				System::Application::Log(str);
				error->Release();
			}
			if (hr != S_OK) {
				if (result) result->Release();
				return nullptr;
			}
			else return result;
		}
	public:
		ID3D12RootSignature* GetRootSignature(RenderingMode mode = RenderingMode::Standard) const noexcept {
			return m_rootSignatures.At(mode);
		}
	public:/* ID3DDeviceが必要な操作 */
		ID3D12Resource* CreateUploadResource(size_t typeSize, uint64_t width, uint32_t height, bool alignment = true) noexcept {
			D3D12_HEAP_PROPERTIES prop{};
			prop.Type = D3D12_HEAP_TYPE_UPLOAD;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 0;
			prop.VisibleNodeMask = 0;
			D3D12_RESOURCE_DESC desc{};
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			//データサイズ
			desc.Width = alignment
				? Helpers::GetAlignmentedSize(width * typeSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * height
				: width * typeSize * height;
			desc.Height = 1;	//Widthに高さを含めているため一次元
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			ID3D12Resource* ret = nullptr;
			if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ret)) != S_OK) {
				return nullptr;
			}
			else return ret;
		}
		ID3D12Resource* CreateReadbackResource(size_t typeSize, uint64_t width, uint64_t height, bool alignment = true) noexcept {
			D3D12_HEAP_PROPERTIES prop{};
			prop.Type = D3D12_HEAP_TYPE_READBACK;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 0;
			prop.VisibleNodeMask = 0;
			D3D12_RESOURCE_DESC desc{};
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			//データサイズ
			desc.Width = alignment
				? Helpers::GetAlignmentedSize(width * typeSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * height
				: width * typeSize * height;
			desc.Height = 1;	//Widthに高さを含めているため一次元
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			ID3D12Resource* ret = nullptr;
			if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&ret)) != S_OK) {
				return nullptr;
			}
			else return ret;
		}
		ID3D12Resource* CreateTextureResource(bool useRenderTarget, uint32_t width, uint32_t height) noexcept {
			D3D12_HEAP_PROPERTIES prop{};
			prop.Type = D3D12_HEAP_TYPE_DEFAULT;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 0;
			prop.VisibleNodeMask = 0;
			D3D12_RESOURCE_DESC desc{};
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Width = width;	//画像の幅(px)
			desc.Height = height;	//画像の高さ(px)
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Flags = useRenderTarget ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : D3D12_RESOURCE_FLAG_NONE;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			ID3D12Resource* ret = nullptr;
			HRESULT hr = S_OK;
			if (useRenderTarget) {
				D3D12_CLEAR_VALUE clear{};
				clear.Color[0] = 1.f;
				clear.Color[1] = 1.f;
				clear.Color[2] = 1.f;
				clear.Color[3] = 1.f;
				clear.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				hr = m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clear, IID_PPV_ARGS(&ret));
			}
			else {
				hr = m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&ret));
			}
			if (hr != S_OK) return nullptr;
			else return ret;
		}
		ID3D12Resource* CreateDepthResource(uint32_t width, uint32_t height) noexcept {
			D3D12_RESOURCE_DESC desc{};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Width = width;
			desc.Height = height;
			desc.DepthOrArraySize = 1;
			desc.Format = DXGI_FORMAT_D32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			D3D12_HEAP_PROPERTIES prop{};
			prop.Type = D3D12_HEAP_TYPE_DEFAULT;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 0;
			prop.VisibleNodeMask = 0;
			D3D12_CLEAR_VALUE clear{};
			clear.DepthStencil.Depth = 1.0f;
			clear.Format = DXGI_FORMAT_D32_FLOAT;
			ID3D12Resource* ret = nullptr;
			if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&ret)) != S_OK) {
				return nullptr;
			}
			else return ret;
		}
	public:

		bool CopyTextureResource(ID3D12Resource& texture, ID3D12Resource& upload, D3D12_RESOURCE_STATES currentState) noexcept;
		bool UpdateTextureResource(ID3D12Resource& resource, D3D12_RESOURCE_STATES currentState, const Image& image) noexcept;
	public:
		bool GetTextureResourceImage(ID3D12Resource& texture, D3D12_RESOURCE_STATES currentState, Image& out) noexcept {
			D3D12_RESOURCE_DESC textureDesc = texture.GetDesc();
			ID3D12Resource* readback = CreateReadbackResource(sizeof(Pixel), textureDesc.Width, textureDesc.Height);
			if (!readback) return false;
			D3D12_RESOURCE_DESC readbackDesc = readback->GetDesc();
			//コピー用コマンドリスト生成
			ID3D12CommandAllocator* allocator = Helpers::CreateCommandAllocator(*m_device);
			ID3D12GraphicsCommandList* list = Helpers::CreateCommandList(*m_device, *allocator);
			ID3D12CommandQueue* queue = Helpers::CreateCommandQueue(*m_device);
			ID3D12Fence* fence = Helpers::CreateFence(*m_device);
			//TextureResourceの状態をCOPY_SOURCEに変更
			D3D12_RESOURCE_BARRIER barrier{};
			barrier = CreateTransitionBarrier(
				texture,
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				currentState,
				D3D12_RESOURCE_STATE_COPY_SOURCE
			);
			list->ResourceBarrier(1, &barrier);
			//コピー元設定
			D3D12_TEXTURE_COPY_LOCATION src{};
			src.pResource = &texture;
			//テクスチャのとき、SubresourceIndex指定
			src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			src.SubresourceIndex = 0;
			//コピー先設定
			D3D12_TEXTURE_COPY_LOCATION dst{};
			dst.pResource = readback;
			//バッファのとき、Footprint指定
			dst.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			dst.PlacedFootprint.Offset = 0;
			dst.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dst.PlacedFootprint.Footprint.Width = static_cast<uint32_t>(textureDesc.Width);	//画像の幅(px)
			dst.PlacedFootprint.Footprint.Height = textureDesc.Height;	//画像の幅(px)
			dst.PlacedFootprint.Footprint.Depth = 1;
			dst.PlacedFootprint.Footprint.RowPitch = static_cast<uint32_t>(readbackDesc.Width / textureDesc.Height);	//幅のバイトサイズ
			//コピー
			list->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
			//テクスチャの状態を元に戻す
			barrier = CreateTransitionBarrier(
				texture,
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				D3D12_RESOURCE_STATE_COPY_SOURCE,
				currentState
			);
			list->ResourceBarrier(1, &barrier);
			list->Close();
			ID3D12CommandList* lists[] = { list };
			queue->ExecuteCommandLists(1, lists);
			//GPUと同期
			uint64_t fenceVal = Helpers::Signal(*queue, *fence);
			Helpers::WaitForFenceValue(*fence, fenceVal);
			allocator->Reset();
			list->Reset(allocator, nullptr);
			//コピー用コマンドリストを解放
			Helpers::SafeRelease(fence, queue, list, allocator);
			//マップしてImageオブジェクト作成
			Pixel* map = nullptr;
			if (readback->Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) {
				Helpers::SafeRelease(readback);
				return false;
			}
			out = Image::CreateImage(
				static_cast<uint32_t>(textureDesc.Width),
				static_cast<uint32_t>(textureDesc.Height)
			);
			const uint32_t width =out.Width();
			const uint32_t height = out.Height();
			const size_t rowPitch = width * sizeof(Pixel);
			const size_t alignment = Helpers::GetAlignmentedSize(rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
			Pixel* pixels = out.Data();
			for (uint32_t y = 0; y < height; ++y) {
				Memory::Copy(pixels, map, rowPitch, true);
				pixels += width;
				map = reinterpret_cast<Pixel*>(reinterpret_cast<uint8_t*>(map) + alignment);
			}
			readback->Unmap(0, nullptr);
			Helpers::SafeRelease(readback);
			return true;
		}
		ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, bool shaderVisible = true) noexcept {
			ID3D12DescriptorHeap* descriptorHeap = nullptr;
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.Type = type;
			desc.NumDescriptors = numDescriptors;
			desc.NodeMask = 0;
			desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			if (m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)) != S_OK) return nullptr;
			else return descriptorHeap;
		}
		bool CreateView(ID3D12DescriptorHeap& heap, ID3D12Resource* resource, ViewFormat format, uint32_t index, uint32_t elementCountForUAV = 0, uint32_t structureByteStride = 0) noexcept {
			D3D12_CPU_DESCRIPTOR_HANDLE handle = heap.GetCPUDescriptorHandleForHeapStart();
			switch (format) {
			case ViewFormat::CBV: {
				handle.ptr += m_CBVSRVUAVIncrementSize * index;
				D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
				desc.BufferLocation = resource ? resource->GetGPUVirtualAddress() : D3D12_GPU_VIRTUAL_ADDRESS();
				desc.SizeInBytes = static_cast<uint32_t>(resource ? resource->GetDesc().Width : 0);
				m_device->CreateConstantBufferView(&desc, handle);
				break;
			}
			case ViewFormat::SRV: {
				handle.ptr += m_CBVSRVUAVIncrementSize * index;
				D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = 1;
				m_device->CreateShaderResourceView(resource, &desc, handle);
				break;
			}
			case ViewFormat::UAV: {
				handle.ptr += m_CBVSRVUAVIncrementSize * index;
				D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
				desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.Buffer.NumElements = elementCountForUAV;
				desc.Buffer.StructureByteStride = structureByteStride;
				m_device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
				break;
			}
			case ViewFormat::RTV: {
				handle.ptr += m_RTVIncrementSize * index;
				D3D12_RENDER_TARGET_VIEW_DESC desc{};
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				m_device->CreateRenderTargetView(resource, &desc, handle);
				break;
			}
			case ViewFormat::DSV: {
				handle.ptr += m_DSVIncrementSize * index;
				D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
				desc.Format = DXGI_FORMAT_D32_FLOAT;
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Flags = D3D12_DSV_FLAG_NONE;
				m_device->CreateDepthStencilView(resource, &desc, handle);
				break;
			}
			default:
				return false;
			}
			return true;
		}
		ID3D12PipelineState* CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) noexcept {
			ID3D12PipelineState* result = nullptr;
			if (m_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&result)) != S_OK) return nullptr;
			else return result;
		}
	public: /* ID3DDevice, IDXGIFactoryが必要な操作 */
		IDXGISwapChain4* CreateSwapChain(ID3D12CommandQueue& queue, HWND hWnd, uint32_t bufferCount) noexcept {
			return Helpers::CreateDXGISwapChain(*m_factory, queue, hWnd, bufferCount);
		}
	public:/* より具体的な操作 */
		IResource* CreateResource(const float* data, size_t count) noexcept {
			ID3D12Resource* resource = CreateUploadResource(sizeof(float), count, 1);
			if (!resource) return nullptr;
			if (!UpdateUploadResource(*resource, data, count)) {
				resource->Release();
				return nullptr;
			}
			ID3D12Resource* resources[] = { resource };
			return new D3DResource{ resources, 1, ResourceFormat::Float, D3D12_RESOURCE_STATE_GENERIC_READ };
		}
		IResource* CreateResource(const uint32_t* data, size_t count) noexcept {
			ID3D12Resource* resource = CreateUploadResource(sizeof(uint32_t), count, 1);
			if (!resource) return nullptr;
			if (!UpdateUploadResource(*resource, data, count)) {
				resource->Release();
				return nullptr;
			}
			ID3D12Resource* resources[] = { resource };
			return new D3DResource{ resources, 1, ResourceFormat::UInt, D3D12_RESOURCE_STATE_GENERIC_READ };
		}
		IResource* CreateResource(const Matrix* data, size_t count) noexcept {
			ID3D12Resource* resource = CreateUploadResource(sizeof(Matrix), count, 1);
			if (!resource) return nullptr;
			if (!UpdateUploadResource(*resource, data, count)) {
				resource->Release();
				return nullptr;
			}
			ID3D12Resource* resources[] = { resource };
			return new D3DResource{ resources, 1, ResourceFormat::Matrix, D3D12_RESOURCE_STATE_GENERIC_READ };
		}
		IResource* CreateResource(const Image& image) noexcept {
			if (!image.Enabled()) return nullptr;
			ID3D12Resource* resource = CreateTextureResource(false, image.Width(), image.Height());
			if (!resource) return nullptr;
			if (!UpdateTextureResource(*resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, image)) {
				resource->Release();
				return nullptr;
			}
			ID3D12Resource* resources[] = { resource };
			return new D3DResource{ resources, 1, ResourceFormat::Texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE };
		}
		IHeap* CreateHeap(uint32_t viewCount, uint32_t viewsSetCount, bool shaderVisible) noexcept {
			ID3D12DescriptorHeap* tmp = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewCount * viewsSetCount, shaderVisible);
			if (!tmp) return nullptr;
			else return new D3DHeap(tmp, viewCount, viewsSetCount);
		}
		IRenderTarget* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Color& defaultColor) noexcept {
			return new D3DRenderTarget(*m_device, width, height, bufferCount, targetCount, defaultColor);
		}
		IRenderTarget* CreateRenderTarget(IWindow& window, uint32_t bufferCount, const Color& defaultColor) {
			return new D3DSwapChain(*m_device, window, bufferCount, defaultColor);
		}
		ManagedObject<IShader> CreateShader(const ShaderDesc& desc) noexcept {
			ManagedObject<IShader> ret = ManagedObject<IShader>::GetObject(desc.name);
			if (ret) return ret;
			ID3DBlob* binary = LoadShader(desc);
			if (!binary) return ManagedObject<IShader>();
			ret = ManagedObject<IShader>(desc.name, new D3DShader(binary));
			m_shaders.Add(ret);
			return ret;
		}
		ManagedObject<IRenderer> CreateRenderer(const String& name, const RendererDesc& desc) noexcept {
			ManagedObject<IRenderer> ret = ManagedObject<IRenderer>::GetObject(name);
			if (ret) return ret;
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
			//RootSignature
			pipelineDesc.pRootSignature = m_rootSignatures.At(desc.mode);
			//シェーダ
			ManagedObject<IShader> vs = CreateShader(desc.vs);
			if (vs) {
				ID3DBlob* vsbinary = static_cast<D3DShader&>(*vs).GetBinary();
				pipelineDesc.VS.pShaderBytecode = vsbinary->GetBufferPointer();
				pipelineDesc.VS.BytecodeLength = vsbinary->GetBufferSize();
			}
			ManagedObject<IShader> ps = CreateShader(desc.ps);
			if (ps) {
				ID3DBlob* psbinary = static_cast<D3DShader&>(*ps).GetBinary();
				pipelineDesc.PS.pShaderBytecode = psbinary->GetBufferPointer();
				pipelineDesc.PS.BytecodeLength = psbinary->GetBufferSize();
			}
			//ラスタイザ
			pipelineDesc.RasterizerState.MultisampleEnable = false;
			pipelineDesc.RasterizerState.CullMode = GetCullingMode(desc.culling);
			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			pipelineDesc.RasterizerState.DepthClipEnable = true;
			//デプスステンシル設定
			pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			pipelineDesc.DepthStencilState.DepthEnable = desc.depthEnabled;
			pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			switch (desc.depthFunc) {
			case DepthFuncType::Less:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
				break;
			case DepthFuncType::LessEq:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
				break;
			case DepthFuncType::Greater:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
				break;
			case DepthFuncType::GreaterEq:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
				break;
			case DepthFuncType::Equal:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL;
				break;
			case DepthFuncType::NotEqual:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL;
				break;
			case DepthFuncType::Never:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
				break;
			case DepthFuncType::Always:
			default:
				pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
				break;
			}
			pipelineDesc.DepthStencilState.StencilEnable = desc.stencilEnabled;
			//ブレンド設定
			pipelineDesc.BlendState.AlphaToCoverageEnable = false;
			pipelineDesc.BlendState.IndependentBlendEnable = false;
			uint8_t rtCount = 0;	//レンダーターゲットの数
			for (RenderTargetDesc& rtDesc : desc.rt) {
				pipelineDesc.BlendState.RenderTarget[rtCount] = D3D12_RENDER_TARGET_BLEND_DESC();
				pipelineDesc.BlendState.RenderTarget[rtCount].BlendEnable = rtDesc.blendEnabled;
				pipelineDesc.BlendState.RenderTarget[rtCount].DestBlend = GetBlendFactor(rtDesc.dstFactor);
				pipelineDesc.BlendState.RenderTarget[rtCount].SrcBlend = GetBlendFactor(rtDesc.srcFactor);
				pipelineDesc.BlendState.RenderTarget[rtCount].BlendOp = GetBlendOp(rtDesc.colorOp);
				pipelineDesc.BlendState.RenderTarget[rtCount].DestBlendAlpha = GetBlendFactor(rtDesc.dstAlphaFactor);
				pipelineDesc.BlendState.RenderTarget[rtCount].SrcBlendAlpha = GetBlendFactor(rtDesc.srcAlphaFactor);
				pipelineDesc.BlendState.RenderTarget[rtCount].BlendOpAlpha = GetBlendOp(rtDesc.alphaOp);
				pipelineDesc.BlendState.RenderTarget[rtCount].LogicOpEnable = false;
				pipelineDesc.BlendState.RenderTarget[rtCount].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED | D3D12_COLOR_WRITE_ENABLE_GREEN | D3D12_COLOR_WRITE_ENABLE_BLUE;
				//フォーマットはR8G8B8A8で固定
				pipelineDesc.RTVFormats[rtCount] = DXGI_FORMAT_R8G8B8A8_UNORM;
				++rtCount;
				if (rtCount >= 8) break;	//最大8つまで使用(DirectXの制限)
			}
			pipelineDesc.NumRenderTargets = rtCount;
			//入力レイアウトは定数で固定
			pipelineDesc.InputLayout.pInputElementDescs = InputElementDesc;
			pipelineDesc.InputLayout.NumElements = InputElementDescCount;
			//インデックスバッファのTriangle Stripに不連続性はない
			pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			//プリミティブ型は三角形で固定
			pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			//サンプリング設定
			pipelineDesc.SampleDesc.Count = 1;
			pipelineDesc.SampleDesc.Quality = 0;
			pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
			ID3D12PipelineState* pipelineState = CreatePipelineState(pipelineDesc);
			if (!pipelineState) return ret;
			ret = ManagedObject<IRenderer>(name, new D3DRenderer(pipelineState));
			m_renderers.Add(ret);
			return ret;
		}
		IMeshResource* CreateMeshResource(const MeshDesc& desc) noexcept {
			using VertexType = Traits::remove_cvref_t<Traits::remove_pointer_t<decltype(desc.vertices.Items())>>;
			using IndexType = Traits::remove_cvref_t<Traits::remove_pointer_t<decltype(desc.indices.Items())>>;
			const uint32_t vertexCount = static_cast<uint32_t>(desc.vertices.Count());
			const uint32_t indexCount = static_cast<uint32_t>(desc.indices.Count());
			HRESULT hr = S_OK;
			ID3D12Resource* vertexBuffer = CreateUploadResource(sizeof(VertexType), vertexCount, 1, false);
			if (vertexBuffer) {
				VertexType* map = nullptr;
				hr = vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&map));
				if (Succeeded(hr)) {
					System::Memory::Copy(map, desc.vertices.Items(), sizeof(VertexType) * vertexCount, true);
					vertexBuffer->Unmap(0, nullptr);
				}
			}
			if (!vertexBuffer || Failed(hr)) {
				Helpers::SafeRelease(vertexBuffer);
				return nullptr;
			}
			ID3D12Resource* indexBuffer = CreateUploadResource(sizeof(IndexType), indexCount, 1, false);
			if (indexBuffer) {
				IndexType* map = nullptr;
				hr = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&map));
				if (Succeeded(hr)) {
					System::Memory::Copy(map, desc.indices.Items(), sizeof(IndexType) * indexCount, true);
					indexBuffer->Unmap(0, nullptr);
				}
			}
			if (!indexBuffer || Failed(hr)) {
				Helpers::SafeRelease(indexBuffer, vertexBuffer);
				return nullptr;
			}
			return new D3DMeshResource(
				vertexBuffer, indexBuffer, vertexCount, indexCount, desc.indexCountsPerMaterial
			);
		}
		IHeap* CreateHeap(HeapType type, uint32_t setCount = 1) noexcept;
		bool CopyView(ManagedObject<IHeap>& dst, uint32_t dstIndex, const ManagedObject<IHeap>& src, uint32_t srcIndex, uint32_t count = 1) noexcept {
			if (!dst || !dst->IsValidPlatform(PlatformID::DirectX)) return false;
			if (!src || !src->IsValidPlatform(PlatformID::DirectX)) return false;
			if (dst->GetAllViewCount() < dstIndex + count) return false;
			if (src->GetAllViewCount() < srcIndex + count) return false;
			ID3D12DescriptorHeap* dstHeap = static_cast<D3DHeap&>(*dst).GetHeap();
			ID3D12DescriptorHeap* srcHeap = static_cast<const D3DHeap&>(*src).GetHeap();
			if (!dstHeap || !srcHeap) return false;
			D3D12_CPU_DESCRIPTOR_HANDLE hDst = dstHeap->GetCPUDescriptorHandleForHeapStart();
			hDst.ptr += m_CBVSRVUAVIncrementSize * dstIndex;
			D3D12_CPU_DESCRIPTOR_HANDLE hSrc = srcHeap->GetCPUDescriptorHandleForHeapStart();
			hSrc.ptr += m_CBVSRVUAVIncrementSize * srcIndex;
			m_device->CopyDescriptorsSimple(
				count, hDst, hSrc, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
			);
			return true;
		}
		ICommandList* CreateCommandList(uint32_t materialCount) noexcept {
			return new D3DBundle(*m_device, materialCount);
		}
	};
}
