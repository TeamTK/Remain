cbuffer global
{
	matrix g_W : packoffset(c0);	 //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_Color : packoffset(c4); //RGBA
};

//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(float4 Pos : POSITION) 
{
	PS_INPUT Out;
	Out.Pos = mul(Pos,g_W);
	return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	return g_Color;
}