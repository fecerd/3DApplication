cmake_minimum_required(VERSION 3.8)

FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("Module")

add_library("System" OBJECT System.ixx)

target_link_libraries("System" PUBLIC "Module")
target_link_libraries("System" PUBLIC "Encoding")
