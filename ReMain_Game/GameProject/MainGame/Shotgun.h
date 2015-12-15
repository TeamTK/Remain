#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "WeaponBase.h"

class Shotgun :public WeaponBase
{
public:
	Shotgun();
	~Shotgun();
	void Update();
	void Shot(Vector3D start, Vector3D end);

private:

};

#endif