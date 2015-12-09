#ifndef _SPHERE_COLLIDER_H_
#define _SPHERE_COLLIDER_H_

#include "BaseCollider.h"

//���̃q�b�g�f�[�^
struct SphereHitData
{
	Vector3D *pPosition;
	float *pRadius;
};

//���Ƌ��̓����蔻��
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

//���ƃJ�v�Z���̓����蔻��
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

//����StaticMesh�̓����蔻��
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