#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include "..\MeshBase.h"

class ShaderShadowMap;

//�A�j���[�V���������Ȃ��ÓI��3D���f����`��
class StaticMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	StaticMesh();
	StaticMesh(const std::string &meshName, bool isLightInterrupted = false);
	~StaticMesh();
	void SetAsset(const std::string &meshName, bool isLightInterrupted = false);
	void SetModelMatrixBuilding();
	const IndexInfo *GetIndex() const;
	const VertexInfo *GetVertex() const;
	const MaterialInfo *GetMaterial() const;
	const Matrix *GetLocalMatrix() const;
	const int GetFaceAllNum() const;
	const int GetMaterialAllNum() const;
	void Render(bool isShadow = false);
	void RenderMatrix(Matrix &matrix, bool isShadow = false);
	void DebugNormal() const;
	void DebugPolygon() const;

private:
	void AllocationMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix, bool isShadow) const;

private:
	StaticMeshData *m_pMeshData; //���f���̏��ւ̃|�C���^
};

#endif