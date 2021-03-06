#include "Handgun.h"
#include "../Player/PlayerInfo.h"
#include "../../GameSystem/Effect/EffectAnimation.h"

Handgun* g_pHandgun;

Handgun::Handgun(int* anim, float* frame, bool* take, EWeapons* weapon, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Handgun", 1)
{
	m_Model.SetAsset("Handgun");
	m_Model.SetMeshState(eBlockingLight);

	m_Ammo = DEFAULT_BULLET_HAVE;
	m_Weapon = weapon;
	m_LoadedAmmo = AMMO_LOADED_HANDGUN;
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	//銃の位置
	if (*m_PlayerAnim == eAnim_SetupHandgun || *m_PlayerAnim == eAnim_RecoilHandgun ||
		(*m_PlayerAnim == eAnim_Hit && *m_TakeWeapon))
	{
		//構え状態
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, -2);
	}
	else if ((*m_PlayerAnim == eAnim_TakeHandgun  && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == eAnim_IdleTakeHandgun || *m_PlayerAnim == eAnim_WalkTakeHandgun ||
		*m_PlayerAnim == eAnim_ReloadHandgun || *m_PlayerAnim == eAnim_RunTakeHandgun ||
		*m_PlayerAnim == eAnim_WalkReloadHandgun)
	{
		//手に持っている状態
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else
	{
		//腰につけている状態
		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
	}
	m_Model.SetParentMatirx(*m_pMatirx);
}

void Handgun::Reload()
{
	//手持ち弾数が0より大きい＆銃に入っている弾数が最大装弾数より小さい
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_HANDGUN)
	{
		int temp = AMMO_LOADED_HANDGUN - m_LoadedAmmo;
		if (m_Ammo <= temp)
		{
			m_LoadedAmmo += m_Ammo;
			m_Ammo = 0;
		}
		else
		{
			m_LoadedAmmo += temp;
			m_Ammo -= temp;
		}
		if (m_Ammo < 0)m_Ammo = 0;
	}
}
