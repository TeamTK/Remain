#include "StaticMeshCollider.h"
#include "ColliderManager.h"
#include "..\Mesh\StaticMesh.h"

StaticMesh_vs_LineSegmentCollider::StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_LineSegmentCollider::~StaticMesh_vs_LineSegmentCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

StaticMesh_vs_SphereCollider::StaticMesh_vs_SphereCollider(StaticMesh *pStaticMeshHitInfo)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_SphereCollider::~StaticMesh_vs_SphereCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}