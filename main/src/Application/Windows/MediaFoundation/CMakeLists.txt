cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(MediaFoundation_0
	SOURCES
		MediaPlayerForMediaFoundation.ixx
	DEPENDS
		MediaPlayer_0	#IMediaPlayer
		Application_4	#Application
)

FCombineObjectLibrary(MediaFoundation
	PUBLIC
		MediaFoundation_0
)

# target_link_libraries(MediaFoundation
# 	PUBLIC
# 		Mfreadwrite.lib
# 		mfplat.lib
# 		Mf.lib
# 		mfuuid.lib
# 		Shlwapi.lib
# 		Strmiids.lib
# )
