#ifndef _SPHERE_COLLIDER_H_
#define _SPHERE_COLLIDER_H_

#include "BaseCollider.h"

//球のヒットデータ
struct SphereHitData
{
	Vector3D *pPosition;
	float *pRadius;
};

//球と球の当たり判定
class SphereCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	SphereCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Sphere &)> func);
	~SphereCollider();

private:
	SphereHitData m_HitData;
	std::function<void(Result_Sphere &)> m_Func;
};

//球とカプセルの当たり判定
class Sphere_vs_CapsuleCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	Sphere_vs_CapsuleCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Capsule &)> func);
	~Sphere_vs_CapsuleCollider();

private:
	SphereHitData m_HitData;
	std::function<void(Result_Capsule &)> m_Func;
};

//球とStaticMeshの当たり判定
class Sphere_vs_StaticMeshCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	Sphere_vs_StaticMeshCollider(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group &)> func);
	~Sphere_vs_StaticMeshCollider();

private:
	SphereHitData m_HitData;
	std::function<void(Result_Porygon_Group &)> m_Func;
};

#endif