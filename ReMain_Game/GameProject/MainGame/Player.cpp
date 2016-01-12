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
#define MAP_HIT_RADIUS 0.3f //�}�b�v�Ƃ̓����蔻��̔��a
#define MAP_HIT_RADIUS_SETWEAPON 0.8f //�}�b�v�Ƃ̓����蔻��̔��a(����\����)

//�A�j���[�V�����X�s�[�h
#define DEFAULT_ANIM_SPEED 30	//�f�t�H���g�X�s�[�h
#define TWICE_ANIM_SPEED 60	//2�{��
#define HALF_ANIM_SPEED 15	//0.5�{��
#define RUN_ANIM_SPEED 50	//����
#define TAKEWEAPON_ANIM_SPEED 40	//������Ƃ�
#define RELOAD_ANIM_SPEED 10	//�����[�h

Vector3D *g_pPlayerPos;

Player::Player() :
	Character(100.0f, "Player", 0), m_CamDir(0.0f, 0.0f, 0.0f),
	m_CameraPos(-50.0f, 2.0f, -12.0f), m_KeyDir(0.0f, 0.0f, 0.0f),
	m_Horizontal(0.134f), m_Vertical(-1.5f),
	m_MoveSpeed(0.0f), m_AnimSpeed(30.0f),
	m_CameraPosY(1.8f), m_CamSpeed(0.000002f),
	m_Radius(0.5f), m_Phase(0),
	m_isCrouch(false), m_isAttack(false),
	m_isTakeWeapon(false), m_isMove(false),
	m_isRun(false), m_SetupWeapon(false),
	m_ToggleCrouch(false), m_isReload(false),
	m_isHit(false),
	m_ChangeTakeWeapon(false),
	m_ChangePutBackWeapon(false),
	m_State(EPlayerState::eState_Idle),
	m_OldState(EPlayerState::eState_Idle),
	m_SelectedWeapon(EWeapons::eShotgun)
{
	m_Model.SetAsset("Player");

	m_Model.SetScale(1.0f, 1.0f, 1.0f);
	m_pos = Vector3D(-48.0f, 0.0f, -11.0f);

	//カメラの当たり判宁E
	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_HitCameraPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);
	//弾薬箱の当たり判宁E
	m_HitAmmoBox.Regist_S_vs_S(&m_pos, &m_Radius, REGIST_FUNC(Player::HitAmmoBox));
	m_HitAmmoBox.SetID(eHITID1, eHITID2 | eHITID3);

	//敵の攻撁E�E当たり判宁E
	m_HitEnemyAttack.Regist_C_vs_C(&m_pos, &m_SightPos, &m_Radius, REGIST_FUNC(Player::HitEnemyAttack));
	m_HitEnemyAttack.SetID(eHITID0, eHITID1);

	g_pShotgun = new Shotgun(&m_PlayAnim, &m_PlayAnimTime, &m_MatrixS);
	g_pHandgun = new Handgun(&m_PlayAnim, &m_PlayAnimTime, &m_MatrixH);

	m_SphereMap.radius = MAP_HIT_RADIUS;

	m_PlayerSightInfo.SetPos(&m_SightPos);

	g_pPlayerPos = &m_pos;

	m_JudgementAnim = 20; //�A�j���[�V�������f�i�{�[���P�ʁj
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

	m_SightPos = m_Model.GetBornPos(6); //頭のボ�Eン位置

	//カメラの当たり判定位置
	m_HitCameraPos = m_pos;
	m_HitCameraPos.y += CAMERA_HIT_HIGHT;

	//当たり判定用 始点終点
	m_Start = m_CameraPos;
	m_End = ((m_LookPos - m_CameraPos) + m_LookPos).GetNormalize();

	//�v���C���[�̃{�[���s��̐؂�ւ�
	if ((m_Anim == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime(m_JudgementAnim) >= 15) ||
		m_Anim == EPlayerAnim::eAnim_SetupGun || m_Anim == EPlayerAnim::eAnim_IdleTakeGun ||
		m_Anim == EPlayerAnim::eAnim_WalkTakeGun || m_Anim == EPlayerAnim::eAnim_RunTakeGun ||
		m_Anim == EPlayerAnim::eAnim_ReloadGun)
	{
		m_MatrixS = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_MatrixS = m_Model.GetBornMatrix(21, true);
	}

	if ((m_Anim == EPlayerAnim::eAnim_TakeHandgun && m_Model.GetPlayTime(m_JudgementAnim) >= 15) ||
		m_Anim == EPlayerAnim::eAnim_SetupHandgun || m_Anim == EPlayerAnim::eAnim_IdleTakeHandgun ||
		m_Anim == EPlayerAnim::eAnim_WalkTakeHandgun || m_Anim == EPlayerAnim::eAnim_RunTakeHandgun ||
		m_Anim == EPlayerAnim::eAnim_ReloadHandgun)
	{
		m_MatrixH = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_MatrixH = m_Model.GetBornMatrix(3, true);
	}

	//弾薬数を表示
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Num.NumDraw(Vector2D(0, 90), g_pShotgun->GetAmmo());
		m_Num.NumDraw(Vector2D(44, 90), g_pShotgun->GetLoadedAmmo());
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Num.NumDraw(Vector2D(0, 90), g_pHandgun->GetAmmo());
		m_Num.NumDraw(Vector2D(44, 90), g_pHandgun->GetLoadedAmmo());
	}

	m_PlayAnim = m_Model.GetPlayAnimation(m_JudgementAnim);
	m_PlayAnimTime = m_Model.GetPlayTime(m_JudgementAnim);
	//printf("%d %d\n", m_ChangePutBackWeapon, m_ChangeTakeWeapon);
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
		//コントローラー入劁E
		m_PadDir = Vector3D((float)Input::XInputPad1.ThumbLeftX(), 0.0f, (float)Input::XInputPad1.ThumbLeftY());
		m_KeyDir = (m_PadDir - Vector3D(128, 128, 128)) / 32767;
	}
	else
	{
		//前後左右移勁E
		if (Input::KeyW.Pressed())	m_KeyDir.z = 1;
		if (Input::KeyS.Pressed())	m_KeyDir.z = -1;
		if (Input::KeyD.Pressed())	m_KeyDir.x = 1;
		if (Input::KeyA.Pressed())	m_KeyDir.x = -1;
	}

	//歩ぁEWASD, 左スチE��チE��)
	if (m_KeyDir.x != 0 || m_KeyDir.z != 0)
	{
		m_State = EPlayerState::eState_Walk;
		m_isMove = true;
	}

	//走めE左シフトキー, 左スチE��チE��押ぁE
	if ((Input::KeyLShift.Pressed() || Input::XInputPad1.ThumbLeftPressed()) && m_isMove)
	{
		m_State = EPlayerState::eState_Run;
		m_isRun = true;
	}

	//しゃが�E(左コントロールキー, 右トリガー) 
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
	if (m_isMove && m_Anim != EPlayerAnim::eAnim_TakeGun && m_Anim != EPlayerAnim::eAnim_TakeHandgun)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_KeyDir.x, m_KeyDir.z), 0.0f);
		m_rot.y = playerRot.y;

		if (m_SetupWeapon)
		{
			m_Phase++;
			//武器を構えた状態�E移勁E
			m_pos += m_Model.GetAxisX(1.0f) * m_KeyDir.x * m_MoveSpeed;
			m_pos += m_Model.GetAxisZ(1.0f) * m_KeyDir.z * m_MoveSpeed;
			//プレイヤーの上下移勁E
			m_pos.y += sinf(m_Phase * (PI * 10) / 150.0f) / 120.0f;
		}
		else
		{
			//通常状態�E移勁E
			Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
			m_pos += pos * m_MoveSpeed;
		}
	}
}

void Player::Weapon()
{
	//武器の刁E��替ぁEホイールクリチE��, 方向キー左右)
	if ((Input::Mouse.WheelClicked() || Input::XInputPad1.ThumbRightClicked()) && !m_isTakeWeapon)
	{
		m_SelectedWeapon = m_SelectWeapon.Select();
	}

	if (m_SelectWeapon.isSelected()) return;

	//武器をとめE
	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && !m_isCrouch)
	{
		if (m_isTakeWeapon && !m_ChangePutBackWeapon && !m_ChangeTakeWeapon)
		{
			m_ChangePutBackWeapon = true;
			m_Model.SetTime(29);
		}
		else if (!m_isTakeWeapon && !m_ChangeTakeWeapon && !m_ChangePutBackWeapon)
		{
			m_ChangeTakeWeapon = true;
			m_Model.SetTime(0);
		}
	}
	//スチE�Eト変更
	if (m_ChangePutBackWeapon)
		m_State = EPlayerState::eState_PutBackWeapon;
	else if (m_ChangeTakeWeapon)
		m_State = EPlayerState::eState_TakeWeapon;


	//銁E��持ってぁE��ときに銁E��構えめEマウス右クリチE��, 左ショルダー)
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

	//リローチE
	if (Input::KeyR.Clicked() && m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun && g_pShotgun->GetLoadedAmmo() < AMMO_LOADED_SHOTGUN)
		{
			m_isReload = true;
			m_Model.SetTime(0);
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun && g_pHandgun->GetLoadedAmmo() < AMMO_LOADED_HANDGUN)
		{
			m_isReload = true;
			m_Model.SetTime(0);
		}
	}

	if (m_isReload)
		m_State = EPlayerState::eState_Reload;

	//発砲　(マウス左クリチE��, 右ショルダボタン)
	if ((Input::Mouse.LClicked() || Input::XInputPad1.ShoulderRightClicked()) && m_SetupWeapon)
	{
		bool isCanShot = false;
		Vector3D dir = (m_LookPos - Camera::GetEyePosition()).GetNormalize();

		if (m_SelectedWeapon == eShotgun && g_pShotgun->CanShot())
		{
			new Bullet("ShotGun", m_LookPos, dir, 1.0f, 10.0f, 0.01f);
			g_pShotgun->ReduceBullet();
			isCanShot = true;
		}
		else if (m_SelectedWeapon == eHandgun && g_pHandgun->CanShot())
		{
			new Bullet("HandGun", m_LookPos, dir, 1.0f, 100.0f, 0.01f);
			g_pHandgun->ReduceBullet();
			isCanShot = true;
		}

		//発砲エフェクト生戁E
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

	if (m_SelectWeapon.isSelected()) return;

	//カメラ移勁E
	if (Input::XInputPad1.GetIsConnection())
	{
		//コントローラー入劁E
		m_Horizontal += (Input::XInputPad1.ThumbRightX() - 128) * m_CamSpeed;
		m_Vertical += (Input::XInputPad1.ThumbRightY() - 128) * m_CamSpeed;
	}
	else
	{
		//マウス入劁E
		m_Horizontal += mouseValue.x * 0.002f;
		m_Vertical += -mouseValue.y * 0.002f;
	}

	Matrix mRX, mRY, mat;
	mRY.RotationRadianY(m_Horizontal);
	mRX.RotationRadianX(m_Vertical);
	mat = mRY * mRX;
	m_CamDir = mat.GetAxisZ();

	//銁E��構えてぁE��とき�Eカメラ位置
	if (m_SetupWeapon)
	{
		m_CamSpeed = 0.0000007f;
		if (m_Vertical >= 0.55f) m_Vertical = 0.55f;	//カメラ角度上限
		if (m_Vertical <= -0.5f) m_Vertical = -0.5f;	//カメラ角度下限

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
	//通常状態�Eカメラ位置
	else
	{
		m_CamSpeed = 0.000002f;
		if (m_Vertical >= 0.45f) m_Vertical = 0.45f;	//カメラ角度下限
		if (m_Vertical <= -0.9f) m_Vertical = -0.9f;	//カメラ角度上限

		m_rot.x = 0.0f;
		newLookPos = m_pos + mat.GetAxisX() * 0.4f;
		newLookPos.y = m_CameraPosY + m_pos.y;

		//カメラの座樁E
		newCameraPos = newLookPos;
		newCameraPos -= m_CamDir * lenge;
	}

	//カメラ補完移勁E
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
	m_Model.SetPlayTime(m_AnimSpeed);
	m_Model.ChangeAnimation(m_Anim);
}

void Player::Idle()
{
	//武器を持って征E��E
	if (m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeGun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeHandgun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
	}
	//征E��E
	else
	{
		m_Anim = EPlayerAnim::eAnim_Idle;
		m_AnimSpeed = HALF_ANIM_SPEED;
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isTakeWeapon) //武器を持ってぁE��とぁE
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Anim = EPlayerAnim::eAnim_WalkTakeGun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Anim = EPlayerAnim::eAnim_WalkTakeHandgun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
	}
	else
	{
		//歩ぁE
		m_Anim = EPlayerAnim::eAnim_Walk;
		m_AnimSpeed = DEFAULT_ANIM_SPEED;
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;

	//武器を持ってぁE��とぁE
	if (m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Anim = EPlayerAnim::eAnim_RunTakeGun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Anim = EPlayerAnim::eAnim_RunTakeHandgun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
	}
	else//武器を持ってぁE��ぁE��ぁE
	{
		m_Anim = EPlayerAnim::eAnim_Run;
		m_AnimSpeed = RUN_ANIM_SPEED;
	}
}

void Player::Crouch()
{
	//武器を持ってぁE��時�E武器をしまってからしゃが�E
	if (m_isTakeWeapon)
	{
		if (m_SelectedWeapon == EWeapons::eShotgun)
		{
			m_Anim = EPlayerAnim::eAnim_TakeGun;
			m_AnimSpeed = -TWICE_ANIM_SPEED;
		}
		else if (m_SelectedWeapon == EWeapons::eHandgun)
		{
			m_Anim = EPlayerAnim::eAnim_TakeHandgun;
			m_AnimSpeed = -TWICE_ANIM_SPEED;
		}

		//���܂��A�j���[�V�����̏I���
		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
		}
	}
	else
	{
		//しゃがみ移衁E
		if (!m_isCrouch)
		{
			m_Anim = EPlayerAnim::eAnim_Crouch;
			m_AnimSpeed = TWICE_ANIM_SPEED;
		}

		//しゃがみ征E��E
		if (m_KeyDir.x == 0 && m_KeyDir.z == 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_Anim = EPlayerAnim::eAnim_CrouchIdle;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//しゃがみ歩ぁE
		else if (m_KeyDir.x != 0 || m_KeyDir.z != 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Anim = EPlayerAnim::eAnim_CrouchWalk;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
			m_isMove = true;
		}

		if (m_Anim == EPlayerAnim::eAnim_Crouch &&m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_isCrouch = true;
		}
	}
}

void Player::TakeWeapon()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		//ショチE��ガンを持って歩ぁE
		if (m_isTakeWeapon && m_isMove && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Anim = EPlayerAnim::eAnim_WalkTakeGun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
		//ショチE��ガンを持って走めE
		else if (m_isTakeWeapon && m_isRun)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Anim = EPlayerAnim::eAnim_RunTakeGun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//ショチE��ガンを持った状態で征E��E
		else if (m_isTakeWeapon && !m_isMove)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeGun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//ショチE��ガンを取めE
		else
		{
			m_Anim = EPlayerAnim::eAnim_TakeGun;
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
		}

		if (m_Anim == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_ChangeTakeWeapon = false;
			m_isTakeWeapon = true;
		}
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		//ハンドガンを持って歩ぁE
		if (m_isTakeWeapon && m_isMove && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Anim = EPlayerAnim::eAnim_WalkTakeHandgun;
			m_AnimSpeed = DEFAULT_ANIM_SPEED;
		}
		//ハンドガンを持って走めE
		else if (m_isTakeWeapon && m_isRun)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Anim = EPlayerAnim::eAnim_RunTakeHandgun;
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//ハンドガンを持った状態で征E��E
		else if (m_isTakeWeapon && !m_isMove)
		{
			m_Anim = EPlayerAnim::eAnim_IdleTakeHandgun;
			m_AnimSpeed = HALF_ANIM_SPEED;
		}
		//ハンドガンを取めE
		else
		{
			m_Anim = EPlayerAnim::eAnim_TakeHandgun;
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
		}

		if (m_Anim == eAnim_TakeHandgun && m_Model.GetPlayTime(m_JudgementAnim) > 28)
		{
			m_ChangeTakeWeapon = false;
			m_isTakeWeapon = true;
		}
	}
}

void Player::PutBackWeapon()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Anim = EPlayerAnim::eAnim_TakeGun;
		m_AnimSpeed = -DEFAULT_ANIM_SPEED;

		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
			m_ChangePutBackWeapon = false;
		}
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Anim = EPlayerAnim::eAnim_TakeHandgun;
		m_AnimSpeed = -DEFAULT_ANIM_SPEED;

		if (m_Model.GetPlayTime(m_JudgementAnim) < 1)
		{
			m_isTakeWeapon = false;
			m_ChangePutBackWeapon = false;
		}
	}
}

void Player::SetupWeapon()
{
	m_MoveSpeed = SETUPWEAPON_MOVE_SPEED;
	m_SetupWeapon = false;

	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Anim = EPlayerAnim::eAnim_SetupGun;
		m_AnimSpeed = TWICE_ANIM_SPEED;
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Anim = EPlayerAnim::eAnim_SetupHandgun;
		m_AnimSpeed = TWICE_ANIM_SPEED;
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

void Player::Reload()
{
	if (m_SelectedWeapon == EWeapons::eShotgun)
	{
		m_Anim = EPlayerAnim::eAnim_ReloadGun;
		m_AnimSpeed = RELOAD_ANIM_SPEED;
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		m_Anim = EPlayerAnim::eAnim_ReloadHandgun;
		m_AnimSpeed = RELOAD_ANIM_SPEED;
	}

	//�A�j���[�V�����I��
	if (m_Model.GetPlayTime(m_JudgementAnim) > 28)
	{
		m_isReload = false;
		m_SelectedWeapon == EWeapons::eShotgun ? g_pShotgun->Reload() : g_pHandgun->Reload();
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
		m_State = m_OldState;
	}
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
	//m_isHit = true;
	//m_OldState = m_State;
	//血し�Eき�EエフェクチE
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = hitData.end;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	new Effect(effectData, "Blood");
}
