#include "EnemyStateManager.h"
#include "Monster_A.h"
#include "Monster_B.h"

#include <map>

class EnemyStateManager::EnemyStatePimpl
{
public:
	std::map<const char*, EnemyState> mapState;
};

EnemyStateManager::EnemyStateManager()
{
	m_pEnemyStatePimpl = new EnemyStatePimpl;
}

EnemyStateManager::~EnemyStateManager()
{
	delete m_pEnemyStatePimpl;
}

EnemyStateManager *EnemyStateManager::GetInstance()
{
	static EnemyStateManager enemyStateManager;
	return &enemyStateManager;
}

void EnemyStateManager::LoadFileState(const char *fileName)
{

}

void EnemyStateManager::Generation(const char *name)
{
	//EnemyState state = m_pEnemyStatePimpl->mapState[name];
	EnemyState state;
	state.enemyName = "Monster_A";
	state.maxSpawn = 1;
	state.flinch = 2;
	state.hp = 10.0f;
	state.walkSpeed = 0.03f;
	state.runSpeed = 0.07f;
	state.mapHitRadius = 0.2f;
	state.bodyRadius = 0.2f;
	state.sightAngle = 60;
	state.sightDistance = 10.0f;
	state.posSpawn = Vector3D(-9.0f, 0.0f, 2.0f);
	state.rotation = Vector3D(0.0f, 0.0f, 0.0f);

	if (state.enemyName == "Monster_A")
	{
		new Monster_A(state);
	}
	else if(state.enemyName == "Monster_B")
	{
		new Monster_B(state);
	}
}