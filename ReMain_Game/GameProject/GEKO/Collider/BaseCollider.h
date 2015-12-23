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

//球同士の当たった結果
struct Result_Sphere
{
	Vector3D position;
	float radius;
	unsigned int targetID;
	Result_Sphere() {};
	Result_Sphere(Vector3D &position, float radius, unsigned int targetID) :
		position(position),
		radius(radius),
		targetID(targetID) {};
	~Result_Sphere() {};
};

//カプセル同士が当たった結果
struct Result_Capsule
{
	Vector3D start;
	Vector3D end;
	float radius;
	unsigned int targetID;
	Result_Capsule() {};
	Result_Capsule(Vector3D &start, Vector3D &end, float radius, unsigned int targetID) :
		start(start),
		end(end),
		radius(radius),
		targetID(targetID) {};
	~Result_Capsule() {};
};

//線分同士が当たった結果
struct Result_LineSegment
{
	Vector3D start;
	Vector3D end;
	unsigned int targetID;
	Result_LineSegment() {};
	Result_LineSegment(Vector3D &start, Vector3D &end, unsigned int targetID) :
		start(start),
		end(end),
		targetID(targetID) {};
};

//ポリゴンに当たった結果
struct Result_Porygon
{
	Vector3D vertexPos[3];
	Vector3D contactPos;
	Vector3D normal;
	int materialIndex;
	unsigned int targetID;
	Result_Porygon() {};
	~Result_Porygon() {};
};

//ポリゴンに当たった結果(球用)
struct Result_Porygon_Sphere
{
	Vector3D vertexPos[3];
	Vector3D contactPos;
	Vector3D normal;
	int materialIndex;
	unsigned int targetID;
	float dist;
};

//当たったポリゴンの結果（複数）
struct Result_Porygon_Group
{
	int hitNum;
	float dist;
	Result_Porygon_Sphere *pArray;
	Result_Porygon_Group() :
		hitNum(0),
		pArray(nullptr) {};
	~Result_Porygon_Group() {};
};

//コリジョン基底
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