#include "MainGame.h"
#include "..\GameSystem\Effect.h"
#include "..\GameSystem\GUI\UI_Reticle.h"
#include "../GameSystem/StageObject.h"
#include "../MainGame/Player.h"
#include "../MainGame/Bullet.h"
#include "../MainGame/Enemy/Enemy.h"
#include "../MainGame/Enemy/Monster_A.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"
#include "../MainGame/AmmoBox/AmmoBox_Handgun.h"

MainGame::MainGame()
{
	//�A�b�Z�b�gtxt�ǂݍ���
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	ImageAsset::LoadFile("TextData\\Image.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	new Monster_A(Vector3D(-9.0f, 0.0f, 7.7f), Vector3D(0.0f, 0.0f, 0.0f));
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
		new Monster_A(Vector3D(-7.0f, 0.0f, 7.7f), Vector3D(0.0f, -2.8f, 0.0f));
	}
}

void MainGame::Render()
{
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}