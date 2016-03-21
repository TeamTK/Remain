#include "UI_Life.h"

#define UI_LIFE_IMAGE_POS 35, 115
#define UI_LIFE_FRAME_IMAGE_POS 15, 115
#define UI_LIFE_MAX 100.0f
#define UI_LIFE_DRAW_MAX 200.0f

UI_Life::UI_Life(float hpMax, float* pHp) :
	Task("UI_Life", 0),
	m_HpMax(hpMax),
	m_pHp(pHp)
{
	m_Render.Regist(5, REGIST_RENDER_FUNC(UI_Life::Render));
	m_LifeImage.SetAsset("UI_Life");
	m_LifeFrameImage.SetAsset("UI_LifeFrame");
	m_LifeBackImage.SetAsset("UI_Life");
	m_LifeBackImage.SetRGB(0, 0, 0);
}

UI_Life::~UI_Life()
{

}

void UI_Life::Update()
{
	m_LifeImage.SetSize((int)(UI_LIFE_DRAW_MAX * (*m_pHp / m_HpMax)), m_LifeImage.GetHeight());
}

void UI_Life::Render()
{
	m_LifeBackImage.Draw(UI_LIFE_IMAGE_POS);
	m_LifeImage.Draw(UI_LIFE_IMAGE_POS);
	m_LifeFrameImage.Draw(UI_LIFE_FRAME_IMAGE_POS);
}