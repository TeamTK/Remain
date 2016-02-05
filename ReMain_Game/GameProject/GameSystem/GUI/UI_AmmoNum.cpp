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
	//’e–ò”‚ð•\Ž¦
	if (g_pShotgun->GetSelect() == EWeapons::eShotgun)
	{
		m_Num.NumDraw(Vector2D(44, 90), g_pShotgun->GetAmmo());
		m_Num.NumDraw(Vector2D(0, 90), g_pShotgun->GetLoadedAmmo());
	}
	else if (g_pHandgun->GetSelect() == EWeapons::eHandgun)
	{
		m_Num.NumDraw(Vector2D(44, 90), g_pHandgun->GetAmmo());
		m_Num.NumDraw(Vector2D(0, 90), g_pHandgun->GetLoadedAmmo());
	}
	m_Slash.SetDrawPos(220, 0, 239, 31);
	m_Slash.SetSize(20, 32);
	m_Slash.Draw(22, 90);
}
