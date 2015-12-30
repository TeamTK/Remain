#ifndef AMMOBOX_HANDGUN_H
#define AMMOBOX_HANDGUN_H

#include "AmmoBoxBase.h"

class AmmoBox_Handgun : public AmmoBoxBase
{
public:
	AmmoBox_Handgun(Vector3D pos, Vector3D rot, int addnum);
	~AmmoBox_Handgun();
	void Update();

private:

};

#endif 