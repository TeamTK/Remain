#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

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
	VertexInfo* pVertex;			     //���_���
	IndexInfo* pIndex;					 //�S�Ẵ|���S���̃C���f�b�N�X
	Matrix localMat;					 //���f���̃��[�J���s��
	int vertexNumAll;					 //���_��
	int faceNumAll;						 //�ʂ̐�
	int normalNumAll;					 //�@���̐�
	int materialNumAll;					 //�}�e���A���̐�
	int materialListNumAll;				 //�}�e���A�����X�g�̐�;
	int uvNumAll;						 //UV�̐�
	MeshInfo() :
		isTexture(true),
		vertexNumAll(0),
		faceNumAll(0),
		normalNumAll(0),
		materialNumAll(0),
		materialListNumAll(0),
		uvNumAll(0) {}
	~MeshInfo() {}
};

//���b�V���f�[�^
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
	MeshInfo m_MeshInfo; //���b�V���̏��

private:

};

#endif 