cmake_minimum_required(VERSION 3.8)

## モジュールファイルをC++ファイルとしてコンパイルできるようにする。
FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Test_0 OBJECT
	Col.ixx
)
add_custom_target(Test_0_t DEPENDS Test_0)

add_library(Test OBJECT)
target_sources(Test PUBLIC
	$<TARGET_OBJECTS:Test_0>
)

add_dependencies(Test Test_0_t)
add_custom_target(Test_t DEPENDS Test)

add_dependencies(Test_0 std Module_6_t)
