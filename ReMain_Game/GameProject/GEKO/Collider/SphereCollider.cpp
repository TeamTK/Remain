#include "SphereCollider.h"
#include "ColliderManager.h"

//球と球の当たり判定
SphereCollider::SphereCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	m_HitData.pPosition = pPos;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

SphereCollider::~SphereCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//球とカプセルの当たり判定
Sphere_vs_CapsuleCollider::Sphere_vs_CapsuleCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	m_HitData.pPosition = pPos;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

Sphere_vs_CapsuleCollider::~Sphere_vs_CapsuleCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//球とカプセルの当たり判定
Sphere_vs_LineSegmentCollider::Sphere_vs_LineSegmentCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	m_HitData.pPosition = pPos;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

Sphere_vs_LineSegmentCollider::~Sphere_vs_LineSegmentCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}

//球とStaticMeshの当たり判定
Sphere_vs_StaticMeshCollider::Sphere_vs_StaticMeshCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group_Sphere &)> func)
{
	m_HitData.pPosition = pPos;
	m_HitData.pRadius = pRadius;
	m_Func = func;
	ColliderManager::GetInstance()->Add(this);
}

Sphere_vs_StaticMeshCollider::~Sphere_vs_StaticMeshCollider()
{
	ColliderManager::GetInstance()->Clear(this);
}