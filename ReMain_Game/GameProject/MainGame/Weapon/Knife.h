#ifndef KNIFE_H
#define KNIFE_H

#include "WeaponBase.h"

class Knife : public WeaponBase
{
public:
	Knife(int* anim, float* frame, Matrix* m);
	~Knife();
	void Update();

private:
};

extern Knife* g_pKnife;

#endif