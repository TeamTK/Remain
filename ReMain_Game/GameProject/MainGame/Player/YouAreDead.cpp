#include "YouAreDead.h"
#include "../../Scene/GameOvar.h"

YouAreDead::YouAreDead(int endTime) :
	Task("YouAreDead", 3),
	m_EndTime(endTime),
	m_AlphaCnt(0)
{
	m_Render.Regist(0, REGIST_RENDER_FUNC(YouAreDead::Render));
	m_Timer.Start();
	m_YouAreDeadImage.SetAsset("YouAreDead");
	m_YouAreDeadImage.SetCenter(m_YouAreDeadImage.GetWidth() / 2, m_YouAreDeadImage.GetHeight() / 2);
	m_YouAreDeadImage.SetAlpha(0);
}

YouAreDead::~YouAreDead()
{
}

void YouAreDead::Update()
{
	if (m_Timer.GetSecond() > m_EndTime)
	{
		TaskManager::Kill("MainGame");
		TaskManager::Kill("Player");
		new GameOvar();
		SetKill();
	}

	m_YouAreDeadImage.SetAlpha((int)m_AlphaCnt);
	m_AlphaCnt += 1.2f;
}

void YouAreDead::Render()
{
	m_YouAreDeadImage.Draw(800 / 2, 600 / 2);
}