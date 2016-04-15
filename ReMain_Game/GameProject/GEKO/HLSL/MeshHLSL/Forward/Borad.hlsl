
Texture2D g_Depth: register(t1); 
SamplerState g_SamLinear : register(s1);

//頂点シェーダー出力構造体
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD1;
	float2 uv : TEXCOORD2;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = pos;
	output.normal = normal;
	output.uv = uv;

	return output;
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	//テクスチャから取り出す
	float depth = g_Depth.Sample(g_SamLinear, input.uv).r;

	return float4(depth, depth, depth, 1.0f);
}
