
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

struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL;
	float3 ray : VECTOR;
	float2 uv : TEXCOORD;
};

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput ret;
	ret.svpos = mul(world, pos);
	ret.ray = normalize(ret.svpos.xyz - eye);
	ret.svpos = mul(proj, mul(view, ret.svpos));
	ret.uv = uv;
	ret.normal = normalize(pos + float4(0, 1, -1, 0));
	ret.normal.w = 0;
	ret.normal = mul(world, ret.normal);
	return ret;
}