
add_subdirectory(Components)

FAddObjectLibrary(Engine_0
	SOURCES
		LayerMask.ixx
		EngineUtilities.ixx
		Bone.ixx
	DEPENDS
		Module
)

FAddObjectLibrary(Engine_1
	SOURCES
		Scene.ixx
	DEPENDS
		Engine_0
		Application	#EventArgs
		Common3D
		Components_1	#Transform, Camera, Light, MeshFilter
)

FAddObjectLibrary(Engine_2
	SOURCES
		SceneManager.ixx
	DEPENDS
		Engine_1
		InputSystem
		Components_4	#Components
)

FAddObjectLibrary(Engine_3
	SOURCES
		Engine.ixx
	DEPENDS
		Engine_2
)

FAddObjectLibrary(Engine_4
	SOURCES
		SceneManager.cpp
		Scene.cpp
	DEPENDS
		Engine_3
)

FCombineObjectLibrary(Engine
	PRIVATE
		Engine_0
		Engine_1
		Engine_2
		Engine_3
		Engine_4
	PUBLIC_LINK
		Components
)
