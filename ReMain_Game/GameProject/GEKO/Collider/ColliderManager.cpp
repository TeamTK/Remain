#include "ColliderManager.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "LineSegment.h"
#include "StaticMeshCollider.h"
#include "..\System\Collision.h"
#include "..\Mesh\StaticMesh.h"
#include <list>
//#include <map>

class ColliderManager::ColliderListPimpl
{
public:
	ColliderListPimpl() {};
	~ColliderListPimpl()
	{
		//��
		SphereList.clear();
		Sphere_vs_CapsuleList.clear();
		Sphere_vs_LineSegmentList.clear();
		Sphere_vs_StaticMeshList.clear();

		//�J�v�Z��
		CapsuleList.clear();
		Capsule_vs_SphereList.clear();
		Capsule_vs_LineSegmentList.clear();

		//����
		LineSegmentList.clear();
		LineSegment_vs_SphereList.clear();
		LineSegment_vs_CapsuleList.clear();
		LineSegment_vs_StaticMeshList.clear();

		//StaticMesh
		StaticMesh_vs_LineSegmentList.clear();
		StaticMesh_vs_SphereList.clear();
	}

	//��
	std::list<SphereCollider*> SphereList; //���΋�
	std::list<Sphere_vs_CapsuleCollider*> Sphere_vs_CapsuleList; //���΃J�v�Z��
	std::list<Sphere_vs_LineSegmentCollider*> Sphere_vs_LineSegmentList; //���ΐ���
	std::list<Sphere_vs_StaticMeshCollider*> Sphere_vs_StaticMeshList; //����StaticMesh

	//�J�v�Z��
	std::list<CapsuleCollider*> CapsuleList; //�J�v�Z���΃J�v�Z��
	std::list<Capsule_vs_SphereCollider*> Capsule_vs_SphereList; //�J�v�Z���΋�
	std::list<Capsule_vs_LineSegmentCollider*> Capsule_vs_LineSegmentList; //�J�v�Z���ΐ���

	//����
	std::list<LineSegmentCollider*> LineSegmentList; //�����ΐ���
	std::list<LineSegment_vs_SphereCollider*> LineSegment_vs_SphereList; //������
	std::list<LineSegment_vs_CapsuleCollider*> LineSegment_vs_CapsuleList; //�����΃J�v�Z��
	std::list<LineSegment_vs_StaticMeshCollider*> LineSegment_vs_StaticMeshList; //������staticMesh

	//StaticMesh
	std::list<StaticMesh_vs_LineSegmentCollider*> StaticMesh_vs_LineSegmentList; //���b�V���ΐ���
	std::list<StaticMesh_vs_SphereCollider*> StaticMesh_vs_SphereList; //���b�V���΋�
};

//�R���C�_�[�}�l�[�W���[//
ColliderManager::ColliderManager()
{
	m_pColliderListPimpl = new ColliderListPimpl();
}

ColliderManager::~ColliderManager()
{
	delete m_pColliderListPimpl;
}

ColliderManager *ColliderManager::GetInstance()
{
	static ColliderManager ColliderManager;
	return &ColliderManager;
}

void ColliderManager::Update()
{
	CheckSphere(); //�����m�̓����蔻�蒲��
	CheckCapsule(); //�J�v�Z�����m�̓����蔻�蒲��
	CheckSphere_vs_Capsule(); //���ƃJ�v�Z���̓����蔻�蒲��
	CheckLineSegment(); //�������m�̓����蔻�蒲��
	CheckStaitcMesh_vs_LineSegment(); //staticMesh�Ɛ����̓����蔻�蒲��
	CheckStaitcMesh_vs_Sphere(); //StaticMesh�Ɖ~�̓����蔻�蒲��
}

void ColliderManager::AllClear()
{
	//��
	m_pColliderListPimpl->SphereList.clear();
	m_pColliderListPimpl->Sphere_vs_CapsuleList.clear();
	m_pColliderListPimpl->Sphere_vs_LineSegmentList.clear();
	m_pColliderListPimpl->Sphere_vs_StaticMeshList.clear();

	//�J�v�Z��
	m_pColliderListPimpl->CapsuleList.clear();
	m_pColliderListPimpl->Capsule_vs_SphereList.clear();
	m_pColliderListPimpl->Capsule_vs_LineSegmentList.clear();

	//����
	m_pColliderListPimpl->LineSegmentList.clear();
	m_pColliderListPimpl->LineSegment_vs_SphereList.clear();
	m_pColliderListPimpl->LineSegment_vs_CapsuleList.clear();
	m_pColliderListPimpl->LineSegment_vs_StaticMeshList.clear();

	//StaticMesh
	m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.clear();
	m_pColliderListPimpl->StaticMesh_vs_SphereList.clear();
}

bool ColliderManager::HitCheckStaticMesh_Line(Result_Porygon *resultPorygon, Vector3D *start, Vector3D *end, unsigned int targetId)
{
	auto it2 = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.begin();
	auto it2End = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.end();
	for (; it2 != it2End; it2++)
	{
		if ((*it2)->m_IsSeep) continue;

		//ID����v�����瓖���蔻��v�Z�J�n
		if (targetId & (*it2)->m_MyId)
		{
			LineSegmentHitData lineInfo;
			lineInfo.pStart = start;
			lineInfo.pEnd = end;

			static Result_Porygon pory;
			if (HitCheckStaticMesh_vs_LineSegment(*(*it2)->m_pStaticMeshInfo, lineInfo, &pory))
			{
				pory.targetID = (*it2)->m_MyId;
				*resultPorygon = pory;
				return true;
			}
		}
	}

	/*
	for (auto& i : m_pColliderListPimpl->StaticMesh_vs_LineSegmentList)
	{
		if (i->m_IsSeep) continue;

		//ID����v�����瓖���蔻��v�Z�J�n
		if (targetId & i->m_MyId)
		{
			LineSegmentHitData lineInfo;
			lineInfo.pStart = start;
			lineInfo.pEnd = end;

			static Result_Porygon pory;
			if (HitCheckStaticMesh_vs_LineSegment(*i->m_pStaticMeshInfo, lineInfo, &pory))
			{
				pory.targetID = i->m_MyId;
				*resultPorygon = pory;
				return true;
			}
		}
	}
	*/
	return false;
}

void ColliderManager::Add(SphereCollider *pSphere)
{
	m_pColliderListPimpl->SphereList.push_back(pSphere);
}

void ColliderManager::Add(Sphere_vs_CapsuleCollider *pSphere)
{
	m_pColliderListPimpl->Sphere_vs_CapsuleList.push_back(pSphere);
}

void ColliderManager::Add(Sphere_vs_LineSegmentCollider *pSphere)
{
	m_pColliderListPimpl->Sphere_vs_LineSegmentList.push_back(pSphere);
}

void ColliderManager::Add(Sphere_vs_StaticMeshCollider *pSphere)
{
	m_pColliderListPimpl->Sphere_vs_StaticMeshList.push_back(pSphere);
}

void ColliderManager::Add(CapsuleCollider *pCapsule)
{
	m_pColliderListPimpl->CapsuleList.push_back(pCapsule);
}

void ColliderManager::Add(Capsule_vs_SphereCollider *pCapsule)
{
	m_pColliderListPimpl->Capsule_vs_SphereList.push_back(pCapsule);
}

void ColliderManager::Add(Capsule_vs_LineSegmentCollider *pCapsule)
{
	m_pColliderListPimpl->Capsule_vs_LineSegmentList.push_back(pCapsule);
}

void ColliderManager::Add(LineSegmentCollider *pLineSegment)
{
	m_pColliderListPimpl->LineSegmentList.push_back(pLineSegment);
}

void ColliderManager::Add(LineSegment_vs_SphereCollider *pLineSegment)
{
	m_pColliderListPimpl->LineSegment_vs_SphereList.push_back(pLineSegment);
}

void ColliderManager::Add(LineSegment_vs_CapsuleCollider *pLineSegment)
{
	m_pColliderListPimpl->LineSegment_vs_CapsuleList.push_back(pLineSegment);
}

void ColliderManager::Add(LineSegment_vs_StaticMeshCollider *pLineSegment)
{
	m_pColliderListPimpl->LineSegment_vs_StaticMeshList.push_back(pLineSegment);
}

void ColliderManager::Add(StaticMesh_vs_LineSegmentCollider *pStaticMesh)
{
	m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.push_back(pStaticMesh);
}

void ColliderManager::Add(StaticMesh_vs_SphereCollider *pStaticMesh)
{
	m_pColliderListPimpl->StaticMesh_vs_SphereList.push_back(pStaticMesh);
}

void ColliderManager::Clear(SphereCollider *pSphere)
{
	auto it = m_pColliderListPimpl->SphereList.begin();
	auto itEnd = m_pColliderListPimpl->SphereList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pSphere)
		{
			m_pColliderListPimpl->SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Sphere_vs_CapsuleCollider *pSphere)
{
	auto it = m_pColliderListPimpl->Sphere_vs_CapsuleList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_CapsuleList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pSphere)
		{
			m_pColliderListPimpl->Sphere_vs_CapsuleList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Sphere_vs_LineSegmentCollider *pSphere)
{
	auto it = m_pColliderListPimpl->Sphere_vs_LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_LineSegmentList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pSphere)
		{
			m_pColliderListPimpl->Sphere_vs_LineSegmentList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Sphere_vs_StaticMeshCollider *pSphere)
{
	auto it = m_pColliderListPimpl->Sphere_vs_StaticMeshList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_StaticMeshList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pSphere)
		{
			m_pColliderListPimpl->Sphere_vs_StaticMeshList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(CapsuleCollider *pCapsule)
{
	auto it = m_pColliderListPimpl->CapsuleList.begin();
	auto itEnd = m_pColliderListPimpl->CapsuleList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pCapsule)
		{
			m_pColliderListPimpl->CapsuleList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Capsule_vs_SphereCollider *pCapsule)
{
	auto it = m_pColliderListPimpl->Capsule_vs_SphereList.begin();
	auto itEnd = m_pColliderListPimpl->Capsule_vs_SphereList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pCapsule)
		{
			m_pColliderListPimpl->Capsule_vs_SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Capsule_vs_LineSegmentCollider *pCapsule)
{
	auto it = m_pColliderListPimpl->Capsule_vs_LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->Capsule_vs_LineSegmentList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pCapsule)
		{
			m_pColliderListPimpl->Capsule_vs_LineSegmentList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegmentCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegmentList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pLineSegment)
		{
			m_pColliderListPimpl->LineSegmentList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegment_vs_SphereCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegment_vs_SphereList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegment_vs_SphereList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pLineSegment)
		{
			m_pColliderListPimpl->LineSegment_vs_SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegment_vs_CapsuleCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegment_vs_CapsuleList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegment_vs_CapsuleList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pLineSegment)
		{
			m_pColliderListPimpl->LineSegment_vs_CapsuleList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegment_vs_StaticMeshCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegment_vs_StaticMeshList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegment_vs_StaticMeshList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pLineSegment)
		{
			m_pColliderListPimpl->LineSegment_vs_StaticMeshList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(StaticMesh_vs_LineSegmentCollider *pStaticMesh)
{
	auto it = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pStaticMesh)
		{
			m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(StaticMesh_vs_SphereCollider *pStaticMesh)
{
	auto it = m_pColliderListPimpl->StaticMesh_vs_SphereList.begin();
	auto itEnd = m_pColliderListPimpl->StaticMesh_vs_SphereList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pStaticMesh)
		{
			m_pColliderListPimpl->StaticMesh_vs_SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::CheckSphere()
{
	auto it = m_pColliderListPimpl->SphereList.begin();
	auto itEnd = m_pColliderListPimpl->SphereList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != itEnd; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckSphere((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_Sphere hitData1;
					static Result_Sphere hitData2;

					hitData1.position = *(*it)->m_HitData.pPosition;
					hitData1.radius = *(*it)->m_HitData.pRadius;
					hitData1.targetID = (*it)->m_MyId;
					hitData1.targetName = (*it)->m_Name;
					hitData1.name = (*it2)->m_Name;

					hitData2.position = *(*it2)->m_HitData.pPosition;
					hitData2.radius = *(*it2)->m_HitData.pRadius;
					hitData2.targetID = (*it2)->m_MyId;
					hitData2.targetName = (*it2)->m_Name;
					hitData2.name = (*it)->m_Name;

					(*it)->m_Func(hitData2);
					(*it2)->m_Func(hitData1);
				}
			}
		}
	}
}

void ColliderManager::CheckSphere_vs_Capsule()
{
	auto it = m_pColliderListPimpl->Sphere_vs_CapsuleList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_CapsuleList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->Capsule_vs_SphereList.begin();
		auto it2End = m_pColliderListPimpl->Capsule_vs_SphereList.end();
		for (; it2 != it2End; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckSphere_vs_Capsule((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_Capsule hitData1;
					static Result_Sphere hitData2;

					hitData1.start = *(*it2)->m_HitData.pStart;
					hitData1.end = *(*it2)->m_HitData.pEnd;
					hitData1.radius = *(*it2)->m_HitData.pRadius;
					hitData1.targetID = (*it2)->m_MyId;
					hitData1.targetName = (*it2)->m_Name;
					hitData1.name = (*it)->m_Name;

					hitData2.position = *(*it)->m_HitData.pPosition;
					hitData2.radius = *(*it)->m_HitData.pRadius;
					hitData2.targetID = (*it)->m_MyId;
					hitData2.targetName = (*it)->m_Name;
					hitData2.name = (*it2)->m_Name;

					(*it)->m_Func(hitData1);  //�J�v�Z�����̏��
					(*it2)->m_Func(hitData2); //�����̏��
				}
			}
		}
	}
}

void ColliderManager::CheckSphere_vs_LineSegment()
{
	auto it = m_pColliderListPimpl->Sphere_vs_LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_LineSegmentList.begin();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->LineSegment_vs_SphereList.begin();
		auto it2End = m_pColliderListPimpl->LineSegment_vs_SphereList.end();
		for (; it2 != it2End; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckSphere_vs_LineSegment((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_LineSegment hitData1;
					static Result_Sphere hitData2;

					hitData1.start = *(*it2)->m_HitData.pStart;
					hitData1.end = *(*it2)->m_HitData.pEnd;
					hitData1.targetID = (*it2)->m_MyId;
					hitData1.targetName = (*it2)->m_Name;
					hitData1.name = (*it)->m_Name;

					hitData2.position = *(*it)->m_HitData.pPosition;
					hitData2.radius = *(*it)->m_HitData.pRadius;
					hitData2.targetID = (*it)->m_MyId;
					hitData2.targetName = (*it)->m_Name;
					hitData2.name = (*it2)->m_Name;

					(*it)->m_Func(hitData1);  //�������̏��
					(*it2)->m_Func(hitData2); //�����̏��
				}
			}
		}
	}
}

void ColliderManager::CheckCapsule()
{
	auto it = m_pColliderListPimpl->CapsuleList.begin();
	auto itEnd = m_pColliderListPimpl->CapsuleList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != itEnd; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckCapsule((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_Capsule hitData1;
					static Result_Capsule hitData2;

					hitData1.start = *(*it)->m_HitData.pStart;
					hitData1.end = *(*it)->m_HitData.pEnd;
					hitData1.radius = *(*it)->m_HitData.pRadius;
					hitData1.targetID = (*it)->m_MyId;
					hitData1.targetName = (*it)->m_Name;
					hitData1.name = (*it2)->m_Name;

					hitData2.start = *(*it2)->m_HitData.pStart;
					hitData2.end = *(*it2)->m_HitData.pEnd;
					hitData2.radius = *(*it2)->m_HitData.pRadius;
					hitData2.targetID = (*it2)->m_MyId;
					hitData2.targetName = (*it2)->m_Name;
					hitData2.name = (*it)->m_Name;

					(*it)->m_Func(hitData2);
					(*it2)->m_Func(hitData1);
				}
			}
		}
	}
}

void ColliderManager::CheckCapsule_vs_LineSegment()
{
	auto it = m_pColliderListPimpl->Capsule_vs_LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->Capsule_vs_LineSegmentList.begin();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->LineSegment_vs_CapsuleList.begin();
		auto it2End = m_pColliderListPimpl->LineSegment_vs_CapsuleList.end();
		for (; it2 != it2End; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckCapsule_vs_LineSegment((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_LineSegment hitData1;
					static Result_Capsule hitData2;

					hitData1.start = *(*it2)->m_HitData.pStart;
					hitData1.end = *(*it2)->m_HitData.pEnd;
					hitData1.targetID = (*it2)->m_MyId;
					hitData1.targetName = (*it2)->m_Name;
					hitData1.name = (*it)->m_Name;

					hitData2.start = *(*it)->m_HitData.pStart;
					hitData2.end = *(*it)->m_HitData.pEnd;
					hitData2.radius = *(*it)->m_HitData.pRadius;
					hitData2.targetID = (*it)->m_MyId;
					hitData2.targetName = (*it)->m_Name;
					hitData2.name = (*it2)->m_Name;


					(*it)->m_Func(hitData1);  //�������̏��
					(*it2)->m_Func(hitData2); //�J�v�Z�����̏��
				}
			}
		}
	}
}

void ColliderManager::CheckLineSegment()
{
	auto it = m_pColliderListPimpl->LineSegmentList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegmentList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != itEnd; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckLineSegment((*it)->m_HitData, (*it2)->m_HitData))
				{
					//���ꂼ��̓����蔻�����n��
					static Result_LineSegment hitData1;
					static Result_LineSegment hitData2;

					hitData1.start = *(*it)->m_HitData.pStart;
					hitData1.end = *(*it)->m_HitData.pEnd;
					hitData1.targetID = (*it)->m_MyId;
					hitData1.targetName = (*it)->m_Name;
					hitData1.name = (*it2)->m_Name;

					hitData2.start = *(*it2)->m_HitData.pStart;
					hitData2.end = *(*it2)->m_HitData.pEnd;
					hitData2.targetID = (*it2)->m_MyId;
					hitData2.targetName = (*it2)->m_Name;
					hitData2.name = (*it)->m_Name;

					(*it)->m_Func(hitData2);
					(*it2)->m_Func(hitData1);
				}
			}
		}
	}
}

void ColliderManager::CheckStaitcMesh_vs_LineSegment()
{
	auto it = m_pColliderListPimpl->LineSegment_vs_StaticMeshList.begin();
	auto itEnd = m_pColliderListPimpl->LineSegment_vs_StaticMeshList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.begin();
		auto it2End = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.end();
		for (; it2 != it2End; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				static Result_Porygon pory;
				if (HitCheckStaticMesh_vs_LineSegment(*(*it2)->m_pStaticMeshInfo, (*it)->m_HitData, &pory))
				{
					pory.targetID = (*it2)->m_MyId;
					pory.targetName = (*it2)->m_Name;
					pory.name = (*it)->m_Name;
					(*it)->m_Func(pory);
				}
			}
		}
	}
}

void ColliderManager::CheckStaitcMesh_vs_Sphere()
{
	auto it = m_pColliderListPimpl->Sphere_vs_StaticMeshList.begin();
	auto itEnd = m_pColliderListPimpl->Sphere_vs_StaticMeshList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->StaticMesh_vs_SphereList.begin();
		auto it2End = m_pColliderListPimpl->StaticMesh_vs_SphereList.end();
		for (; it2 != it2End; it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//ID����v�����瓖���蔻��v�Z�J�n
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				Result_Porygon_Group pory;
				if (HitCheckStaticMesh_vs_Sphere(*(*it2)->m_pStaticMeshInfo, (*it)->m_HitData, &pory))
				{
					pory.targetName = (*it2)->m_Name;
					pory.name = (*it)->m_Name;
					(*it)->m_Func(pory);
					delete[] pory.pArray;
				}
			}
		}
	}
}

bool ColliderManager::HitCheckSphere(SphereHitData &pHitData1, SphereHitData &pHitData2)
{
	SphereInfo hit1(*pHitData1.pPosition, *pHitData1.pRadius);
	SphereInfo hit2(*pHitData2.pPosition, *pHitData2.pRadius);

	return Collision3D::Sphere(hit1, hit2);
}

bool ColliderManager::HitCheckSphere_vs_Capsule(SphereHitData &pHitData1, CapsuleHitData &pHitData2)
{
	SphereInfo hit1(*pHitData1.pPosition, *pHitData1.pRadius);
	LineSegmentInfo segment(*pHitData2.pStart, *pHitData2.pEnd);
	CapsuleInfo hit2(segment, *pHitData2.pRadius);

	return Collision3D::SphereCapsule(hit1, hit2);
}

bool ColliderManager::HitCheckSphere_vs_LineSegment(SphereHitData &pHitData1, LineSegmentHitData &pHitData2)
{
	SphereInfo hit1(*pHitData1.pPosition, *pHitData1.pRadius);
	LineSegmentInfo hit2(*pHitData2.pStart, *pHitData2.pEnd);

	return Collision3D::SphereSegment(hit1, hit2);
}

bool ColliderManager::HitCheckCapsule(CapsuleHitData &pHitData1, CapsuleHitData &pHitData2)
{
	LineSegmentInfo segment1(*pHitData1.pStart, *pHitData1.pEnd);
	LineSegmentInfo segment2(*pHitData2.pStart, *pHitData2.pEnd);

	CapsuleInfo hit1(segment1, *pHitData1.pRadius);
	CapsuleInfo hit2(segment2, *pHitData2.pRadius);

	return Collision3D::Capsule(hit1, hit2);
}

bool ColliderManager::HitCheckCapsule_vs_LineSegment(CapsuleHitData &pHitData1, LineSegmentHitData &pHitData2)
{
	LineSegmentInfo segment1(*pHitData1.pStart, *pHitData1.pEnd);
	LineSegmentInfo Hit2(*pHitData2.pStart, *pHitData2.pEnd);
	CapsuleInfo hit1(segment1, *pHitData1.pRadius);

	return false;
}

bool ColliderManager::HitCheckLineSegment(LineSegmentHitData &pHitData1, LineSegmentHitData &pHitData2)
{
	return true;
}

bool ColliderManager::HitCheckStaticMesh_vs_LineSegment(StaticMesh &hitData1, LineSegmentHitData &hitData2, Result_Porygon *pory)
{
	HitResult_SegmentTriangle hit;

	//���f���̕ϊ��s��
	Matrix world = *hitData1.GetWorldMatrix();
	Matrix local = *hitData1.GetLocalMatrix();
	Matrix m = local * world;
	Matrix inverse = m.GetInverse();

	//�����蔻��̌`��
	TriangleInfo hitTriangle;
	LineSegmentInfo hitLine(*hitData2.pStart * inverse, *hitData2.pEnd * inverse);

	//���_�f�[�^�ƃ}�e���A���f�[�^
	const VertexInfo *ver = hitData1.GetVertex();
	const MaterialInfo *material = hitData1.GetMaterial();

	//�}�e���A�����ƂɃ|���S���Ƃ̓����蔻�������
	int materialAllNum = hitData1.GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//�}�e���A���ʂ̃C���f�b�N�X���擾
		const int *index = hitData1.GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;

		for (int j = 0; j < faceNum; j++)
		{
			hitTriangle.v1 = ver[index[j * 3]].vPos;
			hitTriangle.v2 = ver[index[j * 3 + 1]].vPos;
			hitTriangle.v3 = ver[index[j * 3 + 2]].vPos;

			hit = Collision3D::LineSegmentTriangle(hitLine, hitTriangle);

			//���������瓖���������_�i�[
			if (hit.isHit)
			{
				pory->worldMatrix = world;
				pory->localMatrix = local;
				pory->meshMatrix = m;
				pory->contactPos = hit.pos;
				pory->normal = hit.normal;
				pory->vertexPos[0] = hitTriangle.v1;
				pory->vertexPos[1] = hitTriangle.v2;
				pory->vertexPos[2] = hitTriangle.v3;
				pory->materialIndex = i;
				return true;
			}
		}
	}
	return false;
}

bool ColliderManager::HitCheckStaticMesh_vs_Sphere(StaticMesh &hitData1, SphereHitData &hitData2, Result_Porygon_Group *pory)
{
	HitResult_SphereTriangle Hitdata;

	//���f���̕ϊ��s��
	Matrix world = *hitData1.GetWorldMatrix();
	Matrix local = *hitData1.GetLocalMatrix();
	Matrix m = local * world;

	//�����蔻��̌`��
	TriangleInfo hitTriangle;
	SphereInfo hitSphere(*hitData2.pPosition * m.GetInverse(), *hitData2.pRadius);

	//���_�f�[�^�ƃ}�e���A���f�[�^
	const VertexInfo *ver = hitData1.GetVertex();
	const MaterialInfo *material = hitData1.GetMaterial();

	//�����������
	std::vector<Vector3D> hitPos;
	std::vector<Vector3D> hitNormal;
	std::vector<int> materialNum;
	std::vector<float> hitDist;
	std::vector<TriangleInfo> hitVer;

	//�}�e���A�����ƂɃ|���S���Ƃ̓����蔻�������
	int materialAllNum = hitData1.GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//�}�e���A���ʂ̃C���f�b�N�X���擾
		const int *index = hitData1.GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;

		for (int j = 0; j < faceNum; j++)
		{
			hitTriangle.v1 = ver[index[j * 3]].vPos;
			hitTriangle.v2 = ver[index[j * 3 + 1]].vPos;
			hitTriangle.v3 = ver[index[j * 3 + 2]].vPos;
			Hitdata = Collision3D::SphereTriangle(hitSphere, hitTriangle);

			//���������瓖���������_�i�[
			if (Hitdata.isHit)
			{
				hitPos.emplace_back(Hitdata.pos);
				hitNormal.emplace_back(Hitdata.normal);
				hitVer.emplace_back(hitTriangle.v1, hitTriangle.v2, hitTriangle.v3);
				materialNum.emplace_back(i);
				hitDist.emplace_back(Hitdata.dist);
			}
		}
	}

	if (hitVer.size() <= 0) return false;

	//�����������i�[
	pory->pArray = new Result_Porygon_Sphere[hitVer.size()];
	for (unsigned int i = 0; i < hitVer.size(); i++)
	{
		pory->pArray[i].contactPos = hitPos[i];
		pory->pArray[i].normal = hitNormal[i];
		pory->pArray[i].vertexPos[0];
		pory->pArray[i].vertexPos[1];
		pory->pArray[i].vertexPos[2];
		pory->pArray[i].materialIndex = materialNum[i];
		pory->pArray[i].dist = hitDist[i];
	}
	pory->worldMatrix = world;
	pory->localMatrix = local;
	pory->meshMatrix = m;
	pory->hitNum = hitVer.size();

	//�����폜
	hitPos.clear();
	hitPos.shrink_to_fit();
	hitNormal.clear();
	hitNormal.shrink_to_fit();
	hitVer.clear();
	hitVer.shrink_to_fit();
	materialNum.clear();
	materialNum.shrink_to_fit();
	hitDist.clear();
	hitDist.shrink_to_fit();

	return true;
}