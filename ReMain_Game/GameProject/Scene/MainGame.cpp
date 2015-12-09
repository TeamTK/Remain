#include "MainGame.h"

MainGame::MainGame()
{
	StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject.txt");

	CCharacterManager::GetInstance()->LoadFile("");
	CBulletManager::GetInstance();
	CCharacterManager::GetInstance()->AddPlayer(new Player());
	m_pPlayer = (Player*)CCharacterManager::GetInstance()->GetPlayer();
	CEnemyManager::Add(Vector3D(-5, 0, 10));

//	m_pPlayer = new Player();
	m_pShotgun = new Shotgun(m_pPlayer);
	m_pHandgun = new Handgun(m_pPlayer);
}

MainGame::~MainGame()
{
//	delete m_pPlayer;
	delete m_pShotgun;
	delete m_pHandgun;
	CCharacterManager::GetInstance()->ClearInstance();
	CBulletManager::GetInstance()->ClearInstance();
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->ClearInstance();
}

void MainGame::Update()
{
	CCharacterManager::GetInstance()->Update();
	CBulletManager::GetInstance()->UpDate();
//	m_pPlayer->Update();
	m_pShotgun->Update();
	m_pHandgun->Update();
}

void MainGame::Render()
{
	CCharacterManager::GetInstance()->Render();
	CBulletManager::GetInstance()->Render();
//	m_pPlayer->Render();
	m_pShotgun->Render();
	m_pHandgun->Render();
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}