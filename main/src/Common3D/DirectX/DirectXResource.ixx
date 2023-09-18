export module DirectXResource;
import System;
using namespace System;

export namespace ResourcePaths::DirectX {
	inline constexpr CString DefaultVS = uR"(.\HLSL\DefaultVS.hlsl)";
	inline constexpr CString DefaultPS = uR"(.\HLSL\DefaultPS.hlsl)";
	inline constexpr CString DefaultZPrepassVS = uR"(.\HLSL\Default\Z_PrepassVS.hlsl)";
	inline constexpr CString DefaultClusteredShadingVS = uR"(.\HLSL\Default\ClusteredShadingVS.hlsl)";
	inline constexpr CString DefaultClusteredShadingPS = uR"(.\HLSL\Default\ClusteredShadingPS.hlsl)";
	inline constexpr CString DefaultVideoPS = uR"(.\HLSL\VideoPS.hlsl)";
}
