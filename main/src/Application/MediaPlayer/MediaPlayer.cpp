module MediaPlayer;
import NamespaceHelper;

#if !defined(SDL)
import MediaPlayerForMediaFoundation;
#else
#endif

#if !defined(SDL)

namespace System::Application {
	MediaPlayer::MediaPlayer() noexcept {
		m_mediaPlayer = new Windows::MFMediaPlayer();
	}
}


#else

namespace System::Application {
	MediaPlayer::MediaPlayer() noexcept {

	}
}

#endif