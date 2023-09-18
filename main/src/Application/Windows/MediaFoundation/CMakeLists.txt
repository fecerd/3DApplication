
FAddObjectLibrary(MediaFoundation_0
	SOURCES
		MFHeader.ixx
)

FAddObjectLibrary(MediaFoundation_1
	SOURCES
		MediaPlayerForMediaFoundation.ixx
	DEPENDS
		MediaPlayer_0	#IMediaPlayer
		Application_5	#Application
		MediaFoundation_0
)

FCombineObjectLibrary(MediaFoundation
	PRIVATE
		MediaFoundation_0
		MediaFoundation_1
	PUBLIC_LINK
		Mfreadwrite
		mfplat
		Mf
		mfuuid
		#IUnknown実装用
		Shlwapi
		Strmiids		
)
