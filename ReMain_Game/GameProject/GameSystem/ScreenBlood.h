#ifndef SCREENBLOOD_H
#define SCREENBLOOD_H

#include "../GEKO/GEKO.h"

class ScreenBlood :public Task
{
public:
	ScreenBlood();
	~ScreenBlood();
	void Update();
	void Draw();

private:
	Image m_Blood;
	Timer m_Timer;
	int m_Alpha;
	RenderTask m_RenderTask;

};

#endif