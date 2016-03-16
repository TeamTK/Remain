#include "MainGame.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"

MainGame::MainGame(ChapterType type) :
	Task("MainGame", 0)
{
	switch (type)
	{
	case eChapter_1_1:
		new Chapter_1_1();
		break;

	case eChapter_1_2:
		new Chapter_1_2();
		break;

	case eChapter_1_3:
		new Chapter_1_3();
		break;

	default:
		break;
	}
}

MainGame::~MainGame()
{
	SightManager::GetInstance()->AllClear();
	AuditoryManager::GetInstance()->AllClaer();
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
	TaskManager::Kill("Chapter_1_2");
	TaskManager::Kill("BloodAnim");
	TaskManager::Kill("MuzzleFlash");
	TaskManager::Kill("Map");
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	AuditoryManager::GetInstance()->Update();
	TracerouteManager::Update();
}