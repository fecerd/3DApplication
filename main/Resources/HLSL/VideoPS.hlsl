#include "DefaultHeader.hlsli"

Texture2D<float4> tex : register(t0); //テクスチャ
SamplerState smp : register(s0); //通常サンプラ

cbuffer diff : register(b3)
{
	float4 diffuse; //物体の色(+アルファ値)
	float4 specular; //光沢の色(+表面の滑らかさ)
	float3 ambient; //環境光の色
};

float4 main(Output input) : SV_TARGET
{
	float4 textureColor = tex.Sample(smp, input.uv);
	return float4(textureColor.rgb * diffuse.rgb, diffuse.a);
}