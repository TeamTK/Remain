#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"

class Shotgun : public WeaponBase
{
public:
	Shotgun(int* anim, float* frame, Matrix* m);
	~Shotgun();
	void Update();
	void Reload();		//�e�������[�h

private:
};

extern Shotgun* g_pShotgun;

#endif