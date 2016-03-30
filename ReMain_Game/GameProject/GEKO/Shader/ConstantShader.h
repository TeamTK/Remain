#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "ShaderInfo.h"
#include <vector>

struct CopyBone;

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
	void SetBoneConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBone*> &pCopyBornArray);
	void SetForwardLightConstantBuffer(ID3D11DeviceContext *pDeviceContext);
	void SetDeferredLightConstantBuffer(ID3D11DeviceContext *pDeviceContext);
	void Release();

private:
	ConstantShader();

private:
	ID3D11Buffer* m_pCommonInfoConstantBuffer;		 //�萔�o�b�t�@�[�i���ʏ��j
	ID3D11Buffer* m_pTransformMatrixConstantBuffer;  //�萔�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer* m_pMaterialConstantBuffer;		 //�萔�o�b�t�@�[�i�}�e���A���j
	ID3D11Buffer* m_pBoneConstantBuffer;			 //�萔�o�b�t�@�[�i�{�[���j
	ID3D11Buffer* m_pForwardConstantBuffer;			 //�萔�o�b�t�@�[�i�O�������_�����O�p���C�g�j
	ID3D11Buffer* m_pDeferredConstantBuffer;		 //�萔�o�b�t�@�[�i��������_�����O�p���C�g�j
};

#endif