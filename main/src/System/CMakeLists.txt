cmake_minimum_required(VERSION 3.8)

## モジュールファイルをC++ファイルとしてコンパイルできるようにする。
FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Module)
add_subdirectory(Encoding)

add_library(System_0 OBJECT
	System.ixx
)
add_custom_target(System_0_t DEPENDS System_0)

add_library(System OBJECT)
target_sources(System PUBLIC
	$<TARGET_OBJECTS:System_0>
	$<TARGET_OBJECTS:Module>
	$<TARGET_OBJECTS:Encoding>
)

add_dependencies(System System_0_t)
add_custom_target(System_t DEPENDS System)

add_dependencies(System_0 Module_t Encoding_t)
