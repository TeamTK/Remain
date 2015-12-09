#include "SphereCollider.h"
#include "ColliderManager.h"

//‹…‚Æ‹…‚Ì“–‚½‚è”»’è
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

//‹…‚ÆƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è
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

//‹…‚ÆStaticMesh‚Ì“–‚½‚è”»’è
Sphere_vs_StaticMeshCollider::Sphere_vs_StaticMeshCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group &)> func)
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