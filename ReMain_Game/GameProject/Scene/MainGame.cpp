#include "MainGame.h"
#include "..\GameSystem\Effect.h"

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
	CEnemyManager::Add(eEnemy1, Vector3D(-7.0f, 0.0f, 7.7f), Vector3D(0.0f, -2.8f, 0.0f));
	//CEnemyManager::Add(eEnemy1, Vector3D(22.7f, 0.0, -6.3f), Vector3D(0.0f, -3.7f, 0.0f));
	//CEnemyManager::Add(eEnemy1, Vector3D(36.9f, 0.0f, -11.8f), Vector3D(0.0f, 4.3f, 0.0f));

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
	EffectGeneration::AllClear();
}

void MainGame::Update()
{
	SightManager::GetInstance()->Update();
	CCharacterManager::GetInstance()->Update();
	CBulletManager::GetInstance()->UpDate();
	EffectGeneration::Update();


	//敵追加(仮)
	if (Input::KeyP.Clicked())
	{
		CEnemyManager::Add(eEnemy1, Vector3D(-7.0f, 0.0f, 7.7f), Vector3D(0.0f, -2.8f, 0.0f));
	}
}

void MainGame::Render()
{
	CCharacterManager::GetInstance()->Render();
	CBulletManager::GetInstance()->Render();
	StageObjectManager::GetInstance()->Render();
	EffectGeneration::Render();
	//std::cout << "MainGame" << '\n';
}