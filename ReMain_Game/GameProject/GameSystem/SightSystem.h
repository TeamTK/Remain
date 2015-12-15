#ifndef _SIGHT_SYSTEM_H_
#define _SIGHT_SYSTEM_H_

#include <functional>
#include "..\GEKO\Collider\Collider.h"

class SightManager;

struct SightData
{
	Vector3D *pSightVec;
	float angle;
	float distance;
};

/*
enum SignID
{
	eSIGHID1 = (1 << 0),
	eSIGHID2 = (1 << 1),
	eSIGHID3 = (1 << 2),
	eSIGHID4 = (1 << 3),
	eSIGHID5 = (1 << 4),
	eSIGHID6 = (1 << 5),
	eSIGHID7 = (1 << 6),
	eSIGHID8 = (1 << 7),
	eSIGHID9 = (1 << 8),
	eSIGHID10 = (1 << 9)
};
*/

//ìGÇÃéãäE
class EnemySight
{
public:
	friend SightManager;

public:
	EnemySight();
	~EnemySight();
	bool GetSleep() const;
	void Regist(SightData *pSightdata, std::function<void()> func);
	void Release();
	void Sleep();
	void Awake();

private:
	void HitSight(Result_Porygon &data);

private:
	bool m_isSleep;
	bool m_isObstacle;
	std::function<void()> m_Func;
	SightData *m_pSightData;
	Collider m_HitSight;

};

//ÉvÉåÉCÉÑÅ[ë§ÇÃéãäEèÓïÒ
class PlayerSightInfo
{
public:
	friend SightManager;

public:
	PlayerSightInfo();
	~PlayerSightInfo();
	void SetPos(Vector3D *pPos);
	void Sleep();
	void Awake();

private:
	bool m_isSleep;
	Vector3D *pPos;

};

class SightManager
{
public:
	~SightManager();
	static SightManager *GetInstance();
	void AddEnemy(EnemySight *pEnemySight);
	void AddPlayer(PlayerSightInfo *pPlayerSightInfo);
	void Clear(EnemySight *pEnemySight);
	void AllClear();
	void Update();

private:
	SightManager();

private:
	class SightPimpl;
	SightPimpl *m_pSightPimpl;
	PlayerSightInfo *m_pPlayerSightInfo;
};

#endif