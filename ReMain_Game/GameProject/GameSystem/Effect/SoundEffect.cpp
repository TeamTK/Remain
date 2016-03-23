#include "SoundEffect.h"

SoundEffect::SoundEffect(const std::string &aseetName, double endTime) :
	Task(aseetName.data(), 0),
	m_EndTime(endTime)
{
	m_Sound.SetAseet(aseetName);
	m_Timer.Start();
	//m_Sound.SetVolume(1);
	m_Sound.Play();
}

SoundEffect::~SoundEffect()
{
}

void SoundEffect::Update()
{
	if (m_Timer.GetSecond() > m_EndTime)
	{
		Task::SetKill();
	}
}