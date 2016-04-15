#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include "..\MeshBase.h"

class ShaderShadowMap;

//アニメーションをしない静的な3Dモデルを描画
class StaticMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	StaticMesh();
	StaticMesh(const std::string &meshName, unsigned int priorityGroup, unsigned int priority, unsigned int meshState = MeshState::eNothing);
	~StaticMesh();
	void SetMeshState(unsigned int meshState);
	void SetAsset(const std::string &meshName);
	const IndexInfo *GetIndex() const;
	const VertexInfo *GetVertex() const;
	const MaterialInfo *GetMaterial() const;
	const int GetFaceAllNum() const;
	const int GetMaterialAllNum() const;
	void DebugNormal() const;
	void DebugPolygon() const;

private:
	void ForwardRendering();
	void DeferredRendering();
	void AllocationMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix, bool isShadow) const;
	void RenderFuncDeferred(Matrix &matrix, bool isShadow) const;

private:
	StaticMeshData *m_pMeshData; //モデルの情報へのポインタ
};

#endif