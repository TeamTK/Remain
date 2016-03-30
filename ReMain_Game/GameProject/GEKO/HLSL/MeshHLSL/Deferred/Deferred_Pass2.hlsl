#include "DeferredMeshInfo.hlsl"

//�f�t�@�[�h�֌W
Texture2D g_Albedo : register(t3); //�A���y�h
Texture2D g_Normal : register(t3); //�@��
Texture2D g_Position : register(t3); //�ʒu

SamplerState g_SamLinear_Deferred : register(s0); //���f���̃T���v���[

//���_�V�F�[�_�[�o�͍\����
struct VS_OUTPUT_DEFERRED_PASS2
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
};

VS_OUTPUT_DEFERRED_PASS2 VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT_DEFERRED_PASS2 output = (VS_OUTPUT_DEFERRED_PASS2)0;

	output.Pos = mul(pos, g_mWVP);
	output.Normal = mul(normal, (float3x3)g_mW);
	output.Light = -g_LightDir.xyz;
	output.uv = uv;

	float4 posWorld = mul(pos, g_mW);
	output.EyeVector = normalize(g_Eye - posWorld).xyz;

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT_DEFERRED_PASS2 input) : SV_Target
{
	//�e�N�X�`��������o��
	float4 albedo = g_Albedo.Sample(g_SamLinear_Deferred, input.uv);
	float3 normal = g_Normal.Sample(g_SamLinear_Deferred, input.uv).xyz;
	float3 position = g_Position.Sample(g_SamLinear_Deferred, input.uv).xyz;

	float4 ambient = float4(g_Intensity.rgb * g_Intensity.w, 0.0f);

	float NL = Lambert(normal, input.Light);
	float4 diffuse = albedo * HalfLambert(NL);

	float4 specular = BlinnPhong(normal, input.Light, input.EyeVector);

	//�}�e���A������
	float4 matrial = ambient + diffuse + specular;

	float4 color;

	//���s�����Ɠ_�������v�Z
	float3 lightColor = GetForwardPointLight(position, normal, input.EyeVector);
	lightColor += matrial.rgb;
	color.rgb *= lightColor;

	//color.a *= g_Diffuse.w; //�A���t�@�l���f

	return color;
}
