cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Compression_0 OBJECT
	Zlib.ixx
)
add_library(Compression_1 OBJECT
	Huffman.ixx
	LZ77.ixx
)
add_library(Compression_2 OBJECT
	Compression.ixx
)
add_dependencies(Compression_1 PUBLIC Compression_0)
add_dependencies(Compression_2 PUBLIC Compression_1)
add_library(Compression INTERFACE Compression_0 Compression_1 Compression_2)
