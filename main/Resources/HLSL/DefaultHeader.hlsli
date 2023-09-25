struct Output
{
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float3 ray : VECTOR;
	float2 uv : TEXCOORD;
};