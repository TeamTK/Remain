#ifndef _CHAPTER_1_1_MARAUD_H_
#define _CHAPTER_1_1_MARAUD_H_

#include "EventSceneBase.h"

class Chapter_1_1_Maraud : public EventSceneBase
{
public:
	Chapter_1_1_Maraud();
	~Chapter_1_1_Maraud();
	void Update();

private:
	Timer m_Timer;

};

#endif