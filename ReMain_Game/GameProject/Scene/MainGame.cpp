#include "MainGame.h"
#include "..\GameSystem\Effect.h"
#include "..\GameSystem\GUI\UI_Reticle.h"
#include "../GameSystem/StageObject.h"
#include "../MainGame/Player.h"
#include "../MainGame/Bullet/Bullet.h"
#include "../MainGame/Enemy/Enemy.h"
#include "../MainGame/Enemy/Monster_A.h"
#include "../MainGame/Enemy/Monster_B.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"
#include "../MainGame/AmmoBox/AmmoBox_Handgun.h"

MainGame::MainGame()
{
	//アッセットtxt読み込み
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	ImageAsset::LoadFile("TextData\\Image.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	//敵生成
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");
	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");

	new AmmoBox_Shotgun(Vector3D(-8.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 7);	//とりあえず仮
	new AmmoBox_Handgun(Vector3D(-16.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 6);	//とりあえず仮
	new Player();
	new UI_Reticle();
}

MainGame::~MainGame()
{
	SightManager::GetInstance()->AllClear();
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->ClearInstance();
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();

	//敵追加(仮)
	if (Input::KeyP.Clicked())
	{
		EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");
	}

	//敵削除
	if (Input::KeyO.Clicked())
	{
		TaskManager::Kill("Monster_A");
	}

	//敵停止
	if (Input::KeyI.Clicked())
	{
		TaskManager::Stop("Monster_A");
	}

	//敵起動
	if (Input::KeyU.Clicked())
	{
		TaskManager::Start("Monster_A");
	}
}

void MainGame::Render()
{
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}