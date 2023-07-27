cmake_minimum_required(VERSION 3.8)

# ### 関数定義 Begin ###
# function(FGetLibraryNames libraryName count out)
# 	set(_end)
# 	math(EXPR _end "${count}-1")
# 	set(temp)
# 	foreach(i RANGE 0 ${_end})
# 		list(APPEND temp ${libraryName}_${i})
# 	endforeach()
# 	set(${out} ${temp} PARENT_SCOPE)
# endfunction()

# function(FAddLibrary libname listName count dependentTargetNames)
# 	set(_end)
# 	math(EXPR _end "${count}-1")
# 	set(_objectLibs)
# 	foreach(i RANGE 0 ${_end})
# 		set(_objectLibName ${libname}_${i})
# 		list(APPEND _objectLibs ${_objectLibName})
# 		add_library(${_objectLibName} OBJECT)
# 		target_sources(${_objectLibName} PRIVATE
# 			${${listName}_${i}}
# 		)
# 		if(i)
# 			set(_prev)
# 			math(EXPR _prev "${i}-1")
# 			add_dependencies(${_objectLibName} ${libname}_${_prev})
# 		endif()
# 	endforeach()
# 	add_library(${libname} INTERFACE)
# 	target_link_libraries(${libname} INTERFACE
# 		${_objectLibs}
# 	)
# 	if (NOT dependentTargetNames)
# 		add_dependencies(${libname} ${_objectLibs})
# 	else()
# 		add_dependencies(${libname}_0 ${dependentTargetNames})
# 		add_dependencies(${libname} ${dependentTargetNames} ${_objectLibs})
# 	endif()
# 	add_custom_target(${libname}_t DEPENDS ${libname})
# endfunction()
# ### 関数定義 End ###

#モジュールファイルをC++ファイルとして読み取れるようにする。
FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR})

add_library(src_0 OBJECT
	null.cpp
)
add_dependencies(src_0 std)
add_library(src INTERFACE)
target_link_libraries(src INTERFACE src_0)
add_dependencies(src src_0)
add_custom_target(src_t DEPENDS src)

##成功例1
# FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR}/System/Module)
# set(System_Module_Src_0
# 	System/Module/CSTDINT.ixx
# )
# set(System_Module_Src_1
# 	System/Module/Traits.ixx
# )
# FAddLibrary(System_Module System_Module_Src 2 false)
# set(src_Src_0
# 	a_test.cpp
# 	null.cpp
# )
# FAddLibrary(src src_Src 1 System_Module_t)

##成功例
#for i : [0, N-1]
#	add_library(name_i OBJECT)
#	target_sources(name_i PRIVATE
#		<source file>
#	)
#endfor
#add_library(name INTERFACE)
#target_link_libraries(name INTERFACE
#	name_0 ... name_N-1
#)
#add_dependencies(name child_t name_0 ... name_N-1)
#add_custom_target(name_t DEPENDS name)

##成功例2
# FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR}/System/Module)
# set(lib_name_system_module System_Module)
# add_library(${lib_name_system_module}_0 OBJECT
# 	System/Module/CSTDINT.ixx
# )
# set(part_names_system_module)
# FGetLibraryNames(${lib_name_system_module} 1 part_names_system_module)
# add_library(${lib_name_system_module} INTERFACE)
# target_link_libraries(${lib_name_system_module} INTERFACE
# 	${part_names_system_module}
# )
# add_dependencies(${lib_name_system_module} ${part_names_system_module})
# add_custom_target(${lib_name_system_module}_t DEPENDS ${lib_name_system_module})
# set(lib_name src)
# add_library(${lib_name}_0 OBJECT
# 	a_test.cpp
# 	null.cpp
# )
# set(part_names)
# FGetLibraryNames(${lib_name} 1 part_names)
# add_library(${lib_name} INTERFACE)
# target_link_libraries(${lib_name} INTERFACE
# 	${part_names}
# )
# add_dependencies(${lib_name} ${lib_name_system_module}_t ${part_names})
# add_custom_target(${lib_name}_t DEPENDS ${lib_name})
#

##成功例3
# FSetModuleFileFlags(${CMAKE_CURRENT_SOURCE_DIR}/System/Module)
#add_library(System_Module_0 OBJECT)
#target_sources(System_Module_0 PRIVATE
#	System/Module/CSTDINT.ixx
#)
#add_library(System_Module INTERFACE)
#target_link_libraries(System_Module INTERFACE
#	System_Module_0
#)
#add_dependencies(System_Module System_Module_0)
#add_custom_target(System_Module_t DEPENDS System_Module)
#add_library(src_0 OBJECT)
#target_sources(src_0 PRIVATE
#	a_test.cpp
#	null.cpp
#)
#add_library(src INTERFACE)
#target_link_libraries(src INTERFACE
#	src_0
#)
#add_dependencies(src System_Module_t src_0)
#add_custom_target(src_t DEPENDS src)
