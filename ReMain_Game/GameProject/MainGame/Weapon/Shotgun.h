#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"
#include "../../GameSystem/GUI/Number.h"

class Shotgun : public WeaponBase
{
public:
	Shotgun(int* anim, float* frame, Matrix* m);
	~Shotgun();
	void Update();
	void Reload();		//’e‚ğƒŠƒ[ƒh

private:
	Number m_Num;
};

extern Shotgun* g_pShotgun;

#endif