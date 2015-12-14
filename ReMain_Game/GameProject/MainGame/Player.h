#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"

//�v���C���[�̃A�j���[�V����
enum EPlayerAnimation
{
	eAnim_Crouch,			//���Ⴊ�݈ڍs
	eAnim_CrouchIdle,		//���Ⴊ�ݑҋ@
	eAnim_CrouchWalk,		//���Ⴊ�ݕ���
	eAnim_Die,				//����
	eAnim_Hit,				//��U��
	eAnim_Idle,				//�ҋ@���
	eAnim_IdleTakeGun,		//�V���b�g�K�������ҋ@���
	eAnim_IdleTakeHandgun,	//�n���h�K�������ҋ@���
	eAnim_Run,				//����
	eAnim_RunTakeGun,		//�V���b�g�K���������đ���
	eAnim_RunTakeHandgun,	//�n���h�K���������đ���
	eAnim_SetupGun,			//�V���b�g�K������������Ԃ���\����
	eAnim_SetupHandgun,		//�n���h�K������������Ԃ���\����
	eAnim_TakeGun,			//�V���b�g�K������Ɏ���
	eAnim_TakeHandgun,		//�n���h�K������Ɏ���
	eAnim_Walk,				//����
	eAnim_WalkTakeGun,		//�V���b�g�K���������ĕ���
	eAnim_WalkTakeHandgun	//�n���h�K���������ĕ���
};

//�v���C���[�̏��
enum EPlayerState
{
	eState_Idle,
	eState_Walk,
	eState_Run,
	eState_Crouch,
	eState_TakeWeapon,
	eState_SetupWeapon,
	eState_Hit,
	eState_Die,
};

enum EWeapons
{
	eShotgun = 0,
	eHandgun
};

class Player : public CCharacter
{
public:
	Player();
	~Player();
	void Update();
	void Move();
	void Attack();
	void Camera();
	void Animation();
	Matrix GetBomeMat(int bornIndex);
	int GetAnim();
	float GetPlayTime();
	void SetGunMtx(Matrix m);

	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void TakeWeapon();
	void SetupWeapon();
	void Die();
	void Hit();

private:
	void HitCamera(Result_Porygon &hitData);
	Vector3D Lerp(Vector3D start, Vector3D finish, float percentage);

private:
	Vector3D m_Dir;
	Vector3D m_CamDir;
	Vector3D m_CameraPos;
	Vector3D m_LookPos;
	Matrix m_GunMatrix;
	EPlayerAnimation m_Anim;
	EPlayerState m_State;
	EWeapons m_Weapons;
	Collider m_HitCamera;
	float m_Vertical;
	float m_Horizontal;
	float m_MoveSpeed;
	float m_CameraPosY;
	float m_CameraT;
	bool m_ChangeTakeWeapon;	//�e������
	bool m_isTakeWeapon;	//�e�������Ă��邩
	bool m_isCrouch;	//���Ⴊ��ł��邩
	bool m_isRun;	//�����Ă��邩
	bool m_isMove;		//�ړ�����
	bool m_isAttack;	//�U������
};
#endif
