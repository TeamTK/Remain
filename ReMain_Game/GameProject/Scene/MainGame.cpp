#include "MainGame.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../Chapter/Chapter.h"
#include "../Scene/Title.h"

MainGame::MainGame() :
	Task("MainGame", 0)
{

	new Chapter_1_1();
}

MainGame::~MainGame()
{
	SightManager::GetInstance()->AllClear();
	StageObjectManager::GetInstance()->ClearList();
	TracerouteManager::AllClear();
	TaskManager::Kill("UI_Reticle");
	TaskManager::Kill("UI_AmmoNum");
	TaskManager::Kill("AmmoBox_Shotgun");
	TaskManager::Kill("AmmoBox_Handgun");
	TaskManager::Kill("Monster_A");
	TaskManager::Kill("Monster_B");
	TaskManager::Kill("Handgun");
	TaskManager::Kill("Shotgun");
	TaskManager::Kill("Chapter_1_1");
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	AuditoryManager::GetInstance()->Update();
	TracerouteManager::Update();

	//‰¼
	if (Input::KeyP.Clicked())
	{
		TaskManager::AllKill();
		new Title();
	}
}