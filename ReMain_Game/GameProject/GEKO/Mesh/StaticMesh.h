#ifndef _STATICMESH_H_
#define _STATICMESH_H_

#include "MeshBase.h"

class StaticMesh : public MeshBase
{
public:
	StaticMesh();
	~StaticMesh();
	void SetAsset(MeshData *meshData);
	void SetAsset(const std::string &MeshName);
	VertexInfo *GetVertex();
	Matrix *GetLocalMatrix();
	int GetFaceNum();
	void Render();
	void RenderMatrix(Matrix &matrix);
	void DebugNormal();

private:
	void RenderFunc(Matrix &matrix);

private:
	MeshData *m_pMeshData;

};

#endif