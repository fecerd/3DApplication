module;
#include "DirectXHeader.hpp"
module DirectX;
using namespace System;
using namespace System::Drawing;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace System::Application::Windows::DirectX::Internal;

//Impl
namespace System::Application::Windows::DirectX {
	bool D3DResource::UpdateResource(const float* data, size_t count) noexcept {
		if (m_format != ResourceFormat::Float) return false;
		if (m_state != D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager().UpdateUploadResource(*m_resources[m_current], data, count);
	}
	bool D3DResource::UpdateResource(const Matrix* data, size_t count) noexcept {
		if (m_format != ResourceFormat::Matrix) return false;
		if (m_state != D3D12_RESOURCE_STATE_GENERIC_READ) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager().UpdateUploadResource(*m_resources[m_current], data, count);
	}
	bool D3DResource::UpdateResource(const Image& image) noexcept {
		if (m_format != ResourceFormat::Texture) return false;
		if (m_state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager().UpdateTextureResource(*m_resources[m_current], m_state, image);
	}
	bool D3DResource::GetData(float* out, size_t count) const noexcept {
		if (m_format != ResourceFormat::Float) return false;
		if (m_state != D3D12_RESOURCE_STATE_GENERIC_READ) return false;
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
		if (m_state != D3D12_RESOURCE_STATE_GENERIC_READ) return false;
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
		if (m_state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) return false;
		if (!m_resources) return false;
		return DirectXManager::GetManager().GetTextureResourceImage(*m_resources[m_current], m_state, out);
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
		return DirectXManager::GetManager().CreateView(*m_heap, res, format, index);
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
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		HRESULT hr = device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_allocator));
		if (Succeeded(hr)) {
			hr = device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, m_allocator, nullptr, IID_PPV_ARGS(&m_bundle));
			if (Succeeded(hr)) {
				m_bundle->Close();
				m_descriptorHeap = ManagedObject<IHeap>(
					String::Empty(),
					DirectXManager::GetManager().CreateHeap(4 + 5 * materialCount, 1, true)
				);
				return;
			}
		}
		Helpers::SafeRelease(m_bundle, m_allocator);
	}
	bool D3DBundle::SetHeap_Internal(const ManagedObject<IHeap>& heap, HeapType type) noexcept {
		LockGuard lock{ m_mtx };
		if (!m_beginning.load(memory_order_acquire)) return false;
		if (!heap || !heap->IsValidPlatform(PlatformID::DirectX)) return false;
		uint32_t viewIndex = GetViewIndex(type);
		if (viewIndex == MAX_VALUE<uint32_t>) return false;
		DirectXManager::GetManager().CopyView(m_descriptorHeap, viewIndex, heap, 0, heap->GetAllViewCount());
		m_usedParamID[GetParamID(type)] = true;
		return true;
	}
	bool D3DBundle::BeginCommand() noexcept {
		LockGuard lock{ m_mtx };
		if (m_beginning.load(memory_order_acquire)) return false;
		m_bundle->Reset(m_allocator, nullptr);
		m_bundle->SetGraphicsRootSignature(DirectXManager::GetManager().GetRootSignature());
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
		m_bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		if (!bufferCount || !targetCount) return;
		DirectXManager& manager = DirectXManager::GetManager();
		//ヒープ作成
		ID3D12DescriptorHeap* rtvHeap = manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount * targetCount, false);
		ID3D12DescriptorHeap* dsvHeap = manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bufferCount, false);
		if (!rtvHeap || !dsvHeap) {
			Helpers::SafeRelease(rtvHeap, dsvHeap);
			return;
		}
		m_rtvHeap = ManagedObject<IHeap>(new D3DHeap{ rtvHeap, bufferCount * targetCount });
		m_dsvHeap = ManagedObject<IHeap>(new D3DHeap{ dsvHeap, bufferCount });
		//DepthStencil用リソースをDSVにセット
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager.CreateDepthResource(width, height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				return;
			}
			else dsResources.Add(ds);
			manager.CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		//RenderTarget用リソースをRTVにセット
		m_rtvResources.Reserve(targetCount);
		for (uint32_t i = 0; i < targetCount; ++i) {
			Vector<ID3D12Resource*> rtResources;
			rtResources.Reserve(bufferCount);
			for (uint32_t j = 0; j < bufferCount; ++j) {
				ID3D12Resource* rt = manager.CreateTextureResource(true, width, height);
				if (!rt) {
					for (ID3D12Resource*& res : rtResources) res->Release();
					rtResources.Clear();
					break;
				}
				rtResources.Add(rt);
				manager.CreateView(*rtvHeap, rt, ViewFormat::RTV, i + targetCount * j);
			}
			if (rtResources.Count() == bufferCount) {
				m_rtvResources.Add(ManagedObject<IResource>(
					new D3DResource{
						rtResources.Items(),
						bufferCount,
						ResourceFormat::Texture,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
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
			SetTransitionBarrier(*rt, rtvResources.GetState(), D3D12_RESOURCE_STATE_RENDER_TARGET);
			rtvResources.SetState(D3D12_RESOURCE_STATE_RENDER_TARGET);
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
				static_cast<D3D12_CLEAR_FLAGS>((depthClear ? D3D12_CLEAR_FLAG_DEPTH : 0) | (stencilClear ? D3D12_CLEAR_FLAG_STENCIL : 0)),
				1.f, 0, 0, nullptr
			);
		}
		if (colorClear) m_list->ClearRenderTargetView(hRTV, m_defaultColor, 0, nullptr);
		m_list->SetGraphicsRootSignature(DirectXManager::GetManager().GetRootSignature());
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
		DirectXManager& manager = DirectXManager::GetManager();
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager.CreateDepthResource(width, height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				return false;
			}
			dsResources.Add(ds);
			manager.CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		for (uint32_t i = 0; i < targetCount; ++i) {
			Vector<ID3D12Resource*> rtResources;
			rtResources.Reserve(bufferCount);
			for (uint32_t j = 0; j < bufferCount; ++j) {
				ID3D12Resource* rt = manager.CreateTextureResource(true, width, height);
				if (!rt) {
					for (ID3D12Resource* res : rtResources) res->Release();
					return false;
				}
				rtResources.Add(rt);
				manager.CreateView(*rtvHeap, rt, ViewFormat::RTV, i + targetCount * j);
			}
			m_rtvResources.Add(ManagedObject<IResource>(
				new D3DResource{
					rtResources.Items(),
					bufferCount,
					ResourceFormat::Texture,
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
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
		m_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
			CBVSRVUAVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			RTVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		if (!bufferCount) return;
		DirectXManager& manager = DirectXManager::GetManager();
		//SwapChain作成
		WindowPlatformData data = window.GetPlatformData();
		HWND hWnd = nullptr;
		if (data.Type == WindowType::Windows) hWnd = static_cast<HWND>(data.Data.GetData());
		if (!hWnd) return;
		m_queue = Helpers::CreateCommandQueue(device);
		if (!m_queue) return;
		m_queue->SetName(L"SwapChainQueue");
		m_swapChain = manager.CreateSwapChain(*m_queue, hWnd, bufferCount);
		if (!m_swapChain) {
			Helpers::SafeRelease(m_queue);
			return;
		}
		DXGI_SWAP_CHAIN_DESC1 desc{};
		if (m_swapChain->GetDesc1(&desc) < 0) {
			Helpers::SafeRelease(m_swapChain, m_queue);
			return;
		}
		//ヒープ作成
		ID3D12DescriptorHeap* rtvHeap = manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, false);
		ID3D12DescriptorHeap* dsvHeap = manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bufferCount, false);
		if (!rtvHeap || !dsvHeap) {
			Helpers::SafeRelease(m_swapChain, m_queue);
			Helpers::SafeRelease(rtvHeap, dsvHeap);
			return;
		}
		m_rtvHeap = ManagedObject<IHeap>(new D3DHeap{ rtvHeap, bufferCount });
		m_dsvHeap = ManagedObject<IHeap>(new D3DHeap{ dsvHeap, bufferCount });
		//DepthStencil用リソースをDSVにセット
		Vector<ID3D12Resource*> dsResources;
		dsResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* ds = manager.CreateDepthResource(desc.Width, desc.Height);
			if (!ds) {
				for (ID3D12Resource* res : dsResources) res->Release();
				Helpers::SafeRelease(m_swapChain, m_queue);
				return;
			}
			else dsResources.Add(ds);
			manager.CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
		}
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		//RenderTarget用リソースをRTVにセット
		Vector<ID3D12Resource*> rtResources;
		rtResources.Reserve(bufferCount);
		for (uint32_t i = 0; i < bufferCount; ++i) {
			ID3D12Resource* rt = nullptr;
			if (m_swapChain->GetBuffer(i, IID_PPV_ARGS(&rt)) != S_OK) {
				Helpers::SafeRelease(rt);
				for (ID3D12Resource*& res : rtResources) res->Release();
				m_dsvResources = ManagedObject<IResource>();
				Helpers::SafeRelease(m_swapChain, m_queue);
				return;
			}
			rtResources.Add(rt);
			manager.CreateView(*rtvHeap, rt, ViewFormat::RTV, i);
		}
		m_rtvResources.Add(ManagedObject<IResource>(
			new D3DResource{
				rtResources.Items(),
				bufferCount,
				ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_PRESENT
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
		EndCommand_Internal(true, D3D12_RESOURCE_STATE_PRESENT);
		LockGuard lock{ m_mtx };
		if (!m_rtvHeap || !m_dsvHeap) return false;
		DirectXManager& manager = DirectXManager::GetManager();
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
			desc.Format = DXGI_FORMAT_UNKNOWN;
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
			desc.Format = DXGI_FORMAT_UNKNOWN;
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
				manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.BufferCount, false),
				desc.BufferCount, 1
			)
			);
		m_dsvHeap = ManagedObject<IHeap>(
			new D3DHeap(
				manager.CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, desc.BufferCount, false),
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
			ID3D12Resource* ds = manager.CreateDepthResource(desc.Width, desc.Height);
			if (Failed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&rt))) || !ds) {
				Helpers::SafeRelease(rt, ds);
				for (ID3D12Resource* res : rtResources) res->Release();
				for (ID3D12Resource* res : dsResources) res->Release();
				return false;
			}
			manager.CreateView(*rtvHeap, rt, ViewFormat::RTV, i);
			manager.CreateView(*dsvHeap, ds, ViewFormat::DSV, i);
			rtResources.Add(rt);
			dsResources.Add(ds);
		}
		m_rtvResources.Add(ManagedObject<IResource>(
			new D3DResource{
				rtResources.Items(), desc.BufferCount, ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_PRESENT
			}
		));
		m_dsvResources = ManagedObject<IResource>(
			new D3DResource{
				dsResources.Items(), desc.BufferCount, ResourceFormat::Texture,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			}
		);
		uint32_t current = m_swapChain->GetCurrentBackBufferIndex();
		current = current ? current - 1 : desc.BufferCount - 1;
		static_cast<D3DResource&>(*(m_rtvResources[0])).SetCurrentIndex(current);
		static_cast<D3DResource&>(*m_dsvResources).SetCurrentIndex(current);
		return true;
	}

	DirectXManager::DirectXManager() {
		atexit(DebugReport);
		Helpers::EnableDebugLayer();
		m_factory = Helpers::CreateDXGIFactory();
		m_adapter = Helpers::CreateDXGIAdapter(*m_factory);
		m_device = Helpers::CreateDevice(*m_adapter);
		m_device->QueryInterface(&debugDevice);
		m_CBVSRVUAVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_RTVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DSVIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
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
		rendererDesc.rt.Add(RenderTargetDesc{});
		if (!CreateRenderer(Common3D::DefaultRendererName, rendererDesc)) {
			throw LogicException(__FUNCSIG__, u"デフォルトのPipelineStateの作成に失敗しました。", __FILE__, __LINE__);
		}
		rendererDesc.culling = CullingMode::None;
		if (!CreateRenderer(Common3D::DefaultMMDRendererName, rendererDesc)) {
			throw LogicException(__FUNCSIG__, u"デフォルトのPipelineStateの作成に失敗しました。", __FILE__, __LINE__);
		}
		rendererDesc.mode = RenderingMode::Z_Prepass;
		rendererDesc.vs.name = Common3D::DefaultZPrepassVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultZPrepassVS);
		rendererDesc.ps.name = String::Empty();
		rendererDesc.ps.filepath = String::Empty();
		if (!CreateRenderer(Common3D::DefaultZPrepassRendererName, rendererDesc)) {
			throw LogicException(__FUNCSIG__, u"Z-Prepass用のデフォルトのPipelineStateの作成に失敗しました。", __FILE__, __LINE__);
		}
		rendererDesc.mode = RenderingMode::ClusteredShading;
		rendererDesc.vs.name = Common3D::DefaultClusteredShadingVSName;
		rendererDesc.vs.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultClusteredShadingVS);
		rendererDesc.ps.name = Common3D::DefaultClusteredShadingPSName;
		rendererDesc.ps.filepath = ResourcePaths::GetPathName(ResourcePaths::DirectX::DefaultClusteredShadingPS);
		if (!CreateRenderer(Common3D::DefaultClusteredShadingRendererName, rendererDesc)) {
			throw LogicException(__FUNCSIG__, u"ClusteredShading用のデフォルトのPipelineStateの作成に失敗しました。", __FILE__, __LINE__);
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
			throw LogicException(__FUNCSIG__, u"Video用のデフォルトのPipelineStateの作成に失敗しました。", __FILE__, __LINE__);
		}

		Image image = Image::CreateSingleColorImage(1, 1, Colors::White);
		m_whiteTexture = ManagedObject<IResource>(Common3D::DefaultWhiteTextureName, CreateResource(image));
		image.SetPixel(0, 0, Colors::Black);
		m_blackTexture = ManagedObject<IResource>(Common3D::DefaultBlackTextureName, CreateResource(image));
		image = Image::CreateToonImage();
		m_toonTexture = ManagedObject<IResource>(Common3D::DefaultToonTextureName, CreateResource(image));
	}
	DirectXManager::~DirectXManager() noexcept {
		m_whiteTexture.Release();
		m_blackTexture.Release();
		m_toonTexture.Release();
		for (ID3D12RootSignature*& rs : m_rootSignatures.Values()) Helpers::SafeRelease(rs);
		Helpers::SafeRelease(m_device, m_adapter, m_factory);
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

	bool DirectXManager::CopyTextureResource(ID3D12Resource& texture, ID3D12Resource& upload, D3D12_RESOURCE_STATES currentState) noexcept {
		D3D12_RESOURCE_DESC uploadDesc = upload.GetDesc();
		D3D12_RESOURCE_DESC textureDesc = texture.GetDesc();
		//コピー用コマンドリスト生成
		constexpr uint32_t allocatorCount = 2;
		struct TEMP {
			ID3D12CommandAllocator* allocators[allocatorCount] = {};
			uint32_t currentIndex = 0;
			ID3D12GraphicsCommandList* list = nullptr;
			ID3D12CommandQueue* queue = nullptr;
			ID3D12Fence* fence = nullptr;
			uint64_t fenceVal = 0;
		public:
			~TEMP() noexcept {
				if (fence) Helpers::WaitForFenceValue(*fence, fenceVal);
				if (++currentIndex >= allocatorCount) currentIndex = 0;
				if (allocators[currentIndex]) allocators[currentIndex]->Reset();
				if (list && allocators[currentIndex]) list->Reset(allocators[currentIndex], nullptr);
				Helpers::SafeRelease(fence);
				Helpers::SafeRelease(queue, list);
				for (uint32_t i = 0; i < allocatorCount; ++i) Helpers::SafeRelease(allocators[i]);
			}
		};
		static TEMP temp;
		if (!temp.queue) temp.queue = Helpers::CreateCommandQueue(*m_device);
		if (!temp.fence) temp.fence = Helpers::CreateFence(*m_device);
		else Helpers::WaitForFenceValue(*temp.fence, temp.fenceVal);
		if (!temp.allocators[temp.currentIndex]) temp.allocators[temp.currentIndex] = Helpers::CreateCommandAllocator(*m_device);
		else temp.allocators[temp.currentIndex]->Reset();
		if (!temp.list) temp.list = Helpers::CreateCommandList(*m_device, *temp.allocators[temp.currentIndex]);
		else temp.list->Reset(temp.allocators[temp.currentIndex], nullptr);
		if (!temp.allocators[temp.currentIndex] || !temp.list || !temp.queue) return false;
		if (!temp.fence) return false;

		//テクスチャリソースの状態をCopyDestに変更
		D3D12_RESOURCE_BARRIER barrier{};
		if (currentState != D3D12_RESOURCE_STATE_COPY_DEST) {
			barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, currentState, D3D12_RESOURCE_STATE_COPY_DEST);
			temp.list->ResourceBarrier(1, &barrier);
		}
		//テクスチャ用バッファへコピー
		D3D12_TEXTURE_COPY_LOCATION src{};
		src.pResource = &upload;
		src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.PlacedFootprint.Offset = 0;
		src.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		src.PlacedFootprint.Footprint.Width = static_cast<uint32_t>(textureDesc.Width);	//画像の幅(px)
		src.PlacedFootprint.Footprint.Height = textureDesc.Height;	//画像の幅(px)
		src.PlacedFootprint.Footprint.Depth = 1;
		src.PlacedFootprint.Footprint.RowPitch = static_cast<uint32_t>(uploadDesc.Width / textureDesc.Height);	//幅のバイトサイズ
		D3D12_TEXTURE_COPY_LOCATION dst{};
		dst.pResource = &texture;
		dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.SubresourceIndex = 0;
		temp.list->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
		//テクスチャリソースの状態をピクセルシェーダリソースに変更
		barrier = CreateTransitionBarrier(texture, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		temp.list->ResourceBarrier(1, &barrier);
		temp.list->Close();
		ID3D12CommandList* lists[] = { temp.list };
		temp.queue->ExecuteCommandLists(1, lists);
		//GPUと同期
		temp.fenceVal = Helpers::Signal(*temp.queue, *temp.fence);
		if (++temp.currentIndex >= allocatorCount) temp.currentIndex = 0;
		return true;
	}
	bool DirectXManager::UpdateTextureResource(ID3D12Resource& resource, D3D12_RESOURCE_STATES currentState, const Image& image) noexcept {
		struct TEMP {
			ID3D12Resource* m_resource;
		public:
			~TEMP() noexcept { Helpers::SafeRelease(m_resource); }
		};
		static TEMP temp;
		if (!temp.m_resource) {
			temp.m_resource = CreateUploadResource(sizeof(Pixel), 1920, 1080);
		}
		struct TEMP2 {
			ID3D12Resource* resource = nullptr;
			uint32_t count = 0;
		};
		struct TEMP1 {
			List<TEMP2> uploads;
		public:
			~TEMP1() noexcept {
				System::Timer::Sleep(milliseconds(100));
				for (TEMP2& u : uploads) Helpers::SafeRelease(u.resource);
			}
		};
		static TEMP1 temp1;
		for (auto ite = temp1.uploads.begin(), e = temp1.uploads.end(); ite != e; ++ite) {
			TEMP2& t = *ite;
			--t.count;
			if (!t.count) {
				Helpers::SafeRelease(t.resource);
				temp1.uploads.Remove(ite);
			}
		}
		if (image.Width() == 1920 && image.Height() == 1080) {
			if (!UpdateUploadResource(*temp.m_resource, image)) return false;
			bool ret = CopyTextureResource(resource, *temp.m_resource, currentState);
			return ret;
		}
		else {
			ID3D12Resource* upload = CreateUploadResource(sizeof(Pixel), image.Width(), image.Height());
			if (!upload) return false;
			if (!UpdateUploadResource(*upload, image)) {
				upload->Release();
				return false;
			}
			bool ret = CopyTextureResource(resource, *upload, currentState);
			temp1.uploads.PushBack(TEMP2{ upload, 2 });
			return ret;
		}
	}

}
