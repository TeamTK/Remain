#include "Chapter.h"
#include "../MainGame/Enemy/EnemyStateManager.h"
#include "../GameSystem/StageObject.h"

Chapter_1_1::Chapter_1_1() :
	Task("Chapter_1_1", 0)
{
	//ìGê∂ê¨
	EnemyStateManager::LoadFileState("TextData\\EnemyState.txt");
	EnemyStateManager::LoadFileSpawn("TextData\\EnemySpawn.txt");
	EnemyStateManager::Generation("Normal_Monster_A", "Chapter1-1");
}

Chapter_1_1::~Chapter_1_1()
{
}

void Chapter_1_1::Update()
{

}