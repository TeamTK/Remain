#ifndef _STATIC_MESH_SHADER_H_
#define _STATIC_MESH_SHADER_H_

#include "..\..\System\Direct3D11.h"

struct ConstantBuffer2
{

};

//�V�F�[�_�[�ɓn���p(�ϊ��s��Ȃ�)
struct ConstantBuffer0
{
	D3DXMATRIX  mW;			//���[���h�s��
	D3DXMATRIX  mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXMATRIX  mWLP;		//���C�g���猩���ϊ��s��
	D3DXVECTOR4 LightDir;	//���C�g����
	D3DXVECTOR4 fIntensity; //���C�g�̖��邳
	D3DXVECTOR4 eyePos;		//�J�����ʒu
};

//�V�F�[�_�[�ɓn���p�i�}�e���A���j
struct ConstantBuffer1
{
	D3DXVECTOR4 ambient;	//�A���r�G���g��
	D3DXVECTOR4 diffuse;	//�f�B�t���[�Y�F
	D3DXVECTOR4 specular;	//���ʔ���
};

class StaticMeshShader
{
public:
	~StaticMeshShader();
	static StaticMeshShader* GetInstance();
	bool Init();
	void BaseConstantBuffer(ID3D11DeviceContext *pDeviceContext, const Matrix &mat, bool isShadow);
	void MaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, D3DXVECTOR4 &diffuse, D3DXVECTOR4 &specular, D3DXVECTOR4 &ambient);
	void SetVertexShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
	void SetPixelShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
	void SetInputLayout(ID3D11DeviceContext *pDeviceContext, bool isTexture);
	//void Begin(const char* classInstanceName);
	//void End();
	void Release();	

private:
	StaticMeshShader();
	bool InitVertexShader(ID3D11Device *pDevice);
	bool InitVertexShader_NoTexture(ID3D11Device *pDevice);
	bool InitVertexShader_ShadowMap(ID3D11Device *pDevice);

	bool InitPixelShader(ID3D11Device *pDevice);
	bool InitPixelShader_NoTexture(ID3D11Device *pDevice);
	bool InitPixelShader_ShadowMap(ID3D11Device *pDevice);

private:
	ID3D11InputLayout*	 m_pVertexLayout;				//���_���C�A�E�g
	ID3D11InputLayout*	 m_pVertexLayout_NoTexture;		//�e�N�X�`���[�Ȃ����_���C�A�E�g
	ID3D11VertexShader*  m_pVertexShader;				//���_�V�F�[�_�[
	ID3D11VertexShader*  m_pVertexShader_NoTexture;		//�e�N�X�`���[�Ȃ����_�V�F�[�_�[
	ID3D11VertexShader*  m_pVertexShader_ShadowMap;		//�V���h�E�}�b�v���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader;				//�s�N�Z���V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader_NoTexture;		//�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader_ShadowMap;		//�V���h�E�}�b�v�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		 m_pConstantBuffer0;			//�R���X�^���g�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer*		 m_pMaterialConstantBuffer;		//�R���X�^���g�o�b�t�@�[�i�}�e���A���j
	//ID3D11Buffer*		 m_pConstantBuffer2;			//�R���X�^���g�o�b�t�@�[�i���I�����N�j
	//ID3D11ClassLinkage*  m_pPSClassLinkage;			//���I�V�F�[�_�[�����N
	//ID3D11ClassInstance* m_pClassInstance;			//�s�N�Z���V�F�[�_�[�ɐݒ莞�Ɏg�p����N���X�C���X�^���X
};

#endif