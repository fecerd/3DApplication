
FAddObjectLibrary(Drawing_0
	SOURCES
		Color.ixx
		Rectangle.ixx
	DEPENDS
		Module_0	#CSTDINT
)
FAddObjectLibrary(Drawing_1
	SOURCES
		Pixel.ixx
	DEPENDS
		Drawing_0
		Objects	#Type
)
FAddObjectLibrary(Drawing_2
	SOURCES
		Image.ixx
	DEPENDS
		Drawing_1
		Module_4	#VectorBase, Memory
		IO_1	#File
)
FAddObjectLibrary(Drawing_3
	SOURCES
		BMP.ixx
		PNG.ixx
		TGA.ixx
	DEPENDS
		Drawing_2
		Compression	#Zlib, Huffman
)
FAddObjectLibrary(Drawing_4
	SOURCES
		Drawing.ixx
		Image.cpp
	DEPENDS
		Drawing_3
)

FCombineObjectLibrary(Drawing
	PUBLIC
		Drawing_0
		Drawing_1
		Drawing_2
		Drawing_3
		Drawing_4
)
