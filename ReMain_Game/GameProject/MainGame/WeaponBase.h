#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "Player.h"

class WeaponBase
{
public:
	WeaponBase();
	~WeaponBase();
	void Update();
	void Render();
	Matrix GetMatrix();

protected:
	Player* m_pPlayer;
	StaticMesh m_Model;
	Matrix m_BoneMat;
	Matrix m_Matrix;
	
};

#endif