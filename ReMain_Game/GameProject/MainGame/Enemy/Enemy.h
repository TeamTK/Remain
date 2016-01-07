#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "..\Character.h"
#include "..\..\GameSystem\SightSystem.h"
#include "..\..\GEKO\Task\FunctionTask.h"

//�G�̃X�e�[�^�X
struct EnemyState
{
	int flinch;
	float hp;
	float walkSpeed;
	float runSpeed;
	float mapHitRadius;
	float bodyRadius;
	float sightAngle;
	float sightDistance;
	Vector3D posSpawn;
	Vector3D rotation;
};

//�{�[���̃J�v�Z�����
struct BoneCalpule
{
	float radius;
	int start;
	int end;
	const char *name;
	BoneCalpule(float r, int s, int e, const char* name) :
		radius(r),
		start(s),
		end(e),
		name(name) {}
};

//���̓G
class Enemy : public Character
{
public:
	Enemy(const char* name, EnemyState &enemyState);
	virtual ~Enemy();
	virtual void Update();
	void HitBullet(Result_Sphere& r);
	void HitAttack(Result_Capsule &hitData);

protected:
	void Attack(unsigned int animNum, int animEndTime);
	void Idle(unsigned int animNum);
	void Chase(unsigned int animNum);
	void HitDamage(unsigned int animNum, int animEndTime);
	void Die(unsigned int animNum, int animEndTime);

private:
	void HitSight(const Vector3D *pPos);

protected:
	int m_FlinchNum; //�ϋv�l
	float m_FlinchCnt; //�ϋv�~�ϒl
	float m_WalkSpeed; //�������x
	float m_RunSpeed; //���鑬�x

	//�G�̎��E
	EnemySight m_Sight; //���E�V�X�e��
	SightData m_SightData; //���E�̃f�[�^
	Vector3D m_SightPos; //���E�̈ʒu
	Vector3D m_SightVec; //���E�̕���

	//�ǐ՗p
	Vector3D m_Distance; //�G�ƃv���C���[�Ƃ̋���
	const Vector3D *m_pPlayerPos; //�v���C���[�̈ʒu�̃|�C���^

	//�v���C���[����̍U�����ꂽ���̓����蔻��
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> m_BoneCapsule; //�{�[�����i�J�v�Z���j

	std::vector<float> m_DamageMagnification; //�_���[�W�{��

	Collider *m_pHitAttack; //�v���C���[�ւ̍U���̓����蔻��
	FunctionTask m_FuncTask; //AI�Ǘ�
};

#endif