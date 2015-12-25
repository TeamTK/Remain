#include "WeaponBase.h"

WeaponBase::WeaponBase()
{
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

void WeaponBase::SetPlayerBomeMtx(Matrix* m)
{
	m_BoneMtx = m;
}

void WeaponBase::SetPlayerData(int anim, float frame, Vector3D start, Vector3D end)
{
	m_PlayerAnim = anim;
	m_PlayerAnimFrame = frame;
	m_Start = start;
	m_End = end;

}

int WeaponBase::GetAmmo()
{
	return m_Ammo;
}

bool WeaponBase::CanShot()
{
	return m_Ammo > 0 ? true : false;
}

void WeaponBase::ReduceBullet()
{
	if (m_Ammo > 0)	m_Ammo--;
}