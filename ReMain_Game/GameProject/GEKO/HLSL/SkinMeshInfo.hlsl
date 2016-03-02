#ifndef _SKIN_MESH_INFO_
#define _SKIN_MESH_INFO_

//��`
#define MAX_BONE_MATRICES 255 //�{�[���̍ő吔

/******************************************/
//�R���X�^���g�o�b�t�@�[
/******************************************/

//�{�[���̍s��
cbuffer Global_Bones : register(b3)
{
	matrix g_BoneWorld[MAX_BONE_MATRICES];
};

/******************************************/
//���_�V�F�[�_�[�p�\����
/******************************************/

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Normal;
};

/******************************************/
//���_�V�F�[�_�[�p�֐�
/******************************************/

//���_���X�L�j���O
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