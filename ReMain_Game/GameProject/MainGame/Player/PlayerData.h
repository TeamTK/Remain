#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

struct PData
{
	float HP;
	int Shotgun_Ammo;			//�V���b�g�K���̎莝���e��
	int Shotgun_LoadedAmmo;		//�V���b�g�K���̏e�ɓ����Ă���e��
	int Handgun_Ammo;			//�n���h�K���̎莝���e��
	int Handgun_LoadedAmmo;		//�n���h�K���̏e�ɓ����Ă���e��
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