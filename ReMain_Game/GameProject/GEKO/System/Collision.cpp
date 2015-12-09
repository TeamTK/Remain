#include "Collision.h"
#include <stdlib.h>
#include <math.h>

class CollisionMath;

Collision3D::Collision3D()
{
}

Collision3D::~Collision3D()
{
}

bool Collision3D::Sphere(const SphereInfo &sphere1, const SphereInfo &sphere2)
{
	Vector3D l = sphere2.pos - sphere1.pos;
	float radius = sphere1.radius * sphere1.radius + sphere2.radius * sphere2.radius;

	if (l.LengthSq() <= radius) return true;
	return false;
}

bool Collision3D::SpherePushBack(Vector3D *const pVector, const SphereInfo &sphere1, const SphereInfo &sphere2)
{
	Vector3D vec = sphere2.pos - sphere1.pos;
	float radius = sphere1.radius + sphere2.radius;

	float l = vec.Length();
	if (l <= radius)
	{
		float distance = radius - l;
		if (l > 0.0f) l = 1 / l;

		vec *= l;
		distance /= 2.0f;

		*pVector = vec * distance;

		return true;
	}
	return false;
}

bool Collision3D::SphereCapsule(const SphereInfo &sphere, const CapsuleInfo &capsule)
{
	float l = CollisionMath::DistancePointToLineSegment(capsule.segment, sphere.pos);
	if (l <= sphere.radius + capsule.radius) return true;
	return false;
}

HitResult_SphereTriangle Collision3D::SphereTriangle(const SphereInfo &sphere, const TriangleInfo &triangle)
{
	HitResult_SphereTriangle hitResult;

	//�O�p�`�̖@�������߂�
	Vector3D e1 = triangle.v2 - triangle.v1;
	Vector3D e2 = triangle.v3 - triangle.v1;
	Vector3D normal(Vector3D::Cross(e2, e1).GetNormalize()); //�O�p�`�̖@��

	Vector3D VP = sphere.pos - triangle.v1;	//�O�p�`�̒��_���狅�̒��S�܂ł̃x�N�g��

	float dist = abs(Vector3D::Dot(VP, normal)); //�_����O�p�`�܂ł̋���

	//���̔��a�O�Ȃ瓖�����Ă��Ȃ�
	if (dist > sphere.radius) return hitResult;

	//�_����O�p�`�ɐ����ɉ��낵���n�_�����߂�i�O�p�`��̓_�̈ʒu���Z�o�j
	Vector3D point = sphere.pos - (normal * dist);

	if (CollisionMath::TriangleIntersect(point, triangle, normal))
	{
		hitResult.isHit = true;
		hitResult.normal = normal;
		hitResult.pos = point;
		hitResult.dist = dist;
		return hitResult;
	}

	float l;		//�ŒZ����
	float lengthSq; //������r�p
	Vector3D pos;

	//��1
	LineSegmentInfo segment(triangle.v1, triangle.v2);
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	l = lengthSq = (sphere.pos - point).LengthSq();
	pos = point;

	//��2
	segment.start = triangle.v2; segment.end = triangle.v3;
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	lengthSq = (sphere.pos - point).LengthSq();
	if (l > lengthSq)
	{
		l = lengthSq;
		pos = point;
	}

	//��3
	segment.start = triangle.v3; segment.end = triangle.v1;
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	lengthSq = (sphere.pos - point).LengthSq();
	if (l > lengthSq)
	{
		l = lengthSq;
		pos = point;
	}

	//l = sqrt(l);
	if (l <= sphere.radius * sphere.radius)
	{
		hitResult.isHit = true;
		hitResult.normal = normal;
		hitResult.pos = pos;
		hitResult.dist = sqrt(l);
	}

	return hitResult;
}

HitResult_SphereTriangle Collision3D::SphereTriangle(const SphereInfo &sphere, const TriangleInfo &triangle, const Vector3D &normal)
{
	HitResult_SphereTriangle hitResult;

	Vector3D VP = sphere.pos - triangle.v1;	//�O�p�`�̒��_���狅�̒��S�܂ł̃x�N�g��

	float dist = abs(Vector3D::Dot(VP, normal)); //�_����O�p�`�܂ł̋���
									
	if (dist > sphere.radius) return hitResult;//���̔��a�O�Ȃ瓖�����Ă��Ȃ�

	//�_����O�p�`�ɐ����ɉ��낵���n�_�����߂�i�O�p�`��̓_�̈ʒu���Z�o�j
	Vector3D point = sphere.pos - (normal * dist);

	if (CollisionMath::TriangleIntersect(point, triangle, normal))
	{
		hitResult.isHit = true;
		hitResult.normal = normal;
		hitResult.pos = point;
		hitResult.dist = dist;
		return hitResult;
	}

	float l;		//�ŒZ����
	float lengthSq; //������r�p
	Vector3D pos;

	//��1
	LineSegmentInfo segment(triangle.v1, triangle.v2);
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	l = lengthSq = (sphere.pos - point).LengthSq();
	pos = point;

	//��2
	segment.start = triangle.v2; segment.end = triangle.v3;
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	lengthSq = (sphere.pos - point).LengthSq();
	if (l > lengthSq)
	{
		l = lengthSq;
		pos = point;
	}

	//��3
	segment.start = triangle.v3; segment.end = triangle.v1;
	point = CollisionMath::PointOnLineSegmentNearestPoint(segment, sphere.pos);
	lengthSq = (sphere.pos - point).LengthSq();
	if (l > lengthSq)
	{
		l = lengthSq;
		pos = point;
	}

	//l = sqrt(l);
	if (l <= sphere.radius * sphere.radius)
	{
		hitResult.isHit = true;
		hitResult.normal = normal;
		hitResult.pos = pos;
		hitResult.dist = sqrt(l);
	}

	return hitResult;
}

bool Collision3D::SphereSegment(SphereInfo &sphere, LineSegmentInfo &segment)
{
	float l = CollisionMath::DistancePointToLineSegment(segment, sphere.pos);
	if (l <= sphere.radius) return true;
	return false;
}

bool Collision3D::Capsule(const CapsuleInfo &capsule1, const CapsuleInfo &capsule2)
{
	float l = CollisionMath::DistanceLineSegment(capsule1.segment, capsule2.segment);
	if (l <= capsule1.radius + capsule2.radius) return true;
	return false;
}

bool Collision3D::LineSegment(const LineSegmentInfo &segment1, const LineSegmentInfo &segment2)
{
	Vector3D v1 = segment1.end - segment1.start;
	Vector3D v2 = segment2.end - segment2.start;
	
	Vector3D vp1 = segment2.start - segment1.start;
	Vector3D vp2 = segment2.end - segment1.start;
	Vector3D vp3 = segment1.start - segment2.start;
	Vector3D vp4 = segment1.end - segment2.start;

	float crossToVP1 = Vector3D::Dot(Vector3D::Cross(v1, vp1), Vector3D::Cross(v1, vp2));
	float crossToVP2 = Vector3D::Dot(Vector3D::Cross(v2, vp3), Vector3D::Cross(v2, vp4));

	if (crossToVP1 < 0.0f &&
		crossToVP2 < 0.0f) return true;
	return false;
}

HitResult_SegmentTriangle Collision3D::LineSegmentTriangle(const LineSegmentInfo &segment, const TriangleInfo &triangle)
{
	HitResult_SegmentTriangle hitResult;

	Vector3D e1 = triangle.v2 - triangle.v1;
	Vector3D e2 = triangle.v3 - triangle.v1;
	Vector3D normal = Vector3D::Cross(e2, e1).GetNormalize();

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = Vector3D::Dot((segment.start - triangle.v1), normal);
	float d2 = Vector3D::Dot((segment.end - triangle.v1), normal);

	//d1��d2�������������Ȃ�������Ă��Ȃ�
	if (d1 * d2 > 0.0f) return hitResult;
	
	//�n�_����|���S���܂ł̋����Ɛ����̒����̔䗦�����߂�
	float t = d1 / (d1 - d2);

	if (t < 0) return hitResult;

	//�����ƎO�p�`�̐ڐG�_�����߂�
	Vector3D point = segment.start + (segment.end - segment.start) * t;

	//�ڐG�_���O�p�`��ɂ��邩
	if (!CollisionMath::TriangleIntersect(point, triangle, normal)) return hitResult;

	hitResult.isHit = true;
	hitResult.pos = point;
	hitResult.normal = normal;
	return hitResult;
}

HitResult_SegmentTriangle Collision3D::LineSegmentTriangle(const LineSegmentInfo &segment, const TriangleInfo &triangle, const Vector3D &normal)
{
	HitResult_SegmentTriangle hitResult;

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = Vector3D::Dot(segment.start - triangle.v1, normal);
	float d2 = Vector3D::Dot(segment.end - triangle.v1, normal);

	//d1��d2�������������Ȃ�������Ă��Ȃ�
	if (d1 * d2 > 0) return hitResult;

	//�n�_����|���S���܂ł̋����Ɛ����̒����̔䗦�����߂�
	float t = d1 / (d1 - d2);

	if (t < 0) return hitResult;

	//�����ƎO�p�`�̐ڐG�_�����߂�
	Vector3D point = segment.start + (segment.end - segment.start) * t;

	//�ڐG�_���O�p�`��ɂ��邩
	if (!CollisionMath::TriangleIntersect(point, triangle, normal)) return hitResult;

	hitResult.isHit = true;
	hitResult.pos = point;
	hitResult.normal = normal;
	return hitResult;
}

CollisionMath::CollisionMath()
{
}

CollisionMath::~CollisionMath()
{
}

Vector3D CollisionMath::PointOnLineSegmentNearestPoint(const LineSegmentInfo &segment, const Vector3D &point)
{
	Vector3D v = segment.end - segment.start;
	float l = v.LengthSq();

	//�����̎n�_�ƏI�_�������ꏊ
	if ((int)l == 0) return segment.start;

	//�����ɓ��e
	Vector3D vp = point - segment.start;
	float b = Vector3D::Dot(v, vp);
	float t = b / l;
	
	if (t < 0.0f) return segment.start; //�n�_��艺�Ȃ���n�_	
	if (t > 1.0f) return segment.end;   //�I�_����Ȃ���I�_	

	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return segment.start + v * t;
}

float CollisionMath::DistancePointToLineSegment(const LineSegmentInfo &segment, const Vector3D &point)
{
	Vector3D p = PointOnLineSegmentNearestPoint(segment, point);
	return (point - p).Length();
}

float CollisionMath::DistancePointToLineSegmentSq(const LineSegmentInfo &segment, const Vector3D &point)
{
	Vector3D p = PointOnLineSegmentNearestPoint(segment, point);
	return (point - p).LengthSq();
}

float CollisionMath::DistanceLineSegment(const LineSegmentInfo &segment1, const LineSegmentInfo &segment2)
{
	Vector3D v1 = segment1.end - segment1.start;
	Vector3D v2 = segment2.end - segment2.start;

	Vector3D n = Vector3D::Cross(v1, v2).GetNormalize();

	if (n.LengthSq() != 0)
	{
		float l = Vector3D::Dot(n, (segment1.start - segment2.start));
		Vector3D m = n * l;

		Vector3D start2 = segment2.start + m;
		Vector3D end2 = segment2.end + m;

		float d1 = Vector3D::Dot(Vector3D::Cross(v1, segment1.start - start2), Vector3D::Cross(v1, segment1.start - end2));
		float d2 = Vector3D::Dot(Vector3D::Cross(v2, start2 - segment1.start), Vector3D::Cross(v2, start2 - segment1.end));

		if (d1 < 0 && d2 < 0)
		{
			return fabs(l);
		}
	}

	float len = sqrt(min(min(DistancePointToLineSegmentSq(segment2, segment1.start),
						DistancePointToLineSegmentSq(segment2, segment1.end)),
					 min(DistancePointToLineSegmentSq(segment1, segment2.start),
						DistancePointToLineSegmentSq(segment1, segment2.end))));
	return len;
}

bool CollisionMath::TriangleIntersect(const Vector3D &point, const TriangleInfo &triangle, const Vector3D& Normal)
{
	//�����Z�o
	Vector3D cross1 = Vector3D::Cross(point - triangle.v1, triangle.v2 - triangle.v1);
	Vector3D cross2 = Vector3D::Cross(point - triangle.v2, triangle.v3 - triangle.v2);
	Vector3D cross3 = Vector3D::Cross(point - triangle.v3, triangle.v1 - triangle.v3);

	//�S�Ă̊O�ς̌��������������Ȃ�_�͎O�p�`��ɗL��
	if (Vector3D::Dot(cross1, Normal) > 0.0f) return false;
	if (Vector3D::Dot(cross2, Normal) > 0.0f) return false;
	if (Vector3D::Dot(cross3, Normal) > 0.0f) return false;

	return true;
}