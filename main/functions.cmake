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

## 可変長引数で渡した標準ライブラリヘッダをモジュールとしてコンパイルする。
## また、引数が一つ以上存在するとき、ターゲット"std"が定義される。
## 可変長引数: cstdint, iostreamなどのヘッダ名
function(FPrecompileSTD)
	if (NOT ARGC)
		return()
	endif()
	set(std_list ${ARGV})
	## 標準ライブラリの出力先フォルダを決定
	set(STD_OUTPUT_DIR)
	if (MSBUILD)
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std/$<IF:$<CONFIG:Debug>,Debug,Release>")
	elseif (MSVC)
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std")
	elseif (GCC)
		#出力先フォルダは作業ディレクトリ下のgcm.cacheフォルダ
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gcm.cache/C-/msys64/mingw64/include/c++/13.2.0")
	elseif (CLANG)
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std")
	endif()
	## 出力先フォルダを作成する(GCCでは自動で作られるが、作成してもよい)
	add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/std_mkdir.phony
		COMMAND ${CMAKE_COMMAND} -E make_directory ${STD_OUTPUT_DIR} && ${CMAKE_COMMAND} -E touch ${STD_OUTPUT_DIR}/std_mkdir.phony
	)
	## コンパイルオプションを設定
	set(VERSION_OPTIONS)
	set(DEFINE_OPTIONS)
	set(DEBUG_OPTIONS)
	set(RELEASE_OPTIONS)
	set(INCLUDE_OPTIONS -I"${CMAKE_MY_STD_PATH}")
	set(OTHER_OPTIONS)
	set(MODULE_OPTIONS)
	set(WARNING_OPTIONS)
	if (MSBUILD)
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std:c++latest,-std:c++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE "$<$<BOOL:${MSBUILD}>:-D_M_FP_PRECISE>")
		set(DEBUG_OPTIONS "$<$<BOOL:${MSBUILD}>:-MDd>")
		set(RELEASE_OPTIONS "$<$<BOOL:${MSBUILD}>:-MD>")
		set(OTHER_OPTIONS -EHsc -nologo)
		set(MODULE_OPTIONS -c -exportHeader -ifcOutput;"${STD_OUTPUT_DIR}")
		set(WARNING_OPTIONS)
	elseif (MSVC)
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std:c++latest,-std:c++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -MDd)
		set(RELEASE_OPTIONS -MD)
		set(OTHER_OPTIONS -EHsc -nologo)
		set(MODULE_OPTIONS -c -exportHeader -ifcOutput;"${STD_OUTPUT_DIR}")
		set(WARNING_OPTIONS)
	elseif (GCC)
		set(VERSION_OPTIONS -std=gnu++${CMAKE_CXX_STANDARD})
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -g)
		set(OTHER_OPTIONS -mwindows -x c++ -c)
		set(MODULE_OPTIONS -fmodules-ts -Mno-modules -fmodule-header -fmodule-only)
		set(WARNING_OPTIONS)
	elseif (CLANG)
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std=gnu++2b,-std=gnu++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -g)
		set(OTHER_OPTIONS)
		set(MODULE_OPTIONS --precompile -x c++-system-header)
		set(WARNING_OPTIONS -Wno-ambiguous-ellipsis -Wno-pragma-system-header-outside-header -Wno-unknown-attributes -Wno-user-defined-literals -Wno-keyword-compat -Wno-unknown-warning-option -Wno-deprecated-builtins)
	endif()
	## すべてのオプションをまとめる
	set(ALL_OPTIONS ${VERSION_OPTIONS} ${DEFINE_OPTIONS} $<$<CONFIG:Debug>:${DEBUG_OPTIONS}> $<$<CONFIG:Release>:${RELEASE_OPTIONS}> ${INCLUDE_OPTIONS} ${OTHER_OPTIONS} ${WARNING_OPTIONS} ${MODULE_OPTIONS})
	## 出力されるBMIファイルのパスのリスト
	set(BMI_OUTPUT_LIST)
	## 標準ライブラリをcleanターゲットから外すか決める
	set(NO_CLEAN_STD true)
	if (NO_CLEAN_STD)
		## 呼び出し用ターゲットを作成
		add_custom_target(std_internal)
		#ヘッダごとにカスタムコマンドを設定し、出力されるBMIファイルの絶対パスをリスト化
		foreach(stdname ${std_list})
			set(LAST_OPTIONS)
			set(BMI_OUTPUT_PATH)
			if (MSBUILD OR MSVC)
				set(LAST_OPTIONS -headerName:quote ${stdname})
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.ifc)
			elseif (GCC)
				set(LAST_OPTIONS ${STD_INCLUDE_DIR}/${stdname})
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.gcm)
			elseif (CLANG)
				set(LAST_OPTIONS ${stdname} -o ${STD_OUTPUT_DIR}/${stdname}.pcm)
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.pcm)
			endif()
			## ヘッダのコンパイルコマンド
			add_custom_command(TARGET std_internal POST_BUILD
				COMMAND [ ! -e ${STD_OUTPUT_DIR}/std_end.phony ] && ${CMAKE_COMMAND} -E env -- ${CMAKE_CXX_COMPILER} "$<JOIN:${ALL_OPTIONS}, >" ${LAST_OPTIONS} || ${CMAKE_COMMAND} -E true
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			list(APPEND BMI_OUTPUT_LIST ${BMI_OUTPUT_PATH})
			if (MSBUILD OR MSVC)
				## cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
				add_compile_options("SHELL:-headerUnit:angle ${stdname}=${BMI_OUTPUT_PATH}")
			elseif (GCC)
				## g++はgcm.cacheから読み取ってくれる。(その他の場所はmodule-mapperを使用する必要がある)
			elseif (CLANG)
				## clang.exeは-fmodule-fileオプションでBMIファイルを関連付けする必要がある
				## 作業ディレクトリからの相対パスで構わない
				add_compile_options("SHELL:-fmodule-file=std/${stdname}.pcm")
			endif()
		endforeach()
		## チェック用ファイルを作成
		add_custom_command(TARGET std_internal POST_BUILD
			COMMAND touch ${STD_OUTPUT_DIR}/std_end.phony
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)
		add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/std_start.phony
			COMMAND touch ${STD_OUTPUT_DIR}/std_start.phony
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS ${STD_OUTPUT_DIR}/std_mkdir.phony
		)
		set(STD_BUILD_COMMAND)
		if (MSBUILD)
			set_target_properties(std_internal PROPERTIES SOURCES ${STD_OUTPUT_DIR}/std_start.phony)
			add_custom_target(std DEPENDS std_internal)

			## 標準ライブラリのクリーン用ターゲットを作成
			add_custom_target(clean_std)
			add_custom_command(TARGET clean_std POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E rm -f ${BMI_OUTPUT_LIST} ${STD_OUTPUT_DIR}/std_start.phony ${STD_OUTPUT_DIR}/std_end.phony ${STD_OUTPUT_DIR}/std_clean.phony
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/std_clean.phony
				COMMAND touch ${STD_OUTPUT_DIR}/std_clean.phony
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			set_target_properties(clean_std PROPERTIES SOURCES ${STD_OUTPUT_DIR}/std_clean.phony)
			add_custom_target(clean_phony DEPENDS clean_std)
		else()
			set(STD_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} std_internal)
			## 標準ライブラリコンパイル用ターゲットを作成
			add_custom_target(std
				COMMAND [ ! -e ${STD_OUTPUT_DIR}/std_end.phony ] && ${STD_BUILD_COMMAND} || echo "already build std"
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
				DEPENDS ${STD_OUTPUT_DIR}/std_mkdir.phony
			)
			## 標準ライブラリのクリーン用ターゲットを作成
			add_custom_target(clean_std
				COMMAND ${CMAKE_COMMAND} -E rm -f ${BMI_OUTPUT_LIST} ${STD_OUTPUT_DIR}/std_start.phony ${STD_OUTPUT_DIR}/std_end.phony
			)
		endif()
	else()
		#ヘッダごとにカスタムコマンドを設定し、出力されるBMIファイルの絶対パスをリスト化
		foreach(stdname ${std_list})
			set(LAST_OPTIONS)
			set(BMI_OUTPUT_PATH)
			if (MSBUILD OR MSVC)
				set(LAST_OPTIONS -headerName:quote ${stdname})
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.ifc)
			elseif (GCC)
				set(LAST_OPTIONS ${STD_INCLUDE_DIR}/${stdname})
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.gcm)
			elseif (CLANG)
				set(LAST_OPTIONS ${stdname} -o ${STD_OUTPUT_DIR}/${stdname}.pcm)
				set(BMI_OUTPUT_PATH ${STD_OUTPUT_DIR}/${stdname}.pcm)
			endif()
			add_custom_command(OUTPUT ${BMI_OUTPUT_PATH}
				COMMAND ${CMAKE_CXX_COMPILER} "$<JOIN:${ALL_OPTIONS}, >" ${LAST_OPTIONS}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
				DEPENDS ${STD_OUTPUT_DIR}/std_mkdir.phony
			)
			list(APPEND BMI_OUTPUT_LIST ${BMI_OUTPUT_PATH})
			if (MSBUILD OR MSVC)
				## cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
				add_compile_options("SHELL:-headerUnit:angle ${stdname}=${BMI_OUTPUT_PATH}")
			elseif (GCC)
				## g++はgcm.cacheから読み取ってくれる。(その他の場所はmodule-mapperを使用する必要がある)
			elseif (CLANG)
				## clang.exeは-fmodule-fileオプションでBMIファイルを関連付けする必要がある
				## 作業ディレクトリからの相対パスで構わない
				add_compile_options("SHELL:-fmodule-file=std/${stdname}.pcm")
			endif()
		endforeach()
		#BMIファイルに依存するターゲット"std"を追加する
		add_custom_target(std
			SOURCES ${BMI_OUTPUT_LIST}
		)
	endif()
	#stdに依存する遷移用ターゲット(MSVCに必要)
	add_custom_target(std_t DEPENDS std)
endfunction()

## 引数に渡したソースファイルからオブジェクトライブラリを定義する
## libname: 定義するライブラリ名。遷移用ターゲット${liname}_tも定義される。
## 可変長引数:
##  SOURCES file [...]: ソースファイルのパスを指定する。呼び出し元からの相対パスでよい。一つも指定されていないとき、エラー。
##  DEPENDS [target ...]: 依存するターゲット名を指定する。存在しなくてもよい。遷移用ターゲット${target}_tにも自動的に依存する。
function(FAddObjectLibrary libname)
	## 引数チェック
	if (TARGET libname)
		message(FATAL_ERROR "すでに存在するターゲット名'${libname}'が関数FAddObjectLibraryに指定されました。")
		return()
	endif()
	## 引数の解析
	cmake_parse_arguments("FAddObjectLibrary_${libname}" "" "" "SOURCES;DEPENDS" ${ARGN})
	set(_sources "${FAddObjectLibrary_${libname}_SOURCES}")
	set(_depends "${FAddObjectLibrary_${libname}_DEPENDS}")
	## ソースファイルが1つも指定されていないならエラー
	list(LENGTH _sources _sources_length)
	if (NOT _sources_length)
		message(FATAL_ERROR "関数FAddObjectLibraryにターゲット名'${libname}'のソースファイルが一つも指定されていません。")
		return()
	endif()
	list(TRANSFORM _sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

	## BMIファイルを削除するためのターゲット
	if (NOT(TARGET clean_module))
		add_custom_target(clean_module)
	endif()

	## モジュールインターフェースファイルのリスト
	set(_interface_files)
	## その他のC++ソースファイルのリスト
	set(_cpp_files)
	## BMIファイルのリスト
	set(_bmi_files)
	## BMIファイルの出力先ディレクトリ
	set(_bmi_dir)
	if (MSBUILD)
		set(_bmi_dir "${CMAKE_BINARY_DIR}/ifc/$<IF:$<CONFIG:Debug>,Debug,Release>")
	elseif (MSVC)
		set(_bmi_dir ${CMAKE_BINARY_DIR}/ifc)
	elseif (GCC)
		set(_bmi_dir ${CMAKE_BINARY_DIR}/gcm.cache)
	elseif (CLANG)
		set(_bmi_dir ${CMAKE_BINARY_DIR}/pcm)
	endif()

	## 拡張子ixxとcxxのソースファイルをモジュールファイルとしてコンパイルされるようにする
	## _cpp_files, _interface_files, _bmi_filesはここで設定される
	foreach(_source ${_sources})
		get_filename_component(_module_name ${_source} NAME_WE)
		get_filename_component(_extension ${_source} LAST_EXT)
		## 拡張子がixxもしくはcxxでない場合、何もしない
		if (NOT (${_extension} STREQUAL .ixx) AND NOT (${_extension} STREQUAL .cxx))
			list(APPEND _cpp_files ${_source})
			continue()
		endif()
		## コンパイラごとの設定
		if (MSBUILD)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}.ifc)
			## -ifcOutputはvcxprojの<ModuleOutput>に正常に追加されるが、MSBuildが読み取らないため使用できない。
			set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS "-interface -TP -ifcOutput${_bmi_dir}/${_module_name}.ifc")
		elseif (MSVC)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}.ifc)
		 	set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS "-interface -TP -ifcOutput ${_bmi_dir}\\")
		 	#BMIファイル(./*.ifc)をcleanで削除されるようにする
		 	set_property(TARGET clean_module APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${_bmi_dir}/${_module_name}.ifc)
		elseif (GCC)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}.gcm)
			set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS "-x c++")
		 	#BMIファイル(gcm.cache/*.gcm)をcleanで削除されるようにする
		 	set_property(TARGET clean_module APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${_bmi_dir}/${_module_name}.gcm)
		elseif (CLANG)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}${_extension}.pcm)						
			set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS "-fmodules -x c++-module")
		endif()
		list(APPEND _interface_files ${_source})
	endforeach()

	## BMIファイル出力先ディレクトリを作成するカスタムターゲットを作成
	add_custom_command(OUTPUT ${_bmi_dir}/bmi_mkdir.phony
		COMMAND ${CMAKE_COMMAND} -E make_directory ${_bmi_dir} && ${CMAKE_COMMAND} -E touch ${_bmi_dir}/bmi_mkdir.phony
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	if (NOT TARGET bmi_mkdir)
		add_custom_target(bmi_mkdir SOURCES ${_bmi_dir}/bmi_mkdir.phony)
	endif()

	## CLANGはモジュールファイル -> BMIファイル -> オブジェクトファイルの順に2度コンパイルする必要がある
	if (CLANG AND _interface_files)
		## BMIファイルコンパイル用ターゲットを作成
		add_library(${libname}_bmi OBJECT ${_interface_files})
		## BMIファイルコンパイル用のオプションを設定
		target_compile_options(${libname}_bmi PRIVATE --precompile)
		## 依存関係を設定
		add_dependencies(${libname}_bmi ${_depends})
		foreach(_depend ${_depends})
			## 遷移用ターゲットにも依存させる
			if (TARGET ${_depend}_t)
				add_dependencies(${libname}_bmi ${_depend}_t)
			endif()
			## 依存するターゲットからコンパイルオプションを伝播
			target_compile_options(${libname}_bmi
				PUBLIC $<TARGET_PROPERTY:${_depend},COMPILE_OPTIONS>
			)
		endforeach()
		## BMIファイルをBMI出力ディレクトリに移動するカスタムコマンドを設定
		foreach(_source ${_interface_files})
			get_filename_component(module_name_ext ${_source} NAME)
			## コンパイルされたファイル(.obj)の実体はBMIファイル(.pcm)なので、拡張子を変更しつつBMI出力ディレクトリに移動する
			add_custom_command(OUTPUT ${_bmi_dir}/${module_name_ext}.pcm
				COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${libname}_bmi.dir/${module_name_ext}${CMAKE_CXX_OUTPUT_EXTENSION} ${_bmi_dir}/${module_name_ext}.pcm
			)
		endforeach()
		## 上記コマンドをターゲットにまとめる
		add_custom_target(${libname}_bmi_move SOURCES ${_bmi_files})
		## 移動コマンドターゲット <- BMIファイルコンパイルターゲットの依存関係を設定
		add_dependencies(${libname}_bmi_move ${libname}_bmi)
		## BMIファイルをC++ソースファイルとして認識するようにする
		foreach(_bmi ${_bmi_files})
			set_source_files_properties(${_bmi} PROPERTIES LANGUAGE CXX)
		endforeach()		
	endif()

	## オブジェクトライブラリを作成
	
	add_library(${libname} OBJECT
		${_cpp_files}
		$<IF:$<BOOL:${CLANG}>,${_bmi_files},${_interface_files}>
	)

	## CLANGでは、モジュールが使用できるようにコンパイルオプションを指定
	target_compile_options(${libname} PRIVATE $<$<BOOL:${CLANG}>:-fmodules>)
	## CLANGでは、オブジェクトライブラリ <- BMIファイル移動コマンドの依存関係が必要
	if (CLANG AND TARGET ${libname}_bmi_move)
		add_dependencies(${libname} ${libname}_bmi_move)
	endif()
	## BMI出力ディレクトリが作成されていることに依存する
	add_dependencies(${libname} bmi_mkdir)
	## 遷移用ターゲットを作成
	add_custom_target(${libname}_t DEPENDS ${libname})
	## 依存関係を設定
	add_dependencies(${libname} ${_depends})
	foreach(_depend ${_depends})
		## 遷移用ターゲットにも依存させる
		add_dependencies(${libname} ${_depend}_t)
		## 依存するターゲットからコンパイルオプションを伝播させる(モジュール名とBMIファイル名を関連付けるオプションのため)
		target_compile_options(${libname} PUBLIC $<TARGET_PROPERTY:${_depend},COMPILE_OPTIONS>)
	endforeach()

	## MSBUILDではifcOutputオプションが作動しないため、カスタムコマンドでBMI出力ディレクトリに移す必要がある
	if (MSBUILD)
		foreach(_bmi ${_bmi_files})
			get_filename_component(_module_name_ext ${_bmi} NAME)
			add_custom_command(OUTPUT ${_bmi}
				COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/${_module_name_ext} ${_bmi}
			)
		endforeach()
		add_custom_target(${libname}_bmi_move
			SOURCES ${_bmi_files}
			DEPENDS ${libname}
		)
		add_dependencies(${libname}_t ${libname}_bmi_move)
	endif()

	## モジュール名とBMIファイルを関連付けるオプションを追加する
	if (MSBUILD OR MSVC OR CLANG)
		foreach(_bmi ${_bmi_files})
			get_filename_component(_bmi_name ${_bmi} NAME_WE)
			if (CLANG)
				target_compile_options(${libname} PUBLIC "SHELL:-fmodule-file=${_bmi_name}=${_bmi}")
			else()
				target_compile_options(${libname} PUBLIC "SHELL:-reference ${_bmi_name}=${_bmi}")
			endif()
		endforeach()
	endif()
endfunction()

## オブジェクトライブラリをまとめたスタティックライブラリを定義する
## libname: 定義するライブラリ名。遷移用ターゲット${liname}_tも定義される。
## 可変長引数:
##  PUBLIC [target ...]: target_sources()に"PUBLIC $<TARGET_OBJECTS:target>"として追加するオブジェクトライブラリ名。
##  PRIVATE [target ...]: 上記のPRIVATE版。PUBLICとPRIVATE双方に一つもオブジェクトライブラリが指定されていないとき、エラー。
##  PUBLIC_LINK [target ...]: target_link_libraries()に"PUBLIC target"として追加するターゲット名。存在しなくてもよい。
##  PUBLIC_LINK [target ...]: 上記のPRIVATE版。
function(FCombineObjectLibrary libname)
	if (TARGET libname)
		message(FATAL_ERROR "すでに存在するターゲット名'${libname}'が関数FCombineObjectLibraryに指定されました。")
		return()
	endif()
	## 引数の解析
	cmake_parse_arguments("FCombineObjectLibrary_${libname}" "" "" "PUBLIC;PRIVATE;PUBLIC_LINK;PRIVATE_LINK" ${ARGN})
	set(_public "${FCombineObjectLibrary_${libname}_PUBLIC}")
	set(_private "${FCombineObjectLibrary_${libname}_PRIVATE}")
	set(_public_link "${FCombineObjectLibrary_${libname}_PUBLIC_LINK}")
	set(_private_link "${FCombineObjectLibrary_${libname}_PRIVATE_LINK}")
	## PUBLICとPRIVATE両方が指定されていない場合、エラー
	list(LENGTH _public _public_length)
	list(LENGTH _private _private_length)
	if (NOT _public_length AND NOT _private_length)
		message(FATAL_ERROR "関数FCombineObjectLibraryにターゲット名'${libname}'に統合するターゲット名が一つも指定されていません。")
		return()
	endif()
	## オブジェクトライブラリに指定されたオブジェクトファイルを追加
	add_library(${libname} STATIC)
	set(_arg_public ${_public})
	list(TRANSFORM _arg_public PREPEND "$<TARGET_OBJECTS:")
	list(TRANSFORM _arg_public APPEND ">")
	set(_arg_private ${_private})
	list(TRANSFORM _arg_private PREPEND "$<TARGET_OBJECTS:")
	list(TRANSFORM _arg_private APPEND ">")
	target_sources(${libname}
		PUBLIC ${_arg_public}
		PRIVATE ${_arg_private}
	)
	set(_targets ${_public} ${_private})
	## 指定された全てのライブラリの遷移用ターゲットに依存させる
	foreach(_target ${_targets})
		if (TARGET ${_target}_t)
			add_dependencies(${libname} ${_target}_t)
		else()
			message(STATUS "依存関係: '${libname}'<-'${_target}_t'は追加されませんでした。")
		endif()
		## 依存するターゲットからコンパイルオプションを伝播させる(モジュール名とBMIファイル名を関連付けるオプションのため)
		target_compile_options(${libname} PUBLIC $<TARGET_PROPERTY:${_target},COMPILE_OPTIONS>)
	endforeach()
	## 遷移用ターゲットを作成しておく
	add_custom_target(${libname}_t DEPENDS ${libname})
	## リンクライブラリが指定されているなら設定する
	list(LENGTH _public_link _public_link_length)
	list(LENGTH _private_link _private_link_length)
	if (_public_link_length OR _private_link_length)
		target_link_libraries(${libname}
			PUBLIC ${_public_link}
			PRIVATE ${_private_link}
		)
	endif()
endfunction()

###Functions End###
