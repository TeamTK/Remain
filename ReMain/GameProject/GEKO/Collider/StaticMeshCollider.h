#ifndef _STATIC_MESH_COLLIDER_H_
#define _STATIC_MESH_COLLIDER_H_

#include "BaseCollider.h"

class StaticMesh;

class StaticMesh_vs_LineSegmentCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo);
	~StaticMesh_vs_LineSegmentCollider();

private:
	StaticMesh *m_pStaticMeshInfo;

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

};

#endif