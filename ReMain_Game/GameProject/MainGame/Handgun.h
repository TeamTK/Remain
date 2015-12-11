#ifndef HANDGUN_H
#define HANDGUN_H


#include "WeaponBase.h"

class Handgun :public WeaponBase
{
public:
	Handgun();
	Handgun(Player* p);
	~Handgun();
	void Update();
private:

};

#endif