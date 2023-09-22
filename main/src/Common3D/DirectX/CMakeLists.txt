
FAddObjectLibrary(DirectX_0
	SOURCES
		DXHeader.ixx
)

FAddObjectLibrary(DirectX_1
	SOURCES
		DirectXHelper.ixx
		DirectXResource.ixx
	DEPENDS
		DirectX_0
		System
)

FAddObjectLibrary(DirectX_2
	SOURCES
		DirectX.ixx
	DEPENDS
		DirectX_1
		Application
		Common3D_1 #Common3DInterface
)

FAddObjectLibrary(DirectX_3
	SOURCES
		DirectX.cpp
	DEPENDS
		DirectX_2
)

FCombineObjectLibrary(DirectX
	PRIVATE
		DirectX_0
		DirectX_1
		DirectX_2
		DirectX_3
	PUBLIC_LINK
		d3d12
		dxgi
		d3dcompiler
		dxguid
)
