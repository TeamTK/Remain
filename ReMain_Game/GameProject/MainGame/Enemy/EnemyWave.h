#ifndef _ENEMY_WAVE_H_
#define _ENEMY_WAVE_H_

#include "..\..\GEKO\GEKO.h"

struct EnemyWaveInfo
{
	bool isSearch;
	int waveAllNum;
	int intervalTime;
	const char *stateName;
	const char *spawnName;
	const char *tracerouteName;
};

class EnemyWave : public Task
{
public:
	EnemyWave(const EnemyWaveInfo& waveInfo);
	~EnemyWave();
	void Update();

private:
	bool m_IsSearch;
	const char *m_StateName;
	const char *m_SpawnName;
	const char *m_TracerouteName;
	int m_WaveCntNum;
	int m_WaveAllNum;
	int m_IntervalTime;
	Timer m_WaveInterval;
};

#endif