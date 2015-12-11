#include "Player.h"
#include "../GEKO/System/Input.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;	//しゃがみ姿勢じゃないときのカメラのY座標の高さ
#define CAMERA_CROUCH_POS_Y 0.8f;	//しゃがみ姿勢のときのカメラのY座標の高さ
#define WALK_SPEED 0.07f		//歩くスピード
#define RUN_SPEED 0.16f			//走るスピード
#define CROUCH_WALK_SPPED 0.03f	//しゃがみ歩きスピード

Player::Player() :
	CCharacter(ePlayer), m_CamDir(0.0f, 0.0f, 0.0f),
	m_Dir(0.0f, 0.0f, 0.0f), m_Horizontal(0.0f),
	m_Vertical(0.0f), m_MoveSpeed(0.0f), m_CameraPosY(1.8f),
	m_isCrouch(false), m_isAttack(false), m_isTakeWeapon(false), m_isMove(false), m_ChangeTakeWeapon(false),
	m_Anim(eAnim_Idle), m_State(eState_Idle), m_Weapons(eShotgun)
{
	m_ColliderMap.SetID(eHITID1, eHITID0 | eHITID1 | eHITID2);
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
		//eHITID0…マップ
		//eHITID1…プレイヤー
		//eHITID2…敵
		//eHITID3…弾
		m_pCollider[i].SetID(eHITID1, 0);
	}
	m_Model.SetRotationRadian(0.0f, 3.14f, 0.0f);
	m_Model.SetScale(1.0f, 1.0f, 1.0f);

	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_LookPos, REGIST_FUNC(Player::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);
}

Player::~Player()
{

}

void Player::Update()
{
	Move();
	Attack();
	Animation();
	Camera();
	CCharacter::Update();
}

void Player::Move()
{
	m_Dir = Vector3D(0.0f, 0.0f, 0.0f);
	m_State = eState_Idle;
	m_isMove = false;

	//前後左右移動
	if (Input::KeyW.Pressed())	m_Dir.z = 1;
	if (Input::KeyS.Pressed())	m_Dir.z = -1;
	if (Input::KeyD.Pressed())	m_Dir.x = 1;
	if (Input::KeyA.Pressed())	m_Dir.x = -1;

	//歩く
	if (m_Dir.x != 0 || m_Dir.z != 0)
	{
		m_State = eState_Walk;
		m_isMove = true;
	}

	//走る
	if (m_Dir.x != 0 || m_Dir.z != 0 && Input::KeyLShift.Pressed())
	{
		m_State = eState_Run;
		m_isMove = true;
	}

	//しゃがむ
	if (Input::KeyLControl.Pressed())
	{
		m_State = eState_Crouch;
	}
	else
	{
		m_isCrouch = false;
	}

	//プレイヤーが歩き状態なら移動処理
	if (m_isMove)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_Dir.x, m_Dir.z), 0.0f);

		//playerRot = (m_Model.GetRotation() + (playerRot - m_Model.GetRotation()) * 0.2);

		m_rot.y = playerRot.y;
		Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
		m_pos += pos * m_MoveSpeed;
	}

	if (Input::KeyB.Pressed())
	{
		m_Anim = eAnim_Die;
	}
}

void Player::Attack()
{
	if (Input::KeyF.Clicked())
	{
		m_isTakeWeapon = !m_isTakeWeapon;
	}

	if (Input::KeyT.Clicked() && !m_isTakeWeapon)
	{
		if (m_Weapons == eShotgun)
		{
			m_Weapons = eHandgun;
		}
		else if (m_Weapons == eHandgun)
		{
			m_Weapons = eShotgun;
		}
	}


	if (Input::KeyG.Clicked() && m_isTakeWeapon)
	{
		D3DXVECTOR4 pos;
		D3DXVec3Transform(&pos, &D3DXVECTOR3(0, 0, 20), &m_GunMatrix);
		CBulletManager::GetInstance()->Add(Vector3D(pos.x, pos.y + 1.0f, pos.z), m_CamDir, 2.00f);

		m_State = eState_SetupGun;
		m_isAttack = !m_isAttack;
	}
}

void Player::Camera()
{
	Point mouseValue = Input::Mouse.GetRelativeValue();
	Vector3D look(0.6f, 1.8f, 0.0f);
	static float lenge = 2.0f;

	//マウス入力
	m_Horizontal -= mouseValue.x * 0.002f;
	m_Vertical -= -mouseValue.y * 0.002f;

	//カメラ角度制限
	if (m_Vertical > -0.5f) m_Vertical = -0.5f;	//上限
	if (m_Vertical < -2.0f) m_Vertical = -2.0f;	//下限

	m_LookPos = m_Model.GetAxisX(-0.5f);
	m_LookPos.y = m_CameraPosY + m_pos.y;

	m_CamDir = Vector3D(cosf(m_Vertical) * sinf(m_Horizontal), -sinf(m_Vertical), cosf(m_Vertical) * cosf(m_Horizontal));

	//カメラの座標
	m_CameraPos = m_LookPos;
	m_CameraPos.x += lenge * sinf(m_Vertical) * cosf(m_Horizontal);
	m_CameraPos.y += lenge * cosf(m_Vertical);
	m_CameraPos.z += lenge * sinf(m_Vertical) * sinf(m_Horizontal);

	if (m_isAttack)
	{
		m_Model.SetTranselate(m_pos);
		m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);

		Matrix mat = m_Model.GetBornMatrix(6, true);
		D3DXVECTOR4 eye;
		D3DXVec3Transform(&eye, &D3DXVECTOR3(-0.2f, 0.0f, 0.0f), &mat);
		D3DXVECTOR4 at;
		D3DXVec3Transform(&at, &D3DXVECTOR3(-0.2f, 0.0f, 2.0f), &mat);
		Camera::SetEye(Vector3D(eye.x, eye.y, eye.z));
		Camera::SetLookat(Vector3D(at.x, at.y, at.z));
		Camera::SetUpVec(Vector3D(0, 1, 0));
	}
	else
	{
		Camera::SetEye(m_CameraPos);
		Camera::SetLookat(m_LookPos);
		Camera::SetUpVec(Vector3D(0, 1, 0));
	}
}

void Player::Animation()
{
	m_CameraPosY = CAMERA_NO_CROUCH_POS_Y;
	switch (m_State)
	{
	case eState_Idle:
		Idle();
		break;
	case eState_Walk:
		Walk();
		break;
	case eState_Run:
		Run();
		break;
	case eState_Crouch:
		Crouch();
		break;
	case eState_TakeGun:
		TakeWeapon();
		break;
	case eState_SetupGun:
		SetupWeapon();
		break;
	case eState_Hit:
		Hit();
		break;
	case eState_Die:
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

void Player::SetGunMtx(Matrix m)
{
	m_GunMatrix = m;
}


void Player::Idle()
{
	//武器を持って待機
	if (m_isTakeWeapon)
	{
		m_Model.ChangeAnimation(eAnim_IdleTakeGun);
		m_Model.SetPlayTime(15);
	}
	//待機
	else
	{
		m_Model.ChangeAnimation(eAnim_Idle);
		m_Model.SetPlayTime(15);
	}
}

void Player::Walk()
{
	m_MoveSpeed = WALK_SPEED;

	if (m_isTakeWeapon) //武器を持っているとき
	{
		if (m_Weapons == eShotgun)
		{
			m_Model.ChangeAnimation(eAnim_WalkTakeGun);
			m_Model.SetPlayTime(30);
		}
		else
		{
			m_Model.ChangeAnimation(eAnim_WalkTakeHandgun);
			m_Model.SetPlayTime(30);
		}
	}
	else
	{
		//歩き
		m_Model.ChangeAnimation(eAnim_Walk);
		m_Model.SetPlayTime(30);
	}
}

void Player::Run()
{
	m_MoveSpeed = RUN_SPEED;

	//武器を持っているとき
	if (m_isTakeWeapon)
	{
		if (m_Weapons == eShotgun)
		{
			m_Model.ChangeAnimation(eAnim_RunTakeGun);
			m_Model.SetPlayTime(50);
		}
		else
		{
			m_Model.ChangeAnimation(eAnim_RunTakeHandgun);
			m_Model.SetPlayTime(50);
		}
	}
	else//武器を持っていないとき
	{
		m_Model.ChangeAnimation(eAnim_Run);
		m_Model.SetPlayTime(50);
	}
}

void Player::Crouch()
{
	static int time = 29;

	//武器を持っている時は武器をしまってからしゃがむ
	if (m_isTakeWeapon)
	{
		if (m_Weapons == eShotgun)
		{
			m_Model.ChangeAnimation(eAnim_TakeGun);
			m_Model.SetTime(time);
			if (time >= 0)	time--;
		}
		else
		{
			m_Model.ChangeAnimation(eAnim_TakeHandgun);
			m_Model.SetTime(time);
			if (time >= 0)	time--;
		}
		//しまうアニメーションの終わり
		if (m_Model.GetPlayTime() == 0)
		{
			m_isTakeWeapon = false;
			time = 29;
		}
	}
	else
	{
		//しゃがみ移行
		if (!m_isCrouch)
		{
			m_Model.ChangeAnimation(eAnim_Crouch);
			m_Model.SetPlayTime(70);
		}

		//しゃがみ待機
		if (m_Dir.x == 0 && m_Dir.z == 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_Model.ChangeAnimation(eAnim_CrouchIdle);
			m_Model.SetPlayTime(15);
		}
		//しゃがみ歩き
		else if (m_Dir.x != 0 || m_Dir.z != 0 && m_isCrouch)
		{
			m_CameraPosY = CAMERA_CROUCH_POS_Y;
			m_MoveSpeed = CROUCH_WALK_SPPED;
			m_Model.ChangeAnimation(eAnim_CrouchWalk);
			m_Model.SetPlayTime(30);
			m_isMove = true;
		}

		if (m_Model.GetPlayTime() >= 29 && !m_isCrouch)
		{
			m_isCrouch = true;
		}
	}
}

void Player::TakeWeapon()
{
	if (m_Weapons == eShotgun)
	{
		m_Model.ChangeAnimation(eAnim_TakeGun);
		m_Model.SetPlayTime(40);
		if (m_Model.GetPlayTime() >= 29)
		{
			m_Model.ChangeAnimation(eAnim_IdleTakeGun);
		}
	}
	else if (m_Weapons == eHandgun)
	{

	}
}

void Player::SetupWeapon()
{

}

void Player::Die()
{

}

void Player::Hit()
{

}

void Player::HitCamera(Result_Porygon &hitData)
{
	m_CameraPos = hitData.contactPos;
	Camera::SetEye(hitData.contactPos);
}






Shotgun::Shotgun(Player* p)
{
	m_Shotgun.SetAsset("Shotgun");
	m_pPlayer = p;
}

Shotgun::~Shotgun()
{

}


void Shotgun::Update()
{
	if (m_pPlayer->GetAnim() == 11 && m_pPlayer->GetPlayTime() >= 14)
	{
		//構え状態
		m_Shotgun.SetTranselate(0.05f, 0.05f, 0.0f);
		m_Shotgun.SetRotationDegree(175, -92, 30);
		m_BoneMat = m_pPlayer->GetBomeMat(24);
	}
	else
	{
		//肩にかけている状態
		m_Shotgun.SetTranselate(0.2f, 0.14f, 0.0f);
		m_Shotgun.SetRotationDegree(65, 0, 12);
		m_BoneMat = m_pPlayer->GetBomeMat(21);
	}
	m_Matrix = *m_Shotgun.GetMatrix();
}

void Shotgun::Render()
{
	m_Shotgun.RenderMatrix(*m_Shotgun.GetMatrix() * m_BoneMat);
}

Matrix Shotgun::GetMatrix()
{
	return m_Matrix;
}




Handgun::Handgun(Player* p)
{
	m_pPlayer = p;
	m_Handgun.SetAsset("Handgun");
}

Handgun::~Handgun()
{

}

void Handgun::Update()
{
	if (m_pPlayer->GetAnim() == 12 && m_pPlayer->GetPlayTime() >= 14)
	{
		//構え状態
		m_Handgun.SetTranselate(0.05f, 0.0f, 0.0f);
		m_Handgun.SetRotationDegree(180, 90, 30);
		m_BoneMat = m_pPlayer->GetBomeMat(25);
	}
	else
	{
		m_Handgun.SetTranselate(-0.15f, 0.0f, -0.15f);
		m_Handgun.SetRotationDegree(10, 100, 0);
		m_BoneMat = m_pPlayer->GetBomeMat(3);
	}
	m_Matrix = *m_Handgun.GetMatrix();
}

void Handgun::Render()
{
	m_Handgun.RenderMatrix(*m_Handgun.GetMatrix() * m_BoneMat);
}

Matrix Handgun::GetMatrix()
{
	return m_Matrix;
}