#pragma warning(disable : 3206)

//�{�[����
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

//���_���
struct VS_OUTPUT_DEPTH
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

//�e�N�X�`���Ȃ����_���
struct VS_OUTPUT_DEPTH_NO_TEXTURE
{
	float4 pos : SV_POSITION;
	float4 depth : TEXCOORD0;
};

//���_���X�L�j���O
float4 SkinVertex(float4 pos, uint4  bone, float4 weight)
{
	matrix matA = weight.x * g_BornMatrix[bone.x];
	matrix matB = weight.y * g_BornMatrix[bone.y];
	matrix matC = weight.z * g_BornMatrix[bone.z];
	matrix matD = weight.w * g_BornMatrix[bone.w];
	matrix mat = matA + matB + matC + matD;

	return mul(pos, mat);
}

//�[�x�l�Z�o
float GetDepth(float z, float w, float2 uv)
{
	//return (z / w + 1.0f) * 0.5f * g_texColor.Sample(g_samLinear, uv).a;
	return z / w * g_texColor.Sample(g_samLinear, uv).a;
	//return z / w * g_texColor.Sample(g_samLinear, uv).a;
}

//�[�x�l�Z�o(�e�N�X�`���Ȃ�)
float4 GetDepth_NoTexture(float z, float w)
{
	float color;
	color.r = z / w;
	return float4(color.r, 0.0f, 0.0f, 1.0f);
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
	return GetDepth(input.depth.z, input.depth.w, input.uv);
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
	return GetDepth_NoTexture(input.depth.z, input.depth.w);
}

/******************************************/
//�X�L�����b�V���p
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