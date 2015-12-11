#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"

class Shotgun :public WeaponBase
{
public:
	Shotgun();
	Shotgun(Player* p);
	~Shotgun();
	void Update();
private:

};

#endif