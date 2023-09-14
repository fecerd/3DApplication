
FAddObjectLibrary(DirectInput_0
	SOURCES
		DirectInput.ixx
	DEPENDS
		System
		WinAPI
		InputSystem_1	#IInputManager
)

FCombineObjectLibrary(DirectInput
	PUBLIC
		DirectInput_0
)
