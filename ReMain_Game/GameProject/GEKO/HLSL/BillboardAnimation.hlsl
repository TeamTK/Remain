#pragma warning(disable : 3206)

//�e�N�X�`��
Texture2D g_Texture : register(t0);
SamplerState g_SamLinear : register(s0);

//�ϊ��s��
cbuffer global_0 : register(b1)
{
	matrix g_mWVP;
	float4 g_Color;
};

//���_�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{	
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//���_�V�F�[�_�[
VS_OUTPUT VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(pos, g_mWVP);
	output.uv = uv;

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return g_Color * g_Texture.Sample(g_SamLinear, input.uv);
}