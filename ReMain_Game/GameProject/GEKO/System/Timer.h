#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
public:
	Timer();
	~Timer();
	void Start();
	double GetMinute();
	double GetSecond();
	double GetMilliSecond();

private:
	double m_Start;
};

#endif