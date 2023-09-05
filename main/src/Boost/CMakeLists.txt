cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Boost_0
	SOURCES
		Boost.ixx
)

FCombineObjectLibrary(Boost
	PUBLIC
		Boost_0
)

## GCCの場合、Boostライブラリと標準ライブラリのBMIファイルが競合するため、
## gcm.cache/C-/msys64/mingw64/include/c++/13.2.0を検索から外す必要がある。
## BMIファイルの出力先にも影響するので、gcm.cacheディレクトリ自体は参照する。
if (GCC)
	target_compile_options(Boost_0 
		PRIVATE
			SHELL:"-fmodule-mapper=|@g++-mapper-server -r ./gcm.cache"
	)
endif()
