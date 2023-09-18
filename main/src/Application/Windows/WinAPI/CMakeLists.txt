
FAddObjectLibrary(WinAPI_0
	SOURCES
		WinAPI_Constants.ixx
		WinAPI_Messages.ixx
	DEPENDS
		Module_0
)

FAddObjectLibrary(WinAPI_1
	SOURCES
		WinAPI.ixx
	DEPENDS
		WinAPI_0
)

FCombineObjectLibrary(WinAPI
	PRIVATE
		WinAPI_0
		WinAPI_1
)
