#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

//�}�e���A���\����
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;					//�f�B�t���[�Y
	Vector3D Specular;					//�X�y�L�����[
	Vector3D Ambient;					//�A���r�G���g
	std::string TextureName;			//�e�N�X�`���[�t�@�C����
	int dwNumFace;						//���̃}�e���A���ł���|���S����
	MaterialInfo() :
		pTexture(nullptr),
		dwNumFace(0) {}
	~MaterialInfo() {}
};

//���_�̍\����
struct VertexData
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

//���_�̍\����
struct VertexInfo
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

//���_�̃C���f�b�N�X
struct IndexInfo
{
	int vertexIndex[3];
	int materialIndex;
};

//���b�V���̏��
struct MeshInfo
{
	ID3D11Buffer*		 m_pVertexBuffer;	 //���_�o�b�t�@�[
	ID3D11Buffer**		 m_ppIndexBuffer;	 //�C���f�b�N�X�o�b�t�@�[
	ID3D11SamplerState*  m_pSampleLinear;	 //�e�N�X�`���[�̃T���v���[
	MaterialInfo*		 m_pMaterial;		 //�}�e���A�����
	bool m_IsTexture;						 //�e�N�X�`���[���f
	VertexInfo* pVertex;					 //���_���
	IndexInfo* pIndex;
	Matrix m_LocalMat;						 //���f���̃��[�J���s��
	int vertexNumAll;						 //���_��
	int faceNumAll;							 //�ʂ̐�
	int normalNumAll;						 //�@���̐�
	int materialNumAll;						 //�}�e���A���̐�
	int materialListNumAll;					 //�}�e���A�����X�g�̐�;
	int uvNumAll;							 //UV�̐�
	MeshInfo() :
		m_IsTexture(true),
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
	MeshInfo m_MeshInfo;

private:

};

#endif 