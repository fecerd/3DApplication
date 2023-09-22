
add_subdirectory(DirectX)

FAddObjectLibrary(Common3D_0
	SOURCES
		Common3DUtilities.ixx
	DEPENDS
		System
)

FAddObjectLibrary(Common3D_1
	SOURCES
		Common3DInterface.ixx
	DEPENDS
		Common3D_0
)

FAddObjectLibrary(Common3D_2
	SOURCES
		Common3D.ixx
	DEPENDS
		Common3D_1
		Application
)

FAddObjectLibrary(Common3D_3
	SOURCES
		Common3D.cpp
	DEPENDS
		Common3D_2
		DirectX
)

FCombineObjectLibrary(Common3D
	PRIVATE
		Common3D_0
		Common3D_1
		Common3D_2
		Common3D_3
	PUBLIC_LINK
		DirectX
)
