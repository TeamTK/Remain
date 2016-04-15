#include "../CommonMeshInfo.hlsl"

#ifndef _FORWARD_MESH_INFO_
#define _FORWARD_MESH_INFO_

#define FORWARD_LIGHT_MAX 10

/******************************************/
//�萔�o�b�t�@�[
/******************************************/

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

/*
//�}�e���A���̍��v
float4 MatrialColor(float4 ambient, float4 diffuse, float4 specular, float2 uv)
{
	float4 matrial = ambient + diffuse + specular;
	float4 color = GetTexture(uv);

	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //�A���t�@�l���f
	return color;
}
*/

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

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�v�Z
float4 GetPixel(VS_OUTPUT input)
{
	/*
	float3 leng = g_Eye - input.worldPos;

	if (length(leng) < 5)
	{
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	*/
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
	color = ShadowMap_PS(color.a, input.LightTexCoord, color);

	return color;
}

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�v�Z�Ɩ�
float4 GetPixelFog(VS_OUTPUT input)
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

	return Fog(color, g_FogColor, length(g_Eye - input.worldPos), g_FogDensity);
}

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�Ɖe�v�Z�Ɩ�
float4 GetPixelShadowMap_Fog(VS_OUTPUT_SHADOWMAP input)
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
	color = ShadowMap_PS(color.a, input.LightTexCoord, color);

	return Fog(color, g_FogColor, length(g_Eye - input.worldPos), g_FogDensity);
}

#endif