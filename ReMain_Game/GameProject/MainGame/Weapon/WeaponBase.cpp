#include "WeaponBase.h"

WeaponBase::WeaponBase()
{

}

WeaponBase::WeaponBase(int* anim, float* frame, bool* take, Matrix* m, const char *taskName, unsigned int priority) :
	Task(taskName, priority)
{
	m_Model.SetParentMatirx(m);
	m_BoneMtx = m;
	m_PlayerAnim = anim;
	m_TakeWeapon= take;
	m_PlayerAnimFrame = frame;
	m_Model.SetRenderingRegister(true, 2, 0);
}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Update()
{
}

void WeaponBase::Shot()
{

}

int WeaponBase::GetAmmo()
{
	return m_Ammo;
}

int WeaponBase::GetLoadedAmmo()
{
	return m_LoadedAmmo;
}

int WeaponBase::GetAllAmmo()
{
	return	m_Ammo + m_LoadedAmmo;
}

void WeaponBase::SetAmmo(int num)
{
	m_Ammo = num;
}

void WeaponBase::SetLoadedAmmo(int num)
{
	m_LoadedAmmo = num;
}

bool WeaponBase::CanShot()
{
	return m_LoadedAmmo > 0 ? true : false;
}

void WeaponBase::ReduceBullet()
{
	if (m_LoadedAmmo > 0)	m_LoadedAmmo--;
}

void WeaponBase::AddAmmo(int addnum)
{
	m_Ammo += addnum;
}

void WeaponBase::Reload()
{

}

EWeapons WeaponBase::GetSelect()
{
	return *m_Weapon;
}