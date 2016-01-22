#include "MainGame.h"
#include "../GameSystem/GUI/UI_Reticle.h"
#include "../GameSystem/GUI/UI_AmmoNum.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"
#include "../MainGame/AmmoBox/AmmoBox_Handgun.h"
#include "../Chapter/Chapter.h"

MainGame::MainGame() :
	Task("MainGame", 0)
{
	//�A�b�Z�b�gtxt�ǂݍ���
	//StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	//DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_1.txt");

	new Chapter_1_1();

	new AmmoBox_Shotgun(Vector3D(-8.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 7);	//�Ƃ肠������
	new AmmoBox_Handgun(Vector3D(-16.0f, 0.0f, -1.0f), Vector3D(0.0f, 0.0f, 0.0f), 6);	//�Ƃ肠������
	new Player(Vector3D(-48.0f, 0.0f, -11.0f));
	new UI_Reticle();
	new UI_AmmoNum();
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
	AuditoryManager::GetInstance()->Update();

	if (Input::KeyP.Clicked()) //�G�ǉ�(��)
	{
		EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");
	}

	if (Input::KeyO.Clicked()) //�G�폜
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
}