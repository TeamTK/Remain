#ifndef _SKINMESH_DATA_H_
#define _SKINMESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

#define MAX_BONES 255

//�V�F�[�_�[�ɓn���p(�ϊ��s��Ȃ�)
struct SkinConstantBuffer0
{
	D3DXMATRIX  mW;			//���[���h�s��
	D3DXMATRIX  mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 vLightDir;	//���C�g����
	D3DXVECTOR4 fIntensity; //���C�g�̖��邳
	D3DXVECTOR4 vEye;		//�J�����ʒu
};

//�V�F�[�_�[�ɓn���p�i�}�e���A���j
struct SkinConstantBuffer1
{
	D3DXVECTOR4 vAmbient;	//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;	//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;	//���ʔ���
};

//�V�F�[�_�[�ɓn���p�i�{�[���j
struct BornConstantBuffer
{
	D3DXMATRIX bornMat[MAX_BONES];
	BornConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&bornMat[i]);
		}
	}
};

//�}�e���A���\����
struct SkinMaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;	//�f�B�t���[�Y
	Vector3D Specular;	//�X�y�L�����[
	Vector3D Ambient;	//�A���r�G���g
	std::string TextureName;//�e�N�X�`���[�t�@�C����
	int dwNumFace;		//���̃}�e���A���ł���|���S����
	SkinMaterialInfo()
	{
		pTexture = nullptr;
		dwNumFace = 0;
	}
	~SkinMaterialInfo()
	{
	}
};

//���_�̍\����
struct SkinVertexInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
	UINT BoneIndex[4];	 //�{�[���ԍ�
	float BoneWeight[4];//�{�[���d��
	SkinVertexInfo()
	{
		BoneIndex[0] = 0;
		BoneIndex[1] = 0;
		BoneIndex[2] = 0;
		BoneIndex[3] = 0;

		BoneWeight[0] = 0.0f;
		BoneWeight[1] = 0.0f;
		BoneWeight[2] = 0.0f;
		BoneWeight[3] = 0.0f;
	}
};

enum eBornDebug
{
	eInitMat,
	eOffsetMat,
	eWorld,
	eBornMat
};

//�X�L�����b�V���̏��
struct SkinMeshInfo
{
	ID3D11InputLayout*	 m_pVertexLayout;	 //���_���C�A�E�g
	ID3D11VertexShader*  m_pVertexShader;	 //���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader;	 //�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		 m_pConstantBuffer0; //�R���X�^���g�o�b�t�@�[�i�ϊ��s��j
	ID3D11Buffer*		 m_pConstantBuffer1; //�R���X�^���g�o�b�t�@�[�i�}�e���A���j
	ID3D11Buffer*		 m_pVertexBuffer;	 //���_�o�b�t�@�[
	ID3D11Buffer**		 m_ppIndexBuffer;	 //�C���f�b�N�X�o�b�t�@�[
	ID3D11Buffer*		 m_pConstantBufferBone;
	ID3D11SamplerState*  m_pSampleLinear;	 //�e�N�X�`���[�̃T���v���[
	SkinMaterialInfo*	 m_pMaterial;		 //�}�e���A�����
	ID3D11ShaderResourceView* m_Nothing;     //�e�N�X�`���[���Ȃ��ꍇ�g�p
	DWORD				 m_dwNumMaterial;	 //�}�e���A���̐�
	bool m_IsTexture;						 //�e�N�X�`���[���f
	SkinVertexInfo* pvVertex;				 //���_���
	BornConstantBuffer BornBuffer;
	int vertexNumAll; //���_��
	int faceNumAll;   //�ʂ̐�
	int normalNumAll; //�@���̐�
	int materialNumAll;  //�}�e���A���̐�
	int materialListNumAll; //�}�e���A�����X�g�̐�;
	int uvNumAll;	  //UV�̐�
	SkinMeshInfo()
	{
		m_pVertexBuffer = nullptr;
		m_pMaterial = nullptr;
	}
	~SkinMeshInfo()
	{
	}
};

//�{�[��
struct Born
{
	int indexId;
	Born *brother;
	Born *child;
	std::string BornName;
	D3DXMATRIX initMat;
	D3DXMATRIX offsetMat;
	D3DXMATRIX worldMat;
	D3DXMATRIX bornMat;
	D3DXMATRIX ParentAndChildMat;
	Born()
	{
		brother = nullptr;
		child = nullptr;
	}
};

//�{�[�����
struct BornInfo
{
	Born sBorn;		  //�{�[���K�w�\��
	std::vector<Born *> BornList; //�{�[���A�h���X�i�[
	std::map<int, std::vector<int>> AnimationSetFrameNum; //�e�A�j���[�V�����̃t���[������
	std::map<int, std::map<std::string, std::vector<D3DXMATRIX>>> AnimationSetMat; //�e�A�j���[�V�����̕ϊ��s��
};

class SkinMeshData
{
public:
	SkinMeshData();
	virtual ~SkinMeshData();
	SkinMeshInfo *GetSkinMeshInfo();
	BornInfo *GetBornInfo();
	virtual void Relese();
	virtual void ChangeAnimation(unsigned int num);
	virtual void SetRenewalTime(float animSpeed); //�A�j���[�V�������x�X�V
	virtual void SetTime(float animTime);		  //�w��̃A�j���[�V�������Ԃɐݒ�
	virtual float GetPlayTime();
	virtual int GetPlayAnimation();
	virtual int GetBornAllNum();
	virtual std::string GetBornName(int bornIndex);
	virtual Matrix GetBornWorld(int bornIndex);
	virtual Vector3D GetBornPos(int bornIndex);
	virtual void Update();
	virtual void BornDebug(eBornDebug eBornDebug);
	virtual void AnimationDebug(int animNum);

protected:
	HRESULT InitShader();

protected:
	SkinMeshInfo m_MeshInfo;
	BornInfo m_BornInfo;

};

#endif