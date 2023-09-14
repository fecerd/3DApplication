
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
