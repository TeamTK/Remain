#include "Shotgun.h"
#include "../enums.h"

Shotgun* g_pShotgun;

Shotgun::Shotgun(int* anim, float* frame, Matrix* m) :
	WeaponBase(anim, frame, m, "Shotgun", 0)
{
	m_Model.SetAsset("Shotgun");

	m_Ammo = 6;
	m_LoadedAmmo = AMMO_LOADED_SHOTGUN;
}

Shotgun::~Shotgun()
{

}

void Shotgun::Update()
{
	//�e�̈ʒu
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RecoilGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_Hit)
	{
		//�\�����
		//SetTranselate(��, �O, ��)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((*m_PlayerAnim == EPlayerAnim::eAnim_TakeGun && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_ReloadGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RunTakeGun)
	{
		//��Ɏ����Ă�����
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
}


void Shotgun::Reload()
{
	//�莝���e����0���傫�����e�ɓ����Ă���e�����ő呕�e����菬����
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_SHOTGUN)
	{
		if (m_Ammo < 6)
		{
			m_LoadedAmmo += m_Ammo;
			m_Ammo = 0;
		}
		else
		{
			int temp = AMMO_LOADED_SHOTGUN - m_LoadedAmmo;
			m_LoadedAmmo += temp;
			m_Ammo -= temp;
			if (m_Ammo < 0)m_Ammo = 0;
		}
	}
}