#include "Collider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "LineSegment.h"
#include "StaticMeshCollider.h"
#include <assert.h>

Collider::Collider() :
	m_pBaseCollider(nullptr)
{
}

Collider::~Collider()
{
	if (m_pBaseCollider != nullptr)
	{
		delete m_pBaseCollider;
	}
}

bool Collider::GetSleep() const
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	return m_pBaseCollider->GetSleep();
}

unsigned int Collider::GetMyID() const
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	return m_pBaseCollider->GetMyID();
}

unsigned int Collider::GetTargetID() const
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	return m_pBaseCollider->GetTargetID();
}

const char* Collider::GetName() const
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	return m_pBaseCollider->GetName();
}

//球対球の当たり判定
void Collider::Regist_S_vs_S(Vector3D *pPos, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new SphereCollider(pPos, pRadius, func);
}

//球対カプセルの当たり判定
void Collider::Regist_S_vs_C(Vector3D *pPos, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new Sphere_vs_CapsuleCollider(pPos, pRadius, func);
}

//球対線分の当たり判定
void Collider::Regist_S_vs_L(Vector3D *pPos, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new Sphere_vs_LineSegmentCollider(pPos, pRadius, func);
}

//球対StaticMeshの当たり判定
void Collider::Regist_S_vs_SMesh(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new Sphere_vs_StaticMeshCollider(pPos, pRadius, func);
}

//カプセル対カプセルの当たり判定
void Collider::Regist_C_vs_C(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new CapsuleCollider(pStart, pEnd, pRadius, func);
}

//カプセル対球の当たり判定
void Collider::Regist_C_vs_S(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new Capsule_vs_SphereCollider(pStart, pEnd, pRadius, func);
}

//カプセル対球の当たり判定
void Collider::Regist_C_vs_L(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new Capsule_vs_LineSegmentCollider(pStart, pEnd, pRadius, func);
}

//線分対線分の当たり判定
void Collider::Regist_L_vs_L(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new LineSegmentCollider(pStart, pEnd, func);
}

//線分対球の当たり判定
void Collider::Regist_L_vs_S(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new LineSegment_vs_SphereCollider(pStart, pEnd, func);
}

//線分対カプセルの当たり判定
void Collider::Regist_L_vs_C(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new LineSegment_vs_CapsuleCollider(pStart, pEnd, func);
}

//線分対スタティックメッシュの当たり判定
void Collider::Regist_L_vs_SMesh(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new LineSegment_vs_StaticMeshCollider(pStart, pEnd, func);
}

//線分対スタティックメッシュの当たり判定（複数の結果）
void Collider::Regist_LGroup_vs_SMesh(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon_Group_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new LineSegment_Group_vs_StaticMeshCollider(pStart, pEnd, func);
}

void Collider::Regist_SMesh_vs_L(StaticMesh *pStaticMesh, bool isGPU)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new StaticMesh_vs_LineSegmentCollider(pStaticMesh, isGPU);
}

void Collider::Regist_SMesh_vs_S(StaticMesh *pStaticMesh)
{
	assert((m_pBaseCollider == nullptr) && "Colliderにすでに登録されています");
	m_pBaseCollider = new StaticMesh_vs_SphereCollider(pStaticMesh);
}

void Collider::SetID(unsigned int myId, unsigned int targetId)
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	m_pBaseCollider->SetID(myId, targetId);
}

void Collider::SetName(const char *name)
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	m_pBaseCollider->SetName(name);
}

void Collider::Awake()
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	m_pBaseCollider->Awake();
}

void Collider::Sleep()
{
	assert((m_pBaseCollider != nullptr) && "Colliderは未登録です");
	m_pBaseCollider->Sleep();
}

void Collider::Release()
{
	if (m_pBaseCollider != nullptr)
	{
		delete m_pBaseCollider;
		m_pBaseCollider = nullptr;
	}
}