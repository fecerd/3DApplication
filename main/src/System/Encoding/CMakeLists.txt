﻿
FAddObjectLibrary(Encoding_0
	SOURCES
		CP932.ixx
	DEPENDS
		std
)
FAddObjectLibrary(Encoding_1
	SOURCES
		CodePoint.ixx
	DEPENDS
		Encoding_0
		Module_3
)
FAddObjectLibrary(Encoding_2
	SOURCES
		CodePage.ixx
	DEPENDS
		Encoding_1
		SmartPtrs	#UniquePtr
		Module_4	#VectorBase
)
FAddObjectLibrary(Encoding_3
	SOURCES
		Encoding.ixx
	DEPENDS
		Encoding_2
)

FCombineObjectLibrary(Encoding
	PRIVATE
		Encoding_0
		Encoding_1
		Encoding_2
		Encoding_3
)
