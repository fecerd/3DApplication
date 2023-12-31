﻿export module MediaPlayerForMediaFoundation;
import MFHeader;
import System;
import Application;	//Log
import IMediaPlayer;
using namespace System::Drawing;
using namespace MF;

#undef IID_PPV_ARGS
#define IID_PPV_ARGS(ppType) MF::GetIID(ppType), MF::IID_PPV_ARGS_Helper(ppType)

//MediaFoundation Utilities
namespace System::Application::Windows::Internal {
	class MediaFoundation {
		MediaFoundation() noexcept = delete;
		~MediaFoundation() noexcept = delete;
	public:
		/// <summary>
		/// メディアイベントからオブジェクトを取得する
		/// </summary>
		template<class Q>
		static HRESULT GetEventObject(IMFMediaEvent* mediaEvent, Q*& object) noexcept {
			object = nullptr;
			PROPVARIANT var;
			HRESULT hr = mediaEvent->GetValue(&var);
			if (hr >= 0) {
				if (var.vt == VARENUM::VT_UNKNOWN) hr = var.punkVal->QueryInterface(&object);
				else hr = MF_E_INVALIDTYPE;
				PropVariantClear(&var);
			}
			return hr;
		}
		/// <summary>
		/// メディアシンク(再生用レンダラ)を作成する
		/// </summary>
		/// <param name="streamDesc">ストリーム情報</param>
		/// <param name="hWnd">出力先ウィンドウハンドル</param>
		/// <param name="out">作成したメディアシンクを受け取る参照変数</param>
		static HRESULT CreateMediaSinkActivate(IMFStreamDescriptor& streamDesc, HWND hWnd, IMFActivate*& out) noexcept {
			IMFMediaTypeHandler* handler = nullptr;
			HRESULT hr = streamDesc.GetMediaTypeHandler(&handler);
			if (hr < 0) return SafeRelease(hr, handler);
			GUID guidMajorType;
			hr = handler->GetMajorType(&guidMajorType);
			SafeRelease(handler);
			if (hr < 0) return hr;
			IMFActivate* activate = nullptr;
			if (guidMajorType == MFMediaType_Audio) hr = MFCreateAudioRendererActivate(&activate);
			else if (guidMajorType == MFMediaType_Video) hr = MFCreateVideoRendererActivate(hWnd, &activate);
			else hr = E_FAIL;
			if (hr < 0) return SafeRelease(hr, activate);
			out = activate;
			return hr;
		}
		/// <summary>
		/// 入力ノードをトポロジに追加する
		/// </summary>
		/// <param name="topology">IMFTopology</param>
		/// <param name="mediaSource">メディアソース</param>
		/// <param name="sourcePD">メディアソースのプレゼンテーション情報</param>
		/// <param name="streamDesc">ストリーム情報</param>
		/// <param name="out">トポロジに追加された入力ノードを受け取る参照変数。(変換ノードの補完に使用する)</param>
		static HRESULT AddSourceNode(IMFTopology& topology, IMFMediaSource& mediaSource, IMFPresentationDescriptor& sourcePD, IMFStreamDescriptor& streamDesc, IMFTopologyNode*& out) noexcept {
			IMFTopologyNode* node = nullptr;
			HRESULT hr = MFCreateTopologyNode(
				MF_TOPOLOGY_TYPE::MF_TOPOLOGY_SOURCESTREAM_NODE, &node
			);
			if (hr < 0) return SafeRelease(hr, node);
			node->SetUnknown(MF_TOPONODE_SOURCE, &mediaSource);
			node->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, &sourcePD);
			node->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, &streamDesc);
			hr = topology.AddNode(node);
			if (hr < 0) return SafeRelease(hr, node);
			out = node;
			return hr;
		}
		/// <summary>
		/// 出力ノードをトポロジに追加する
		/// </summary>
		/// <param name="topology">IMFTopology</param>
		/// <param name="activate">レンダラ</param>
		/// <param name="streamSinkID">使用するストリームID</param>
		/// <param name="out">トポロジに追加された出力ノードを受け取る参照変数。(変換ノードの補完に使用する)</param>
		static HRESULT AddOutputNode(IMFTopology& topology, IMFActivate& activate, DWORD streamSinkID, IMFTopologyNode*& out) noexcept {
			IMFTopologyNode* node = nullptr;
			HRESULT hr = MFCreateTopologyNode(
				MF_TOPOLOGY_TYPE::MF_TOPOLOGY_OUTPUT_NODE, &node
			);
			if (hr < 0) return SafeRelease(hr, node);
			node->SetObject(&activate);
			node->SetUINT32(MF_TOPONODE_STREAMID, streamSinkID);
			node->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
			hr = topology.AddNode(node);
			if (hr < 0) return SafeRelease(hr, node);
			out = node;
			return hr;
		}
		/// <summary>
		/// IMFTopologyに入力ノード(メディアソースの指定したストリーム)・出力ノード(EVRまたはSAR)を追加し、部分トポロジを構築する。
		/// その後、変換ノードが自動で補完され、再生用の完全トポロジを返す
		/// </summary>
		/// <param name="topology">IMFTopology</param>
		/// <param name="mediaSource">メディアソース</param>
		/// <param name="sourcePD">メディアソースのプレゼンテーション情報</param>
		/// <param name="streamIndex">メディアに含まれる対象のストリーム番号</param>
		/// <param name="hWnd">出力先のウィンドウハンドル</param>
		static HRESULT AddBranchToPartialTopology(
			IMFTopology& topology, IMFMediaSource& mediaSource,
			IMFPresentationDescriptor& sourcePD, DWORD streamIndex, HWND hWnd
		) noexcept {
			IMFStreamDescriptor* streamDesc = nullptr;
			BOOL selected = FALSE;
			HRESULT hr = sourcePD.GetStreamDescriptorByIndex(streamIndex, &selected, &streamDesc);
			if (hr < 0 || !selected) return SafeRelease(hr, streamDesc);
			IMFActivate* sinkActivate = nullptr;
			hr = CreateMediaSinkActivate(*streamDesc, hWnd, sinkActivate);
			if (hr < 0) return SafeRelease(hr, sinkActivate, streamDesc);
			IMFTopologyNode* sourceNode = nullptr;
			hr = AddSourceNode(topology, mediaSource, sourcePD, *streamDesc, sourceNode);
			if (hr < 0) return SafeRelease(hr, sourceNode, sinkActivate, streamDesc);
			IMFTopologyNode* outputNode = nullptr;
			hr = AddOutputNode(topology, *sinkActivate, 0, outputNode);
			if (hr >= 0) sourceNode->ConnectOutput(0, outputNode, 0);
			return SafeRelease(hr, outputNode, sourceNode, sinkActivate, streamDesc);
		}
		/// <summary>
		/// メディアソースから再生用トポロジを作成する
		/// </summary>
		/// <param name="mediaSource">メディアソース</param>
		/// <param name="sourcePD">メディアソースのプレゼンテーション情報</param>
		/// <param name="hWnd">出力先のウィンドウハンドル。出力ノード作成に使用</param>
		/// <param name="out">作成したIMFTopologyを受け取る参照変数</param>
		static HRESULT CreatePlaybackTopology(
			IMFMediaSource& mediaSource, IMFPresentationDescriptor& sourcePD, HWND hWnd, IMFTopology*& out
		) noexcept {
			IMFTopology* topology = nullptr;
			HRESULT hr = MFCreateTopology(&topology);
			if (hr < 0) return SafeRelease(hr, topology);
			DWORD streamDescCount = 0;
			hr = sourcePD.GetStreamDescriptorCount(&streamDescCount);
			if (hr < 0) return SafeRelease(hr, topology);
			for (DWORD i = 0; i < streamDescCount; ++i) {
				hr = AddBranchToPartialTopology(*topology, mediaSource, sourcePD, i, hWnd);
				if (hr < 0) return SafeRelease(hr, topology);
			}
			out = topology;
			return hr;
		}
		static HRESULT CreateMediaSource(const String& filePath, IMFMediaSource*& out) noexcept {
			IMFSourceResolver* sourceResolver = nullptr;
			HRESULT hr = MFCreateSourceResolver(&sourceResolver);
			if (hr < 0) return SafeRelease(hr, sourceResolver);
			MF_OBJECT_TYPE objectType = MF_OBJECT_TYPE::MF_OBJECT_INVALID;
			IUnknown* source = nullptr;
			//非同期の場合、BeginCreateObjectFromURLを使用する
			hr = sourceResolver->CreateObjectFromURL(
				filePath.w_str(),
				MF::MF_RESOLUTION_MEDIASOURCE,
				nullptr,
				&objectType,
				&source
			);
			if (hr < 0) return SafeRelease(hr, source, sourceResolver);
			hr = source->QueryInterface(IID_PPV_ARGS(&out));
			return SafeRelease(hr, source, sourceResolver);
		}
	};
}
//Internal Structures
namespace System::Application::Windows::Internal {
	//IMFMediaEventを保持するキュー
	class MediaEventQueue {
		class MediaEventFragment {
			friend class MediaEventQueue;
			IMFMediaEvent* mediaEvent = nullptr;
			MediaEventFragment* next = nullptr;
		public:
			MediaEventFragment() noexcept = delete;
			MediaEventFragment(const MediaEventFragment&) = delete;
			MediaEventFragment(IMFMediaEvent*& newMediaEvent) noexcept : mediaEvent(newMediaEvent), next(nullptr) {
				newMediaEvent = nullptr;
			}
			~MediaEventFragment() {
				SafeRelease(mediaEvent);
				next = nullptr;
			}
		} *begin = nullptr;
		Mutex mtx = Mutex();	//非同期用ミューテックス
	public:
		MediaEventQueue() noexcept = default;
		MediaEventQueue(const MediaEventQueue&) noexcept = delete;
		MediaEventQueue(MediaEventQueue&& rref) noexcept : begin(rref.begin), mtx(Mutex()) {
			LockGuard lock(rref.mtx);
			rref.begin = nullptr;
		}
		~MediaEventQueue() {
			LockGuard lock(mtx);
			while (begin) {
				MediaEventFragment* next = begin->next;
				delete begin;
				begin = next;
			}
		}
	public:
		IMFMediaEvent* Pop() noexcept {
			LockGuard lock(mtx);
			IMFMediaEvent* ret = nullptr;
			if (begin) {
				ret = begin->mediaEvent;
				begin->mediaEvent = nullptr;
				MediaEventFragment* next = begin->next;
				delete begin;
				begin = next;
			}
			return ret;
		}
		void Push(IMFMediaEvent*& mediaEvent) noexcept {
			LockGuard lock(mtx);
			MediaEventFragment* newfrag = new MediaEventFragment(mediaEvent);
			if (!begin) begin = newfrag;
			else {
				MediaEventFragment* tmp = begin;
				while (tmp->next) tmp = tmp->next;
				tmp->next = newfrag;
			}
		}
	};
	//メディアソース情報
	struct MFMediaSource {
		IMFMediaSource* source = nullptr;
		int64_t duration = 0;
	public:
		MFMediaSource() noexcept = default;
		MFMediaSource(const MFMediaSource&) noexcept = delete;
		MFMediaSource(MFMediaSource&& arg) noexcept
			: source(arg.source), duration(arg.duration) {
			arg.source = nullptr;
			arg.duration = 0;
		}
		~MFMediaSource() noexcept {
			Shutdown();
		}
	public:
		bool Load(const String& filePath) noexcept {
			MFStartup(MF_VERSION);
			HRESULT hr = S_OK;
			IMFPresentationDescriptor* sourcePD = nullptr;
			do {
				hr = MediaFoundation::CreateMediaSource(filePath, source);
				if (hr < 0) break;
				hr = source->CreatePresentationDescriptor(&sourcePD);
				if (hr < 0) break;
				hr = sourcePD->GetUINT64(MF_PD_DURATION, reinterpret_cast<UINT64*>(&duration));
				if (hr < 0) break;
			} while (false);
			SafeRelease(sourcePD);
			if (hr < 0) {
				duration = 0;
				SafeRelease(source);
				MFShutdown();
			}
			return hr >= 0;
		}
		void Shutdown() noexcept {
			if (source) {
				source->Shutdown();
				MFShutdown();
			}
			source = nullptr;
			duration = 0;
		}
	public:
		MFMediaSource& operator=(const MFMediaSource&) noexcept = delete;
		MFMediaSource& operator=(MFMediaSource&& rhs) noexcept {
			if (this == &rhs) return *this;
			Shutdown();
			source = rhs.source;
			rhs.source = nullptr;
			duration = rhs.duration;
			rhs.duration = 0;
			return *this;
		}
	};
	//メディアプレイヤーの状態
	enum class MFPlayerState : uint8_t {
		Uninitialized,
		Ready,
		OpenPending,
		Started,
		Paused,
		Stopped,
		Closing,
		Closed
	};
}
using namespace System::Application::Windows::Internal;

//MFMediaPlayer
export namespace System::Application::Windows {
	class MFMediaPlayer : public IMFAsyncCallback, public IMediaPlayer {
		LONG m_refCount = 0;
		IMFMediaSession* m_session = nullptr;
		IMFVideoDisplayControl* m_videoDisplayControl = nullptr;
		HWND m_hVideoWindow = nullptr;
		Atomic<MFPlayerState> m_state = MFPlayerState::Uninitialized;
		bool m_loop = false;
		uint32_t m_loopCount = 0;
		MediaEventQueue m_mediaEventQueue;
		Atomic<HANDLE> m_closeEvent = static_cast<HANDLE>(nullptr);
		HashMap<String, MFMediaSource> m_sources;
		String m_currentSourceName;
		MediaPlayerSourceType m_currentSourceType = MediaPlayerSourceType::Local;
	private:
		inline static Mutex s_mtx;
		inline static IMFSimpleAudioVolume* s_simpleAudioVolume = nullptr;
		inline static int32_t s_volume = 100;
		inline static bool s_muted = false;
		IMFAudioStreamVolume* m_audioStreamVolume = nullptr;
		int32_t m_volume = 100;
		bool m_muted = false;
	private:
		static HashMap<String, MFMediaSource>& GlobalSources() noexcept {
			static HashMap<String, MFMediaSource> ret;
			return ret;
		}
	public:
		MFMediaPlayer() noexcept {
			static bool first = true;
			if (first) {
				System::Application::Log(
					Exception(
						u"MediaFoundationは内部で例外(bad_hresult)を投げることがありますが、これはリバースエンジニアリングを困難にするためにEVRが投げるものであり、正常です。\n"
					).what()
				);
				first = false;
			}
			//MediaFoundationの初期化
			CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
			MFStartup(MF_VERSION);
			HRESULT hr = MFCreateMediaSession(nullptr, &m_session);
			if (hr < 0) return;
			hr = m_session->BeginGetEvent(this, nullptr);
			m_state.store(MFPlayerState::Ready, memory_order::release);
		}
		~MFMediaPlayer() noexcept;
	private:/* IUnknown関連 */
		HRESULT __stdcall QueryInterface(const IID& iid, void** object) override {
			if (!object) return E_POINTER;
			static const QITAB qit[] = {
				GetQITAB<MFMediaPlayer, IMFAsyncCallback>(),
				GetQITABSentinel()
			};
			return QISearch(this, qit, iid, object);
		}
		ULONG __stdcall AddRef() override {
			return InterlockedIncrement(&m_refCount);
		}
		ULONG __stdcall Release() override {
			long cRef = InterlockedDecrement(&m_refCount);
			return cRef;
		}
	private:/* IMFAsyncCallback関連 */
		HRESULT __stdcall GetParameters(DWORD*, DWORD*) override {
			return E_NOTIMPL;
		}
		//Atomic<IMFAsyncResult*> m_asyncResult = nullptr;
		HRESULT __stdcall Invoke(IMFAsyncResult* asyncResult) override {
			IMFMediaEvent* mediaEvent = nullptr;
			//処理中はイベント取得を停止
			HRESULT hr = m_session->EndGetEvent(asyncResult, &mediaEvent);
			if (hr < 0) return SafeRelease(hr, mediaEvent);
			MediaEventType eventType;
			hr = mediaEvent->GetType(&eventType);
			if (hr < 0) return SafeRelease(hr, mediaEvent);
			if (eventType != MF::MESessionClosed) {
				//終了処理以外はイベントキューに追加する
				m_mediaEventQueue.Push(mediaEvent);
				SafeRelease(mediaEvent);
				//イベント取得を再開
				hr = m_session->BeginGetEvent(this, nullptr);
			}
			else {
				//CAsyncResult::Release()が例外を出してmfplat.dllを異常終了させることがある(デバッグ10回に一度程度)。
				//asyncResultの参照を増やしておけば、(メモリリークはするが)この現象が起きなくなる。
				//DLLの内部スレッドの終了より前にasyncResultオブジェクトが破棄されると起こる？
				asyncResult->AddRef();
				Update();
				SafeRelease(mediaEvent);
				m_state.store(MFPlayerState::Closed, memory_order::release);
				//デストラクタで待機中のm_closeEventを送る
				SetEvent(m_closeEvent.load(memory_order::acquire));
			}
			return hr;
		}
	private:/* Update関数から呼び出されるイベント */
		/// <summary>
		/// トポロジの状態が変化したときに呼び出されるイベント
		/// </summary>
		/// <param name="mediaEvent">イベント情報</param>
		HRESULT OnTopologyStatus(IMFMediaEvent* mediaEvent) noexcept {
			UINT32 status;
			HRESULT hr = mediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status);
			if (hr < 0) return hr;
			//トポロジの状態がReadyになったとき、使用するVideoControlを取得しなおして再生を開始する
			if (status == MF_TOPOSTATUS::MF_TOPOSTATUS_READY) {
				SafeRelease(m_videoDisplayControl, m_audioStreamVolume);
				HRESULT hr2 = MFGetService(m_session, MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&m_videoDisplayControl));
				if (hr2 < 0) SafeRelease(m_videoDisplayControl);
				hr2 = MFGetService(m_session, MR_STREAM_VOLUME_SERVICE, IID_PPV_ARGS(&m_audioStreamVolume));
				if (hr2 < 0) SafeRelease(m_audioStreamVolume);
				else {
					UINT32 count = 0;
					hr2 = m_audioStreamVolume->GetChannelCount(&count);
					for (UINT32 i = 0; i < count; ++i) {
						const float volume = m_muted ? 0.f : m_volume / 100.f;
						hr2 = m_audioStreamVolume->SetChannelVolume(i, volume);
					}
				}
				{
					LockGuard lock{ s_mtx };
					if (!s_simpleAudioVolume) {
						hr2 = MFGetService(m_session, MR_POLICY_VOLUME_SERVICE, IID_PPV_ARGS(&s_simpleAudioVolume));
						if (hr2 < 0) SafeRelease(s_simpleAudioVolume);
					}
					if (s_simpleAudioVolume) {
						const float volume = s_muted ? 0.f : s_volume / 100.f;
						hr2 = s_simpleAudioVolume->SetMasterVolume(volume);
					}
				}
			}
			return hr;
		}
		/// <summary>
		/// プレゼンテーション(再生)が終了したときに呼び出されるイベント
		/// </summary>
		/// <param name="mediaEvent">イベント情報</param>
		HRESULT OnPresentationEnded(IMFMediaEvent* mediaEvent) noexcept {
			if (m_loop) {
				Stop();	//ループする前に停止しないと6回目あたりでフリーズする。
				return StartPlayBack(MediaPlayerSeekPos::Begin, nanoseconds(0));
			}
			m_state.store(MFPlayerState::Stopped, memory_order::release);
			return S_OK;
		}
		/// <summary>
		/// 新たなプレゼンテーションの準備が整ったときに呼び出されるイベント
		/// </summary>
		/// <param name="mediaEvent">イベント情報</param>
		HRESULT OnNewPresentation(IMFMediaEvent* mediaEvent) noexcept {
			return S_OK;
		}
		/// <summary>
		/// イベント拡張用
		/// </summary>
		virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType) noexcept { return S_OK; }
	private:
		/// <summary>
		/// MediaFoundationで使用される100ナノ秒単位の数値に変換する
		/// </summary>
		/// <param name="s">T型の秒数</param>
		template<System::Seconds S>
		int64_t ToMFDuration(const S& s) noexcept {
			nanoseconds nano = nanoseconds(s);
			return static_cast<int64_t>(nano.count()) / 100;
		}
		/// <summary>
		/// メディアセッションの再生を開始する
		/// </summary>
		/// <param name="ns">開始位置(ns)</param>
		template<System::Seconds S>
		HRESULT StartPlayBack(MediaPlayerSeekPos pos = MediaPlayerSeekPos::Current, S offset = S(0)) noexcept {
			PROPVARIANT varStart;
			PropVariantInit(&varStart);
			const int64_t duration = ToMFDuration(offset);
			if (pos == MediaPlayerSeekPos::Begin) {
				varStart.vt = VARENUM::VT_I8;
				varStart.hVal.QuadPart = duration;
			}
			else if (pos == MediaPlayerSeekPos::Current) {
				if (duration == 0) {
					varStart.vt = VARENUM::VT_EMPTY;
				}
				else {
					MFTIME time = GetCurrentMFTime();
					varStart.vt = VARENUM::VT_I8;
					varStart.hVal.QuadPart = time + duration;
				}
			}
			else if (pos == MediaPlayerSeekPos::End) {
				MFMediaSource* source = nullptr;
				if (m_currentSourceType == MediaPlayerSourceType::Local) source = m_sources.AtPtr(m_currentSourceName);
				else if (m_currentSourceType == MediaPlayerSourceType::Global) source = GlobalSources().AtPtr(m_currentSourceName);
				if (!source) return E_FAIL;
				varStart.vt = VARENUM::VT_I8;
				varStart.hVal.QuadPart = source->duration + duration;
			}
			HRESULT hr = m_session->Start(&GUID_NULL, &varStart);
			if (hr >= 0) m_state.store(MFPlayerState::Started, memory_order::release);
			PropVariantClear(&varStart);
			return hr;
		}
		/// <summary>
		/// 指定したメディアソースをセッションに設定する
		/// </summary>
		/// <param name="name">メディアソース名</param>
		/// <param name="type">メディアソース名の検索範囲</param>
		bool SetSource(const String& name, MediaPlayerSourceType type) noexcept {
			MFMediaSource* source = nullptr;
			if (type == MediaPlayerSourceType::Local) source = m_sources.AtPtr(name);
			else if (type == MediaPlayerSourceType::Global) source = GlobalSources().AtPtr(name);
			if (!source) return false;
			IMFPresentationDescriptor* sourcePD = nullptr;
			HRESULT hr = source->source->CreatePresentationDescriptor(&sourcePD);
			if (hr < 0) return false;
			IMFTopology* topology = nullptr;
			hr = MediaFoundation::CreatePlaybackTopology(
				*source->source,
				*sourcePD,
				m_hVideoWindow,
				topology
			);
			SafeRelease(sourcePD);
			if (hr < 0) return SafeRelease(hr, topology) >= 0;
			hr = m_session->SetTopology(
				MFSESSION_SETTOPOLOGY_FLAGS::MFSESSION_SETTOPOLOGY_IMMEDIATE, topology
			);
			SafeRelease(topology);
			if (hr < 0) return false;
			m_currentSourceName = name;
			m_currentSourceType = type;
			m_state.store(MFPlayerState::OpenPending, memory_order::release);
			return true;
		}

		MFTIME GetCurrentMFTime() noexcept {
			MFTIME ret = MFTIME(0);
			if (!m_session) return ret;
			IMFClock* clock = nullptr;
			HRESULT hr = m_session->GetClock(&clock);
			if (hr >= 0) static_cast<IMFPresentationClock*>(clock)->GetTime(&ret);
			SafeRelease(clock);
			return ret;
		}
	public:
		/// <summary>
		/// WM_PAINT(ウィンドウの再描画命令)を受信したときに呼び出す。
		/// 3Dデバイスを使用する場合、GetCurrentFrame関数でテクスチャを取得できるため不要
		/// </summary>
		HRESULT Repaint() noexcept {
			return m_videoDisplayControl ? m_videoDisplayControl->RepaintVideo() : S_OK;
		}
		/// <summary>
		/// WM_SIZED(ウィンドウのサイズ変更)を受信したときに呼び出す。
		/// 描画領域をウィンドウ左上から右下(width, height)に設定する。
		/// 3Dデバイスを使用する場合、GetCurrentFrame関数でテクスチャを取得できるため不要
		/// </summary>
		/// <param name="width">幅(px)</param>
		/// <param name="height">高さ(px)</param>
		HRESULT ResizeVideo(WORD width, WORD height) noexcept {
			if (m_videoDisplayControl) {
				RECT rc = { 0, 0, width, height };
				return m_videoDisplayControl->SetVideoPosition(nullptr, &rc);
			}
			else return S_OK;
		}
	public:
		bool Load(const String& name, MediaPlayerSourceType type, const String& filePath, bool setCurrent = false) noexcept override {
			if (name.IsNullOrEmpty()) return false;
			MFMediaSource source;
			if (!source.Load(filePath)) return false;
			switch (type) {
			case MediaPlayerSourceType::Local:
				if (!m_sources.Insert(name, static_cast<MFMediaSource&&>(source))) return false;
				break;
			case MediaPlayerSourceType::Global:
				if (!GlobalSources().Insert(name, static_cast<MFMediaSource&&>(source))) return false;
				break;
			default:
				return false;
			}
			if (setCurrent) return SetSource(name, type);
			else return true;
		}
		bool Play(MediaPlayerSeekPos pos, int64_t nsOffset) noexcept override {
			return StartPlayBack(pos, nanoseconds(nsOffset)) >= 0;
		}
		bool Play(const String& name, MediaPlayerSourceType type, MediaPlayerSeekPos pos, int64_t nsOffset) noexcept override {
			if (name != m_currentSourceName) {
				Stop();
				if (SetSource(name, type)) return StartPlayBack(pos, nanoseconds(nsOffset)) >= 0;
				else return false;
			}
			else return StartPlayBack(pos, nanoseconds(nsOffset)) >= 0;
		}
		bool Pause() noexcept override {
			if (m_state.load(memory_order::acquire) != MFPlayerState::Started) return false;
			HRESULT hr = m_session->Pause();
			if (hr >= 0) m_state.store(MFPlayerState::Paused, memory_order::release);
			return hr >= 0;
		}
		bool Stop() noexcept override {
			MFPlayerState current = m_state.load(memory_order::acquire);
			if (current != MFPlayerState::Started && current != MFPlayerState::Paused) return false;
			HRESULT hr = m_session->Stop();
			if (hr >= 0) m_state.store(MFPlayerState::Stopped, memory_order::release);
			return hr >= 0;
		}
		bool IsPlaying() noexcept override { return m_state.load(memory_order::acquire) == MFPlayerState::Started; }
		bool IsPaused() noexcept override { return m_state.load(memory_order::acquire) == MFPlayerState::Paused; }
		bool IsEnded() noexcept override { return m_state.load(memory_order::acquire) == MFPlayerState::Stopped; }
		void Update() noexcept override {
			IMFMediaEvent* mediaEvent = nullptr;
			while ((mediaEvent = m_mediaEventQueue.Pop()) != nullptr) {
				MediaEventType eventType = MF::MEUnknown;
				HRESULT hr = mediaEvent->GetType(&eventType);
				if (hr < 0) {
					SafeRelease(mediaEvent);
					continue;
				}
				HRESULT status = S_OK;
				hr = mediaEvent->GetStatus(&status);
				if (hr >= 0 && status < 0) hr = status;
				if (hr < 0) {
					SafeRelease(mediaEvent);
					continue;
				}
				switch (eventType) {
				case MF::MESessionTopologyStatus:
					hr = OnTopologyStatus(mediaEvent);
					break;
				case MF::MEEndOfPresentation:
					hr = OnPresentationEnded(mediaEvent);
					break;
				case MF::MENewPresentation:
					hr = OnNewPresentation(mediaEvent);
					break;
				default:
					hr = OnSessionEvent(mediaEvent, eventType);
					break;
				}
				SafeRelease(mediaEvent);
			}
		}
		void SetLoopMode(bool loop) noexcept override { m_loop = loop; }
		void SetWindow(IWindow* window) noexcept override {
			HWND tmp = nullptr;
			if (window) {
				WindowPlatformData data = window->GetPlatformData();
				switch (data.GetWindowType()) {
				case WindowType::Windows:
					tmp = static_cast<HWND>(data.GetNativePtr());
					break;
				default:
					tmp = nullptr;
					break;
				}
			}
			if (m_hVideoWindow == tmp) return;
			m_hVideoWindow = tmp;
			MFTIME current = GetCurrentMFTime();
			m_session->SetTopology(
				MFSESSION_SETTOPOLOGY_FLAGS::MFSESSION_SETTOPOLOGY_CLEAR_CURRENT, nullptr
			);
			SetSource(m_currentSourceName, m_currentSourceType);
			Play(MediaPlayerSeekPos::Begin, static_cast<int64_t>(current) * 100);
		}
		Image GetCurrentFrame() noexcept override {
			if (!m_videoDisplayControl) return Image();
			BITMAPINFOHEADER header{};
			header.biSize = sizeof(BITMAPINFOHEADER);
			BYTE* buffer = nullptr;
			DWORD size = 0;
			LONGLONG timeStamp = 0;
			m_videoDisplayControl->GetCurrentImage(&header, &buffer, &size, &timeStamp);
			if (!buffer) return Image();
			Image ret = Image(static_cast<uint32_t>(header.biWidth), static_cast<uint32_t>(header.biHeight));
			Pixel* data = ret.Data();
			BYTE* tmp = buffer;
			for (uint32_t h = ret.Height(), w = ret.Width(), y = 0; y < h; ++y) {
				Pixel* line = data + (w * (h - y - 1));
				for (uint32_t x = 0; x < w; ++x) {
					line[x].r = tmp[2];
					line[x].g = tmp[1];
					line[x].b = tmp[0];
					//アルファ値に0が入るため
					line[x].a = 255;// tmp[3];
					tmp += 4;
				}
			}
			CoTaskMemFree(buffer);
			return ret;
		}
	public:
		/// @brief 絶対値で音量を設定する。
		/// @param absoluteVolume 音量の絶対値。[0, 100]に丸められる。
		/// @return 設定できなかったとき、false。
		bool SetVolume(int32_t absoluteVolume) noexcept override {
			absoluteVolume = Math::Clamp<int32_t>(absoluteVolume, 0, 100);
			if (m_volume == absoluteVolume) return true;
			if (!m_audioStreamVolume) return false;
			UINT32 count = 0;
			HRESULT hr = m_audioStreamVolume->GetChannelCount(&count);
			if (hr < 0) return false;
			for (UINT32 i = 0; i < count; ++i) {
				const float volume = m_muted ? 0.f : absoluteVolume / 100.f;
				hr = m_audioStreamVolume->SetChannelVolume(i, volume);
				if (hr < 0) break;
			}
			if (hr >= 0) m_volume = absoluteVolume;
			return true;
		}
		/// @brief 現在の音量からの相対値で音量を設定する。
		/// @param relativeVolume 現在の音量からの相対値。
		/// @details 設定後の音量の絶対値が[0, 100]を超えることはない。
		/// @return 設定できなかったとき、false。
		bool SetRelativeVolume(int32_t relativeVolume) noexcept override {
			return SetVolume(m_volume + relativeVolume);
		}
		/// @brief 現在の音量を取得する。
		/// @return [0, 100]の値を返す。失敗したとき、負の値を返す。
		int32_t GetVolume() const noexcept override {
			return m_volume;
		}
		/// @brief ミュート状態にする、もしくは解除する
		/// @param mute trueのとき、ミュート状態にする。falseのとき、解除する。
		/// @return 設定に成功した、もしくは既に指定した状態のとき、true。
		bool SetMute(bool mute) noexcept override {
			if (mute == m_muted) return true;
			if (!m_audioStreamVolume) return false;
			UINT32 count = 0;
			HRESULT hr = m_audioStreamVolume->GetChannelCount(&count);
			if (hr < 0) return false;
			for (UINT32 i = 0; i < count; ++i) {
				const float volume = mute ? 0.f : m_volume / 100.f;
				hr = m_audioStreamVolume->SetChannelVolume(i, volume);
				if (hr < 0) break;
			}
			if (hr >= 0) m_muted = mute;
			return true;
		}
		/// @brief ミュート状態を切り替える
		/// @return 成功したとき、true。
		bool ToggleMute() noexcept override {
			return SetMute(!m_muted);
		}
		/// @brief 現在、ミュート状態か調べる。
		bool IsMuted() const noexcept override {
			return m_muted;
		}
	public:
		/// @brief 絶対値でマスター音量を設定する。
		/// @param absoluteVolume 音量の絶対値。[0, 100]に丸められる。
		/// @return 設定できなかったとき、false。
		bool SetMasterVolume(int32_t absoluteVolume) noexcept override {
			LockGuard lock{ s_mtx };
			absoluteVolume = Math::Clamp<int32_t>(absoluteVolume, 0, 100);
			if (s_volume == absoluteVolume) return true;
			if (!s_simpleAudioVolume) return false;
			HRESULT hr = s_simpleAudioVolume->SetMasterVolume(absoluteVolume / 100.f);
			if (hr < 0) return false;
			s_volume = absoluteVolume;
			return true;
		}
		/// @brief 現在のマスター音量からの相対値でマスター音量を設定する。
		/// @param relativeVolume 現在の音量からの相対値。
		/// @details 設定後の音量の絶対値が[0, 100]を超えることはない。
		/// @return 設定できなかったとき、false。
		bool SetRelativeMasterVolume(int32_t relativeVolume) noexcept override {
			LockGuard lock{ s_mtx };
			int32_t absoluteVolume = Math::Clamp<int32_t>(s_volume + relativeVolume, 0, 100);
			if (s_volume == absoluteVolume) return true;
			if (!s_simpleAudioVolume) return false;
			HRESULT hr = s_simpleAudioVolume->SetMasterVolume(absoluteVolume / 100.f);
			if (hr < 0) return false;
			s_volume = absoluteVolume;
			return true;
		}
		/// @brief 現在のマスター音量を取得する。
		/// @return [0, 100]の値を返す。失敗したとき、負の値を返す。
		int32_t GetMasterVolume() const noexcept override {
			int32_t ret = 0;
			{
				LockGuard lock{ s_mtx };
				ret = s_volume;
			}
			return ret;
		}
		/// @brief マスター音量をミュート状態にする、もしくは解除する
		/// @param mute trueのとき、ミュート状態にする。falseのとき、解除する。
		/// @details この関数によってそれぞれのメディアプレイヤーのミュート状態が変わることはない。
		/// @return 設定に成功した、もしくは既に指定した状態のとき、true。
		bool SetMuteMaster(bool mute) noexcept override {
			LockGuard lock{ s_mtx };
			if (mute == s_muted) return true;
			if (!s_simpleAudioVolume) return false;
			const float volume = s_muted ? 0.f : s_volume / 100.f;
			HRESULT hr = s_simpleAudioVolume->SetMasterVolume(volume);
			if (hr < 0) return false;
			m_muted = mute;
			return true;
		}
		/// @brief マスター音量のミュート状態を切り替える
		/// @return 成功したとき、true。
		bool ToggleMuteMaster() noexcept override {
			bool mute = false;
			{
				LockGuard lock{ s_mtx };
				mute = s_muted;
			}
			return SetMuteMaster(!mute);
		}
		/// @brief 現在、マスター音量がミュート状態か調べる。
		bool IsMutedMaster() const noexcept override {
			LockGuard lock{ s_mtx };
			return s_muted;
		}
	public:
		/// @brief 再生位置を移動する。
		/// @param nsOffset 現在の位置からのオフセット(ns)
		/// @return 成功したとき、true。
		bool SeekRelative(int64_t nsOffset) noexcept override {
			MFPlayerState state = m_state.load(memory_order::acquire);
			bool ret = Play(MediaPlayerSeekPos::Current, nsOffset);
			if (!ret) return false;
			if (state == MFPlayerState::Paused) return Pause();
			else if (state == MFPlayerState::Stopped) return Stop();
			else return ret;
		}
		/// @brief 再生速度を変更する。
		/// @param speed 再生速度の倍率。1.0fが通常の速度。
		/// @return 設定できなかったとき、false。
		bool SetSpeed(float speed) noexcept override {
			if (speed < 0.f) return false;
			IMFGetService* service = nullptr;
			IMFRateSupport* rateSupport = nullptr;
			IMFRateControl* rateControl = nullptr;
			HRESULT hr = S_OK;
			do {
				hr = m_session->QueryInterface(IID_PPV_ARGS(&service));
				if (hr < 0) break;
				hr = service->GetService(MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&rateSupport));
				if (hr < 0) break;
				hr = rateSupport->IsRateSupported(FALSE, speed, nullptr);
				if (hr < 0) break;
				hr = service->GetService(MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&rateControl));
				if (hr < 0) break;
				hr = rateControl->SetRate(FALSE, speed);
			} while (false);
			SafeRelease(rateControl, rateSupport, service);
			return hr >= 0;
		}
		/// @brief 現在の再生速度を取得する。
		/// @return 失敗したとき、負の値を返す。
		float GetSpeed() const noexcept override {
			float ret = 0.f;
			IMFGetService* service = nullptr;
			IMFRateControl* rateControl = nullptr;
			HRESULT hr = S_OK;
			do {
				hr = m_session->QueryInterface(IID_PPV_ARGS(&service));
				if (hr < 0) break;
				hr = service->GetService(MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&rateControl));
				if (hr < 0) break;
				hr = rateControl->GetRate(nullptr, &ret);
			} while (false);
			SafeRelease(rateControl, service);
			if (hr < 0) return -1.f;
			else return ret;
		}
	};
}
