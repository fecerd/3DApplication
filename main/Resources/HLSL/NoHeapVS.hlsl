#include "DefaultHeader.hlsli"

Output main(float4 pos : POSITION)
{
	Output ret;
	ret.svpos = pos;
	ret.normal = float4(0, 0, 0, 0);
	ret.vnormal = float4(0, 0, 0, 0);
	ret.ray = float3(0, 0, 0);
	ret.uv = float2(0, 0);
	return ret;
}