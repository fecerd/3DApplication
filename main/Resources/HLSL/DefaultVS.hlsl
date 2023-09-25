#include "DefaultHeader.hlsli"

cbuffer cbuff0 : register(b0)
{
	matrix view;
	matrix proj;
	float3 eye; //World空間上のカメラ位置
}

cbuffer cbuff1 : register(b1)
{
	matrix world;
}

cbuffer cbuff2 : register(b2)
{
	matrix bones[256];
}

Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, uint2 boneno : BONENO, min16uint weight : BONEWEIGHT)
{	
	Output ret;
	float w = weight / 100.0f;
	matrix bonemat = bones[boneno[0]] * w + bones[boneno[1]] * (1 - w);
	pos = mul(bonemat, pos);
	ret.svpos = mul(world, pos);
	ret.ray = normalize(ret.svpos.xyz - eye);
	ret.svpos = mul(proj, mul(view, ret.svpos));
	normal.w = 0;
	ret.normal = mul(world, normalize(normal));
	ret.vnormal = mul(view, ret.normal);
	ret.uv = uv;
	return ret;
}