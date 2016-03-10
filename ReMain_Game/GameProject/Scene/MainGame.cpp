#include "MainGame.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../Chapter/Chapter.h"
#include "../Scene/Title.h"
#include "../Scene/NowLoading.h"

//#define BOSS_DEBUG_MODE

MainGame::MainGame() :
	Task("MainGame", 0)
{
#ifndef BOSS_DEBUG_MODE
	new Chapter_1_1();
#endif
#ifdef BOSS_DEBUG_MODE
	new Chapter_1_3();
#endif
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
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	AuditoryManager::GetInstance()->Update();
	TracerouteManager::Update();
}