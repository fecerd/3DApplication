
add_subdirectory(DirectInput)
add_subdirectory(Forms)
add_subdirectory(MediaFoundation)
add_subdirectory(WinAPI)

FAddObjectLibrary(Windows_0
	SOURCES
		WindowClass.ixx
	DEPENDS
		Objects
		WinAPI
)

FAddObjectLibrary(Windows_1
	SOURCES
		DeviceContext.ixx
	DEPENDS
		Windows_0
		System
		Application_0	#ApplicationUtility
)

FAddObjectLibrary(Windows_2
	SOURCES
		WindowsEventArgs.ixx
	DEPENDS
		Windows_1
		Application_1	#EventArgs
)

FAddObjectLibrary(Windows_3
	SOURCES
		ControlManager.ixx
	DEPENDS
		Windows_2
		Forms
		Application_3	#IApplicationManager
)

FCombineObjectLibrary(Windows
	PRIVATE
		Windows_0
		Windows_1
		Windows_2
		Windows_3
	PUBLIC_LINK
		DirectInput
		Forms
		MediaFoundation
		WinAPI
)
