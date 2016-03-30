#include "../CommonMeshInfo.hlsl"

#define DEFERRED_LIGHT_MAX 100

//��������_�����O�p�_����
cbuffer Global_DeferredLight : register(b4)
{
	float4 g_DLihgtPos[DEFERRED_LIGHT_MAX];  //�_�����̈ʒu
	float4 g_DRange[DEFERRED_LIGHT_MAX];     //�_�����̔��a
	float g_DLihgtNum;						 //�_�����̐�
};

/******************************************/
//���_���o�͗p
/******************************************/

//���_�V�F�[�_�[�o�͍\����
struct VS_OUTPUT_DEFERRED
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float2 uv : TEXCOORD2;
};

//���_�V�F�[�_�[�o�͍\����(�e����)
struct VS_OUTPUT_DEFERRED_SHADOWMAP
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 lightTexCoord : TEXCOORD2;
	float2 uv : TEXCOORD3;
};

/******************************************/
//�s�N�Z���V�F�[�_�[���o�͗p
/******************************************/

//�s�N�Z���V�F�[�_�[�o�͍\���́iMRT�j
struct PS_OUTPUT_DEFERRED
{
	float4 albedo : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

/******************************************/
//���_�V�F�[�_�[�p�֐�
/******************************************/

//���_��ϊ�
VS_OUTPUT_DEFERRED GetVertexOutPut_Deferred(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;

	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;

	return output;
}

//���_��ϊ��i�e�j
VS_OUTPUT_DEFERRED_SHADOWMAP GetVertexShadowMapOutPut_Deferred(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_DEFERRED_SHADOWMAP output = (VS_OUTPUT_DEFERRED_SHADOWMAP)0;

	output.pos = mul(pos, g_mWVP);

	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;
	output.lightTexCoord = mul(pos, g_mWLP);

	return output;
}

/******************************************/
//�s�N�Z���V�F�[�_�[�p�֐�
/******************************************/

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�v�Z
PS_OUTPUT_DEFERRED GetPixel_Deferred(VS_OUTPUT_DEFERRED input)
{
	PS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;

	output.albedo = GetTexture(input.uv);
	output.normal = float4(input.normal, 1.0f);
	output.position = float4(input.worldPos, 1.0f);

	return output;
}

//�s�N�Z���V�F�[�_�[�ł̃��C�e�B���O�v�Z(�e�p)
PS_OUTPUT_DEFERRED GetPixelShadowMap_Deferred(VS_OUTPUT_DEFERRED_SHADOWMAP input)
{
	PS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;
	
	return output;
}

//��������_�����O�p�|�C���g���C�g�v�Z
float3 GetDeferredPointLight(float3 pos, float3 normal, float3 eyeVector)
{
	float3 color;     //�ŏI���ʐF
	float3 lightDir;  //���_����_�����ւ̕���
	float3 lightVec;  //�_�����̃x�N�g��
	float distance;   //���_����_�����ւ̋���
	float percentage; //�_�����̔��a�̊���
	float result;     //�v�Z����
	float attenution; //����

	//�����̓_�����v�Z
	for (int i = 0; i < g_DLihgtNum; i++)
	{
		lightVec = g_DLihgtPos[i].xyz - pos;
		lightDir = normalize(lightVec);

		//�����@��
		float4 ambient = g_Ambient;

		//�g�U���ˌ��@��
		float NL = Lambert(normal, lightDir);
		float4 diffuse = g_Diffuse * HalfLambert(NL);

		//���ʔ��ˌ��@��
		float4 specular = g_Specular * BlinnPhong(normal, lightDir, eyeVector);

		//�����v�Z
		distance = length(lightVec);
		percentage = distance / g_DRange[i].w;
		percentage = percentage * percentage * percentage * percentage;
		result = saturate(1.0f - percentage);
		result = result * result;
		attenution = result / (1.0f + (distance * distance));

		color += ((ambient + diffuse + specular).rgb * attenution * g_DLihgtPos[i].w) * g_DRange[i].rgb;
	}

	return color;
}