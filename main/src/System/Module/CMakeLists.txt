cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Objects)

add_library(Module_0 OBJECT
	CSTDINT.ixx
	Coroutine.ixx
	ExclusiveObject.ixx
	SmartPtr.ixx
)
add_library(Module_1 OBJECT
	Traits.ixx
	XOR128.ixx
)
add_library(Module_2 OBJECT
	BitStream.ixx
	FlagSet.ixx
	Iterators.ixx
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
)
add_library(Module_7 OBJECT
	IEnumerable.ixx
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
add_library(Module_12 OBJECT Module.ixx)

add_library(Module OBJECT)
target_sources(Module PUBLIC
	$<TARGET_OBJECTS:Module_0>
	$<TARGET_OBJECTS:Module_1>
	$<TARGET_OBJECTS:Module_2>
	$<TARGET_OBJECTS:Module_3>
	$<TARGET_OBJECTS:Module_4>
	$<TARGET_OBJECTS:Module_5>
	$<TARGET_OBJECTS:Module_6>
	$<TARGET_OBJECTS:Module_7>
	$<TARGET_OBJECTS:Module_8>
	$<TARGET_OBJECTS:Module_9>
	$<TARGET_OBJECTS:Module_10>
	$<TARGET_OBJECTS:Module_11>
	$<TARGET_OBJECTS:Module_12>
)

add_custom_target(Module_0_t DEPENDS Module_0)
add_custom_target(Module_1_t DEPENDS Module_1)
add_custom_target(Module_2_t DEPENDS Module_2)
add_custom_target(Module_3_t DEPENDS Module_3)
add_custom_target(Module_4_t DEPENDS Module_4)
add_custom_target(Module_5_t DEPENDS Module_5)
add_custom_target(Module_6_t DEPENDS Module_6)
add_custom_target(Module_7_t DEPENDS Module_7)
add_custom_target(Module_8_t DEPENDS Module_8)
add_custom_target(Module_9_t DEPENDS Module_9)
add_custom_target(Module_10_t DEPENDS Module_10)
add_custom_target(Module_11_t DEPENDS Module_11)
add_custom_target(Module_12_t DEPENDS Module_12)
add_dependencies(Module_1 Module_0_t)
add_dependencies(Module_2 Module_1_t)
add_dependencies(Module_3 Module_2_t)
add_dependencies(Module_4 Module_3_t)
add_dependencies(Module_5 Module_4_t)
add_dependencies(Module_6 Module_5_t)
add_dependencies(Module_7 Module_6_t)
add_dependencies(Module_8 Module_7_t)
add_dependencies(Module_9 Module_8_t)
add_dependencies(Module_10 Module_9_t)
add_dependencies(Module_11 Module_10_t)
add_dependencies(Module_12 Module_11_t)
add_dependencies(Module Module_12_t)
add_custom_target(Module_t DEPENDS Module)

add_dependencies(Module_0 std)
add_dependencies(Module_4 Encoding_1_t)
add_dependencies(Module_8 Objects_t)
add_dependencies(Module_11 Encoding_t)
