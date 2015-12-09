#ifndef _MESHDATA_H_
#define _MESHDATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

//�V�F�[�_�[�ɓn���p(�ϊ��s��Ȃ�)
struct ConstantBuffer0
{
	D3DXMATRIX  mW;			//���[���h�s��
	D3DXMATRIX  mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 vLightDir;	//���C�g����
	D3DXVECTOR4 fIntensity; //���C�g�̖��邳
	D3DXVECTOR4 vEye;		//�J�����ʒu
};

//�V�F�[�_�[�ɓn���p�i�}�e���A���j
struct ConstantBuffer1
{
	D3DXVECTOR4 vAmbient;	//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;	//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;	//���ʔ���
};

//�}�e���A���\����
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;	//�f�B�t���[�Y
	Vector3D Specular;	//�X�y�L�����[
	Vector3D Ambient;	//�A���r�G���g
	std::string TextureName;//�e�N�X�`���[�t�@�C����
	int dwNumFace;		//���̃}�e���A���ł���|���S����
	MaterialInfo()
	{
		//ZeroMemory(this, sizeof(MaterialInfo));
		pTexture = nullptr;
		dwNumFace = 0;
	}
	~MaterialInfo()
	{
		//SAFE_RELEASE(pTexture);
	}
};

/*
struct Material
{
	D3DXVECTOR4 Ambient;	//�A���r�G���g
	D3DXVECTOR4 Diffuse;	//�f�B�t���[�Y
	D3DXVECTOR4 Specular;	//�X�y�L�����[

	Material(D3DXVECTOR4 &Diffuse, D3DXVECTOR4 &Specular, D3DXVECTOR4 &Ambient) :
		Diffuse(Diffuse),
		Specular(Specular),
		Ambient(Ambient)
	{
	}
};

struct TextureInfo
{
	std::string TextureName;//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	TextureInfo(const std::string &TextureName) : 
		TextureName(TextureName)
	{
	}
	
};
*/

//���_�̍\����
struct VertexInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
};

//���b�V���̏��
struct MeshInfo
{
	ID3D11InputLayout*	 m_pVertexLayout;	 //���_���C�A�E�g
	ID3D11VertexShader*  m_pVertexShader;	 //���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader;	 //�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		 m_pConstantBuffer0; //�R���X�^���g�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer*		 m_pConstantBuffer1; //�R���X�^���g�o�b�t�@�[�i�}�e���A���j
	ID3D11Buffer*		 m_pVertexBuffer;	 //���_�o�b�t�@�[
	ID3D11Buffer**		 m_ppIndexBuffer;	 //�C���f�b�N�X�o�b�t�@�[
	ID3D11SamplerState*  m_pSampleLinear;	 //�e�N�X�`���[�̃T���v���[
	MaterialInfo*		 m_pMaterial;		 //�}�e���A�����
	ID3D11ShaderResourceView* m_Nothing;     //�e�N�X�`���[���Ȃ��ꍇ�g�p
	bool m_IsTexture;						 //�e�N�X�`���[���f
	VertexInfo* pvVertex;					 //���_���
	Matrix m_LocalMat;
	int vertexNumAll; //���_��
	int faceNumAll;   //�ʂ̐�
	int normalNumAll; //�@���̐�
	int materialNumAll;  //�}�e���A���̐�
	int materialListNumAll; //�}�e���A�����X�g�̐�;
	int uvNumAll;	  //UV�̐�
	MeshInfo()
	{
		m_pVertexBuffer = nullptr;
		m_pMaterial = nullptr;
		m_IsTexture = true;
	}
	~MeshInfo()
	{
	}
};

class MeshData
{
public:
	MeshData();
	~MeshData();
	MeshInfo *GetMeshInfo();
	virtual void Relese();

protected:
	HRESULT InitShader();

protected:
	MeshInfo m_MeshInfo;

private:

};

#endif 