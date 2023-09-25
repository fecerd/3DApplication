#include "ClusteredShadingHeader.hlsli"

Texture2D<float4> tex : register(t0); //テクスチャ
Texture2D<float4> sph : register(t1); //乗算スフィア
Texture2D<float4> spa : register(t2); //加算スフィア
Texture2D<float4> toon : register(t3); //トゥーンテクスチャ
SamplerState smp : register(s0); //通常サンプラ
SamplerState smpToon : register(s1); //トゥーン用サンプラ(繰り返しON)

cbuffer cb3 : register(b3)
{
	float4 diffuse; //物体の色(+アルファ値)
	float4 specular; //光沢の色(+表面の滑らかさ)
	float3 ambient; //環境光の色
};

cbuffer cb4 : register(b4)
{
	float4 attenuation;
	float4 clusterSize;
}

cbuffer cb5 : register(b5)
{

}

float4 main(VSOutput input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}