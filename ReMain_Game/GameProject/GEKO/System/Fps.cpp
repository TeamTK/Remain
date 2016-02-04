#include "Fps.h"
#include <Windows.h>

Fps::Fps() :
	m_StartTime(0),
	m_Count(0),
	m_Fps(0),
	N(60),
	FPS(60)
{

}

Fps* const Fps::GetInstance()
{
	static Fps fps;
	return &fps;
}

float Fps::GetFPS()
{
	return GetInstance()->m_Fps;
}

void Fps::SetFps(int fps)
{
	Fps* p_Fps = GetInstance();
	p_Fps->N = fps;
	p_Fps->FPS = fps;
}

void Fps::Update()
{
	Fps* p_Fps = GetInstance();
	//1�t���[���ڂȂ玞�����L��
	if (p_Fps->m_Count == 0) p_Fps->m_StartTime = timeGetTime();

	//60�t���[���ڂȂ畽�ς��v�Z����
	if (p_Fps->m_Count == p_Fps->N)
	{ 
		int t = timeGetTime();
		p_Fps->m_Fps = 1000.f / ((t - p_Fps->m_StartTime) / (float)p_Fps->N);
		p_Fps->m_Count = 0;
		p_Fps->m_StartTime = t;
	}
	p_Fps->m_Count++;
}

void Fps::Wait()
{
	Fps* p_Fps = GetInstance();
	int tookTime = timeGetTime() - p_Fps->m_StartTime;				//������������
	int waitTime = p_Fps->m_Count * 1000 / p_Fps->FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) Sleep(waitTime);								//�ҋ@
}