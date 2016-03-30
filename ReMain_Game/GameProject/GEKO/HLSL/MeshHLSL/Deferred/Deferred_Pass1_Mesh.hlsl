#include "DeferredMeshInfo.hlsl"

/******************************************/
//�e�Ȃ������_�����O
/******************************************/

VS_OUTPUT_DEFERRED VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexOutPut_Deferred(pos, normal, uv);
}

//�s�N�Z���V�F�[�_�[
PS_OUTPUT_DEFERRED PS(VS_OUTPUT_DEFERRED input)
{
	return GetPixel_Deferred(input);
}

/******************************************/
//�e�����_�����O
/******************************************/

VS_OUTPUT_DEFERRED_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexShadowMapOutPut_Deferred(pos, normal, uv);
}

//�s�N�Z���V�F�[�_�[
PS_OUTPUT_DEFERRED PS_ShadowMap(VS_OUTPUT_DEFERRED_SHADOWMAP input)
{
	return GetPixelShadowMap_Deferred(input);
}
