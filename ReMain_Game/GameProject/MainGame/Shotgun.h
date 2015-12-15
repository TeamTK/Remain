#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"

class Shotgun :public WeaponBase
{
public:
	Shotgun();
	~Shotgun();
	void Update();
	void Shot();
	void Hit(Result_Capsule &r);

private:

};

#endif