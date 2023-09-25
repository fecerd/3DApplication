#include "DefaultHeader.hlsli"

Texture2D<float4> tex : register(t0); //テクスチャ
Texture2D<float4> sph : register(t1); //乗算スフィア
Texture2D<float4> spa : register(t2); //加算スフィア
Texture2D<float4> toon : register(t3); //トゥーンテクスチャ
SamplerState smp : register(s0); //通常サンプラ
SamplerState smpToon : register(s1); //トゥーン用サンプラ(繰り返しON)

cbuffer diff : register(b3)
{
	float4 diffuse; //物体の色(+アルファ値)
	float4 specular; //光沢の色(+表面の滑らかさ)
	float3 ambient; //環境光の色
};

//HLSLのアラインメントは16Byte
//以下の場合、float[8]{ lightColor{ r, g, b }, unused, lightPos{ r, g, b }, unused }を
//紐付ける必要がある
cbuffer light : register(b4)
{
	float3 lightColor;	//lightの色
	float3 lightDirection;	//lightの向き(正規化済み)
}

//フォンシェーディング
//color : マテリアルの色(光の色*ディフューズ*テクスチャ)
//normal : ワールド変換済み法線
//ray : 視点からこのオブジェクトへの正規化済み視線ベクトル
float4 Phong(float4 color, float3 normal, float3 ray)
{
	float3 halfVector = normalize(-lightDirection + -ray);
	float diffuseK = saturate(dot(normal, -lightDirection));
	float specularK = pow(saturate(dot(normal, halfVector)), specular.a);
	float4 toonDiffuse = toon.Sample(smpToon, float2(0, 1.0 - diffuseK));
	float3 dest = color.rgb * toonDiffuse.rgb + specular.rgb * specularK + ambient.rgb * lightColor.rgb * 0.5;
	return float4(dest, color.a);
}

struct PSOutput {
	float4 out0 : SV_TARGET0;
	float4 out1 : SV_TARGET1;
	float4 out2 : SV_TARGET2;
};

PSOutput main_func(Output input)
{
	float2 sphereMapUV = (input.vnormal.xy + float2(1, -1)) * float2(0.5, -0.5);
	float4 textureColor = tex.Sample(smp, input.uv);
	float4 sphColor = sph.Sample(smp, sphereMapUV);
	float4 spaColor = spa.Sample(smp, sphereMapUV);
	float4 color = float4(lightColor.rgb * diffuse.rgb * textureColor.rgb * sphColor.rgb + spaColor.rgb, diffuse.a * textureColor.a);
	PSOutput output;
	float3 ambientColor = ambient * (lightColor.rgb * float3(0.1, 0.1, 0.1));
	output.out0 = Phong(color, input.normal.rgb, input.ray) + float4(ambientColor, 0);
	output.out1 = color + float4(ambientColor, 0);
	output.out2 = input.vnormal;
	return output;
}

float4 Phong_test(float4 color, float3 normal, float3 ray)
{
	float3 halfVector = normalize(-lightDirection + -ray);
	float diffuseK = saturate(dot(normal, -lightDirection));
	float specularK = pow(saturate(dot(normal, halfVector)), specular.a);
	float4 toonDiffuse = toon.Sample(smpToon, float2(0, 1.0 - diffuseK));

	float4 diffuseColor = float4(color.rgb * diffuseK, color.a);
	float4 specularColor = float4(specular.rgb * specularK, 0);
	float4 ambientColor = float4(ambient.rgb * (lightColor.rgb * float3(0.1, 0.1, 0.1)), 0);

	return diffuseColor + specularColor + ambientColor;
}

PSOutput test_func(Output input)
{
	float2 sphereMapUV = (input.vnormal.xy + float2(1, -1)) * float2(0.5, -0.5);
	float4 textureColor = tex.Sample(smp, input.uv);
	float4 sphColor = sph.Sample(smp, sphereMapUV);
	float4 spaColor = spa.Sample(smp, sphereMapUV);
	float4 color = float4(lightColor.rgb * diffuse.rgb * textureColor.rgb * sphColor.rgb + spaColor.rgb, diffuse.a * textureColor.a);

	float3 halfVector = normalize(-lightDirection.xyz + -input.ray.xyz);
	float diffuseK = saturate(dot(input.normal.xyz, -lightDirection.xyz));
	float specularK = pow(saturate(dot(input.normal.xyz, halfVector)), specular.a);
	float4 toonDiffuse = toon.Sample(smpToon, float2(0, 1.0 - diffuseK));

	float4 diffuseColor = float4(color.rgb * max(toonDiffuse.r, 0.0), color.a);
	float4 specularColor = float4(specular.rgb * specularK, 0);
//	float4 ambientColor = float4(ambient.rgb * (lightColor.rgb * float3(0.1, 0.1, 0.1)), 0);
	float4 ambientColor = float4((ambient.rgb * 0.1 + color.rgb * 0.3) * lightColor.rgb, 0);

	PSOutput output;
	output.out1 = float4(max(diffuseColor.rgb + specularColor.rgb, ambientColor.rgb), color.a);
	output.out0 = diffuseColor + specularColor + ambientColor;
	output.out2 = input.vnormal;
	if (output.out0.a == 0) discard;
	output.out0.rgb *= output.out0.a;
	return output;
}

PSOutput main(Output input)
{	
	return test_func(input);
}
