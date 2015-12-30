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
		//�\�����
		//SetTranselate(��, �O, ��)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((m_PlayerAnim == EPlayerAnim::eAnim_TakeGun && m_PlayerAnimFrame >= 15) ||
		m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeGun ||
		m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeGun ||
		m_PlayerAnim == EPlayerAnim::eAnim_RunTakeGun)
	{
		//�\�����
		//SetTranselate(��, �O, ��)
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
	}
	else
	{
		//���ɂ����Ă�����
		m_Model.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
	}
	m_Matrix = *m_Model.GetMatrix();
}
