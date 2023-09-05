cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Test_0
	SOURCES
		Col.ixx
	DEPENDS
		std
		Module_7
)

FCombineObjectLibrary(Test
	PUBLIC
		Test_0
)
