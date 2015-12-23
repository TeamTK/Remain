#include "MainGame.h"
#include "..\GameSystem\Effect.h"
#include "..\GameSystem\GUI\UI_Reticle.h"

MainGame::MainGame()
{
	//�A�b�Z�b�gtxt�ǂݍ���
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	ImageAsset::LoadFile("TextData\\Image.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	CBulletManager::GetInstance();
	new CEnemy(Vector3D(-7.0f, 0.0f, 7.7f), Vector3D(0.0f, -2.8f, 0.0f));
	new Player();
	new UI_Reticle();
}

MainGame::~MainGame()
{
	SightManager::GetInstance()->AllClear();
	CBulletManager::GetInstance()->AllClear();
	CBulletManager::GetInstance()->ClearInstance();
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->ClearInstance();
	EffectGeneration::AllClear();
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	CBulletManager::GetInstance()->UpDate();
	EffectGeneration::Update();

	TaskManager::DrawName();

	//�G�ǉ�(��)
	if (Input::KeyP.Clicked())
	{
		new CEnemy(Vector3D(-7.0f, 0.0f, 7.7f), Vector3D(0.0f, -2.8f, 0.0f));
	}
}

void MainGame::Render()
{
	CBulletManager::GetInstance()->Render();
	StageObjectManager::GetInstance()->Render();
	EffectGeneration::Render();
	//std::cout << "MainGame" << '\n';
}