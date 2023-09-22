
FAddObjectLibrary(Components_0
	SOURCES
		Components-Declaration.ixx
)

FAddObjectLibrary(Components_1
	SOURCES
		Components-Component.ixx
	DEPENDS
		Components_0
		System
)

FAddObjectLibrary(Components_2
	SOURCES
		Components-Transform.ixx
		Components-Light.ixx
		Components-Rigidbody.ixx
	DEPENDS
		Components_1
)

FAddObjectLibrary(Components_3
	SOURCES
		Components-Script.ixx
	DEPENDS
		Components_2
		Application	#EventArgs
)

FAddObjectLibrary(Components_4
	SOURCES
		Components-Animator_Internal.ixx
		Components-Camera.ixx
		Components-MeshFilter.ixx
	DEPENDS
		Components_3
		Engine_0	#Bone
		Common3D
)

FAddObjectLibrary(Components_5
	SOURCES
		Components-Animator.ixx
	DEPENDS
		Components_4
)

FAddObjectLibrary(Components_6
	SOURCES
		Components-GameObject.ixx
	DEPENDS
		Components_5
		Engine_1	#Scene
)

FAddObjectLibrary(Components_7
	SOURCES
		Components.ixx
	DEPENDS
		Components_6
)

FAddObjectLibrary(Components_8
	SOURCES
		Components-Animator.cpp
		Components-Camera.cpp
		Components-GameObject.cpp
		Components-Light.cpp
		Components-MeshFilter.cpp
		Components-Rigidbody.cpp
	DEPENDS
		Components_7
)

FAddObjectLibrary(Components_9
	SOURCES
		Components-Script.cpp
	DEPENDS
		Components_8
		Engine_2	#SceneManager
)

FCombineObjectLibrary(Components
	PRIVATE
		Components_0
		Components_1
		Components_2
		Components_3
		Components_4
		Components_5
		Components_6
		Components_7
		Components_8
		Components_9
)
