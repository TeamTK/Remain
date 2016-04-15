#ifndef _MESH_INFO_H_
#define _MESH_INFO_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
#include <map>

//�����_�����O�̎��
enum RenderingType
{
	eForward,
	eDeferred,
};

//3D���f���̏��
enum MeshState
{
	eNothing = (1 << 0),
	eBlockingLight = (1 << 1),
	eShadow = (1 << 2),
	eFog = (1 << 3),
};

//3D���f���s��̎��
enum MatrixType
{
	eLocalMatrix,
	eWorldMatrix,
	eModelMatrix,
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

//�{�[���̃f�o�b�N���
enum eBoneDebug
{
	eInitMat,
	eOffsetMat,
	eWorld,
	eBoneMat
};

//�{�[���K�w�\��
struct Bone
{
	int indexId;
	Bone *brother;
	Bone *child;
	std::string boneName;
	Matrix initMat;
	Matrix offsetMat;
	Bone() :
		indexId(0),
		brother(nullptr),
		child(nullptr),
		boneName("NoName") {}
};

//�R�s�[����{�[��
struct CopyBone
{
	CopyBone *brother;
	CopyBone *child;
	Matrix worldMat;
	Matrix boneMat;
	Matrix parentAndChildMat;
	CopyBone() :
		brother(nullptr),
		child(nullptr) {}
};

//�{�[�����
struct BoneInfo
{
	Bone bone;	//�{�[���K�w�\��
	std::vector<Bone *> BoneList; //�{�[���A�h���X�i�[
	std::map<int, std::vector<int>> AnimationSetFrameNum; //�e�A�j���[�V�����̃t���[������
	std::map<int, std::map<std::string, std::vector<Matrix>>> AnimationSetMat; //�e�A�j���[�V�����̕ϊ��s��
};

#endif