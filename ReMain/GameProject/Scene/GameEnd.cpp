#include "GameEnd.h"

GameEnd::GameEnd()
{
}

GameEnd::~GameEnd()
{
}

void GameEnd::Update()
{
	SceneManager::Quit(255, EScene::E_BASE_CAMP);
}

void GameEnd::Render()
{
	std::cout << "GameEnd" << '\n';
}