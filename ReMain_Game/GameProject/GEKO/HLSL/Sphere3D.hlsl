//変換行列や色
cbuffer global_0 : register(b0)
{
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_Color; //球の色
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 Pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(Pos, g_mWVP);

	return output;
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_Color;
}