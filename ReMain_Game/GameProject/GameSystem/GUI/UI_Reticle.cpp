#include "UI_Reticle.h"


UI_Reticle::UI_Reticle(bool* setupGun) :
	Task("UI_Reticle", 1),
	m_isSetupWeapon(setupGun),
	m_isOldSetupWeapon(*setupGun)
{
	if (*m_isSetupWeapon)
	{
		m_Reticle.SetDrawRegister(true, 5, 0);
	}

	m_Reticle.SetAsset("Reticle");
	m_Reticle.SetCenter(32, 32);
	m_Reticle.SetAlpha(150);
	m_Reticle.SetPosition(400, 300);
}

UI_Reticle::~UI_Reticle()
{

}

void UI_Reticle::Update()
{
	if (*m_isSetupWeapon != m_isOldSetupWeapon)
	{
		m_Reticle.SetDrawRegister(*m_isSetupWeapon, 5, 0);
	}
	m_isOldSetupWeapon = *m_isSetupWeapon;
}