#include "Player.h"
#include "../GEKO/System/Input.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;	//しゃがみ姿勢じゃないときのカメラのY座標の高さ
#define CAMERA_CROUCH_POS_Y 0.8f;	//しゃがみ姿勢のときのカメラのY座標の高さ
#define WALK_SPEED 0.07f		//歩くスピード
#define RUN_SPEED 0.16f			//走るスピード
#define CROUCH_WALK_SPPED 0.03f	//しゃがみ歩きスピード

Player::Player() :
	CCharacter(ePlayer), m_CamDir(0.0f, 0.0f, 0.0f),
	m_KeyDir(0.0f, 0.0f, 0.0f), m_Horizontal(0.134f),// m_CameraT(1.0f),
	m_Vertical(-1.5f), m_MoveSpeed(0.0f), m_CameraPosY(1.8f),
	m_isCrouch(false), m_isAttack(false), m_isTakeWeapon(false), m_isMove(false),
	m_ChangeTakeWeapon(false), m_isRun(false),
	m_State(EPlayerState::eState_Idle), m_Weapons(EWeapons::eShotgun)
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

	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_LookPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);

	aa.SetAsset("Grass_1");

	m_pShotgun = new Shotgun();
	m_pHandgun = new Handgun();

	m_PlayerSightInfo.SetPos(&m_SightPos);
}

Player::~Player()
{
	delete m_pShotgun;
	delete m_pHandgun;

}

void Player::Update()
{
	m_SightPos = m_pos;
	m_SightPos.y = 2.5f;

	Move();
	Attack();
	Animation();
	Camera();
	CCharacter::Update();

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
		m_Matrix = m_Model.GetBornMatrix(3, true);
	}

	//当たり判定用 始点終点
	m_Start = m_CameraPos;
	m_End = ((m_LookPos - m_CameraPos).GetNormalize()) * 4 + m_LookPos;

	m_pShotgun->SetPlayerBomeMtx(&m_Matrix);
	m_pShotgun->SetPlayerData(m_Model.GetPlayAnimation(), m_Model.GetPlayTime(), m_Start, m_End);

	m_pShotgun->Update();
	//m_pHandgun->Update();

	m_pShotgun->Render();
	//m_pHandgun->Render();

	aa.Render();

	//printf("ChangeTakeWeapon %d TakeWeapon %d Attack %d\n", m_ChangeTakeWeapon, m_isTakeWeapon, m_isAttack);
	//printf("State %d\n", m_State);
	//printf("%d\n", m_isCrouch);
	//printf("%f %f\n", m_Vertical, m_Horizontal);
	//printf("%f %f\n", m_pos.x, m_pos.z);
	//printf("Move %d Run %d\n", m_isMove, m_isRun);
	//printf("%f\n", m_Vertical);
}

void Player::Move()
{
	m_KeyDir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = EPlayerState::eState_Idle;
	m_isMove = false;
	m_isRun = false;

	//前後左右移動
	if (Input::KeyW.Pressed())	m_KeyDir.z = 1;
	if (Input::KeyS.Pressed())	m_KeyDir.z = -1;
	if (Input::KeyD.Pressed())	m_KeyDir.x = 1;
	if (Input::KeyA.Pressed())	m_KeyDir.x = -1;

	//歩く
	if (m_KeyDir.x != 0 || m_KeyDir.z != 0)
	{
		m_State = EPlayerState::eState_Walk;
		m_isMove = true;
	}

	//走る
	if (m_isMove && Input::KeyLShift.Pressed())
	{
		m_State = EPlayerState::eState_Run;
		m_isRun = true;
	}

	//しゃがむ
	if (Input::KeyLControl.Pressed())
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

		if (m_isAttack)
		{
			//武器を構えた状態の移動
		}
		else
		{
			Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
			m_pos += pos * m_MoveSpeed;
		}
	}
}

void Player::Attack()
{
	//銃をもつ
	if (Input::KeyF.Clicked())
	{
		m_Model.SetPlayTime(0);
		m_ChangeTakeWeapon = !m_ChangeTakeWeapon;
	}
	else if (Input::KeyLControl.Pressed())
	{
		m_State = EPlayerState::eState_Crouch;
	}

	if (m_ChangeTakeWeapon)
	{
		m_Model.SetPlayTime(0);
		m_State = EPlayerState::eState_TakeWeapon;
	}
	else
	{
		m_isTakeWeapon = false;
	}

	//武器の切り替え
	if (Input::KeyT.Clicked() && !m_isTakeWeapon)
	{
		m_Model.SetPlayTime(0);
		if (m_Weapons == EWeapons::eShotgun)
		{
			m_Weapons = EWeapons::eHandgun;
		}
		else if (m_Weapons == EWeapons::eHandgun)
		{
			m_Weapons = EWeapons::eShotgun;
		}
	}

	//銃を持っているときに銃を構える
	if (Input::Mouse.RPressed() && m_isTakeWeapon)
	{
		m_State = EPlayerState::eState_SetupWeapon;
		//if (!m_isAttack) m_CameraT = 0.0f;
		m_isAttack = true;
	}
	else
	{
		//if (m_isAttack) m_CameraT = 0.0f;
		m_isAttack = false;
	}

	if (Input::Mouse.LPressed() && m_isAttack)
	{
		//Vector4D pos;
		//CBulletManager::GetInstance()->Add(m_CameraPos, ((m_LookPos - m_CameraPos).GetNormalize()) * 4 + m_LookPos, 2.00f);
		Fiqure::RenderLine3D(m_CameraPos, ((m_LookPos - m_CameraPos).GetNormalize()) * 4 + m_LookPos, Vector3D(1.0f, 0.0f, 0.0f));

		aa.SetTranselate(((m_LookPos - m_CameraPos).GetNormalize()) * 4 + m_LookPos);
	}
}

void Player::Camera()
{
	Vector3D newCameraPos;
	Vector3D newLookPos;
	Point mouseValue = Input::Mouse.GetRelativeValue();
	Vector3D look(0.6f, 1.8f, 0.0f);
	static float lenge = 2.0f;

	//マウス入力
	m_Horizontal += mouseValue.x * 0.002f;
	m_Vertical += -mouseValue.y * 0.002f;

	Matrix mRX, mRY, mat;
	mRY.RotationRadianY(m_Horizontal);
	mRX.RotationRadianX(m_Vertical);
	mat = mRY * mRX;
	m_CamDir = mat.GetAxisZ();

	if (m_isAttack)
	{
		//カメラ角度制限
		if (m_Vertical < -0.15f) m_Vertical = -0.15f; //
		if (m_Vertical > 0.5f) m_Vertical = 0.5f;	//

		m_rot.x = -m_Vertical;
		m_rot.y = m_Horizontal;

		m_Model.SetTranselate(m_pos);
		m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);

		Matrix mat = m_Model.GetBornMatrix(6, true);
		D3DXVECTOR4 eye;
		D3DXVec3Transform(&eye, &D3DXVECTOR3(-0.2f, 0.15f, -0.3f), &mat);
		D3DXVECTOR4 at;
		D3DXVec3Transform(&at, &D3DXVECTOR3(-0.2f, 0.2f, 2.0f), &mat);
		newCameraPos = Vector3D(eye.x, eye.y, eye.z);
		newLookPos = Vector3D(at.x, at.y, at.z);
	}
	else
	{
		//カメラ角度制限
		if (m_Vertical < -0.15f) m_Vertical = -0.15f;
		if (m_Vertical > 1.2f) m_Vertical = 1.2f;

		m_rot.x = 0.0f;
		newLookPos = m_pos + mat.GetAxisX() * 0.4f;
		newLookPos.y = m_CameraPosY;

		//カメラの座標
		newCameraPos = newLookPos;
		newCameraPos -= m_CamDir * lenge;
	}
	/*
		m_CameraT += 0.01f;

		if (m_CameraT > 1.0f)
		{
			m_CameraT = 1.0f;
			m_CameraPos = newCameraPos;
			m_LookPos = newLookPos;
		}
		else
		{
			m_CameraPos = Lerp(m_CameraPos, newCameraPos, m_CameraT);
			m_LookPos = Lerp(m_LookPos, newLookPos, m_CameraT);
		}
		*/

	m_CameraPos = Lerp(m_CameraPos, newCameraPos, 0.3f);
	m_LookPos = Lerp(m_LookPos, newLookPos, 0.3f);

	Camera::SetEye(m_CameraPos);
	Camera::SetLookat(m_LookPos);
}

void Player::Animation()
{
	m_CameraPosY = CAMERA_NO_CROUCH_POS_Y;
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
		m_Matrix = m_Model.GetBornMatrix(24, true);
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
		m_Model.SetPlayTime(15);
	}
	//待機
	else
	{
		m_Matrix = m_Model.GetBornMatrix(3, true);
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Idle);
		m_Model.SetPlayTime(15);
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isTakeWeapon) //武器を持っているとき
	{
		m_Matrix = m_Model.GetBornMatrix(24, true);
		if (m_Weapons == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeGun);
			m_Model.SetPlayTime(30);
		}
		else if (m_Weapons == EWeapons::eHandgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeHandgun);
			m_Model.SetPlayTime(30);
		}
	}
	else
	{
		//歩き
		m_Matrix = m_Model.GetBornMatrix(3, true);
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Walk);
		m_Model.SetPlayTime(30);
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;

	//武器を持っているとき
	if (m_isTakeWeapon)
	{
		m_Matrix = m_Model.GetBornMatrix(24, true);
		if (m_Weapons == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeGun);
			m_Model.SetPlayTime(50);
		}
		else if (m_Weapons == EWeapons::eHandgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeHandgun);
			m_Model.SetPlayTime(50);
		}
	}
	else//武器を持っていないとき
	{
		m_Matrix = m_Model.GetBornMatrix(3, true);
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_Run);
		m_Model.SetPlayTime(50);
	}
}

void Player::Crouch()
{
	m_Matrix = m_Model.GetBornMatrix(3, true);
	//武器を持っている時は武器をしまってからしゃがむ
	if (m_isTakeWeapon)
	{
		m_Matrix = m_Model.GetBornMatrix(24, true);
		if (m_Weapons == EWeapons::eShotgun)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
			m_Model.SetTime(-29);
		}
		else if (m_Weapons == EWeapons::eHandgun)
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
			m_Model.SetPlayTime(60);
		}

		//しゃがみ待機
		if (m_KeyDir.x == 0 && m_KeyDir.z == 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_CrouchIdle);
			m_Model.SetPlayTime(15);
		}
		//しゃがみ歩き
		else if (m_KeyDir.x != 0 || m_KeyDir.z != 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_CrouchWalk);
			m_Model.SetPlayTime(30);
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
	m_Matrix = m_Model.GetBornMatrix(24, true);
	if (m_Weapons == EWeapons::eShotgun)
	{
		//ショットガンを持って歩く
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeGun);
			m_Model.SetPlayTime(30);
		}
		//ショットガンを持って走る
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeGun);
			m_Model.SetPlayTime(50);
		}
		//ショットガンを持った状態で待機
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeGun);
			m_Model.SetPlayTime(15);
		}
		//ショットガンを取る
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeGun);
			m_Model.SetPlayTime(30);
		}

		//銃を取るアニメーションの終了時
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeGun && m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
		}
	}
	else if (m_Weapons == EWeapons::eHandgun)
	{
		//ハンドガンを持って歩く
		if (m_isMove && m_isTakeWeapon && !m_isRun)
		{
			m_MoveSpeed = WALK_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_WalkTakeHandgun);
			m_Model.SetPlayTime(30);
		}
		//ハンドガンを持って走る
		else if (m_isRun && m_isTakeWeapon)
		{
			m_MoveSpeed = RUN_SPEED;
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_RunTakeHandgun);
			m_Model.SetPlayTime(50);
		}
		//ハンドガンを持った状態で待機
		else if (!m_isMove && m_isTakeWeapon)
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_IdleTakeHandgun);
			m_Model.SetPlayTime(30);
		}
		//ハンドガンを取る
		else
		{
			m_Model.ChangeAnimation(EPlayerAnim::eAnim_TakeHandgun);
			m_Model.SetPlayTime(30);
		}

		//銃を取るアニメーションの終了時
		if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_TakeHandgun &&	m_Model.GetPlayTime() >= 29)
		{
			m_isTakeWeapon = true;
		}
	}

}

void Player::SetupWeapon()
{
	m_Matrix = m_Model.GetBornMatrix(24, true);
	m_MoveSpeed = WALK_SPEED;

	if (m_Weapons == EWeapons::eShotgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_SetupGun);
		m_Model.SetPlayTime(50);
	}
	else if (m_Weapons == EWeapons::eHandgun)
	{
		m_Model.ChangeAnimation(EPlayerAnim::eAnim_SetupHandgun);
		m_Model.SetPlayTime(50);
	}
	//構え状態で停止
	if (m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupGun || m_Model.GetPlayAnimation() == EPlayerAnim::eAnim_SetupHandgun &&
		m_Model.GetPlayTime() >= 29)
	{
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

Vector3D Player::Lerp(Vector3D start, Vector3D finish, float percentage)
{
	return start * (1 - percentage) + finish * percentage;
}