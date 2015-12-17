#ifndef HANDGUN_H
#define HANDGUN_H

#include "WeaponBase.h"

class Handgun :public WeaponBase
{
public:
	Handgun();
	~Handgun();
	void Update();
	void Shot();
	void Hit(Result_Capsule &r);
private:

};

#endif