#ifndef _STATIC_MESH_SHADER_H_
#define _STATIC_MESH_SHADER_H_

#include "..\System\Direct3D11.h"

class StaticMeshShader
{
public:
	~StaticMeshShader();
	static StaticMeshShader* GetInstance();
	HRESULT Init();
	void Begin();
	void End();
	void Release();

private:
	StaticMeshShader();

private:
	ID3D11InputLayout*	 m_pVertexLayout;	 //���_���C�A�E�g
	ID3D11VertexShader*  m_pVertexShader;	 //���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader;	 //�s�N�Z���V�F�[�_�[
	ID3D11ClassLinkage*  m_pPSClassLinkage;  //���I�V�F�[�_�[�����N
	ID3D11ClassInstance* m_pClassInstance;   //�s�N�Z���V�F�[�_�[�ɐݒ莞�Ɏg�p����N���X�C���X�^���X
	ID3D11Buffer*		 m_pConstantBuffer0; //�R���X�^���g�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer*		 m_pConstantBuffer1; //�R���X�^���g�o�b�t�@�[�i�}�e���A���j
	ID3D11Buffer*		 m_pConstantBuffer2; //�R���X�^���g�o�b�t�@�[�i���I�����N�j
	std::string			 m_ClassInstanceName; //�N���X�C���X�^���X��
};

#endif