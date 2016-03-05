#ifndef _SHADER_INFO_
#define _SHADER_INFO_

#include "..\System\Direct3D11.h"

#define MAX_BONES 255

//�S�Ẵ��b�V���ɋ���
struct CommonInfoConstantBuffer
{
	Vector4D lightDir;	//���C�g����
	Vector4D intensity;	//���C�g�̖��邳
	Vector4D eyePos;	//�J�����ʒu
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
struct BornConstantBuffer
{
	Matrix bornMat[MAX_BONES];
	BornConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&bornMat[i]);
		}
	}
};

#endif