#ifndef _MENU_H_
#define _MENU_H_

#include "../../Chapter/Chapter.h"

class Menu : public Task
{
public:
	Menu();
	~Menu();
	void Update();

private:
	void Select();
	void StopRendering();

private:
	enum SelectState
	{
		eNo,
		eRetry,
		eQuit
	};

	bool m_IsStop;
	bool m_IsDecision;
	Vector2D m_SelectPos;
	Image m_RetryImage;
	Image m_QuitImage;
	Image m_SelectImage;
	Image m_BackgroundImage;
	ScreenTransfer_In m_Transfer_In;
	SelectState m_SelectState;
	Sound m_OpenSound;
	Sound m_DecisionSound;
	Sound m_CursorSound;
};

#endif