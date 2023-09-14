
add_subdirectory(Module)
add_subdirectory(Encoding)
add_subdirectory(Compression)
add_subdirectory(IO)
add_subdirectory(Drawing)
add_subdirectory(Formats)
add_subdirectory(Font)

FAddObjectLibrary(System_0
	SOURCES
		System.ixx
	DEPENDS
		Compression
		Drawing
		Encoding
		Font
		Formats
		IO
		Module
)

FCombineObjectLibrary(System
	PUBLIC
		System_0
	PUBLIC_LINK
		Compression
		Drawing
		Encoding
		Font
		Formats
		IO
		Module
)
