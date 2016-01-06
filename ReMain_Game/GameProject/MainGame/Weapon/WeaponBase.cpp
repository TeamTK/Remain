#include "WeaponBase.h"

WeaponBase::WeaponBase()
{

}

WeaponBase::WeaponBase(int* anim, float* frame, Matrix* m, const char *taskName, unsigned int priority) :
	Task(taskName, priority)
{
	m_BoneMtx = m;
	m_PlayerAnim = anim;
	m_PlayerAnimFrame = frame;
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(WeaponBase::Render));
}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Update()
{
}

void WeaponBase::Render()
{
	Matrix m = *m_Model.GetMatrix();
	m_Model.RenderMatrix(m * *m_BoneMtx);
}

void WeaponBase::Shot()
{

}

int WeaponBase::GetAmmo()
{
	return m_LoadedAmmo + m_Ammo;
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