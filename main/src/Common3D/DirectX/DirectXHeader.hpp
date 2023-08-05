#pragma once
#include<crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define __SPECSTRINGS_STRICT_LEVEL 0
#include<d3d12.h>
#undef __nullnullterminated
#include<dxgi1_6.h>
#include<dxgidebug.h>
#include<d3dcompiler.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#undef MessageBox
#undef MB_OK
#undef SIZE_MAX
#undef GetObject
#undef UpdateResource

inline constexpr bool Succeeded(HRESULT hr) noexcept {
	return SUCCEEDED(hr);
}
inline constexpr bool Failed(HRESULT hr) noexcept {
	return FAILED(hr);
}
