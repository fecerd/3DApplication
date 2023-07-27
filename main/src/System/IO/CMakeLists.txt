cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})
add_library(IO_0 OBJECT
	Path.ixx
)
add_library(IO_1 OBJECT
	File.ixx
	Path.cpp
	ResourcePath.ixx
)
add_library(IO_2 OBJECT
	IO.ixx
)
add_dependencies(IO_1 PUBLIC IO_0)
add_dependencies(IO_2 PUBLIC IO_1)
add_library(IO INTERFACE IO_0 IO_1 IO_2)
