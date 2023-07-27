cmake_minimum_required (VERSION 3.8)

#CMakeによるコンパイラ確認時のエラー回避
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

###関数設定 Begin###
#関数群の読み込み
include(./functions.cmake)
###関数設定 End###

###コンパイラ設定 Begin###
set(CMAKE_CXX_STANDARD 23)
set(STD_LIBRARY_DIR)
if (MSBUILD)
	message(STATUS "MSBuild Mode")
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
	set(MSVC_TOOLSET_VERSION 143)
elseif (MSVC)
	message(STATUS "MSVC Mode")
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
	#MSVCではMANIFESTを無効にしないとコンパイルできない
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -MANIFEST:NO")
	set(MSVC_TOOLSET_VERSION 143)
elseif(GCC)
	message(STATUS "No MSVC Mode")
	add_compile_options(-fmodules-ts -Mno-modules -DUNICODE -D_UNICODE)
endif()
###コンパイラ設定 End###

project ("3DApplication")

#必要なヘッダを設定
FPrecompileSTD(iostream cstdint)

add_subdirectory(src)
add_executable(main)
target_sources(main PRIVATE
	src/main_test.cpp
)
target_link_libraries(main PRIVATE src)
add_dependencies(main src_t)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

###TARGET src Begin###
#add_subdirectory(src)
###TARGET src End###

##Correct
#add_library(temp_a OBJECT src/a_test.cpp)
#add_library(temp_b OBJECT src/null.cpp)
#add_library(temp_c OBJECT src/System/Module/CSTDINT.ixx)
#add_custom_target(temp DEPENDS temp_a temp_b temp_c)
#add_executable(main)
#target_sources(main PRIVATE
#	src/main_test.cpp
#	$<TARGET_OBJECTS:temp_a>
#	$<TARGET_OBJECTS:temp_b>
#	$<TARGET_OBJECTS:temp_c>
#)
#add_dependencies(main temp)

##Correct
#add_library(System_Module OBJECT src/System/Module/CSTDINT.ixx)
#add_custom_target(src DEPENDS System_Module)
#add_executable(main)
#target_sources(main PRIVATE src/main_test.cpp $<TARGET_OBJECTS:System_Module>)
#add_dependencies(main src)

##Correct
#FSetModuleFileFlags(src/System/Module)
#add_library(System_Module STATIC src/System/Module/CSTDINT.ixx)
#add_custom_target(src DEPENDS System_Module)
#add_executable(main)
#target_sources(main PRIVATE src/main_test.cpp)
#target_link_libraries(main PRIVATE System_Module)
#add_dependencies(main src)

#if (MSVC)
#	set_source_files_properties(src/System/Module/CSTDINT.ixx PROPERTIES COMPILE_FLAGS "-interface -TP")
#else()
#	set_source_files_properties(src/System/Module/CSTDINT.ixx PROPERTIES COMPILE_FLAGS "-x c++")
#endif()
