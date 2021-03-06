#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"

class Shotgun : public WeaponBase
{
public:
	Shotgun(int* anim, float* frame, bool* take, EWeapons* weapon, Matrix* m);
	~Shotgun();
	void Update();
	void Reload();		//弾をリロード

private:
};

extern Shotgun* g_pShotgun;

#endif