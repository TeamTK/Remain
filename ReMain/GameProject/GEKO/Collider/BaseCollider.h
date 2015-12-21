#ifndef _BASE_COLLIDER_H_
#define _BASE_COLLIDER_H_

#include <functional>
#include "..\System\Math.h"

#define REGIST_FUNC(member_Func) std::bind(&member_Func, this, std::placeholders::_1)

class ColliderManager;
class StaticMesh;
class Vector3D;

enum HitID
{
	eHITID0 = (1 << 0),
	eHITID1 = (1 << 1),
	eHITID2 = (1 << 2),
	eHITID3 = (1 << 3),
	eHITID4 = (1 << 4),
	eHITID5 = (1 << 5),
	eHITID6 = (1 << 6),
	eHITID7 = (1 << 7),
	eHITID8 = (1 << 8),
	eHITID9 = (1 << 9),
	eHITID10 = (1 << 10),
	eHITID11 = (1 << 11),
	eHITID12 = (1 << 12),
	eHITID13 = (1 << 13),
	eHITID14 = (1 << 14),
	eHITID15 = (1 << 15),
	eHITID16 = (1 << 16),
	eHITID17 = (1 << 17),
	eHITID18 = (1 << 18),
	eHITID19 = (1 << 19),
	eHITID20 = (1 << 20),
	eHITID21 = (1 << 21),
	eHITID22 = (1 << 22),
	eHITID23 = (1 << 23),
	eHITID24 = (1 << 24),
	eHITID25 = (1 << 25),
	eHITID26 = (1 << 26),
	eHITID27 = (1 << 27),
	eHITID28 = (1 << 28),
	eHITID29 = (1 << 29),
	eHITID30 = (1 << 30)
};

//�����m�̓�����������
struct Result_Sphere
{
	Vector3D position;
	float radius;
	Result_Sphere() {};
	Result_Sphere(Vector3D &position, float radius) : 
		position(position),
		radius(radius){};
	~Result_Sphere() {};
};

//�J�v�Z�����m��������������
struct Result_Capsule
{
	Vector3D start;
	Vector3D end;
	float radius;
	Result_Capsule() {};
	Result_Capsule(Vector3D &start, Vector3D &end, float radius) :
		start(start),
		end(end),
		radius(radius) {};
	~Result_Capsule() {};
};

//�������m��������������
struct Result_LineSegment
{
	Vector3D start;
	Vector3D end;
	Result_LineSegment() {};
	Result_LineSegment(Vector3D &start, Vector3D &end) :
		start(start),
		end(end) {};
};

//�|���S���ɓ�����������
struct Result_Porygon
{
	Vector3D vertexPos[3];
	Vector3D contactPos;
	Vector3D normal;
	int materialIndex;
	float dist;
	Result_Porygon() {};
	~Result_Porygon() {};
};

//���������|���S���̌��ʁi�����j
struct Result_Porygon_Group
{
	int hitNum;
	Result_Porygon *pArray;
	Result_Porygon_Group() :
		hitNum(0),
		pArray(nullptr) {};
	~Result_Porygon_Group() {};
};

//�R���W�������
class BaseCollider
{
public:
	BaseCollider();
	virtual ~BaseCollider();
	bool GetSleep() const;
	unsigned int GetMyID() const;
	unsigned int GetTargetID() const;
	void SetID(unsigned int myId, unsigned int targetId);
	void Sleep();
	void Awake();

protected:
	unsigned int m_MyId;
	unsigned int m_TargetId;
	bool		 m_IsSeep;
};

#endif