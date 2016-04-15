#include "ForwardMeshInfo.hlsl"

/******************************************/
//�e�Ȃ������_�����O
/******************************************/

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexOutPut(pos, normal, uv);
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	return GetPixel(input);
}

/******************************************/
//�e�����_�����O
/******************************************/

VS_OUTPUT_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexShadowMapOutPut(pos, normal, uv);
}

//�s�N�Z���V�F�[�_�[
float4 PS_ShadowMap(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	return GetPixelShadowMap(input);
}

/******************************************/
//�t�H�O�����_�����O
/******************************************/

//�s�N�Z���V�F�[�_�[
float4 PS_Fog(VS_OUTPUT input) : SV_Target
{
	return GetPixelFog(input);
}

//�s�N�Z���V�F�[�_�[
float4 PS_ShadowMap_Fog(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	return GetPixelShadowMap_Fog(input);
}