#ifndef HANDGUN_H
#define HANDGUN_H

#include "WeaponBase.h"


class Handgun :public WeaponBase
{
public:
	Handgun(int* anim, float* frame, Matrix* m);
	~Handgun();
	void Update();
	void Reload();		//�e�������[�h

private:

};
extern Handgun* g_pHandgun;
#endif