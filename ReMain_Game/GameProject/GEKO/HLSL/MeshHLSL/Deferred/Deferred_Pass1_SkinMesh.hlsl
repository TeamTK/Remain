#include "DeferredMeshInfo.hlsl"
#include "../SkinMeshInfo.hlsl"

/******************************************/
//影なしレンダリング
/******************************************/

//バーテックスシェーダー
VS_OUTPUT_DEFERRED VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	Skin skinned = SkinVertex(pos, normal, Bones, Weights);

	VS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;
	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;

	return output;
}

//ピクセルシェーダー
PS_OUTPUT_DEFERRED PS(VS_OUTPUT_DEFERRED input)
{
	PS_OUTPUT_DEFERRED output = (PS_OUTPUT_DEFERRED)0;
	output.albedo = GetTexture(input.uv);
	output.normal = float4(input.normal, 1.0f);
	output.position = input.worldPos;
	return output;
}

/******************************************/
//影レンダリング
/******************************************/

VS_OUTPUT_DEFERRED_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT_DEFERRED_SHADOWMAP output = (VS_OUTPUT_DEFERRED_SHADOWMAP)0;

	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;
	output.lightTexCoord = mul(pos, g_mWLP);

	return output;
}

//ピクセルシェーダー
PS_OUTPUT_DEFERRED PS_ShadowMap(VS_OUTPUT_DEFERRED_SHADOWMAP input)
{
	PS_OUTPUT_DEFERRED output = (PS_OUTPUT_DEFERRED)0;
	output.albedo = GetTexture(input.uv);
	output.normal = float4(input.normal, 1.0f);
	output.position = float4(input.worldPos, 1.0f);
	return output;
}