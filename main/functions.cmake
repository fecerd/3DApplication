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
## 成功したとき、G_DETECT_BMI_OPTIONS変数が公開される。
## g++のみ変数STD_INCLUDE_DIRに標準ライブラリが配置されているディレクトリを設定してから呼び出す必要がある。
## 可変長引数: cstdint, iostreamなどのヘッダ名
function(FPrecompileSTD)
	if (NOT ARGC)
		return()
	endif()
	set(std_list ${ARGV})
	## 標準ライブラリの出力先フォルダを決定
	set(STD_OUTPUT_DIR "${G_STD_OUTPUT_DIR}")

	## 出力先フォルダを作成する(GCCでは自動で作られるが、作成してもよい)
	add_custom_command(OUTPUT ${STD_OUTPUT_DIR}/std_mkdir.phony
		COMMAND ${CMAKE_COMMAND} -E make_directory ${STD_OUTPUT_DIR} && ${CMAKE_COMMAND} -E touch ${STD_OUTPUT_DIR}/std_mkdir.phony
	)
	## コンパイルオプションを設定
	set(ALL_OPTIONS "${G_COMPILE_OPTIONS}" "${G_CUSTOM_INCLUDE_OPTIONS}" "${G_COMPILE_MODULE_HEADER_OPTIONS}")

	## 出力されるBMIファイルのパスのリスト
	set(BMI_OUTPUT_LIST)
	## 標準ライブラリのBMIファイルを読み込むためのコンパイルオプション
	set(DETECT_BMI_LIST)
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
				COMMAND [ ! -e ${STD_OUTPUT_DIR}/std_end.phony ] && ${CMAKE_COMMAND} -E env -- ${CMAKE_CXX_COMPILER} ${ALL_OPTIONS} ${LAST_OPTIONS} || ${CMAKE_COMMAND} -E true
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			)
			list(APPEND BMI_OUTPUT_LIST ${BMI_OUTPUT_PATH})
			if (MSBUILD OR MSVC)
				## cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
				list(APPEND DETECT_BMI_LIST "SHELL:-headerUnit:angle ${stdname}=${BMI_OUTPUT_PATH}")
			elseif (GCC)
				## g++はgcm.cacheから読み取ってくれる。(その他の場所はmodule-mapperを使用する必要がある)
			elseif (CLANG)
				## clang.exeは-fmodule-fileオプションでBMIファイルを関連付けする必要がある
				## 作業ディレクトリからの相対パスで構わない
				list(APPEND DETECT_BMI_LIST "SHELL:-fmodule-file=std/${stdname}.pcm")
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
				COMMAND ${CMAKE_CXX_COMPILER} ${ALL_OPTIONS} ${LAST_OPTIONS}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
				DEPENDS ${STD_OUTPUT_DIR}/std_mkdir.phony
			)
			list(APPEND BMI_OUTPUT_LIST ${BMI_OUTPUT_PATH})
			if (MSBUILD OR MSVC)
				## cl.exeは-headerUnitオプションでヘッダ名とBMIファイルを関連付けする必要がある
				list(APPEND DETECT_BMI_LIST "SHELL:-headerUnit:angle ${stdname}=${BMI_OUTPUT_PATH}")
			elseif (GCC)
				## g++はgcm.cacheから読み取ってくれる。(その他の場所はmodule-mapperを使用する必要がある)
			elseif (CLANG)
				## clang.exeは-fmodule-fileオプションでBMIファイルを関連付けする必要がある
				## 作業ディレクトリからの相対パスで構わない
				list(APPEND DETECT_BMI_LIST "SHELL:-fmodule-file=std/${stdname}.pcm")
			endif()
		endforeach()
		#BMIファイルに依存するターゲット"std"を追加する
		add_custom_target(std SOURCES ${BMI_OUTPUT_LIST})
	endif()
	## stdに依存する遷移用ターゲット
	add_custom_target(std_t DEPENDS std)
	## BMIファイル読み取り用のオプションを外部に見えるようにする
	set(G_DETECT_BMI_OPTIONS "${DETECT_BMI_LIST}" PARENT_SCOPE)
endfunction()

## ソースファイルにオプションを設定する
## _filename: 設定するソースファイルのパス
## 可変長引数: 設定するオプション
function(FSetFileCompileOptions _filename)
	if (ARGC LESS 2)
		return()
	endif()
	string(REPLACE ";" " " _options "${ARGN}")
	set_source_files_properties(${_filename} PROPERTIES COMPILE_FLAGS "${_options}")
endfunction()

## 依存するターゲットの持つBMIファイルの参照オプションをライブラリのコンパイルオプションに追加する。
## 成功したとき、${libname}_BMI_OPTIONS変数が公開される。
## libname: 設定するライブラリ名。すでに作成されている必要がある
## 可変長引数
##  DEPENDS [target ...]: 依存するターゲット名を指定する。存在しなくてもよい。
##  CUSTOM [target ...]: 依存するカスタムターゲット名を指定する。存在しなくてもよい。
function(FSetLibraryDepends libname)
	## 引数の解析
	cmake_parse_arguments("FAddObjectLibrary_${libname}" "" "" "DEPENDS;CUSTOM" ${ARGN})
	set(_depends "${FAddObjectLibrary_${libname}_DEPENDS}")
	set(_custom "${FAddObjectLibrary_${libname}_CUSTOM}")
	if (_custom)
		add_dependencies(${libname} ${_custom})
	endif()
	## 一つも指定されていないなら何もしない
	list(LENGTH _depends _depends_length)
	if (NOT _depends_length)
		## 関数外に変数をエクスポート
		set(${libname}_BMI_OPTIONS "${${libname}_BMI_OPTIONS}" PARENT_SCOPE)
		return()
	endif()
	## 依存するターゲットを設定する
	add_dependencies(${libname} ${_depends})
	foreach(depend ${_depends})
		## 遷移用ターゲットにも依存させる
		add_dependencies(${libname} ${depend}_t)
		## 依存するターゲットからBMIファイルの参照オプションを引き継ぐ
		if (${depend} STREQUAL std)
			list(APPEND ${libname}_BMI_OPTIONS "${G_DETECT_BMI_OPTIONS}")
		else()
			if (TARGET ${depend})
				if (DEFINED ${depend}_BMI_OPTIONS)
					list(APPEND ${libname}_BMI_OPTIONS "${${depend}_BMI_OPTIONS}")
				else()
					get_target_property(target_dir ${depend} SOURCE_DIR)
					get_directory_property(options DIRECTORY "${target_dir}" DEFINITION ${depend}_BMI_OPTIONS)
					list(APPEND ${libname}_BMI_OPTIONS "${options}")
				endif()
			else()
				list(APPEND ${libname}_BMI_OPTIONS "$<TARGET_PROPERTY:${depend},COMPILE_OPTIONS>")
			endif()
		endif()
	endforeach()
	## 重複するものを削除
	list(REMOVE_DUPLICATES ${libname}_BMI_OPTIONS)
	## コンパイルオプションを設定
	target_compile_options(${libname}
		PRIVATE
			${${libname}_BMI_OPTIONS}
	)
	## 関数外に変数をエクスポート
	set(${libname}_BMI_OPTIONS "${${libname}_BMI_OPTIONS}" PARENT_SCOPE)
endfunction()

## 引数に渡したソースファイルからオブジェクトライブラリを定義する。
## 遷移用ターゲット***_t、削除用ターゲット***_clean、依存先まで削除するターゲット***_clean_dも定義される。
## libname: 定義するライブラリ名。
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
	set(_bmi_dir "${G_BMI_OUTPUT_DIR}")

	## BMIファイル出力先ディレクトリを作成するカスタムターゲットを作成
	add_custom_command(OUTPUT ${_bmi_dir}/bmi_mkdir.phony
		COMMAND ${CMAKE_COMMAND} -E make_directory ${_bmi_dir} && ${CMAKE_COMMAND} -E touch ${_bmi_dir}/bmi_mkdir.phony
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	if (NOT TARGET bmi_mkdir)
		add_custom_target(bmi_mkdir SOURCES ${_bmi_dir}/bmi_mkdir.phony)
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
			FSetFileCompileOptions(${_source} ${G_COMPILE_MODULE_NO_PRECOMPILE_OPTIONS})
			## 以下のコマンドでvcxprojの<ModuleOutput>に追加でき、実際に出力されるが、
			## 同時に出力されるjsonファイル内のパス(\区切り)と-referenceオプションのパス(/区切り)が
			## セパレータの違いにより読み取りエラーを引き起こしてしまう。
			## デフォルトで${CMAKE_CURRENT_BINARY_DIR}下に出力されるBMIファイルをカスタムコマンドで移動することで対応する。
			## (jsonファイルを削除するという手もある)
			## set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS "-ifcOutput${_bmi_dir}/${_module_name}.ifc")
		elseif (MSVC)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}.ifc)
			FSetFileCompileOptions(${_source} ${G_COMPILE_MODULE_NO_PRECOMPILE_OPTIONS})
		 	#BMIファイル(./*.ifc)をcleanで削除されるようにする
		 	set_property(TARGET clean_module APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${_bmi_dir}/${_module_name}.ifc)
		elseif (GCC)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}.gcm)
		 	#BMIファイル(gcm.cache/*.gcm)をcleanで削除されるようにする
		 	set_property(TARGET clean_module APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${_bmi_dir}/${_module_name}.gcm)
		elseif (CLANG)
			list(APPEND _bmi_files ${_bmi_dir}/${_module_name}${_extension}.pcm)
		endif()
		list(APPEND _interface_files ${_source})
	endforeach()

	## CLANGはモジュールファイル -> BMIファイル -> オブジェクトファイルの順に2度コンパイルする必要がある
	if (CLANG AND _interface_files)
		## BMIファイルコンパイル用ターゲットを作成
		add_library(${libname}_bmi OBJECT ${_interface_files})
		## BMIファイルコンパイル用のオプションを設定
		target_compile_options(${libname}_bmi PRIVATE "${G_COMPILE_MODULE_PRECOMPILE_OPTIONS}")
		## 依存関係を設定
		FSetLibraryDepends(${libname}_bmi
			DEPENDS
				${_depends}
		)
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

	## オブジェクトライブラリを定義する
	if (CLANG)
		add_library(${libname} OBJECT
			${_bmi_files}
			${_cpp_files}
		)
		if (TARGET ${libname}_bmi_move)
			add_dependencies(${libname} ${libname}_bmi_move)
		endif()
	else()
		add_library(${libname} OBJECT
			${_interface_files}
			${_cpp_files}
		)
	endif()
	target_compile_options(${libname} PRIVATE ${G_COMPILE_NO_HEADER_OPTIONS})
	add_custom_target(${libname}_t DEPENDS ${libname})

	## ターゲット限定の削除コマンド
	add_custom_target(${libname}_clean_internal
		COMMAND ${CMAKE_COMMAND} -E rm -f $<TARGET_OBJECTS:${libname}> ${_bmi_files}
		COMMAND_EXPAND_LISTS	#これがないとgenerator expressionのリストがセミコロン区切りのstringになってしまう
	)
	add_custom_target(${libname}_clean DEPENDS ${libname}_clean_internal)
	## このターゲットに依存するターゲットも含めて削除するコマンド
	add_custom_target(${libname}_clean_d_internal DEPENDS ${libname}_clean_internal)
	add_custom_target(${libname}_clean_d DEPENDS ${libname}_clean_d_internal)
	foreach(_depend ${_depends})
		if (TARGET ${_depend} AND NOT(${_depend} STREQUAL std))
			## すでに存在するなら単純に依存関係を追加する
			add_dependencies(${_depend}_clean_d_internal ${libname}_clean_d_internal)
		else()
			## まだ定義されていない依存ターゲットはキャッシュ変数に保存しておく
			set(_tmp_list "${Cache_${_depend}_clean_depends}")
			list(APPEND _tmp_list ${libname}_clean_d_internal)
			set(Cache_${_depend}_clean_depends "${_tmp_list}" CACHE INTERNAL "")
		endif()
	endforeach()
	## キャッシュ変数が存在するなら依存関係を追加し、削除する
	if (Cache_${libname}_clean_depends)
		add_dependencies(${libname}_clean_d_internal ${Cache_${libname}_clean_depends})
		unset(Cache_${libname}_clean_depends CACHE)
	endif()

	## このライブラリが出力するBMIファイルの参照オプションをリストに追加
	if (MSVC OR MSBUILD)
		foreach(_bmi ${_bmi_files})
			get_filename_component(module_name ${_bmi} NAME_WE)
			string(REPLACE "-" ":" module_name ${module_name})
			list(APPEND ${libname}_BMI_OPTIONS "SHELL:-reference ${module_name}=${_bmi}")
			## 上述の理由により、MSBUILDではBMIファイルをカスタムコマンドで移動する
			if (MSBUILD)
				get_filename_component(module_name_ext ${_bmi} NAME)
				add_custom_command(OUTPUT ${_bmi}
					COMMAND [ -e ${CMAKE_CURRENT_BINARY_DIR}/${module_name_ext} ] && ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/${module_name_ext} ${_bmi} || echo "None"
				)
			endif()
		endforeach()
		if (MSBUILD)
			add_custom_target(${libname}_bmi_move
				SOURCES ${_bmi_files}
				DEPENDS ${libname}
			)
			add_dependencies(${libname}_t ${libname}_bmi_move)
		endif()
	elseif (CLANG)
		foreach(_bmi ${_bmi_files})
			get_filename_component(module_name ${_bmi} NAME_WE)
			string(REPLACE "-" ":" module_name ${module_name})
			list(APPEND ${libname}_BMI_OPTIONS "SHELL:-fmodule-file=${module_name}=${_bmi}")
		endforeach()
	endif()

	## 依存するライブラリの持つ参照オプションをリストに追加
	FSetLibraryDepends(${libname}
		DEPENDS
			${_depends}
		CUSTOM
			bmi_mkdir
	)
	## 関数外に変数をエクスポート
	set(${libname}_BMI_OPTIONS "${${libname}_BMI_OPTIONS}" PARENT_SCOPE)
endfunction()

## オブジェクトライブラリをまとめたスタティックライブラリを定義する
## 遷移用ターゲット***_t、削除用ターゲット***_clean、依存先まで削除するターゲット***_clean_dも定義される。
## libname: 定義するライブラリ名。
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
	## 依存するターゲットからコンパイルオプションを伝播させる(モジュール名とBMIファイル名を関連付けるオプションのため)
	FSetLibraryDepends(${libname}
		DEPENDS
			${_public} ${_private}
	)
	## 関数外に変数をエクスポート
	set(${libname}_BMI_OPTIONS "${${libname}_BMI_OPTIONS}" PARENT_SCOPE)
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

	## ターゲット限定の削除コマンド
	set(remove_args ${_public} ${_private})
	list(TRANSFORM remove_args APPEND _clean_internal)
	add_custom_target(${libname}_clean_internal
		## 単純に$<TARGET_FILE:${libname}>を使用するとそのファイルが依存関係に組み込まれてしまうため、以下のように取得する。
		COMMAND ${CMAKE_COMMAND} -E rm -f $<TARGET_FILE_PREFIX:${libname}>$<TARGET_FILE_BASE_NAME:${libname}>$<TARGET_FILE_SUFFIX:${libname}>
		## 上のコマンドはディレクトリを含まないため、WORKING_DIRECTORYを指定する必要がある。
		WORKING_DIRECTORY $<TARGET_PROPERTY:${libname},BINARY_DIR>
		COMMAND_EXPAND_LISTS
	)
	## 依存するオブジェクトライブラリの削除コマンドも呼び出す
	add_custom_target(${libname}_clean DEPENDS ${libname}_clean_internal ${remove_args})
	## このターゲットに依存するターゲットも含めて削除するコマンド
	add_custom_target(${libname}_clean_d_internal DEPENDS ${libname}_clean_internal)
	add_custom_target(${libname}_clean_d DEPENDS ${libname}_clean_d_internal)
	## キャッシュ変数が存在するなら依存関係を追加し、削除する
	if (Cache_${libname}_clean_depends)
		add_dependencies(${libname}_clean_d_internal ${Cache_${libname}_clean_depends})
		unset(Cache_${libname}_clean_depends CACHE)
	endif()

	## 依存するオブジェクトライブラリの削除用ターゲット***_clean_dにこのライブラリの削除用ターゲットを追加する
	## なお、オブジェクトライブラリはすべて同ディレクトリ内で定義済みとする
	foreach(arg ${_public} ${_private})
		add_dependencies(${arg}_clean_d_internal ${libname}_clean_d_internal)
		## 依存するオブジェクトライブラリの削除用ターゲットも呼び出す
		add_custom_target(${libname}_${arg}_clean_internal
			DEPENDS ${arg}_clean_d_internal
		)
		add_dependencies(${libname}_clean_d ${libname}_${arg}_clean_internal)
	endforeach()
endfunction()

## Boostライブラリをリンクする
## target: BoostライブラリをPUBLICリンクするターゲット名。executableのものでよい。
function(FLinkBoost target onlythread)
	## find_Boostで探せるBoostライブラリのコンポーネントリスト
	set(Boost_Component_List
		atomic
		chrono
		container
		context
		contract
		coroutine
		date_time
		exception
		fiber
		filesystem
		graph
		headers
		iostreams
		json
		locale
		log_setup
		log
		math_c99
		nowide
		numpy
		prg_exec_monitor
		program_options
		python
		random
		regex
		serialization
		stacktrace_noop
		stacktrace_windbg_cached
		stacktrace_windbg
		system
		test_exec_monitor
		thread
		timer
		type_erasure
		unit_test_framework
		url
		wave
		wserialization
	)

	set(Boost_USE_STATIC_LIBS ON) 
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_DEBUG OFF)
	## 現状はthreadさえ使えればよい
	set(Only_Thread ${onlythread})

	if (GCC)
		set(Boost_NO_SYSTEM_PATHS OFF)
		find_package(Boost 1.83.0 QUIET REQUIRED COMPONENTS ${Boost_Component_List})
		if (Only_Thread)
			target_link_libraries(${target}
				PUBLIC
					Boost::chrono
					Boost::log
					Boost::system
					Boost::thread
			)
			## 上記のコマンドでは以下のようなライブラリがリンクされるらしい(メモ)
			# target_link_libraries(${target}
			# 	PUBLIC
			# 		C:/msys64/mingw64/lib/libboost_chrono-mt.a
			# 		C:/msys64/mingw64/lib/libboost_log-mt.a
			# 		C:/msys64/mingw64/lib/libboost_system-mt.a
			# 		C:/msys64/mingw64/lib/libboost_thread-mt.a
			# 		ws2_32
			# 		C:/msys64/mingw64/lib/libboost_filesystem-mt.a
			# 		C:/msys64/mingw64/lib/libboost_atomic-mt.a
			# 		C:/msys64/mingw64/lib/libboost_regex-mt.a
			# )
		else()
			target_link_libraries(${target}
				PUBLIC
					Boost::atomic
					Boost::chrono
					Boost::container
					Boost::context
					Boost::contract
					Boost::coroutine
					Boost::date_time
					Boost::exception
					Boost::fiber
					Boost::filesystem
					Boost::graph
					Boost::headers
					Boost::iostreams
					Boost::json
					Boost::locale
					Boost::log
					Boost::log_setup
					Boost::math_c99
					Boost::nowide
					Boost::numpy
					Boost::prg_exec_monitor
					Boost::program_options
					Boost::python
					Boost::random
					Boost::regex
					Boost::serialization
					Boost::stacktrace_noop
					Boost::stacktrace_windbg_cached
					Boost::stacktrace_windbg
					Boost::system
					Boost::test_exec_monitor
					Boost::thread
					Boost::timer
					Boost::type_erasure
					Boost::unit_test_framework
					Boost::url
					Boost::wave
					Boost::wserialization
			)
		endif()
	elseif(CLANG)
		## clang64下のboostが検索できないため、現状では使用できない
	elseif(MSVC OR MSBUILD)
		## cl.exe用boostは未インストールのため、現状では使用できない
	endif()
endfunction()

## コンパイルフラグ全般を設定する
## ここで設定されたコンパイルフラグがFPrecompileSTD()やFAddObjectLibrary()内で使用される
function(FSetCompileOptions _my_std_path)
	set(STD_OUTPUT_DIR)
	set(BMI_OUTPUT_DIR)
	set(VERSION_OPTIONS)
	set(DEFINE_OPTIONS)
	set(DEBUG_OPTIONS)
	set(RELEASE_OPTIONS)
	set(MODULE_OPTIONS)	#モジュール関連追加フラグ
	set(WARNING_OPTIONS)
	set(OTHER_OPTIONS)
	set(MODULE_HEADER_OPTIONS)	#標準ライブラリモジュールコンパイル用オプション
	set(MODULE_PRECOMPILE_OPTIONS)	#インターフェースモジュールプリコンパイル用オプション
	set(MODULE_NO_PRECOMPILE_OPTIONS)	#インターフェースモジュール本コンパイル用オプション
	set(NO_HEADER_OPTIONS)	#本コンパイル用オプション(標準ライブラリを除く)
	set(INCLUDE_PATHS)

	if (MSBUILD)
		message(STATUS "MSBuild Mode")
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std/$<IF:$<CONFIG:Debug>,Debug,Release>")
		set(BMI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ifc/$<IF:$<CONFIG:Debug>,Debug,Release>")
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std:c++latest,-std:c++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -MDd)
		set(RELEASE_OPTIONS -MD)
		set(MODULE_OPTIONS)
		set(WARNING_OPTIONS)
		set(OTHER_OPTIONS -EHsc -nologo -fp:precise -bigobj)
		set(MODULE_HEADER_OPTIONS -c -exportHeader -ifcOutput "${STD_OUTPUT_DIR}\\")
		set(MODULE_PRECOMPILE_OPTIONS)
		set(MODULE_NO_PRECOMPILE_OPTIONS -interface -TP)
		set(NO_HEADER_OPTIONS)
	elseif (MSVC)
		message(STATUS "MSVC Mode")
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std")
		set(BMI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ifc")
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std:c++latest,-std:c++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -MDd)
		set(RELEASE_OPTIONS -MD)
		set(MODULE_OPTIONS)
		set(WARNING_OPTIONS)
		set(OTHER_OPTIONS -EHsc -nologo -bigobj)
		set(MODULE_HEADER_OPTIONS -c -exportHeader -ifcOutput "${STD_OUTPUT_DIR}\\")
		set(MODULE_PRECOMPILE_OPTIONS)
		set(MODULE_NO_PRECOMPILE_OPTIONS -interface -TP -ifcOutput "${BMI_OUTPUT_DIR}\\")
		set(NO_HEADER_OPTIONS)
	elseif (GCC)
		message(STATUS "GCC Mode")
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gcm.cache/C-/msys64/mingw64/include/c++/13.2.0")
		set(BMI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gcm.cache")
		set(VERSION_OPTIONS -std=c++${CMAKE_CXX_STANDARD})
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -g -D_DEBUG)
		set(MODULE_OPTIONS -fmodules-ts -Mno-modules)
		set(WARNING_OPTIONS -Wno-attributes -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-reorder -Wno-uninitialized)
		set(OTHER_OPTIONS -mwindows -x c++ -static)
		set(MODULE_HEADER_OPTIONS -fmodule-header -fmodule-only -c)
		set(MODULE_PRECOMPILE_OPTIONS)
		set(MODULE_NO_PRECOMPILE_OPTIONS)
		set(NO_HEADER_OPTIONS)
		set(INCLUDE_PATHS
			C:/msys64/mingw64/include/directx
		)
	elseif (CLANG)
		message(STATUS "CLANG Mode")
		set(STD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/std")
		set(BMI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/pcm")
		set(VERSION_OPTIONS "$<IF:$<STREQUAL:${CMAKE_CXX_STANDARD},23>,-std=c++2b,-std=c++${CMAKE_CXX_STANDARD}>")
		set(DEFINE_OPTIONS -DUNICODE -D_UNICODE)
		set(DEBUG_OPTIONS -g -D_DEBUG)
		set(MODULE_OPTIONS)
		set(WARNING_OPTIONS -Wno-ambiguous-ellipsis -Wno-pragma-system-header-outside-header -Wno-unknown-attributes -Wno-user-defined-literals -Wno-keyword-compat -Wno-unknown-warning-option -Wno-deprecated-builtins -Wno-unused-command-line-argument -Wno-nonportable-include-path -Wno-pragma-pack -Wno-ignored-attributes)
		set(OTHER_OPTIONS
			-mwindows
		)
		set(MODULE_HEADER_OPTIONS --precompile -x c++-system-header)
		set(MODULE_PRECOMPILE_OPTIONS --precompile -x c++-module)
		set(MODULE_NO_PRECOMPILE_OPTIONS)
		set(INCLUDE_PATHS
#			C:/msys64/clang64/include/directx
			C:/msys64/mingw64/include/directx
#			"\"C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.37.32822/include\""
#			"\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared\""
#			"\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um\""
#			"\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt\""
		)
	endif()
	## すべてのオプションをまとめる
	set(ALL_OPTIONS)
	set(_DEBUG_OPTIONS)
	set(_RELEASE_OPTIONS)
	foreach(op ${VERSION_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()
	foreach(op ${DEFINE_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()
	foreach(op ${DEBUG_OPTIONS})
		list(APPEND ALL_OPTIONS $<$<CONFIG:Debug>:${op}>)
		list(APPEND _DEBUG_OPTIONS $<$<CONFIG:Debug>:${op}>)
	endforeach()
	foreach(op ${RELEASE_OPTIONS})
		list(APPEND ALL_OPTIONS $<$<CONFIG:Release>:${op}>)
		list(APPEND _RELEASE_OPTIONS $<$<CONFIG:Release>:${op}>)
	endforeach()
	foreach(op ${OTHER_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()
	foreach(op ${WARNING_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()
	foreach(op ${MODULE_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()

	set(INCLUDE_OPTIONS)
	set(CUSTOM_INCLUDE_OPTIONS)
	if (EXISTS "${_my_std_path}")
		list(APPEND CUSTOM_INCLUDE_OPTIONS -I)
		list(APPEND CUSTOM_INCLUDE_OPTIONS "${_my_std_path}")
		list(APPEND INCLUDE_OPTIONS "SHELL:-I ${_my_std_path}")
	endif()
	foreach(_path ${INCLUDE_PATHS})
		list(APPEND CUSTOM_INCLUDE_OPTIONS -I)
		list(APPEND CUSTOM_INCLUDE_OPTIONS "${_path}")
		list(APPEND INCLUDE_OPTIONS "SHELL:-I ${_path}")
	endforeach()

	## VERSION, DEFINE, DEBUG, RELEASE, WARNING, OTHER, MODULEオプションがすべてのライブラリに適用される
	add_compile_options("${ALL_OPTIONS}")
	## _my_std_path, INCLUDE_PATHSがすべてのライブラリでインクルードされる
	add_compile_options("${INCLUDE_OPTIONS}")

	## すべてのライブラリに適用されているオプション(インクルード除く)
	set(G_COMPILE_OPTIONS "${ALL_OPTIONS}" PARENT_SCOPE)
	## すべてのライブラリに適用されているインクルードオプション
	set(G_INCLUDE_OPTIONS "${INCLUDE_OPTIONS}" PARENT_SCOPE)
	## 標準ライブラリモジュールの出力先ディレクトリ
	set(G_STD_OUTPUT_DIR "${STD_OUTPUT_DIR}" PARENT_SCOPE)
	## インターフェースモジュールのBMIファイルの出力先ディレクトリ
	set(G_BMI_OUTPUT_DIR "${BMI_OUTPUT_DIR}" PARENT_SCOPE)
	## 各オプションのリスト
	set(G_COMPILE_VERSION_OPTIONS "${VERSION_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_DEFINE_OPTIONS "${DEFINE_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_DEBUG_OPTIONS "${_DEBUG_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_RELEASE_OPTIONS "${_RELEASE_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_MODULE_OPTIONS "${MODULE_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_WARNING_OPTIONS "${WARNING_OPTIONS}" PARENT_SCOPE)
	set(G_COMPILE_OTHER_OPTIONS "${OTHER_OPTIONS}" PARENT_SCOPE)
	## 標準ライブラリモジュールのプリコンパイルにのみ使用されるオプション
	set(G_COMPILE_MODULE_HEADER_OPTIONS "${MODULE_HEADER_OPTIONS}" PARENT_SCOPE)
	## インターフェースモジュールのプリコンパイルにのみ使用されるオプション
	set(G_COMPILE_MODULE_PRECOMPILE_OPTIONS "${MODULE_PRECOMPILE_OPTIONS}" PARENT_SCOPE)
	## BMIファイルからのコンパイルにのみ使用されるオプション
	set(G_COMPILE_MODULE_NO_PRECOMPILE_OPTIONS "${MODULE_NO_PRECOMPILE_OPTIONS}" PARENT_SCOPE)
	## BMIファイルもしくはC++ソースファイルからのコンパイルに適用されるオプション
	set(G_COMPILE_NO_HEADER_OPTIONS "${NO_HEADER_OPTIONS}" PARENT_SCOPE)
	## カスタムコマンドで使用できるインクルードオプション。標準ライブラリモジュールのプリコンパイルに使用する
	set(G_CUSTOM_INCLUDE_OPTIONS "${CUSTOM_INCLUDE_OPTIONS}" PARENT_SCOPE)
endfunction()

## リンカフラグを設定する
function(FSetLinkerOptions)
	set(DEBUG_OPTIONS)
	set(RELEASE_OPTIONS)
	set(OTHER_OPTIONS)
	set(WINDOWS_OPTIONS)

	if (MSBUILD)

	elseif (MSVC)
		set(OTHER_OPTIONS
			-MANIFEST:NO	## MSVCではMANIFESTを無効にしないとリンクできない
		)
		set(WINDOWS_OPTIONS
			-SUBSYSTEM:WINDOWS -entry:mainCRTStartup	## サブシステムとエントリポイントの指定
		)
	elseif (GCC)
		set(DEBUG_OPTIONS -g)
		## 多重定義でリンクエラーがでる。
		## あまり得策ではないが、多重定義を無視するリンカオプションをつけて対応する。
		set(OTHER_OPTIONS -Wl,--allow-multiple-definition)
	elseif (CLANG)
		set(WINDOWS_OPTIONS
			-Wl,-subsystem,windows	##サブシステムの指定
			-Wl,--allow-multiple-definition
#			-LC:/source/vscode/3dapplication/my_std/clang
#			-LC:/msys64/clang64/lib
#			-Wl,-fuse-ld=lld
#			-nodefaultlibs
#			-Xlinker C:/msys64/clang64/bin/lld.exe
		)
	endif()

	set(ALL_OPTIONS)
	set(_DEBUG_OPTIONS)
	set(_RELEASE_OPTIONS)
	foreach(op ${DEBUG_OPTIONS})
		list(APPEND ALL_OPTIONS $<$<CONFIG:Debug>:${op}>)
		list(APPEND _DEBUG_OPTIONS $<$<CONFIG:Debug>:${op}>)
	endforeach()
	foreach(op ${RELEASE_OPTIONS})
		list(APPEND ALL_OPTIONS $<$<CONFIG:Release>:${op}>)
		list(APPEND _RELEASE_OPTIONS $<$<CONFIG:Release>:${op}>)
	endforeach()
	foreach(op ${OTHER_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()
	foreach(op ${WINDOWS_OPTIONS})
		list(APPEND ALL_OPTIONS "${op}")
	endforeach()

	add_link_options("${ALL_OPTIONS}")
	set(G_LINKER_OPTIONS "${ALL_OPTIONS}" PARENT_SCOPE)	
	set(G_LINKER_DEBUG_OPTIONS "${_DEBUG_OPTIONS}" PARENT_SCOPE)	
	set(G_LINKER_RELEASE_OPTIONS "${_RELEASE_OPTIONS}" PARENT_SCOPE)	
	set(G_LINKER_OTHER_OPTIONS "${OTHER_OPTIONS}" PARENT_SCOPE)
endfunction()

###Functions End###
