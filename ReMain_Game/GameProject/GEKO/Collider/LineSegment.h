#ifndef _LINE_SEGMENT_H_
#define _LINE_SEGMENT_H_

#include "BaseCollider.h"

struct HitResult_SegmentTriangle;

//線分のヒットデータ
struct LineSegmentHitData
{
	Vector3D *pStart;
	Vector3D *pEnd;
};

//線分と線分の当たり判定
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

//線と球との当たり判定
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

//線とカプセルとの当たり判定
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

//線とStaticMeshとの当たり判定
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

//線とStaticMeshとの当たり判定(複数の結果をもらう)
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