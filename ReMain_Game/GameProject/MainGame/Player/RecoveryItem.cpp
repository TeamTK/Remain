#include "RecoveryItem.h"

RecoveryItem::RecoveryItem(float x, float y, float z) :
	Task("RecoveryItem", 0),
	m_Radius(0.7f)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_Render.Regist(3, REGIST_RENDER_FUNC(RecoveryItem::Render));
	m_HitSphere.Regist_S_vs_S(&m_Pos, &m_Radius, REGIST_FUNC(RecoveryItem::HitPlayer));
	m_HitSphere.SetID(eHITID5, eHITID1);
}

RecoveryItem::~RecoveryItem()
{
}

void RecoveryItem::Update()
{

}

void RecoveryItem::Render()
{
	Fiqure::RenderBillboard(m_Pos, 1.0f, "UI_None");
}

void RecoveryItem::HitPlayer(Result_Sphere data)
{
	Task::SetKill();
}