#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "Scene.h"
#include "../GameSystem/StageObject.h"
#include "../MainGame/Player.h"
#include "../MainGame/CBullet.h"
#include "../MainGame/CEnemy.h"

class MainGame : public Scene
{
public:
	MainGame();
	~MainGame();
	void Update();
	void Render();

private:
	Player* m_pPlayer;
};

#endif