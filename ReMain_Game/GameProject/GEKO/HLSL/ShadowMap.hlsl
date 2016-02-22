#pragma warning(disable : 3206)

//��`
#define MAX_BONE_MATRICES 255

//�e�N�X�`���[
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//�ϊ��s��⃉�C�g
cbuffer WLP_Mesh : register(b0)
{
	matrix g_mWLP; //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍���
};

//�ϊ��s��⃉�C�g
cbuffer WLP_SkinMesh : register(b0)
{
	matrix g_WLP; //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍���
	matrix g_BornMatrix[MAX_BONE_MATRICES]; //�{�[���s��
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT_DEPTH
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

struct VS_OUTPUT_DEPTH_NO_TEXTURE
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
};

//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
float4 SkinVert(float4 pos, uint4  bone, float4 weight)
{
	matrix matA = weight.x * g_BornMatrix[bone.x];
	matrix matB = weight.y * g_BornMatrix[bone.y];
	matrix matC = weight.z * g_BornMatrix[bone.z];
	matrix matD = weight.w * g_BornMatrix[bone.w];
	matrix mat = matA + matB + matC + matD;

	return mul(pos, mat);
}

/******************************************/
//���b�V���p
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
	float color;
	color.r = input.depth.z / input.depth.w;
	color.r *= g_texColor.Sample(g_samLinear, input.uv).a;
	return color;
}

/******************************************/
//���b�V���p�i�e�N�X�`���Ȃ��j
/******************************************/

VS_OUTPUT_DEPTH_NO_TEXTURE VS_NoTexture(float4 pos : POSITION)
{
	VS_OUTPUT_DEPTH_NO_TEXTURE output = (VS_OUTPUT_DEPTH_NO_TEXTURE)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.pos = mul(pos, g_mWLP);
	output.depth = output.pos;
	return output;
}

float4 PS_NoTexture(VS_OUTPUT_DEPTH_NO_TEXTURE input) : SV_Target
{
	float4 color;
	color.r = input.depth.z / input.depth.w;
	return color;
}

/******************************************/
//�X�L�����b�V���p
/******************************************/

VS_OUTPUT_DEPTH VS_Skin(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, uint4  boneIndex : BONE_INDEX, float4 weight : BONE_WEIGHT)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;

	float4 skinPos = SkinVert(pos, boneIndex, weight);
	
	output.pos = mul(skinPos, g_WLP);
	output.depth = output.pos;
	output.uv = uv;
	return output;
}

float4 PS_Skin(VS_OUTPUT_DEPTH input) : SV_Target
{
	float color;
	color.r = input.depth.z / input.depth.w;
	color.r *= g_texColor.Sample(g_samLinear, input.uv).a;
	return color;
}