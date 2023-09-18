
FAddObjectLibrary(SDL_0
	SOURCES
		SDLWindowManager.ixx
	DEPENDS
		System
		Application_3	#IApplicationManager
)

FCombineObjectLibrary(SDL
	PRIVATE
		SDL_0
)
