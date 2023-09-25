
struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 vpos : POSITION;
	float4 vnormal : NORMAL;
	float4 vtangent : TANGENT;
	float4 vbinormal : BINORMAL;
	float2 uv : TEXCOORD;
};
