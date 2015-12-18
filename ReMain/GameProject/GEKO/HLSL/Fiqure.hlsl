cbuffer global
{
	matrix g_W : packoffset(c0);	 //ワールドから射影までの変換行列
	float4 g_Color : packoffset(c4); //RGBA
};

//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//バーテックスシェーダー
PS_INPUT VS(float4 Pos : POSITION) 
{
	PS_INPUT Out;
	Out.Pos = mul(Pos,g_W);
	return Out;
}

//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	return g_Color;
}