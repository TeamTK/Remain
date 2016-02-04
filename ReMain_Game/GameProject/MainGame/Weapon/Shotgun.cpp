#include "Shotgun.h"
#include "../Player/PlayerInfo.h"

Shotgun* g_pShotgun;

Shotgun::Shotgun(int* anim, float* frame, bool* take, EWeapons* weapon, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Shotgun", 0)
{
	m_Model.SetAsset("Shotgun");

	m_Ammo = 6;
	m_Weapon = weapon;
	m_LoadedAmmo = AMMO_LOADED_SHOTGUN;
}

Shotgun::~Shotgun()
{

}

void Shotgun::Update()
{
	//�e�̈ʒu
	if (*m_PlayerAnim == eAnim_SetupGun || *m_PlayerAnim == eAnim_RecoilGun ||
		(*m_PlayerAnim == eAnim_Hit && *m_TakeWeapon))
	{
		//�\�����
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((*m_PlayerAnim == eAnim_TakeGun && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == eAnim_IdleTakeGun || *m_PlayerAnim == eAnim_WalkTakeGun ||
		*m_PlayerAnim == eAnim_ReloadGun || *m_PlayerAnim == eAnim_RunTakeGun ||
		*m_PlayerAnim == eAnim_WalkReloadGun)
	{
		//��Ɏ����Ă�����
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
	}
	else
	{
		//���ɂ����Ă�����
		m_Model.SetTranselate(0.22f, 0.2f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
	}
}


void Shotgun::Reload()
{
	//�莝���e����0���傫�����e�ɓ����Ă���e�����ő呕�e����菬����
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_SHOTGUN)
	{
		int num = AMMO_LOADED_SHOTGUN - m_LoadedAmmo;
		m_LoadedAmmo += num;
		m_Ammo -= num;
		if (m_Ammo < 0)m_Ammo = 0;
	}
}