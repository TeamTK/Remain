#ifndef _SOUND_EFFECT_H_
#define _SOUND_EFFECT_H_

#include "..\..\GEKO\GEKO.h"

class SoundEffect : Task
{
public:
	SoundEffect(const std::string &aseetName, double endTime);
	~SoundEffect();
	void Update();

private:
	double m_EndTime;
	Sound m_Sound;
	Timer m_Timer;
};

#endif