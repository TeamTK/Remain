#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "MeshInfo.h"

//メッシュデータ
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
	MeshInfo m_MeshInfo; //メッシュの情報
};

class StaticMeshData : public MeshData
{
public:
	StaticMeshData();
	virtual ~StaticMeshData();
	VertexInfo* GetVertex();

protected:
	VertexInfo* m_pVertex; //頂点情報
};

class DyanmicMeshData : public MeshData
{
public:
	DyanmicMeshData();
	virtual ~DyanmicMeshData();
	SkinVertexInfo* GetVertex();
	const BornInfo *GetBornInfo() const;
	virtual void Update(CopyBorn *pCopyBorn, unsigned int animNum, float *animFrame, bool *pIsAnimEnd);
	virtual void CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal);

private:
	void DeleteHierarchy(Born *pBorn);

protected:
	SkinVertexInfo* m_pVertex; //頂点情報
	BornInfo m_BornInfo;
};

#endif 