#include "Chapter.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyWave.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"

void Kill()
{
	TaskManager::Kill("TPSCamera");
	TaskManager::Kill("UI_Reticle");
	TaskManager::Kill("UI_AmmoNum");
	TaskManager::Kill("AmmoBox_Shotgun");
	TaskManager::Kill("AmmoBox_Handgun");
	TaskManager::Kill("Monster_A");
	TaskManager::Kill("Monster_B");
	TaskManager::Kill("Handgun");
	TaskManager::Kill("Shotgun");
	TaskManager::Kill("ScreenBlood");
}

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0),
	m_Radius(4.0f),
	m_pos(-7.0f, 0.0f, 4.0f),
	m_StageChangePos(40.0f, 0.0f, -17.0f),
	m_isHit(false)
{
	GEKO::SetShadowPosition(20.0f, 200.0f, 0.0f);

	new Player(Vector3D(-45.0f, 0.0f, -11.0f), 64, -2);

	//経路探索データ読み込みと構築
	static StaticMesh mesh("Chapter_1_1_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_1_Traceroute", false);

	//アッセットtxt読み込み
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_1.txt");

	//敵生成
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//ステージ移動用
	m_StageChange.Regist_S_vs_S(&m_StageChangePos, &m_Radius, REGIST_FUNC(Chapter_1_1::StageChange));
	m_StageChange.SetID(eHITID4, eHITID1);

	//敵を出現
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Chapter_1_1::HitPlayer));
	m_MapCol.SetID(eHITID5, eHITID1);

	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);

	m_BGM.SetAseet("Field1");
	m_BGM.SetLoop(true);
	m_BGM.Play();
}

Chapter_1_1::~Chapter_1_1()
{
	TracerouteManager::ClearTopography("Chapter_1_1_Traceroute");
	m_BGM.Stop();
}

void Chapter_1_1::Update()
{
	/*
	if (Input::KeyP.Clicked()) //敵追加(仮)
	{
		EnemyWaveInfo info;
		info.isSearch = true;
		info.waveAllNum = 1;
		info.intervalTime = 1;
		info.spawnName = "Chapter1-1";
		info.stateName = "Normal_Monster_A";
		info.tracerouteName = "Chapter_1_1_Traceroute";
		new EnemyWave(info);
	}

	if (Input::KeyO.Clicked()) //敵削除
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
	*/
}

void Chapter_1_1::HitPlayer(Result_Sphere &data)
{
	if (!m_isHit)
	{
		EnemyWaveInfo info;
		info.isSearch = true;
		info.waveAllNum = 1;
		info.intervalTime = 1;
		info.spawnName = "Chapter1-1";
		info.stateName = "Normal_Monster_A";
		info.tracerouteName = "Chapter_1_1_Traceroute";
		new EnemyWave(info);

		m_isHit = true;
	}
}

void Chapter_1_1::StageChange(Result_Sphere &data)
{
	Kill();
	TaskManager::Kill("Chapter_1_1");
	new Chapter_1_2();
}



//******************************************
//				Chapter_1_2
//******************************************

Chapter_1_2::Chapter_1_2() :
	Task("Chapter_1_2", 0),
	m_Radius(2.0f),
	m_pos(-7.0f, 0.0f, 4.0f),
	m_isHit(false)
{
	GEKO::SetShadowPosition(-7.0f, 340.0f, -209.0f);

	new Player(Vector3D(-28.0f, 0.0f, -98.0f), 80, -2);

	//経路探索データ読み込みと構築
	static StaticMesh mesh("Chapter_1_2_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_2_Traceroute", false);

	//アッセットtxt読み込み
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_2.txt");

	//敵生成
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//敵を出現
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Chapter_1_2::HitPlayer));
	m_MapCol.SetID(eHITID5, eHITID1);

	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);

	m_BGM.SetAseet("Field1");
	m_BGM.SetLoop(true);
	m_BGM.Play();
}

Chapter_1_2::~Chapter_1_2()
{
	TracerouteManager::ClearTopography("Chapter_1_2_Traceroute");
	Kill();
	m_BGM.Stop();
}

void Chapter_1_2::Update()
{
	/*
	if (Input::KeyP.Clicked()) //敵追加(仮)
	{
		EnemyWaveInfo info;
		info.isSearch = true;
		info.waveAllNum = 1;
		info.intervalTime = 1;
		info.spawnName = "Chapter1-1";
		info.stateName = "Normal_Monster_A";
		info.tracerouteName = "Chapter_1_2_Traceroute";
		new EnemyWave(info);
	}

	if (Input::KeyO.Clicked()) //敵削除
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
	*/
}

void Chapter_1_2::HitPlayer(Result_Sphere &data)
{
	if (!m_isHit)
	{
		EnemyWaveInfo info;
		info.isSearch = true;
		info.waveAllNum = 1;
		info.intervalTime = 1;
		info.spawnName = "Chapter1-1";
		info.stateName = "Normal_Monster_A";
		info.tracerouteName = "Chapter_1_2_Traceroute";
		new EnemyWave(info);

		m_isHit = true;
	}
}