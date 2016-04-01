#include "Handgun.h"
#include "../Player/PlayerInfo.h"
#include "../../GameSystem/Effect/EffectAnimation.h"

Handgun* g_pHandgun;

Handgun::Handgun(int* anim, float* frame, bool* take, EWeapons* weapon, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Handgun", 1)
{
	m_Model.SetAsset("Handgun");
	m_Model.SetMeshState(eBlockingLight | eParentMatirx);

	m_Ammo = DEFAULT_BULLET_HAVE;
	m_Weapon = weapon;
	m_LoadedAmmo = AMMO_LOADED_HANDGUN;
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	//e‚ÌˆÊ’u
	if (*m_PlayerAnim == eAnim_SetupHandgun || *m_PlayerAnim == eAnim_RecoilHandgun ||
		(*m_PlayerAnim == eAnim_Hit && *m_TakeWeapon))
	{
		//\‚¦ó‘Ô
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, -2);
	}
	else if ((*m_PlayerAnim == eAnim_TakeHandgun  && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == eAnim_IdleTakeHandgun || *m_PlayerAnim == eAnim_WalkTakeHandgun ||
		*m_PlayerAnim == eAnim_ReloadHandgun || *m_PlayerAnim == eAnim_RunTakeHandgun ||
		*m_PlayerAnim == eAnim_WalkReloadHandgun)
	{
		//Žè‚ÉŽ‚Á‚Ä‚¢‚éó‘Ô
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 0);
	}
	else
	{
		//˜‚É‚Â‚¯‚Ä‚¢‚éó‘Ô
		m_Model.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Model.SetRotationDegree(10, 100, 0);
	}
}

void Handgun::Reload()
{
	//ŽèŽ‚¿’e”‚ª0‚æ‚è‘å‚«‚¢•e‚É“ü‚Á‚Ä‚¢‚é’e”‚ªÅ‘å‘•’e”‚æ‚è¬‚³‚¢
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
