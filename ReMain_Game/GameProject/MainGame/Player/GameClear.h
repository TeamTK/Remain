#ifndef _GAME_CLEAR_H_
#define _GAME_CLEAR_H_

#include "../../GameSystem/ScreenTransfer.h"

class GameClear : public Task
{
public:
	GameClear();
	~GameClear();
	void Update();

private:
	float m_Alpha;
	ScreenTransfer_In m_Transfer_In;
	Image m_GameClearImage;
};

#endif