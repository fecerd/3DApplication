cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Compression_0
	SOURCES
		Zlib.ixx
	DEPENDS
		Objects
		Module_3
)

FAddObjectLibrary(Compression_1
	SOURCES
		Huffman.ixx
		LZ77.ixx
	DEPENDS
		Compression_0
		Module_10
)

FAddObjectLibrary(Compression_2
	SOURCES
		Compression.ixx
	DEPENDS
		Compression_1
)

FCombineObjectLibrary(Compression
	PUBLIC
		Compression_0
		Compression_1
		Compression_2
)
