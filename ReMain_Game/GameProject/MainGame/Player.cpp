#include "Player.h"
#include "../GEKO/System/Input.h"
#include "CBullet.h"
#include "..\GameSystem\Effect.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;//しゃがみ姿勢じゃないときのカメラのY座標の高さ
#define CAMERA_CROUCH_POS_Y 0.95f;	//しゃがみ姿勢のときのカメラのY座標の高さ
#define WALK_SPEED 0.07f			//歩くスピード
#define RUN_SPEED 0.16f				//走るスピード
#define CROUCH_WALK_SPPED 0.02f		//しゃがみ歩きスピード
#define	SETUPWEAPON_MOVE_SPEED 0.03;//武器を構えた時の移動速度
#define CAMERA_LENGE	2.0f		//プレイヤーとカメラの距離
#define PI 3.14159265359f

//アニメーションスピード
#define IDLE_ANIM_SPEED 15	//待ち
#define WALK_ANIM_SPEED 30	//歩き
#define RUN_ANIM_SPEED 50	//走り
#define CROUCH_ANIM_SPEED 60	//しゃがみ
#define TAKEWEAPON_ANIM_SPEED 40	//武器をとる
#define SETUPWEAPON_ANIM_SPEED 60	//武器を構える

Player::Player() :
	CCharacter(ePlayer), m_CamDir(0.0f, 0.0f, 0.0f), m_CameraPos(-50.0f, 2.0f, -12.0f),
	m_KeyDir(0.0f, 0.0f, 0.0f), m_Horizontal(0.134f), m_Phase(0.0f), m_AnimSpeed(30),
	m_Vertical(-1.5f), m_MoveSpeed(0.0f), m_CameraPosY(1.8f), m_CamSpeed(0.000002f),
	m_isCrouch(false), m_isAttack(false), m_isTakeWeapon(false), m_isMove(false),
	m_ChangeTakeWeapon(false), m_isRun(false), m_SetupWeapon(false), m_ToggleCrouch(false),
	m_State(EPlayerState::eState_Idle), m_SelectedWeapon(EWeapons::eShotgun)
{
	m_ColliderMap.SetID(eHITID1, eHITID0 | eHITID1 | eHITID2);
	for (unsigned int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
		//eHITID0…マップ
		//eHITID1…プレイヤー
		//eHITID2…敵
		//eHITID3…弾
		m_pCollider[i].SetID(eHITID1, 0);
	}
	m_Model.SetScale(1.0f, 1.0f, 1.0f);
	m_pos = Vector3D(-48.0f, 0.0f, -11.0f);

	//レティクル(仮)
	m_Reticle.SetAsset("Reticle");
	m_Reticle.SetCenter(32, 32);

	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_LookPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);

	m_pShotgun = new Shotgun();
	m_pHandgun = new Handgun();
	m_SelectWeapon = new SelectWeapon();

	m_PlayerSightInfo.SetPos(&m_pos);
}

Player::~Player()
{
	delete m_pShotgun;
	delete m_pHandgun;
	delete m_SelectWeapon;

}

void Player::Update()
{
	Move();
	Attack();
	Camera();
	Animation();
	CCharacter::Update();
	m_SelectWeapon->Update();

	//当たり判定用 始点終点
	m_Start = m_CameraPos;
	m_End = ((m_LookPos - m_CameraPos) + m_LookPos).GetNormalize();

	//プレイヤーのボーン行列の切り替え
	int ainmState = m_Model.GetPlayAnimation();
	if ((ainmState == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime() >= 15) ||
		ainmState == EPlayerAnim::eAnim_SetupGun ||
		ainmState == EPlayerAnim::eAnim_IdleTakeGun ||
		ainmState == EPlayerAnim::eAnim_WalkTakeGun ||
		ainmState == EPlayerAnim::eAnim_RunTakeGun)
	{
		m_Matrix = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_Matrix = m_Model.GetBornMatrix(21, true);
	}
	m_pShotgun->SetPlayerBomeMtx(&m_Matrix);
	m_pShotgun->SetPlayerData(m_Model.GetPlayAnimation(), m_Model.GetPlayTime(), m_Start, m_End);
	m_pShotgun->Update();
	m_pShotgun->Render();

	if ((ainmState == EPlayerAnim::eAnim_TakeHandgun && m_Model.GetPlayTime() >= 15) ||
		ainmState == EPlayerAnim::eAnim_SetupHandgun ||
		ainmState == EPlayerAnim::eAnim_IdleTakeHandgun ||
		ainmState == EPlayerAnim::eAnim_WalkTakeHandgun ||
		ainmState == EPlayerAnim::eAnim_RunTakeHandgun)
	{
		m_Matrix = m_Model.GetBornMatrix(24, true);
	}
	else
	{
		m_Matrix = m_Model.GetBornMatrix(3, true);
	}
	m_pHandgun->SetPlayerBomeMtx(&m_Matrix);
	m_pHandgun->SetPlayerData(m_Model.GetPlayAnimation(), m_Model.GetPlayTime(), m_Start, m_End);
	m_pHandgun->Update();
	m_pHandgun->Render();

	//レティクル(仮)
	m_Reticle.SetAlpha(150.0f);
	m_Reticle.Draw(400, 300);

//	printf("%d\n", m_SetupWeapon);
//	printf("%d %d %d\n", m_ChangeTakeWeapon, m_isTakeWeapon, m_isCrouch);
}

void Player::Move()
{
	m_KeyDir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = EPlayerState::eState_Idle;
	m_isMove = false;
	m_isRun = false;

	if (Input::XInputPad1.GetIsConnection())
	{
		//コントローラー入力
		m_PadDir = Vector3D(Input::XInputPad1.ThumbLeftX(), 0.0f, Input::XInputPad1.ThumbLeftY());
		m_KeyDir = (m_PadDir - Vector3D(128, 128, 128)) / 32767;
	}
	else
	{
		//前後左右移動
		if (Input::KeyW.Pressed())	m_KeyDir.z = 1;
		if (Input::KeyS.Pressed())	m_KeyDir.z = -1;
		if (Input::KeyD.Pressed())	m_KeyDir.x = 1;
		if (Input::KeyA.Pressed())	m_KeyDir.x = -1;
	}

	//歩く(WASD, 左スティック)
	if (m_KeyDir.x != 0 || m_KeyDir.z != 0)
	{
		m_State = EPlayerState::eState_Walk;
		m_isMove = true;
	}

	//走る(左シフトキー, 左スティック押し)
	if ((Input::KeyLShift.Pressed() || Input::XInputPad1.ThumbLeftPressed()) && m_isMove)
	{
		m_State = EPlayerState::eState_Run;
		m_isRun = true;
	}

	//しゃがむ(左コントロールキー, 右トリガー) 
	if (Input::KeyLControl.Clicked() || Input::XInputPad1.TriggerRight())
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

	//プレイヤーが歩き状態なら移動処理
	if (m_isMove && m_Model.GetPlayAnimation() != EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayAnimation() != EPlayerAnim::eAnim_TakeHandgun)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_KeyDir.x, m_KeyDir.z), 0.0f);
		m_rot.y = playerRot.y;

		if (m_SetupWeapon)
		{
			m_Phase++;
			//武器を構えた状態の移動
			m_pos += m_Model.GetAxisX(1.0f) * m_KeyDir.x * m_MoveSpeed;
			m_pos += m_Model.GetAxisZ(1.0f) * m_KeyDir.z * m_MoveSpeed;
			//プレイヤーの上下移動
			m_pos.y += sinf(m_Phase * (PI * 10) / 150.0f) / 120.0f;
		}
		else
		{
			//通常状態の移動
			Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
			m_pos += pos * m_MoveSpeed;
		}
	}
}

void Player::Attack()
{
	//銃をもつ(Fキー, Bボタン)
	if ((Input::KeyF.Clicked() || Input::XInputPad1.BClicked()) && !m_isCrouch)
	{
		m_ChangeTakeWeapon = !m_ChangeTakeWeapon;
	}

	//ステートの変更
	if (m_ChangeTakeWeapon)
	{
		m_State = EPlayerState::eState_TakeWeapon;
	}
	else
	{
		m_isTakeWeapon = false;
	}

	//武器の切り替え(Rキー, 方向キー左右)
	if ((Input::KeyR.Clicked() || Input::XInputPad1.ThumbRightClicked()) && !m_isTakeWeapon)
	{
		m_SelectWeapon->Select();
	}

	//銃を持っているときに銃を構える(マウス右クリック, 左ショルダー)
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

	//発砲　(マウス左クリック, 右ショルダボタン)
	if ((Input::Mouse.LClicked() || Input::XInputPad1.ShoulderRightClicked()) && m_SetupWeapon)
	{
		EffectInfo effectData;
		effectData.imageName = "GunEffect";
		effectData.num = 30;
		effectData.pos = m_Model.GetBornPos(24) + m_Model.GetAxisZ(0.9f);
		effectData.scale = Vector3D(0.05f, 0.05f, 0.1f);
		effectData.speed = 0.05f;
		effectData.time = 60;
		EffectGeneration::Add(effectData);
		CBulletManager::GetInstance()->Add(m_LookPos, (m_LookPos - Camera::GetEyePosition()).GetNormalize(), 1.0f);
	}
}

void Player::Camera()
{
	Vector3D newCameraPos;
	Vector3D newLookPos;
	Point mouseValue = Input::Mouse.GetRelativeValue();
	Vector3D look(0.6f, 1.8f, 0.0f);
	static float lenge = CAMERA_LENGE;

	//カメラ移動
	if (Input::XInputPad1.GetIsConnection())
	{
		//コントローラー入力
		m_Horizontal += (Input::XInputPad1.ThumbRightX() - 128) * m_CamSpeed;
		m_Vertical += (Input::XInputPad1.ThumbRightY() - 128) * m_CamSpeed;
	}
	else
	{
		//マウス入力
		m_Horizontal += mouseValue.x * 0.002f;
		m_Vertical += -mouseValue.y * 0.002f;
	}

	Matrix mRX, mRY, mat;
	mRY.RotationRadianY(m_Horizontal);
	mRX.RotationRadianX(m_Vertical);
	mat = mRY * mRX;
	m_CamDir = mat.GetAxisZ();

	//銃を構えているときのカメラ位置
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
		eye = Vector4D(-0.25f, 0.15f, -0.3f, 1.0f) * mat;
		Vector4D at;
		at = Vector4D(-0.25f, 0.15f, 0.5f, 1.0f) *  mat;
		newCameraPos = Vector3D(eye.x, eye.y, eye.z);
		newLookPos = Vector3D(at.x, at.y, at.z);
	}
	//通常状態のカメラ位置
	else
	{
		m_CamSpeed = 0.000002f;
		if (m_Vertical >= 0.45f) m_Vertical = 0.45f;	//カメラ角度下限
		if (m_Vertical <= -0.9f) m_Vertical = -0.9f;	//カメラ角度上限

		m_rot.x = 0.0f;
		newLookPos = m_pos + mat.GetAxisX() * 0.4f;
		newLookPos.y = m_CameraPosY;

		//カメラの座標
		newCameraPos = newLookPos;
		newCameraPos -= m_CamDir * lenge;
	}

	//カメラ補完移動
	if (m_SetupWeapon)
	{
		m_CameraPos =Vector3D::Lerp(m_CameraPos, newCameraPos, 0.8f);
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
//	printf("%d\n", m_AnimSpeed);
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
	//武器を持って待機
	if (m_isTakeWeapon)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
		m_AnimSpeed = IDLE_ANIM_SPEED;
	}
	//待機
	else
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Idle);
		m_AnimSpeed = IDLE_ANIM_SPEED;
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isTakeWeapon) //武器を持っているとき
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
		//歩き
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Walk);
		m_AnimSpeed = WALK_ANIM_SPEED;
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;

	//武器を持っているとき
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
	else//武器を持っていないとき
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Run);
		m_AnimSpeed = RUN_ANIM_SPEED;
	}
}

void Player::Crouch()
{
	//武器を持っている時は武器をしまってからしゃがむ
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
		//しまうアニメーションの終わり
		if (m_Model.GetPlayTime() == 0)
		{
			m_isTakeWeapon = false;
		}
	}
	else
	{
		//しゃがみ移行
		if (!m_isCrouch)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_Crouch);
			m_AnimSpeed = CROUCH_ANIM_SPEED;

		}

		//しゃがみ待機
		if (m_KeyDir.x == 0 && m_KeyDir.z == 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_CrouchIdle);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//しゃがみ歩き
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
		//ショットガンを持って歩く
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeGun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
		//ショットガンを持って走る
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeGun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//ショットガンを持った状態で待機
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//ショットガンを取る
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
		}

		//銃を取るアニメーションの終了時
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
		}
	}
	else if (m_SelectedWeapon == EWeapons::eHandgun)
	{
		//ハンドガンを持って歩く
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeHandgun);
			m_AnimSpeed = WALK_ANIM_SPEED;
		}
		//ハンドガンを持って走る
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeHandgun);
			m_AnimSpeed = RUN_ANIM_SPEED;
		}
		//ハンドガンを持った状態で待機
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeHandgun);
			m_AnimSpeed = IDLE_ANIM_SPEED;
		}
		//ハンドガンを取る
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeHandgun);
			m_AnimSpeed = TAKEWEAPON_ANIM_SPEED;
		}

		//銃を取るアニメーションの終了時
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeHandgun &&	m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
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
	//構え状態で停止
	if ((m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupGun || m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupHandgun) &&
		m_Model.GetPlayTime() >= 29)
	{
		m_SetupWeapon = true;
		m_Model.SetTime(29);
	}
}

void Player::Die()
{

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