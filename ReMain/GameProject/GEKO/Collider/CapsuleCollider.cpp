#include "CapsuleCollider.h"
#include "ColliderManager.h"

//カプセルとカプセルの当たり判定
CapsuleCollider::CapsuleCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

CapsuleCollider::~CapsuleCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//カプセルと球の当たり判定
Capsule_vs_SphereCollider::Capsule_vs_SphereCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

Capsule_vs_SphereCollider::~Capsule_vs_SphereCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//カプセルと線分の当たり判定
Capsule_vs_LineSegmentCollider::Capsule_vs_LineSegmentCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	m_HitData.pStart = pStart;
	m_HitData.pEnd = pEnd;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

Capsule_vs_LineSegmentCollider::~Capsule_vs_LineSegmentCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}