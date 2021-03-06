#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H
#include "../Weapon/WeaponInfo.h"

struct PData
{
	float HP;
	int Shotgun_Ammo;		//ショットガンの手持ち弾数
	int Shotgun_LoadedAmmo;	//ショットガンの銃に入っている弾数
	int Handgun_Ammo;		//ハンドガンの手持ち弾数
	int Handgun_LoadedAmmo;	//ハンドガンの銃に入っている弾数
	int RecoveryItemNum;	//回復薬の数
	bool isTakeWeapon;		//武器を持っているか
	EWeapons Weapon;		//持っている武器
};

class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	static void SetData(PData* d);
	static PData GetData();

private:
	PData m_Data;
	static PlayerData* GetInstance();
};

#endif