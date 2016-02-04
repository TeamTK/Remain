#ifndef _ENEMY_STATE_MANAGER_H_
#define _ENEMY_STATE_MANAGER_H_

class EnemyStateManager
{
public:
	~EnemyStateManager();
	static void LoadFileState(const char *fileName);
	static void LoadFileSpawn(const char *fileName);
	static void Generation(const char *stateName , const char *spawnName, const char* tracerouteName, bool isSearch);

private:
	EnemyStateManager();
	static EnemyStateManager *GetInstance();

private:
	class  EnemyStatePimpl;
	EnemyStatePimpl *m_pEnemyStatePimpl;
};

#endif