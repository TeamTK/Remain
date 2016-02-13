#include "Chapter.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyWave.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0),
	m_Radius(2.0f),
	m_pos(-7.0f, 0.0f, 4.0f),
	m_isHit(false)
{
	new Player(Vector3D(-45.0f, 0.0f, -11.0f));

	//åoòHíTçıÉfÅ[É^ì«Ç›çûÇ›Ç∆ç\íz
	StaticMeshAsset::LoadMesh("media\\Traceroute_1_1.x", "Chapter_1_1_Traceroute");
	static StaticMesh mesh("Chapter_1_1_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_1_Traceroute", false);

	//ÉAÉbÉZÉbÉgtxtì«Ç›çûÇ›
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_1.txt");

	//ìGê∂ê¨
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//ìGÇèoåª
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Chapter_1_1::HitPlayer));
	m_MapCol.SetID(eHITID5, eHITID1);

	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);
}

Chapter_1_1::~Chapter_1_1()
{
	TracerouteManager::ClearTopography("Chapter_1_2_Traceroute");
}

void Chapter_1_1::Update()
{
	if (Input::KeyP.Clicked()) //ìGí«â¡(âº)
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

	if (Input::KeyO.Clicked()) //ìGçÌèú
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
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
		info.tracerouteName = "Chapter_1_2_Traceroute";
		new EnemyWave(info);

		m_isHit = true;
	}
}




Chapter_1_2::Chapter_1_2() :
	Task("Chapter_1_2", 0),
	m_Radius(2.0f),
	m_pos(-7.0f, 0.0f, 4.0f),
	m_isHit(false)
{
	new Player(Vector3D(0.0f, 1.0f, 0.0f));

	//åoòHíTçıÉfÅ[É^ì«Ç›çûÇ›Ç∆ç\íz
	StaticMeshAsset::LoadMesh("media\\Traceroute_1_2.x", "Chapter_1_2_Traceroute");
	static StaticMesh mesh("Chapter_1_2_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_2_Traceroute", false);

	//ÉAÉbÉZÉbÉgtxtì«Ç›çûÇ›
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_2.txt");

	//ìGê∂ê¨
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//ìGÇèoåª
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Chapter_1_2::HitPlayer));
	m_MapCol.SetID(eHITID5, eHITID1);

	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);
}

Chapter_1_2::~Chapter_1_2()
{
	TracerouteManager::ClearTopography("Chapter_1_2_Traceroute");
}

void Chapter_1_2::Update()
{

	if (Input::KeyP.Clicked()) //ìGí«â¡(âº)
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

	if (Input::KeyO.Clicked()) //ìGçÌèú
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
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