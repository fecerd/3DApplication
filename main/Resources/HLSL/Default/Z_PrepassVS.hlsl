
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

float4 main(float4 pos : POSITION, uint2 boneno : BONENO, min16uint weight : BONEWEIGHT) : SV_POSITION
{
	float w = weight / 100.f;
	matrix bonemat = bones[boneno[0]] * w + bones[boneno[1]] * (1 - w);
	return  mul(proj, mul(view, mul(world, mul(bonemat, pos))));
}
