
FAddObjectLibrary(IEnumerables_0
	SOURCES
		IEnumeratorBase.ixx
	DEPENDS
		Module_7	#Exception, Function
)

FAddObjectLibrary(IEnumerables_1
	SOURCES
		IEnumerator.ixx
	DEPENDS
		IEnumerables_0
		Boost
)

FAddObjectLibrary(IEnumerables_2
	SOURCES
		Enumerators.ixx
	DEPENDS
		IEnumerables_1
)

FAddObjectLibrary(IEnumerables_3
	SOURCES
		IEnumerable.ixx
	DEPENDS
		IEnumerables_2
)

FCombineObjectLibrary(IEnumerables
	PUBLIC
		IEnumerables_0
		IEnumerables_1
		IEnumerables_2
		IEnumerables_3
)
