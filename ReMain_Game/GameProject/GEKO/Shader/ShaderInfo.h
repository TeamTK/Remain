#ifndef _SHADER_INFO_
#define _SHADER_INFO_

#include "..\System\Direct3D11.h"

#define MAX_BONES 255
#define FORWARD_LIGHT_MAX 10
#define DEFERRED_LIGHT_MAX 100
#define SHADER_TYPE_TOTAL 2
#define SHADER_TYPE_TOTAL_VS 2
#define SHADER_TYPE_TOTAL_PS 4

//�S�Ẵ��b�V���ɋ���
struct CommonInfoConstantBuffer
{
	Vector4D lightDir;	//���C�g����
	Vector4D intensity;	//���C�g�̖��邳�ƐF
	Vector4D eyePos;	//�J�����ʒu
	Vector4D fogColor;  //�t�H�O�̐F
	ALIGN16 float fogDensity; //�t�H�O�̖��x
};

//�V�F�[�_�[�ɓn���p(�ϊ��s��Ȃ�)
struct TransformMatrixConstantBuffer
{
	Matrix mW;	 //���[���h�s��
	Matrix mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	Matrix mWLP; //���C�g���猩���ϊ��s��
};

//�V�F�[�_�[�ɓn���p�i�}�e���A���j
struct MaterialConstantBuffer
{
	Vector4D ambient;	//�A���r�G���g��
	Vector4D diffuse;	//�f�B�t���[�Y�F
	Vector4D specular;	//���ʔ���
};

//�V�F�[�_�[�ɓn���p�i�{�[���j
struct BoneConstantBuffer
{
	Matrix boneMat[MAX_BONES];
	BoneConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&boneMat[i]);
		}
	}
};

//�V�F�[�_�[�ɓn���p�i�O�������_�����O�p���C�g�j
struct ForwardLightConstantBuffer
{
	Vector4D lihgtPos[FORWARD_LIGHT_MAX];
	Vector4D range[FORWARD_LIGHT_MAX];
	ALIGN16 float lightNum;
};

//�V�F�[�_�[�ɓn���p�i��������_�����O�p���C�g�j
struct DeferredLightConstantBuffer
{
	Vector4D lihgtPos[DEFERRED_LIGHT_MAX];
	Vector4D range[DEFERRED_LIGHT_MAX];
	ALIGN16 float lightNum;
};

enum class ShaderType
{
	eNormal = 0,
	eShadow = 1,
};

enum class ShaderTypeVS
{
	eNormal = 0,
	eNormal_Shadow = 1,
};

enum class ShaderTypePS
{
	eNormal = 0,
	eNormal_Shadow = 1,
	eNormal_Fog = 2,
	eShadow_Fog = 3,
};

#endif