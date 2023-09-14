
FAddObjectLibrary(InputSystem_0
	SOURCES
		InputSystemUtility.ixx
	DEPENDS
		System
)

FAddObjectLibrary(InputSystem_1
	SOURCES
		IInputManager.ixx
	DEPENDS
		InputSystem_0
		Application_0	#ApplicationUtility
)

FAddObjectLibrary(InputSystem_2
	SOURCES
		InputSystem.ixx
	DEPENDS
		InputSystem_1
)

FAddObjectLibrary(InputSystem_3
	SOURCES
		InputSystem.cpp
	DEPENDS
		DirectInput
)

FCombineObjectLibrary(InputSystem
	PUBLIC
		InputSystem_0
		InputSystem_1
		InputSystem_2
		InputSystem_3
)
