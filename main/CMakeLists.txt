cmake_minimum_required (VERSION 3.8)

## CMakeによるコンパイラ確認時のエラー回避(プロジェクト名より先にないとMSVCでエラー)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

## 関数群の読み込み
include(./functions.cmake)

## コンパイラ設定
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(STD_LIBRARY_DIR)
FSetCompileOptions("${CMAKE_MY_STD_PATH}")

## リンカオプション設定
FSetLinkerOptions()

## プロジェクト名設定
project ("3DApplication")

## 必要なヘッダを設定
FPrecompileSTD(
	cstdint
	initializer_list
	new
	iostream
	fstream
	cstring
	string
	condition_variable
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
	stdexcept
	source_location
	atomic
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
