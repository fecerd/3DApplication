#include "ClusteredShadingHeader.hlsli"

Texture2D<float4> tex : register(t0); //�e�N�X�`��
Texture2D<float4> sph : register(t1); //��Z�X�t�B�A
Texture2D<float4> spa : register(t2); //���Z�X�t�B�A
Texture2D<float4> toon : register(t3); //�g�D�[���e�N�X�`��
SamplerState smp : register(s0); //�ʏ�T���v��
SamplerState smpToon : register(s1); //�g�D�[���p�T���v��(�J��Ԃ�ON)

cbuffer cb3 : register(b3)
{
	float4 diffuse; //���̂̐F(+�A���t�@�l)
	float4 specular; //����̐F(+�\�ʂ̊��炩��)
	float3 ambient; //�����̐F
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