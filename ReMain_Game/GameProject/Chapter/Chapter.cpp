#include "Chapter.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../MainGame/Enemy/Wanderings.h"
#include "../MainGame/Enemy/Boss.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Player/PlayerData.h"
#include "../MainGame/Player/RecoveryItem.h"
#include "../MainGame/AmmoBox/AmmoBox_Shotgun.h"
#include "../MainGame/AmmoBox/AmmoBox_Handgun.h"

#define CHAPTER_TRANSFER_IN_SPEED 4.0f
#define CHAPTER_TRANSFER_OUT_SPEED 4.0f

ChapterType g_ChapterType;

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
	TaskManager::Kill("BloodAnim");
	TaskManager::Kill("MuzzleFlash");
	TaskManager::Kill("Map");
	TaskManager::Kill("RecoveryItem");
	TaskManager::Kill("Lamp");
	StageObjectManager::GetInstance()->ClearList();
}

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0),
	m_Radius(4.0f),
	m_StageChangePos(40.0f, 0.0f, -17.0f)
{
	g_ChapterType = ChapterType::eChapter_1_1;

	DirectionalLight::SetDistance(140.0f);

	PData data;
	data.HP = 100.0f;
	data.Shotgun_Ammo = 6;
	data.Shotgun_LoadedAmmo = 6;
	data.Handgun_Ammo = 6;
	data.Handgun_LoadedAmmo = 6;
	data.isTakeWeapon = false;
	data.Weapon = eShotgun;
	data.RecoveryItemNum = 0;
	new Player(&data, Vector3D(-45.0f, 0.0f, -11.0f), 64.0f, 64, -2);

	//経路探索データ読み込みと構築
	m_TracerouteMesh.SetAsset("Chapter_1_1_Traceroute");
	TracerouteManager::RegistTopography(&m_TracerouteMesh, "Chapter_1_1_Traceroute", false);

	//アッセットtxt読み込み
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_1.txt");

	//敵徘徊パターン読み込み
	WanderingsManager::LoadFile("TextData\\Wandering_Chapter_1_1.txt");

	//敵生成
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn_Chapter_1-1.txt");

	//ステージ移動用
	m_StageChange.Regist_S_vs_S(&m_StageChangePos, &m_Radius, REGIST_FUNC(Chapter_1_1::StageChange));
	m_StageChange.SetID(eHITID4, eHITID1);

	//補充用弾
	new AmmoBox_Shotgun(Vector3D(-9.5f, 0.0f, 14.4f), Vector3D(0.0f, 200.0f, 0.0f), 6);
	new AmmoBox_Handgun(Vector3D(19.0f, 0.0f, -13.0f), Vector3D(0.0f, 200.0f, 0.0f), 6);

	//回復アイテム生成
	new RecoveryItem(-15.0f, 0.0f, 0.0f);

	//ステージBGM
	m_BGM.SetAseet("Field2");
	m_BGM.SetLoop(true);
	m_BGM.Play();

	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1", "Chapter_1_1_Traceroute", false);

	m_Transfer_Out.Start(CHAPTER_TRANSFER_OUT_SPEED);
}

Chapter_1_1::~Chapter_1_1()
{
	TracerouteManager::ClearTopography("Chapter_1_1_Traceroute");
	m_BGM.Stop();
}

void Chapter_1_1::Update()
{
	m_Transfer_In.Update();
	m_Transfer_Out.Update();

	if (m_Transfer_In.GetIsEndTransfer())
	{
		Kill();
		SetKill();
		new Chapter_1_2();
	}
}

void Chapter_1_1::StageChange(Result_Sphere &data)
{
	m_StageChange.Sleep();
	m_Transfer_In.Start(CHAPTER_TRANSFER_IN_SPEED);
}



//******************************************
//				Chapter_1_2
//******************************************

Chapter_1_2::Chapter_1_2() :
	Task("Chapter_1_2", 0),
	m_Radius(3.0f),
	m_StageChangePos(64.0f, 0.0f, 39.0f)
{
	g_ChapterType = ChapterType::eChapter_1_2;

	DirectionalLight::SetDistance(160.0f);

	PData data;
	data.HP = PlayerData::GetData().HP;
	data.Shotgun_Ammo = PlayerData::GetData().Shotgun_Ammo;
	data.Shotgun_LoadedAmmo = PlayerData::GetData().Shotgun_LoadedAmmo;
	data.Handgun_Ammo = PlayerData::GetData().Handgun_Ammo;
	data.Handgun_LoadedAmmo = PlayerData::GetData().Handgun_LoadedAmmo;
	data.isTakeWeapon = PlayerData::GetData().isTakeWeapon;
	data.Weapon = PlayerData::GetData().Weapon;
	data.RecoveryItemNum = PlayerData::GetData().RecoveryItemNum;
	new Player(&data, Vector3D(-28.0f, 0.0f, -98.0f), 34.0f, 34.0f, -1.3f);

	//経路探索データ読み込みと構築
	m_TracerouteMesh.SetAsset("Chapter_1_2_Traceroute");
	TracerouteManager::RegistTopography(&m_TracerouteMesh, "Chapter_1_2_Traceroute", false);

	//アッセットtxt読み込み
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_2.txt");

	//ステージ移動用
	m_StageChange.Regist_S_vs_S(&m_StageChangePos, &m_Radius, REGIST_FUNC(Chapter_1_2::StageChange));
	m_StageChange.SetID(eHITID4, eHITID1);

	//敵徘徊パターン読み込み
	WanderingsManager::LoadFile("TextData\\Wandering_Chapter_1_2.txt");

	//敵生成
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn_Chapter_1-2.txt");
	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-2", "Chapter_1_2_Traceroute", false);

	//補充用弾
	new AmmoBox_Shotgun(Vector3D(-18.0f, 0.0f, 2.0f), Vector3D(0.0f, 200.0f, 0.0f), 6);
	new AmmoBox_Handgun(Vector3D(-8.0f, 0.0f, -27.0f), Vector3D(0.0f, 200.0f, 0.0f), 6);
	new AmmoBox_Handgun(Vector3D(32.0f, 0.0f, 9.8f), Vector3D(0.0f, 200.0f, 0.0f), 6);
	new AmmoBox_Handgun(Vector3D(20.0f, 0.0f, 48.0f), Vector3D(0.0f, 200.0f, 0.0f), 6);

	//回復アイテム生成
	new RecoveryItem(10.0f, 0.0f, -14.0f);
	new RecoveryItem(17.0f, 0.0f, 15.0f);

	//ステージBGM
	m_BGM.SetAseet("Field1");
	m_BGM.SetLoop(true);
	m_BGM.Play();

	m_Transfer_Out.Start(CHAPTER_TRANSFER_OUT_SPEED);
}

Chapter_1_2::~Chapter_1_2()
{
	TracerouteManager::ClearTopography("Chapter_1_2_Traceroute");
	m_BGM.Stop();
}

void Chapter_1_2::Update()
{
	m_Transfer_In.Update();
	m_Transfer_Out.Update();

	if (m_Transfer_In.GetIsEndTransfer())
	{
		Kill();
		SetKill();
		new Chapter_1_3();
	}
}

void Chapter_1_2::StageChange(Result_Sphere &data)
{
	m_StageChange.Sleep();
	m_Transfer_In.Start(CHAPTER_TRANSFER_IN_SPEED);
}



//******************************************
//				Chapter_1_3
//******************************************

Chapter_1_3::Chapter_1_3() :
	Task("Chapter_1_3", 0)
{
	g_ChapterType = ChapterType::eChapter_1_3;

	DirectionalLight::SetDistance(100.0f);

	PData data;
	data.HP = PlayerData::GetData().HP;
	data.Shotgun_Ammo = PlayerData::GetData().Shotgun_Ammo;
	data.Shotgun_LoadedAmmo = PlayerData::GetData().Shotgun_LoadedAmmo;
	data.Handgun_Ammo = PlayerData::GetData().Handgun_Ammo;
	data.Handgun_LoadedAmmo = PlayerData::GetData().Handgun_LoadedAmmo;
	data.isTakeWeapon = PlayerData::GetData().isTakeWeapon;
	data.Weapon = PlayerData::GetData().Weapon;
	data.RecoveryItemNum = PlayerData::GetData().RecoveryItemNum;
	new Player(&data, Vector3D(13.0f, 0.0f, 12.0f), -134.0f, -140.0f, -5.0f);

	//アッセットtxt読み込み
	StageObjectManager::GetInstance()->ClearList();
	StageObjectManager::GetInstance()->LoadObject("TextData\\StageObject_Cha1_3.txt");

	new AmmoBox_Shotgun(Vector3D(-6.0f, 0.0f, -2.0f), Vector3D(0.0f, 200.0f, 0.0f), 6);
	new RecoveryItem(-2.0f, 0.0f, 0.0f);

	BossState state;
	state.hp = 60;
	state.flinch = 20;
	state.spawnPos = Vector3D(4.0f, 0.0f, -12.0f);
	state.spawnRot = Vector3D(0.0f, 180.0f, 0.0f);
	new Boss(state);

	//ステージBGM
	m_BGM.SetAseet("Field1");
	m_BGM.SetLoop(true);
	m_BGM.Play();

	m_Transfer_Out.Start(CHAPTER_TRANSFER_OUT_SPEED);
}

Chapter_1_3::~Chapter_1_3()
{
	Kill();
	m_BGM.Stop();
}

void Chapter_1_3::Update()
{
	m_Transfer_In.Update();
	m_Transfer_Out.Update();
}