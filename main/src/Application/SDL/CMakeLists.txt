cmake_minimum_required(VERSION 3.8)

FAddObjectLibrary(SDL_0
	SOURCES
		SDLWindowManager.ixx
	DEPENDS
		System
		Application_3	#IApplicationManager
)

FCombineObjectLibrary(SDL
	PUBLIC
		SDL_0
)
