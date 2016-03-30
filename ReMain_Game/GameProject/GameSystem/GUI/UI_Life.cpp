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
	m_LifeImage.SetDrawRegister(true, 5, 0);
	m_LifeFrameImage.SetDrawRegister(true, 5, 1);
	m_LifeBackImage.SetDrawRegister(true, 5, 2);

	m_LifeImage.SetAsset("UI_Life");
	m_LifeFrameImage.SetAsset("UI_LifeFrame");
	m_LifeBackImage.SetAsset("UI_Life");
	m_LifeBackImage.SetRGB(0, 0, 0);

	m_LifeImage.SetSize((int)(UI_LIFE_DRAW_MAX * (*m_pHp / m_HpMax)), m_LifeImage.GetHeight());
	m_LifeBackImage.SetPosition(UI_LIFE_IMAGE_POS);
	m_LifeImage.SetPosition(UI_LIFE_IMAGE_POS);
	m_LifeFrameImage.SetPosition(UI_LIFE_FRAME_IMAGE_POS);
}

UI_Life::~UI_Life()
{

}

void UI_Life::Update()
{
	m_LifeImage.SetSize((int)(UI_LIFE_DRAW_MAX * (*m_pHp / m_HpMax)), m_LifeImage.GetHeight());
}