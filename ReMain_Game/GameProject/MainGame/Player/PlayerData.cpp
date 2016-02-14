#include "PlayerData.h"

PlayerData::PlayerData()
{

}

PlayerData::~PlayerData()
{

}

void PlayerData::SetData(PData d)
{
	GetInstance()->m_Data = d;
}

PData PlayerData::GetData()
{
	return GetInstance()->m_Data;
}

PlayerData* PlayerData::GetInstance()
{

	static PlayerData playerData;
	return &playerData;
}