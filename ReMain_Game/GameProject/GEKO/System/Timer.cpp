#include "Timer.h"
#include <windows.h>

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
	m_Start = GetTickCount();
}

double Timer::GetMinute()
{
	return GetSecond() * 0.016666f;
}

double Timer::GetSecond()
{
	return GetMilliSecond() * 0.001f;
}

double Timer::GetMilliSecond()
{
	return GetTickCount() - m_Start;
}