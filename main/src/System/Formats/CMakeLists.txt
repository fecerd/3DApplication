cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Formats_0
	SOURCES
		PMD.ixx
		VMD.ixx
	DEPENDS
		Module
		IO
		Boost
)
FAddObjectLibrary(Formats_1
	SOURCES
		FBX.ixx
	DEPENDS
		Formats_0
		Compression
)
FAddObjectLibrary(Formats_2
	SOURCES
		Formats.ixx
	DEPENDS
		Formats_1
)

FCombineObjectLibrary(Formats
	PUBLIC
		Formats_0
		Formats_1
		Formats_2
)
