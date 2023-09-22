
FAddObjectLibrary(Components_0
	SOURCES
		Component.ixx
	DEPENDS
		System
)

FAddObjectLibrary(Components_1
	SOURCES
		Camera.ixx
		Light.ixx
		MeshFilter.ixx
		Transform.ixx
	DEPENDS
		Components_0
		Common3D
		Engine_0	#LayerMask, Bone, EngineUtility
)

FAddObjectLibrary(Components_2
	SOURCES
		Animator.ixx
		Rigidbody.ixx
		Script.ixx
	DEPENDS
		Components_1
		Engine_1	#Scene
		Application	#EventArgs
)

FAddObjectLibrary(Components_3
	SOURCES
		GameObject.ixx
	DEPENDS
		Components_2
)

FAddObjectLibrary(Components_4
	SOURCES
		Components.ixx
		Animator.cpp
		GameObject.cpp
		MeshFilter.cpp
	DEPENDS
		Components_3
)

FCombineObjectLibrary(Components
	PRIVATE
		Components_0
		Components_1
		Components_2
		Components_3
		Components_4
)

target_link_libraries(Components PUBLIC Engine)
