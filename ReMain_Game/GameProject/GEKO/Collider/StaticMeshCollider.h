#ifndef _STATIC_MESH_COLLIDER_H_
#define _STATIC_MESH_COLLIDER_H_

#include "BaseCollider.h"
#include "../Shader/CopmuteShader/LineTriangleCopmute.h"

class StaticMesh;

//StaticMesh‚Æü‚Ì“–‚½‚è”»’è
class StaticMesh_vs_LineSegmentCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo, bool isGPU);
	~StaticMesh_vs_LineSegmentCollider();

private:
	bool m_isGPU;
	LineTriangleCopmute m_Copmute;
	StaticMesh *m_pStaticMeshInfo;
	Vector3D *m_pNormal;
};

//StaticMesh‚Æ‹…‚Ì“–‚½‚è”»’è
class StaticMesh_vs_SphereCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	StaticMesh_vs_SphereCollider(StaticMesh *pStaticMeshHitInfo);
	~StaticMesh_vs_SphereCollider();

private:
	StaticMesh *m_pStaticMeshInfo;
	Vector3D *m_pNormal;
};

#endif