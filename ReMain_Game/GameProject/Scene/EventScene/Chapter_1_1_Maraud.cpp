#include "Chapter_1_1_Maraud.h"

Chapter_1_1_Maraud::Chapter_1_1_Maraud() :
	EventSceneBase("Chapter_1_1_Maraud", 0)
{
	TaskManager::Stop("Player");
	TaskManager::Stop("TPSCamera");
	TaskManager::Kill("Monster_A");
	TaskManager::Kill("Monster_B");

	m_Timer.Start();
	//Camera::SetEye();
}

Chapter_1_1_Maraud::~Chapter_1_1_Maraud()
{
}

void Chapter_1_1_Maraud::Update()
{
	if (m_Timer.GetSecond() > 2)
	{

	}
}