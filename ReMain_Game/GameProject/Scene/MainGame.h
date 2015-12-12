#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "Scene.h"
#include "../GameSystem/StageObject.h"
#include "../MainGame/Player.h"
#include "../MainGame/CBullet.h"
#include "../MainGame/CEnemy.h"
#include "../MainGame/Shotgun.h"
#include "../MainGame/Handgun.h"

class MainGame : public Scene
{
public:
	MainGame();
	~MainGame();
	void Update();
	void Render();

private:
	Player* m_pPlayer;
	Shotgun* m_pShotgun;
	Handgun* m_pHandgun;
};

#endif