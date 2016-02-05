#include "ScreenBlood.h"



ScreenBlood::ScreenBlood() :
	Task("ScreenBlood", 1),
	m_Alpha(255)
{
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(ScreenBlood::Draw));
	m_Blood.SetAsset("ScreenBlood");
	m_Timer.Start();
}

ScreenBlood::~ScreenBlood()
{

}

void ScreenBlood::Update()
{
	if (m_Timer.GetSecond() > 3.0)
	{
		m_Alpha -= 1;
	}

	if (m_Alpha <= 0)
	{
		Task::SetKill();
	}
}

void ScreenBlood::Draw()
{
	m_Blood.SetAlpha(m_Alpha);
	m_Blood.Draw(0, 0);
}
