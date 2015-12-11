#include "ColliderManager.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "LineSegment.h"
#include "StaticMeshCollider.h"
#include "..\System\Collision.h"
#include "..\Mesh\StaticMesh.h"
//#include "..\Figure\Fiqure.h"
#include <list>

class ColliderManager::ColliderListPimpl
{
public:
	ColliderListPimpl() {};
	~ColliderListPimpl()
	{
		SphereList.clear();
		CapsuleList.clear();
		LineSegmentList.clear();
		Sphere_vs_CapsuleList.clear();
		Sphere_vs_StaticMeshList.clear();
		Capsule_vs_SphereList.clear();
		LineSegment_vs_StaticMeshList.clear();
		StaticMesh_vs_LineSegmentList.clear();
		StaticMesh_vs_SphereList.clear();
	}
	std::list<SphereCollider*> SphereList; //球
	std::list<CapsuleCollider*> CapsuleList; //カプセル
	std::list<LineSegmentCollider*> LineSegmentList; //線分
	std::list<Sphere_vs_CapsuleCollider*> Sphere_vs_CapsuleList; //球対カプセル
	std::list<Sphere_vs_StaticMeshCollider*> Sphere_vs_StaticMeshList; //球対カプセル
	std::list<Capsule_vs_SphereCollider*> Capsule_vs_SphereList; //カプセル対球
	std::list<LineSegment_vs_StaticMeshCollider*> LineSegment_vs_StaticMeshList; //線分対staticMesh
	std::list<StaticMesh_vs_LineSegmentCollider*> StaticMesh_vs_LineSegmentList; //メッシュ
	std::list<StaticMesh_vs_SphereCollider*> StaticMesh_vs_SphereList; //メッシュ
};

//コライダーマネージャー//
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

void ColliderManager::Add(SphereCollider *pSphere)
{
	m_pColliderListPimpl->SphereList.push_back(pSphere);
}

void ColliderManager::Add(Sphere_vs_CapsuleCollider *pSphere)
{
	m_pColliderListPimpl->Sphere_vs_CapsuleList.push_back(pSphere);
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

void ColliderManager::Add(LineSegmentCollider *pLineSegment)
{
	m_pColliderListPimpl->LineSegmentList.push_back(pLineSegment);
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

void ColliderManager::Update()
{
	CheckSphere(); //球同士の当たり判定調査
	CheckCapsule(); //カプセル同士の当たり判定調査
	CheckSphere_vs_Capsule(); //球とカプセルの当たり判定調査
	CheckLineSegment(); //線分同士の当たり判定調査
	CheckStaitcMesh_vs_LineSegment(); //staticMeshと線分の当たり判定調査
	CheckStaitcMesh_vs_Sphere(); //StaticMeshと円の当たり判定調査
}

void ColliderManager::Clear(SphereCollider *pSphere)
{
	auto it = m_pColliderListPimpl->SphereList.begin();
	for (; it != m_pColliderListPimpl->SphereList.end(); it++)
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
	for (; it != m_pColliderListPimpl->Sphere_vs_CapsuleList.end(); it++)
	{
		if (*it == pSphere)
		{
			m_pColliderListPimpl->Sphere_vs_CapsuleList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(Sphere_vs_StaticMeshCollider *pSphere)
{
	auto it = m_pColliderListPimpl->Sphere_vs_StaticMeshList.begin();
	for (; it != m_pColliderListPimpl->Sphere_vs_StaticMeshList.end(); it++)
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
	for (; it != m_pColliderListPimpl->CapsuleList.end(); it++)
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
	for (; it != m_pColliderListPimpl->Capsule_vs_SphereList.end(); it++)
	{
		if (*it == pCapsule)
		{
			m_pColliderListPimpl->Capsule_vs_SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegmentCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegmentList.begin();
	for (; it != m_pColliderListPimpl->LineSegmentList.end(); it++)
	{
		if (*it == pLineSegment)
		{
			m_pColliderListPimpl->LineSegmentList.erase(it);
			break;
		}
	}
}

void ColliderManager::Clear(LineSegment_vs_StaticMeshCollider *pLineSegment)
{
	auto it = m_pColliderListPimpl->LineSegment_vs_StaticMeshList.begin();
	for (; it != m_pColliderListPimpl->LineSegment_vs_StaticMeshList.end(); it++)
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
	for (; it != m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.end(); it++)
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
	for (; it != m_pColliderListPimpl->StaticMesh_vs_SphereList.end(); it++)
	{
		if (*it == pStaticMesh)
		{
			m_pColliderListPimpl->StaticMesh_vs_SphereList.erase(it);
			break;
		}
	}
}

void ColliderManager::AllClear()
{
	m_pColliderListPimpl->SphereList.clear();
	m_pColliderListPimpl->CapsuleList.clear();
	m_pColliderListPimpl->LineSegmentList.clear();
	m_pColliderListPimpl->Sphere_vs_CapsuleList.clear();
	m_pColliderListPimpl->Sphere_vs_StaticMeshList.clear();
	m_pColliderListPimpl->Capsule_vs_SphereList.clear();
	m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.clear();
	m_pColliderListPimpl->LineSegment_vs_StaticMeshList.clear();
	m_pColliderListPimpl->StaticMesh_vs_SphereList.clear();
}

void ColliderManager::CheckSphere()
{
	auto it = m_pColliderListPimpl->SphereList.begin();
	for (; it != m_pColliderListPimpl->SphereList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != m_pColliderListPimpl->SphereList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckSphere((*it)->m_HitData, (*it2)->m_HitData))
				{
					//それぞれの当たり判定情報を渡す
					static Result_Sphere hitData1;
					static Result_Sphere hitData2;

					hitData1.position = *(*it)->m_HitData.pPosition;
					hitData1.radius = *(*it)->m_HitData.pRadius;

					hitData2.position = *(*it2)->m_HitData.pPosition;
					hitData2.radius = *(*it2)->m_HitData.pRadius;

					/*
					Vector3D v = hitData1.position - hitData2.position;
					float leng = v.Length();
					float radius = hitData1.radius - hitData2.radius;

					float l = abs(radius - leng);
					printf("l=%f\n",l);

					(*(*it)->m_HitData.pPosition) = v.GetNormalize() + hitData1.position;
					//(*(*it2)->m_HitData.pPosition) = v.GetNormalize() * l;
					*/
					(*it)->m_Func(hitData2);
					(*it2)->m_Func(hitData1);
				}
			}
		}
	}
}

void ColliderManager::CheckCapsule()
{
	auto it = m_pColliderListPimpl->CapsuleList.begin();
	for (; it != m_pColliderListPimpl->CapsuleList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != m_pColliderListPimpl->CapsuleList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckCapsule((*it)->m_HitData, (*it2)->m_HitData))
				{
					//それぞれの当たり判定情報を渡す
					static Result_Capsule hitData1;
					static Result_Capsule hitData2;

					hitData1.start = *(*it)->m_HitData.pStart;
					hitData1.end = *(*it)->m_HitData.pEnd;
					hitData1.radius = *(*it)->m_HitData.pRadius;

					hitData2.start = *(*it2)->m_HitData.pStart;
					hitData2.end = *(*it2)->m_HitData.pEnd;
					hitData2.radius = *(*it2)->m_HitData.pRadius;

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
	for (; it != m_pColliderListPimpl->Sphere_vs_CapsuleList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->Capsule_vs_SphereList.begin();
		for (; it2 != m_pColliderListPimpl->Capsule_vs_SphereList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckSphere_vs_Capsule((*it)->m_HitData, (*it2)->m_HitData))
				{
					//それぞれの当たり判定情報を渡す
					static Result_Capsule hitData1;
					static Result_Sphere hitData2;

					hitData1.start = *(*it2)->m_HitData.pStart;
					hitData1.end = *(*it2)->m_HitData.pEnd;
					hitData1.radius = *(*it2)->m_HitData.pRadius;

					hitData2.position = *(*it)->m_HitData.pPosition;
					hitData2.radius = *(*it)->m_HitData.pRadius;

					(*it)->m_Func(hitData1);  //カプセル側の情報
					(*it2)->m_Func(hitData2); //球側の情報
				}
			}
		}
	}
}

void ColliderManager::CheckLineSegment()
{
	auto it = m_pColliderListPimpl->LineSegmentList.begin();
	for (; it != m_pColliderListPimpl->LineSegmentList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = ++it;
		for (it--; it2 != m_pColliderListPimpl->LineSegmentList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				if (HitCheckLineSegment((*it)->m_HitData, (*it2)->m_HitData))
				{
					//それぞれの当たり判定情報を渡す
					static Result_LineSegment hitData1;
					static Result_LineSegment hitData2;

					hitData1.start = *(*it)->m_HitData.pStart;
					hitData1.end = *(*it)->m_HitData.pEnd;

					hitData2.start = *(*it2)->m_HitData.pStart;
					hitData2.end = *(*it2)->m_HitData.pEnd;

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
	for (; it != m_pColliderListPimpl->LineSegment_vs_StaticMeshList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.begin();
		for (; it2 != m_pColliderListPimpl->StaticMesh_vs_LineSegmentList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				Result_Porygon pory;
				if (HitCheckStaticMesh_vs_LineSegment(*(*it2)->m_pStaticMeshInfo, (*it)->m_HitData, &pory))
				{
					(*it)->m_Func(pory);
					//(*it2)->m_Func(&(*it)->m_HitData);
				}
			}
		}
	}
}

void ColliderManager::CheckStaitcMesh_vs_Sphere()
{
	auto it = m_pColliderListPimpl->Sphere_vs_StaticMeshList.begin();
	for (; it != m_pColliderListPimpl->Sphere_vs_StaticMeshList.end(); it++)
	{
		if ((*it)->m_IsSeep) continue;
		auto it2 = m_pColliderListPimpl->StaticMesh_vs_SphereList.begin();
		for (; it2 != m_pColliderListPimpl->StaticMesh_vs_SphereList.end(); it2++)
		{
			if ((*it2)->m_IsSeep) continue;

			//IDが一致したら当たり判定計算開始
			if ((*it)->m_TargetId & (*it2)->m_MyId)
			{
				Result_Porygon_Group pory;
				if (HitCheckStaticMesh_vs_Sphere(*(*it2)->m_pStaticMeshInfo, (*it)->m_HitData, &pory))
				{
					(*it)->m_Func(pory);
					delete[] pory.pArray;
					//(*it2)->m_Func(&(*it)->m_HitData);
				}
			}
		}
	}
}

bool ColliderManager::HitCheckSphere(SphereHitData &pHitData1, SphereHitData &pHitData2)
{
	SphereInfo hit1(*pHitData1.pPosition, *pHitData1.pRadius);
	SphereInfo hit2(*pHitData2.pPosition, *pHitData2.pRadius);

	if (Collision3D::Sphere(hit1, hit2)) return true;
	return false;
}

bool ColliderManager::HitCheckCapsule(CapsuleHitData &pHitData1, CapsuleHitData &pHitData2)
{
	LineSegmentInfo segment1(*pHitData1.pStart, *pHitData1.pEnd);
	LineSegmentInfo segment2(*pHitData2.pStart, *pHitData2.pEnd);

	CapsuleInfo hit1(segment1, *pHitData1.pRadius);
	CapsuleInfo hit2(segment2, *pHitData2.pRadius);

	if (Collision3D::Capsule(hit1, hit2)) return true;
	return false;
}

bool ColliderManager::HitCheckSphere_vs_Capsule(SphereHitData &pHitData1, CapsuleHitData &pHitData2)
{
	SphereInfo hit1(*pHitData1.pPosition, *pHitData1.pRadius);
	LineSegmentInfo segment(*pHitData2.pStart, *pHitData2.pEnd);
	CapsuleInfo hit2(segment, *pHitData2.pRadius);

	if (Collision3D::SphereCapsule(hit1, hit2)) return true;
 
	return false;
}

bool ColliderManager::HitCheckLineSegment(LineSegmentHitData &pHitData1, LineSegmentHitData &pHitData2)
{
	return true;
}

bool ColliderManager::HitCheckStaticMesh_vs_LineSegment(StaticMesh &hitData1, LineSegmentHitData &hitData2, Result_Porygon *pory)
{
	HitResult_SegmentTriangle hit;
	Matrix m = *hitData1.GetLocalMatrix() * *hitData1.GetMatrix(); //モデルの変換行列
	Matrix inverse = m.GetInverse();
	TriangleInfo hitTriangle;
	LineSegmentInfo hitLine(*hitData2.pStart * inverse, *hitData2.pEnd * inverse);
	VertexInfo *ver = hitData1.GetVertex();
	MaterialInfo *material = hitData1.GetMaterial();

	//マテリアルごとにポリゴンとの当たり判定をする
	int materialAllNum = hitData1.GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		int *index = hitData1.GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;
		for (int j = 0; j < faceNum; j++)
		{
			hitTriangle.v1 = ver[index[j * 3]].vPos;
			hitTriangle.v2 = ver[index[j * 3 + 1]].vPos;
			hitTriangle.v3 = ver[index[j * 3 + 2]].vPos;

			hit = Collision3D::LineSegmentTriangle(hitLine, hitTriangle);

			//当たったら当たった頂点格納
			if (hit.isHit)
			{
				pory->contactPos = hit.pos * m;
				pory->normal = Vector3D::Matrix3x3(hit.normal, m).GetNormalize();
				pory->vertexPos[0] = hitTriangle.v1 * m;
				pory->vertexPos[1] = hitTriangle.v2 * m;
				pory->vertexPos[2] = hitTriangle.v3 * m;
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
	Matrix m = *hitData1.GetLocalMatrix() * *hitData1.GetMatrix(); //モデルの変換行列
	TriangleInfo hitTriangle;
	SphereInfo hitSphere(*hitData2.pPosition * m.GetInverse(), *hitData2.pRadius);
	VertexInfo *ver = hitData1.GetVertex();
	MaterialInfo *material = hitData1.GetMaterial();
	std::vector<Vector3D> hitPos;
	std::vector<Vector3D> hitNormal;
	std::vector<int> materialNum;
	std::vector<float> hitDist;
	std::vector<TriangleInfo> hitVer;

	//マテリアルごとにポリゴンとの当たり判定をする
	int materialAllNum = hitData1.GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		int *index = hitData1.GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;
		for (int j = 0; j < faceNum; j++)
		{
			hitTriangle.v1 = ver[index[j * 3]].vPos;
			hitTriangle.v2 = ver[index[j * 3 + 1]].vPos;
			hitTriangle.v3 = ver[index[j * 3 + 2]].vPos;
			Hitdata = Collision3D::SphereTriangle(hitSphere, hitTriangle);

			//当たったら当たった頂点格納
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

	//当たった情報格納
	pory->pArray = new Result_Porygon[hitVer.size()];
	for (unsigned int i = 0; i < hitVer.size(); i++)
	{
		pory->pArray[i].contactPos = hitPos[i] * m;
		pory->pArray[i].normal = Vector3D::Matrix3x3(hitNormal[i], m).GetNormalize();
		pory->pArray[i].vertexPos[0] = hitVer[i].v1 * m;
		pory->pArray[i].vertexPos[1] = hitVer[i].v2 * m;
		pory->pArray[i].vertexPos[2] = hitVer[i].v3 * m;
		pory->pArray[i].materialIndex = materialNum[i];
		pory->pArray[i].dist = hitDist[i];
	}
	pory->hitNum = hitVer.size();

	//仮情報削除
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