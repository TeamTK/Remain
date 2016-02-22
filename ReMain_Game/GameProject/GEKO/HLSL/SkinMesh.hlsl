#pragma warning(disable : 3206)

//��`
#define MAX_BONE_MATRICES 255

//�O���[�o��
Texture2D g_texColor: register(t0);
Texture2D g_Texture: register(t2);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global_0 : register(b0)
{
	matrix g_mW;		//���[���h�s��
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir; //���C�g�̕����x�N�g��
	float4 g_Intensity;//�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
	float4 g_vEye;		//�J�����ʒu
};

cbuffer global_1 : register(b1)
{
	float4 g_Ambient; //�A���r�G���g��
	float4 g_Diffuse; //�g�U����(�F�j
	float4 g_Specular; //���ʔ���
};

cbuffer global_bones : register(b2)//�{�[���̃|�[�Y�s�񂪓���
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
	float2 uv : TEXCOORD3;
	//uint4  Bones : BONE_INDEX;//�{�[���̃C���f�b�N�X
	//float4 Weights : BONE_WEIGHT;//�{�[���̏d��
};

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Normal;
};

//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
Skin SkinVert(float4 Pos, float4 Normal, uint4  Bones, float4 Weights)
{
	Skin Output = (Skin)0;

	matrix matA = Weights.x * g_mConstBoneWorld[Bones.x];
	matrix matB = Weights.y * g_mConstBoneWorld[Bones.y];
	matrix matC = Weights.z * g_mConstBoneWorld[Bones.z];
	matrix matD = Weights.w * g_mConstBoneWorld[Bones.w];
	matrix mat = matA + matB + matC + matD;

	Output.Pos = mul(Pos, mat);
	Output.Normal = mul(Normal, (float3x3)mat);

	return Output;
}

//�����o�[�g����
float Lambert(float3 normal, float3 light)
{
	return max(dot(normal, light), 0.0f);
}

//�n�[�t�����o�[�g����
float HalfLambert(float NL)
{
	float Half = NL * 0.5f + 0.5f;
	return Half * Half;
}

//�t�H������
float4 Phong(float NL, float3 n, float3 l, float3 v)
{
	float3 reflect = normalize(2 * NL * n - l);
	return pow(max(dot(reflect, v), 0.0f), 4);
}

//�u�����t�H������
float4 BlinnPhong(float3 n, float3 l, float3 v)
{
	float3 h = normalize(v + l);
	return pow(max(dot(n, h), 0.0f), 4);
}

//�}�e���A���̍��v
float4 MatrialColor(float4 ambient, float4 diffuse, float4 specular, float2 uv)
{
	float4 matrial = ambient + diffuse + specular;
	float4 color;
	float4 TexColor1 = g_texColor.Sample(g_samLinear, uv);
	float4 TexColor2 = g_Texture.Sample(g_samLinear, uv);;

	color = TexColor1 * (1 - TexColor2.a) + TexColor2 * TexColor2.a;

	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //�A���t�@�l���f
	return color;
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
	output.Light = -g_vLightDir;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_vEye);

	output.uv = tex; //�e�N�X�`���i�[
	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//���ʔ��ˌ��@��
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//���f���ŏI�F�@�R�̍��̍��v
	return MatrialColor(ambient, diffuse, specular, input.uv);
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
	output.Light = -g_vLightDir;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_vEye);

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
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
	float4 color = ambient + diffuse + specular;

	color.rgb *= g_Intensity.rgb; //���C�g�̐F���f
	color *= g_Intensity.w; //���C�g���x�𔽉f
	color.a = g_Diffuse.w; //�A���t�@�l���f

	return color;
}