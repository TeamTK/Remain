#include "ForwardMeshInfo.hlsl"
#include "../SkinMeshInfo.hlsl"

/******************************************/
//�e�Ȃ������_�����O
/******************************************/

//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexOutPut(skinned.Pos, skinned.Normal, uv);
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	return GetPixel(input);
}

/******************************************/
//�e�����_�����O
/******************************************/

VS_OUTPUT_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexShadowMapOutPut(skinned.Pos, skinned.Normal, uv);
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