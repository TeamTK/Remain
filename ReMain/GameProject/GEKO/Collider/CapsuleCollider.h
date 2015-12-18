#ifndef _CAPSULE_COLLIDER_H_
#define _CAPSULE_COLLIDER_H_

#include "BaseCollider.h"

//�J�v�Z���̃q�b�g�f�[�^
struct CapsuleHitData
{
	Vector3D *pStart;
	Vector3D *pEnd;
	float *pRadius;
};

//�J�v�Z���ƃJ�v�Z���̓����蔻��
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

//�J�v�Z���Ƌ��̓����蔻��
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

//�J�v�Z���Ɛ����̓����蔻��
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