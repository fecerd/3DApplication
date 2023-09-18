
FAddObjectLibrary(DirectInput_0
	SOURCES
		DirectInput.ixx
	DEPENDS
		Module
		WinAPI
		InputSystem_1	#IInputManager
)

FAddObjectLibrary(DirectInput_1
	SOURCES
		DirectInput.cpp
	DEPENDS
		DirectInput_0
		Headers
)

FCombineObjectLibrary(DirectInput
	PRIVATE
		DirectInput_0
		DirectInput_1
	PUBLIC_LINK
		dinput8
		dxguid
		Module
)
