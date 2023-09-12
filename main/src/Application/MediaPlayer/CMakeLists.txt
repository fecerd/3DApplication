cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(MediaPlayer_0
	SOURCES
		IMediaPlayer.ixx
	DEPENDS
		System
		Application_2	#IWindow
)

FAddObjectLibrary(MediaPlayer_1
	SOURCES
		MediaPlayer.ixx
	DEPENDS
		MediaPlayer_0
)

FAddObjectLibrary(MediaPlayer_2
	SOURCES
		MediaPlayer.cpp
	DEPENDS
		MediaPlayer_1	#MediaPlayerForMediaFoundation
		MediaFoundation
)

FCombineObjectLibrary(MediaPlayer
	PUBLIC
		MediaPlayer_0
		MediaPlayer_1
		MediaPlayer_2
)
