#include "DeferredMeshInfo.hlsl"
#include "../SkinMeshInfo.hlsl"

/******************************************/
//影なしレンダリング
/******************************************/

//バーテックスシェーダー
VS_OUTPUT_DEFERRED VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexOutPut_Deferred(skinned.Pos, skinned.Normal, uv);
}

//ピクセルシェーダー
PS_OUTPUT_DEFERRED PS(VS_OUTPUT_DEFERRED input)
{
	return GetPixel_Deferred(input);
}

/******************************************/
//影レンダリング
/******************************************/

VS_OUTPUT_DEFERRED_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexShadowMapOutPut_Deferred(skinned.Pos, skinned.Normal, uv);
}

//ピクセルシェーダー
PS_OUTPUT_DEFERRED PS_ShadowMap(VS_OUTPUT_DEFERRED_SHADOWMAP input)
{
	return GetPixelShadowMap_Deferred(input);
}