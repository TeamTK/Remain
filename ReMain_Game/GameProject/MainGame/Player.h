#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"
#include "enums.h"

#include "../MainGame/Shotgun.h"
#include "../MainGame/Handgun.h"
#include "..\GameSystem\SightSystem.h"

class Player : public CCharacter
{
public:
	Player();
	~Player();
	void Update();
	void Move();
	void Attack();
	void Camera();
	void Animation();
	Matrix GetBomeMat(int bornIndex);
	int GetAnim();
	float GetPlayTime();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void TakeWeapon();
	void SetupWeapon();
	void Die();
	void Hit();

	void HitCamera(Result_Porygon &hitData);
	Vector3D Lerp(Vector3D start, Vector3D finish, float percentage);

private:
	Shotgun* m_pShotgun;
	Handgun* m_pHandgun;
	Vector3D m_KeyDir;		//キー入力軸
	Vector3D m_CamDir;		//カメラの方向
	Vector3D m_CameraPos;	//カメラの位置
	Vector3D m_LookPos;		//カメラの注視点
	Vector3D m_Start;		//線分の当たり判定用 始点
	Vector3D m_End;			//線分の当たり判定用 終点
	Vector3D m_SightPos;
	Matrix m_Matrix;		//ショットガンの行列
	EPlayerState m_State;	//プレイヤーの状態	
	EWeapons m_Weapons;		//選択している銃
	Collider m_HitCamera;	//カメラの当たり判定
	Collider m_Bullet;		//弾の当たり判定
	PlayerSightInfo m_PlayerSightInfo;
	float m_Vertical;		//垂直方向のマウス入力量
	float m_Horizontal;		//水平方向のマウス入力量
	float m_MoveSpeed;		//移動速度
	float m_CameraPosY;		//カメラのY座標
	//float m_CameraT;
	bool m_ChangeTakeWeapon;	//銃を持つ
	bool m_isTakeWeapon;	//銃を持っているか
	bool m_isCrouch;	//しゃがんでいるか
	bool m_isRun;		//走っているか
	bool m_isMove;		//移動中か
	bool m_isAttack;	//攻撃中か

};
#endif
