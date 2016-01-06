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
	//�A�b�Z�b�gtxt�ǂݍ���
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	ImageAsset::LoadFile("TextData\\Image.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	//�G����
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");
	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");

	new AmmoBox_Shotgun(Vector3D(-8.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 7);	//�Ƃ肠������
	new AmmoBox_Handgun(Vector3D(-16.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 6);	//�Ƃ肠������
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

	//TaskManager::DrawName();

	//�G�ǉ�(��)
	if (Input::KeyP.Clicked())
	{
		EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");
	}
}

void MainGame::Render()
{
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}