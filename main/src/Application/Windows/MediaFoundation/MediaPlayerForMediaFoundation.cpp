module MediaPlayerForMediaFoundation;
import MFHeader;
import System;
import Application;	//Log
import IMediaPlayer;
using namespace System;
using namespace System::Drawing;
using namespace System::Application;
using namespace System::Application::Windows;
using namespace MF;

#undef IID_PPV_ARGS
#define IID_PPV_ARGS(ppType) MF::GetIID(ppType), MF::IID_PPV_ARGS_Helper(ppType)

MFMediaPlayer::~MFMediaPlayer() noexcept {
	Stop();
	{
		LockGuard lock{ s_mtx };
		SafeRelease(s_simpleAudioVolume);
	}
	SafeRelease(m_videoDisplayControl, m_audioStreamVolume);
	m_state.store(MFPlayerState::Closing, memory_order::release);
	if (m_session) {
		//待機イベントを作成
		m_closeEvent.store(CreateEvent(nullptr, FALSE, FALSE, nullptr), memory_order::release);
		//MESessionClosedイベントを送る
		if (m_session->Close() >= 0) {
			//Invoke関数内から待機イベントが設定されるまで待つ
			DWORD result = WaitForSingleObject(m_closeEvent.load(memory_order::acquire), 1500);
			if (result == WAIT_TIMEOUT) {
				Application::Log(Exception(u"MediaFoundationがタイムアウトしました。").what());
				m_state.store(MFPlayerState::Closed, memory_order::release);
			}
			else {
				while (m_state.load(memory_order::acquire) != MFPlayerState::Closed);
			}
		}
		//待機イベントを解放
		CloseHandle(m_closeEvent.load(memory_order::acquire));
		m_closeEvent.store(nullptr, memory_order::release);
		//メディアセッションを終了
		m_session->Shutdown();
		SafeRelease(m_session);
	}
	m_currentSourceName = String();
	m_currentSourceType = MediaPlayerSourceType::Local;
	m_state.store(MFPlayerState::Uninitialized, memory_order::release);
	//MediaFoundationの終了
	MFShutdown();
	CoUninitialize();
}
