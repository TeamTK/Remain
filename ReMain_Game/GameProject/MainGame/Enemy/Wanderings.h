#ifndef _WANDERINGS_H_
#define _WANDERINGS_H_

#include "..\..\GEKO\GEKO.h"

struct WanderingsInfo;

class Wanderings
{
public:
	Wanderings();
	~Wanderings();
	void Init(std::string name, TracerouteSearch *pSearch,
			  Vector3D *pPos, Vector3D *pRot);
	void Update();
	void Start();
	void Stop();

private:
	WanderingsInfo *m_pWanderingsInfo;
	TracerouteSearch *m_pSearch;
	Vector3D *m_pPos;
	Vector3D *m_pRot;
	Timer m_StopTimer;
	int m_TargetNum;
	bool m_IsStart;
	//bool *m_pWaiting;
};

class WanderingsManager
{
public:
	~WanderingsManager();
	static void LoadFile(const char* fileName);
	static WanderingsInfo *GetWanderingsInfo(std::string myName);

private:
	WanderingsManager();
	static WanderingsManager *GetInstance();

private:
	class WanderingsPimpl;
	WanderingsPimpl *m_pWanderingsPimpl;
};

#endif