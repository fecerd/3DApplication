﻿
FAddObjectLibrary(SmartPtrs_0
	SOURCES
		UniquePtr.ixx
	DEPENDS
		std
		Module_2
)

FAddObjectLibrary(SmartPtrs_1
	SOURCES
		SharedPtr.ixx
	DEPENDS
		SmartPtrs_0
)

FAddObjectLibrary(SmartPtrs_2
	SOURCES
		SmartPtrs.ixx
	DEPENDS
		SmartPtrs_1
)

FCombineObjectLibrary(SmartPtrs
	PRIVATE
		SmartPtrs_0
		SmartPtrs_1
		SmartPtrs_2
)
