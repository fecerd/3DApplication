#include "ClusteredShadingHeader.hlsli"

cbuffer cb0 : register(b0)
{
	matrix view;
	matrix proj;
}

cbuffer cb1 : register(b1)
{
	matrix world;
}

cbuffer cb2 : register(b2)
{
	matrix bones[256];
}

VSOutput main(float4 pos : POSITION, float4 normal : NORMAL, float4 tangent : TANGENT, float4 binormal : BINORMAL, float2 uv : TEXCOORD, uint2 boneno : BONENO, min16uint weight : BONEWEIGHT)
{
	VSOutput ret;
	float w = weight / 100.f;
	matrix bonemat = bones[boneno[0]] * w + bones[boneno[1]] * (1 - w);
	matrix vw = mul(view, world);
	ret.vpos = mul(vw, mul(bonemat, pos));
	ret.svpos = mul(proj, ret.vpos);
	ret.vnormal = mul(vw, normal);
	ret.vtangent = mul(vw, tangent);
	ret.vbinormal = mul(vw, binormal);
	ret.uv = uv;
	return ret;
}
