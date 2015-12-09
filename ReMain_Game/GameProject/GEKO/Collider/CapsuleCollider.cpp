#include "CapsuleCollider.h"
#include "ColliderManager.h"

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