#ifndef _LINE_SEGMENT_H_
#define _LINE_SEGMENT_H_

#include "BaseCollider.h"

struct HitResult_SegmentTriangle;

//�����̃q�b�g�f�[�^
struct LineSegmentHitData
{
	Vector3D *pStart;
	Vector3D *pEnd;
};

//�����Ɛ����̓����蔻��
class LineSegmentCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	LineSegmentCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_LineSegment &)> func);
	~LineSegmentCollider();

private:
	LineSegmentHitData m_HitData;
	std::function<void(Result_LineSegment &)> m_Func;
};

//���Ƌ��Ƃ̓����蔻��
class LineSegment_vs_SphereCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	LineSegment_vs_SphereCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Sphere &)> func);
	~LineSegment_vs_SphereCollider();

private:
	LineSegmentHitData m_HitData;
	std::function<void(Result_Sphere &)> m_Func;
};

//���ƃJ�v�Z���Ƃ̓����蔻��
class LineSegment_vs_CapsuleCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	LineSegment_vs_CapsuleCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Capsule &)> func);
	~LineSegment_vs_CapsuleCollider();

private:
	LineSegmentHitData m_HitData;
	std::function<void(Result_Capsule &)> m_Func;
};

//����StaticMesh�Ƃ̓����蔻��
class LineSegment_vs_StaticMeshCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	LineSegment_vs_StaticMeshCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon &)> func);
	~LineSegment_vs_StaticMeshCollider();

private:
	LineSegmentHitData m_HitData;
	std::function<void(Result_Porygon &)> m_Func;
};

//����StaticMesh�Ƃ̓����蔻��(�����̌��ʂ����炤)
class LineSegment_Group_vs_StaticMeshCollider : public BaseCollider
{
public:
	friend ColliderManager;

public:
	LineSegment_Group_vs_StaticMeshCollider(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon_Group_LineSegment &)> func);
	~LineSegment_Group_vs_StaticMeshCollider();

private:
	LineSegmentHitData m_HitData;
	std::function<void(Result_Porygon_Group_LineSegment &)> m_Func;
};

#endif