#include "EnemyWave.h"
#include "EnemyStateManager.h"

EnemyWave::EnemyWave(const EnemyWaveInfo& waveInfo) :
	Task("EnemyWave", 5),
	m_IsSearch(waveInfo.isSearch),
	m_StateName(waveInfo.stateName),
	m_SpawnName(waveInfo.spawnName),
	m_TracerouteName(waveInfo.tracerouteName),
	m_WaveCntNum(0),
	m_WaveAllNum(waveInfo.waveAllNum),
	m_IntervalTime(waveInfo.intervalTime)
{
	m_WaveInterval.Start();
}

EnemyWave::~EnemyWave()
{
}

void EnemyWave::Update()
{
	//ウエーブ数が最大になると終了
	if (m_WaveCntNum >= m_WaveAllNum)
	{
		SetKill();
		return;
	}

	//指定の時間になったら敵を出現
	if (m_WaveInterval.GetSecond() > m_IntervalTime)
	{
		m_WaveCntNum++;
		m_WaveInterval.Start();
		EnemyStateManager::Generation(m_StateName, m_SpawnName, m_TracerouteName, m_IsSearch);
	}
}