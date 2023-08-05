module;
#include "DirectXHeader.hpp"
export module DirectXHelper;
import System;

//IUnknown
export namespace System::Application::Windows::DirectX::Helpers {
	template<class T, class ...Args>
	void SafeRelease(T*& object, Args*& ...args) noexcept {
		if (object) object->Release();
		object = nullptr;
		if constexpr (sizeof...(Args) != 0) SafeRelease(args...);
	}
	template<class T, class ...Args>
	bool SafeReleaseResult(bool result, T*& object, Args*& ...args) noexcept {
		if (object) object->Release();
		object = nullptr;
		if constexpr (sizeof...(Args) != 0) return SafeReleaseResult(result, args...);
		else return result;
	}
}
//DXGI
export namespace System::Application::Windows::DirectX::Helpers {
	IDXGIFactory4* CreateDXGIFactory() {
		IDXGIFactory6* tmp6 = nullptr;
		if (CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&tmp6)) == S_OK) return tmp6;
		if (tmp6) tmp6->Release();
		IDXGIFactory4* tmp4 = nullptr;
		if (CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&tmp4)) == S_OK) return tmp4;
		else throw Exception(__FUNCSIG__, "DXGIFactoryの生成に失敗しました。", __FILE__, __LINE__);
	}
	IDXGIAdapter* GetAdapter(IDXGIFactory4& factory, const String& name) noexcept {
		Vector<IDXGIAdapter*> adapters;
		IDXGIAdapter* tmp = nullptr;
		for (uint32_t i = 0; factory.EnumAdapters(i, &tmp) != DXGI_ERROR_NOT_FOUND; ++i) adapters.Add(tmp);
		tmp = nullptr;
		if (adapters.Count() == 0) return tmp;
		else if (name.IsNullOrEmpty()) {
			tmp = adapters[0];
			adapters[0] = nullptr;
			for (auto& x : adapters) if (x) x->Release();
			return tmp;
		}
		for (auto& ad : adapters) {
			DXGI_ADAPTER_DESC desc{};
			ad->GetDesc(&desc);
			String str = desc.Description;
			if (str.find(name) != String::npos) {
				tmp = ad;
				ad = nullptr;
				break;
			}
		}
		for (auto& x : adapters) if (x) x->Release();
		return tmp;
	}
	IDXGIAdapter* CreateDXGIAdapter(IDXGIFactory4& factory) {
		IDXGIAdapter* ret = GetAdapter(factory, nullptr);
		if (ret) return ret;
		else throw Exception(__FUNCSIG__, "DXGIAdapterの取得に失敗しました。", __FILE__, __LINE__);
	}
	IDXGISwapChain4* CreateDXGISwapChain(IDXGIFactory4& factory, ID3D12CommandQueue& cmdQueue, HWND hWnd, uint32_t bufferCount) {
		if (!hWnd) throw LogicException(__FUNCSIG__, "DXGISwapChainの生成にはnullptrでないウィンドウハンドルが必要です。", __FILE__, __LINE__);
		DXGI_SWAP_CHAIN_DESC1 desc{};
		desc.Width = 0;
		desc.Height = 0;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.BufferCount = bufferCount;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.Stereo = false;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		IDXGISwapChain4* ret = nullptr;
		if (factory.CreateSwapChainForHwnd(&cmdQueue, hWnd, &desc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&ret)) == S_OK) return ret;
		else throw Exception(__FUNCSIG__, "DXGISwapChainの生成に失敗しました。", __FILE__, __LINE__);
	}
}
//Direct3D初期化用
export namespace System::Application::Windows::DirectX::Helpers {
	void EnableDebugLayer() {
		ID3D12Debug* debugLayer = nullptr;
		if (D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)) != S_OK)
			throw Exception(__FUNCSIG__, "デバッグインターフェースの取得に失敗しました。", __FILE__, __LINE__);
		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
	ID3D12Device* CreateDevice(IDXGIAdapter& adapter) {
		ID3D12Device* ret = nullptr;
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_2,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};
		for (auto& level : levels) {
			if (D3D12CreateDevice(&adapter, level, IID_PPV_ARGS(&ret)) == S_OK) break;
		}
		if (ret) {
#ifdef _DEBUG
			ret->SetName(L"Common3D::Device");
#endif
			return ret;
		}
		else throw Exception(__FUNCSIG__, "D3DDeviceの生成に失敗しました。", __FILE__, __LINE__);
	}
	ID3D12CommandAllocator* CreateCommandAllocator(ID3D12Device& device) {
		ID3D12CommandAllocator* ret = nullptr;
		if (device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&ret)) == S_OK) return ret;
		else throw Exception(__FUNCSIG__, "D3DCommandAllocatorの生成に失敗しました。", __FILE__, __LINE__);
	}
	ID3D12GraphicsCommandList* CreateCommandList(ID3D12Device& device, ID3D12CommandAllocator& allocator, ID3D12PipelineState* initialState = nullptr) {
		ID3D12GraphicsCommandList* ret = nullptr;
		if (device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, &allocator, initialState, IID_PPV_ARGS(&ret)) == S_OK) return ret;
		else throw Exception(__FUNCSIG__, "D3DCommandListの生成に失敗しました。", __FILE__, __LINE__);
	}
	ID3D12CommandQueue* CreateCommandQueue(ID3D12Device& device) {
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		ID3D12CommandQueue* ret = nullptr;
		if (device.CreateCommandQueue(&desc, IID_PPV_ARGS(&ret)) == S_OK) return ret;
		else throw Exception(__FUNCSIG__, "D3DCommandQueueの生成に失敗しました。", __FILE__, __LINE__);
	}
	ID3D12Fence* CreateFence(ID3D12Device& device) {
		ID3D12Fence* ret = nullptr;
		if (device.CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&ret)) == S_OK) return ret;
		else throw Exception(__FUNCSIG__, "D3DFenceの生成に失敗しました。", __FILE__, __LINE__);
	}
}
//GPU同期用
export namespace System::Application::Windows::DirectX::Helpers {
	uint64_t Signal(ID3D12CommandQueue& cmdQueue, ID3D12Fence& fence) noexcept {
		static uint64_t fenceVal = 0;
		cmdQueue.Signal(&fence, ++fenceVal);
		return fenceVal;
	}
	void WaitForFenceValue(ID3D12Fence& fence, uint64_t fenceVal) noexcept {
		if (!(fence.GetCompletedValue() >= fenceVal)) {
			HANDLE fenceEv = CreateEventW(nullptr, false, false, nullptr);
			fence.SetEventOnCompletion(fenceVal, fenceEv);
			WaitForSingleObject(fenceEv, INFINITE);
			CloseHandle(fenceEv);
		}
	}
}
//Create Resource
export namespace System::Application::Windows::DirectX::Helpers {
	size_t GetAlignmentedSize(size_t size, size_t alignment) noexcept {
		size_t tmp = size % alignment;
		return size + (tmp == 0 ? 0 : alignment - tmp);
	}
	ID3D12Resource* CreateTextureResource(ID3D12Device& device, uint64_t width, uint32_t height) noexcept {
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;	//テクスチャのAlignmentは0, 4, 64KB、もしくは4MB。ここでは64KB
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.DepthOrArraySize = 1;
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;	//レンダーターゲットに設定できる
		resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.Width = width;
		resDesc.Height = height;
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 0;
		prop.VisibleNodeMask = 0;
		D3D12_CLEAR_VALUE clear{};
		clear.Color[0] = 1.f;
		clear.Color[1] = 1.f;
		clear.Color[2] = 1.f;
		clear.Color[3] = 1.f;
		clear.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ID3D12Resource* tmp = nullptr;
		if (device.CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clear, IID_PPV_ARGS(&tmp)) == S_OK) return tmp;
		else return nullptr;
	}
	ID3D12Resource* CreateEmptyBuffer(ID3D12Device& device, uint64_t dataSize) {
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		D3D12_RESOURCE_DESC bufDesc{};
		bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufDesc.Width = dataSize;
		bufDesc.Height = 1;
		bufDesc.DepthOrArraySize = 1;
		bufDesc.MipLevels = 1;
		bufDesc.Format = DXGI_FORMAT_UNKNOWN;
		bufDesc.SampleDesc.Count = 1;
		bufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		ID3D12Resource* ret = nullptr;
		if (device.CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &bufDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ret)) < S_OK)
			throw Exception(__FUNCSIG__, "バッファの生成に失敗しました。", __FILE__, __LINE__);
		return ret;
	}
	template<class T>
	ID3D12Resource* CreateBuffer(ID3D12Device& device, const T* data, size_t count) {
		if (!data)
			throw LogicException(
				__FUNCSIG__, u"nullptrからバッファを作成することはできません。", __FILE__, __LINE__
			);
		const size_t size = sizeof(T) * count;
		ID3D12Resource* buffer = CreateEmptyBuffer(device, size);
		T* map = nullptr;
		if (buffer->Map(0, nullptr, reinterpret_cast<void**>(&map)) < S_OK)
			throw Exception(__FUNCSIG__, "バッファのマップに失敗しました。", __FILE__, __LINE__);
		Copy(data, map, size);
		buffer->Unmap(0, nullptr);
		return buffer;
	}
}
//RTV, DSV
export namespace System::Application::Windows::DirectX::Helpers {
	ID3D12DescriptorHeap* CreateDescriptorHeapForRTV(ID3D12Device& device, uint32_t numDescriptors) noexcept {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ID3D12DescriptorHeap* tmp = nullptr;
		if (device.CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&tmp)) == S_OK) return tmp;
		else return nullptr;
	}
	void CreateRTV(ID3D12Device& device, ID3D12DescriptorHeap& descriptorHeap, uint64_t incrementSize, ID3D12Resource* resource) noexcept {
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorHeap.GetCPUDescriptorHandleForHeapStart();
		handle.ptr += incrementSize;
		device.CreateRenderTargetView(resource, &rtvDesc, handle);
	}
	ID3D12Resource* CreateDepthStencilResource(ID3D12Device& device, uint64_t width, uint32_t height) noexcept {
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
		ID3D12Resource* tmp = nullptr;
		if (device.CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear, IID_PPV_ARGS(&tmp)) == S_OK) return tmp;
		else return nullptr;
	}
	ID3D12DescriptorHeap* CreateDescriptorHeapForDSV(ID3D12Device& device, uint32_t numDescriptors) noexcept {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NodeMask = 0;
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ID3D12DescriptorHeap* tmp = nullptr;
		if (device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&tmp)) == S_OK) return tmp;
		else return nullptr;
	}
	void CreateDSV(ID3D12Device& device, ID3D12DescriptorHeap& descriptorHeap, uint64_t incrementSize, ID3D12Resource* resource) noexcept {
		D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;
		D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorHeap.GetCPUDescriptorHandleForHeapStart();
		handle.ptr += incrementSize;
		device.CreateDepthStencilView(resource, &desc, handle);
	}
}
//ラスタイザステート設定用
export namespace System::Application::Windows::DirectX::Helpers {
	D3D12_VIEWPORT GetViewPort(uint32_t width, uint32_t height) noexcept {
		D3D12_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0;
		return viewport;
	}
	D3D12_RECT GetScissorRect(uint32_t left, uint32_t top, uint32_t width, uint32_t height) noexcept {
		D3D12_RECT scissorRect{};
		scissorRect.left = left;
		scissorRect.top = top;
		scissorRect.right = static_cast<decltype(scissorRect.right)>(scissorRect.left + width);
		scissorRect.bottom = static_cast<decltype(scissorRect.bottom)>(scissorRect.top + height);
		return scissorRect;
	}
}