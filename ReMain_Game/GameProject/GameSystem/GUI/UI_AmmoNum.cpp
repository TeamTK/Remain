#include "UI_AmmoNum.h"
#include "../../MainGame/Weapon/Shotgun.h"
#include "../../MainGame/Weapon/Handgun.h"

UI_AmmoNum::UI_AmmoNum() :
	Task("UI_AmmoNum", 1)
{
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(UI_AmmoNum::Draw));
	m_Slash.SetAsset("Number");

}

UI_AmmoNum::~UI_AmmoNum()
{

}

void UI_AmmoNum::Update()
{

}

void UI_AmmoNum::Draw()
{
	m_Slash.SetDrawPos(219, 0, 259, 31);
	m_Slash.SetSize(41, 32);
	m_Slash.Draw(110, 41);

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
