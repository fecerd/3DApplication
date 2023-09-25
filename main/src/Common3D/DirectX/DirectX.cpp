module;
#define DEFINE_CRTDBG_new
#include "../../CRTDBG/crtdbg_wrapper.hpp"
#include <guiddef.h>
module DirectX;
import System;
import Application;	//IWindow
import Common3DInterface;
import DirectXHelper;
import DirectXResource;
import DXHeader;
using namespace System::Drawing;
using namespace System::Application::Common3D;
using namespace System::Application::Windows::DirectX::Internal;
using namespace DX;

#undef IID_PPV_ARGS
#define IID_PPV_ARGS(ppType) DX::GetIID(ppType), DX::IID_PPV_ARGS_Helper(ppType)

namespace System::Application::Windows::DirectX::Internal {
	ID3D12RootSignature* CreateRootSignature(ID3D12Device& device) noexcept {
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
	ranges[5].NumDescriptors = 1;
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
}

//Impl
namespace System::Application::Windows::DirectX {
	bool D3DResource::UpdateResource(const float* data, size_t count) noexcept {
		if (m_format != ResourceFormat::Float) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager()->UpdateUploadResource(*m_resources[m_current], data, count);
	}
	bool D3DResource::UpdateResource(const Matrix* data, size_t count) noexcept {
		if (m_format != ResourceFormat::Matrix) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager()->UpdateUploadResource(*m_resources[m_current], data, count);
	}
	bool D3DResource::UpdateResource(const Image& image) noexcept {
		if (m_format != ResourceFormat::Texture) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager()->UpdateTextureResource(*m_resources[m_current], m_state, image);
	}
	bool D3DResource::GetData(float* out, size_t count) const noexcept {
		if (m_format != ResourceFormat::Float) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		ID3D12Resource*& resource = m_resources[m_current];
		float* map = nullptr;
		if (resource->Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
		D3D12_RESOURCE_DESC desc = resource->GetDesc();
		for (UINT64 x = 0; x < desc.Width && x < count; ++x) out[x] = map[x];
		resource->Unmap(0, nullptr);
		for (UINT64 x = desc.Width; x < count; ++x) out[x] = 0.f;
		return true;
	}
	bool D3DResource::GetData(Matrix* out, size_t count) const noexcept {
		if (m_format != ResourceFormat::Matrix) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		ID3D12Resource*& resource = m_resources[m_current];
		Matrix* map = nullptr;
		if (resource->Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
		D3D12_RESOURCE_DESC desc = resource->GetDesc();
		for (UINT64 x = 0; x < desc.Width && x < count; ++x) out[x] = map[x];
		resource->Unmap(0, nullptr);
		for (UINT64 x = desc.Width; x < count; ++x) out[x] = Matrix();
		return true;
	}
	bool D3DResource::GetData(Image& out) const noexcept {
		if (m_format != ResourceFormat::Texture) return false;
		if (m_state != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager()->GetTextureResourceImage(*m_resources[m_current], m_state, out);
	}

	bool D3DHeap::SetView(uint32_t index, ViewFormat format, ManagedObject<IResource> resource) noexcept {
		switch (format) {
		case ViewFormat::CBV:
		case ViewFormat::SRV:
		case ViewFormat::UAV:
			break;
		default:
			return false;
		}
		if (GetAllViewCount() <= index) return false;
		if (resource && !resource->IsValidPlatform(PlatformID::DirectX)) return false;
		ID3D12Resource* res = resource ? static_cast<D3DResource&>(*resource).GetCurrent() : nullptr;
		return DirectXManager::GetManager()->CreateView(*m_heap, res, format, index);
	}

	uint32_t GetViewIndex(HeapType type) noexcept {
		switch (type) {
		case HeapType::Camera: return 0;
		case HeapType::Object: return 1;
		case HeapType::Animation: return 2;
		case HeapType::Scene: return 3;
		case HeapType::Material: return 4;
		default: return MAX_VALUE<uint32_t>;
		}
	}

	D3DBundle::D3DBundle(ID3D12Device& device, uint32_t materialCount) noexcept {
		if (!CBVSRVUAVIncrementSize && !RTVIncrementSize && !DSVIncrementSize) {
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		HRESULT hr = device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_allocator));
		if (Succeeded(hr)) {
			hr = device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE, m_allocator, nullptr, IID_PPV_ARGS(&m_bundle));
			if (Succeeded(hr)) {
				m_bundle->Close();
				m_descriptorHeap = ManagedObject<IHeap>(
					String::Empty(),
					DirectXManager::GetManager()->CreateHeap(4 + 5 * materialCount, 1, true)
				);
				return;
			}
		}
		SafeRelease(m_bundle, m_allocator);
	}
	bool D3DBundle::SetHeap_Internal(const ManagedObject<IHeap>& heap, HeapType type) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!heap || !heap->IsValidPlatform(PlatformID::DirectX)) return false;
		uint32_t viewIndex = GetViewIndex(type);
		if (viewIndex == MAX_VALUE<uint32_t>) return false;
		DirectXManager::GetManager()->CopyView(m_descriptorHeap, viewIndex, heap, 0, heap->GetAllViewCount());
		m_usedParamID[GetParamID(type)] = true;
		return true;
	}
	bool D3DBundle::BeginCommand() noexcept {
		LockGuard lock{ m_mtx };
		if (m_beginning.load(memory_order_acquire)) return false;
		m_bundle->Reset(m_allocator, nullptr);
		m_bundle->SetGraphicsRootSignature(DirectXManager::GetManager()->GetRootSignature());
		for (uint32_t i = 0; i < ParamCount; ++i) m_usedParamID[i] = false;
		m_beginning.store(true, memory_order_release);
		return true;
	}
	bool D3DBundle::SetRenderer(ManagedObject<IRenderer> renderer) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!renderer || !renderer->IsValidPlatform(PlatformID::DirectX)) return false;
		ID3D12PipelineState* pipelineState = static_cast<D3DRenderer&>(*renderer).GetPipelineState();
		if (!pipelineState) return false;
		m_bundle->SetPipelineState(pipelineState);
		return true;
	}
	bool D3DBundle::SetMesh(ManagedObject<IMeshResource> mesh) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!mesh || !mesh->IsValidPlatform(PlatformID::DirectX)) return false;
		D3DMeshResource& meshResource = static_cast<D3DMeshResource&>(*mesh);
		D3D12_VERTEX_BUFFER_VIEW vbv = meshResource.GetVBV();
		D3D12_INDEX_BUFFER_VIEW ibv = meshResource.GetIBV();
		m_bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_bundle->IASetVertexBuffers(0, 1, &vbv);
		m_bundle->IASetIndexBuffer(&ibv);
		return true;
	}
	bool D3DBundle::SetMaterialIndex(uint32_t index) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		ID3D12DescriptorHeap* heap = static_cast<D3DHeap&>(*m_descriptorHeap).GetHeap();
		if (!heap) return false;
		D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += CBVSRVUAVIncrementSize * (4 + index * 5);
		m_bundle->SetDescriptorHeaps(1, &heap);
		m_bundle->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Material), handle);
		return true;
	}
	bool D3DBundle::DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		m_bundle->DrawIndexedInstanced(useIndexCount, 1, startIndex, 0, 0);
		return true;
	}
	bool D3DBundle::EndCommand() noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		m_bundle->Close();
		m_beginning.store(false, memory_order_release);
		return true;
	}

	D3DRenderTarget::D3DRenderTarget(ID3D12Device& device, uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Color& defaultColor) noexcept {
		if (!CBVSRVUAVIncrementSize && !RTVIncrementSize && !DSVIncrementSize) {
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		if (!bufferCount || !targetCount) return;
		ExclusiveObject<DirectXManager> manager = DirectXManager::GetManager();
		//ヒープ作成
		ID3D12DescriptorHeap* rtvHeap = manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount * targetCount, false);
		ID3D12DescriptorHeap* dsvHeap = manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bufferCount, false);
		if (!rtvHeap || !dsvHeap) {
			SafeRelease(rtvHeap, dsvHeap);
			return;
		}
		m_rtvHeap = ManagedObject<IHeap>(new D3DHeap{ rtvHeap, bufferCount * targetCount });
		m_dsvHeap = ManagedObject<IHeap>(new D3DHeap{ dsvHeap, bufferCount });
		//DepthStencil用リソースをDSVにセット
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager->CreateDepthResource(width, height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				return;
			}
			else dsResources.Add(ds);
			manager->CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		//RenderTarget用リソースをRTVにセット
		m_rtvResources.Reserve(targetCount);
		for (uint32_t i = 0; i < targetCount; ++i) {
			Vector<ID3D12Resource*> rtResources;
			rtResources.Reserve(bufferCount);
			for (uint32_t j = 0; j < bufferCount; ++j) {
				ID3D12Resource* rt = manager->CreateTextureResource(true, width, height);
				if (!rt) {
					for (ID3D12Resource*& res : rtResources) res->Release();
					rtResources.Clear();
					break;
				}
				rtResources.Add(rt);
				manager->CreateView(*rtvHeap, rt, ViewFormat::RTV, i + targetCount * j);
			}
			if (rtResources.Count() == bufferCount) {
				m_rtvResources.Add(ManagedObject<IResource>(
					new D3DResource{
						rtResources.Items(),
						bufferCount,
						ResourceFormat::Texture,
						D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
					}
				));
			}
			else return;
		}
		//描画コマンド用メンバを作成
		m_allocator = Helpers::CreateCommandAllocator(device);
		m_list = Helpers::CreateCommandList(device, *m_allocator);
		m_queue = Helpers::CreateCommandQueue(device);
		m_fence = Helpers::CreateFence(device);
		//BeginCommand時に塗りつぶされる色を設定
		m_defaultColor[0] = defaultColor.R / 255.f;
		m_defaultColor[1] = defaultColor.G / 255.f;
		m_defaultColor[2] = defaultColor.B / 255.f;
		m_defaultColor[3] = defaultColor.Reserved / 255.f;
	}
	bool D3DRenderTarget::BeginCommand(bool colorClear, bool depthClear, bool stencilClear) noexcept {
		LockGuard lock{ m_mtx };
		if (m_beginning.load(memory_order_acquire)) return false;
		ID3D12DescriptorHeap* rtvHeap = static_cast<D3DHeap&>(*m_rtvHeap).GetHeap();
		ID3D12DescriptorHeap* dsvHeap = static_cast<D3DHeap&>(*m_dsvHeap).GetHeap();
		if (!rtvHeap || !dsvHeap) return false;
		const Size<uint32_t> size = GetSize();
		const uint32_t targetCount = GetTargetCount();
		if (!targetCount) return false;
		for (uint32_t i = 0; i < targetCount; ++i) {
			D3DResource& rtvResources = static_cast<D3DResource&>(*(m_rtvResources[i]));
			ID3D12Resource* rt = rtvResources.GetNext();
			if (!rt) return false;
			SetTransitionBarrier(*rt, rtvResources.GetState(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
			rtvResources.SetState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		}
		const uint32_t nextIndex = static_cast<D3DResource&>(*(m_rtvResources[0])).GetNextIndex();
		D3D12_CPU_DESCRIPTOR_HANDLE hRTV = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		hRTV.ptr += RTVIncrementSize * (nextIndex * targetCount);
		D3D12_CPU_DESCRIPTOR_HANDLE hDSV = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		hDSV.ptr += DSVIncrementSize * nextIndex;
		m_list->OMSetRenderTargets(targetCount, &hRTV, true, &hDSV);
		if (depthClear || stencilClear) {
			m_list->ClearDepthStencilView(
				hDSV,
				static_cast<D3D12_CLEAR_FLAGS>((depthClear ? D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH : 0) | (stencilClear ? D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_STENCIL : 0)),
				1.f, 0, 0, nullptr
			);
		}
		if (colorClear) m_list->ClearRenderTargetView(hRTV, m_defaultColor, 0, nullptr);
		m_list->SetGraphicsRootSignature(DirectXManager::GetManager()->GetRootSignature());
		m_beginning.store(true, memory_order_release);
		return true;
	}
	bool D3DRenderTarget::EndCommand_Internal(bool sync, D3D12_RESOURCE_STATES after) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		const uint32_t targetCount = GetTargetCount();
		for (uint32_t i = 0; i < targetCount; ++i) {
			D3DResource& rtvResources = static_cast<D3DResource&>(*(m_rtvResources[i]));
			ID3D12Resource* rt = rtvResources.GetNext();
			if (!rt) return false;
			SetTransitionBarrier(*rt, rtvResources.GetState(), after);
			rtvResources.SetState(after);
		}
		if (Failed(m_list->Close())) return false;
		ID3D12CommandList* lists[] = { m_list };
		m_queue->ExecuteCommandLists(1, lists);
		if (sync) {
			uint64_t fenceVal = Helpers::Signal(*m_queue, *m_fence);
			Helpers::WaitForFenceValue(*m_fence, fenceVal);
		}
		for (uint32_t i = 0; i < targetCount; ++i) m_rtvResources[i]->Next();
		m_dsvResources->Next();
		m_allocator->Reset();
		m_list->Reset(m_allocator, nullptr);
		m_beginning.store(false, memory_order_release);
		return true;
	}
	bool D3DRenderTarget::SetHeap(HeapType type, const ManagedObject<IHeap>& heap, uint32_t viewsSetIndex) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!heap || !heap->IsValidPlatform(PlatformID::DirectX)) return false;
		if (heap->GetViewsSetCount() <= viewsSetIndex) return false;
		ID3D12DescriptorHeap* descriptorHeap = static_cast<D3DHeap&>(*heap).GetHeap();
		D3D12_GPU_DESCRIPTOR_HANDLE handle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += CBVSRVUAVIncrementSize * (viewsSetIndex * heap->GetViewCount());
		m_list->SetDescriptorHeaps(1, &descriptorHeap);
		m_list->SetGraphicsRootDescriptorTable(GetParamID(type), handle);
		return true;
	}
	bool D3DRenderTarget::SetCommandList(ManagedObject<ICommandList> list) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!list || !list->IsValidPlatform(PlatformID::DirectX)) return false;
		D3DBundle& d3dbundle = static_cast<D3DBundle&>(*list);
		ID3D12GraphicsCommandList* bundle = d3dbundle.GetBundle();
		ID3D12DescriptorHeap* heap = d3dbundle.GetHeap();
		if (!bundle || !heap) return false;
		m_list->SetDescriptorHeaps(1, &heap);
		D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
		if (d3dbundle.UsedHeap(HeapType::Camera)) m_list->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Camera), handle);
		handle.ptr += CBVSRVUAVIncrementSize;
		if (d3dbundle.UsedHeap(HeapType::Object)) m_list->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Object), handle);
		handle.ptr += CBVSRVUAVIncrementSize;
		if (d3dbundle.UsedHeap(HeapType::Animation)) m_list->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Animation), handle);
		handle.ptr += CBVSRVUAVIncrementSize;
		if (d3dbundle.UsedHeap(HeapType::Scene)) m_list->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Scene), handle);
		handle.ptr += CBVSRVUAVIncrementSize;
		if (d3dbundle.UsedHeap(HeapType::Material)) m_list->SetGraphicsRootDescriptorTable(GetParamID(HeapType::Material), handle);
		m_list->ExecuteBundle(bundle);
		return true;
	}
	bool D3DRenderTarget::ResizeBuffer(uint32_t width, uint32_t height) noexcept {
		if (m_beginning.load(memory_order_acquire)) EndCommand(true);
		LockGuard lock{ m_mtx };
		const uint32_t targetCount = GetTargetCount();
		const uint32_t bufferCount = GetBufferCount();
		if (!bufferCount || !targetCount) return false;
		BeginCommand(false, false, false);
		EndCommand(true);
		if (!m_rtvHeap || !m_dsvHeap) return false;
		ID3D12DescriptorHeap* rtvHeap = static_cast<D3DHeap&>(*m_rtvHeap).GetHeap();
		ID3D12DescriptorHeap* dsvHeap = static_cast<D3DHeap&>(*m_dsvHeap).GetHeap();
		if (!rtvHeap || !dsvHeap) return false;
		ExclusiveObject<DirectXManager> manager = DirectXManager::GetManager();
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager->CreateDepthResource(width, height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				return false;
			}
			dsResources.Add(ds);
			manager->CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		for (uint32_t i = 0; i < targetCount; ++i) {
			Vector<ID3D12Resource*> rtResources;
			rtResources.Reserve(bufferCount);
			for (uint32_t j = 0; j < bufferCount; ++j) {
				ID3D12Resource* rt = manager->CreateTextureResource(true, width, height);
				if (!rt) {
					for (ID3D12Resource* res : rtResources) res->Release();
					return false;
				}
				rtResources.Add(rt);
				manager->CreateView(*rtvHeap, rt, ViewFormat::RTV, i + targetCount * j);
			}
			m_rtvResources.Add(ManagedObject<IResource>(
				new D3DResource{
					rtResources.Items(),
					bufferCount,
					ResourceFormat::Texture,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
				}
			));
		}
		return true;
	}
	bool D3DRenderTarget::SetMesh(ManagedObject<IMeshResource> mesh) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!mesh || !mesh->IsValidPlatform(PlatformID::DirectX)) return false;
		D3DMeshResource& meshResource = static_cast<D3DMeshResource&>(*mesh);
		D3D12_VERTEX_BUFFER_VIEW vbv = meshResource.GetVBV();
		D3D12_INDEX_BUFFER_VIEW ibv = meshResource.GetIBV();
		m_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_list->IASetVertexBuffers(0, 1, &vbv);
		m_list->IASetIndexBuffer(&ibv);
		return true;
	}
	bool D3DRenderTarget::SetRenderer(ManagedObject<IRenderer> renderer) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!renderer || !renderer->IsValidPlatform(PlatformID::DirectX)) return false;
		ID3D12PipelineState* pipelineState = static_cast<D3DRenderer&>(*renderer).GetPipelineState();
		if (!pipelineState) return false;
		m_list->SetPipelineState(pipelineState);
		return true;
	}
	bool D3DRenderTarget::DrawIndexed(uint32_t startIndex, uint32_t useIndexCount) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		m_list->DrawIndexedInstanced(useIndexCount, 1, startIndex, 0, 0);
		return true;
	}

	D3DSwapChain::D3DSwapChain(ID3D12Device& device, IWindow& window, uint32_t bufferCount, const Color& defaultColor) noexcept {
		if (!CBVSRVUAVIncrementSize && !RTVIncrementSize && !DSVIncrementSize) {
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		if (!bufferCount) return;
		ExclusiveObject<DirectXManager> manager = DirectXManager::GetManager();
		//SwapChain作成
		WindowPlatformData data = window.GetPlatformData();
		HWND hWnd = nullptr;
		if (data.HasWindowType(WindowType::Windows)) hWnd = static_cast<HWND>(data.GetNativePtr());
		if (!hWnd) return;
		m_queue = Helpers::CreateCommandQueue(device);
		if (!m_queue) return;
#if false
		m_queue->SetName(L"SwapChainQueue");
#endif
		m_swapChain = manager->CreateSwapChain(*m_queue, hWnd, bufferCount);
		if (!m_swapChain) {
			SafeRelease(m_queue);
			return;
		}
		DXGI_SWAP_CHAIN_DESC1 desc{};
		if (m_swapChain->GetDesc1(&desc) < 0) {
			SafeRelease(m_swapChain, m_queue);
			return;
		}
		//ヒープ作成
		ID3D12DescriptorHeap* rtvHeap = manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, false);
		ID3D12DescriptorHeap* dsvHeap = manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bufferCount, false);
		if (!rtvHeap || !dsvHeap) {
			SafeRelease(m_swapChain, m_queue);
			SafeRelease(rtvHeap, dsvHeap);
			return;
		}
		m_rtvHeap = ManagedObject<IHeap>(new D3DHeap{ rtvHeap, bufferCount });
		m_dsvHeap = ManagedObject<IHeap>(new D3DHeap{ dsvHeap, bufferCount });
		//DepthStencil用リソースをDSVにセット
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager->CreateDepthResource(desc.Width, desc.Height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				SafeRelease(m_swapChain, m_queue);
				return;
			}
			else dsResources.Add(ds);
			manager->CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		//RenderTarget用リソースをRTVにセット
		Vector<ID3D12Resource*> rtResources;
		rtResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* rt = nullptr;
			if (m_swapChain->GetBuffer(i, IID_PPV_ARGS(&rt)) != S_OK) {
				SafeRelease(rt);
				for (ID3D12Resource*& res : rtResources) res->Release();
				m_dsvResources = ManagedObject<IResource>();
				SafeRelease(m_swapChain, m_queue);
				return;
			}
			rtResources.Add(rt);
			manager->CreateView(*rtvHeap, rt, ViewFormat::RTV, i);
		}
		m_rtvResources.Add(ManagedObject<IResource>(
			new D3DResource{
				rtResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT
			}
		));
		//現在のバックバッファのインデックスをNextに設定
		uint32_t current = m_swapChain->GetCurrentBackBufferIndex();
		current = current ? current - 1 : bufferCount - 1;
		static_cast<D3DResource&>(*(m_rtvResources[0])).SetCurrentIndex(current);
		static_cast<D3DResource&>(*m_dsvResources).SetCurrentIndex(current);
		//描画コマンド用メンバを作成
		m_allocator = Helpers::CreateCommandAllocator(device);
		m_list = Helpers::CreateCommandList(device, *m_allocator);
		m_fence = Helpers::CreateFence(device);
		//BeginCommand時に塗りつぶされる色を設定
		m_defaultColor[0] = defaultColor.R / 255.f;
		m_defaultColor[1] = defaultColor.G / 255.f;
		m_defaultColor[2] = defaultColor.B / 255.f;
		m_defaultColor[3] = defaultColor.Reserved / 255.f;
	}
	bool D3DSwapChain::ResizeBuffer(uint32_t width, uint32_t height) noexcept {
		BeginCommand(false, false, false);
		EndCommand_Internal(true, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
		LockGuard lock{ m_mtx };
		if (!m_rtvHeap || !m_dsvHeap) return false;
		ExclusiveObject<DirectXManager> manager = DirectXManager::GetManager();
		m_rtvHeap.Release();
		m_dsvHeap.Release();
		m_rtvResources.Clear();
		m_dsvResources.Release();
		BOOL b;
		IDXGIOutput* out = nullptr;
		m_swapChain->GetFullscreenState(&b, &out);
		if (out) out->Release();
		/*if (b) {
			m_swapChain->SetFullscreenState(false, nullptr);
			m_swapChain->SetFullscreenState(true, nullptr);
			DXGI_MODE_DESC desc{};
			desc.Width = 1920;
			desc.Height = 1080;
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			desc.RefreshRate.Denominator = 0;
			desc.RefreshRate.Numerator = 0;
			desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_swapChain->ResizeTarget(&desc);
			width = 1920;
			height = 1080;
		}
		else {
			DXGI_MODE_DESC desc{};
			desc.Width = width;
			desc.Height = height;
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			desc.RefreshRate.Denominator = 0;
			desc.RefreshRate.Numerator = 0;
			desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_swapChain->ResizeTarget(&desc);
		}*/
		DXGI_SWAP_CHAIN_DESC1 desc{};
		if (Failed(m_swapChain->GetDesc1(&desc))) return false;
		if (Failed(m_swapChain->ResizeBuffers(desc.BufferCount, width, height, desc.Format, 0))) return false;
		if (Failed(m_swapChain->GetDesc1(&desc))) return false;
		m_rtvHeap = ManagedObject<IHeap>(
			new D3DHeap(
				manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.BufferCount, false),
				desc.BufferCount, 1
			)
			);
		m_dsvHeap = ManagedObject<IHeap>(
			new D3DHeap(
				manager->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, desc.BufferCount, false),
				desc.BufferCount, 1
			)
			);
		ID3D12DescriptorHeap* rtvHeap = static_cast<D3DHeap&>(*m_rtvHeap).GetHeap();
		ID3D12DescriptorHeap* dsvHeap = static_cast<D3DHeap&>(*m_dsvHeap).GetHeap();
		if (!rtvHeap || !dsvHeap) return false;
		Vector<ID3D12Resource*> rtResources;
		Vector<ID3D12Resource*> dsResources;
		rtResources.Reserve(desc.BufferCount);
		dsResources.Reserve(desc.BufferCount);
		for (uint32_t i = 0; i < desc.BufferCount; ++i) {
			ID3D12Resource* rt = nullptr;
			ID3D12Resource* ds = manager->CreateDepthResource(desc.Width, desc.Height);
			if (Failed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&rt))) || !ds) {
				SafeRelease(rt, ds);
				for (ID3D12Resource* res : rtResources) res->Release();
				for (ID3D12Resource* res : dsResources) res->Release();
				return false;
			}
			manager->CreateView(*rtvHeap, rt, ViewFormat::RTV, i);
			manager->CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
			rtResources.Add(rt);
			dsResources.Add(ds);
		}
		m_rtvResources.Add(ManagedObject<IResource>(
			new D3DResource{
				rtResources.Items(), desc.BufferCount, ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT
			}
		));
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(), desc.BufferCount, ResourceFormat::Texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		uint32_t current = m_swapChain->GetCurrentBackBufferIndex();
		current = current ? current - 1 : desc.BufferCount - 1;
		static_cast<D3DResource&>(*(m_rtvResources[0])).SetCurrentIndex(current);
		static_cast<D3DResource&>(*m_dsvResources).SetCurrentIndex(current);
		return true;
	}
	bool D3DSwapChain::EndCommand(bool sync) noexcept {
		if (EndCommand_Internal(sync, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT)) {
			if (sync) return m_swapChain->Present(0, 0) == S_OK;
			else return true;
		}
		else return false;
	}


	DirectXManager::DirectXManager() {
		Helpers::EnableDebugLayer();
		m_factory = Helpers::CreateDXGIFactory();
		m_adapter = Helpers::CreateDXGIAdapter(*m_factory);
		m_device = Helpers::CreateDevice(*m_adapter);
		SafeRelease(debugDevice);
		m_device->QueryInterface(&debugDevice);

		m_copyCommandList = MakeUnique<CopyCommandList>(*m_device);

		m_CBVSRVUAVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_RTVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DSVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_rootSignatures.Insert(RenderingMode::Standard, CreateRootSignature(*m_device));
		m_rootSignatures.Insert(RenderingMode::Z_Prepass, CreateRootSignatureForZPrepass(*m_device));
		m_rootSignatures.Insert(RenderingMode::ClusteredShading, CreateRootSignatureForCluster(*m_device));
		RendererDesc rendererDesc{};
		rendererDesc.vs.name = Common3D::DefaultVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultVS);
		rendererDesc.vs.entry = Common3D::DefaultShaderEntry;
		rendererDesc.vs.target = Common3D::DefaultVSTarget;
		rendererDesc.ps.name = Common3D::DefaultPSName;
		rendererDesc.ps.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultPS);
		rendererDesc.ps.entry = Common3D::DefaultShaderEntry;
		rendererDesc.ps.target = Common3D::DefaultPSTarget;
		rendererDesc.culling = CullingMode::Back;
		rendererDesc.depthEnabled = true;
		RenderTargetDesc rtDesc{};
		rtDesc.blendEnabled = true;
		rtDesc.dstFactor = BlendFactors::SrcAlphaInv;
		rtDesc.srcFactor = BlendFactors::One;
		rtDesc.colorOp = BlendOp::Add;
		rtDesc.dstAlphaFactor = BlendFactors::SrcAlphaInv;
		rtDesc.srcAlphaFactor = BlendFactors::One;
		rtDesc.alphaOp = BlendOp::Add;
		rendererDesc.rt.Add(rtDesc);
		if (!CreateRenderer(Common3D::DefaultRendererName, rendererDesc)) {
			throw LogicException(u"デフォルトのPipelineStateの作成に失敗しました。");
		}
		rendererDesc.culling = CullingMode::None;
		if (!CreateRenderer(Common3D::DefaultMMDRendererName, rendererDesc)) {
			throw LogicException(u"デフォルトのPipelineStateの作成に失敗しました。");
		}
		rendererDesc.mode = RenderingMode::Z_Prepass;
		rendererDesc.vs.name = Common3D::DefaultZPrepassVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultZPrepassVS);
		rendererDesc.ps.name = String::Empty();
		rendererDesc.ps.filepath = String::Empty();
		if (!CreateRenderer(Common3D::DefaultZPrepassRendererName, rendererDesc)) {
			throw LogicException(u"Z-Prepass用のデフォルトのPipelineStateの作成に失敗しました。");
		}
		rendererDesc.mode = RenderingMode::ClusteredShading;
		rendererDesc.vs.name = Common3D::DefaultClusteredShadingVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultClusteredShadingVS);
		rendererDesc.ps.name = Common3D::DefaultClusteredShadingPSName;
		rendererDesc.ps.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultClusteredShadingPS);
		if (!CreateRenderer(Common3D::DefaultClusteredShadingRendererName, rendererDesc)) {
			throw LogicException(u"ClusteredShading用のデフォルトのPipelineStateの作成に失敗しました。");
		}
		rendererDesc.mode = RenderingMode::Standard;
		rendererDesc.vs.name = Common3D::DefaultVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultVS);
		rendererDesc.vs.entry = Common3D::DefaultShaderEntry;
		rendererDesc.vs.target = Common3D::DefaultVSTarget;
		rendererDesc.ps.name = Common3D::DefaultVideoPSName;
		rendererDesc.ps.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultVideoPS);
		rendererDesc.ps.entry = Common3D::DefaultShaderEntry;
		rendererDesc.ps.target = Common3D::DefaultPSTarget;
		if (!CreateRenderer(Common3D::DefaultVideoRendererName, rendererDesc)) {
			throw LogicException(u"Video用のデフォルトのPipelineStateの作成に失敗しました。");
		}

		Image image = Image::CreateSingleColorImage(1, 1, Colors::White);
		m_whiteTexture = ManagedObject<IResource>(Common3D::DefaultWhiteTextureName, CreateResource(image));
		image.SetPixel(0, 0, Colors::Black);
		m_blackTexture = ManagedObject<IResource>(Common3D::DefaultBlackTextureName, CreateResource(image));
		image = Image::CreateToonImage();
		m_toonTexture = ManagedObject<IResource>(Common3D::DefaultToonTextureName, CreateResource(image));
	}
	DirectXManager::~DirectXManager() noexcept {
		//Queueもここで待機できる
		m_copyCommandList.Reset();

		m_whiteTexture.Release();
		m_blackTexture.Release();
		m_toonTexture.Release();
		for (ID3D12RootSignature*& rs : m_rootSignatures.Values()) SafeRelease(rs);

		SafeRelease(m_device, m_adapter, m_factory);
	}

	bool DirectXManager::CopyTextureResource(ID3D12Resource& texture, ID3D12Resource& upload, D3D12_RESOURCE_STATES currentState) noexcept {
		D3D12_RESOURCE_DESC textureDesc = texture.GetDesc();
		D3D12_RESOURCE_DESC uploadDesc = upload.GetDesc();

		m_copyCommandList->BeginCommand();

		//テクスチャリソースの状態をCopyDestに変更
		D3D12_RESOURCE_BARRIER barrier{};
		if (currentState != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST) {
			barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, currentState, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			m_copyCommandList->ResourceBarrier(1, &barrier);
		}
		//テクスチャ用バッファへコピー
		D3D12_TEXTURE_COPY_LOCATION src{};
		src.pResource = &upload;
		src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.PlacedFootprint.Offset = 0;
		src.PlacedFootprint.Footprint.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		src.PlacedFootprint.Footprint.Width = static_cast<uint32_t>(textureDesc.Width);	//画像の幅(px)
		src.PlacedFootprint.Footprint.Height = textureDesc.Height;	//画像の幅(px)
		src.PlacedFootprint.Footprint.Depth = 1;
		src.PlacedFootprint.Footprint.RowPitch = static_cast<uint32_t>(uploadDesc.Width / textureDesc.Height);	//幅のバイトサイズ
		D3D12_TEXTURE_COPY_LOCATION dst{};
		dst.pResource = &texture;
		dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.SubresourceIndex = 0;
		m_copyCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
		//テクスチャリソースの状態をピクセルシェーダリソースに変更
		barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		m_copyCommandList->ResourceBarrier(1, &barrier);
		m_copyCommandList->EndCommand();
		return true;
	}

	bool DirectXManager::UpdateTextureResource(ID3D12Resource& resource, D3D12_RESOURCE_STATES currentState, const Image& image) noexcept {
		ID3D12Resource* pUpload = m_copyCommandList->GetUploadResourceCache(sizeof(Pixel), image.Width(), image.Height());
		if (!pUpload) return false;
		if (!UpdateUploadResource(*pUpload, image)) return false;
#if defined(__GNUC__)
		bool ret = false;
		{
			ID3D12Resource& texture = resource;
			ID3D12Resource& upload = *pUpload;

			m_copyCommandList->BeginCommand();

			//テクスチャリソースの状態をCopyDestに変更
			D3D12_RESOURCE_BARRIER barrier{};
			if (currentState != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST) {
				barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, currentState, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
				m_copyCommandList->ResourceBarrier(1, &barrier);
			}
			//テクスチャ用バッファへコピー
			D3D12_TEXTURE_COPY_LOCATION src{};
			src.pResource = &upload;
			src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			src.PlacedFootprint.Offset = 0;
			src.PlacedFootprint.Footprint.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			src.PlacedFootprint.Footprint.Width = static_cast<uint32_t>(image.Width());	//画像の幅(px)
			src.PlacedFootprint.Footprint.Height = image.Height();	//画像の幅(px)
			src.PlacedFootprint.Footprint.Depth = 1;
			src.PlacedFootprint.Footprint.RowPitch = static_cast<uint32_t>(image.Width() * sizeof(Pixel) / image.Height());	//幅のバイトサイズ
			D3D12_TEXTURE_COPY_LOCATION dst{};
			dst.pResource = &texture;
			dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst.SubresourceIndex = 0;
			m_copyCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
			//テクスチャリソースの状態をピクセルシェーダリソースに変更
			barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			m_copyCommandList->ResourceBarrier(1, &barrier);
			m_copyCommandList->EndCommand();
			ret = true;
		}
#else
		bool ret = CopyTextureResource(resource, *pUpload, currentState);
#endif
		return ret;
	}

}

namespace System::Application::Windows::DirectX {
	RecursiveMutex& GetMutex() noexcept {
		static RecursiveMutex ret{};
		return ret;
	}

	void DirectXManager::DebugReport() noexcept {
		LockGuard lock{ GetMutex() };
		if (debugDevice) {
			debugDevice->ReportLiveDeviceObjects(
				static_cast<D3D12_RLDO_FLAGS>(D3D12_RLDO_FLAGS::D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS::D3D12_RLDO_IGNORE_INTERNAL)
			);
		}
		SafeRelease(debugDevice);
	}
	ExclusiveObject<DirectXManager> DirectXManager::GetManager() {
		{
			LockGuard lock{ GetMutex() };
			if (!pManager) {
				try {
					pManager = new DirectXManager();
					atexit([](){ DirectXManager::CloseManager(); });
				} catch (std_exception& ex) {
					pManager = nullptr;
					System::Application::Log(String(ex.what()));
				}
			}
		}
		return ExclusiveObject{ pManager, GetMutex() };
	}
	bool DirectXManager::CloseManager() {
		{
			LockGuard lock{ GetMutex() };
			delete pManager;
			pManager = nullptr;
		}
		DebugReport();
		return true;
	}

	D3D12_RESOURCE_BARRIER DirectXManager::CreateTransitionBarrier(
		ID3D12Resource& resource, uint32_t subResource,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after
	) {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = &resource;
		barrier.Transition.Subresource = subResource;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		return barrier;
	}

	bool DirectXManager::UpdateUploadResource(ID3D12Resource& resource, const Drawing::Image& image) noexcept {
		Drawing::Pixel* map = nullptr;
		if (resource.Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) return false;
		const uint32_t width = image.Width();
		const uint32_t height = image.Height();
		const size_t rowPitch = width * sizeof(Drawing::Pixel);
		const size_t alignment = Helpers::GetAlignmentedSize(rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
		const Drawing::Pixel* pixels = image.Data();
		for (uint32_t y = 0; y < height; ++y) {
			Memory::Copy(map, pixels, rowPitch, true);
			pixels += width;
			map = reinterpret_cast<Drawing::Pixel*>(reinterpret_cast<uint8_t*>(map) + alignment);
		}
		resource.Unmap(0, nullptr);
		return true;
	}
	ID3DBlob* DirectXManager::LoadShader(const ShaderDesc& desc) noexcept {
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

	ID3D12RootSignature* DirectXManager::GetRootSignature(RenderingMode mode) const noexcept {
		return m_rootSignatures.At(mode);
	}

	ID3D12Resource* DirectXManager::CreateUploadResource(size_t typeSize, uint64_t width, uint32_t height, bool alignment) noexcept {
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
		if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ret)) != S_OK) {
			return nullptr;
		}
		else return ret;
	}
	ID3D12Resource* DirectXManager::CreateReadbackResource(size_t typeSize, uint64_t width, uint64_t height, bool alignment) noexcept {
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
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
		if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&ret)) != S_OK) {
			return nullptr;
		}
		else return ret;
	}
	ID3D12Resource* DirectXManager::CreateTextureResource(bool useRenderTarget, uint32_t width, uint32_t height) noexcept {
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 0;
		prop.VisibleNodeMask = 0;
		D3D12_RESOURCE_DESC desc{};
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Width = width;	//画像の幅(px)
		desc.Height = height;	//画像の高さ(px)
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = useRenderTarget ? D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
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
			clear.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			hr = m_device->CreateCommittedResource(
				&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clear, IID_PPV_ARGS(&ret)
			);
		}
		else {
			hr = m_device->CreateCommittedResource(
				&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr, IID_PPV_ARGS(&ret)
			);
		}
		if (hr != S_OK) return nullptr;
		else return ret;
	}
	ID3D12Resource* DirectXManager::CreateDepthResource(uint32_t width, uint32_t height) noexcept {
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Width = width;
		desc.Height = height;
		desc.DepthOrArraySize = 1;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 0;
		prop.VisibleNodeMask = 0;
		D3D12_CLEAR_VALUE clear{};
		clear.DepthStencil.Depth = 1.0f;
		clear.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		ID3D12Resource* ret = nullptr;
		if (m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&ret)) != S_OK) {
			return nullptr;
		}
		else return ret;
	}

	bool DirectXManager::GetTextureResourceImage(ID3D12Resource& texture, D3D12_RESOURCE_STATES currentState, Image& out) noexcept {
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
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE
		);
		list->ResourceBarrier(1, &barrier);
		//コピー元設定
		D3D12_TEXTURE_COPY_LOCATION src{};
		src.pResource = &texture;
		//テクスチャのとき、SubresourceIndex指定
		src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		src.SubresourceIndex = 0;
		//コピー先設定
		D3D12_TEXTURE_COPY_LOCATION dst{};
		dst.pResource = readback;
		//バッファのとき、Footprint指定
		dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		dst.PlacedFootprint.Offset = 0;
		dst.PlacedFootprint.Footprint.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
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
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE,
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
		SafeRelease(fence, queue, list, allocator);
		//マップしてImageオブジェクト作成
		Pixel* map = nullptr;
		if (readback->Map(0, nullptr, reinterpret_cast<void**>(&map)) != S_OK) {
			SafeRelease(readback);
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
		SafeRelease(readback);
		return true;
	}
	ID3D12DescriptorHeap* DirectXManager::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, bool shaderVisible) noexcept {
		ID3D12DescriptorHeap* descriptorHeap = nullptr;
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Type = type;
		desc.NumDescriptors = numDescriptors;
		desc.NodeMask = 0;
		desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)) != S_OK) return nullptr;
		else return descriptorHeap;
	}
	bool DirectXManager::CreateView(ID3D12DescriptorHeap& heap, ID3D12Resource* resource, ViewFormat format, uint32_t index, uint32_t elementCountForUAV, uint32_t structureByteStride) noexcept {
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
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			m_device->CreateShaderResourceView(resource, &desc, handle);
			break;
		}
		case ViewFormat::UAV: {
			handle.ptr += m_CBVSRVUAVIncrementSize * index;
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			desc.Buffer.NumElements = elementCountForUAV;
			desc.Buffer.StructureByteStride = structureByteStride;
			m_device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
			break;
		}
		case ViewFormat::RTV: {
			handle.ptr += m_RTVIncrementSize * index;
			D3D12_RENDER_TARGET_VIEW_DESC desc{};
			desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			m_device->CreateRenderTargetView(resource, &desc, handle);
			break;
		}
		case ViewFormat::DSV: {
			handle.ptr += m_DSVIncrementSize * index;
			D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
			desc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
			m_device->CreateDepthStencilView(resource, &desc, handle);
			break;
		}
		default:
			return false;
		}
		return true;
	}
	ID3D12PipelineState* DirectXManager::CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) noexcept {
		ID3D12PipelineState* result = nullptr;
		if (m_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&result)) != S_OK) return nullptr;
		else return result;
	}

	IDXGISwapChain4* DirectXManager::CreateSwapChain(ID3D12CommandQueue& queue, HWND hWnd, uint32_t bufferCount) noexcept {
		return Helpers::CreateDXGISwapChain(*m_factory, queue, hWnd, bufferCount);
	}

	IResource* DirectXManager::CreateResource(const float* data, size_t count) noexcept {
		ID3D12Resource* resource = CreateUploadResource(sizeof(float), count, 1);
		if (!resource) return nullptr;
		if (!UpdateUploadResource(*resource, data, count)) {
			resource->Release();
			return nullptr;
		}
		ID3D12Resource* resources[] = { resource };
		return new D3DResource{ resources, 1, ResourceFormat::Float, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ };
	}
	IResource* DirectXManager::CreateResource(const uint32_t* data, size_t count) noexcept {
		ID3D12Resource* resource = CreateUploadResource(sizeof(uint32_t), count, 1);
		if (!resource) return nullptr;
		if (!UpdateUploadResource(*resource, data, count)) {
			resource->Release();
			return nullptr;
		}
		ID3D12Resource* resources[] = { resource };
		return new D3DResource{ resources, 1, ResourceFormat::UInt, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ };
	}
	IResource* DirectXManager::CreateResource(const Matrix* data, size_t count) noexcept {
		ID3D12Resource* resource = CreateUploadResource(sizeof(Matrix), count, 1);
		if (!resource) return nullptr;
		if (!UpdateUploadResource(*resource, data, count)) {
			resource->Release();
			return nullptr;
		}
		ID3D12Resource* resources[] = { resource };
		return new D3DResource{ resources, 1, ResourceFormat::Matrix, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ };
	}
	IResource* DirectXManager::CreateResource(const Image& image) noexcept {
		if (!image.Enabled()) return nullptr;
		ID3D12Resource* resource = CreateTextureResource(false, image.Width(), image.Height());
		if (!resource) return nullptr;
		if (!UpdateTextureResource(*resource, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, image)) {
			resource->Release();
			return nullptr;
		}
		ID3D12Resource* resources[] = { resource };
		return new D3DResource{ resources, 1, ResourceFormat::Texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE };
	}

	IHeap* DirectXManager::CreateHeap(uint32_t viewCount, uint32_t viewsSetCount, bool shaderVisible) noexcept {
		ID3D12DescriptorHeap* tmp = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, viewCount * viewsSetCount, shaderVisible);
		if (!tmp) return nullptr;
		else return new D3DHeap(tmp, viewCount, viewsSetCount);
	}
	IRenderTarget* DirectXManager::CreateRenderTarget(uint32_t width, uint32_t height, uint32_t bufferCount, uint32_t targetCount, const Drawing::Color& defaultColor) noexcept {
		return new D3DRenderTarget(*m_device, width, height, bufferCount, targetCount, defaultColor);
	}
	IRenderTarget* DirectXManager::CreateRenderTarget(IWindow& window, uint32_t bufferCount, const Drawing::Color& defaultColor) {
		return new D3DSwapChain(*m_device, window, bufferCount, defaultColor);
	}
	ManagedObject<IShader> DirectXManager::CreateShader(const ShaderDesc& desc) noexcept {
		ManagedObject<IShader> ret = ManagedObject<IShader>::GetObject(desc.name);
		if (ret) return ret;
		ID3DBlob* binary = LoadShader(desc);
		if (!binary) return ManagedObject<IShader>();
		ret = ManagedObject<IShader>(desc.name, new D3DShader(binary));
		m_shaders.Add(ret);
		return ret;
	}
	ManagedObject<IRenderer> DirectXManager::CreateRenderer(const String& name, const RendererDesc& desc) noexcept {
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
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		pipelineDesc.RasterizerState.DepthClipEnable = true;
		//デプスステンシル設定
		pipelineDesc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		pipelineDesc.DepthStencilState.DepthEnable = desc.depthEnabled;
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
		switch (desc.depthFunc) {
		case DepthFuncType::Less:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
			break;
		case DepthFuncType::LessEq:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS_EQUAL;
			break;
		case DepthFuncType::Greater:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_GREATER;
			break;
		case DepthFuncType::GreaterEq:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_GREATER_EQUAL;
			break;
		case DepthFuncType::Equal:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_EQUAL;
			break;
		case DepthFuncType::NotEqual:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NOT_EQUAL;
			break;
		case DepthFuncType::Never:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
			break;
		case DepthFuncType::Always:
		default:
			pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
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
			pipelineDesc.BlendState.RenderTarget[rtCount].RenderTargetWriteMask = 
				D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_RED
				| D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_GREEN
				| D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_BLUE	//;
				| D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALPHA;
			//フォーマットはR8G8B8A8で固定
			pipelineDesc.RTVFormats[rtCount] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			++rtCount;
			if (rtCount >= 8) break;	//最大8つまで使用(DirectXの制限)
		}
		pipelineDesc.NumRenderTargets = rtCount;
		//入力レイアウトは定数で固定
		pipelineDesc.InputLayout.pInputElementDescs = InputElementDesc;
		pipelineDesc.InputLayout.NumElements = InputElementDescCount;
		//インデックスバッファのTriangle Stripに不連続性はない
		pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		//プリミティブ型は三角形で固定
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
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
	IMeshResource* DirectXManager::CreateMeshResource(const MeshDesc& desc) noexcept {
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
				Memory::Copy(map, desc.vertices.Items(), sizeof(VertexType) * vertexCount, true);
				vertexBuffer->Unmap(0, nullptr);
			}
		}
		if (!vertexBuffer || Failed(hr)) {
			SafeRelease(vertexBuffer);
			return nullptr;
		}
		ID3D12Resource* indexBuffer = CreateUploadResource(sizeof(IndexType), indexCount, 1, false);
		if (indexBuffer) {
			IndexType* map = nullptr;
			hr = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&map));
			if (Succeeded(hr)) {
				Memory::Copy(map, desc.indices.Items(), sizeof(IndexType) * indexCount, true);
				indexBuffer->Unmap(0, nullptr);
			}
		}
		if (!indexBuffer || Failed(hr)) {
			SafeRelease(indexBuffer, vertexBuffer);
			return nullptr;
		}
		return new D3DMeshResource(
			vertexBuffer, indexBuffer, vertexCount, indexCount, desc.indexCountsPerMaterial
		);
	}
	IHeap* DirectXManager::CreateHeap(HeapType type, uint32_t setCount) noexcept {
		IHeap* ret = nullptr;
		if (type == HeapType::Material) {
			ret = CreateHeap(5, setCount, false);
			for (uint32_t i = 0; i < setCount; ++i) {
				ret->SetView(i * 5 + 0, ViewFormat::CBV, ManagedObject<IResource>{});
				ret->SetView(i * 5 + 1, ViewFormat::SRV, ManagedObject<IResource>{});
				ret->SetView(i * 5 + 2, ViewFormat::SRV, ManagedObject<IResource>{});
				ret->SetView(i * 5 + 3, ViewFormat::SRV, ManagedObject<IResource>{});
				ret->SetView(i * 5 + 4, ViewFormat::SRV, ManagedObject<IResource>{});
			}
		}
		else {
			if (type == HeapType::Camera || type == HeapType::Scene) ret = CreateHeap(1, setCount, true);
			else ret = CreateHeap(1, setCount, false);
			for (uint32_t i = 0; i < setCount; ++i) {
				ret->SetView(i * 1 + 0, ViewFormat::CBV, ManagedObject<IResource>{});
			}
		}
		return ret;
	}
	bool DirectXManager::CopyView(ManagedObject<IHeap>& dst, uint32_t dstIndex, const ManagedObject<IHeap>& src, uint32_t srcIndex, uint32_t count) noexcept {
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
			count, hDst, hSrc, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
		return true;
	}
	ICommandList* DirectXManager::CreateCommandList(uint32_t materialCount) noexcept {
		return new D3DBundle(*m_device, materialCount);
	}

}
