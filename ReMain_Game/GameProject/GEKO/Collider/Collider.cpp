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
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	return m_pBaseCollider->GetSleep();
}

unsigned int Collider::GetMyID() const
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	return m_pBaseCollider->GetMyID();
}

unsigned int Collider::GetTargetID() const
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	return m_pBaseCollider->GetTargetID();
}

//‹…‘Î‹…‚Ì“–‚½‚è”»’è
void Collider::Regist_S_vs_S(Vector3D *pPos, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new SphereCollider(pPos, pRadius, func);
}

//‹…‘ÎƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è
void Collider::Regist_S_vs_C(Vector3D *pPos, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new Sphere_vs_CapsuleCollider(pPos, pRadius, func);
}

//‹…‘Îü•ª‚Ì“–‚½‚è”»’è
void Collider::Regist_S_vs_L(Vector3D *pPos, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new Sphere_vs_LineSegmentCollider(pPos, pRadius, func);
}

//‹…‘ÎStaticMesh‚Ì“–‚½‚è”»’è
void Collider::Regist_S_vs_SMesh(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new Sphere_vs_StaticMeshCollider(pPos, pRadius, func);
}

//ƒJƒvƒZƒ‹‘ÎƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è
void Collider::Regist_C_vs_C(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new CapsuleCollider(pStart, pEnd, pRadius, func);
}

//ƒJƒvƒZƒ‹‘Î‹…‚Ì“–‚½‚è”»’è
void Collider::Regist_C_vs_S(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new Capsule_vs_SphereCollider(pStart, pEnd, pRadius, func);
}

//ƒJƒvƒZƒ‹‘Î‹…‚Ì“–‚½‚è”»’è
void Collider::Regist_C_vs_L(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new Capsule_vs_LineSegmentCollider(pStart, pEnd, pRadius, func);
}

//ü•ª‘Îü•ª‚Ì“–‚½‚è”»’è
void Collider::Regist_L_vs_L(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_LineSegment &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new LineSegmentCollider(pStart, pEnd, func);
}

//ü•ª‘Î‹…‚Ì“–‚½‚è”»’è
void Collider::Regist_L_vs_S(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Sphere &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new LineSegment_vs_SphereCollider(pStart, pEnd, func);
}

//ü•ª‘ÎƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è
void Collider::Regist_L_vs_C(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Capsule &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new LineSegment_vs_CapsuleCollider(pStart, pEnd, func);
}

void Collider::Regist_L_vs_SMesh(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon &)> func)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new LineSegment_vs_StaticMeshCollider(pStart, pEnd, func);
}

void Collider::Regist_SMesh_vs_L(StaticMesh *pStaticMesh)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new StaticMesh_vs_LineSegmentCollider(pStaticMesh);
}

void Collider::Regist_SMesh_vs_S(StaticMesh *pStaticMesh)
{
	assert((m_pBaseCollider == nullptr) && "Collider‚É‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pBaseCollider = new StaticMesh_vs_SphereCollider(pStaticMesh);
}

void Collider::SetID(unsigned int myId, unsigned int targetId)
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	m_pBaseCollider->SetID(myId, targetId);
}

void Collider::SetName(const char *name)
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	m_pBaseCollider->SetName(name);
}

void Collider::Awake()
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
	m_pBaseCollider->Awake();
}

void Collider::Sleep()
{
	assert((m_pBaseCollider != nullptr) && "Collider‚Í–¢“o˜^‚Å‚·");
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