cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(Boost_0
	SOURCES
		BoostThread.ixx
		BoostCoroutine.ixx
)

FCombineObjectLibrary(Boost
	PUBLIC
		Boost_0
)

