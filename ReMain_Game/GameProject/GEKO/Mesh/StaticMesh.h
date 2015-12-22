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
	const VertexInfo *GetVertex() const;
	const MaterialInfo *GetMaterial() const;
	const Matrix *GetLocalMatrix() const;
	const int *GetPolygonIndex(int materialIndex) const;
	const int GetFaceAllNum() const;
	const int GetMaterialAllNum() const;
	void Render();
	void RenderMatrix(Matrix &matrix);
	void DebugNormal();
	void DebugPolygon();

private:
	void RenderFunc(Matrix &matrix);

private:
	MeshData *m_pMeshData;

};

#endif