#include "MainGame.h"

MainGame::MainGame()
{
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	CCharacterManager::GetInstance()->LoadFile("");
	CBulletManager::GetInstance();
	CCharacterManager::GetInstance()->AddPlayer(new Player());
	m_pPlayer = (Player*)CCharacterManager::GetInstance()->GetPlayer();
	CEnemyManager::Add(eEnemy1, Vector3D(-5, 0, 10));

}

MainGame::~MainGame()
{
	SightManager::GetInstance()->AllClear();
	CCharacterManager::GetInstance()->AllClear();
	CCharacterManager::GetInstance()->ClearInstance();
	CBulletManager::GetInstance()->AllClear();
	CBulletManager::GetInstance()->ClearInstance();
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->ClearInstance();
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	CCharacterManager::GetInstance()->Update();
	CBulletManager::GetInstance()->UpDate();
}

void MainGame::Render()
{
	CCharacterManager::GetInstance()->Render();
	CBulletManager::GetInstance()->Render();
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}