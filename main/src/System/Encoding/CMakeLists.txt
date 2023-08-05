cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Encoding_0 OBJECT
	CP932.ixx
)
add_library(Encoding_1 OBJECT
	CodePoint.ixx
)
add_library(Encoding_2 OBJECT
	CodePage.ixx
)
add_library(Encoding_3 OBJECT
	Encoding.ixx
)

add_library(Encoding OBJECT)
target_sources(Encoding PUBLIC
	$<TARGET_OBJECTS:Encoding_0>
	$<TARGET_OBJECTS:Encoding_1>
	$<TARGET_OBJECTS:Encoding_2>
	$<TARGET_OBJECTS:Encoding_3>
)
add_custom_target(Encoding_0_t DEPENDS Encoding_0)
add_custom_target(Encoding_1_t DEPENDS Encoding_1)
add_custom_target(Encoding_2_t DEPENDS Encoding_2)
add_custom_target(Encoding_3_t DEPENDS Encoding_3)
add_dependencies(Encoding_1 Encoding_0_t)
add_dependencies(Encoding_2 Encoding_1_t)
add_dependencies(Encoding_3 Encoding_2_t)
add_dependencies(Encoding Encoding_3_t)
add_custom_target(Encoding_t DEPENDS Encoding)

add_dependencies(Encoding_0 std)
add_dependencies(Encoding_1 Module_2_t)
add_dependencies(Encoding_2 Module_8_t)

