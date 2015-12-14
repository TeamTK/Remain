#pragma once

#include "../GEKO/GEKO.h"
#include "CBullet.h"
#include "../GEKO/Collider/Collider.h"
enum {
	ePlayer,
	eEnemy1,
	eCharacterMax
};
class CCharacter
{
	struct SCharacterData
	{
		char aseetName[64];
		int	hp;
		float collitionMapRad;
		struct SBoneCalpule
		{
			float radius;
			int start;
			int end;
			SBoneCalpule(float r, int s, int e)
			{
				radius = r;
				start = s;
				end = e;
			}
		};
		std::vector<SBoneCalpule> BoneCapsule;

	};
protected:
	bool m_isActive;
	float m_Hp;
	Vector3D m_pos;
	Vector3D m_rot;
	Collider m_ColliderMap;
	SphereInfo m_SphereMap;

	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	SCharacterData *m_pCharaData;
public:
	CCharacter() {}
	CCharacter(int type);
	virtual ~CCharacter();
	DynamicMesh m_Model;
	virtual void Update();
	virtual void Render();
	virtual void HitBullet();
	//	void Capsule_vs_LineSegmentCallback(Result_LineSegment& r);_
	void Capsule_vs_LineSegmentCallback(Result_Sphere& r);
	void Sphere_vs_MeshCallback(Result_Porygon_Group& r);
	friend class CCharacterManager;
	friend class CCharacter;
};


class CCharacterManager
{
	static CCharacterManager *m_Obj;
	std::vector<CCharacter*> m_Core;
	CCharacter *m_pPayer;
public:
	CCharacterManager();
	static CCharacterManager* GetInstance()
	{
		if (!m_Obj) m_Obj = new CCharacterManager;
		return m_Obj;
	}
	static void ClearInstance()
	{
		delete m_Obj;
		m_Obj = NULL;
	}
	void LoadFile(const char*path);
	void Update();
	void Render();
	void Add(CCharacter* c);
	void AllClear();
	CCharacter*GetPlayer()
	{
		return m_pPayer;

	}
	void AddPlayer(CCharacter *c);

};