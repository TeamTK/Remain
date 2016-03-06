#ifndef _WANDERINGS_H_
#define _WANDERINGS_H_

#include "..\..\GEKO\GEKO.h"

struct WanderingsInfo;

class Wanderings
{
public:
	Wanderings();
	~Wanderings();
	bool GetIsStopTime();
	void Init(std::string name, TracerouteSearch *pSearch,
			  Vector3D *pPos, Vector3D *pRot);
	void Update();
	void Start();
	void Stop();
	void DebugDraw();

private:
	WanderingsInfo *m_pWanderingsInfo;
	TracerouteSearch *m_pSearch;
	TracerouteTarget m_Target;
	Vector3D *m_pPos;
	Vector3D *m_pRot;
	int m_TargetNum;
	bool m_IsStart;
	bool m_IsStopTime;
	float m_WaitingCnt;
};

class WanderingsManager
{
public:
	~WanderingsManager();
	static void LoadFile(const char* fileName);
	static WanderingsInfo *GetWanderingsInfo(std::string myName);
	static void DebugDraw(std::string myName);

private:
	WanderingsManager();
	static WanderingsManager *GetInstance();

private:
	class WanderingsPimpl;
	WanderingsPimpl *m_pWanderingsPimpl;
};

#endif