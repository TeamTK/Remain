#include "LineSegment.h"
#include "ColliderManager.h"

LineSegmentCollider::LineSegmentCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_LineSegment &)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

LineSegmentCollider::~LineSegmentCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

LineSegment_vs_StaticMeshCollider::LineSegment_vs_StaticMeshCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon&)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

LineSegment_vs_StaticMeshCollider::~LineSegment_vs_StaticMeshCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}