#include "Chapter.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "../MainGame/Player/Player.h"
#include "../MainGame/Enemy/EnemyWave.h"

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0)
{
	new Player(Vector3D(-45.0f, 0.0f, -11.0f));

	//Œo˜H’Tõƒf[ƒ^“Ç‚İ‚İ‚Æ\’z
	StaticMeshAsset::LoadMesh("media\\Traceroute_1_1.x", "Chapter_1_1_Traceroute");
	static StaticMesh mesh("Chapter_1_1_Traceroute");
	TracerouteManager::RegistTopography(&mesh, "Chapter_1_1_Traceroute", false);

	//“G¶¬
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");
	//EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1", "Chapter_1_1_Traceroute");
}

Chapter_1_1::~Chapter_1_1()
{
	TracerouteManager::ClearTopography("Chapter_1_1_Traceroute");
}

void Chapter_1_1::Update()
{
	if (Input::KeyP.Clicked()) //“G’Ç‰Á(‰¼)
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

	if (Input::KeyO.Clicked()) //“Gíœ
	{
		TaskManager::Kill("Monster_A");
		TaskManager::Kill("Monster_B");
	}
}