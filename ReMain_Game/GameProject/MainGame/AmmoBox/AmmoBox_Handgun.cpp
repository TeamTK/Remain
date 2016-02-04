#include "AmmoBox_Handgun.h"

AmmoBox_Handgun::AmmoBox_Handgun(Vector3D pos, Vector3D rot, int addnum):
	AmmoBoxBase(pos, rot, "AmmoBox_Handgun", 0)
{
	m_AddedNum = addnum;
	m_Pos = pos;
	m_Box.SetAsset("AmmoBox");
	m_Box.SetTranselate(m_Pos);
	m_Box.SetRotationRadian(rot.x, rot.y, rot.z);

	//ÉvÉåÉCÉÑÅ[Ç∆ÇÃìñÇΩÇËîªíË
	m_Collider.Regist_S_vs_S(&m_Pos, &m_Radius, REGIST_FUNC(AmmoBoxBase::HitPlayer));
	m_Collider.SetID(eHITID3, eHITID1);

	m_Collider.SetName("AmmoBox_Handgun_12");
}

AmmoBox_Handgun::~AmmoBox_Handgun()
{

}

void AmmoBox_Handgun::Update()
{
	Fiqure::RenderBillboard(m_Pos + Vector3D(0.0f, 1.5f, 0.0f), 0.3f, "UI_Handgun");
}