#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"
#include "../GameSystem/GUI/Number.h"

class Shotgun :public WeaponBase
{
public:
	Shotgun();
	~Shotgun();
	void Update();

private:
	Number m_Num;
};

#endif