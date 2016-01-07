#ifndef _SHOTGUN_BULLET_H_
#define _SHOTGUN_BULLET_H_

#include "Bullet.h"

class ShotGunBullet : public Bullet
{
public:
	ShotGunBullet(const Vector3D &start, const Vector3D &dir, float speed, float time, float radius);
	~ShotGunBullet();

private:

};

#endif