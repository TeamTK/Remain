#pragma warning(disable : 3206)

//テクスチャー
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//変換行列やライト
cbuffer global_0 : register(b0)
{
	matrix g_mWLP; //ワールド・”ライトビュー”・プロジェクションの合成
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT_DEPTH
{
	float4 Pos : SV_POSITION;
	//float4 Depth : POSITION;
	float4 Depth : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

struct VS_OUTPUT_DEPTH_NO_TEXTURE
{
	float4 Pos : SV_POSITION;
	float4 Depth : POSITION; //TEXCOORD0かも
};

VS_OUTPUT_DEPTH VS(float4 Pos : POSITION, float4 normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;

	output.Pos = mul(Pos, g_mWLP);
	output.Depth = output.Pos;
	output.Tex = Tex;
	return output;
}

float4 PS(VS_OUTPUT_DEPTH input) : SV_Target
{
	float color;
	color.r = input.Depth.z / input.Depth.w * g_texColor.Sample(g_samLinear, input.Tex).a;
	return color;
}

VS_OUTPUT_DEPTH_NO_TEXTURE VS_NoTexture(float4 Pos : POSITION)
{
	VS_OUTPUT_DEPTH_NO_TEXTURE output = (VS_OUTPUT_DEPTH_NO_TEXTURE)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(Pos, g_mWLP);
	output.Depth = output.Pos;
	return output;
}

float4 PS_NoTexture(VS_OUTPUT_DEPTH_NO_TEXTURE input) : SV_Target
{
	float4 color;
	color.r = input.Depth.z / input.Depth.w;
	return color;
}