#include "GameClear.h"
#include "../../Scene/Title.h"

#define ALPHA_SPEED 1.0f
#define ALPHA_MAX 200
#define GAME_CLEAR_IMAGE_W 120
#define GAME_CLEAR_IMAGE_H 200

GameClear::GameClear() :
	Task("GameClear", 0),
	m_Alpha(0.0)
{
	m_Transfer_In.Start(3);

	m_GameClearImage.SetDrawRegister(true, 6, 0);
	m_GameClearImage.SetAsset("GameClear");
	m_GameClearImage.SetAlpha(0);
	m_GameClearImage.SetPosition(GAME_CLEAR_IMAGE_W, GAME_CLEAR_IMAGE_H);
}

GameClear::~GameClear()
{
}

void GameClear::Update()
{
	if (m_Alpha >= ALPHA_MAX)
	{
		m_Transfer_In.Update();

		if (m_Transfer_In.GetIsEndTransfer())
		{
			TaskManager::AllKill();
			new Title();
		}
	}
	else
	{
		m_Alpha += ALPHA_SPEED;
		m_GameClearImage.SetAlpha((int)m_Alpha);
	}
}