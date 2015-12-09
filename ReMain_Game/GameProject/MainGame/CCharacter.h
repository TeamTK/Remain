#pragma once

#include "../GEKO/GEKO.h"
#include "CBullet.h"
//#include "../GEKO/Collider/ColliderManager.h"
//#include "../GEKO/Collider/CapsuleCollider.h"
class CCharacter
{
protected:
	bool m_isActive;
	float m_Hp;
	Vector3D m_pos;
	Vector3D m_rot;
public:
	CCharacter();
	DynamicMesh m_Model;
//	Capsule_vs_LineSegmentCollider m_CapsuleCollider;
	CapsuleInfo m_Capsule;
	virtual void Update();
	virtual void Render();
//	void Capsule_vs_LineSegmentCallback(Result_LineSegment& r);
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
	static CCharacterManager* GetInstance() {
		if (!m_Obj) m_Obj = new CCharacterManager;
		return m_Obj;
	}
	static void ClearInstance() {
		delete m_Obj;
		m_Obj = NULL;
	}
	void LoadFile(const char*path);
	void Update();
	void Render();
	void Add(CCharacter* c);
	CCharacter*GetPlayer() {
		return m_pPayer;

	}
	void AddPlayer(CCharacter *c);

};