#ifndef HANDGUN_H
#define HANDGUN_H

#include "WeaponBase.h"

class Handgun :public WeaponBase
{
public:
	Handgun();
	~Handgun();
	void Update();
	void Shot(Vector3D start, Vector3D end);
private:

};

#endif