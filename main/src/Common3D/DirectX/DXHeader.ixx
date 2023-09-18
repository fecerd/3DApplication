module;
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#define __SPECSTRINGS_STRICT_LEVEL 0
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#if defined(__GNUC__)
#define _MSC_VER 1933	//msys2のみ必要
#include <d3d12sdklayers.h>
#endif
#include <d3d12.h>
#undef __nullnullterminated
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#undef MessageBox
#undef MB_OK
#undef SIZE_MAX
#undef GetObject
#undef UpdateResource
export module DXHeader;

export namespace DX {
	using BOOL = ::BOOL;
	using WORD = ::WORD;
	using DWORD = ::DWORD;
	using UINT = ::UINT;
	using FLOAT = ::FLOAT;
	using ULONG = ::ULONG;
	using LONG = ::LONG;
	using LONGLONG = ::LONGLONG;
	using BYTE = ::BYTE;
	using UINT64 = ::UINT64;
	using UINT32 = ::UINT32;
	using RECT = ::RECT;
	using BITMAPINFOHEADER = ::BITMAPINFOHEADER;

	using LPVOID = ::LPVOID;
	using LPCSTR = ::LPCSTR;
	using LPCWSTR = ::LPCWSTR;

	using GUID = ::GUID;
	using IID = ::IID;
	using LPSECURITY_ATTRIBUTES = ::LPSECURITY_ATTRIBUTES;

	using IUnknown = ::IUnknown;

	using HRESULT = ::HRESULT;
	using HWND = ::HWND;
	using HANDLE = ::HANDLE;

	using IDXGIFactory = ::IDXGIFactory;
	using IDXGIFactory1 = ::IDXGIFactory1;
	using IDXGIFactory2 = ::IDXGIFactory2;
	using IDXGIFactory3 = ::IDXGIFactory3;
	using IDXGIFactory4 = ::IDXGIFactory4;
	using IDXGIFactory5 = ::IDXGIFactory5;
	using IDXGIFactory6 = ::IDXGIFactory6;
	using IDXGIFactory7 = ::IDXGIFactory7;

	using IDXGIAdapter = ::IDXGIAdapter;
	using IDXGIAdapter1 = ::IDXGIAdapter1;
	using IDXGIAdapter2 = ::IDXGIAdapter2;
	using IDXGIAdapter3 = ::IDXGIAdapter3;
	using IDXGIAdapter4 = ::IDXGIAdapter4;

	using IDXGISwapChain = ::IDXGISwapChain;
	using IDXGISwapChain1 = ::IDXGISwapChain1;
	using IDXGISwapChain2 = ::IDXGISwapChain2;
	using IDXGISwapChain3 = ::IDXGISwapChain3;
	using IDXGISwapChain4 = ::IDXGISwapChain4;

	using IDXGIOutput = ::IDXGIOutput;
	using IDXGIOutput1 = ::IDXGIOutput1;
	using IDXGIOutput2 = ::IDXGIOutput2;
	using IDXGIOutput3 = ::IDXGIOutput3;
	using IDXGIOutput4 = ::IDXGIOutput4;
	using IDXGIOutput5 = ::IDXGIOutput5;
	using IDXGIOutput6 = ::IDXGIOutput6;


	using DXGI_ADAPTER_DESC = ::DXGI_ADAPTER_DESC;
	using DXGI_SWAP_CHAIN_DESC1 = ::DXGI_SWAP_CHAIN_DESC1;

	using ID3D12Debug = ::ID3D12Debug;
	using ID3D12Debug1 = ::ID3D12Debug1;
	using ID3D12Debug2 = ::ID3D12Debug2;
	using ID3D12Debug3 = ::ID3D12Debug3;
	using ID3D12Debug4 = ::ID3D12Debug4;
	using ID3D12Debug5 = ::ID3D12Debug5;
	using ID3D12Debug6 = ::ID3D12Debug6;

	using ID3D12DebugDevice = ::ID3D12DebugDevice;
	using ID3D12DebugDevice1 = ::ID3D12DebugDevice1;
	using ID3D12DebugDevice2 = ::ID3D12DebugDevice2;

	using ID3D12Device = ::ID3D12Device;
	using ID3D12Device1 = ::ID3D12Device1;
	using ID3D12Device2 = ::ID3D12Device2;
	using ID3D12Device3 = ::ID3D12Device3;
	using ID3D12Device4 = ::ID3D12Device4;
	using ID3D12Device5 = ::ID3D12Device5;
	using ID3D12Device6 = ::ID3D12Device6;
	using ID3D12Device7 = ::ID3D12Device7;
	using ID3D12Device8 = ::ID3D12Device8;
	using ID3D12Device9 = ::ID3D12Device9;
	using ID3D12Device10 = ::ID3D12Device10;

	using ID3D12CommandAllocator = ::ID3D12CommandAllocator;

	using ID3D12CommandList = ::ID3D12CommandList;
	using ID3D12GraphicsCommandList = ::ID3D12GraphicsCommandList;
	using ID3D12GraphicsCommandList1 = ::ID3D12GraphicsCommandList1;
	using ID3D12GraphicsCommandList2 = ::ID3D12GraphicsCommandList2;
	using ID3D12GraphicsCommandList3 = ::ID3D12GraphicsCommandList3;
	using ID3D12GraphicsCommandList4 = ::ID3D12GraphicsCommandList4;
	using ID3D12GraphicsCommandList5 = ::ID3D12GraphicsCommandList5;
	using ID3D12GraphicsCommandList6 = ::ID3D12GraphicsCommandList6;
	using ID3D12GraphicsCommandList7 = ::ID3D12GraphicsCommandList7;

	using ID3D12CommandQueue = ::ID3D12CommandQueue;
	using ID3D12PipelineState = ::ID3D12PipelineState;
	using ID3D12Fence = ::ID3D12Fence;
	using ID3D12Fence1 = ::ID3D12Fence1;

	using ID3D12Resource = ::ID3D12Resource;
	using ID3D12Resource1 = ::ID3D12Resource1;
	using ID3D12Resource2 = ::ID3D12Resource2;

	using ID3D12DescriptorHeap = ::ID3D12DescriptorHeap;
	using ID3D12RootSignature = ::ID3D12RootSignature;

	using ID3DBlob = ::ID3DBlob;


	using D3D12_COMMAND_QUEUE_DESC = ::D3D12_COMMAND_QUEUE_DESC;
	using D3D12_RESOURCE_DESC = ::D3D12_RESOURCE_DESC;
	using D3D12_RESOURCE_DESC1 = ::D3D12_RESOURCE_DESC1;
	using D3D12_HEAP_PROPERTIES = ::D3D12_HEAP_PROPERTIES;
	using D3D12_CLEAR_VALUE = ::D3D12_CLEAR_VALUE;
	using D3D12_DESCRIPTOR_HEAP_DESC = ::D3D12_DESCRIPTOR_HEAP_DESC;
	using D3D12_RENDER_TARGET_VIEW_DESC = ::D3D12_RENDER_TARGET_VIEW_DESC;
	using D3D12_CPU_DESCRIPTOR_HANDLE = ::D3D12_CPU_DESCRIPTOR_HANDLE;
	using D3D12_GPU_DESCRIPTOR_HANDLE = ::D3D12_GPU_DESCRIPTOR_HANDLE;
	using D3D12_DEPTH_STENCIL_VIEW_DESC = ::D3D12_DEPTH_STENCIL_VIEW_DESC;
	using D3D12_VIEWPORT = ::D3D12_VIEWPORT;
	using D3D12_RECT = ::D3D12_RECT;
	using D3D12_DESCRIPTOR_RANGE = ::D3D12_DESCRIPTOR_RANGE;
	using D3D12_DESCRIPTOR_RANGE1 = ::D3D12_DESCRIPTOR_RANGE1;
	using D3D12_ROOT_PARAMETER = ::D3D12_ROOT_PARAMETER;
	using D3D12_ROOT_PARAMETER1 = ::D3D12_ROOT_PARAMETER1;
	using D3D12_STATIC_SAMPLER_DESC = ::D3D12_STATIC_SAMPLER_DESC;
	using D3D12_ROOT_SIGNATURE_DESC = ::D3D12_ROOT_SIGNATURE_DESC;
	using D3D12_ROOT_SIGNATURE_DESC1 = ::D3D12_ROOT_SIGNATURE_DESC1;
	using D3D12_INPUT_ELEMENT_DESC = ::D3D12_INPUT_ELEMENT_DESC;
	using D3D12_RESOURCE_BARRIER = ::D3D12_RESOURCE_BARRIER;
	using D3D12_VERTEX_BUFFER_VIEW = ::D3D12_VERTEX_BUFFER_VIEW;
	using D3D12_INDEX_BUFFER_VIEW = ::D3D12_INDEX_BUFFER_VIEW;
	using D3D_SHADER_MACRO = ::D3D_SHADER_MACRO;
	using ID3DInclude = ::ID3DInclude;
	using D3D12_TEXTURE_COPY_LOCATION = ::D3D12_TEXTURE_COPY_LOCATION;
	using D3D12_CONSTANT_BUFFER_VIEW_DESC = ::D3D12_CONSTANT_BUFFER_VIEW_DESC;
	using D3D12_SHADER_RESOURCE_VIEW_DESC = ::D3D12_SHADER_RESOURCE_VIEW_DESC;
	using D3D12_UNORDERED_ACCESS_VIEW_DESC = ::D3D12_UNORDERED_ACCESS_VIEW_DESC;
	using D3D12_GPU_VIRTUAL_ADDRESS = ::D3D12_GPU_VIRTUAL_ADDRESS;
	using D3D12_GRAPHICS_PIPELINE_STATE_DESC = ::D3D12_GRAPHICS_PIPELINE_STATE_DESC;
	using D3D12_RENDER_TARGET_BLEND_DESC = ::D3D12_RENDER_TARGET_BLEND_DESC;


#if defined(__GNUC__) && !defined(__clang__)
	using DXGI_ALPHA_MODE = ::DXGI_ALPHA_MODE;
	using DXGI_SWAP_CHAIN_FLAG = ::DXGI_SWAP_CHAIN_FLAG;
	using DXGI_FORMAT = ::DXGI_FORMAT;
	using DXGI_SCALING = ::DXGI_SCALING;
	using DXGI_SWAP_EFFECT = ::DXGI_SWAP_EFFECT;
	using D3D_FEATURE_LEVEL = ::D3D_FEATURE_LEVEL;
	using D3D12_COMMAND_LIST_TYPE = ::D3D12_COMMAND_LIST_TYPE;
	using D3D12_COMMAND_QUEUE_FLAGS = ::D3D12_COMMAND_QUEUE_FLAGS;
	using D3D12_COMMAND_QUEUE_PRIORITY = ::D3D12_COMMAND_QUEUE_PRIORITY;
	using D3D12_FENCE_FLAGS = ::D3D12_FENCE_FLAGS;
	using D3D12_RESOURCE_DIMENSION = ::D3D12_RESOURCE_DIMENSION;
	using D3D12_RESOURCE_FLAGS = ::D3D12_RESOURCE_FLAGS;
	using D3D12_TEXTURE_LAYOUT = ::D3D12_TEXTURE_LAYOUT;
	using D3D12_HEAP_TYPE = ::D3D12_HEAP_TYPE;
	using D3D12_CPU_PAGE_PROPERTY = ::D3D12_CPU_PAGE_PROPERTY;
	using D3D12_MEMORY_POOL = ::D3D12_MEMORY_POOL;
	using D3D12_HEAP_FLAGS = ::D3D12_HEAP_FLAGS;
	using D3D12_RESOURCE_STATES = ::D3D12_RESOURCE_STATES;
	using D3D12_DESCRIPTOR_HEAP_TYPE = ::D3D12_DESCRIPTOR_HEAP_TYPE;
	using D3D12_DESCRIPTOR_HEAP_FLAGS = ::D3D12_DESCRIPTOR_HEAP_FLAGS;
	using D3D12_RTV_DIMENSION = ::D3D12_RTV_DIMENSION;
	using D3D12_DSV_DIMENSION = ::D3D12_DSV_DIMENSION;
	using D3D12_DSV_FLAGS = ::D3D12_DSV_FLAGS;
	using D3D12_DESCRIPTOR_RANGE_TYPE = ::D3D12_DESCRIPTOR_RANGE_TYPE;
	using D3D12_SHADER_VISIBILITY = ::D3D12_SHADER_VISIBILITY;
	using D3D12_ROOT_PARAMETER_TYPE = ::D3D12_ROOT_PARAMETER_TYPE;
	using D3D12_TEXTURE_ADDRESS_MODE = ::D3D12_TEXTURE_ADDRESS_MODE;
	using D3D12_STATIC_BORDER_COLOR = ::D3D12_STATIC_BORDER_COLOR;
	using D3D12_FILTER = ::D3D12_FILTER;
	using D3D12_COMPARISON_FUNC = ::D3D12_COMPARISON_FUNC;
	using D3D12_ROOT_SIGNATURE_FLAGS = ::D3D12_ROOT_SIGNATURE_FLAGS;
	using D3D_ROOT_SIGNATURE_VERSION = ::D3D_ROOT_SIGNATURE_VERSION;
	using D3D12_CULL_MODE = ::D3D12_CULL_MODE;
	using D3D12_BLEND = ::D3D12_BLEND;
	using D3D12_BLEND_OP = ::D3D12_BLEND_OP;
	using D3D12_INPUT_CLASSIFICATION = ::D3D12_INPUT_CLASSIFICATION;
	using D3D12_RESOURCE_BARRIER_TYPE = ::D3D12_RESOURCE_BARRIER_TYPE;
	using D3D12_RESOURCE_BARRIER_FLAGS = ::D3D12_RESOURCE_BARRIER_FLAGS;
	using D3D12_RLDO_FLAGS = ::D3D12_RLDO_FLAGS;
	using D3D12_TEXTURE_COPY_TYPE = ::D3D12_TEXTURE_COPY_TYPE;
	using D3D12_SRV_DIMENSION = ::D3D12_SRV_DIMENSION;
	using D3D12_UAV_DIMENSION = ::D3D12_UAV_DIMENSION;
	using D3D12_FILL_MODE = ::D3D12_FILL_MODE;
	using D3D12_DEPTH_WRITE_MASK = ::D3D12_DEPTH_WRITE_MASK;
	using D3D12_COLOR_WRITE_ENABLE = ::D3D12_COLOR_WRITE_ENABLE;
	using D3D12_INDEX_BUFFER_STRIP_CUT_VALUE = ::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE;
	using D3D12_PRIMITIVE_TOPOLOGY_TYPE = ::D3D12_PRIMITIVE_TOPOLOGY_TYPE;
	using D3D12_CLEAR_FLAGS = ::D3D12_CLEAR_FLAGS;
	using D3D_PRIMITIVE_TOPOLOGY = ::D3D_PRIMITIVE_TOPOLOGY;
#endif
}

#if !defined(__GNUC__) || defined(__clang__)
export {
	using DXGI_ALPHA_MODE = ::DXGI_ALPHA_MODE;
	using DXGI_SWAP_CHAIN_FLAG = ::DXGI_SWAP_CHAIN_FLAG;
	using DXGI_FORMAT = ::DXGI_FORMAT;
	using DXGI_SCALING = ::DXGI_SCALING;
	using DXGI_SWAP_EFFECT = ::DXGI_SWAP_EFFECT;

	using D3D_FEATURE_LEVEL = ::D3D_FEATURE_LEVEL;
	using D3D12_COMMAND_LIST_TYPE = ::D3D12_COMMAND_LIST_TYPE;
	using D3D12_COMMAND_QUEUE_FLAGS = ::D3D12_COMMAND_QUEUE_FLAGS;
	using D3D12_COMMAND_QUEUE_PRIORITY = ::D3D12_COMMAND_QUEUE_PRIORITY;
	using D3D12_FENCE_FLAGS = ::D3D12_FENCE_FLAGS;
	using D3D12_RESOURCE_DIMENSION = ::D3D12_RESOURCE_DIMENSION;
	using D3D12_RESOURCE_FLAGS = ::D3D12_RESOURCE_FLAGS;
	using D3D12_TEXTURE_LAYOUT = ::D3D12_TEXTURE_LAYOUT;
	using D3D12_HEAP_TYPE = ::D3D12_HEAP_TYPE;
	using D3D12_CPU_PAGE_PROPERTY = ::D3D12_CPU_PAGE_PROPERTY;
	using D3D12_MEMORY_POOL = ::D3D12_MEMORY_POOL;
	using D3D12_HEAP_FLAGS = ::D3D12_HEAP_FLAGS;
	using D3D12_RESOURCE_STATES = ::D3D12_RESOURCE_STATES;
	using D3D12_DESCRIPTOR_HEAP_TYPE = ::D3D12_DESCRIPTOR_HEAP_TYPE;
	using D3D12_DESCRIPTOR_HEAP_FLAGS = ::D3D12_DESCRIPTOR_HEAP_FLAGS;
	using D3D12_RTV_DIMENSION = ::D3D12_RTV_DIMENSION;
	using D3D12_DSV_DIMENSION = ::D3D12_DSV_DIMENSION;
	using D3D12_DSV_FLAGS = ::D3D12_DSV_FLAGS;
	using D3D12_DESCRIPTOR_RANGE_TYPE = ::D3D12_DESCRIPTOR_RANGE_TYPE;
	using D3D12_SHADER_VISIBILITY = ::D3D12_SHADER_VISIBILITY;
	using D3D12_ROOT_PARAMETER_TYPE = ::D3D12_ROOT_PARAMETER_TYPE;
	using D3D12_TEXTURE_ADDRESS_MODE = ::D3D12_TEXTURE_ADDRESS_MODE;
	using D3D12_STATIC_BORDER_COLOR = ::D3D12_STATIC_BORDER_COLOR;
	using D3D12_FILTER = ::D3D12_FILTER;
	using D3D12_COMPARISON_FUNC = ::D3D12_COMPARISON_FUNC;
	using D3D12_ROOT_SIGNATURE_FLAGS = ::D3D12_ROOT_SIGNATURE_FLAGS;
	using D3D_ROOT_SIGNATURE_VERSION = ::D3D_ROOT_SIGNATURE_VERSION;
	using D3D12_CULL_MODE = ::D3D12_CULL_MODE;
	using D3D12_BLEND = ::D3D12_BLEND;
	using D3D12_BLEND_OP = ::D3D12_BLEND_OP;
	using D3D12_INPUT_CLASSIFICATION = ::D3D12_INPUT_CLASSIFICATION;
	using D3D12_RESOURCE_BARRIER_TYPE = ::D3D12_RESOURCE_BARRIER_TYPE;
	using D3D12_RESOURCE_BARRIER_FLAGS = ::D3D12_RESOURCE_BARRIER_FLAGS;
	using D3D12_RLDO_FLAGS = ::D3D12_RLDO_FLAGS;
	using D3D12_TEXTURE_COPY_TYPE = ::D3D12_TEXTURE_COPY_TYPE;
	using D3D12_SRV_DIMENSION = ::D3D12_SRV_DIMENSION;
	using D3D12_UAV_DIMENSION = ::D3D12_UAV_DIMENSION;
	using D3D12_FILL_MODE = ::D3D12_FILL_MODE;
	using D3D12_DEPTH_WRITE_MASK = ::D3D12_DEPTH_WRITE_MASK;
	using D3D12_COLOR_WRITE_ENABLE = ::D3D12_COLOR_WRITE_ENABLE;
	using D3D12_INDEX_BUFFER_STRIP_CUT_VALUE = ::D3D12_INDEX_BUFFER_STRIP_CUT_VALUE;
	using D3D12_PRIMITIVE_TOPOLOGY_TYPE = ::D3D12_PRIMITIVE_TOPOLOGY_TYPE;
	using D3D12_CLEAR_FLAGS = ::D3D12_CLEAR_FLAGS;
	using D3D_PRIMITIVE_TOPOLOGY = ::D3D_PRIMITIVE_TOPOLOGY;
}
#endif

namespace DX::Internal {
	inline constexpr HRESULT S_OK_Internal = S_OK;
	inline constexpr HRESULT E_FAIL_Internal = E_FAIL;
	inline constexpr HRESULT E_POINTER_Internal = E_POINTER;
	inline constexpr HRESULT E_NOTIMPL_Internal = E_NOTIMPL;
	inline constexpr BOOL TRUE_Internal = TRUE;
	inline constexpr BOOL FALSE_Internal = FALSE;
	inline constexpr DWORD INFINITE_Internal = INFINITE;

	inline constexpr HRESULT DXGI_ERROR_NOT_FOUND_Internal = DXGI_ERROR_NOT_FOUND;
	inline constexpr UINT DXGI_CREATE_FACTORY_DEBUG_Internal = DXGI_CREATE_FACTORY_DEBUG;
	inline constexpr UINT DXGI_USAGE_BACK_BUFFER_Internal = DXGI_USAGE_BACK_BUFFER;

	inline constexpr UINT64 D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT_Internal = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	inline constexpr UINT D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND_Internal = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	inline constexpr FLOAT D3D12_FLOAT32_MAX_Internal = D3D12_FLOAT32_MAX;
	inline constexpr UINT D3D12_APPEND_ALIGNED_ELEMENT_Internal = D3D12_APPEND_ALIGNED_ELEMENT;
	inline constexpr UINT D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES_Internal = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	inline ID3DInclude* const D3D_COMPILE_STANDARD_FILE_INCLUDE_Internal = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	inline constexpr UINT D3DCOMPILE_DEBUG_Internal = D3DCOMPILE_DEBUG;
	inline constexpr UINT D3DCOMPILE_SKIP_OPTIMIZATION_Internal = D3DCOMPILE_SKIP_OPTIMIZATION;
	inline constexpr UINT D3D12_TEXTURE_DATA_PITCH_ALIGNMENT_Internal = D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
	inline constexpr UINT D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING_Internal = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	inline constexpr UINT D3D12_DEFAULT_SAMPLE_MASK_Internal = D3D12_DEFAULT_SAMPLE_MASK;

#undef S_OK
#undef E_FAIL
#undef E_POINTER
#undef E_NOTIMPL
#undef TRUE
#undef FALSE
#undef INFINITE

#undef DXGI_ERROR_NOT_FOUND
#undef DXGI_CREATE_FACTORY_DEBUG
#undef DXGI_USAGE_BACK_BUFFER

#undef D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT
#undef D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
#undef D3D12_FLOAT32_MAX
#undef D3D12_APPEND_ALIGNED_ELEMENT
#undef D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES
#undef D3D_COMPILE_STANDARD_FILE_INCLUDE
#undef D3DCOMPILE_DEBUG
#undef D3DCOMPILE_SKIP_OPTIMIZATION
#undef D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
#undef D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
#undef D3D12_DEFAULT_SAMPLE_MASK
}

export namespace DX {
	inline constexpr HRESULT S_OK = Internal::S_OK_Internal;
	inline constexpr HRESULT E_FAIL = Internal::E_FAIL_Internal;
	inline constexpr HRESULT E_POINTER = Internal::E_POINTER_Internal;
	inline constexpr HRESULT E_NOTIMPL = Internal::E_NOTIMPL_Internal;	
	inline constexpr BOOL TRUE = Internal::TRUE_Internal;
	inline constexpr BOOL FALSE = Internal::FALSE_Internal;
	inline constexpr DWORD INFINITE = Internal::INFINITE_Internal;

	inline constexpr HRESULT DXGI_ERROR_NOT_FOUND = Internal::DXGI_ERROR_NOT_FOUND_Internal;
	inline constexpr UINT DXGI_CREATE_FACTORY_DEBUG = Internal::DXGI_CREATE_FACTORY_DEBUG_Internal;
	inline constexpr UINT DXGI_USAGE_BACK_BUFFER = Internal::DXGI_USAGE_BACK_BUFFER_Internal;

	inline constexpr UINT64 D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT = Internal::D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT_Internal;
	inline constexpr UINT D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND = Internal::D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND_Internal;
	inline constexpr FLOAT D3D12_FLOAT32_MAX = Internal::D3D12_FLOAT32_MAX_Internal;
	inline constexpr UINT D3D12_APPEND_ALIGNED_ELEMENT = Internal::D3D12_APPEND_ALIGNED_ELEMENT_Internal;
	inline constexpr UINT D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES = Internal::D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES_Internal;
	inline ID3DInclude* const D3D_COMPILE_STANDARD_FILE_INCLUDE = Internal::D3D_COMPILE_STANDARD_FILE_INCLUDE_Internal;
	inline constexpr UINT D3DCOMPILE_DEBUG = Internal::D3DCOMPILE_DEBUG_Internal;
	inline constexpr UINT D3DCOMPILE_SKIP_OPTIMIZATION = Internal::D3DCOMPILE_SKIP_OPTIMIZATION_Internal;
	inline constexpr UINT D3D12_TEXTURE_DATA_PITCH_ALIGNMENT = Internal::D3D12_TEXTURE_DATA_PITCH_ALIGNMENT_Internal;
	inline constexpr UINT D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING = Internal::D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING_Internal;
	inline constexpr UINT D3D12_DEFAULT_SAMPLE_MASK = Internal::D3D12_DEFAULT_SAMPLE_MASK_Internal;
}

export namespace DX {
	inline constexpr bool Succeeded(HRESULT hr) noexcept {
		return SUCCEEDED(hr);
	}
	inline constexpr bool Failed(HRESULT hr) noexcept {
		return FAILED(hr);
	}

	template <class T, class ...Args>
	void SafeRelease(T*& object, Args*& ...args) noexcept {
		if (object) {
			object->Release();
			object = nullptr;
		}
		if constexpr (sizeof...(Args) != 0) SafeRelease(args...);
	}
	template<class T, class ...Args>
	bool SafeReleaseResult(bool result, T*& object, Args*& ...args) noexcept {
		if (object) {
			object->Release();
			object = nullptr;
		}
		if constexpr (sizeof...(Args) == 0) return result;
		else return SafeReleaseResult(result, args...);
	}

	HRESULT CreateDXGIFactory2(UINT Flags, const IID &riid, void **ppFactory) noexcept {
		return ::CreateDXGIFactory2(Flags, riid, ppFactory);
	}

	HRESULT D3D12GetDebugInterface(const IID &riid, void **ppvDebug) noexcept {
		return ::D3D12GetDebugInterface(riid, ppvDebug);
	}

	HRESULT D3D12CreateDevice(
		IUnknown *pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, const IID &riid, void **ppDevice
	) noexcept {
		return ::D3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
	}

	HRESULT D3D12SerializeRootSignature(
		const D3D12_ROOT_SIGNATURE_DESC *pRootSignature, D3D_ROOT_SIGNATURE_VERSION Version,
		ID3DBlob **ppBlob, ID3DBlob **ppErrorBlob
	) noexcept {
		return ::D3D12SerializeRootSignature(pRootSignature, Version, ppBlob, ppErrorBlob);
	}

	HRESULT D3DCompileFromFile(
		LPCWSTR pFileName, const D3D_SHADER_MACRO *pDefines, ID3DInclude *pInclude,
		LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2,
		ID3DBlob **ppCode, ID3DBlob **ppErrorMsgs
	) noexcept {
		return ::D3DCompileFromFile(
			pFileName, pDefines, pInclude, pEntrypoint, pTarget,
			Flags1, Flags2, ppCode, ppErrorMsgs
		);
	}

#undef CreateEvent
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset,
		BOOL bInitialState, LPCWSTR lpName
	) noexcept {
		return ::CreateEventW(lpEventAttributes, bManualReset, bInitialState, lpName);
	}
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset,
		BOOL bInitialState, LPCSTR lpName
	) noexcept {
		return ::CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName);
	}
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset,
		BOOL bInitialState, std::nullptr_t
	) noexcept {
		return ::CreateEventA(lpEventAttributes, bManualReset, bInitialState, nullptr);
	}
	BOOL SetEvent(HANDLE hEvent) noexcept {
		return ::SetEvent(hEvent);
	}

	BOOL CloseHandle(HANDLE hObject) noexcept {
		return ::CloseHandle(hObject);
	}

	DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) noexcept {
		return ::WaitForSingleObject(hHandle, dwMilliseconds);
	}

#if defined(__GNUC__) && !defined(__clang__)
	//g++でこれがないと未定義になる。
	template<class T> const GUID& __mingw_uuidof();
#endif

	template<class T>
	constexpr IID GetIID(T**) noexcept {
		return __uuidof(T);
	}
	template<class T>
	requires requires(T** pp) { static_cast<IUnknown*>(*pp); }
	void** IID_PPV_ARGS_Helper(T** pp) noexcept {
        return reinterpret_cast<void**>(pp);
	}

	//以下のマクロをimport先で定義する
	#undef IID_PPV_ARGS
	#define IID_PPV_ARGS(ppType) MF::GetIID(ppType), MF::IID_PPV_ARGS_Helper(ppType)
}

