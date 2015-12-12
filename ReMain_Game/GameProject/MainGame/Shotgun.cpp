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
	if (m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_SetupGun && m_pPlayer->GetPlayTime() >= 29)
	{
		//\‚¦ó‘Ô
		m_Model.SetTranselate(0.05f, 0.0f, 0.0f);
		m_Model.SetRotationDegree(180, 90, 30);
		m_BoneMat = m_pPlayer->GetBomeMat(25);
	}
	else
	{
		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(3);
	}
	m_Matrix = *m_Model.GetMatrix();
}