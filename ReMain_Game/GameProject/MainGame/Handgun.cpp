#include "Handgun.h"
#include "enums.h"

Handgun* g_pHandgun;

Handgun::Handgun(int* anim, float* frame, Matrix* m) :
	WeaponBase(anim, frame, m, "Handgun", 0)
{
	m_Model.SetAsset("Handgun");

	m_Ammo = 6;
	m_LoadedAmmo = AMMO_LOADED_HANDGUN;
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	//銃の位置
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupHandgun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else if ((*m_PlayerAnim == EPlayerAnim::eAnim_TakeHandgun  && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeHandgun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeHandgun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RunTakeHandgun)
	{
		//手に持っている状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else
	{
		//腰につけている状態
		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
	}
}

void Handgun::Reload()
{
	//手持ち弾数が0より大きい＆銃に入っている弾数が最大装弾数より小さい
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_HANDGUN)
	{
		int temp = AMMO_LOADED_HANDGUN - m_LoadedAmmo;
		m_LoadedAmmo += temp;
		m_Ammo -= temp;
	}
}
