#pragma once
#include <cstdlib>	//clangに必要
#include <cstddef>	//clangに必要
#define __SPECSTRINGS_STRICT_LEVEL 0
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#if defined(__GNUC__)
#define _MSC_VER 1933	//msys2のみ必要
#include <d3d12sdklayers.h>
#endif
#include <d3d12.h>
#undef __nullnullterminated
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
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
