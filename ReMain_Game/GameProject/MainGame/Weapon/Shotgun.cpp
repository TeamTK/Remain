#include "Shotgun.h"
#include "../Player/PlayerInfo.h"
#include "../../GameSystem/Effect/EffectAnimation.h"

Shotgun* g_pShotgun;

Shotgun::Shotgun(int* anim, float* frame, bool* take, EWeapons* weapon, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Shotgun", 0)
{
	m_Model.SetAsset("Shotgun", true);

	m_Ammo = DEFAULT_BULLET_HAVE;
	m_Weapon = weapon;
	m_LoadedAmmo = AMMO_LOADED_SHOTGUN;
}

Shotgun::~Shotgun()
{

}

void Shotgun::Update()
{
	//銃の位置
	if (*m_PlayerAnim == eAnim_SetupGun || *m_PlayerAnim == eAnim_RecoilGun ||
		(*m_PlayerAnim == eAnim_Hit && *m_TakeWeapon))
	{
		//構え状態
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, -2);
	}
	else if ((*m_PlayerAnim == eAnim_TakeGun && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == eAnim_IdleTakeGun || *m_PlayerAnim == eAnim_WalkTakeGun ||
		*m_PlayerAnim == eAnim_ReloadGun || *m_PlayerAnim == eAnim_RunTakeGun ||
		*m_PlayerAnim == eAnim_WalkReloadGun)
	{
		//手に持っている状態
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
	}
	else
	{
		//肩にかけている状態
		m_Model.SetTranselate(0.22f, 0.2f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
	}
}


void Shotgun::Reload()
{
	//手持ち弾数が0より大きい＆銃に入っている弾数が最大装弾数より小さい
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_SHOTGUN)
	{
		int num = AMMO_LOADED_SHOTGUN - m_LoadedAmmo;
		m_LoadedAmmo += num;
		m_Ammo -= num;
		if (m_Ammo < 0)m_Ammo = 0;
	}
}