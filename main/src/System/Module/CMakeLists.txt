cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Encoding_0 OBJECT
	../Encoding/CP932.ixx
)
add_library(Encoding_1 OBJECT
	../Encoding/CodePoint.ixx
)
add_library(Encoding_2 OBJECT
	../Encoding/CodePage.ixx
)
add_library(Encoding_3 OBJECT
	../Encoding/Encoding.ixx
)
add_library(Encoding INTERFACE Encoding_0 Encoding_1 Encoding_2 Encoding_3)
add_library(Module_0 OBJECT
	CSTDINT.ixx
	Coroutine.ixx
	ExclusiveObject.ixx
	SmartPtr.ixx
)
add_library(Module_1 OBJECT
	CSTDINT.ixx
	Coroutine.ixx
	ExclusiveObject.ixx
	SmartPtr.ixx
)
add_library(Module_2 OBJECT
	BitStream.ixx
	FlagSet.ixx
	Iterator.ixx
	Math.ixx
	Sorts.ixx
	TypeID.ixx
)
add_library(Module_3 OBJECT
	Float.ixx
	Memory.ixx
	VectorBase.ixx
)
add_library(Module_4 OBJECT
	Memory.cpp
	StringBlock.ixx
	Suballocator.ixx
)
add_library(Module_5 OBJECT
	Exception.ixx
	TypeName.ixx
)
add_library(Module_6 OBJECT
	Exception.cpp
	ForwardList.ixx
	Function.ixx
	Objects/Object_Internal.ixx
)
add_library(Module_7 OBJECT
	IEnumerable.ixx
	Objects/Objects.ixx
	Property.ixx
	Thread.ixx
)
add_library(Module_8 OBJECT
	Counter.ixx
	CString.ixx
	List.ixx
	Point.ixx
	Size.ixx
	TaskWorker.ixx
	Vector.ixx
	Vector2.ixx
	Vector3.ixx
)
add_library(Module_9 OBJECT
	BSpline.ixx
	DisposingObject.ixx
	EventHandler.ixx
	HashMap.ixx
	Matrix.ixx
	Range.ixx
)
add_library(Module_10 OBJECT
	ManagedObject.ixx
	Quaternion.ixx
	Time.ixx
)
add_library(Module_11 OBJECT
	DualQuaternion.ixx
	StringBlock.cpp
	Time.cpp
)
add_library(Module_12 OBJECT
	Module.ixx
)
add_library(Module INTERFACE
	Module_0
	Module_1
	Module_2
	Module_3
	Module_4
	Module_5
	Module_6
	Module_7
	Module_8
	Module_9
	Module_10
	Module_11
	Module_12
)

add_dependencies(Encoding_1 Module_2)
add_dependencies(Module_4 Encoding_1)
add_dependencies(Encoding_2 Module_8)
add_dependencies(Module_11 Encoding)


#add_library("Module" STATIC
#	BitStream.ixx
#	BSpline.ixx
#	Coroutine.ixx
#	Counter.ixx
#	CSTDINT.ixx
#	CString.ixx
#	DisposingObject.ixx
#	DualQuaternion.ixx
#	EventHandler.ixx
#	Exception.ixx
#	Exception.cpp
#	ExclusiveObject.ixx
#	FlagSet.ixx
#	Float.ixx
#	ForwardList.ixx
#	Function.ixx
#	HashMap.ixx
#	IEnumerable.ixx
#	Iterators.ixx
#	List.ixx
#	ManagedObject.ixx
#	Math.ixx
#	Matrix.ixx
#	Memory.ixx
#	Module.ixx
#	Point.ixx
#	Property.ixx
#	Quaternion.ixx
#	Range.ixx
#	Size.ixx
#	SmartPtr.ixx
#	Sorts.ixx
#	StringBlock.ixx
#	StringBlock.cpp
#	Suballocator.ixx
#	TaskWorker.ixx
#	Thread.ixx
#	Time.ixx
#	Time.cpp
#	Traits.ixx
#	TypeID.ixx
#	TypeName.ixx
#	Vector.ixx
#	VectorBase.ixx
#	Vector2.ixx
#	Vector3.ixx
#	XOR128.ixx
#)

#target_link_libraries("Module" PUBLIC "Object")
#target_link_libraries("Module" PUBLIC "Encoding")
