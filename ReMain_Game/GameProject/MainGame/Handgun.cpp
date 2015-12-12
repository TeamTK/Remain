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
	if (m_pPlayer->GetAnim() == EPlayerAnimation::eAnim_SetupHandgun && m_pPlayer->GetPlayTime() >= 29)
	{
		//\‚¦ó‘Ô
		m_Model.SetTranselate(0.05f, 0.05f, 0.0f);
		m_Model.SetRotationDegree(175, -92, 30);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else
	{
		//Œ¨‚É‚©‚¯‚Ä‚¢‚éó‘Ô
		m_Model.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
		m_BoneMat = m_pPlayer->GetBomeMat(21);
	}
	m_Matrix = *m_Model.GetMatrix();
}