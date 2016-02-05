#ifndef _YOU_ARE_DEAD_H_
#define _YOU_ARE_DEAD_H_

#include "..\..\GEKO\GEKO.h"

class YouAreDead : public Task
{
public:
	YouAreDead(int endTime);
	~YouAreDead();
	void Update();
	void Render();

private:
	int m_EndTime;
	float m_AlphaCnt;
	RenderTask m_Render;
	Timer m_Timer;
	Image m_YouAreDeadImage;
};

#endif