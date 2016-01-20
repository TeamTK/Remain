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
	//銃の位置
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RecoilGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_Hit)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((*m_PlayerAnim == EPlayerAnim::eAnim_TakeGun && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_ReloadGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RunTakeGun)
	{
		//手に持っている状態
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
}


void Shotgun::Reload()
{
	//手持ち弾数が0より大きい＆銃に入っている弾数が最大装弾数より小さい
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