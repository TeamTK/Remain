#ifndef _STATIC_MESH_SHADER_H_
#define _STATIC_MESH_SHADER_H_

#include "..\..\System\Direct3D11.h"

//StaticMesh�^�̐ÓI��3D���f���̃V�F�[�_�[���Ǘ�
class StaticMeshShader
{
public:
	~StaticMeshShader();
	static StaticMeshShader* GetInstance();
	bool Init();
	void SetShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
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
	ID3D11InputLayout*	 m_pVertexLayout;			//���_���C�A�E�g
	ID3D11InputLayout*	 m_pVertexLayout_NoTexture;	//�e�N�X�`���[�Ȃ����_���C�A�E�g
	ID3D11VertexShader*  m_pVertexShader;			//���_�V�F�[�_�[
	ID3D11VertexShader*  m_pVertexShader_NoTexture;	//�e�N�X�`���[�Ȃ����_�V�F�[�_�[
	ID3D11VertexShader*  m_pVertexShader_ShadowMap;	//�V���h�E�}�b�v���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader;			//�s�N�Z���V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader_NoTexture;	//�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader_ShadowMap;	//�V���h�E�}�b�v�s�N�Z���V�F�[�_�[
};

#endif