#include "SphereCollider.h"
#include "ColliderManager.h"

//���Ƌ��̓����蔻��
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

//���ƃJ�v�Z���̓����蔻��
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

//���ƃJ�v�Z���̓����蔻��
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

//����StaticMesh�̓����蔻��
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