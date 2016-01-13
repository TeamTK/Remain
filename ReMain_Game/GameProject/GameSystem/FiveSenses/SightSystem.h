#ifndef _SIGHT_SYSTEM_H_
#define _SIGHT_SYSTEM_H_

#include <functional>
#include "..\..\GEKO\Collider\Collider.h"

class SightManager;

struct SightData
{
	Vector3D *pSightPos;
	Vector3D *pSightVec;
	float angle;
	float distance;
};

//ìGÇÃéãäE
class EnemySight
{
public:
	friend SightManager;

public:
	EnemySight();
	~EnemySight();
	bool GetSleep() const;
	void Regist(SightData *pSightdata, std::function<void(const Vector3D *)> func);
	void Release();
	void Sleep();
	void Awake();

private:
	void HitSight(Result_Porygon &data);

private:
	bool m_isSleep;
	bool m_isObstacle;
	std::function<void(const Vector3D *)> m_Func;
	SightData *m_pSightData;
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
	Vector3D *m_pPos;
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