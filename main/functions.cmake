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

#ファイル名を取得する。
#rootDir: 相対パス表記するときのルートディレクトリ名。falseのとき絶対パス
#searchDir: ファイルを探索するディレクトリ名
#searchPattern: 検索するパターン。相対パスのとき、ルートは${searchDir}
#out: 検索結果のリスト
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
#searchDir: ファイルを探索するディレクトリ
function(FSetModuleFileFlags searchDir)
	file(GLOB ixx_files LIST_DIRECTORIES false ${searchDir}/*.ixx)
	file(GLOB cxx_files LIST_DIRECTORIES false ${searchDir}/*.cxx)
	set(module_files ${cxx_files} ${ixx_files})
	## BMIファイルを削除するためのターゲット
	if (GCC AND NOT(TARGET clean_module))
		add_custom_target(clean_module)
	endif()
	## モジュールファイル(.ixx, .cxx)ごとにコンパイルオプションを追加
	foreach(module_file ${module_files})
		cmake_path(NORMAL_PATH module_file)
		if (MSBUILD)
			set_source_files_properties(${module_file} PROPERTIES COMPILE_FLAGS "-interface -TP")
		elseif (MSVC)
			set_source_files_properties(${module_file} PROPERTIES COMPILE_FLAGS "-interface -TP -ifcOutput ${CMAKE_BINARY_DIR}\\")
		elseif (GCC)
			set_source_files_properties(${module_file} PROPERTIES COMPILE_FLAGS "-x c++")
			#BMIファイル(gcm.cache/*.gcm)をcleanで削除されるようにする
			get_filename_component(gcm_name ${module_file} NAME_WE)
			set_property(TARGET clean_module APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/gcm.cache/${gcm_name}.gcm)
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
		#出力先フォルダを作成する
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std")
		file(MAKE_DIRECTORY ${STD_OUTPUT_DIR})
		#コンパイルオプションを設定
		if (${CMAKE_CXX_STANDARD} STREQUAL 23)
			set(_MSVC_HEADER_COMPILE_OPTIONS -std:c++latest)
		else()
			set(_MSVC_HEADER_COMPILE_OPTIONS -std:c++${CMAKE_CXX_STANDARD})
		endif()
		set(_MSVC_HEADER_COMPILE_OPTIONS ${_MSVC_HEADER_COMPILE_OPTIONS} -EHsc -DUNICODE -D_UNICODE -nologo -D_DLL -c -exportHeader)
		if (MSBUILD)
			set(_MSVC_HEADER_COMPILE_OPTIONS ${_MSVC_HEADER_COMPILE_OPTIONS} -D_M_FP_PRECISE)
		endif()
		set(_DEBUG_OPTION1 -D_DEBUG)
		set(_DEBUG_OPTION2)
		if (MSBUILD)
			set(_DEBUG_OPTION2 -MDd)
		endif()
		#ヘッダごとにカスタムコマンドを設定し、出力されるBMIファイル(.ifc)の絶対パスをリスト化
		foreach(stdname ${std_list})
			add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/${stdname}.ifc
				COMMAND ${CMAKE_CXX_COMPILER} $<$<CONFIG:Debug>:${_DEBUG_OPTION1}> $<$<CONFIG:Debug>:${_DEBUG_OPTION2}> ${_MSVC_HEADER_COMPILE_OPTIONS} -ifcOutput "${STD_OUTPUT_DIR}\\" -headerName:quote ${stdname}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			list(APPEND _MSVC_STD_OUTPUT_PATH_LIST ${STD_OUTPUT_DIR}/${stdname}.ifc)
			#cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
			add_compile_options("SHELL:-headerUnit:angle ${stdname}=${STD_OUTPUT_DIR}/${stdname}.ifc")
		endforeach()
		#BMIファイルに依存するターゲット"std"を追加する
		add_custom_target(std
			SOURCES ${_MSVC_STD_OUTPUT_PATH_LIST}
		)
	elseif(GCC)
		#出力先フォルダは作業ディレクトリ下のgcm.cacheフォルダ
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gcm.cache/C-/msys64/mingw64/include/c++/13.2.0")
		#コンパイルオプションを設定
		set(_GCC_HEADER_COMPILE_OPTIONS -std=gnu++${CMAKE_CXX_STANDARD} -mwindows -fmodules-ts -Mno-modules -x c++ -fmodule-header -fmodule-only -DUNICODE -D_UNICODE -c)
		#ヘッダごとにカスタムコマンドを設定し、出力されるBMIファイル(.gcm)の絶対パスをリスト化
		foreach(stdname ${std_list})
			add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/${stdname}.gcm
				COMMAND ${CMAKE_CXX_COMPILER} $<$<CONFIG:Debug>:-g> ${_GCC_HEADER_COMPILE_OPTIONS} ${STD_INCLUDE_DIR}/${stdname}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			list(APPEND _GCC_STD_OUTPUT_PATH_LIST ${STD_OUTPUT_DIR}/${stdname}.gcm)
		endforeach()
		#BMIファイルに依存するターゲット"std"を追加する
		add_custom_target(std
			SOURCES ${_GCC_STD_OUTPUT_PATH_LIST}
		)
	endif()
endfunction()

###Functions End###
