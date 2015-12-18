#pragma warning(disable : 3206)

//��`
#define MAX_BONE_MATRICES 255

//�O���[�o��
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mW;		//���[���h�s��
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir; //���C�g�̕����x�N�g��
	float4 g_fIntensity;//�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
	float4 g_vEye;		//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient; //�A���r�G���g��
	float4 g_Diffuse; //�g�U����(�F�j
	float4 g_Specular; //���ʔ���
};

cbuffer global_bones:register(b2)//�{�[���̃|�[�Y�s�񂪓���
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
	uint4  Bones : BONE_INDEX;//�{�[���̃C���f�b�N�X
	float4 Weights : BONE_WEIGHT;//�{�[���̏d��
};

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Normal;
};

//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ��@�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}

//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
Skin SkinVert(float4 Pos, float4 Normal, uint4  Bones, float4 Weights)
{
	Skin Output = (Skin)0;

	//�{�[��0
	uint iBone = Bones.x;
	float fWeight= Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Normal += fWeight * mul(Normal, (float3x3)m);

	//�{�[��1
	iBone = Bones.y;
	fWeight = Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos,m);
	Output.Normal += fWeight * mul(Normal, (float3x3)m);

	//�{�[��2
	iBone = Bones.z;
	fWeight = Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos,m);
	Output.Normal += fWeight * mul(Normal, (float3x3)m);

	//�{�[��3
	iBone = Bones.w;
	fWeight = Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos,m);
	Output.Normal += fWeight * mul(Normal, (float3x3)m);

	return Output;
}

//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 tex : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	Skin vSkinned = SkinVert(Pos, Normal, Bones, Weights);

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(vSkinned.Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = normalize(mul(vSkinned.Normal, (float3x3)g_mW));

	//�f�B���N�V���i�����C�g
	output.Light = g_vLightDir;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld);

	output.Tex = tex; //�e�N�X�`���i�[
	return output;
}

//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS_NoTeX(float4 Pos : POSITION, float4 Normal : NORMAL, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	Skin vSkinned = SkinVert(Pos, Normal, Bones, Weights);

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(vSkinned.Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = normalize(mul(vSkinned.Normal, (float3x3)g_mW));

	//�f�B���N�V���i�����C�g
	output.Light = g_vLightDir;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld);

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse = g_Diffuse * NL;

	//���ʔ��ˌ��@��
	float3 reflect = normalize(2 * NL * input.Normal - input.Light);
	float4 specular = pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 matrial = ambient + diffuse + specular;
	float4 color = g_texColor.Sample(g_samLinear, input.Tex);
	color.rgb *= matrial.rgb * g_fIntensity.rgb * g_fIntensity.w;
	color.a *= g_Diffuse.w; //�A���t�@�l���f

	return color;
}

//�s�N�Z���V�F�[�_�[
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse = (g_Diffuse / 2 + g_texColor.Sample(g_samLinear, input.Tex) / 2) * NL;

	//���ʔ��ˌ��@��
	float3 reflect = normalize(2 * NL*input.Normal - input.Light);
	float4 specular = pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 color = ambient + diffuse + specular;

	color.rgb *= g_fIntensity.rgb; //���C�g�̐F���f
	color *= g_fIntensity.w; //���C�g���x�𔽉f
	color.a = g_Diffuse.w; //�A���t�@�l���f

	return color;
}