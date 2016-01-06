#include "Player.h"
#include "../MainGame/Bullet/Bullet.h"
#include "../GEKO/System/Input.h"
#include "../GameSystem/Effect.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;//���Ⴊ�ݎp������Ȃ��Ƃ��̃J������Y���W�̍���
#define CAMERA_CROUCH_POS_Y 0.95f;	//���Ⴊ�ݎp���̂Ƃ��̃J������Y���W�̍���
#define CAMERA_HIT_HIGHT 1.5f;		//�J�����̓����蔻��̍���
#define WALK_SPEED 0.07f			//�����X�s�[�h
#define RUN_SPEED 0.16f				//����X�s�[�h
#define CROUCH_WALK_SPPED 0.02f		//���Ⴊ�ݕ����X�s�[�h
#define	SETUPWEAPON_MOVE_SPEED 0.03f;//������\�������̈ړ����x
#define CAMERA_LENGE	2.0f		//�v���C���[�ƃJ�����̋���
#define PI 3.14159265359f
#define MAP_HIT_RADIUS 0.2f //�}�b�v�Ƃ̓����蔻��̔��a

//�A�j���[�V�����X�s�[�h
#define IDLE_ANIM_SPEED 15	//�҂�
#define WALK_ANIM_SPEED 30	//����
#define RUN_ANIM_SPEED 50	//����
#define CROUCH_ANIM_SPEED 60	//���Ⴊ��
#define TAKEWEAPON_ANIM_SPEED 40	//������Ƃ�
#define SETUPWEAPON_ANIM_SPEED 60	//������\����
#define RELOAD_ANIM_SPEED 10	//�����[�h

Vector3D *g_pPlayerPos;

Player::Player() :
	Character(100.0f, "Player", 0), m_CamDir(0.0f, 0.0f, 0.0f), m_CameraPos(-50.0f, 2.0f, -12.0f),
	m_KeyDir(0.0f, 0.0f, 0.0f), m_Horizontal(0.134f), m_Phase(0), m_AnimSpeed(30.0f), m_Radius(0.4f),
	m_Vertical(-1.5f), m_MoveSpeed(0.0f), m_CameraPosY(1.8f), m_CamSpeed(0.000002f),
	m_isCrouch(false), m_isAttack(false), m_isTakeWeapon(false), m_isMove(false),
	m_isTakeAnim(false), m_ChangeTakeWeapon(false), m_isRun(false), m_SetupWeapon(false),
	m_ToggleCrouch(false), m_isReload(false),
	m_State(EPlayerState::eState_Idle), m_SelectedWeapon(EWeapons::eShotgun)
{
	m_Model.SetAsset("Player");

	m_Model.SetScale(1.0f, 1.0f, 1.0f);
	m_pos = Vector3D(-48.0f, 0.0f, -11.0f);

	//�J�����̓����蔻��
	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_HitCameraPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);
	//�e�򔠂̓����蔻��
	m_HitAmmoBox.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::HitAmmoBox));
	m_HitAmmoBox.SetID(eHITID1, eHITID2 | eHITID3);

	//�G�̍U���̓����蔻��
	m_HitEnemyAttack.Regist_C_vs_C(&m_pos, &m_SightPos, &m_Radius, REGIST_FUNC(Player::HitEnemyAttack));
	m_HitEnemyAttack.SetID(eHITID0, eHITID1);

	g_pShotgun = new Shotgun(&m_PlayAnim, &m_PlayAnimTime, &m_MatrixS);
	g_pHandgun = new Handgun(&m_PlayAnim, &m_PlayAnimTime, &m_MatrixH);

	m_SphereMap.radius = MAP_HIT_RADIUS;

	m_PlayerSightInfo.SetPos(&m_SightPos);

	g_pPlayerPos = &m_pos;
}

Player::~Player()
{

}

void Player::Update()
{
	Move();
	Weapon();
	Camera();
	Animation();
	Character::Update();
	m_SelectWeapon.Update();

	m_SightPos = m_Model.GetBornPos(6); //���̃{�[���ʒu

	//�J�����̓����蔻��ʒu
	m_HitCameraPos = m_pos;
	m_HitCameraPos.y += CAMERA_HIT_HIGHT;

	//�����蔻��p �n�_�I�_
	m_Start = m_CameraPos;
	m_End = ((m_LookPos - m_CameraPos) + m_LookPos).GetNormalize();

	//�v���C���[�̃{�[���s��̐؂�ւ�
	int ainmState = m_Model.GetPlayAnimation();
	if ((ainmState == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime() >= 15) ||
		ainmState == EPlayerAnim::eAnim_SetupGun ||
		ainmState == EPlayerAnim::eAnim_IdleTakeGun ||
		ainmState == EPlayerAnim::eAnim_WalkTakeGun ||
		ainmState == EPlayerAnim::eAnim_RunTakeGun ||
		ainmState == EPlayerAnim::eAnim_ReloadGun)
	{
		m_MatrixS = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_MatrixS = m_Model.GetBornMatrix(21, true);
	}

	if ((ainmState == EPlayerAnim::eAnim_TakeHandgun && m_Model.GetPlayTime() >= 15) ||
		ainmState == EPlayerAnim::eAnim_SetupHandgun ||
		ainmState == EPlayerAnim::eAnim_IdleTakeHandgun ||
		ainmState == EPlayerAnim::eAnim_WalkTakeHandgun ||
		ainmState == EPlayerAnim::eAnim_RunTakeHandgun ||
		ainmState == EPlayerAnim::eAnim_ReloadHandgun)
	{
		m_MatrixH = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_MatrixH = m_Model.GetBornMatrix(3, true);
	}

	m_PlayAnim = m_Model.GetPlayAnimation();
	m_PlayAnimTime = m_Model.GetPlayTime();
}

void Player::Move()
{
	m_KeyDir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = EPlayerState::eState_Idle;
	m_isMove = false;
	m_isRun = false;

	if (m_SelectWeapon.GetisSelected()) return;

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
	else
	{
		m_isCrouch = false;
	}

	//�v���C���[��������ԂȂ�ړ�����
	if (m_isMove && m_Model.GetPlayAnimation() != EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayAnimation() != EPlayerAnim::eAnim_TakeHandgun)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_KeyDir.x, m_KeyDir.z), 0.0f);
		m_rot.y = playerRot.y;

		if (m_SetupWeapon)
		{
			m_Phase++;
			//������\������Ԃ̈ړ�
			m_pos += m_Model.GetAxisX(1.0f) * m_KeyDir.x * m_MoveSpeed;
			m_pos += m_Model.GetAxisZ(1.0f) * m_KeyDir.z * m_MoveSpeed;
			//�v���C���[�̏㉺�ړ�
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

void Player::Weapon()
{
	//����̐؂�ւ�(�z�C�[���N���b�N, �����L�[���E)
	if ((Input::Mouse.WheelClicked() || Input::XInputPad1.ThumbRightClicked()) && !m_isTakeWeapon)
	{
		m_SelectedWeapon = m_SelectWeapon.Select();
	}

	//�e������(F�L�[, B�{�^��)
	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && !m_isCrouch && !m_isTakeAnim)
	{
		m_ChangeTakeWeapon = !m_ChangeTakeWeapon;
	}

	//�X�e�[�g�̕ύX
	if (m_ChangeTakeWeapon)
		m_State = EPlayerState::eState_TakeWeapon;
	else
		m_isTakeWeapon = false;

	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && m_isTakeWeapon)
	{
		m_State = EPlayerState::eState_PutBackWeapon;
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
	if (Input::KeyR.Clicked() && m_isTakeWeapon)
		m_isReload = true;

	if (m_isReload)
		m_State = EPlayerState::eState_Reload;

	//���C�@(�}�E�X���N���b�N, �E�V�����_�{�^��)
	if ((Input::Mouse.LClicked() || Input::XInputPad1.ShoulderRightClicked()) && m_SetupWeapon)
	{
		bool isCanShot = false;

		if (m_SelectedWeapon == eShotgun && g_pShotgun->CanShot())
		{
			new Bullet(m_LookPos, (m_LookPos - Camera::GetEyePosition()).GetNormalize(), 1.0f, 100.0f, 0.01f);
			g_pShotgun->ReduceBullet();
			isCanShot = true;
		}
		else if (m_SelectedWeapon == eHandgun && g_pHandgun->CanShot())
		{
			new Bullet(m_LookPos, (m_LookPos - Camera::GetEyePosition()).GetNormalize(), 1.0f, 100.0f, 0.01f);
			g_pHandgun->ReduceBullet();
			isCanShot = true;
		}

		//���C�G�t�F�N�g����
		if (isCanShot)
		{
			EffectInfo effectData;
			effectData.imageName = "GunEffect";
			effectData.num = 30;
			effectData.pos = m_Model.GetBornPos(24) + m_Model.GetAxisZ(0.9f);
			effectData.scale = Vector3D(0.05f, 0.05f, 0.1f);
			effectData.speed = 0.05f;
			effectData.time = 60;
			new Effect(effectData, "GunEffect");
		}
	}
}

void Player::Camera()
{
	Vector3D newCameraPos;
	Vector3D newLookPos;
	Point mouseValue = Input::Mouse.GetRelativeValue();
	static float lenge = CAMERA_LENGE;

	if (m_SelectWeapon.GetisSelected()) return;

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
		newLookPos = m_pos + mat.GetAxisX() * 0.4f;
		newLookPos.y = m_CameraPosY;

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
	m_CameraPosY = CAMERA_NO_CROUCH_POS_Y;
	m_Model.SetPlayTime(m_AnimSpeed);
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
	case EPlayerState::eState_TakeWeapon:
		TakeWeapon();
		break;
	case EPlayerState::eState_PutBackWeapon:
		PutBackWeapon();
		break;
	case EPlayerState::eState_SetupWeapon:
		SetupWeapon();
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
}

Matrix Player::GetBomeMat(int bornIndex)
{
	return m_Model.GetBornMatrix(bornIndex, true);
}

int Player::GetAnim()
{
	return m_Model.GetPlayAnimation();
}

float Player::GetPlayTime()
{
	return m_Model.GetPlayTime();
}

void Player::Idle()
{
	//����������đҋ@
	if (m_isTakeWeapon)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
		m_AnimSpeed = IDLE_ANIM_SPEED;
	}
	//�ҋ@
	else
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Idle);
		m_AnimSpeed = IDLE_ANIM_SPEED;
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isTakeWeapon) //����������Ă���Ƃ�
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeGun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeHandgun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
	}
	else
	{
		//����
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Walk);
		m_AnimSpeed = WALK_ANIM_SPEED;
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;

	//����������Ă���Ƃ�
	if (m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeGun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeHandgun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
	}
	else//����������Ă��Ȃ��Ƃ�
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Run);
		m_AnimSpeed = RUN_ANIM_SPEED;
	}
}

void Player::Crouch()
{
	//����������Ă��鎞�͕�������܂��Ă��炵�Ⴊ��
	if (m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
			m_Model.SetTime(-29);
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeHandgun);
			m_Model.SetTime(-29);
		}
		//���܂��A�j���[�V�����̏I���
		if (m_Model.GetPlayTime() == 0)
		{
			m_isTakeWeapon = false;
		}
	}
	else
	{
		//���Ⴊ�݈ڍs
		if (!m_isCrouch)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_Crouch);
			m_AnimSpeed = CROUCH_ANIM_SPEED;
		}

		//���Ⴊ�ݑҋ@
		if (m_KeyDir.x == 0 && m_KeyDir.z == 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_CrouchIdle);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//���Ⴊ�ݕ���
		else if (m_KeyDir.x != 0 || m_KeyDir.z != 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_CrouchWalk);
			m_AnimSpeed = WALK_ANIM_SPEED;
			m_isMove = true;
		}

		if (m_Model.GetPlayTime() >= 29 && m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_Crouch)
		{
			m_isCrouch = true;
		}
	}
}

void Player::TakeWeapon()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		//�V���b�g�K���������ĕ���
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeGun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
		//�V���b�g�K���������đ���
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeGun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//�V���b�g�K������������Ԃőҋ@
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//�V���b�g�K�������
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
			m_isTakeAnim = true;
		}

		//�e�����A�j���[�V�����̏I����
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
			m_isTakeAnim = false;
		}
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		//�n���h�K���������ĕ���
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeHandgun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
		//�n���h�K���������đ���
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeHandgun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//�n���h�K������������Ԃőҋ@
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeHandgun);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//�n���h�K�������
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeHandgun);
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
			m_isTakeAnim = true;
		}

		//�e�����A�j���[�V�����̏I����
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeHandgun &&	m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
			m_isTakeAnim = false;
		}
	}
}

void Player::PutBackWeapon()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
		m_Model.SetTime(-29);

		if (m_Model.GetPlayTime() == 0)
		{
			m_isTakeWeapon = false;
			m_ChangeTakeWeapon = false;
		}
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeHandgun);
		m_Model.SetTime(-29);

		if (m_Model.GetPlayTime() == 0)
		{
			m_isTakeWeapon = false;
			m_ChangeTakeWeapon = false;
		}
	}
}

void Player::SetupWeapon()
{
	m_MoveSpeed = SETUPWEAPON_MOVE_SPEED;
	m_SetupWeapon = false;

	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_SetupGun);
		m_AnimSpeed = SETUPWEAPON_ANIM_SPEED;
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_SetupHandgun);
		m_AnimSpeed = SETUPWEAPON_ANIM_SPEED;
	}
	//�\����ԂŒ�~
	if ((m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupGun || m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupHandgun) &&
		m_Model.GetPlayTime() >= 29)
	{
		m_SetupWeapon = true;
		m_Model.SetTime(29);
	}
}

void Player::Reload()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_ReloadGun);
		m_AnimSpeed = RELOAD_ANIM_SPEED;
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_ReloadHandgun);
		m_AnimSpeed = RELOAD_ANIM_SPEED;
	}

	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime() >= 29)
	{
		m_isReload = false;
		m_SelectedWeapon == EWeapons::eShotgun ? g_pShotgun->Reload() : g_pHandgun->Reload();
	}
}

void Player::Die()
{
	m_Model.ChangeAnimation(EPlayerAnim::eAnim_Die);
}

void Player::Hit()
{

}

void Player::HitCamera(Result_Porygon &hitData)
{
	Vector3D vec = m_LookPos - m_CameraPos;
	float dist = (hitData.contactPos - m_CameraPos).Length();
	m_CameraPos += vec.GetNormalize() * dist;
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
	//�����Ԃ��̃G�t�F�N�g
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = hitData.end;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	new Effect(effectData, "Blood");
}