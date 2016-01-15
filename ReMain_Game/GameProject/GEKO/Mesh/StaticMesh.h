#ifndef _STATICMESH_H_
#define _STATICMESH_H_

#include "MeshBase.h"

class StaticMesh : public MeshBase
{
public:
	StaticMesh();
	StaticMesh(const std::string &meshName);
	~StaticMesh();
	void SetAsset(const std::string &meshName);
	const VertexInfo *GetVertex() const;
	const MaterialInfo *GetMaterial() const;
	const Matrix *GetLocalMatrix() const;
	const int *GetPolygonIndex(int materialIndex) const;
	const int GetFaceAllNum() const;
	const int GetMaterialAllNum() const;
	void Render() const;
	void RenderMatrix(Matrix &matrix) const;
	void DebugNormal() const;
	void DebugPolygon() const;

private:
	void AllocationMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix) const;

private:
	MeshData *m_pMeshData;

};

#endif