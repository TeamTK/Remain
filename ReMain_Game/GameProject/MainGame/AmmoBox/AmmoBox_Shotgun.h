#ifndef AMMOBOX_SHOTGUN_H
#define AMMOBOX_SHOTGUN_H

#include "AmmoBoxBase.h"

class AmmoBox_Shotgun : public AmmoBoxBase
{
public:
	AmmoBox_Shotgun(Vector3D pos, Vector3D rot, int addnum);
	~AmmoBox_Shotgun();
	void Update();

private:

};

#endif 