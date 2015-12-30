#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "Character.h"
#include "enums.h"

#include "../MainGame/Shotgun.h"
#include "../MainGame/Handgun.h"
#include "../GameSystem/SightSystem.h"
#include"../GameSystem/GUI/UI_SelectWeapon.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
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
	void PutBackWeapon();
	void SetupWeapon();
	void Die();
	void Hit();

	void HitCamera(Result_Porygon &hitData);
	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);

private:
	Shotgun m_pShotgun;
	Handgun m_pHandgun;
	UI_SelectWeapon m_SelectWeapon;

	Vector3D m_KeyDir;		//キー入力軸
	Vector3D m_PadDir;		//コントローラー入力値
	Vector3D m_CamDir;		//カメラの方向
	Vector3D m_CameraPos;	//カメラの位置
	Vector3D m_LookPos;		//カメラの注視点
	Vector3D m_Start;		//線分の当たり判定用 始点
	Vector3D m_End;			//線分の当たり判定用 終点
	Vector3D m_SightPos;	//視線判定の位置
	Matrix m_Matrix;		//ショットガンの行列
	EPlayerState m_State;	//プレイヤーの状態	
	EWeapons m_SelectedWeapon;	//選択している銃
	Collider m_HitCamera;	//カメラの当たり判定
	Collider m_Bullet;		//弾の当たり判定
	Collider m_HitAmmoBox;	//弾薬箱の当たり判定
	Collider m_HitEnemyAttack; //敵からの攻撃の当たり判定
	PlayerSightInfo m_PlayerSightInfo; //プレイヤーの視界情報
	float m_Vertical;		//垂直方向のマウス入力量
	float m_Horizontal;		//水平方向のマウス入力量
	float m_MoveSpeed;		//移動速度
	float m_CameraPosY;		//カメラのY座標
	float m_CamSpeed;		//カメラの回転速度
	float m_AnimSpeed;		//アニメーションスピード
	float m_Radius;
	int m_Phase;
	bool m_ChangeTakeWeapon;	//銃を持つ
	bool m_ToggleCrouch;
	bool m_isTakeWeapon;	//銃を持っているか
	bool m_SetupWeapon;	//銃を構えているか
	bool m_isCrouch;	//しゃがんでいるか
	bool m_isRun;		//走っているか
	bool m_isMove;		//移動中か
	bool m_isAttack;	//攻撃中か
};
#endif
