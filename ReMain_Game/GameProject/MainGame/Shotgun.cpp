#include "Shotgun.h"

Shotgun::Shotgun()
{
}

Shotgun::Shotgun(Player* p)
{
	m_Model.SetAsset("Shotgun");
	m_pPlayer = p;
}

Shotgun::~Shotgun()
{

}

void Shotgun::Update()
{
	if (m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_SetupGun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else if (m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_TakeGun || 
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_IdleTakeGun ||
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_WalkTakeGun ||
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_RunTakeGun)
	{
		//構え状態
		//SetTranselate(左, 前, 下)
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else
	{
		//肩にかけている状態
		m_Model.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
		m_BoneMat = m_pPlayer->GetBomeMat(21);
	}
	m_Matrix = *m_Model.GetMatrix();
}