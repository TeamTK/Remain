/**
* @file Collision.h
* @brief �����蔻��Ɠ����蔻��Ɏg�����w
* @author �Ð�@�O
* @version 0.4.2.2
* @date 2015/10/1
*/

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Math.h"

//�����
struct SphereInfo
{
	Vector3D pos;
	float radius;
	SphereInfo(){}
	SphereInfo(const Vector3D &pos, float radius)
	{
		this->pos = pos;
		this->radius = radius;
	}
};

//�����
struct LineSegmentInfo
{
	Vector3D start;
	Vector3D end;
	LineSegmentInfo(){}
	LineSegmentInfo(const Vector3D &start, const Vector3D &end) :
		start(start),
		end(end) {}
};

//�J�v�Z�����
struct CapsuleInfo
{
	LineSegmentInfo segment;
	float radius;
	CapsuleInfo() {};
	CapsuleInfo(const LineSegmentInfo &segment, float radius) :
		segment(segment),
		radius(radius) {}
	CapsuleInfo(const Vector3D &start, const Vector3D &end, float radius) :
		segment(LineSegmentInfo(start, end)),
		radius(radius) {}
};

//�O�p�`
struct TriangleInfo
{
	Vector3D v1;
	Vector3D v2;
	Vector3D v3;
	TriangleInfo() {};
	TriangleInfo(const Vector3D &pos1, const Vector3D &pos2, const Vector3D &pos3) : 
		v1(pos1), v2(pos2), v3(pos3){};
};

//���ƎO�p�`�̏Փˌ���
struct HitResult_SphereTriangle
{
	bool isHit;
	float dist;
	Vector3D pos; //�ڐG�_
	Vector3D normal; //�ڐG�����O�p�`�̖@��
	HitResult_SphereTriangle() :
		isHit(false), pos(0.0, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {};
};

//�����ƎO�p�`�̏Փˌ���
struct HitResult_SegmentTriangle
{
	bool isHit;
	Vector3D pos; //�ڐG�_
	Vector3D normal; //�ڐG�����O�p�`�̖@��
	HitResult_SegmentTriangle() :
		isHit(false), pos(0.0, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f){};
};

class CollisionMath;

class Collision3D
{
public:
	Collision3D();
	~Collision3D();
	static bool Sphere(const SphereInfo &sphere1, const SphereInfo &sphere2);
	static bool SpherePushBack(Vector3D *const pVector, const SphereInfo &sphere1, const SphereInfo &sphere2);
	static bool SphereCapsule(const SphereInfo &sphere, const CapsuleInfo &capsule);
	static HitResult_SphereTriangle SphereTriangle(const SphereInfo &sphere, const TriangleInfo &triangle);
	static HitResult_SphereTriangle SphereTriangle(const SphereInfo &sphere, const TriangleInfo &triangle, const Vector3D &normal);
	static bool SphereSegment(SphereInfo &sphere, LineSegmentInfo &segment);
	static bool Capsule(const CapsuleInfo &capsule1, const CapsuleInfo &capsule2);
	static bool LineSegment(const LineSegmentInfo &segment1, const LineSegmentInfo &segment2);
	static HitResult_SegmentTriangle LineSegmentTriangle(const LineSegmentInfo &segment, const TriangleInfo &triangle);
	static HitResult_SegmentTriangle LineSegmentTriangle(const LineSegmentInfo &segment, const TriangleInfo &triangle, const Vector3D &normal);

private:

};

class CollisionMath
{
public:
	CollisionMath();
	~CollisionMath();
	static Vector3D PointOnLineSegmentNearestPoint(const LineSegmentInfo &segment, const Vector3D &point);
	static float DistancePointToLineSegment(const LineSegmentInfo &segment, const Vector3D &point);
	static float DistancePointToLineSegmentSq(const LineSegmentInfo &segment, const Vector3D &point);
	static float DistanceLineSegment(const LineSegmentInfo &segment1, const LineSegmentInfo &segment2);
	static bool TriangleIntersect(const Vector3D &point, const TriangleInfo &triangle, const Vector3D& Normal);

private:

};

#endif