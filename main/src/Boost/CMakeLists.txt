
FAddObjectLibrary(Boost_0
	SOURCES
		BoostThread.ixx
		BoostCoroutine.ixx
)

FCombineObjectLibrary(Boost
	PRIVATE
		Boost_0
)
