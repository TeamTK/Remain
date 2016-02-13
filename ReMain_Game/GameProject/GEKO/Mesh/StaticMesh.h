#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include "MeshBase.h"
class ShaderShadowMap;

class StaticMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	StaticMesh();
	StaticMesh(const std::string &meshName, bool isLightInterrupted = false);
	~StaticMesh();
	void SetAsset(const std::string &meshName, bool isLightInterrupted = false);
	const IndexInfo *GetIndex() const;
	const VertexInfo *GetVertex() const;
	const MaterialInfo *GetMaterial() const;
	const Matrix *GetLocalMatrix() const;
	const int GetFaceAllNum() const;
	const int GetMaterialAllNum() const;
	void Render(bool isShadow = false) const;
	void RenderMatrix(Matrix &matrix, bool isShadow = false) const;
	void DebugNormal() const;
	void DebugPolygon() const;

private:
	void AllocationMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix, bool isShadow) const;

private:
	MeshData *m_pMeshData;

};

#endif