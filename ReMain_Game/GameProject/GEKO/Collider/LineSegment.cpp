#include "LineSegment.h"
#include "ColliderManager.h"

//ü•ª‚Æü•ª‚Ì“–‚½‚è”»’è
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

//ü‚Æ‹…‚Æ‚Ì“–‚½‚è”»’è
LineSegment_vs_SphereCollider::LineSegment_vs_SphereCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Sphere&)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

LineSegment_vs_SphereCollider::~LineSegment_vs_SphereCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//ü‚ÆƒJƒvƒZƒ‹‚Æ‚Ì“–‚½‚è”»’è
LineSegment_vs_CapsuleCollider::LineSegment_vs_CapsuleCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Capsule&)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

LineSegment_vs_CapsuleCollider::~LineSegment_vs_CapsuleCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//ü‚ÆStaticMesh‚Æ‚Ì“–‚½‚è”»’è
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

//ü‚ÆStaticMesh‚Æ‚Ì“–‚½‚è”»’è(•¡”)
LineSegment_Group_vs_StaticMeshCollider::LineSegment_Group_vs_StaticMeshCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon_Group_LineSegment&)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

LineSegment_Group_vs_StaticMeshCollider::~LineSegment_Group_vs_StaticMeshCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}