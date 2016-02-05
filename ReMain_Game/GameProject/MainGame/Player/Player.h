#ifndef PlAYER_H
#define PlAYER_H

#include "../../MainGame/Weapon/Shotgun.h"
#include "../../MainGame/Weapon/Handgun.h"
#include "../../MainGame/Weapon/Knife.h"
#include "../../GameSystem/GUI/UI_SelectWeapon.h"
#include "PlayerInfo.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
{
public:
	Player(Vector3D pos);
	~Player();
	void Update();
	void Move();
	void Attack();
	void Animation();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void StandUp();
	void TakeWeapon();
	void PutBackWeapon();
	void SetupWeapon();
	void Recoil();
	void Reload();
	void StealthAttack();
	void Die();
	void Hit();

	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);

private:
	Vector3D m_KeyDir;		//�L�[���͎�
	Vector3D m_LookPos;		//�J�����̒����_
	Vector3D m_SightPos;	//��������̈ʒu
	Matrix m_MatrixS;		//�V���b�g�K���p�v���C���[�̍s��
	Matrix m_MatrixH;		//�n���h�K���p�v���C���[�̍s��
	Matrix m_MatrixK;		//�i�C�t�p�v���C���[�̍s��
	UI_SelectWeapon m_SelectWeapon;
	EPlayerState m_State;	//�v���C���[�̏��
	EPlayerAnim m_Anim;		//�v���C���[�̃A�j���[�V����
	EWeapons m_SelectedWeapon; //�I�����Ă���e
	Collider m_Bullet;		//�e�̓����蔻��
	Collider m_HitAmmoBox;	//�e�򔠂̓����蔻��
	Collider m_HitEnemyAttack; //�G����̍U���̓����蔻��
	PlayerSightInfo m_PlayerSightInfo; //�v���C���[�̎��E���
	AuditoryObject m_AuditoryObject; //���o�Ώ�
	Timer m_Timer;					//�_���[�W��H�������̖��G����
	TracerouteTarget m_Target;		//�o�H�T���Ώ�
	Sound m_HandGunFire;			//�n���h�K���̔��C��
	Sound m_ShotGunFire;			//�V���b�g�K���̔��C��
	Sound m_GunEmpty;			    //�e���e�؂�ɂȂ������̉�
	Sound m_GunADS;					//�e���\�������̉�
	Sound m_HitEnemyAttackSound;	//�G����U����H�������

	CameraInfo m_CameraInfo;

	int m_Phase;
	int m_PlayAnim;			//����ɓn���p
	int m_Volume;			//�v���C���[���o�����̉���

	float m_MoveSpeed;		//�ړ����x
	float m_AnimSpeed;		//�A�j���[�V�����X�s�[�h
	float m_PlayAnimTime;	//����ɓn���p
	float m_Radius;			//�e�򔠂Ƃ̓����蔻��̔��a

	bool m_ChangeTakeWeapon;		//�e������
	bool m_ChangePutBackWeapon;		//�e�����܂�
	bool m_ToggleCrouch;			//���Ⴊ�݃g�O��
	bool m_isCrouch;				//���Ⴊ��ł��邩
	bool m_isShiftCrouch;			//���Ⴊ��ł��邩
	bool m_isTakeWeapon;			//�e�������Ă��邩
	bool m_SetupWeapon;				//�e���\���Ă��邩
	bool m_isRun;					//�����Ă��邩
	bool m_isMove;					//�ړ�����
	bool m_isAttack;				//�U������
	bool m_isReload;				//�����[�h����
	bool m_isHit;					//�G�̍U��������������
	bool m_isShot;					//���C������
	bool m_isSteAttack;				//�X�e���X�A�^�b�N
	bool m_isDead;					//���S���f
	bool m_IsStop;					//�v���C���[�Ɋ֌W����^�X�N��~
};

#endif