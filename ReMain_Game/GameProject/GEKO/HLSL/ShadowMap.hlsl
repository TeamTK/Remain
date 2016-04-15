#pragma warning(disable : 3206)

//ボーン数
#define MAX_BONE_MATRICES 255

//テクスチャー
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//変換行列やライト
cbuffer WLP_Mesh : register(b0)
{
	matrix g_mWLP; //ワールド・”ライトビュー”・プロジェクションの合成
};

//変換行列やライト
cbuffer WLP_SkinMesh : register(b0)
{
	matrix g_WLP; //ワールド・”ライトビュー”・プロジェクションの合成
	matrix g_BornMatrix[MAX_BONE_MATRICES]; //ボーン行列
};

//頂点情報
struct VS_OUTPUT_DEPTH
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

//テクスチャなし頂点情報
struct VS_OUTPUT_DEPTH_NO_TEXTURE
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
};

//頂点をスキニング
float4 SkinVertex(float4 pos, uint4  bone, float4 weight)
{
	matrix matA = weight.x * g_BornMatrix[bone.x];
	matrix matB = weight.y * g_BornMatrix[bone.y];
	matrix matC = weight.z * g_BornMatrix[bone.z];
	matrix matD = weight.w * g_BornMatrix[bone.w];
	matrix mat = matA + matB + matC + matD;

	return mul(pos, mat);
}

//深度値算出
float GetDepth(float z, float w, float2 uv)
{
	//return (z / w + 1.0f) * 0.5f * g_texColor.Sample(g_samLinear, uv).a;
	return z / w * g_texColor.Sample(g_samLinear, uv).a;
	//return z / w * g_texColor.Sample(g_samLinear, uv).a;
}

//深度値算出(テクスチャなし)
float4 GetDepth_NoTexture(float z, float w)
{
	float color;
	color.r = z / w;
	return float4(color.r, 0.0f, 0.0f, 1.0f);
}

/******************************************/
//メッシュ用
/******************************************/

VS_OUTPUT_DEPTH VS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;

	output.pos = mul(pos, g_mWLP);
	output.depth = output.pos;
	output.uv = uv;
	return output;
}

float4 PS(VS_OUTPUT_DEPTH input) : SV_Target
{
	return GetDepth(input.depth.z, input.depth.w, input.uv);
}

/******************************************/
//メッシュ用（テクスチャなし）
/******************************************/

VS_OUTPUT_DEPTH_NO_TEXTURE VS_NoTexture(float4 pos : POSITION)
{
	VS_OUTPUT_DEPTH_NO_TEXTURE output = (VS_OUTPUT_DEPTH_NO_TEXTURE)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.pos = mul(pos, g_mWLP);
	output.depth = output.pos;
	return output;
}

float4 PS_NoTexture(VS_OUTPUT_DEPTH_NO_TEXTURE input) : SV_Target
{
	return GetDepth_NoTexture(input.depth.z, input.depth.w);
}

/******************************************/
//スキンメッシュ用
/******************************************/

VS_OUTPUT_DEPTH VS_Skin(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, uint4  boneIndex : BONE_INDEX, float4 weight : BONE_WEIGHT)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;

	float4 skinPos = SkinVertex(pos, boneIndex, weight);
	
	output.pos = mul(skinPos, g_WLP);
	output.depth = output.pos;
	output.uv = uv;
	return output;
}

float4 PS_Skin(VS_OUTPUT_DEPTH input) : SV_Target
{
	return GetDepth(input.depth.z, input.depth.w, input.uv);
}