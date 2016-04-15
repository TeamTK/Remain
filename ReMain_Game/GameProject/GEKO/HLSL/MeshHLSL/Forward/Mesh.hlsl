#include "ForwardMeshInfo.hlsl"

/******************************************/
//影なしレンダリング
/******************************************/

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexOutPut(pos, normal, uv);
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	return GetPixel(input);
}

/******************************************/
//影レンダリング
/******************************************/

VS_OUTPUT_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	return GetVertexShadowMapOutPut(pos, normal, uv);
}

//ピクセルシェーダー
float4 PS_ShadowMap(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	return GetPixelShadowMap(input);
}

/******************************************/
//フォグレンダリング
/******************************************/

//ピクセルシェーダー
float4 PS_Fog(VS_OUTPUT input) : SV_Target
{
	return GetPixelFog(input);
}

//ピクセルシェーダー
float4 PS_ShadowMap_Fog(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	return GetPixelShadowMap_Fog(input);
}