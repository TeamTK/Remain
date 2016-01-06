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

//‹…“¯Žm‚Ì“–‚½‚Á‚½Œ‹‰Ê
struct Result_Sphere
{
	Vector3D position;
	float radius;
	unsigned int targetID;
	const char *name;
	const char *targetName;
	Result_Sphere() :
		name("NoName") {};
	~Result_Sphere() {};
};

//ƒJƒvƒZƒ‹“¯Žm‚ª“–‚½‚Á‚½Œ‹‰Ê
struct Result_Capsule
{
	Vector3D start;
	Vector3D end;
	float radius;
	unsigned int targetID;
	const char *name;
	const char *targetName;
	Result_Capsule() :
		name("NoName") {};
	~Result_Capsule() {};
};

//ü•ª“¯Žm‚ª“–‚½‚Á‚½Œ‹‰Ê
struct Result_LineSegment
{
	Vector3D start;
	Vector3D end;
	unsigned int targetID;
	const char *name;
	const char *targetName;
	Result_LineSegment() :
		name("NoName") {};
};

//ƒ|ƒŠƒSƒ“‚É“–‚½‚Á‚½Œ‹‰Ê
struct Result_Porygon
{
	Vector3D vertexPos[3];
	Vector3D contactPos;
	Vector3D normal;
	int materialIndex;
	unsigned int targetID;
	const char *name;
	const char *targetName;
	Result_Porygon() {};
	~Result_Porygon() {};
};

//ƒ|ƒŠƒSƒ“‚É“–‚½‚Á‚½Œ‹‰Ê(‹…—p)
struct Result_Porygon_Sphere
{
	Vector3D vertexPos[3];
	Vector3D contactPos;
	Vector3D normal;
	int materialIndex;
	unsigned int targetID;
	float dist;
};

//“–‚½‚Á‚½ƒ|ƒŠƒSƒ“‚ÌŒ‹‰Êi•¡”j
struct Result_Porygon_Group
{
	int hitNum;
	float dist;
	const char *name;
	const char *targetName;
	Result_Porygon_Sphere *pArray;
	Result_Porygon_Group() :
		hitNum(0),
		dist(0.0f),
		name("NoName"),
		pArray(nullptr){};
	~Result_Porygon_Group() {};
};

//ƒRƒŠƒWƒ‡ƒ“Šî’ê
class BaseCollider
{
public:
	BaseCollider();
	virtual ~BaseCollider();
	bool GetSleep() const;
	unsigned int GetMyID() const;
	unsigned int GetTargetID() const;
	const char* GetName() const;
	void SetID(unsigned int myId, unsigned int targetId);
	void SetName(const char *name);
	void Sleep();
	void Awake();

protected:
	unsigned int m_MyId;
	unsigned int m_TargetId;
	bool		 m_IsSeep;
	const char* m_Name;
};

#endif