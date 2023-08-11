cmake_minimum_required (VERSION 3.8)

## CMakeによるコンパイラ確認時のエラー回避
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

###関数設定 Begin###
## 関数群の読み込み
include(./functions.cmake)
###関数設定 End###

###コンパイラ設定 Begin###
set(CMAKE_CXX_STANDARD 23)
set(STD_LIBRARY_DIR)
add_compile_options($<$<CONFIG:Debug>:-D_DEBUG>)
if (MSBUILD)
	message(STATUS "MSBuild Mode")
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
elseif (MSVC)
	message(STATUS "MSVC Mode")
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
	## MSVCではMANIFESTを無効にしないとコンパイルできない
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -MANIFEST:NO")
elseif(GCC)
	message(STATUS "GCC Mode")
	add_compile_options(-fmodules-ts -Mno-modules -DUNICODE -D_UNICODE -mwindows)
	## MSVCではコンパイルが通るコードがg++では多重定義でリンクエラーとなる。
	## あまり得策ではないが、多重定義を無視するリンカオプションをつけて対応する。
	add_link_options(-Wl,--allow-multiple-definition)
endif()
###コンパイラ設定 End###

## プロジェクト名設定
project ("3DApplication")

## 必要なヘッダを設定
FPrecompileSTD(
	condition_variable
	cstdint
	compare
	thread
	type_traits
	iterator
	concepts
	mutex
	memory
	chrono
	coroutine
	limits
	initializer_list
	iostream
	stdexcept
	source_location
	cstring
	atomic
	fstream
	new
	exception
	functional
)

## ソースファイルディレクトリを追加
add_subdirectory(src)

## 実行可能形式ターゲット設定(ファイル名はmain)
add_executable(main)
## main関数を含むObjectLibrary"src"をExecutable"main"にリンクする。
target_link_libraries(main PRIVATE src)
## 出力先の変更
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
