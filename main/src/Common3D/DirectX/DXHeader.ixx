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
	using D3D12_BOX = ::D3D12_BOX;

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

#if defined(__GNUC__) //&& !defined(__clang__)

#define WINADAPTER_IID(InterfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
export inline constexpr GUID InterfaceName##_IID = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }; \
export template <> constexpr _GUID const& __mingw_uuidof< InterfaceName >() \
{ \
    return InterfaceName##_IID; \
}
WINADAPTER_IID(ID3D12Object, 0xc4fec28f, 0x7966, 0x4e95, 0x9f, 0x94, 0xf4, 0x31, 0xcb, 0x56, 0xc3, 0xb8);
WINADAPTER_IID(ID3D12DeviceChild, 0x905db94b, 0xa00c, 0x4140, 0x9d, 0xf5, 0x2b, 0x64, 0xca, 0x9e, 0xa3, 0x57);
WINADAPTER_IID(ID3D12RootSignature, 0xc54a6b66, 0x72df, 0x4ee8, 0x8b, 0xe5, 0xa9, 0x46, 0xa1, 0x42, 0x92, 0x14);
WINADAPTER_IID(ID3D12RootSignatureDeserializer, 0x34AB647B, 0x3CC8, 0x46AC, 0x84, 0x1B, 0xC0, 0x96, 0x56, 0x45, 0xC0, 0x46);
WINADAPTER_IID(ID3D12VersionedRootSignatureDeserializer, 0x7F91CE67, 0x090C, 0x4BB7, 0xB7, 0x8E, 0xED, 0x8F, 0xF2, 0xE3, 0x1D, 0xA0);
WINADAPTER_IID(ID3D12Pageable, 0x63ee58fb, 0x1268, 0x4835, 0x86, 0xda, 0xf0, 0x08, 0xce, 0x62, 0xf0, 0xd6);
WINADAPTER_IID(ID3D12Heap, 0x6b3b2502, 0x6e51, 0x45b3, 0x90, 0xee, 0x98, 0x84, 0x26, 0x5e, 0x8d, 0xf3);
WINADAPTER_IID(ID3D12Resource, 0x696442be, 0xa72e, 0x4059, 0xbc, 0x79, 0x5b, 0x5c, 0x98, 0x04, 0x0f, 0xad);
WINADAPTER_IID(ID3D12CommandAllocator, 0x6102dee4, 0xaf59, 0x4b09, 0xb9, 0x99, 0xb4, 0x4d, 0x73, 0xf0, 0x9b, 0x24);
WINADAPTER_IID(ID3D12Fence, 0x0a753dcf, 0xc4d8, 0x4b91, 0xad, 0xf6, 0xbe, 0x5a, 0x60, 0xd9, 0x5a, 0x76);
WINADAPTER_IID(ID3D12Fence1, 0x433685fe, 0xe22b, 0x4ca0, 0xa8, 0xdb, 0xb5, 0xb4, 0xf4, 0xdd, 0x0e, 0x4a);
WINADAPTER_IID(ID3D12PipelineState, 0x765a30f3, 0xf624, 0x4c6f, 0xa8, 0x28, 0xac, 0xe9, 0x48, 0x62, 0x24, 0x45);
WINADAPTER_IID(ID3D12DescriptorHeap, 0x8efb471d, 0x616c, 0x4f49, 0x90, 0xf7, 0x12, 0x7b, 0xb7, 0x63, 0xfa, 0x51);
WINADAPTER_IID(ID3D12QueryHeap, 0x0d9658ae, 0xed45, 0x469e, 0xa6, 0x1d, 0x97, 0x0e, 0xc5, 0x83, 0xca, 0xb4);
WINADAPTER_IID(ID3D12CommandSignature, 0xc36a797c, 0xec80, 0x4f0a, 0x89, 0x85, 0xa7, 0xb2, 0x47, 0x50, 0x82, 0xd1);
WINADAPTER_IID(ID3D12CommandList, 0x7116d91c, 0xe7e4, 0x47ce, 0xb8, 0xc6, 0xec, 0x81, 0x68, 0xf4, 0x37, 0xe5);
WINADAPTER_IID(ID3D12GraphicsCommandList, 0x5b160d0f, 0xac1b, 0x4185, 0x8b, 0xa8, 0xb3, 0xae, 0x42, 0xa5, 0xa4, 0x55);
WINADAPTER_IID(ID3D12GraphicsCommandList1, 0x553103fb, 0x1fe7, 0x4557, 0xbb, 0x38, 0x94, 0x6d, 0x7d, 0x0e, 0x7c, 0xa7);
WINADAPTER_IID(ID3D12GraphicsCommandList2, 0x38C3E585, 0xFF17, 0x412C, 0x91, 0x50, 0x4F, 0xC6, 0xF9, 0xD7, 0x2A, 0x28);
WINADAPTER_IID(ID3D12CommandQueue, 0x0ec870a6, 0x5d7e, 0x4c22, 0x8c, 0xfc, 0x5b, 0xaa, 0xe0, 0x76, 0x16, 0xed);
WINADAPTER_IID(ID3D12Device, 0x189819f1, 0x1db6, 0x4b57, 0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7);
WINADAPTER_IID(ID3D12PipelineLibrary, 0xc64226a8, 0x9201, 0x46af, 0xb4, 0xcc, 0x53, 0xfb, 0x9f, 0xf7, 0x41, 0x4f);
WINADAPTER_IID(ID3D12PipelineLibrary1, 0x80eabf42, 0x2568, 0x4e5e, 0xbd, 0x82, 0xc3, 0x7f, 0x86, 0x96, 0x1d, 0xc3);
WINADAPTER_IID(ID3D12Device1, 0x77acce80, 0x638e, 0x4e65, 0x88, 0x95, 0xc1, 0xf2, 0x33, 0x86, 0x86, 0x3e);
WINADAPTER_IID(ID3D12Device2, 0x30baa41e, 0xb15b, 0x475c, 0xa0, 0xbb, 0x1a, 0xf5, 0xc5, 0xb6, 0x43, 0x28);
WINADAPTER_IID(ID3D12Device3, 0x81dadc15, 0x2bad, 0x4392, 0x93, 0xc5, 0x10, 0x13, 0x45, 0xc4, 0xaa, 0x98);
WINADAPTER_IID(ID3D12ProtectedSession, 0xA1533D18, 0x0AC1, 0x4084, 0x85, 0xB9, 0x89, 0xA9, 0x61, 0x16, 0x80, 0x6B);
WINADAPTER_IID(ID3D12ProtectedResourceSession, 0x6CD696F4, 0xF289, 0x40CC, 0x80, 0x91, 0x5A, 0x6C, 0x0A, 0x09, 0x9C, 0x3D);
WINADAPTER_IID(ID3D12Device4, 0xe865df17, 0xa9ee, 0x46f9, 0xa4, 0x63, 0x30, 0x98, 0x31, 0x5a, 0xa2, 0xe5);
WINADAPTER_IID(ID3D12LifetimeOwner, 0xe667af9f, 0xcd56, 0x4f46, 0x83, 0xce, 0x03, 0x2e, 0x59, 0x5d, 0x70, 0xa8);
WINADAPTER_IID(ID3D12SwapChainAssistant, 0xf1df64b6, 0x57fd, 0x49cd, 0x88, 0x07, 0xc0, 0xeb, 0x88, 0xb4, 0x5c, 0x8f);
WINADAPTER_IID(ID3D12LifetimeTracker, 0x3fd03d36, 0x4eb1, 0x424a, 0xa5, 0x82, 0x49, 0x4e, 0xcb, 0x8b, 0xa8, 0x13);
WINADAPTER_IID(ID3D12StateObject, 0x47016943, 0xfca8, 0x4594, 0x93, 0xea, 0xaf, 0x25, 0x8b, 0x55, 0x34, 0x6d);
WINADAPTER_IID(ID3D12StateObjectProperties, 0xde5fa827, 0x9bf9, 0x4f26, 0x89, 0xff, 0xd7, 0xf5, 0x6f, 0xde, 0x38, 0x60);
WINADAPTER_IID(ID3D12Device5, 0x8b4f173b, 0x2fea, 0x4b80, 0x8f, 0x58, 0x43, 0x07, 0x19, 0x1a, 0xb9, 0x5d);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedDataSettings, 0x82BC481C, 0x6B9B, 0x4030, 0xAE, 0xDB, 0x7E, 0xE3, 0xD1, 0xDF, 0x1E, 0x63);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedDataSettings1, 0xDBD5AE51, 0x3317, 0x4F0A, 0xAD, 0xF9, 0x1D, 0x7C, 0xED, 0xCA, 0xAE, 0x0B);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedDataSettings2, 0x61552388, 0x01ab, 0x4008, 0xa4, 0x36, 0x83, 0xdb, 0x18, 0x95, 0x66, 0xea);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedData, 0x98931D33, 0x5AE8, 0x4791, 0xAA, 0x3C, 0x1A, 0x73, 0xA2, 0x93, 0x4E, 0x71);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedData1, 0x9727A022, 0xCF1D, 0x4DDA, 0x9E, 0xBA, 0xEF, 0xFA, 0x65, 0x3F, 0xC5, 0x06);
WINADAPTER_IID(ID3D12DeviceRemovedExtendedData2, 0x67FC5816, 0xE4CA, 0x4915, 0xBF, 0x18, 0x42, 0x54, 0x12, 0x72, 0xDA, 0x54);
WINADAPTER_IID(ID3D12Device6, 0xc70b221b, 0x40e4, 0x4a17, 0x89, 0xaf, 0x02, 0x5a, 0x07, 0x27, 0xa6, 0xdc);
WINADAPTER_IID(ID3D12ProtectedResourceSession1, 0xD6F12DD6, 0x76FB, 0x406E, 0x89, 0x61, 0x42, 0x96, 0xEE, 0xFC, 0x04, 0x09);
WINADAPTER_IID(ID3D12Device7, 0x5c014b53, 0x68a1, 0x4b9b, 0x8b, 0xd1, 0xdd, 0x60, 0x46, 0xb9, 0x35, 0x8b);
WINADAPTER_IID(ID3D12Device8, 0x9218E6BB, 0xF944, 0x4F7E, 0xA7, 0x5C, 0xB1, 0xB2, 0xC7, 0xB7, 0x01, 0xF3);
WINADAPTER_IID(ID3D12Resource1, 0x9D5E227A, 0x4430, 0x4161, 0x88, 0xB3, 0x3E, 0xCA, 0x6B, 0xB1, 0x6E, 0x19);
WINADAPTER_IID(ID3D12Resource2, 0xBE36EC3B, 0xEA85, 0x4AEB, 0xA4, 0x5A, 0xE9, 0xD7, 0x64, 0x04, 0xA4, 0x95);
WINADAPTER_IID(ID3D12Heap1, 0x572F7389, 0x2168, 0x49E3, 0x96, 0x93, 0xD6, 0xDF, 0x58, 0x71, 0xBF, 0x6D);
WINADAPTER_IID(ID3D12GraphicsCommandList3, 0x6FDA83A7, 0xB84C, 0x4E38, 0x9A, 0xC8, 0xC7, 0xBD, 0x22, 0x01, 0x6B, 0x3D);
WINADAPTER_IID(ID3D12MetaCommand, 0xDBB84C27, 0x36CE, 0x4FC9, 0xB8, 0x01, 0xF0, 0x48, 0xC4, 0x6A, 0xC5, 0x70);
WINADAPTER_IID(ID3D12GraphicsCommandList4, 0x8754318e, 0xd3a9, 0x4541, 0x98, 0xcf, 0x64, 0x5b, 0x50, 0xdc, 0x48, 0x74);
WINADAPTER_IID(ID3D12ShaderCacheSession, 0x28e2495d, 0x0f64, 0x4ae4, 0xa6, 0xec, 0x12, 0x92, 0x55, 0xdc, 0x49, 0xa8);
WINADAPTER_IID(ID3D12Device9, 0x4c80e962, 0xf032, 0x4f60, 0xbc, 0x9e, 0xeb, 0xc2, 0xcf, 0xa1, 0xd8, 0x3c);
WINADAPTER_IID(ID3D12Device10, 0x517f8718, 0xaa66, 0x49f9, 0xb0, 0x2b, 0xa7, 0xab, 0x89, 0xc0, 0x60, 0x31);
WINADAPTER_IID(ID3D12Device11, 0x5405c344, 0xd457, 0x444e, 0xb4, 0xdd, 0x23, 0x66, 0xe4, 0x5a, 0xee, 0x39);
WINADAPTER_IID(ID3D12VirtualizationGuestDevice, 0xbc66d368, 0x7373, 0x4943, 0x87, 0x57, 0xfc, 0x87, 0xdc, 0x79, 0xe4, 0x76);
WINADAPTER_IID(ID3D12Tools, 0x7071e1f0, 0xe84b, 0x4b33, 0x97, 0x4f, 0x12, 0xfa, 0x49, 0xde, 0x65, 0xc5);
WINADAPTER_IID(ID3D12SDKConfiguration, 0xe9eb5314, 0x33aa, 0x42b2, 0xa7, 0x18, 0xd7, 0x7f, 0x58, 0xb1, 0xf1, 0xc7);
WINADAPTER_IID(ID3D12SDKConfiguration1, 0x8aaf9303, 0xad25, 0x48b9, 0x9a, 0x57, 0xd9, 0xc3, 0x7e, 0x00, 0x9d, 0x9f);
WINADAPTER_IID(ID3D12DeviceFactory, 0x61f307d3, 0xd34e, 0x4e7c, 0x83, 0x74, 0x3b, 0xa4, 0xde, 0x23, 0xcc, 0xcb);
WINADAPTER_IID(ID3D12DeviceConfiguration, 0x78dbf87b, 0xf766, 0x422b, 0xa6, 0x1c, 0xc8, 0xc4, 0x46, 0xbd, 0xb9, 0xad);
WINADAPTER_IID(ID3D12GraphicsCommandList5, 0x55050859, 0x4024, 0x474c, 0x87, 0xf5, 0x64, 0x72, 0xea, 0xee, 0x44, 0xea);
WINADAPTER_IID(ID3D12GraphicsCommandList6, 0xc3827890, 0xe548, 0x4cfa, 0x96, 0xcf, 0x56, 0x89, 0xa9, 0x37, 0x0f, 0x80);
WINADAPTER_IID(ID3D12GraphicsCommandList7, 0xdd171223, 0x8b61, 0x4769, 0x90, 0xe3, 0x16, 0x0c, 0xcd, 0xe4, 0xe2, 0xc1);
WINADAPTER_IID(ID3D12GraphicsCommandList8, 0xee936ef9, 0x599d, 0x4d28, 0x93, 0x8e, 0x23, 0xc4, 0xad, 0x05, 0xce, 0x51);
WINADAPTER_IID(ID3D12Debug, 0x344488b7, 0x6846, 0x474b, 0xb9, 0x89, 0xf0, 0x27, 0x44, 0x82, 0x45, 0xe0);
WINADAPTER_IID(ID3D12Debug1, 0xaffaa4ca, 0x63fe, 0x4d8e, 0xb8, 0xad, 0x15, 0x90, 0x00, 0xaf, 0x43, 0x04);
WINADAPTER_IID(ID3D12Debug2, 0x93a665c4, 0xa3b2, 0x4e5d, 0xb6, 0x92, 0xa2, 0x6a, 0xe1, 0x4e, 0x33, 0x74);
WINADAPTER_IID(ID3D12Debug3, 0x5cf4e58f, 0xf671, 0x4ff1, 0xa5, 0x42, 0x36, 0x86, 0xe3, 0xd1, 0x53, 0xd1);
WINADAPTER_IID(ID3D12Debug4, 0x014b816e, 0x9ec5, 0x4a2f, 0xa8, 0x45, 0xff, 0xbe, 0x44, 0x1c, 0xe1, 0x3a);
WINADAPTER_IID(ID3D12Debug5, 0x548d6b12, 0x09fa, 0x40e0, 0x90, 0x69, 0x5d, 0xcd, 0x58, 0x9a, 0x52, 0xc9);
WINADAPTER_IID(ID3D12Debug6, 0x82a816d6, 0x5d01, 0x4157, 0x97, 0xd0, 0x49, 0x75, 0x46, 0x3f, 0xd1, 0xed);
WINADAPTER_IID(ID3D12DebugDevice1, 0xa9b71770, 0xd099, 0x4a65, 0xa6, 0x98, 0x3d, 0xee, 0x10, 0x02, 0x0f, 0x88);
WINADAPTER_IID(ID3D12DebugDevice, 0x3febd6dd, 0x4973, 0x4787, 0x81, 0x94, 0xe4, 0x5f, 0x9e, 0x28, 0x92, 0x3e);
WINADAPTER_IID(ID3D12DebugDevice2, 0x60eccbc1, 0x378d, 0x4df1, 0x89, 0x4c, 0xf8, 0xac, 0x5c, 0xe4, 0xd7, 0xdd);
WINADAPTER_IID(ID3D12DebugCommandQueue, 0x09e0bf36, 0x54ac, 0x484f, 0x88, 0x47, 0x4b, 0xae, 0xea, 0xb6, 0x05, 0x3a);
WINADAPTER_IID(ID3D12DebugCommandQueue1, 0x16be35a2, 0xbfd6, 0x49f2, 0xbc, 0xae, 0xea, 0xae, 0x4a, 0xff, 0x86, 0x2d);
WINADAPTER_IID(ID3D12DebugCommandList1, 0x102ca951, 0x311b, 0x4b01, 0xb1, 0x1f, 0xec, 0xb8, 0x3e, 0x06, 0x1b, 0x37);
WINADAPTER_IID(ID3D12DebugCommandList, 0x09e0bf36, 0x54ac, 0x484f, 0x88, 0x47, 0x4b, 0xae, 0xea, 0xb6, 0x05, 0x3f);
WINADAPTER_IID(ID3D12DebugCommandList2, 0xaeb575cf, 0x4e06, 0x48be, 0xba, 0x3b, 0xc4, 0x50, 0xfc, 0x96, 0x65, 0x2e);
WINADAPTER_IID(ID3D12DebugCommandList3, 0x197d5e15, 0x4d37, 0x4d34, 0xaf, 0x78, 0x72, 0x4c, 0xd7, 0x0f, 0xdb, 0x1f);
WINADAPTER_IID(ID3D12SharingContract, 0x0adf7d52, 0x929c, 0x4e61, 0xad, 0xdb, 0xff, 0xed, 0x30, 0xde, 0x66, 0xef);
WINADAPTER_IID(ID3D12InfoQueue, 0x0742a90b, 0xc387, 0x483f, 0xb9, 0x46, 0x30, 0xa7, 0xe4, 0xe6, 0x14, 0x58);
WINADAPTER_IID(ID3D12InfoQueue1, 0x2852dd88, 0xb484, 0x4c0c, 0xb6, 0xb1, 0x67, 0x16, 0x85, 0x00, 0xe6, 0x00);

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

