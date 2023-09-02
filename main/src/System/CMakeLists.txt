cmake_minimum_required(VERSION 3.8)

## サブディレクトリ
add_subdirectory(Module)
add_subdirectory(Encoding)

FAddObjectLibrary(System_0
	SOURCES
		System.ixx
	DEPENDS
		Module
		Encoding
)

FCombineObjectLibrary(System
	PUBLIC
		System_0
	PUBLIC_LINK
		Encoding
		Module
)
