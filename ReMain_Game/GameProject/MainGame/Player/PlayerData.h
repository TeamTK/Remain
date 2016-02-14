#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

struct PData
{
	float HP;
	int Shotgun_Ammo;			//ƒVƒ‡ƒbƒgƒKƒ“‚Ìè‚¿’e”
	int Shotgun_LoadedAmmo;		//ƒVƒ‡ƒbƒgƒKƒ“‚Ìe‚É“ü‚Á‚Ä‚¢‚é’e”
	int Handgun_Ammo;			//ƒnƒ“ƒhƒKƒ“‚Ìè‚¿’e”
	int Handgun_LoadedAmmo;		//ƒnƒ“ƒhƒKƒ“‚Ìe‚É“ü‚Á‚Ä‚¢‚é’e”
};

class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	static void SetData(PData d);
	static PData GetData();

private:
	PData m_Data;
	static PlayerData* GetInstance();
};

#endif