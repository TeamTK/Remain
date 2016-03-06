#ifndef _YOU_ARE_DEAD_H_
#define _YOU_ARE_DEAD_H_

#include "..\..\GEKO\GEKO.h"
#include "..\..\GameSystem\ScreenTransfer.h"

class YouAreDead : public Task
{
public:
	YouAreDead(int endTime);
	~YouAreDead();
	void Update();
	void Render();

private:
	void Select();

private:
	enum SelectState
	{
		eNo,
		eRetry,
		eQuit
	};

	bool m_IsDecision;
	int m_EndTime;
	float m_AlphaCnt;
	Vector2D m_SelectPos;
	RenderTask m_Render;
	Timer m_Timer;
	Image m_YouAreDeadImage;
	Image m_RetryImage;
	Image m_QuitImage;
	Image m_SelectImage;
	ScreenTransfer_In m_Transfer_In;
	SelectState m_SelectState;
};

#endif
