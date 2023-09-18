module;
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#include <mfapi.h>
#include <mfidl.h>
#include <evr.h>
#include <Mferror.h>
#include <Shlwapi.h>
#include <mfreadwrite.h>
//g++の内部コンパイルエラーを防ぐためのヘッダモジュール
//他のコンパイラにとっては冗長だがエラーはでないのでよしとする。
export module MFHeader;

export namespace MF {
	using WORD = ::WORD;
	using DWORD = ::DWORD;
	using ULONG = ::ULONG;
	using LONG = ::LONG;
	using LONGLONG = ::LONGLONG;
	using BYTE = ::BYTE;
	using UINT64 = ::UINT64;
	using UINT32 = ::UINT32;
	using BOOL = ::BOOL;
	using RECT = ::RECT;
	using BITMAPINFOHEADER = ::BITMAPINFOHEADER;

	using LPVOID = ::LPVOID;
	using LPCSTR = ::LPCSTR;
	using LPCWSTR = ::LPCWSTR;

	using HRESULT = ::HRESULT;
	using HWND = ::HWND;
	using HANDLE = ::HANDLE;
	using GUID = ::GUID;
	using IID = ::IID;
	using LPSECURITY_ATTRIBUTES = ::LPSECURITY_ATTRIBUTES;

	using IUnknown = ::IUnknown;

	using IMFMediaEvent = ::IMFMediaEvent;
	using IMFStreamDescriptor = ::IMFStreamDescriptor;
	using IMFActivate = ::IMFActivate;
	using IMFMediaTypeHandler = ::IMFMediaTypeHandler;
	using IMFTopology = ::IMFTopology;
	using IMFMediaSource = ::IMFMediaSource;
	using IMFPresentationDescriptor = ::IMFPresentationDescriptor;
	using IMFStreamDescriptor = ::IMFStreamDescriptor;
	using IMFTopologyNode = ::IMFTopologyNode;
	using IMFSourceResolver = ::IMFSourceResolver;
	using IMFAsyncCallback = ::IMFAsyncCallback;
	using IMFMediaSession = ::IMFMediaSession;
	using IMFVideoDisplayControl = ::IMFVideoDisplayControl;
	using IMFAsyncResult = ::IMFAsyncResult;
	using MediaEventType = ::MediaEventType;
	using MFTIME = ::MFTIME;
	using IMFClock = ::IMFClock;
	using IMFPresentationClock = ::IMFPresentationClock;

	using QITAB = ::QITAB;
	using LPCQITAB = ::LPCQITAB;
	using PROPVARIANT = ::PROPVARIANT;

	//以下はコンパイラによって異なる型名を持つため、変数として定義する。
	inline constexpr DWORD MF_RESOLUTION_MEDIASOURCE = ::MF_RESOLUTION_MEDIASOURCE;
	inline constexpr DWORD MESessionClosed = ::MESessionClosed;
	inline constexpr DWORD MEUnknown = ::MEUnknown;
	inline constexpr DWORD MESessionTopologyStatus = ::MESessionTopologyStatus;
	inline constexpr DWORD MEEndOfPresentation = ::MEEndOfPresentation;
	inline constexpr DWORD MENewPresentation = ::MENewPresentation;

//g++ではグローバルでexportできない、かつ使用側でもenum名による修飾が必須
#if defined(__GNUC__) && !defined(__clang__)
	using VARENUM = ::VARENUM;
	using MF_TOPOLOGY_TYPE = ::MF_TOPOLOGY_TYPE;
	using MF_OBJECT_TYPE = ::MF_OBJECT_TYPE;
	using MF_TOPOSTATUS = ::MF_TOPOSTATUS;
	using MFSESSION_SETTOPOLOGY_FLAGS = ::MFSESSION_SETTOPOLOGY_FLAGS;
	using tagCOINIT = ::tagCOINIT;
	using COINIT = ::COINIT;
#endif
}

#if !defined(__GNUC__) || defined(__clang__)
export {
	using VARENUM = ::VARENUM;
	using MF_TOPOLOGY_TYPE = ::MF_TOPOLOGY_TYPE;
	using MF_OBJECT_TYPE = ::MF_OBJECT_TYPE;
	using MF_TOPOSTATUS = ::MF_TOPOSTATUS;
	using MFSESSION_SETTOPOLOGY_FLAGS = ::MFSESSION_SETTOPOLOGY_FLAGS;
	using tagCOINIT = ::tagCOINIT;
	using COINIT = ::COINIT;
}
#endif

//g++ではGUID型間の比較演算子が曖昧になるため、明示的に定義。
//他のコンパイラでは不要だが、エラーはでない。
export constexpr bool operator==(MF::GUID lhs, MF::GUID rhs) noexcept {
	return lhs.Data1 == rhs.Data1 && lhs.Data2 == rhs.Data2 && lhs.Data3 == rhs.Data3
		&& lhs.Data4[0] == rhs.Data4[0] && lhs.Data4[1] == rhs.Data4[1] && lhs.Data4[2] == rhs.Data4[2] && lhs.Data4[3] == rhs.Data4[3];
}

//マクロ
namespace MF {
	namespace Internal {
		inline constexpr HRESULT S_OK_Internal = S_OK;
		inline constexpr HRESULT MF_E_INVALIDTYPE_Internal = MF_E_INVALIDTYPE;
		inline constexpr HRESULT E_FAIL_Internal = E_FAIL;
		inline constexpr HRESULT E_POINTER_Internal = E_POINTER;
		inline constexpr HRESULT E_NOTIMPL_Internal = E_NOTIMPL;
		inline constexpr BOOL TRUE_Internal = TRUE;
		inline constexpr BOOL FALSE_Internal = FALSE;
		inline constexpr ULONG MF_SDK_VERSION_Internal = MF_SDK_VERSION;
		inline constexpr ULONG MF_API_VERSION_Internal = MF_API_VERSION;
		inline constexpr ULONG MF_VERSION_Internal = MF_VERSION;
		inline constexpr DWORD INFINITE_Internal = INFINITE;
#undef S_OK
#undef MF_E_INVALIDTYPE
#undef E_FAIL
#undef E_POINTER
#undef TRUE
#undef FALSE
#undef MF_SDK_VERSION
#undef MF_API_VERSION
#undef MF_VERSION
#undef INFINITE
#undef E_NOTIMPL
	}
}

export namespace MF {
	inline constexpr HRESULT S_OK = Internal::S_OK_Internal;
	inline constexpr HRESULT MF_E_INVALIDTYPE = Internal::MF_E_INVALIDTYPE_Internal;
	inline constexpr HRESULT E_FAIL = Internal::E_FAIL_Internal;
	inline constexpr HRESULT E_POINTER = Internal::E_POINTER_Internal;
	inline constexpr HRESULT E_NOTIMPL = Internal::E_NOTIMPL_Internal;	
	inline constexpr BOOL TRUE = Internal::TRUE_Internal;
	inline constexpr BOOL FALSE = Internal::FALSE_Internal;
	inline constexpr ULONG MF_SDK_VERSION = Internal::MF_VERSION_Internal;
	inline constexpr ULONG MF_API_VERSION = Internal::MF_API_VERSION_Internal;
	inline constexpr ULONG MF_VERSION = Internal::MF_VERSION_Internal;
	inline constexpr DWORD INFINITE = Internal::INFINITE_Internal;

	inline const GUID MFMediaType_Audio = ::MFMediaType_Audio;
	inline const GUID MFMediaType_Video = ::MFMediaType_Video;
	inline const GUID MF_EVENT_TOPOLOGY_STATUS = ::MF_EVENT_TOPOLOGY_STATUS;
	inline const GUID MR_VIDEO_RENDER_SERVICE = ::MR_VIDEO_RENDER_SERVICE;
	inline const IID MF_TOPONODE_SOURCE = ::MF_TOPONODE_SOURCE;
	inline const IID MF_TOPONODE_PRESENTATION_DESCRIPTOR = ::MF_TOPONODE_PRESENTATION_DESCRIPTOR;
	inline const IID MF_TOPONODE_STREAM_DESCRIPTOR = ::MF_TOPONODE_STREAM_DESCRIPTOR;
	inline const IID MF_TOPONODE_STREAMID = ::MF_TOPONODE_STREAMID;
	inline const IID MF_TOPONODE_NOSHUTDOWN_ON_REMOVE = ::MF_TOPONODE_NOSHUTDOWN_ON_REMOVE;
	inline const IID MF_PD_DURATION = ::MF_PD_DURATION;
	inline const IID GUID_NULL = ::GUID_NULL;
}

export namespace MF {
	template<class Derived, class Base>
	constexpr QITAB GetQITAB() noexcept {
		return QITAB{
			&__uuidof(Base),
			static_cast<decltype(QITAB::dwOffset)>(((DWORD)((DWORD_PTR)static_cast<Base*>((Derived*)8)))-8)
		};
	}
	consteval QITAB GetQITABSentinel() noexcept {
		return QITAB{ nullptr, 0 };
	}

	HRESULT QISearch(void *that, LPCQITAB pqit, const IID &riid, void **ppv) noexcept {
		return ::QISearch(that, pqit, riid, ppv);
	}

	template <class T>
	void SafeRelease(T*& object) noexcept {
		if (object) {
			object->Release();
			object = nullptr;
		}
	}
	template<class T, class ...Args>
	HRESULT SafeRelease(HRESULT hr, T*& object, Args*& ...args) noexcept {
		SafeRelease(object);
		if constexpr (sizeof...(Args) == 0) return hr;
		else return SafeRelease(hr, args...);
	}
	
	inline void PropVariantInit(PROPVARIANT *pvar) noexcept {
		::PropVariantInit(pvar);
	}
	HRESULT PropVariantClear(PROPVARIANT* var) noexcept {
		return ::PropVariantClear(var);
	}
	HRESULT MFCreateAudioRendererActivate(IMFActivate **ppActivate) noexcept {
		return ::MFCreateAudioRendererActivate(ppActivate);
	}
	HRESULT MFCreateVideoRendererActivate(HWND hwndVideo, IMFActivate **ppActivate) noexcept {
		return ::MFCreateVideoRendererActivate(hwndVideo, ppActivate);
	}
	HRESULT MFCreateTopologyNode(MF_TOPOLOGY_TYPE NodeType, IMFTopologyNode **ppNode) noexcept {
		return ::MFCreateTopologyNode(NodeType, ppNode);
	}
	HRESULT MFCreateTopology(IMFTopology **ppTopo) noexcept {
		return ::MFCreateTopology(ppTopo);
	}
	HRESULT MFCreateSourceResolver(IMFSourceResolver **ppISourceResolver) noexcept {
		return ::MFCreateSourceResolver(ppISourceResolver);
	}
	HRESULT MFCreateMediaSession(IMFAttributes *pConfiguration, IMFMediaSession **ppMediaSession) noexcept {
		return ::MFCreateMediaSession(pConfiguration, ppMediaSession);
	}

	HRESULT MFGetService(
		IUnknown *punkObject, const GUID &guidService, const IID &riid, LPVOID *ppvObject
	) noexcept {
		return ::MFGetService(punkObject, guidService, riid, ppvObject);
	}

	HRESULT MFStartup(ULONG Version, DWORD dwFlags = 0UL) noexcept {
		return ::MFStartup(Version, dwFlags);
	}
	HRESULT MFShutdown() noexcept {
		return ::MFShutdown();
	}

	HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit) noexcept {
		return ::CoInitializeEx(pvReserved, dwCoInit);
	}
	void CoUninitialize() noexcept {
		return ::CoUninitialize();
	}

	void CoTaskMemFree(LPVOID pv) noexcept {
		::CoTaskMemFree(pv);
	}

#undef InterlockedIncrement
	LONG InterlockedIncrement(volatile LONG *Addend) noexcept {
		return ::_InterlockedIncrement(Addend);
	}
	unsigned long long InterlockedIncrement(volatile unsigned long long *Addend) noexcept {
		return ::_InterlockedIncrement(Addend);
	}
	unsigned long InterlockedIncrement(volatile unsigned long *Addend) noexcept {
		return ::_InterlockedIncrement(Addend);
	}
	unsigned int InterlockedIncrement(volatile unsigned int *Addend) noexcept {
		return ::_InterlockedIncrement(Addend);
	}
#undef InterlockedDecrement
	LONG InterlockedDecrement(volatile LONG *Addend) noexcept {
		return ::_InterlockedDecrement(Addend);
	}
	unsigned long long InterlockedDecrement(volatile unsigned long long *Addend) noexcept {
		return ::_InterlockedDecrement(Addend);
	}
	unsigned long InterlockedDecrement(volatile unsigned long *Addend) noexcept {
		return ::_InterlockedDecrement(Addend);
	}
	unsigned int InterlockedDecrement(volatile unsigned int *Addend) noexcept {
		return ::_InterlockedDecrement(Addend);
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
