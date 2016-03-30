#ifndef _SKIN_MESH_INFO_
#define _SKIN_MESH_INFO_

//定義
#define MAX_BONE_MATRICES 255 //ボーンの最大数

/******************************************/
//コンスタントバッファー
/******************************************/

//ボーンの行列
cbuffer Global_Bones : register(b3)
{
	matrix g_BoneWorld[MAX_BONE_MATRICES];
};

/******************************************/
//頂点シェーダー用構造体
/******************************************/

//スキニング後の頂点・法線が入る
struct Skin
{
	float4 Pos;
	float3 Normal;
};

/******************************************/
//頂点シェーダー用関数
/******************************************/

//頂点をスキニング
Skin SkinVertex(float4 Pos, float3 Normal, uint4  Bones, float4 Weights)
{
	Skin Output = (Skin)0;

	matrix matA = Weights.x * g_BoneWorld[Bones.x];
	matrix matB = Weights.y * g_BoneWorld[Bones.y];
	matrix matC = Weights.z * g_BoneWorld[Bones.z];
	matrix matD = Weights.w * g_BoneWorld[Bones.w];
	matrix mat = matA + matB + matC + matD;

	Output.Pos = mul(Pos, mat);
	Output.Normal = mul(Normal, (float3x3)mat);

	return Output;
}

#endif