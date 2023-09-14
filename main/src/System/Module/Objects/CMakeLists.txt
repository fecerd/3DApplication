
FAddObjectLibrary(Objects_0
	SOURCES
		Objects_Internal.ixx
	DEPENDS
		std
		Module_6
)
FAddObjectLibrary(Objects_1
	SOURCES
		Objects_Internal.cpp
		Objects.ixx
	DEPENDS
		Objects_0
)

FCombineObjectLibrary(Objects
	PUBLIC
		Objects_0
		Objects_1
)
