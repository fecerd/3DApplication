export module DirectXResource;
import System;
export import ResourcePath;
using namespace System;

export namespace ResourcePaths::DirectX {
	constexpr CString DefaultVS = uR"(.\HLSL\DefaultVS.hlsl)";
	constexpr CString DefaultPS = uR"(.\HLSL\DefaultPS.hlsl)";
	constexpr CString DefaultZPrepassVS = uR"(.\HLSL\Default\Z_PrepassVS.hlsl)";
	constexpr CString DefaultClusteredShadingVS = uR"(.\HLSL\Default\ClusteredShadingVS.hlsl)";
	constexpr CString DefaultClusteredShadingPS = uR"(.\HLSL\Default\ClusteredShadingPS.hlsl)";
	constexpr CString DefaultVideoPS = uR"(.\HLSL\VideoPS.hlsl)";
}
