#ifndef _CAPSULE_COLLIDER_H_
#define _CAPSULE_COLLIDER_H_

#include "BaseCollider.h"

//カプセルのヒットデータ
struct CapsuleHitData
{
	Vector3D *pStart;
	Vector3D *pEnd;
	float *pRadius;
};

//カプセルとカプセルの当たり判定
class CapsuleCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	CapsuleCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Capsule &)> func);
	~CapsuleCollider();

private:
	CapsuleHitData m_HitData;
	std::function<void(Result_Capsule &)> m_Func;
};

//カプセルと球の当たり判定
class Capsule_vs_SphereCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	Capsule_vs_SphereCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Sphere &)> func);
	~Capsule_vs_SphereCollider();

private:
	CapsuleHitData m_HitData;
	std::function<void(Result_Sphere &)> m_Func;
};

//カプセルと線分の当たり判定
class Capsule_vs_LineSegmentCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	Capsule_vs_LineSegmentCollider(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_LineSegment &)> func);
	~Capsule_vs_LineSegmentCollider();

private:
	CapsuleHitData m_HitData;
	std::function<void(Result_LineSegment &)> m_Func;
};

#endif