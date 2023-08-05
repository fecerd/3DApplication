cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Drawing_0 OBJECT
	Color.ixx
	Rectangle.ixx
)
add_library(Drawing_1 OBJECT
	Pixel.ixx
)
add_library(Drawing_2 OBJECT
	Image.ixx
)
add_library(Drawing_3 OBJECT
	BMP.ixx
	PNG.ixx
	TGA.ixx
)
add_library(Drawing_4 OBJECT
	Drawing.ixx
	Image.cpp
)

add_dependencies(Drawing_1 PUBLIC Drawing_0)
add_dependencies(Drawing_2 PUBLIC Drawing_1)
add_dependencies(Drawing_3 PUBLIC Drawing_2)
add_dependencies(Drawing_4 PUBLIC Drawing_3)
add_library(Drawing INTERFACE Drawing_0 Drawing_1 Drawing_2 Drawing_3 Drawing_4)
