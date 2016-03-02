#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "..\System\Direct3D11.h"
#include "..\AnimationMesh\SkinMeshData.h"

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

class ConstantShader
{
public:
	~ConstantShader();
	static ConstantShader* GetInstance();
	bool Init();
	void SetCommonInfoConstantBuffer();
	void SetTransformMatrixConstantBuffer(ID3D11DeviceContext *pDeviceContext, Matrix &mat, bool isShadow);
	void SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, 
								   const Vector4D &diffuse, const Vector4D &specular, const Vector4D &ambient);
	void SetBornConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBorn*> &pCopyBornArray);
	void Release();

private:
	ConstantShader();

private:
	ID3D11Buffer* m_pCommonInfoConstantBuffer;		 //�R���X�^���g�o�b�t�@�[�i���ʏ��j
	ID3D11Buffer* m_pTransformMatrixConstantBuffer;  //�R���X�^���g�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer* m_pMaterialConstantBuffer;		 //�R���X�^���g�o�b�t�@�[�i�}�e���A���j
	ID3D11Buffer* m_pBornConstantBuffer;			 //�R���X�^���g�o�b�t�@�[�i�{�[���j
};

#endif