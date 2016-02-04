#include "LineSegment.h"
#include "ColliderManager.h"

//�����Ɛ����̓����蔻��
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

//���Ƌ��Ƃ̓����蔻��
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

//���ƃJ�v�Z���Ƃ̓����蔻��
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

//����StaticMesh�Ƃ̓����蔻��
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

//����StaticMesh�Ƃ̓����蔻��(����)
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