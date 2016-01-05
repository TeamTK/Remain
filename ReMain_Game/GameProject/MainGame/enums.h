#ifndef ENUMS_H
#define ENUMS_H

//�v���C���[�̃A�j���[�V����
enum EPlayerAnim
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
	eState_PutBackWeapon,
	eState_SetupWeapon,
	eState_Reload,
	eState_Hit,
	eState_Die,
};

//����
enum EWeapons
{
	eHand,
	eShotgun,
	eHandgun,
	eNone,
	WeaponAll
};

#endif