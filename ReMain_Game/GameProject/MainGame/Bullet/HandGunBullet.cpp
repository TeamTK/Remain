#include "HandGunBullet.h"

HandGunBullet::HandGunBullet(const Vector3D &start, const Vector3D &dir, float speed, float time, float radius) :
	Bullet(start, dir, speed, time, radius)
{
}

HandGunBullet::~HandGunBullet()
{
}