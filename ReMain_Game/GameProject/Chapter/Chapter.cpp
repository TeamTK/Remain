#include "Chapter.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyWave.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"
#include "../MainGame/Player/PlayerData.h"
#include "../MainGame/Enemy/Wanderings.h"

void Kill()
{
	TaskManager::Kill("Player");
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
	StageObjectManager::GetInstance()->ClearList();
}

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0),
	m_Radius(4.0f),
	m_pos(-7.0f, 0.0f, 4.0f),
	m_StageChangePos(40.0f, 0.0f, -17.0f)
{
	DirectionalLight::SetDistance(140.0f);

	PData data;
	data.HP = 100.0f;
	data.Shotgun_Ammo = 6;
	data.Shotgun_LoadedAmmo = 6;
	data.Handgun_Ammo = 6;
	data.Handgun_LoadedAmmo = 6;
	data.isTakeWeapon = false;
	data.Weapon = eShotgun;

	new Player(&data, Vector3D(-45.0f, 0.0f, -11.0f), 64, -2);

	//åoòHíTçıÉfÅ[É^ì«Ç›çûÇ›Ç∆ç\íz
	static StaticMesh mesh("Chapter_1_1_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_1_Traceroute", false);

	//ÉAÉbÉZÉbÉgtxtì«Ç›çûÇ›
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_1.txt");

	//ìGúpújÉpÉ^Å[Éìì«Ç›çûÇ›
	WanderingsManager::LoadFile("TextData\\Wandering_Chapter_1_1.txt");

	//ìGê∂ê¨
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//ÉXÉeÅ[ÉWà⁄ìÆóp
	m_StageChange.Regist_S_vs_S(&m_StageChangePos, &m_Radius, REGIST_FUNC(Chapter_1_1::StageChange));
	m_StageChange.SetID(eHITID4, eHITID1);

	//ìGÇèoåª
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Chapter_1_1::HitPlayer));
	m_MapCol.SetID(eHITID5, eHITID1);

	m_MapCol.Sleep();

	m_Render.Regist(6, REGIST_RENDER_FUNC(Chapter_1_1::Render));

	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);

	m_BGM.SetAseet("Field2");
	m_BGM.SetLoop(true);
	m_BGM.Play();

	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1", "Chapter_1_1_Traceroute", true);
}

Chapter_1_1::~Chapter_1_1()
{
	TracerouteManager::ClearTopography("Chapter_1_1_Traceroute");
	m_BGM.Stop();
}

void Chapter_1_1::Update()
{
	m_Transfer_In.Update();

	if (m_Transfer_In.GetIsEndTransfer())
	{
		Kill();
		SetKill();
		new Chapter_1_2();
	}
	/*
	if (Input::KeyP.Clicked()) //ìGí«â¡(âº)
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

	if (Input::KeyO.Clicked()) //ìGçÌèú
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
	*/

	if (Input::KeyO.Clicked()) //ìGçÌèú
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
}

void Chapter_1_1::Render()
{
	m_Transfer_In.Render();
}

void Chapter_1_1::HitPlayer(Result_Sphere &data)
{
	EnemyWaveInfo info;
	info.isSearch = true;
	info.waveAllNum = 1;
	info.intervalTime = 1;
	info.spawnName = "Chapter1-1";
	info.stateName = "Normal_Monster_A";
	info.tracerouteName = "Chapter_1_1_Traceroute";
	new EnemyWave(info);

	m_MapCol.Sleep();
}

void Chapter_1_1::StageChange(Result_Sphere &data)
{
	m_StageChange.Sleep();
	m_Transfer_In.Start(3);
}



//******************************************
//				Chapter_1_2
//******************************************

Chapter_1_2::Chapter_1_2() :
	Task("Chapter_1_2", 0),
	m_Radius(2.0f),
	m_pos(-7.0f, 0.0f, 4.0f)
{
	DirectionalLight::SetDistance(160.0f);

	PData data;
	data.HP = PlayerData::GetData().HP;
	data.Shotgun_Ammo = PlayerData::GetData().Shotgun_Ammo;
	data.Shotgun_LoadedAmmo = PlayerData::GetData().Shotgun_LoadedAmmo;
	data.Handgun_Ammo = PlayerData::GetData().Handgun_Ammo;
	data.Handgun_LoadedAmmo = PlayerData::GetData().Handgun_LoadedAmmo;
	data.isTakeWeapon = PlayerData::GetData().isTakeWeapon;
	data.Weapon = PlayerData::GetData().Weapon;

	new Player(&data, Vector3D(-28.0f, 0.0f, -98.0f), 80, -2);

	//åoòHíTçıÉfÅ[É^ì«Ç›çûÇ›Ç∆ç\íz
	static StaticMesh mesh("Chapter_1_2_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_2_Traceroute", false);

	//ÉAÉbÉZÉbÉgtxtì«Ç›çûÇ›
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_2.txt");

	//ìGê∂ê¨
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");

	//ìGÇèoåª
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
	*/
}

void Chapter_1_2::HitPlayer(Result_Sphere &data)
{
	EnemyWaveInfo info;
	info.isSearch = true;
	info.waveAllNum = 1;
	info.intervalTime = 1;
	info.spawnName = "Chapter1-1";
	info.stateName = "Normal_Monster_A";
	info.tracerouteName = "Chapter_1_2_Traceroute";
	new EnemyWave(info);

	m_MapCol.Sleep();
}