#include "UI_Reticle.h"


UI_Reticle::UI_Reticle(bool* setupGun) :
	Task("UI_Reticle", 1)
{
	m_RenderTask.Regist(5, REGIST_RENDER_FUNC(UI_Reticle::Draw));

	m_Reticle.SetAsset("Reticle");
	m_Reticle.SetCenter(32, 32);
	m_Reticle.SetAlpha(150);

	m_isSetupWeapon = setupGun;
}

UI_Reticle::~UI_Reticle()
{

}

void UI_Reticle::Update()
{

}

void UI_Reticle::Draw()
{
	m_Reticle.Draw(400, 300);
}
