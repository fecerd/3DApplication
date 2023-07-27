###Functions Begin###

#リストの要素を一行ずつ表示する。
#TITLE: リストの内容を囲むタイトル
#LIST: 表示するリスト
function(FPrintList TITLE LIST)
	message(STATUS "##### " ${TITLE} " Begin #####")
	foreach(ITEM ${LIST})
		message(STATUS "${ITEM}")
	endforeach()
	message(STATUS "##### " ${TITLE} " End #####")
endfunction()

#return_test(arg1 arg2)で呼び出したとき、
#ローカル変数outは文字列"arg2"が入り、${out}は"arg2"を表す。
#よって、arg2が呼び出し元で変数名を表すとき、set(${out} ... PARENT_SCOPE)は
#変数arg2への代入となる。
function(FReturnTest in out)
	set(${out} ${in} PARENT_SCOPE)
endfunction()

#ファイル名を取得する。
#rootDir: 相対パス表記するときのルートディレクトリ名。falseのとき絶対パス
#searchPattern: 検索するパターン。相対パスのとき、ルートは呼び出し元のCMAKE_SOURCE_DIR
#out: 検索結果のリスト
function(FGetFilenames rootDir searchPattern out)
	if (IS_DIRECTORY ${rootDir})
		file(GLOB ftmp LIST_DIRECTORIES false RELATIVE ${rootDir} ${searchPattern})
	else()
		file(GLOB ftmp LIST_DIRECTORIES false ${searchPattern})
	endif()
	set(${out} ${ftmp} PARENT_SCOPE)
endfunction()

function(FGetFilenamesEx rootDir searchDir searchPattern out)
	if (IS_DIRECTORY ${rootDir})
		file(GLOB ftmp LIST_DIRECTORIES false RELATIVE ${rootDir} ${searchDir}/${searchPattern})
	else()
		file(GLOB ftmp LIST_DIRECTORIES false ${searchDir}/${searchPattern})
	endif()
	set(${out} ${ftmp} PARENT_SCOPE)
endfunction()

#ファイルをコピーする。
#filename: コピーするファイル名
#srcDir: コピー元のファイルが存在するディレクトリ名
#dstDir: コピー先のディレクトリ名。存在しない場合、自動で生成する
function(FCopyFile filename srcDir dstDir)
	if (NOT EXISTS ${srcDir}/${filename})
		message(STATUS "Not Exists source file: " ${srcDir}/${filename})
		return()
	endif()
	if (NOT IS_DIRECTORY ${dstDir})
		return()
	endif()
	if (NOT EXISTS ${dstDir})
		file(MAKE_DIRECTORY ${dstDir})
	endif()
	file(COPY ${srcDir}/${filename} DESTINATION ${dstDir})
endfunction()

#定義されている変数を表示する。
function(FPrintVariableNames)
	get_cmake_property(variableNames VARIABLES)
	foreach(var ${variableNames})
		message(STATUS "${var}=${${var}}")
	endforeach()
endfunction()

#指定したディレクトリ下のモジュールファイル(.ixx, .cxx)を探索し、
#MSVCならコンパイルフラグ"-interface -TP"、GCCなら"-x c++"を付与する。
#sourceDir: ファイルを探索するディレクトリ
function(FSetModuleFileFlags searchDir)
	FGetFilenamesEx(false ${searchDir} *.cxx cxx_files)
	FGetFilenamesEx(false ${searchDir} *.ixx ixx_files)
	set(module_files ${cxx_files} ${ixx_files})
	foreach(module_file ${module_files})
		if (MSBUILD OR MSVC)
			set_source_files_properties(${module_file} PROPERTIES COMPILE_FLAGS "-interface -TP")
		elseif (GCC)
			set_source_files_properties(${module_file} PROPERTIES COMPILE_FLAGS "-x c++")
		endif()
	endforeach()
endfunction()

#可変長引数で渡した標準ライブラリヘッダをモジュールとしてコンパイルする。
#また、引数が一つ以上存在するとき、ターゲット"std"が定義される。
#可変長引数: cstdint, iostreamなどのヘッダ名
function(FPrecompileSTD)
	if (NOT ARGC)
		return()
	endif()
	set(std_list ${ARGV})
	#標準ライブラリをコンパイルする
	if (MSBUILD OR MSVC)
		set(_MSVC_HEADER_COMPILE_OPTIONS -std:c++latest -EHsc -DUNICODE -D_UNICODE -nologo -D_DLL -c -exportHeader)
		if (MSBUILD)
			set(_MSVC_HEADER_COMPILE_OPTIONS ${_MSVC_HEADER_COMPILE_OPTIONS} -D_M_FP_PRECISE)
			add_custom_target(std
				COMMAND ${CMAKE_CXX_COMPILER} $<$<CONFIG:Debug>:-D_DEBUG> ${_MSVC_HEADER_COMPILE_OPTIONS} -headerName:quote ${std_list}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
		elseif(MSVC)
			if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
				set(_MSVC_HEADER_COMPILE_OPTIONS ${_MSVC_HEADER_COMPILE_OPTIONS} -D_DEBUG)
			else()
				set(_MSVC_HEADER_COMPILE_OPTIONS ${_MSVC_HEADER_COMPILE_OPTIONS})
			endif()
			add_custom_target(std
				COMMAND ${CMAKE_CXX_COMPILER} ${_MSVC_HEADER_COMPILE_OPTIONS} -headerName:quote ${std_list}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
		endif()
		#cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
		list(LENGTH std_list std_list_length)
		math(EXPR _end "${std_list_length}-1")
		foreach(i RANGE 0 ${_end})
			list(GET std_list ${i} std_name)
			add_compile_options("SHELL:-headerUnit:angle ${std_name}=${std_name}.ifc")
		endforeach()
	elseif(GCC)
		#ヘッダの絶対パスをリスト化
		foreach(stdname ${std_list})
			list(APPEND std_fullpath_list ${STD_INCLUDE_DIR}/${stdname})
		endforeach()
		set(_GCC_HEADER_COMPILE_OPTIONS -std=gnu++23 -fmodules-ts -Mno-modules -x c++ -fmodule-header -fmodule-only -DUNICODE -D_UNICODE -c)
		if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			set(_MSVC_HEADER_COMPILE_OPTIONS ${_GCC_HEADER_COMPILE_OPTIONS} -g)
		endif()
		add_custom_target(std
			COMMAND ${CMAKE_CXX_COMPILER} ${_GCC_HEADER_COMPILE_OPTIONS} ${std_fullpath_list}
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)
	endif()
endfunction()

###Functions End###
