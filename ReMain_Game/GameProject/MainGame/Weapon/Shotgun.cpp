#include "Shotgun.h"
#include "../enums.h"

Shotgun* g_pShotgun;

Shotgun::Shotgun(int* anim, float* frame, Matrix* m):
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
	//e‚ÌˆÊ’u
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupGun)
	{
		//\‚¦ó‘Ô
		//SetTranselate(ã, ‘O, ¶)
		m_Model.SetTranselate(0.03f, 0.1f, 0.05f);
		m_Model.SetRotationDegree(180, -90, 8);
	}
	else if ((*m_PlayerAnim == EPlayerAnim::eAnim_TakeGun && *m_PlayerAnimFrame >= 15) ||
		*m_PlayerAnim == EPlayerAnim::eAnim_IdleTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_WalkTakeGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_ReloadGun ||
		*m_PlayerAnim == EPlayerAnim::eAnim_RunTakeGun)
	{
		//Žè‚ÉŽ‚Á‚Ä‚¢‚éó‘Ô
		//SetTranselate(¶, ‘O, ‰º)
		m_Model.SetTranselate(0.0f, 0.11f, 0.05f);
		m_Model.SetRotationDegree(200, 0, -80);
	}
	else
	{
		//Œ¨‚É‚©‚¯‚Ä‚¢‚éó‘Ô
		m_Model.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Model.SetRotationDegree(65, 0, 12);
	}
	m_Num.NumDraw(0, 90, m_Ammo);
	m_Num.NumDraw(44, 90, m_LoadedAmmo);
}


void Shotgun::Reload()
{
	//ŽèŽ‚¿’e”‚ª0‚æ‚è‘å‚«‚¢•e‚É“ü‚Á‚Ä‚¢‚é’e”‚ªÅ‘å‘•’e”‚æ‚è¬‚³‚¢
	if (m_Ammo > 0 && m_LoadedAmmo < AMMO_LOADED_SHOTGUN)
	{
		int temp = AMMO_LOADED_SHOTGUN - m_LoadedAmmo;
		m_LoadedAmmo += temp;
		m_Ammo -= temp;
	}
}