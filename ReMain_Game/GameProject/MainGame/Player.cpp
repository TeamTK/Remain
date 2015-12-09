#include "Player.h"
#include "../GEKO/System/Input.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;	//しゃがみ姿勢じゃないときのカメラのY座標の高さ
#define CAMERA_CROUCH_POS_Y 0.8f;	//しゃがみ姿勢のときのカメラのY座標の高さ
#define WALK_SPEED 0.07f		//歩くスピード
#define RUN_SPEED 0.16f			//走るスピード
#define CROUCH_WALK_SPPED 0.03f	//しゃがみ歩きスピード

Player::Player() :
	m_Dir(0.0f, 0.0f, 0.0f), m_Horizontal(0.0f),
	m_Vertical(0.0f), m_MoveSpeed(0), m_CameraPosY(1.8f),
	m_isCrouch(false), m_isAttack(false), m_isMove(false), isTakeWeapons(false),
	m_Anim(eIdle), m_State(eWait), m_Weapons(eShotgun)
{
	DynamicMeshAsset::LoadMesh("media\\Player.x", "Player");
	m_Model.SetAsset("Player");
	m_Model.SetRotationRadian(0.0f, 3.14f, 0.0f);
	m_Model.SetScale(1.0f, 1.0f, 1.0f);
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
/*
void Player::Render()
{
	m_Model.Render();
}
*/
void Player::Move()
{
	m_Dir = Vector3D(0.0f, 0.0f, 0.0f);
	m_Anim = EPlayerAnimation::eIdle;
	m_State = EPlayerState::eWait;
	m_isMove = false;

	//前後左右移動
	if (Input::KeyW.Pressed())	m_Dir.z = 1;
	if (Input::KeyS.Pressed())	m_Dir.z = -1;
	if (Input::KeyD.Pressed())	m_Dir.x = 1;
	if (Input::KeyA.Pressed())	m_Dir.x = -1;

	if (m_Dir.x != 0 || m_Dir.z != 0 && !isTakeWeapons)
	{
		//歩く
		m_Anim = eWalk;
		m_State = eMove;
	}
	else if (m_Dir.x != 0 || m_Dir.z != 0 && isTakeWeapons)
	{
		//武器を持って歩く
		if (m_Weapons == eShotgun)
		{
			m_Anim = eWalkTakeGun;
			m_State = eMove;
		}
		else
		{
			m_Anim = eWalkTakeHandgun;
			m_State = eMove;
		}
	}
	else
	{
		m_Anim = eIdle;
		m_State = eWait;
	}


	if (Input::KeyLShift.Pressed() && m_State == eMove && !isTakeWeapons)
	{
		//走る
		m_Anim = eRun;
		m_State = eMove;
	}
	else if (Input::KeyLShift.Pressed() && m_State == eMove && isTakeWeapons)
	{
		//武器を持って走る
		if (m_Weapons == eShotgun)
		{
			m_Anim = eRunTakeGun;
			m_State = eMove;
		}
		else
		{
			m_Anim = eRunTakeHandgun;
			m_State = eMove;
		}
	}

	if (m_State == eWait && isTakeWeapons)
	{
		m_Anim = eIdleTakeGun;
	}

	//しゃがみ状態に移行
	if (Input::KeyLControl.Pressed())
	{
		if (m_isCrouch)
		{	//しゃがみ待機
			m_Anim = EPlayerAnimation::eCrouchIdle;
		}
		else
		{	//しゃがむ途中
			m_Anim = EPlayerAnimation::eCrouch;
		}
		if (Input::KeyLControl.Pressed() && m_State == EPlayerState::eMove)
		{
			m_Anim = EPlayerAnimation::eCrouchWalk;
		}
	}
	else //キーを離した
	{
		m_isCrouch = false;
	}
	if (Input::KeyLControl.Clicked())
	{
		m_Model.SetTime(0);
	}

	//プレイヤーが歩き状態なら移動処理
	if (m_State == EPlayerState::eMove)
	{
		const D3DXMATRIX *camDir = Camera::GetView();
		Vector3D playerRot(0.0f, atan2f(camDir->m[0][2], camDir->m[2][2]) + atan2f(m_Dir.x, m_Dir.z), 0.0f);

		//playerRot = (m_Model.GetRotation() + (playerRot - m_Model.GetRotation()) * 0.2);

		//m_Model.SetRotationRadian(0.0f, playerRot.y, 0.0f);
		m_rot.y = playerRot.y;

		Vector3D pos(sinf(m_Model.GetRotation().y), 0.0f, cosf(m_Model.GetRotation().y));
		//Vector3D playerPos = m_Model.GetTranselate();

		m_pos += pos * m_MoveSpeed;
		//m_Model.SetTranselate(playerPos);
	}

	if (Input::KeyB.Pressed())
	{
		m_Anim = EPlayerAnimation::eDie;
	}
}

void Player::Attack()
{
	if (Input::KeyG.Clicked())
	{
		isTakeWeapons = !isTakeWeapons;
	}


	if (Input::KeyT.Clicked() && !isTakeWeapons)
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

	if (isTakeWeapons)
	{
		if (m_Weapons == eShotgun)
		{
			m_Anim = eTakeGun;
		}
		else if (m_Weapons == eHandgun)
		{
			m_Anim = eTakeHandgun;
		}
	}


	if (Input::KeyG.Pressed()) {

		D3DXVECTOR4 pos;
	//	D3DXVec3Transform(&pos, &D3DXVECTOR3(0, 0, 20), &m_GunMatrix);
	//	CBulletManager::GetInstance()->Add(Vector3D(pos.x, pos.y, pos.z), m_CamDir, 1.00f);
		CBulletManager::GetInstance()->Add(Vector3D(m_pos.x, m_pos.y+10.0f, m_pos.z), Vector3D(0,0,1), 0.10f);
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

	Vector3D v = m_Model.GetTranselate() + m_Model.GetAxisX(1.0f) * 0.4f;
	v.y = m_CameraPosY;

	//カメラの座標
	Vector3D camPos = v;
	camPos.x += lenge * sinf(m_Vertical) * cosf(m_Horizontal);
	camPos.y += lenge * cosf(m_Vertical);
	camPos.z += lenge * sinf(m_Vertical) * sinf(m_Horizontal);

	if (m_isAttack)
	{


	}
	else
	{
		Camera::SetEye(camPos);
		Camera::SetLookat(v);
	}
}

void Player::Animation()
{
	m_CameraPosY = CAMERA_NO_CROUCH_POS_Y;

	switch (m_Anim)
	{
	case eCrouch:
		m_Model.ChangeAnimation(eCrouch);
		m_Model.SetPlayTime(80);

		if (m_Model.GetPlayTime() >= 29)
		{
			m_Anim = eCrouchIdle;
			m_isCrouch = true;
		}
		break;
	case eCrouchIdle:
		m_Model.ChangeAnimation(eCrouchIdle);
		m_Model.SetPlayTime(15);
		m_CameraPosY = CAMERA_CROUCH_POS_Y;

		break;
	case eCrouchWalk:
		m_Model.ChangeAnimation(eCrouchWalk);
		m_Model.SetPlayTime(30);
		m_MoveSpeed = CROUCH_WALK_SPPED;
		m_CameraPosY = CAMERA_CROUCH_POS_Y;

		break;
	case eDie:
		m_Model.ChangeAnimation(eDie);
		m_Model.SetPlayTime(30);
		if (m_Model.GetPlayTime() >= 29)
		{
			m_Model.SetTime(29);
		}

		break;
	case eHit:
		m_Model.ChangeAnimation(eHit);
		m_Model.SetPlayTime(30);

		break;
	case eIdle:
		m_Model.ChangeAnimation(eIdle);
		m_Model.SetPlayTime(15);

		break;
	case eIdleTakeGun:
		m_Model.ChangeAnimation(eIdleTakeGun);
		m_Model.SetPlayTime(15);

		break;
	case eIdleTakeHandgun:
		m_Model.ChangeAnimation(eIdleTakeHandgun);
		m_Model.SetPlayTime(15);

		break;
	case eRun:
		m_Model.ChangeAnimation(eRun);
		m_Model.SetPlayTime(50);
		m_MoveSpeed = RUN_SPEED;

		break;
	case eRunTakeGun:
		m_Model.ChangeAnimation(eRunTakeGun);
		m_Model.SetPlayTime(50);
		m_MoveSpeed = RUN_SPEED;

		break;
	case eRunTakeHandgun:
		m_Model.ChangeAnimation(eRunTakeHandgun);
		m_Model.SetPlayTime(50);
		m_MoveSpeed = RUN_SPEED;

		break;
	case eSetupGun:
		m_Model.ChangeAnimation(eSetupGun);
		m_Model.SetPlayTime(50);

		break;
	case eSetupHandgun:
		m_Model.ChangeAnimation(eSetupHandgun);
		m_Model.SetPlayTime(50);

		break;
	case eTakeGun:
		m_Model.ChangeAnimation(eTakeGun);
		m_Model.SetPlayTime(40);
		if (m_Model.GetPlayTime() >= 29)
		{
			m_Model.ChangeAnimation(eIdleTakeGun);
		}

		break;
	case eTakeHandgun:
		m_Model.ChangeAnimation(eTakeHandgun);
		m_Model.SetPlayTime(40);
		if (m_Model.GetPlayTime() >= 29)
		{
			m_Model.ChangeAnimation(eIdleTakeHandgun);
		}

		break;
	case eWalk:
		m_Model.ChangeAnimation(eWalk);
		m_Model.SetPlayTime(30);
		m_MoveSpeed = WALK_SPEED;

		break;
	case eWalkTakeGun:
		m_Model.ChangeAnimation(eWalkTakeGun);
		m_Model.SetPlayTime(30);
		m_MoveSpeed = WALK_SPEED;

		break;
	case eWalkTakeHandgun:
		m_Model.ChangeAnimation(eWalkTakeHandgun);
		m_Model.SetPlayTime(30);
		m_MoveSpeed = WALK_SPEED;

		break;
	default:
		break;
	}
	//printf("%d\n", m_Anim);
}


Matrix Player::GetBomeMat(int bornIndex)
{
	return m_Model.GetBornMatrix(bornIndex, true);
}

int Player::GetAnim()
{
	return m_Anim;
}

float Player::GetPlayTime()
{
	return m_Model.GetPlayTime();
}





Shotgun::Shotgun(Player* p)
{
	StaticMeshAsset::LoadMesh("media\\Shotgun.x", "Shotgun");
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
}

void Shotgun::Render()
{
	m_Shotgun.RenderMatrix(*m_Shotgun.GetMatrix() * m_BoneMat);
}





Handgun::Handgun(Player* p)
{
	m_pPlayer = p;
	StaticMeshAsset::LoadMesh("media\\Handgun.x", "Handgun");
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
}

void Handgun::Render()
{
	m_Handgun.RenderMatrix(*m_Handgun.GetMatrix() * m_BoneMat);
}