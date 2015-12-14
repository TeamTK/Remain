#include "Handgun.h"

Handgun::Handgun()
{
}

Handgun::Handgun(Player* p)
{
	m_Model.SetAsset("Handgun");
	m_pPlayer = p;
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	if (m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_SetupHandgun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else if ((m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_TakeHandgun  && m_pPlayer->GetPlayTime() >= 15) ||
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_IdleTakeHandgun ||
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_WalkTakeHandgun ||
			 m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_RunTakeHandgun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else
	{
		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(3);
	}
	m_Matrix = *m_Model.GetMatrix();
}