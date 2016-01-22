#include "Player.h"
#include "../MainGame/Bullet/Bullet.h"
#include "../GEKO/System/Input.h"
#include "../GameSystem/Effect/EffectSphere.h"
#include "../GameSystem/Effect/EffectParabola.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;//���Ⴊ�ݎp������Ȃ��Ƃ��̃J������Y���W�̍���
#define CAMERA_HIT_HIGHT 1.5f;		//�J�����̓����蔻��̍���
#define WALK_SPEED 0.05f			//�����X�s�[�h
#define RUN_SPEED 0.12f				//����X�s�[�h
#define CROUCH_WALK_SPPED 0.02f		//���Ⴊ�ݕ����X�s�[�h
#define	SETUPWEAPON_MOVE_SPEED 0.03f;//������\�������̈ړ����x
#define CAMERA_LENGE	2.0f		//�v���C���[�ƃJ�����̋���
#define PI 3.14159265359f
#define MAP_HIT_RADIUS 0.3f //�}�b�v�Ƃ̓����蔻��̔��a
#define MAP_HIT_RADIUS_SETWEAPON 0.8f //�}�b�v�Ƃ̓����蔻��̔��a(����\����)

//�A�j���[�V�����X�s�[�h
#define DEFAULT_ANIM_SPEED 30	//�f�t�H���g�X�s�[�h
#define TWICE_ANIM_SPEED 60	//2�{��
#define HALF_ANIM_SPEED 15	//0.5�{��
#define RUN_ANIM_SPEED 50	//����
#define ANIM_SPEED_40 40	//������Ƃ�
#define ANIM_SPEED_10 10	//�����[�h

#define VOLUME_CROUCH 1
#define VOLUME_WALK 2
#define VOLUME_RUN 3

Vector3D *g_pPlayerPos;

Player::Player(Vector3D pos) :
	Character(100.0f, "Player", 0), m_CamDir(0.0f, 0.0f, 0.0f),
	m_CameraPos(-50.0f, 2.0f, -12.0f), m_KeyDir(0.0f, 0.0f, 0.0f),
	m_Horizontal(0.134f), m_Vertical(-1.5f),
	m_MoveSpeed(0.0f), m_AnimSpeed(30.0f),
	m_CamSpeed(0.000002f),
	m_Radius(0.5f), m_Phase(0),
	m_isCrouch(false), m_isAttack(false),
	m_isTakeWeapon(false), m_isMove(false),
	m_isRun(false), m_SetupWeapon(false),
	m_ToggleCrouch(false), m_isReload(false),
	m_isHit(false), m_isShot(false),
	m_ChangeTakeWeapon(false), m_isSteAttack(false),
	m_ChangePutBackWeapon(false),
	m_State(EPlayerState::eState_Idle),
	m_SelectedWeapon(EWeapons::eShotgun)
{
	m_Model.SetAsset("Player");

	m_Model.SetScale(1.0f, 1.0f, 1.0f);
	m_pos = pos;
	Camera::SetEye(-50.0f, 2.0f, -12.0f);
	//�J�����̓����蔻��
	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_HitCameraPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);
	//�e�򔠂̓����蔻��
	m_HitAmmoBox.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::HitAmmoBox));
	m_HitAmmoBox.SetID(eHITID1, eHITID2 | eHITID3);

	//�G�̍U���̓����蔻��
	m_HitEnemyAttack.Regist_C_vs_C(&m_pos, &m_SightPos, &m_Radius, REGIST_FUNC(Player::HitEnemyAttack));
	m_HitEnemyAttack.SetID(eHITID0, eHITID1);

	g_pShotgun = new Shotgun(&m_PlayAnim, &m_PlayAnimTime, &m_isTakeWeapon, &m_SelectedWeapon, &m_MatrixS);
	g_pHandgun = new Handgun(&m_PlayAnim, &m_PlayAnimTime, &m_isTakeWeapon, &m_SelectedWeapon, &m_MatrixH);


	m_SphereMap.radius = MAP_HIT_RADIUS;

	m_PlayerSightInfo.SetPos(&m_SightPos);

	g_pPlayerPos = &m_pos;

	m_JudgementAnim = 20; //�A�j���[�V�������f�i�{�[���P�ʁj

	m_Volume = VOLUME_WALK;
	m_AuditoryObject.SetPos(&m_pos);
	m_AuditoryObject.SetVolume(&m_Volume);

}

Player::~Player()
{

}

void Player::Update()
{
	Move();
	Attack();
	Camera();
	Animation();
	Character::Update();
	m_SelectWeapon.Update();

	if (m_Timer.GetSecond() >= 1.5)	m_HitEnemyAttack.Awake();

	m_SightPos = m_Model.GetBornPos(6); //���̃{�[���ʒu
	m_MatrixK = m_Model.GetBornMatrix(25, true);	//�w�̃{�[���ʒu

	//�J�����̓����蔻��ʒu
	m_HitCameraPos = m_pos;
	m_HitCameraPos.y += CAMERA_HIT_HIGHT;

	//printf("%d %d\n", m_ToggleCrouch, m_isCrouch);
	//printf("%d\n", m_isSteAttack);

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
}

void Player::Move()
{
	m_KeyDir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = EPlayerState::eState_Idle;
	m_isMove = false;
	m_isRun = false;

	if (m_SelectWeapon.isSelected()) return;

	if (Input::XInputPad1.GetIsConnection())
	{
		//�R���g���[���[����
		m_PadDir = Vector3D((float)Input::XInputPad1.ThumbLeftX(), 0.0f, (float)Input::XInputPad1.ThumbLeftY());
		m_KeyDir = (m_PadDir - Vector3D(128, 128, 128)) / 32767;
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
	if ((Input::KeyLControl.Clicked() || Input::XInputPad1.TriggerRight()))
	{
		m_ToggleCrouch = !m_ToggleCrouch;
	}

	if (m_ToggleCrouch)
	{
		m_State = EPlayerState::eState_Crouch;
	}
	else if (m_isCrouch && !m_ToggleCrouch)
	{
		m_State = EPlayerState::eState_StandUp;
	}

	//�v���C���[��������ԂȂ�ړ�����
	if (m_isMove && m_Anim != EPlayerAnim::eAnim_TakeGun && m_Anim != EPlayerAnim::eAnim_TakeHandgun)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_KeyDir.x, m_KeyDir.z), 0.0f);
		m_rot.y = playerRot.y;

		if (m_SetupWeapon)
		{
			//������\������Ԃ̈ړ�
			m_pos += m_Model.GetAxisX(1.0f) * m_KeyDir.x * m_MoveSpeed;
			m_pos += m_Model.GetAxisZ(1.0f) * m_KeyDir.z * m_MoveSpeed;
			//�v���C���[�̏㉺�ړ�
			m_Phase++;
			m_pos.y += sinf(m_Phase * (PI * 10) / 150.0f) / 120.0f;
		}
		else
		{
			//�ʏ��Ԃ̈ړ�
			Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
			m_pos += pos * m_MoveSpeed;
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
	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && !m_isCrouch)
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
	if ((Input::Mouse.RPressed() || Input::XInputPad1.ShoulderLeftPressed()) && m_isTakeWeapon)
	{
		m_AnimSpeed = 60;
		m_State = EPlayerState::eState_SetupWeapon;
		m_SetupWeapon = true;
	}
	else
	{
		m_SetupWeapon = false;
	}

	//�����[�h
	if (Input::KeyR.Clicked() && m_isTakeWeapon && !m_isReload &&!m_isShot)
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

	bool Stealth = false;
	if (Input::KeyH.Pressed())	Stealth = true;

	if (Input::Mouse.LClicked() || Input::XInputPad1.ShoulderRightClicked())
	{
		//���C�@(�}�E�X���N���b�N, �E�V�����_�{�^��)
		if (m_SetupWeapon && !m_isShot && !m_isReload)
		{
			bool isCanShot = false;
			Vector3D dir = (m_LookPos - Camera::GetEyePosition()).GetNormalize();

			if (m_SelectedWeapon == eShotgun && g_pShotgun->CanShot())
			{
				new Bullet("ShotGun", m_LookPos, dir, 1.0f, 10.0f, 0.01f);
				g_pShotgun->ReduceBullet();
				isCanShot = true;
				m_Model.SetTime(0);
				m_isShot = true;
			}
			else if (m_SelectedWeapon == eHandgun && g_pHandgun->CanShot())
			{
				new Bullet("HandGun", m_LookPos, dir, 1.0f, 100.0f, 0.01f);
				g_pHandgun->ReduceBullet();
				isCanShot = true;
				m_Model.SetTime(0);
				m_isShot = true;
			}
			//���C�G�t�F�N�g����
			if (isCanShot)
			{
				EffectInfo effectData;
				effectData.imageName = "GunEffect";
				effectData.num = 15;
				effectData.pos = m_Model.GetBornPos(24) + m_Model.GetAxisZ(0.9f);
				effectData.scale = Vector3D(0.05f, 0.05f, 0.1f);
				effectData.speed = 0.05f;
				effectData.time = 30;
				new EffectParabola(effectData, "GunEffect", dir);
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

void Player::Camera()
{
	Vector3D newCameraPos;
	Vector3D newLookPos;
	Point mouseValue = Input::Mouse.GetRelativeValue();
	static float lenge = CAMERA_LENGE;

	if (m_SelectWeapon.isSelected()) return;

	//�J�����ړ�
	if (Input::XInputPad1.GetIsConnection())
	{
		//�R���g���[���[����
		m_Horizontal += (Input::XInputPad1.ThumbRightX() - 128) * m_CamSpeed;
		m_Vertical += (Input::XInputPad1.ThumbRightY() - 128) * m_CamSpeed;
	}
	else
	{
		//�}�E�X����
		m_Horizontal += mouseValue.x * 0.002f;
		m_Vertical += -mouseValue.y * 0.002f;
	}

	Matrix mRX, mRY, mat;
	mRY.RotationRadianY(m_Horizontal);
	mRX.RotationRadianX(m_Vertical);
	mat = mRY * mRX;
	m_CamDir = mat.GetAxisZ();

	//�e���\���Ă���Ƃ��̃J�����ʒu
	if (m_SetupWeapon)
	{
		m_CamSpeed = 0.0000007f;
		if (m_Vertical >= 0.55f) m_Vertical = 0.55f;	//�J�����p�x���
		if (m_Vertical <= -0.5f) m_Vertical = -0.5f;	//�J�����p�x����

		m_rot.x = -m_Vertical;
		m_rot.y = m_Horizontal;

		m_Model.SetTranselate(m_pos);
		m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);

		Matrix mat = m_Model.GetBornMatrix(6, true);
		Vector4D eye;
		eye = Vector4D(-0.25f, 0.15f, -0.1f, 1.0f) * mat;
		Vector4D at;
		at = Vector4D(-0.25f, 0.15f, 0.5f, 1.0f) *  mat;
		newCameraPos = Vector3D(eye.x, eye.y, eye.z);
		newLookPos = Vector3D(at.x, at.y, at.z);
	}
	//�ʏ��Ԃ̃J�����ʒu
	else
	{
		m_CamSpeed = 0.000002f;
		if (m_Vertical >= 0.45f) m_Vertical = 0.45f;	//�J�����p�x����
		if (m_Vertical <= -0.9f) m_Vertical = -0.9f;	//�J�����p�x���

		m_rot.x = 0.0f;
		newLookPos = m_Model.GetBornPos(6) + mat.GetAxisX() * 0.4f;

		//�J�����̍��W
		newCameraPos = newLookPos;
		newCameraPos -= m_CamDir * lenge;
	}

	//�J�����⊮�ړ�
	if (m_SetupWeapon)
	{
		m_CameraPos = Vector3D::Lerp(m_CameraPos, newCameraPos, 0.8f);
		m_LookPos = Vector3D::Lerp(m_LookPos, newLookPos, 0.8f);
	}
	else
	{
		m_CameraPos = Vector3D::Lerp(m_CameraPos, newCameraPos, 0.3f);
		m_LookPos = Vector3D::Lerp(m_LookPos, newLookPos, 0.3f);
	}

	Camera::SetEye(m_CameraPos);
	Camera::SetLookat(m_LookPos);
}

void Player::Animation()
{
	m_SphereMap.radius = MAP_HIT_RADIUS;

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

	printf("%d\n", m_Anim);
	m_Model.SetPlayTime(m_AnimSpeed);
	m_Model.ChangeAnimation(m_Anim);
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
	else
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
			m_AnimSpeed = TWICE_ANIM_SPEED;
		}

		//���Ⴊ�ݑҋ@
		if (m_KeyDir.x == 0 && m_KeyDir.z == 0 && m_isCrouch)
		{
			m_Anim = EPlayerAnim::eAnim_CrouchIdle;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//���Ⴊ�ݕ���
		else if (m_KeyDir.x != 0 || m_KeyDir.z != 0 && m_isCrouch)
		{
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Anim = EPlayerAnim::eAnim_CrouchWalk;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
			m_isMove = true;
		}

		if (m_Anim == EPlayerAnim::eAnim_Crouch && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
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
	m_SetupWeapon = false;

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
		m_Model.GetPlayTime(m_JudgementAnim) > 28)
	{
		m_SphereMap.radius = MAP_HIT_RADIUS_SETWEAPON;
		m_SetupWeapon = true;
		m_Model.SetTime(28);
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
		m_AnimSpeed = TWICE_ANIM_SPEED;
		break;
	}
	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 20)
	{
		m_isShot = false;
		m_Model.SetTime(28);
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
	m_AnimSpeed = DEFAULT_ANIM_SPEED;
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

void Player::HitCamera(Result_Porygon &hitData)
{
	Camera::SetEye(hitData.contactPos * hitData.meshMatrix);
	Camera::SetLookat(Vector3D::Lerp(m_Model.GetBornPos(6), m_LookPos, 0.6f));
}

void Player::HitAmmoBox(Result_Sphere& r)
{
	if (r.targetID & eHITID2)
	{
		if (Input::KeyQ.Clicked() || Input::XInputPad1.AClicked())
			g_pShotgun->AddAmmo(12);
	}
	else if (r.targetID & eHITID3)
	{
		if (Input::KeyQ.Clicked() || Input::XInputPad1.AClicked())
			g_pHandgun->AddAmmo(12);
	}
}

void Player::HitEnemyAttack(Result_Capsule &hitData)
{
	m_isHit = true;
	//�����Ԃ��̃G�t�F�N�g
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 30;
	effectData.pos = hitData.end;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 60;
	new EffectParabola(effectData, "Blood", hitData.start.GetNormalize());
	m_Timer.Start();
	m_HitEnemyAttack.Sleep();
}