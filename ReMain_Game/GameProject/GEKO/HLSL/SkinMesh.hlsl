#include "CommonMeshInfo.hlsl"
#include "SkinMeshInfo.hlsl"

/******************************************/
//影なしレンダリング
/******************************************/

//バーテックスシェーダー
VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexOutPut(skinned.Pos, skinned.Normal, uv);
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	return GetPixel(input);
}

/******************************************/
//影レンダリング
/******************************************/

VS_OUTPUT_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	return GetVertexShadowMapOutPut(skinned.Pos, skinned.Normal, uv);
}

//ピクセルシェーダー
float4 PS_ShadowMap(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	return GetPixelShadowMap(input);
}