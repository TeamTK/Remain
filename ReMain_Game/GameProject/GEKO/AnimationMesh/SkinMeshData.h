#ifndef _SKINMESH_DATA_H_
#define _SKINMESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

#define MAX_BONES 255

//�}�e���A���\����
struct SkinMaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D diffuse;	//�f�B�t���[�Y
	Vector3D specular;	//�X�y�L�����[
	Vector3D ambient;	//�A���r�G���g
	std::string textureName;//�e�N�X�`���[�t�@�C����
	int numPolygon;		//���̃}�e���A���ł���|���S����
	SkinMaterialInfo() :
		pTexture(nullptr),
		numPolygon(0) {}
	~SkinMaterialInfo() {}
};

//���_�̍\����
struct SkinVertexInfo
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
	UINT boneIndex[4];	 //�{�[���ԍ�
	float boneWeight[4];//�{�[���d��
	SkinVertexInfo()
	{
		boneIndex[0] = 0;
		boneIndex[1] = 0;
		boneIndex[2] = 0;
		boneIndex[3] = 0;

		boneWeight[0] = 0.0f;
		boneWeight[1] = 0.0f;
		boneWeight[2] = 0.0f;
		boneWeight[3] = 0.0f;
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
	ID3D11Buffer*		 pVertexBuffer;	 //���_�o�b�t�@�[
	ID3D11Buffer**		 ppIndexBuffer;	 //�C���f�b�N�X�o�b�t�@�[
	ID3D11SamplerState*  pSampleLinear;	 //�e�N�X�`���[�̃T���v���[
	SkinMaterialInfo*	 pMaterial;		 //�}�e���A�����
	DWORD				 dwNumMaterial;	 //�}�e���A���̐�
	bool isTexture;						 //�e�N�X�`���[���f
	SkinVertexInfo* pVertex;				 //���_���
	int vertexNumAll;						 //���_��
	int faceNumAll;						 //�ʂ̐�
	int normalNumAll;						 //�@���̐�
	int materialNumAll;					 //�}�e���A���̐�
	int materialListNumAll;				 //�}�e���A�����X�g�̐�;
	int uvNumAll;							 //UV�̐�
	SkinMeshInfo() :
		isTexture(true),
		pVertexBuffer(nullptr),
		pMaterial(nullptr),
		pVertex(nullptr),
		faceNumAll(0),
		normalNumAll(0),
		materialNumAll(0),
		materialListNumAll(0),
		uvNumAll(0) {}
	~SkinMeshInfo() {}
};

//�{�[���K�w�\��
struct Born
{
	int indexId;
	Born *brother;
	Born *child;
	std::string BornName;
	Matrix initMat;
	Matrix offsetMat;
	Born() :
		indexId(0),
		brother(nullptr),
		child(nullptr),
		BornName("NoName") {}
};

//�R�s�[����{�[��
struct CopyBorn
{
	CopyBorn *brother;
	CopyBorn *child;
	Matrix worldMat;
	Matrix bornMat;
	Matrix ParentAndChildMat;
	unsigned int animNum;
	float animFrame;
	CopyBorn() :
		brother(nullptr),
		child(nullptr),
		animNum(0),
		animFrame(0.0f) {}
};

//�{�[�����
struct BornInfo
{
	Born sBorn;	//�{�[���K�w�\��
	std::vector<Born *> BornList; //�{�[���A�h���X�i�[
	std::map<int, std::vector<int>> AnimationSetFrameNum; //�e�A�j���[�V�����̃t���[������
	std::map<int, std::map<std::string, std::vector<Matrix>>> AnimationSetMat; //�e�A�j���[�V�����̕ϊ��s��
};

//�X�L�����b�V���f�[�^
class SkinMeshData
{
public:
	SkinMeshData();
	virtual ~SkinMeshData();
	const SkinMeshInfo *GetSkinMeshInfo() const;
	const BornInfo *GetBornInfo() const;
	virtual void Relese();
	virtual void Update(CopyBorn *pCopyBorn);
	virtual void CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal);

protected:
	HRESULT InitShader();

protected:
	SkinMeshInfo m_MeshInfo;
	BornInfo m_BornInfo;

};

#endif