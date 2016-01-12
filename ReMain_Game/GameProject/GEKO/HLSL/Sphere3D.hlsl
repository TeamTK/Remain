//�ϊ��s���F
cbuffer global_0 : register(b0)
{
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_Color; //���̐F
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 Pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(Pos, g_mWVP);

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_Color;
}