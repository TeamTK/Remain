#pragma warning(disable : 3206)

//�e�N�X�`���[
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//�ϊ��s��⃉�C�g
cbuffer global_0:register(b0)
{
	matrix g_mW;		//���[���h�s��
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir; //���C�g�̕����x�N�g��
	float4 g_fIntensity;//�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
	float4 g_vEye;		//�J�����ʒu
};

//�}�e���A��
cbuffer global_1:register(b1)
{
	float4 g_Ambient; //�A���r�G���g��
	float4 g_Diffuse; //�g�U����(�F�j
	float4 g_Specular; //���ʔ���
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};

interface BaseColor
{
	float4 GetColor(VS_OUTPUT input);
};

BaseColor g_BaseColor;

//�t�H���V�F�[�f�B���O
class PhongShading : BaseColor
{
	float4 g_EyePos; //���_���W

	float4 GetColor(VS_OUTPUT input)
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
		float4 matrial = ambient + diffuse + specular;
		float4 color = g_texColor.Sample(g_samLinear, input.Tex);
		color.rgb *= matrial.rgb * g_fIntensity.rgb * g_fIntensity.w;
		color.a *= g_Diffuse.w; //�A���t�@�l���f

		return color;
	}
};

//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = mul(Normal, (float3x3)g_mW);

	//�f�B���N�V���i�����C�g
	output.Light = g_vLightDir.xyz;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float4 PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld).xyz;
	output.Tex = Tex;
	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_BaseColor.GetColor(input);
}