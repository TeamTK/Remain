#pragma once
#include "CCharacter.h"

class CEnemy : public CCharacter {
public:
	CEnemy();
	CEnemy(Vector3D pos);
	void Update();
};

class CEnemyManager {
public:
	static CEnemy *Add(Vector3D pos);
};