#include "Player.h"
#include "YouAreDead.h"
#include "TPSCamera.h"
#include "PlayerData.h"
#include "../../MainGame/Bullet/Bullet.h"
#include "../../GameSystem/Effect/EffectSphere.h"
#include "../../GameSystem/Effect/EffectParabola.h"
#include "../../GameSystem/GUI/UI_Reticle.h"
#include "../../GameSystem/GUI/UI_AmmoNum.h"
#include "../../GameSystem/Effect/EffectAnimation.h"
#include "../../GameSystem/ScreenBlood.h"

#define WALK_SPEED 3.0f			  //�����X�s�[�h
#define RUN_SPEED 6.0f				  //����X�s�[�h
#define CROUCH_WALK_SPPED 1.0f		  //���Ⴊ�ݕ����X�s�[�h
#define	SETUPWEAPON_MOVE_SPEED 1.0f; //������\�������̈ړ����x
#define MAP_HIT_RADIUS 0.4f			  //�}�b�v�Ƃ̓����蔻��̔��a
#define MAP_HIT_RADIUS_SETWEAPON 0.8f //�}�b�v�Ƃ̓����蔻��̔��a(����\����)

#define PI 3.14159265359f

//�A�j���[�V�����X�s�[�h
#define DEFAULT_ANIM_SPEED 30	//�f�t�H���g�X�s�[�h
#define TWICE_ANIM_SPEED 60	//2�{��
#define HALF_ANIM_SPEED 15	//0.5�{��
#define RUN_ANIM_SPEED 50	//����
#define ANIM_SPEED_40 35	//������Ƃ�
#define ANIM_SPEED_10 10	//�����[�h

//�v���C���[���o������
#define VOLUME_CROUCH 1
#define VOLUME_WALK 2
#define VOLUME_RUN 3
#define VOLUME_ATTACK 3

Vector3D *g_pPlayerPos;

Player::Player(Vector3D pos) :
	Character(100.0f, "Player", 0),
	m_LookPos(-47.9f, 1.7f, -11.4f),
	m_KeyDir(0.0f, 0.0f, 0.0f),
	m_MoveSpeed(0.0f),
	m_AnimSpeed(30.0f),
	m_Radius(0.5f),
	m_Phase(0),
	m_OneFlameTime(0),
	m_isCrouch(false),
	m_isAttack(false),
	m_isTakeWeapon(false),
	m_isMove(false),
	m_isRun(false),
	m_SetupWeapon(false),
	m_ToggleCrouch(false),
	m_isShiftCrouch(false),
	m_isReload(false),
	m_isHit(false),
	m_isShot(false),
	m_ChangeTakeWeapon(false),
	m_isSteAttack(false),
	m_ChangePutBackWeapon(false),
	m_isDead(false),
	m_IsStop(false),
	m_State(EPlayerState::eState_Idle),
	m_SelectedWeapon(EWeapons::eShotgun)
{
	//�v���C���[���f��������
	m_Model.SetAsset("Player");
	m_Model.SetScale(1.0f, 1.0f, 1.0f);
	m_Model.SetTranselate(pos);
	m_Model.SetRotationRadian(0.0f, m_rot.y, 0.0f);
	m_Model.WorldMatrixBuilding();
	m_Model.SetTime(0);
	m_Model.Render();

	m_pos = pos;
	m_rot.y = 1.0f;

	//�J�����N���X�𐶐�
	m_CameraInfo.pIsStop = &m_IsStop;
	m_CameraInfo.pLookPos = &m_LookPos;
	m_CameraInfo.pModel = &m_Model;
	m_CameraInfo.pPos = &m_pos;
	m_CameraInfo.pRot = &m_rot;
	m_CameraInfo.pSetupWeapon = &m_SetupWeapon;
	m_CameraInfo.pState = &m_State;
	new TPSCamera(&m_CameraInfo);

	g_pShotgun = new Shotgun(&m_PlayAnim, &m_PlayAnimTime, &m_isTakeWeapon, &m_SelectedWeapon, &m_MatrixS);
	g_pHandgun = new Handgun(&m_PlayAnim, &m_PlayAnimTime, &m_isTakeWeapon, &m_SelectedWeapon, &m_MatrixH);

	//�e�򔠂̓����蔻��
	m_HitAmmoBox.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::HitAmmoBox));
	m_HitAmmoBox.SetID(eHITID1, eHITID2 | eHITID3);

	//�G�̍U���̓����蔻��
	m_HitEnemyAttack.Regist_C_vs_C(&m_pos, &m_SightPos, &m_Radius, REGIST_FUNC(Player::HitEnemyAttack));
	m_HitEnemyAttack.SetID(eHITID0, eHITID1);

	//�X�e�[�W�ړ��p
	m_StageChange.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::StageChange));
	m_StageChange.SetID(eHITID1, eHITID4);

	//�G���o��
	m_MapCol.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::HitMap));
	m_MapCol.SetID(eHITID1, eHITID5);

	m_SphereMap.radius = MAP_HIT_RADIUS;

	//���E�Ώۈʒu
	m_PlayerSightInfo.SetPos(&m_SightPos);

	g_pPlayerPos = &m_pos;

	m_JudgementAnim = 20; //�A�j���[�V�������f�i�{�[���P�ʁj

	//�v���C���[���o����
	m_Volume = VOLUME_WALK;
	m_AuditoryObject.SetPos(&m_pos);
	m_AuditoryObject.SetVolume(&m_Volume);

	//�o�H�T���Ώ�
	m_Target.SetName("Player");
	m_Target.SetPosition(&m_pos);

	//�e�֌W�̉�
	m_HandGunFire.SetAseet("HandGunFire");
	m_ShotGunFire.SetAseet("ShotGunFire");
	m_GunEmpty.SetAseet("GunEmpty");
	m_GunADS.SetAseet("GunADS");
	m_HitEnemyAttackSound.SetAseet("HitEnemyAttack");

	m_BodyRadius = 0.7f;

	//�v���C���[UI
	new UI_Reticle();
	new UI_AmmoNum();

	m_NoActionTime.Start();
}

Player::~Player()
{
	TaskManager::Kill("TPSCamera");
}

void Player::Update()
{
	//1�t���[���^�C��
	m_OneFlameTime = GEKO::GetOneFps();

	//�v���C���[������ł��Ȃ����̏���
	if (!m_isDead)
	{
		Move();
		Attack();
		m_SelectWeapon.Update();

		if (m_SelectWeapon.isSelected()) m_IsStop = true;
		else m_IsStop = false;
	}

	Animation();
	Character::Update();

	if ((m_Timer.GetSecond() >= 1.5) && !m_isDead)	 m_HitEnemyAttack.Awake();

	m_SightPos = m_Model.GetBornPos(6); //���̃{�[���ʒu
	m_MatrixK = m_Model.GetBornMatrix(25, true);	//�w�̃{�[���ʒu


	//�v���C���[�̃{�[���s��̐؂�ւ�
	if ((m_Anim == eAnim_TakeGun && m_Model.GetPlayTime(m_JudgementAnim) > 16) ||
		m_Anim == eAnim_SetupGun || m_Anim == eAnim_IdleTakeGun ||
		m_Anim == eAnim_WalkTakeGun || m_Anim == eAnim_RunTakeGun ||
		m_Anim == eAnim_RecoilGun || m_Anim == eAnim_ReloadGun ||
		(m_Anim == eAnim_Hit && m_isTakeWeapon) || m_Anim == eAnim_WalkReloadGun)
	{
		//��̃{�[��
		m_MatrixS = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		//���̃{�[��
		m_MatrixS = m_Model.GetBornMatrix(21, true);
	}

	if ((m_Anim == eAnim_TakeHandgun && m_Model.GetPlayTime(m_JudgementAnim) > 16) ||
		m_Anim == eAnim_SetupHandgun || m_Anim == eAnim_IdleTakeHandgun ||
		m_Anim == eAnim_WalkTakeHandgun || m_Anim == eAnim_RunTakeHandgun ||
		m_Anim == eAnim_RecoilHandgun || m_Anim == eAnim_ReloadHandgun ||
		(m_Anim == eAnim_Hit && m_isTakeWeapon) || m_Anim == eAnim_WalkReloadHandgun)
	{
		//��̃{�[��
		m_MatrixH = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		//���̃{�[��
		m_MatrixH = m_Model.GetBornMatrix(3, true);
	}

	m_PlayAnim = m_Model.GetPlayAnimation(m_JudgementAnim);
	m_PlayAnimTime = m_Model.GetPlayTime(m_JudgementAnim);

	m_Model.GetTranselate().DebugDraw("");
}

void Player::Move()
{
	m_KeyDir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = EPlayerState::eState_Idle;
	m_isMove = false;
	m_isRun = false;

	if (m_SelectWeapon.isSelected()) return;
	if (m_NoActionTime.GetSecond() < 2.0f)	return;

	if (Input::XInputPad1.GetIsConnection())
	{
		//�R���g���[���[����
		Vector3D PadDir = Vector3D((float)Input::XInputPad1.ThumbLeftX(), 0.0f, (float)Input::XInputPad1.ThumbLeftY());
		PadDir = (PadDir - Vector3D(128, 128, 128)) / 32767;
		if (abs(PadDir.x) < DeadValue && abs(PadDir.z) < DeadValue)
		{
			PadDir = Vector3D(0.0f, 0.0f, 0.0f);
		}
		m_KeyDir = PadDir;
	}
	else
	{
		//�O�㍶�E�ړ�
		if (Input::KeyW.Pressed())	m_KeyDir.z = 1;
		if (Input::KeyS.Pressed())	m_KeyDir.z = -1;
		if (Input::KeyD.Pressed())	m_KeyDir.x = 1;
		if (Input::KeyA.Pressed())	m_KeyDir.x = -1;
	}

	//����(WASD, ���X�e�B�b�N)
	if (m_KeyDir.x != 0 || m_KeyDir.z != 0)
	{
		m_State = EPlayerState::eState_Walk;
		m_isMove = true;
	}

	//����(���V�t�g�L�[, ���X�e�B�b�N����)
	if ((Input::KeyLShift.Pressed() || Input::XInputPad1.ThumbLeftPressed()) && m_isMove)
	{
		m_State = EPlayerState::eState_Run;
		m_isRun = true;
	}

	//���Ⴊ��(���R���g���[���L�[, �E�g���K�[) 
	if ((Input::KeyLControl.Clicked() || Input::XInputPad1.TriggerRightClicked()) && !m_isShiftCrouch)
	{
		m_isShiftCrouch = true;
		m_ToggleCrouch = !m_ToggleCrouch;
	}

	if (m_ToggleCrouch)
	{
		m_State = EPlayerState::eState_Crouch;
	}
	else if (m_isCrouch && !m_ToggleCrouch)
	{
		m_State = EPlayerState::eState_StandUp;
		m_isShiftCrouch = true;
	}

	//�v���C���[��������ԂȂ�ړ�����
	if (m_isMove && m_Anim != EPlayerAnim::eAnim_TakeGun && m_Anim != EPlayerAnim::eAnim_TakeHandgun)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot = Vector3D(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_KeyDir.x, m_KeyDir.z), 0.0f);
		m_rot.y = playerRot.y;

		if (m_SetupWeapon)
		{
			//������\������Ԃ̈ړ�
			m_pos += m_Model.GetAxisX(1.0f) * m_KeyDir.x * m_MoveSpeed * m_OneFlameTime;
			m_pos += m_Model.GetAxisZ(1.0f) * m_KeyDir.z * m_MoveSpeed * m_OneFlameTime;
			//�v���C���[�̏㉺�ړ�
			m_Phase++;
			m_pos.y += sinf(m_Phase * (PI * 10) / 150.0f) / 120.0f;
		}
		else
		{
			//�ʏ��Ԃ̈ړ�
			Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
			m_pos += (pos * m_MoveSpeed) * m_OneFlameTime;
		}
	}
}

void Player::Attack()
{
	//����̐؂�ւ�(�z�C�[���N���b�N, �����L�[���E)
	if ((Input::Mouse.WheelClicked() || Input::XInputPad1.ThumbRightClicked()))
	{
		m_SelectedWeapon = m_SelectWeapon.Select();
	}

	if (m_SelectWeapon.isSelected()) return;

	//������Ƃ�
	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && !m_isCrouch && !m_SetupWeapon)
	{
		if (m_isTakeWeapon && !m_ChangePutBackWeapon && !m_ChangeTakeWeapon)
		{
			m_ChangePutBackWeapon = true;
			m_Model.SetTime(29);
		}
		else if (!m_isTakeWeapon && !m_ChangePutBackWeapon && !m_ChangeTakeWeapon)
		{
			m_ChangeTakeWeapon = true;
			m_Model.SetTime(0);
		}
	}

	//�e�������Ă���Ƃ��ɏe���\����(�}�E�X�E�N���b�N, ���V�����_�[)
	if ((Input::Mouse.RPressed() || Input::XInputPad1.ShoulderLeftPressed()) && (m_isTakeWeapon && !m_isReload))
	{
		if (!m_SetupWeapon) m_GunADS.Play(); //�\����
		m_AnimSpeed = 30;
		m_State = EPlayerState::eState_SetupWeapon;
		m_SetupWeapon = true;
	}
	else
	{
		m_SetupWeapon = false;
		m_Model.StartAnimation();

		//�����[�h
		if ((Input::KeyR.Clicked() || Input::XInputPad1.XClicked()) && m_isTakeWeapon && !m_isReload && !m_isShot)
		{
			if (m_SelectedWeapon == EWeapons::eShotgun &&
				g_pShotgun->GetLoadedAmmo() < AMMO_LOADED_SHOTGUN &&
				g_pShotgun->GetAmmo() > 0)
			{
				m_isReload = true;
				m_Model.SetTime(0);
			}
			else if (m_SelectedWeapon == EWeapons::eHandgun &&
				g_pHandgun->GetLoadedAmmo() < AMMO_LOADED_HANDGUN &&
				g_pHandgun->GetAmmo() > 0)
			{
				m_isReload = true;
				m_Model.SetTime(0);
			}
		}
	}

	//��
	bool Stealth = false;
	if (Input::KeyH.Pressed())	Stealth = true;

	//���C�@(�}�E�X���N���b�N, �E�V�����_�{�^��)
	if (Input::Mouse.LClicked() || Input::XInputPad1.ShoulderRightClicked())
	{
		if (m_SetupWeapon && !m_isShot && !m_isReload)
		{
			m_Model.StartAnimation();
			m_Volume = VOLUME_ATTACK;
			bool isCanShot = false;
			Vector3D dir = (m_LookPos - Camera::GetEyePosition()).GetNormalize();

			if (m_SelectedWeapon == eShotgun && g_pShotgun->CanShot())
			{
				new Bullet("ShotGun", m_LookPos, dir, 1.0f, 10.0f, 0.01f);
				m_ShotGunFire.Play();
				g_pShotgun->ReduceBullet();
				isCanShot = true;
				m_isShot = true;
				m_Model.SetTime(0);
			}
			else if (m_SelectedWeapon == eHandgun && g_pHandgun->CanShot())
			{
				new Bullet("HandGun", m_LookPos, dir, 1.0f, 100.0f, 0.01f);
				m_HandGunFire.Play();
				g_pHandgun->ReduceBullet();
				isCanShot = true;
				m_isShot = true;
				m_Model.SetTime(0);
			}
			else
			{
				m_GunEmpty.Play();
			}

			//���C�G�t�F�N�g����
			if (isCanShot)
			{
				/*
				EffectInfo effectData;
				effectData.imageName = "GunEffect";
				effectData.num = 15;
				effectData.pos = m_Model.GetBornPos(24) + m_Model.GetAxisZ(0.9f);
				effectData.size = 0.05f;
				effectData.speed = 0.05f;
				effectData.time = 30;
				new EffectParabola(effectData, "GunEffect", dir);
				*/

				EffectAnimationInfo info;
				info.frameNum = 8;
				info.pos = m_Model.GetBornPos(24) + m_Model.GetAxisZ(0.9f);
				info.size = 0.1f;
				info.sizeW = 256;
				info.sizeH = 256;
				info.speed = 40;
				new EffectAnimation("MuzzleFlash", info);
			}
		}
		//�X�e���X�A�^�b�N
		else if (m_isCrouch && Stealth && !m_isSteAttack)
		{
			new Knife(&m_PlayAnim, &m_PlayAnimTime, &m_isSteAttack, &m_MatrixK);
			m_isSteAttack = true;
			m_isCrouch = false;
		}
	}

	//�X�e�[�g�ύX
	if (m_ChangePutBackWeapon)
		m_State = EPlayerState::eState_PutBackWeapon;
	else if (m_ChangeTakeWeapon)
		m_State = EPlayerState::eState_TakeWeapon;

	if (m_isSteAttack)
		m_State = EPlayerState::eState_StealthAttack;

	if (m_isReload)
		m_State = EPlayerState::eState_Reload;

	if (m_isShot)
		m_State = EPlayerState::eState_Recoil;

	if (m_isHit)
		m_State = EPlayerState::eState_Hit;
}

void Player::Animation()
{
	m_SphereMap.radius = MAP_HIT_RADIUS;

	//printf("A%d\n", m_Anim);
	//printf("S%d\n", m_State);

	switch (m_State)
	{
	case EPlayerState::eState_Idle:
		Idle();
		break;
	case EPlayerState::eState_Walk:
		Walk();
		break;
	case EPlayerState::eState_Run:
		Run();
		break;
	case EPlayerState::eState_Crouch:
		Crouch();
		break;
	case EPlayerState::eState_StandUp:
		StandUp();
		break;
	case EPlayerState::eState_TakeWeapon:
		TakeWeapon();
		break;
	case EPlayerState::eState_PutBackWeapon:
		PutBackWeapon();
		break;
	case EPlayerState::eState_SetupWeapon:
		SetupWeapon();
		break;
	case EPlayerState::eState_StealthAttack:
		StealthAttack();
		break;
	case EPlayerState::eState_Recoil:
		Recoil();
		break;
	case EPlayerState::eState_Reload:
		Reload();
		break;
	case EPlayerState::eState_Hit:
		Hit();
		break;
	case EPlayerState::eState_Die:
		Die();
		break;
	default:
		break;
	}

	m_Model.SetPlayTime(m_AnimSpeed * m_OneFlameTime);
	m_Model.ChangeAnimation(m_Anim);

	//printf("A_%d\n", m_Anim);
	//printf("S_%d\n", m_State);
}

void Player::Idle()
{
	//����������đҋ@
	if (m_isTakeWeapon)
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_IdleTakeGun;
			m_AnimSpeed = HALF_ANIM_SPEED;
			break;

		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_IdleTakeHandgun;
			m_AnimSpeed = HALF_ANIM_SPEED;
			break;
		}
	}
	//�ҋ@
	else if (!m_isTakeWeapon)
	{
		m_Anim = EPlayerAnim::eAnim_Idle;
		m_AnimSpeed = HALF_ANIM_SPEED;
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;
	m_Volume = VOLUME_WALK;

	if (m_isTakeWeapon) //����������Ă���Ƃ�
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_WalkTakeGun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
			break;
		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_WalkTakeHandgun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
			break;
		}
	}
	else
	{
		//����
		m_Anim = EPlayerAnim::eAnim_Walk;
		m_AnimSpeed = DEFAULT_ANIM_SPEED;
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;
	m_Volume = VOLUME_RUN;

	//����������Ă���Ƃ�
	if (m_isTakeWeapon)
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_RunTakeGun;
			m_AnimSpeed = RUN_ANIM_SPEED;
			break;
		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_RunTakeHandgun;
			m_AnimSpeed = RUN_ANIM_SPEED;
			break;
		}
	}
	else//����������Ă��Ȃ��Ƃ�
	{
		m_Anim = EPlayerAnim::eAnim_Run;
		m_AnimSpeed = RUN_ANIM_SPEED;
	}
}

void Player::Crouch()
{
	m_Volume = VOLUME_CROUCH;

	//����������Ă��鎞�͕�������܂��Ă��炵�Ⴊ��
	if (m_isTakeWeapon)
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_TakeGun;
			m_AnimSpeed = -ANIM_SPEED_40;
			break;
		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_TakeHandgun;
			m_AnimSpeed = -ANIM_SPEED_40;
			break;
		}
		//���܂��A�j���[�V�����̏I���
		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
		}
	}
	else
	{
		//���Ⴊ�݈ڍs
		if (!m_isCrouch)
		{
			m_Anim = EPlayerAnim::eAnim_Crouch;
			m_AnimSpeed = TWICE_ANIM_SPEED - 25;
		}

		//���Ⴊ�ݑҋ@
		if (!m_isMove && m_isCrouch)
		{
			m_Anim = EPlayerAnim::eAnim_CrouchIdle;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//���Ⴊ�ݕ���
		else if (m_isMove && m_isCrouch)
		{
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Anim = EPlayerAnim::eAnim_CrouchWalk;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
			m_isMove = true;
		}

		if (m_Anim == EPlayerAnim::eAnim_Crouch && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_isShiftCrouch = false;
			m_isCrouch = true;
		}
	}
}

void Player::StandUp()
{
	m_Anim = EPlayerAnim::eAnim_Crouch;
	m_AnimSpeed = -DEFAULT_ANIM_SPEED;

	if (m_Anim == EPlayerAnim::eAnim_Crouch && m_Model.GetPlayTime(m_JudgementAnim) < 1)
	{
		m_isShiftCrouch = false;
		m_isCrouch = false;
	}
}

void Player::TakeWeapon()
{
	switch (m_SelectedWeapon)
	{
	case eShotgun:
		//�V���b�g�K���������ĕ���
		if (m_isTakeWeapon && m_isMove && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Anim = EPlayerAnim::eAnim_WalkTakeGun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
		//�V���b�g�K���������đ���
		else if (m_isTakeWeapon && m_isRun)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Anim = EPlayerAnim::eAnim_RunTakeGun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//�V���b�g�K������������Ԃőҋ@
		else if (m_isTakeWeapon && !m_isMove)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeGun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//�V���b�g�K�������
		else
		{
			m_Anim = EPlayerAnim::eAnim_TakeGun;
			m_AnimSpeed = ANIM_SPEED_40;
		}

		if (m_Anim == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_ChangeTakeWeapon = false;
			m_isTakeWeapon = true;
		}
		break;
	case eHandgun:
		//�n���h�K���������ĕ���
		if (m_isTakeWeapon && m_isMove && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Anim = EPlayerAnim::eAnim_WalkTakeHandgun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
		//�n���h�K���������đ���
		else if (m_isTakeWeapon && m_isRun)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Anim = EPlayerAnim::eAnim_RunTakeHandgun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//�n���h�K������������Ԃőҋ@
		else if (m_isTakeWeapon && !m_isMove)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeHandgun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//�n���h�K�������
		else
		{
			m_Anim = EPlayerAnim::eAnim_TakeHandgun;
			m_AnimSpeed = ANIM_SPEED_40;
		}

		if (m_Anim == eAnim_TakeHandgun && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_ChangeTakeWeapon = false;
			m_isTakeWeapon = true;
		}
		break;
	}
}

void Player::PutBackWeapon()
{
	switch (m_SelectedWeapon)
	{
	case eShotgun:
		m_Anim = EPlayerAnim::eAnim_TakeGun;
		m_AnimSpeed = -DEFAULT_ANIM_SPEED;

		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
			m_ChangePutBackWeapon = false;
		}
		break;
	case eHandgun:
		m_Anim = EPlayerAnim::eAnim_TakeHandgun;
		m_AnimSpeed = -DEFAULT_ANIM_SPEED;

		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
			m_ChangePutBackWeapon = false;
		}
		break;
	}
}

void Player::SetupWeapon()
{
	m_MoveSpeed = SETUPWEAPON_MOVE_SPEED;

	switch (m_SelectedWeapon)
	{
	case eShotgun:
		m_Anim = EPlayerAnim::eAnim_SetupGun;
		m_AnimSpeed = TWICE_ANIM_SPEED;
		break;
	case eHandgun:
		m_Anim = EPlayerAnim::eAnim_SetupHandgun;
		m_AnimSpeed = TWICE_ANIM_SPEED;
		break;
	}
	//�\����ԂŒ�~
	if ((m_Anim == EPlayerAnim::eAnim_SetupGun ||
		m_Anim == EPlayerAnim::eAnim_SetupHandgun) &&
		m_Model.GetPlayTime(m_JudgementAnim) >= 28.0)
	{
		m_SphereMap.radius = MAP_HIT_RADIUS_SETWEAPON;
		m_Model.StopAnimation();
	}
}

void Player::Recoil()
{
	switch (m_SelectedWeapon)
	{
	case eShotgun:
		m_Anim = EPlayerAnim::eAnim_RecoilGun;
		m_AnimSpeed = DEFAULT_ANIM_SPEED;
		break;
	case eHandgun:
		m_Anim = EPlayerAnim::eAnim_RecoilHandgun;
		m_AnimSpeed = DEFAULT_ANIM_SPEED;
		break;
	}
	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 20.0)
	{
		m_isShot = false;
		m_Model.SetTime(28.0);
	}
}

void Player::Reload()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isMove)
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_WalkReloadGun;
			m_AnimSpeed = ANIM_SPEED_10;
			break;
		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_WalkReloadHandgun;
			m_AnimSpeed = ANIM_SPEED_10;
			break;
		}
	}
	else
	{
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			m_Anim = EPlayerAnim::eAnim_ReloadGun;
			m_AnimSpeed = ANIM_SPEED_10;
			break;
		case eHandgun:
			m_Anim = EPlayerAnim::eAnim_ReloadHandgun;
			m_AnimSpeed = ANIM_SPEED_10;
			break;
		}
	}

	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 28)
	{
		m_isReload = false;
		//���e���𑝂₷
		switch (m_SelectedWeapon)
		{
		case eShotgun:
			g_pShotgun->Reload();
			break;
		case eHandgun:
			g_pHandgun->Reload();
			break;
		}
	}
}

void Player::StealthAttack()
{
	m_Anim = EPlayerAnim::eAnim_StealthAttack;
	m_AnimSpeed = DEFAULT_ANIM_SPEED;

	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 58)
	{
		TaskManager::Kill("Knife");
		m_isSteAttack = false;
		m_State = eState_Crouch;
	}
}

void Player::Die()
{
	m_Anim = EPlayerAnim::eAnim_Die;
	m_AnimSpeed = DEFAULT_ANIM_SPEED - 10;

	if (m_Model.GetPlayTime(m_JudgementAnim) > 28)
	{
		TaskManager::Stop("Player");
	}
}

void Player::Hit()
{
	m_Anim = EPlayerAnim::eAnim_Hit;
	m_AnimSpeed = DEFAULT_ANIM_SPEED;

	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 28)
	{
		m_isHit = false;
	}
}

void Player::HitAmmoBox(Result_Sphere& r)
{
	//������e�̎�ނɂ���Ď擾����e�򐔂����܂�
	if (r.targetID & eHITID2)
	{
		if (r.targetName == "AmmoBox_Shotgun_12") g_pShotgun->AddAmmo(12);
	}
	else if (r.targetID & eHITID3)
	{
		if (r.targetName == "AmmoBox_Handgun_12") g_pHandgun->AddAmmo(12);
	}
}

void Player::HitEnemyAttack(Result_Capsule &hitData)
{
	//�����Ԃ��̃G�t�F�N�g
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 30;
	effectData.pos = hitData.end;
	effectData.size = 1.0f;
	effectData.speed = 0.1f;
	effectData.time = 60;
	new EffectParabola(effectData, "Blood", hitData.start.GetNormalize());
	m_Timer.Start();
	m_HitEnemyAttack.Sleep();

	m_HitEnemyAttackSound.Play(); //�G����U����H�������

	//�̗͌����i���j
	m_Hp -= 10;

	//��ʂɌ����Ԃ�
	new ScreenBlood();

	m_isHit = true;

	//HP���[���Ȃ�Q�[���I�[�o�[���o��player������
	if (m_Hp <= 0)
	{
		//���S��ԂɈڍs
		new YouAreDead(10);
		m_State = EPlayerState::eState_Die;
		m_isDead = true;
		m_Model.SetTime(0);
		TaskManager::Stop("TPSCamera");

		//�v���C���[����E�΂ߏ�ɃJ������ݒu���Ē����_�̓v���C���[�̑O���ɐݒu
		Vector3D z = m_Model.GetAxisZ(*m_Model.GetWorldMatrix(), 1.0f);
		Vector3D x = m_Model.GetAxisX(*m_Model.GetWorldMatrix(), 1.0f);
		Vector3D y = m_Model.GetAxisY(*m_Model.GetWorldMatrix(), 1.0f);
		Camera::SetEye(m_pos + (z + x + y) * 3.0f);
		Camera::SetLookat(m_pos + z);
	}
}

void Player::HitMap(Result_Sphere &data)
{

}


void Player::StageChange(Result_Sphere &data)
{
	PData playerData;
	playerData.HP = m_Hp;
	playerData.Shotgun_LoadedAmmo = g_pShotgun->GetLoadedAmmo();
	playerData.Shotgun_Ammo = g_pShotgun->GetAmmo();
	playerData.Handgun_LoadedAmmo = g_pHandgun->GetLoadedAmmo();
	playerData.Handgun_Ammo = g_pHandgun->GetAmmo();

	PlayerData::SetData(playerData);
	
	TaskManager::Kill("Player");
	TaskManager::Kill("UI_AmmoNum");
	TaskManager::Kill("UI_Reticle");
}