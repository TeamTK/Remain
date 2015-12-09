cbuffer global : register(b0)
{
	matrix g_W;
	float4 g_Color;
};

cbuffer global2 : register(b1)
{
	float4 g_Pos[2];
};

//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(uint index : INDEX)
{
	PS_INPUT Out;

	Out.Pos = mul(g_Pos[index], g_W);
	return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	return g_Color;
}