cmake_minimum_required(VERSION 3.8)

## BMIファイル(.ifc)の出力先が作業ディレクトリでないため、モジュール名とBMIファイルパスを関連付けるオプションが必要。
## mainやFSetModuleFileFlags()内で設定するとエラーが出る(要検証)
if (MSBUILD)
	file(GLOB_RECURSE ixx_files LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/src/*.ixx)
	file(GLOB_RECURSE cxx_files LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/src/*.cxx)
	set(module_files ${cxx_files} ${ixx_files})
	foreach(module_file ${module_files})
		cmake_path(RELATIVE_PATH module_file BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE relative_path)
		get_filename_component(module_dir ${relative_path} DIRECTORY)
		get_filename_component(module_name ${relative_path} NAME_WE)
		add_compile_options("SHELL:-reference ${module_name}=${CMAKE_BINARY_DIR}/${module_dir}/${module_name}.ifc")
	endforeach()
endif()

## モジュールファイルをC++ファイルとしてコンパイルできるようにする。
FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(src_0 OBJECT
	null.cpp
)
add_library(src_1 OBJECT
	main_test.cpp
)
add_library(src OBJECT)
#リンクするオブジェクトファイルを指定する
target_sources(src PUBLIC
	$<TARGET_OBJECTS:src_1>
	$<TARGET_OBJECTS:src_0>
)

add_custom_target(src_0_t DEPENDS src_0)
add_custom_target(src_1_t DEPENDS src_1)
add_dependencies(src_1 src_0_t)
add_dependencies(src src_1_t)
add_custom_target(src_t DEPENDS src)

add_subdirectory(System)
add_dependencies(src_0 System_t)

add_subdirectory(Test)
add_dependencies(src_1 Test_t)

## MSVCの場合、オブジェクトライブラリ"src"に下位オブジェクトライブラリ"System","Test"をPUBLICリンクすればよい。
## Executable"main"に使用するため、PUBLICリンクである必要がある。
## オブジェクトライブラリはコンパイル順にオブジェクトライブラリ"xxx_N"を作成し、上位で利用するためのオブジェクトライブラリ"xxx"に
## target_sources(xxx PUBLIC $<TARGET_OBJECTS:xxx_N>)で追加する。
target_link_libraries(src
	PUBLIC System
	PUBLIC Test
)
