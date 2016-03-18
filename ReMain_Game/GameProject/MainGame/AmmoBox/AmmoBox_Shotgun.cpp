#include "AmmoBox_Shotgun.h"

AmmoBox_Shotgun::AmmoBox_Shotgun(Vector3D pos, Vector3D rot, int addnum):
	AmmoBoxBase(pos, rot, "AmmoBox_Shotgun", 0)
{
	m_AddedNum = addnum;
	m_Pos = pos;
	m_Box.SetAsset("AmmoBox");
	m_Box.SetTranselate(m_Pos);
	m_Box.SetRotationRadian(rot.x, rot.y, rot.z);

	//プレイヤーとの当たり判定
	m_Collider.Regist_S_vs_S(&m_Pos, &m_Radius, REGIST_FUNC(AmmoBoxBase::HitPlayer));
	m_Collider.SetID(eHITID2, eHITID1);

	m_Collider.SetName("AmmoBox_Shotgun_12");
}

AmmoBox_Shotgun::~AmmoBox_Shotgun()
{

}

void AmmoBox_Shotgun::Update()
{
	Fiqure::RenderBillboard(m_Pos + Vector3D(0.0f, 1.0f, 0.0f), 0.3f, "UI_Shotgun");
}