cmake_minimum_required(VERSION 3.8)

## サブディレクトリ
add_subdirectory(Boost)
add_subdirectory(System)
add_subdirectory(Test)

## ターゲット追加
FAddObjectLibrary(src_0
	SOURCES
		null.cpp
	DEPENDS
		System
)
FAddObjectLibrary(src_1
	SOURCES
		main_test.cpp
	DEPENDS
		src_0
		Test
)

## src全体のターゲット
## 実行可能形式ターゲット'main'にsrc以下のライブラリをリンクするために、
## ターゲット'src'に対してPUBLICでリンクする。
FCombineObjectLibrary(src
	PUBLIC
		src_0
		src_1
	PUBLIC_LINK
		System
		Test
		Boost
)
