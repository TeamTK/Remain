cbuffer global : register(b0)
{
	matrix g_W;
	float4 g_Color;
};

cbuffer global2 : register(b1)
{
	float4 g_Pos[2];
};

//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//バーテックスシェーダー
PS_INPUT VS(uint index : INDEX)
{
	PS_INPUT Out;

	Out.Pos = mul(g_Pos[index], g_W);
	return Out;
}

//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	return g_Color;
}