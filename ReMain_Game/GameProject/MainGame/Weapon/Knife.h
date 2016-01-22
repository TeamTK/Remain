#ifndef KNIFE_H
#define KNIFE_H

#include "WeaponBase.h"

class Knife : public WeaponBase
{
public:
	Knife(int* anim, float* frame, bool* take, Matrix* m);
	~Knife();
	void Update();

private:
};

#endif