#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "ShaderInfo.h"
#include <vector>

struct CopyBorn;

//�V�F�[�_�[�ɑ���R���X�^���g�o�b�t�@�[���Ǘ�(StaticMesh�EDynamicMesh�̂�)
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