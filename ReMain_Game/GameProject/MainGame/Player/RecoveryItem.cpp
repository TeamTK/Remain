#include "RecoveryItem.h"

RecoveryItem::RecoveryItem(float x, float y, float z) :
	Task("RecoveryItem", 0),
	m_Radius(0.7f)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_HitSphere.Regist_S_vs_S(&m_Pos, &m_Radius, REGIST_FUNC(RecoveryItem::HitPlayer));
	m_HitSphere.SetID(eHITID5, eHITID1);

	m_Model.SetRenderingRegister(true, 3, 0);
	m_Model.SetAsset("RecoveryItem");
	m_Model.SetTranselate(m_Pos);

	m_ItemBillboard.SetRenderingRegister(true, 3, 0);
	m_ItemBillboard.SetImageAsset("UI_RecoveryItem");
	m_ItemBillboard.SetPosition(m_Pos + Vector3D(0.0f, 1.0f, 0.0f));
	m_ItemBillboard.SetSize(0.3f);
}

RecoveryItem::~RecoveryItem()
{
}

void RecoveryItem::Update()
{

}

void RecoveryItem::HitPlayer(Result_Sphere data)
{
	Task::SetKill();
}