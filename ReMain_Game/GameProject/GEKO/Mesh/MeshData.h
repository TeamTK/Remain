#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "MeshInfo.h"

//���b�V���f�[�^
class MeshData
{
public:
	MeshData();
	virtual ~MeshData();
	MeshInfo *GetMeshInfo();
	virtual void Relese();

protected:
	HRESULT InitShader();

protected:
	MeshInfo m_MeshInfo; //���b�V���̏��
};

class StaticMeshData : public MeshData
{
public:
	StaticMeshData();
	virtual ~StaticMeshData();
	VertexInfo* GetVertex();

protected:
	VertexInfo* m_pVertex; //���_���
};

class DyanmicMeshData : public MeshData
{
public:
	DyanmicMeshData();
	virtual ~DyanmicMeshData();
	SkinVertexInfo* GetVertex();
	const BoneInfo *GetBoneInfo() const;
	virtual void Update(CopyBone *pCopyBone, unsigned int animNum, float *animFrame, bool *pIsAnimEnd);
	virtual void CopyBoneTree(CopyBone *pBoneCopy, std::vector<CopyBone*> *pCopyBoneArray, Bone *pBoneOriginal);

private:
	void DeleteHierarchy(Bone *pBone);

protected:
	SkinVertexInfo* m_pVertex; //���_���
	BoneInfo m_BoneInfo;
};

#endif 