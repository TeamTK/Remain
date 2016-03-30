#pragma warning(disable : 3206)

#ifndef _COMMON_MESH_INFO_
#define _COMMON_MESH_INFO_

#define FORWARD_LIGHT_MAX 10

/******************************************/
//�e�N�X�`�����\�[�X
/******************************************/

Texture2D g_texColor : register(t0); //���f���̃e�N�X�`��
Texture2D g_ShadowMapTexture : register(t1); //�[�x�e�N�X�`��
Texture2D g_Texture : register(t2); //�}���`�e�N�X�`��
SamplerState g_SamLinear : register(s0); //���f���̃T���v���[
SamplerState g_ShadowMapSamLinear : register(s1); //�[�x�̃T���v���[

/******************************************/
//�萔�o�b�t�@�[
/******************************************/

//�S�Ẵ��b�V�����ʏ��
cbuffer Global_CommonInfo : register(b0)
{
	float4 g_LightDir;  //���C�g�̕����x�N�g��
	float4 g_Intensity; //�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
	float4 g_Eye;		//�J�����ʒu
};

//�ϊ��s��
cbuffer Global_TransformMatrix : register(b1)
{
	matrix g_mW;	//���[���h�s��
	matrix g_mWVP;	//���[���h����ˉe�܂ł̕ϊ��s��
	matrix g_mWLP;	//���[���h �~ ���C�g�r���[ �~ ���C�g�ˉe
};

//�}�e���A��
cbuffer Global_Material : register(b2)
{
	float4 g_Ambient;  //�A���r�G���g��
	float4 g_Diffuse;  //�g�U����(�F�j
	float4 g_Specular; //���ʔ���
};

//�O�������_�����O�p�_����
cbuffer Global_ForwardLight : register(b4)
{
	float4 g_FLihgtPos[FORWARD_LIGHT_MAX];  //�_�����̈ʒu�Ɩ��邳
	float4 g_FRange[FORWARD_LIGHT_MAX];     //�_�����̔��a�ƐF
	float g_FLihgtNum;						//�_�����̐�
};

/******************************************/
//���_���o�͗p
/******************************************/

//���_�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
	float3 worldPos : TEXCOORD4;
};

//���_�V�F�[�_�[�o�͍\����(�e����)
struct VS_OUTPUT_SHADOWMAP
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float3 worldPos : TEXCOORD3;
	float2 uv : TEXCOORD4;
	float4 LightTexCoord : TEXCOORD5;
};

/******************************************/
//���_�V�F�[�_�[�p�֐�
/******************************************/

//���_��ϊ�
VS_OUTPUT GetVertexOutPut(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = mul(normal, (float3x3)g_mW);

	//�f�B���N�V���i�����C�g
	output.Light = -g_LightDir.xyz;

	//�����x�N�g��
	float4 posWorld = mul(pos, g_mW);
	output.EyeVector = normalize(g_Eye - posWorld).xyz;

	output.uv = uv;

	output.worldPos = posWorld;

	return output;
}

//���_��ϊ��i�e�j
VS_OUTPUT_SHADOWMAP GetVertexShadowMapOutPut(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_SHADOWMAP output = (VS_OUTPUT_SHADOWMAP)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = mul(normal, (float3x3)g_mW);

	//�f�B���N�V���i�����C�g
	output.Light = -g_LightDir.xyz;

	//�����x�N�g��
	float4 posWorld = mul(pos, g_mW);
	output.EyeVector = normalize(g_Eye - posWorld).xyz;

	output.worldPos = posWorld;

	output.uv = uv; //uv�i�[

	output.LightTexCoord = mul(pos, g_mWLP);

	return output;
}

/******************************************/
//�s�N�Z���V�F�[�_�[�p�֐�
/******************************************/

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

//�e�N�X�`���擾�i�}���`�Ή��j
float4 GetTexture(float2 uv)
{
	float4 TexColor1 = g_texColor.Sample(g_SamLinear, uv);
	float4 TexColor2 = g_Texture.Sample(g_SamLinear, uv);
	return TexColor1 * (1 - TexColor2.a) + TexColor2 * TexColor2.a;
}

//�}�e���A���̍��v
float4 MatrialColor(float4 ambient, float4 diffuse, float4 specular, float2 uv)
{
	float4 matrial = ambient + diffuse + specular;
	float4 color = GetTexture(uv);

	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //�A���t�@�l���f
	return color;
}

//�O�������_�����O�p�|�C���g���C�g�v�Z
float3 GetForwardPointLight(float3 pos, float3 normal, float3 eyeVector)
{
	float3 color;     //�ŏI���ʐF
	float3 lightDir;  //���_����_�����ւ̕���
	float3 lightVec;  //�_�����̃x�N�g��
	float distance;   //���_����_�����ւ̋���
	float percentage; //�_�����̔��a�̊���
	float result;     //�v�Z����
	float attenution; //����

	//�����̓_�����v�Z
	for (int i = 0; i < g_FLihgtNum; i++)
	{
		lightVec = g_FLihgtPos[i].xyz - pos;
		lightDir = normalize(lightVec);

		//�����@��
		float4 ambient = g_Ambient;

		//�g�U���ˌ��@��
		float NL = Lambert(normal, lightDir);
		float4 diffuse = g_Diffuse * HalfLambert(NL);;

		//���ʔ��ˌ��@��
		float4 specular = g_Specular * BlinnPhong(normal, lightDir, eyeVector);

		//�����v�Z
		distance = length(lightVec);
		percentage = distance / g_FRange[i].w;
		percentage = percentage * percentage * percentage * percentage;
		result = saturate(1.0f - percentage);
		result = result * result;
		attenution = result / (1.0f + (distance * distance));

		color += ((ambient + diffuse + specular).rgb * attenution * g_FLihgtPos[i].w) * g_FRange[i].rgb;
	}

	return color;
}

//�V���h�E�}�b�v���f
float4 ShadowMap_PS(float depth, float4 lightTexCoord, float4 color)
{
	//���C�g����݂�Z�l
	float ZValue = lightTexCoord.z / lightTexCoord.w;

	//�[�x�e�N�X�`�����Q�Ƃ���UV���Z�o
	float2 TexCoord;
	TexCoord.x = (lightTexCoord.x / lightTexCoord.w * 0.5f) + 0.5f;
	TexCoord.y = (-lightTexCoord.y / lightTexCoord.w * 0.5f) + 0.5f;

	float sm = g_ShadowMapTexture.Sample(g_ShadowMapSamLinear, TexCoord).r;
	if (sm <= 0.0f) return color;

	if (ZValue + 0.005f > sm)
	{
		color.rgb *= 0.5f;
	}

	return color;
}

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�v�Z
float4 GetPixel(VS_OUTPUT input)
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//���ʔ��ˌ��@��
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//���s�����ł̃}�e���A������
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//���s�����Ɠ_�������v�Z
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	color.a *= g_Diffuse.w; //�A���t�@�l���f

	return color;
}

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�Ɖe�v�Z
float4 GetPixelShadowMap(VS_OUTPUT_SHADOWMAP input)
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//���ʔ��ˌ��@��
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//���s�����ł̃}�e���A������
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//���s�����Ɠ_�������v�Z
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	//�e�t��
	color = ShadowMap_PS(input.Pos.z, input.LightTexCoord, color);

	return color;
}

#endif