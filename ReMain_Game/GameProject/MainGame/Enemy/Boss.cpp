#include "Boss.h"

#define BOSS_ATTACK_ENDTIME 29
#define BOSS_HITDAMAGE_ENDTIME 29
#define BOSS_DIE_ENDTIME 28

#define BOSS_ATTACK_SPEED 20.0f
#define BOSS_NORMAL_SPEED 30.0f

Boss::Boss(BossState &bossState) :
	Task("Boss", 1),
	m_AnimType(eAnimationAttack),
	m_AnimSpeed(BOSS_NORMAL_SPEED)
{
	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(Boss::Render));

	m_Model.SetAsset("Boss", true);
	m_Model.SetTranselate(bossState.spawnPos);
	m_Model.SetRotationDegree(0, (int)bossState.spawnRot.y, 0);
}

Boss::~Boss()
{

}

void Boss::Attack()
{
	m_AnimType = eAnimationAttack;
	m_AnimSpeed = BOSS_ATTACK_ENDTIME;
	m_AnimEndTime = BOSS_HITDAMAGE_ENDTIME;
}

void Boss::Idle()
{
	m_AnimType = eAnimationIdle;
	m_AnimSpeed = BOSS_NORMAL_SPEED;
}

void Boss::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimEndTime = BOSS_HITDAMAGE_ENDTIME;
}

void Boss::Die()
{
	//m_AnimType = eAnimationDie;
	m_AnimEndTime = BOSS_DIE_ENDTIME;
	m_AnimSpeed = BOSS_NORMAL_SPEED;
}

void Boss::Update()
{
	m_Model.ChangeAnimation(m_AnimType);
	m_Model.SetPlayTime(m_AnimSpeed * m_OneFlameTime);
}

void Boss::Render()
{
	m_Model.Render();
}