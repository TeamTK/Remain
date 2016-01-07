#ifndef _HANDGUN_BULLET_H_
#define _HANDGUN_BULLET_H_

#include "Bullet.h"

class HandGunBullet : public Bullet
{
public:
	HandGunBullet(const Vector3D &start, const Vector3D &dir, float speed, float time, float radius);
	~HandGunBullet();

private:

};



#endif