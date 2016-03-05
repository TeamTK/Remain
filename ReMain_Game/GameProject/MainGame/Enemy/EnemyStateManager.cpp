#include "EnemyStateManager.h"
#include "Monster_A.h"
#include "Monster_B.h"

#include <map>
#include <vector>
#include <assert.h>
#include <string>

//�G�̐ݒ���
struct EnemyStateInfo
{
	int flinch;
	float hp;
	float walkSpeed;
	float runSpeed;
	float mapHitRadius;
	float bodyRadius;
	float sightAngle;
	float sightDistance;
};

//�G�̏o�����
struct EnemySpawnInfo
{
	Vector3D pos;
	Vector3D rotation;
	std::string wanderingName;

	EnemySpawnInfo(const Vector3D &pos, const Vector3D &rotation, std::string wanderingName) :
		pos(pos),
		rotation(rotation),
		wanderingName(wanderingName){}
};

class EnemyStateManager::EnemyStatePimpl
{
public:
	std::map<std::string, EnemyStateInfo> mapState;
	std::map<std::string, std::map<std::string, std::vector<EnemySpawnInfo>>> mapSpawn;
};

//�G�̐ݒ�Ǘ�
EnemyStateManager::EnemyStateManager()
{
	m_pEnemyStatePimpl = new EnemyStatePimpl;
}

EnemyStateManager::~EnemyStateManager()
{
	m_pEnemyStatePimpl->mapState.clear();

	auto it = m_pEnemyStatePimpl->mapSpawn.begin();
	auto itEnd = m_pEnemyStatePimpl->mapSpawn.end();
	for (; it != itEnd; it++)
	{
		auto it2 = it->second.begin();
		auto it2End = it->second.end();
		for (; it2 != it2End; it2++)
		{
			it2->second.shrink_to_fit();
			it2->second.clear();
		}
		it->second.clear();
	}
	m_pEnemyStatePimpl->mapSpawn.clear();

	delete m_pEnemyStatePimpl;
}

EnemyStateManager *EnemyStateManager::GetInstance()
{
	static EnemyStateManager enemyStateManager;
	return &enemyStateManager;
}

void EnemyStateManager::LoadFileState(const char *fileName)
{
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, fileName, "rt");
	assert(fp != NULL && "�G�̏��ǂݍ��ݎ��s");

	char key[256];
	EnemyStateInfo info;

	std::string str;

	while (!feof(fp))
	{
		fscanf_s(fp, "%s", key, sizeof(key));

		if (strcmp(key, "EnemyState") == 0)
		{
			//��Ԃ̖��O
			fscanf_s(fp, "%s", key, sizeof(key));
			str = key;

			//���ݒl
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.flinch = std::stoi(key);

			//�̗�
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.hp = std::stof(key);

			//�����X�s�[�h
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.walkSpeed = std::stof(key);

			//����X�s�[�h
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.runSpeed = std::stof(key);

			//�n�`�̓����蔼�a
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.mapHitRadius = std::stof(key);

			//�L�����N�^���s�̓����蔼�a
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.bodyRadius = std::stof(key);

			//���E�̊p�x
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.sightAngle = std::stof(key);

			//���E�̔F������
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));
			info.sightDistance = std::stof(key);

			GetInstance()->m_pEnemyStatePimpl->mapState[str].flinch = info.flinch;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].hp = info.hp;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].walkSpeed = info.walkSpeed;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].runSpeed = info.runSpeed;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].mapHitRadius = info.mapHitRadius;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].bodyRadius = info.bodyRadius;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].sightAngle = info.sightAngle;
			GetInstance()->m_pEnemyStatePimpl->mapState[str].sightDistance = info.sightDistance;
		}
	}
}

void EnemyStateManager::LoadFileSpawn(const char *fileName)
{
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, fileName, "rt");
	assert(fp != NULL && "�G�̃X�|�[�����ǂݍ��ݎ��s");

	char key[256];
	std::string enemySpawnName;
	std::string enemyName;

	int cnt = 0;
	float x, y, z;
	x = y = z = 0.0f;

	std::vector<Vector3D> position;
	std::vector<Vector3D> rotation;
	std::vector<std::string> wanderingName;

	while (!feof(fp))
	{
		fscanf_s(fp, "%s", key, sizeof(key));

		//EnemySpawn�P�ʂœǂݍ���
		if (strcmp(key, "EnemySpawn") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			enemySpawnName = key;

			fscanf_s(fp, "%s", key, sizeof(key));

			while (strcmp(key, "End") != 0)
			{
				fscanf_s(fp, "%s", key, sizeof(key));

				//�G�̎�ނ��Ƃɓǂݍ���
				if (strcmp(key, "EnemyName") == 0)
				{
					//�G�̖��O
					fscanf_s(fp, "%s", key, sizeof(key));
					enemyName = key;

					fscanf_s(fp, "%s", key, sizeof(key));
					fscanf_s(fp, "%s", key, sizeof(key));

					//�ʒu�̐�
					fscanf_s(fp, "%s", key, sizeof(key));
					cnt = std::stoi(key);

					//�ʒu�̓ǂݍ���
					fgets(key, sizeof(key), fp);
					for (int i = 0; i < cnt; i++)
					{
						fgets(key, sizeof(key), fp);
						sscanf_s(key, "%f, %f, %f", &x, &y, &z);
						position.emplace_back(x, y, z);
					}

					fscanf_s(fp, "%s", key, sizeof(key));

					//��]�l�̐�
					fscanf_s(fp, "%s", key, sizeof(key));
					cnt = std::stoi(key);

					//��]�l�ǂݍ���
					fgets(key, sizeof(key), fp);
					for (int i = 0; i < cnt; i++)
					{
						fgets(key, sizeof(key), fp);
						sscanf_s(key, "%f, %f, %f", &x, &y, &z);
						rotation.emplace_back(x, y, z);
					}

					fscanf_s(fp, "%s", key, sizeof(key));

					//�p�j�p�^�[���̖��O�̐�
					fscanf_s(fp, "%s", key, sizeof(key));
					cnt = std::stoi(key);

					//�p�j�p�^�[���̖��O�ǂݍ���
					fgets(key, sizeof(key), fp);
					for (int i = 0; i < cnt; i++)
					{
						fscanf_s(fp, "%s", key, sizeof(key));
						wanderingName.emplace_back(key);
					}

					unsigned int posNum = position.size();
					unsigned int rotNum = rotation.size();
					unsigned int nameNum = wanderingName.size();
					assert(posNum == rotNum && "�ʒu�Ɖ�]�̍��v����v���܂���");

					//�i�[
					EnemyStatePimpl * p = GetInstance()->m_pEnemyStatePimpl;
					for (int i = 0; i < cnt; i++)
					{
						p->mapSpawn[enemySpawnName][enemyName].emplace_back(position[i], rotation[i], wanderingName[i]);
					}

					position.shrink_to_fit();
					position.clear();
					rotation.shrink_to_fit();
					rotation.clear();
					wanderingName.shrink_to_fit();
					wanderingName.clear();
				}
			}	
		}
	}
}

void EnemyStateManager::Generation(const char *stateName, const char *spawnName, const char* tracerouteName, bool isSearch)
{
	EnemyStateInfo stateinfo = GetInstance()->m_pEnemyStatePimpl->mapState[stateName];

	EnemyState state;
	state.isSearch = isSearch;
	state.flinch = stateinfo.flinch;
	state.hp = stateinfo.hp;
	state.walkSpeed = stateinfo.walkSpeed;
	state.runSpeed = stateinfo.runSpeed;
	state.mapHitRadius = stateinfo.mapHitRadius;
	state.bodyRadius = stateinfo.bodyRadius;
	state.sightAngle = stateinfo.sightAngle;
	state.sightDistance = stateinfo.sightDistance;
	state.topographyName = tracerouteName;

	//�w�萔�̓G�𐶐����ăp�����[�^�[�����蓖��
	for (auto& it : GetInstance()->m_pEnemyStatePimpl->mapSpawn[spawnName])
	{
		if (it.first == "Monster_A")
		{
			unsigned int num = it.second.size();
			for (unsigned int i = 0; i < num; i++)
			{
				state.posSpawn = it.second[i].pos;
				state.rotation = it.second[i].rotation;
				state.wanderingName = it.second[i].wanderingName;
				new Monster_A(state);
			}
		}
		else if (it.first == "Monster_B")
		{
			unsigned int num = it.second.size();
			for (unsigned int i = 0; i < num; i++)
			{
				state.posSpawn = it.second[i].pos;
				state.rotation = it.second[i].rotation;
				state.wanderingName = it.second[i].wanderingName;
				new Monster_B(state);
			}
		}
	}
}