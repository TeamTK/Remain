#include "Handgun.h"
#include "enums.h"

Handgun::Handgun()
{
	m_Model.SetAsset("Handgun");

	m_Ammo = 6;
	m_Collider.Regist_L_vs_C(&m_Start, &m_End, REGIST_FUNC(Handgun::Hit));
	m_Collider.SetID(eHITID3, eHITID2);
	m_Collider.Sleep();
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	if (m_PlayerAnim == EPlayerAnim::eAnim_SetupHandgun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else if ((m_PlayerAnim == EPlayerAnim::eAnim_TakeHandgun  && m_PlayerAnimFrame >= 15) ||
			 m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeHandgun ||
			 m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeHandgun ||
			 m_PlayerAnim == EPlayerAnim::eAnim_RunTakeHandgun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else
	{

		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
	}
	m_Matrix = *m_Model.GetMatrix();
}

void Handgun::Hit(Result_Capsule &r)
{
	m_Collider.Sleep();
}

void Handgun::Shot()
{
	m_Collider.Awake();
}