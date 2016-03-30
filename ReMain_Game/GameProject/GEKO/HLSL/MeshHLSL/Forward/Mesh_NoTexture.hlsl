#pragma warning(disable : 3206)

//�S�Ẵ��b�V�����ʏ��
cbuffer CommonInfo : register(b0)
{
	float4 g_LightDir;  //���C�g�̕����x�N�g��
	float4 g_Intensity; //�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
	float4 g_Eye;		//�J�����ʒu
};

//�ϊ��s��⃉�C�g
cbuffer global_0:register(b1)
{
	matrix g_mW;		//���[���h�s��
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
};

//�}�e���A��
cbuffer global_1:register(b2)
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
};

//�o�[�e�b�N�X�V�F�[�_�[
VS_OUTPUT VS_NoTexture(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������j
	output.Normal = mul(Normal, (float3x3)g_mW);

	//�f�B���N�V���i�����C�g
	output.Light = -g_LightDir.xyz;

	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float4 PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_Eye).xyz;

	return output;
}

//�s�N�Z���V�F�[�_�[
float4 PS_NoTexture( VS_OUTPUT input ) : SV_Target
{
	//�����@��
	float4 ambient = g_Ambient;

	//�g�U���ˌ��@��
	float NL = max(dot(input.Normal, input.Light), 0.0f);
	float Half = NL * 0.5f + 0.5f;
	float4 diffuse = g_Diffuse * (Half * Half);

	//���ʔ��ˌ��@��
	float3 reflect = normalize(2 * NL*input.Normal - input.Light);
	float4 specular = pow(max(dot(reflect, input.EyeVector), 0.0f), 4) * g_Specular;

	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 color = ambient + diffuse + specular;

	color.rgb *= g_Intensity.rgb; //���C�g�̐F���f
	color *= g_Intensity.w; //���C�g���x�𔽉f
	color.a = g_Diffuse.w; //�A���t�@�l���f

	return color;
}