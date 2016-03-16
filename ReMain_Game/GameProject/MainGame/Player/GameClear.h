#ifndef _GAME_CLEAR_H_
#define _GAME_CLEAR_H_

#include "../../GEKO/GEKO.h"
#include "../../GameSystem/ScreenTransfer.h"

class GameClear : public Task
{
public:
	GameClear();
	~GameClear();
	void Update();
	void Render();

private:
	float m_Alpha;
	RenderTask m_Render;
	ScreenTransfer_In m_Transfer_In;
	Image m_GameClearImage;
};

#endif