#include "DeferredMeshInfo.hlsl"

/******************************************/
//影なしレンダリング
/******************************************/

VS_OUTPUT_DEFERRED VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
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
	output.normal = float4(EncodeNormal(input.normal), 1.0f);
	output.position = float4(input.worldPos.xyz, input.worldPos.z / input.worldPos.w);

	return output;
}

/******************************************/
//影レンダリング
/******************************************/

VS_OUTPUT_DEFERRED_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT_DEFERRED_SHADOWMAP output = (VS_OUTPUT_DEFERRED_SHADOWMAP)0;

	output.pos = mul(pos, g_mWVP);

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

	float3 normal = input.normal * 0.5 + 0.5;
	output.normal = float4(normal, 1.0f);

	output.position = float4(input.worldPos, 1.0f);

	return output;
}
