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

	m_pShotgun = new Shotgun(m_pPlayer);
	m_pHandgun = new Handgun(m_pPlayer);
}

MainGame::~MainGame()
{
	delete m_pShotgun;
	delete m_pHandgun;
	CCharacterManager::GetInstance()->AllClear();
	CCharacterManager::GetInstance()->ClearInstance();
	CBulletManager::GetInstance()->AllClear();
	CBulletManager::GetInstance()->ClearInstance();
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->ClearInstance();
}

void MainGame::Update()
{
	CCharacterManager::GetInstance()->Update();
	CBulletManager::GetInstance()->UpDate();
	m_pShotgun->Update();
	m_pHandgun->Update();
	m_pPlayer->SetGunMtx(m_pShotgun->GetMatrix());
}

void MainGame::Render()
{
	CCharacterManager::GetInstance()->Render();
	CBulletManager::GetInstance()->Render();
	m_pShotgun->Render();
	m_pHandgun->Render();
	StageObjectManager::GetInstance()->Render();
	//std::cout << "MainGame" << '\n';
}