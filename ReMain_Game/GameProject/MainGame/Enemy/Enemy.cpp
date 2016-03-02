#include "Enemy.h"
#include "../Player/Player.h"
#include "../../GameSystem/Effect/EffectAnimation.h"
#include "../AmmoBox/AmmoBox_Shotgun.h"
#include "../AmmoBox/AmmoBox_Handgun.h"
#include <random>

#define SEARCH_INTERVAL 60 //�o�H�T���J�n�Ԋu
#define DROP_PROBABILITY 2 //�A�C�e���h���b�v��
#define GUN_POWER_HANDGUN 2.0f //�V���b�g�K���̈З�
#define GUN_POWER_SHOTGUN 5.0f //���e�̈З�

Enemy::Enemy(const char* name, EnemyState &enemyState) :
	Character(10, name, 1),
	m_FlinchCnt(0.0f),
	m_AnimSpeed(ENEMY_NORMAL_SPEED),
	m_SearchCnt(0),
	m_OneFlameTime(0)
{
	//�e��ݒ�l���蓖��
	m_SphereMap.radius = enemyState.mapHitRadius; //�}�b�v�Ƃ̔��a
	m_BodyRadius = enemyState.bodyRadius; //�G�̑̂̔��a
	m_Hp = enemyState.hp;
	m_RunSpeed = enemyState.runSpeed;
	m_WalkSpeed = enemyState.walkSpeed;
	m_FlinchNum = enemyState.flinch;
	m_AuditoryRange = 10.0f;
	m_pos = enemyState.posSpawn;
	m_rot = enemyState.rotation;
	m_Model.SetAsset(name, true);

	//���E�V�X�e��
	m_SightData.angle = enemyState.sightAngle;
	m_SightData.distance = enemyState.sightDistance;
	m_SightData.pSightPos = &m_SightPos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(Enemy::HitSight));

	//���o�T�m
	m_AuditorySense.SetFunc(REGIST_FUNC(Enemy::Auditory));
	m_AuditorySense.SetPos(&m_pos);
	m_AuditorySense.SetDetectionRange(&m_AuditoryRange);

	//�o�H�T��
	m_Search.SetPosition(&m_pos);
	m_Search.SetTargetName("Player");
	m_Search.SetTopography(enemyState.topographyName);

	//�o�H�T���J�n���f
	if (enemyState.isSearch)
	{
		m_Search.StartSerch();
		m_Search.StartMove();
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_pPlayerPos = g_pPlayerPos;
	}
	else
	{
		m_Search.StopSerch();
		m_Search.StopMove();
	}

	//�p�j������
	m_Wandering.Init(enemyState.wanderingName, &m_Search, &m_pos, &m_rot);
}

Enemy::~Enemy()
{
	m_BoneCapsule.clear();
	m_BoneCapsule.shrink_to_fit();

	delete[] m_pHitAttackBody;
	delete[] m_pCapsule;

	delete[] m_pHitAttack;
}

void Enemy::Attack()
{
	if (m_Model.GetPlayTime(COMMON_BORN_ANIM_ENEMY) >= m_AnimEndTime)
	{
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_FuncTask.Stop("Attack");
		m_FuncTask.Start("Chase");
	}
}

void Enemy::Idle()
{

}

void Enemy::Chase()
{
	//�v���C���[��ǐ�
	m_Distance = (*m_pPlayerPos - m_pos);
	float leng = m_Distance.LengthSq();

	//�U�����f(�o�H�T���ŃS�[��������)
	if (m_Search.GetIsGoal())
	{
		m_pos += m_Distance.GetNormalize() * m_RunSpeed * m_OneFlameTime;
		m_rot = Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0);

		if (leng < 3)
		{
			m_Model.SetTime(0);
			m_FuncTask.Stop("Chase");
			m_FuncTask.Start("Attack");
		}
		m_Search.StopMove();
	}
	else
	{
		m_Search.StartMove();
		m_Search.StartSerch();

		m_pos += *m_Search.GetTargetDirection() * m_WalkSpeed * m_OneFlameTime;
		m_rot = Vector3D(0.0f, atan2f(m_Search.GetTargetDirection()->x, m_Search.GetTargetDirection()->z), 0.0);
	}
}

void Enemy::HitDamage()
{
	if (m_Model.GetPlayTime(m_JudgementAnim) >= m_AnimEndTime)
	{
		m_FuncTask.Stop("HitDamage");
		m_FuncTask.Start("Chase");
	}
}

void Enemy::Die()
{
	if (m_Model.GetPlayTime(m_JudgementAnim) >= m_AnimEndTime)
	{
		std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
		std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
		std::uniform_int_distribution<> rand(0, 1);
		std::uniform_int_distribution<> rand2(0, DROP_PROBABILITY);

		//�e���h���b�v�i5����1�j
		if (rand2(mt) == 0)
		{
			switch (rand(mt))
			{
			case 0:
				new AmmoBox_Shotgun(m_pos, Vector3D(0.0f, 0.0f, 0.0f), 7);
				break;

			case 1:
				new AmmoBox_Handgun(m_pos, Vector3D(0.0f, 0.0f, 0.0f), 6);
				break;

			default:
				break;
			}
		}

		m_Model.StopAnimation();
		m_FuncTask.AllStop();
		Task::SetKill();
	}
}

void Enemy::Wandering()
{
	m_Wandering.Update();
}

void Enemy::Update()
{
	m_OneFlameTime = GEKO::GetOneFps();
	//�U������鑤�̓����蔻��X�V
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCapsule[i].radius = m_BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(m_BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(m_BoneCapsule[i].end);

		if (!m_FuncTask.Running("Die")) m_pHitAttackBody[i].Awake();
	}
	m_SphereMap.pos = m_pos;
	m_SphereMap.pos.y += m_SphereMap.radius;

	//���E�̍X�V
	m_SightPos = m_pos;
	m_SightPos.y += 2.0f;
	m_SightVec = m_Model.GetAxisZ(1.0f);

	//�A�j���[�V�����؂�ւ��Ƒ��x
	m_Model.SetPlayTime(m_AnimSpeed * m_OneFlameTime);
	m_Model.ChangeAnimation(m_AnimType);

	m_FuncTask.Update();
	m_FuncTask.RunningDraw();

	//�o�H�T��������Ԋu
	if (m_SearchCnt >= SEARCH_INTERVAL && m_Sight.GetSleep())
	{
		m_Search.StartSerch();
		m_SearchCnt = 0;
	}
	else
	{
		m_Search.StopSerch();
		m_SearchCnt++;
	}
	Character::Update();
	//m_Search.DebugMigrationPath();
}

void Enemy::HitBullet(Result_Sphere& r)
{
	//�����Ԃ��̃G�t�F�N�g
	EffectAnimationInfo info;
	info.frameNum = 8;
	info.pos = r.position;
	info.size = 2.0f;
	info.sizeW = 256;
	info.sizeH = 256;
	info.speed = 1.0f;
	new EffectAnimation("BloodAnim", info);

	m_Model.SetTexture(&m_DamegeBlood);

	//�e�̎�ނ��Ƃ̈З�
	float GunPower = 1.0f;
	if (r.targetName == "HandGun")
	{
		GunPower = GUN_POWER_HANDGUN;
	}
	else if (r.targetName == "ShotGun")
	{
		GunPower = GUN_POWER_SHOTGUN;
	}

	//�����蔻���~�i�e����́j
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttackBody[i].Sleep();

		//���ʂ��Ƃ̃_���[�W�v�Z
		if (r.name == m_pHitAttackBody[i].GetName())
		{
			float damegeNum = m_DamageMagnification[i] * GunPower;
			m_Hp -= damegeNum;
			m_FlinchCnt += damegeNum;
		}
	}

	m_FuncTask.AllStop();

	//HP�[���ɂȂ�Ύ��S
	if (m_Hp >= 0)
	{
		//���ݓ���
		if (m_FlinchCnt >= (float)m_FlinchNum)
		{
			m_Model.SetTime(0);
			m_FuncTask.Start("HitDamage");
			m_FlinchCnt = 0;
			for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		}
		else
		{
			m_FuncTask.Start("Chase");
		}

		//�v���C���[�𔭌����Ă��Ȃ��čU�����󂯂���
		if (!m_Sight.GetSleep())
		{
			m_pPlayerPos = g_pPlayerPos;
			m_Sight.Sleep();
			m_AuditorySense.Stop();
			m_Search.StartSerch();
			m_Search.StartMove();
			m_Wandering.Stop();
		}
	}
	else
	{
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_Model.SetTime(0);
		m_FuncTask.AllStop();
		m_FuncTask.Start("Die");
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_Wandering.Stop();
	}
}

void Enemy::HitAttack(Result_Capsule &hitData)
{
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
}

void Enemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = g_pPlayerPos;
	m_FuncTask.AllStop();
	m_FuncTask.Start("Chase");
	m_Sight.Sleep();
	m_AuditorySense.Stop();
	m_Search.StartSerch();
	m_Search.StartMove();
	m_Wandering.Stop();
}

void Enemy::Auditory(int volume)
{
	switch (volume)
	{
	case eOblivious:
		break;

	case eCaution:
		break;

	case eDiscovery:
		m_pPlayerPos = g_pPlayerPos;
		m_FuncTask.AllStop();
		m_FuncTask.Start("Chase");
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_Search.StartSerch();
		m_Search.StartMove();
		m_Wandering.Stop();
		break;

	default:
		break;
	}
}