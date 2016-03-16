#include "UI_Life.h"

#define UI_LIFE_IMAGE_POS 700, 100
#define UI_LIFE_MAX 100.0f
#define UI_LIFE_DRAW_MAX 200.0f

UI_Life::UI_Life() :
	m_hp(0.0f)
{
	m_Render.Regist(6, REGIST_RENDER_FUNC(UI_Life::Render));
	m_LifeImage.SetAsset("UI_Life");
	m_LifeImage.SetCenter(m_LifeImage.GetWidth(), 0);
}

UI_Life::~UI_Life()
{

}

void UI_Life::SetIsHPDraw(bool isDraw)
{
	if (isDraw) m_Render.Sleep();
	else        m_Render.Awake();
}

void UI_Life::SetHp(float hp)
{
	m_hp = hp;
}

void UI_Life::Update()
{
	float t = 1.0f - (m_hp / UI_LIFE_MAX);

	m_LifeImage.SetDrawPos(UI_LIFE_DRAW_MAX * t, 0, m_LifeImage.GetWidth(), m_LifeImage.GetHeight());
}

void UI_Life::Render()
{
	m_LifeImage.Draw(UI_LIFE_IMAGE_POS);
}