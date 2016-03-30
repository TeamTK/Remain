#include "UI_AmmoNum.h"
#include "../../MainGame/Weapon/Shotgun.h"
#include "../../MainGame/Weapon/Handgun.h"

UI_AmmoNum::UI_AmmoNum() :
	Task("UI_AmmoNum", 1)
{
	m_Slash.SetDrawRegister(true, 5, 0);
	m_Slash.SetAsset("Number");
	m_Slash.SetDrawArea(219, 0, 259, 31);
	m_Slash.SetSize(41, 32);
	m_Slash.SetPosition(110, 41);
}

UI_AmmoNum::~UI_AmmoNum()
{

}

void UI_AmmoNum::Update()
{
	//’e–ò”‚ð•\Ž¦
	if (g_pShotgun->GetSelect() == EWeapons::eShotgun)
	{
		m_Num.NumDraw(Vector2D(130, 60), g_pShotgun->GetAmmo(), true);
		m_Num.NumDraw(Vector2D(130, 19), g_pShotgun->GetLoadedAmmo(), true);
	}
	else if (g_pHandgun->GetSelect() == EWeapons::eHandgun)
	{
		m_Num.NumDraw(Vector2D(130, 60), g_pHandgun->GetAmmo(), true);
		m_Num.NumDraw(Vector2D(130, 19), g_pHandgun->GetLoadedAmmo(), true);
	}
}