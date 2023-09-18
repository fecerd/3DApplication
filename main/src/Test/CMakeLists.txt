
FAddObjectLibrary(Test_0
	SOURCES
		Col.ixx
	DEPENDS
		std
		Module_7
)

FAddObjectLibrary(Test_1
	SOURCES
		WindowsTest.ixx
	DEPENDS
		Module
		Headers
)

FCombineObjectLibrary(Test
	PRIVATE
		Test_0
		Test_1
)
