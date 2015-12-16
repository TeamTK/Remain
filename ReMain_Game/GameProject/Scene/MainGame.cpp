#include "MainGame.h"

MainGame::MainGame()
{
	//アッセットtxt読み込み
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
	ImageAsset::LoadFile("TextData\\Image.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

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