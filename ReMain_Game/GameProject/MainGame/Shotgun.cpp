#include "Shotgun.h"
#include "enums.h"

Shotgun::Shotgun()
{
	m_Model.SetAsset("Shotgun");

	m_Ammo = 14;
}

Shotgun::~Shotgun()
{

}

void Shotgun::Update()
{
	if (m_PlayerAnim == EPlayerAnim::eAnim_SetupGun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((m_PlayerAnim == EPlayerAnim::eAnim_TakeGun && m_PlayerAnimFrame >= 15) ||
		m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeGun ||
		m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeGun ||
		m_PlayerAnim == EPlayerAnim::eAnim_RunTakeGun)
	{
		//構え状態
		//SetTranselate(左, 前, 下)
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
	}
	else
	{
		//肩にかけている状態
		m_Model.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
	}
	m_Matrix = *m_Model.GetMatrix();
}
