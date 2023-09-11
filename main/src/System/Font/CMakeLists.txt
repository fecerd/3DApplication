cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Font_0_0
	SOURCES
		OpenType_Internal0.ixx
	DEPENDS
		Module
		IO
		Drawing
)

FAddObjectLibrary(Font_0
	SOURCES
		IFont.ixx
		OpenType_Internal.ixx
	DEPENDS
		Font_0_0
		Module
		Drawing
		IO
)
FAddObjectLibrary(Font_1
	SOURCES
		OpenType.ixx
		OpenType_Internal.cpp
	DEPENDS
		Font_0
		Encoding
)
FAddObjectLibrary(Font_2
	SOURCES
		FontFactory.ixx
	DEPENDS
		Font_1
)

FAddObjectLibrary(Font_3
	SOURCES
		Font.ixx
	DEPENDS
		Font_2
)

FCombineObjectLibrary(Font
	PUBLIC
		Font_0
		Font_1
		Font_2
		Font_3
		Font_0_0
)
