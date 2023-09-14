
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
