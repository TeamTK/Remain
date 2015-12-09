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

#endif