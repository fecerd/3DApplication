cmake_minimum_required (VERSION 3.8)

## CMakeによるコンパイラ確認時のエラー回避
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

###関数設定 Begin###
## 関数群の読み込み
include(./functions.cmake)
###関数設定 End###

###コンパイラ設定 Begin###
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(STD_LIBRARY_DIR)
add_compile_options(-I ${CMAKE_MY_STD_PATH})
if (MSBUILD)
	message(STATUS "MSBuild Mode")
	add_compile_options($<IF:$<CONFIG:Debug>,/MDd,/MD>)
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
elseif (MSVC)
	message(STATUS "MSVC Mode")
	add_compile_options(-EHsc -DUNICODE -D_UNICODE -nologo)
	add_compile_options($<IF:$<CONFIG:Debug>,/MDd,/MD>)
	## MSVCではMANIFESTを無効にしないとコンパイルできない
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -MANIFEST:NO")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -MANIFEST:NO")
elseif(GCC)
	message(STATUS "GCC Mode")
	add_compile_options(-fmodules-ts -Mno-modules -DUNICODE -D_UNICODE -mwindows)
	add_compile_options($<$<CONFIG:Debug>:-D_DEBUG>)
	## MSVCではコンパイルが通るコードがg++では多重定義でリンクエラーとなる。
	## あまり得策ではないが、多重定義を無視するリンカオプションをつけて対応する。
	add_link_options(-Wl,--allow-multiple-definition)
	## 内部で使用されているstd::auto_ptr<>が警告を出すため、抑制しておく。
	add_compile_options(-Wno-attributes)
	add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-reorder -Wno-uninitialized)
	add_compile_options($<$<CONFIG:Debug>:-g>)
	add_link_options(-g)
elseif (CLANG)
	message(STATUS "CLANG Mode")
	add_compile_options(-DUNICODE -D_UNICODE)
	add_compile_options($<$<CONFIG:Debug>:-D_DEBUG>)
	## 不要な-MD、-MT、-MFオプションがpcm->objのコンパイル時に設定されてしまう。
	## あまり得策ではないが警告を出力しないことで対応する。
	add_compile_options(-Wno-unused-command-line-argument)
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
	typeinfo
	cassert
	utility	#無いとg++のBoostでエラーがでる
)

## ソースファイルディレクトリを追加
add_subdirectory(src)

## 実行可能形式ターゲット設定(ファイル名はmain)
add_executable(main)
## main関数を含むObjectLibrary"src"をExecutable"main"にリンクする。
target_link_libraries(main PRIVATE src)
## 出力先の変更
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

## Boostライブラリをリンクする
FLinkBoost(main false)
