module MediaPlayer;
using namespace System::Application;

#ifndef SDL
import MediaPlayerForMediaFoundation;
MediaPlayer::MediaPlayer() noexcept {
	m_mediaPlayer = new Windows::MFMediaPlayer();
}
#else
MediaPlayer::MediaPlayer() noexcept {

}
#endif