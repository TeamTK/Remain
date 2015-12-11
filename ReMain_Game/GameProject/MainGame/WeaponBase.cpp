#include "WeaponBase.h"

WeaponBase::WeaponBase()
{
}

WeaponBase::WeaponBase(Player* p)
{
	m_pPlayer = p;
}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Update()
{

}

void WeaponBase::Render()
{
	m_Model.RenderMatrix(*m_Model.GetMatrix() * m_BoneMat);
}

Matrix WeaponBase::GetMatrix()
{
	return m_Matrix;
}