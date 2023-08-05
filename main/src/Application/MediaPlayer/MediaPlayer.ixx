export module MediaPlayer;
import System;
import Drawing;
import IWindow;
import IMediaPlayer;
using System::Drawing::Image;

export namespace System::Application {
	class MediaPlayer {
		IMediaPlayer* m_mediaPlayer = nullptr;
	public:
		MediaPlayer() noexcept;
		MediaPlayer(const MediaPlayer&) noexcept = delete;
		MediaPlayer(MediaPlayer&& arg) noexcept : m_mediaPlayer(arg.m_mediaPlayer) {
			arg.m_mediaPlayer = nullptr;
		}
		~MediaPlayer() noexcept {
			delete m_mediaPlayer;
			m_mediaPlayer = nullptr;
		}
	public:
		bool Load(const String& name, MediaPlayerSourceType type, const String& filePath, bool setCurrent = false) noexcept {
			return m_mediaPlayer ? m_mediaPlayer->Load(name, type, filePath, setCurrent) : false;
		}
		template<System::Seconds S = seconds>
		bool Play(MediaPlayerSeekPos pos = MediaPlayerSeekPos::Current, S offset = S(0)) noexcept {
			return m_mediaPlayer ? m_mediaPlayer->Play(pos, nanoseconds(offset).count()) : false;
		}
		template<System::Seconds S = seconds>
		bool Play(const String& name, MediaPlayerSourceType type, MediaPlayerSeekPos pos = MediaPlayerSeekPos::Begin, S offset = S(0)) noexcept {
			return m_mediaPlayer ? m_mediaPlayer->Play(name, type, pos, nanoseconds(offset).count()) : false;
		}
		bool Pause() noexcept { return m_mediaPlayer ? m_mediaPlayer->Pause() : false; }
		bool Stop() noexcept { return m_mediaPlayer ? m_mediaPlayer->Stop() : false; }
	public:
		bool IsPlaying() noexcept { return m_mediaPlayer ? m_mediaPlayer->IsPlaying() : false; }
		bool IsPaused() noexcept { return m_mediaPlayer ? m_mediaPlayer->IsPaused() : false; }
		bool IsEnded() noexcept { return m_mediaPlayer ? m_mediaPlayer->IsEnded() : false; }
	public:
		void Update() noexcept { if (m_mediaPlayer) m_mediaPlayer->Update(); }
		void SetLoopMode(bool loop) noexcept { if (m_mediaPlayer) m_mediaPlayer->SetLoopMode(loop); }
		void SetWindow(IWindow* window) noexcept { if (m_mediaPlayer) m_mediaPlayer->SetWindow(window); }
		Image GetCurrentFrame() noexcept { return m_mediaPlayer ? m_mediaPlayer->GetCurrentFrame() : Image(); }
	public:
		MediaPlayer& operator=(const MediaPlayer&) noexcept = delete;
		MediaPlayer& operator=(MediaPlayer&& rhs) noexcept {
			if (this == &rhs) return *this;
			delete m_mediaPlayer;
			m_mediaPlayer = rhs.m_mediaPlayer;
			rhs.m_mediaPlayer = nullptr;
			return *this;
		}
	};
}