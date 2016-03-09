#ifndef _MESH_INFO_H_
#define _MESH_INFO_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
#include <map>

enum MeshState
{
	eShadow = (1 << 0),
};

//�}�e���A���\����
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture; //���f���̃e�N�X�`��
	Vector4D diffuse;					//�f�B�t���[�Y
	Vector3D specular;					//�X�y�L�����[
	Vector3D ambient;					//�A���r�G���g
	std::string textureName;			//�e�N�X�`���[�t�@�C����
	int numPolygon;						//���̃}�e���A���ł���|���S����
	MaterialInfo() :
		pTexture(nullptr),
		numPolygon(0) {}
	~MaterialInfo() {}
};

//���_�̍\����
struct VertexInfo
{
	Vector3D pos;    //���_�ʒu
	Vector3D normal; //���_�̖@��
	Vector2D uv;	 //���_�̃e�N�X�`���̈ʒu
};

//���_�̃C���f�b�N�X
struct IndexInfo
{
	int vertexIndex[3]; //�|���S���̒��_�̃C���f�b�N�X�ԍ�
	int materialIndex;  //�|���S���̃}�e���A���C���f�b�N�X�ԍ�
};

//���b�V���̏��
struct MeshInfo
{
	ID3D11Buffer*		 pVertexBuffer;	 //���_�o�b�t�@�[
	ID3D11Buffer**		 ppIndexBuffer;	 //�C���f�b�N�X�o�b�t�@�[
	ID3D11SamplerState*  pSampleLinear;	 //�e�N�X�`���[�̃T���v���[
	MaterialInfo*		 pMaterial;		 //�}�e���A�����
	bool isTexture;						 //�e�N�X�`���[���f
	IndexInfo* pIndex;					 //�S�Ẵ|���S���̃C���f�b�N�X
	Matrix localMatrix;					 //���f���̃��[�J���s��
	int vertexNumAll;					 //���_��
	int faceNumAll;						 //�ʂ̐�
	int materialNumAll;					 //�}�e���A���̐�
	MeshInfo() :
		isTexture(true),
		vertexNumAll(0),
		faceNumAll(0),
		materialNumAll(0) {}
	~MeshInfo() {}
};

/******************************************/
//�X�L�����b�V���A�j���[�V�����p�f�[�^
/******************************************/

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
	CopyBorn() :
		brother(nullptr),
		child(nullptr) {}
};

//�{�[�����
struct BornInfo
{
	Born sBorn;	//�{�[���K�w�\��
	std::vector<Born *> BornList; //�{�[���A�h���X�i�[
	std::map<int, std::vector<int>> AnimationSetFrameNum; //�e�A�j���[�V�����̃t���[������
	std::map<int, std::map<std::string, std::vector<Matrix>>> AnimationSetMat; //�e�A�j���[�V�����̕ϊ��s��
};

#endif