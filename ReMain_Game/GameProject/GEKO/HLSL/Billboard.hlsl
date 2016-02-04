#pragma warning(disable : 3206)

//グローバル
Texture2D g_texColor : register(t0);
SamplerState g_samLinear : register(s0);

//グローバル
cbuffer global_0 : register(b0)
{
	matrix g_mWVP;
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD1;
};

//バーテックスシェーダー
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;

	return output;
}

//ピクセルシェーダー
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return g_texColor.Sample(g_samLinear, input.Tex);
}