
Texture2D<float4> tex : register(t0); //テクスチャ
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

struct VSOutput {
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL;
	float3 ray : VECTOR;
	float2 uv : TEXCOORD;
};

struct PSOutput {
	float4 out0 : SV_TARGET0;
	float4 out1 : SV_TARGET1;
	float4 out2 : SV_TARGET2;
};

PSOutput test_func(VSOutput input)
{
	float4 textureColor = tex.Sample(smpToon, input.uv);
	float4 color = float4(lightColor.rgb * diffuse.rgb * textureColor.rgb, diffuse.a * textureColor.a);

	float4 normal = input.normal;

	float diffuseK = saturate(dot(normal.xyz, -lightDirection.xyz));
	float4 toonDiffuse = toon.Sample(smpToon, float2(0, 1.0 - diffuseK));

	float4 diffuseColor = float4(color.rgb * max(toonDiffuse.r, 0.0), color.a);
	float4 ambientColor = float4(color.rgb * 0.4, 0);

	PSOutput output;
	output.out0 = float4(color.rgb * toonDiffuse.r + ambientColor.rgb * (1 - toonDiffuse.r), color.a);
	output.out1 = diffuseColor + ambientColor;
	output.out2 = float4(0.f, 0.f, 0.f, 0.f);
	if (output.out0.a == 0) discard;
	output.out0.rgb *= output.out0.a;
	return output;
}

PSOutput main(VSOutput input)
{	
	return test_func(input);
}
