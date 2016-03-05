#include "Fps.h"
#include <Windows.h>

Fps::Fps() :
	m_StartTime(0),
	m_Count(0),
	m_Fps(0),
	m_OneFps(0.0f),
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

float Fps::GetOneFPS()
{
	return GetInstance()->m_OneFps;
}

int Fps::GetSettingFPS() 
{
	return GetInstance()->FPS;
}

void Fps::SetFps(int fps)
{
	Fps* p_Fps = GetInstance();
	p_Fps->FPS = fps;
}

void Fps::Update()
{
	Fps* p_Fps = GetInstance();
	//1フレーム目なら時刻を記憶
	if (p_Fps->m_Count == 0) p_Fps->m_StartTime = timeGetTime();

	//指定のフレーム目なら平均を計算する
	if (p_Fps->m_Count == p_Fps->FPS)
	{ 
		int t = timeGetTime();
		p_Fps->m_Fps = 1000.f / ((t - p_Fps->m_StartTime) / (float)p_Fps->FPS);
		p_Fps->m_Count = 0;
		p_Fps->m_StartTime = t;
	}
	p_Fps->m_Count++;

	//現在のfpsが0だったら設定したfpsで1フレームを求める
	if (p_Fps->m_Fps == 0.0f)
	{
		p_Fps->m_OneFps = 1 / (float)p_Fps->FPS;
	}
	else
	{
		p_Fps->m_OneFps = 1 / p_Fps->m_Fps;
	}
}

void Fps::Wait()
{
	Fps* p_Fps = GetInstance();
	int tookTime = timeGetTime() - p_Fps->m_StartTime;				//かかった時間
	int waitTime = p_Fps->m_Count * 1000 / p_Fps->FPS - tookTime;	//待つべき時間
	if (waitTime > 0) Sleep(waitTime);								//待機
}