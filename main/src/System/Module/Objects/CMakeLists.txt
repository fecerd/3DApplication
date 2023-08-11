cmake_minimum_required(VERSION 3.8)

## モジュールファイルをC++ファイルとしてコンパイルできるようにする。
FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Objects_0 OBJECT
	Objects_Internal.ixx
)
add_library(Objects_1 OBJECT
	Objects_Internal.cpp
	Objects.ixx
)

add_library(Objects OBJECT)
target_sources(Objects PUBLIC
	$<TARGET_OBJECTS:Objects_0>
	$<TARGET_OBJECTS:Objects_1>
)
add_custom_target(Objects_0_t DEPENDS Objects_0)
add_custom_target(Objects_1_t DEPENDS Objects_1)
add_dependencies(Objects_1 Objects_0_t)
add_dependencies(Objects Objects_1_t)
add_custom_target(Objects_t DEPENDS Objects)

add_dependencies(Objects_0 Module_5_t std)
