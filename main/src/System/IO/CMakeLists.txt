cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(IO_0
	SOURCES
		Path.ixx
	DEPENDS
		std	#fstream
		Module_1 #Traits
		Objects	#String
)
FAddObjectLibrary(IO_1
	SOURCES
		File.ixx
		Path.cpp
		ResourcePath.ixx
	DEPENDS
		IO_0
		Module_5	#Memory
)
FAddObjectLibrary(IO_2
	SOURCES
		IO.ixx
	DEPENDS
		IO_1
)

FCombineObjectLibrary(IO
	PUBLIC
		IO_0
		IO_1
		IO_2
)
