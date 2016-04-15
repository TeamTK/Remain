#pragma warning(disable : 3206)

#ifndef _COMMON_MESH_INFO_
#define _COMMON_MESH_INFO_

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
	float4 g_FogColor;  //�t�H�O�̐F
	float g_FogDensity; //�t�H�O�̖��x
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

/******************************************/
//���_���o�͗p
/******************************************/

/******************************************/
//���_�V�F�[�_�[�p�֐�
/******************************************/

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

//�V���h�E�}�b�v���f
float4 ShadowMap_PS(float alpha, float4 lightTexCoord, float4 color)
{
	//���C�g����݂�Z�l
	//float ZValue = (lightTexCoord.z / lightTexCoord.w + 1.0f) * 0.5f * alpha;
	float ZValue = lightTexCoord.z / lightTexCoord.w * alpha;

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

//�t�H�O���v�Z
float4 Fog(float4 color, float4 fogColor, float distance, float density)
{
	float e = 2.71828;
	float d = distance * density;
	float f = pow(e, -pow(d, 4));
	f = saturate(f);
	float3 c = f * color.rgb + (1 - f) * fogColor.rgb;

	return float4(c, color.a);
}

#endif