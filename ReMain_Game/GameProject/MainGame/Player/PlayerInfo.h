#ifndef _PLAYER_INFO_H_
#define _PLAYER_INFO_H_

#include "../Character.h"

#define DeadValue 0.1f	//�p�b�h���͂̃f�b�h�l

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
	eAnim_RecoilGun,		//�V���b�g�K������
	eAnim_RecoilHandgun,	//�n���h�K������
	eAnim_ReloadGun,		//�V���b�g�K�������[�h
	eAnim_ReloadHandgun,	//�n���h�K�������[�h
	eAnim_Run,				//����
	eAnim_RunTakeGun,		//�V���b�g�K���������đ���
	eAnim_RunTakeHandgun,	//�n���h�K���������đ���
	eAnim_SetupGun,			//�V���b�g�K������������Ԃ���\����
	eAnim_SetupHandgun,		//�n���h�K������������Ԃ���\����
	eAnim_StealthAttack,	//�X�e���X�A�^�b�N
	eAnim_TakeGun,			//�V���b�g�K������Ɏ���
	eAnim_TakeHandgun,		//�n���h�K������Ɏ���
	eAnim_Walk,				//����
	eAnim_WalkTakeGun,		//�V���b�g�K���������ĕ���
	eAnim_WalkTakeHandgun,	//�n���h�K���������ĕ���
	eAnim_WalkReloadGun,	//�����[�h������
	eAnim_WalkReloadHandgun	//�����[�h������
};

//�v���C���[�̏��
enum EPlayerState
{
	eState_Idle,
	eState_Walk,
	eState_Run,
	eState_Crouch,
	eState_StandUp,
	eState_TakeWeapon,
	eState_PutBackWeapon,
	eState_SetupWeapon,
	eState_StealthAttack,
	eState_Recoil,
	eState_Reload,
	eState_Hit,
	eState_Die,
};

struct CameraInfo
{
	bool *pIsStop;
	bool *pSetupWeapon;	//�e���\���Ă��邩
	DynamicMesh *pModel;
	Vector3D *pPos;
	Vector3D *pRot;
	Vector3D *pLookPos;	//�J�����̒����_
	EPlayerState *pState;
};

#endif