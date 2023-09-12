cmake_minimum_required(VERSION 3.8)

add_subdirectory(InputSystem)
add_subdirectory(MediaPlayer)
add_subdirectory(SDL)
add_subdirectory(Windows)

FAddObjectLibrary(Application_0
	SOURCES
		ApplicationUtility.ixx
		ApplicationEnum.ixx
		Log.ixx
	DEPENDS
		System
)

FAddObjectLibrary(Application_1
	SOURCES
		EventArgs.ixx		
	DEPENDS
		Application_0
)

FAddObjectLibrary(Application_2
	SOURCES
		IWindow.ixx
	DEPENDS
		Application_1
)

FAddObjectLibrary(Application_3
	SOURCES
		IApplicationManager.ixx
	DEPENDS
		Application_2
)

FAddObjectLibrary(Application_4
	SOURCES
		Application.ixx
	DEPENDS
		Application_3
)

FAddObjectLibrary(Application_5
	SOURCES
		Log.cpp
	DEPENDS
		Application_4
		WinAPI
)

FAddObjectLibrary(Application_6
	SOURCES
		Application.cpp
		IApplicationManager.cpp
	DEPENDS
		Application_5
		Windows_3	#ControlManager
)

FAddObjectLibrary(Application_7
	SOURCES
		IWindow.cpp
	DEPENDS
		Application_6
		InputSystem
)

FCombineObjectLibrary(Application
	PUBLIC
		Application_0
		Application_1
		Application_2
		Application_3
		Application_4
		Application_5
		Application_6
		Application_7
	PUBLIC_LINK
		InputSystem
		MediaPlayer
		SDL
		Windows
)
