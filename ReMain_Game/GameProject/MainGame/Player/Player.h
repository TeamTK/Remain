#ifndef PlAYER_H
#define PlAYER_H

#include "../../MainGame/Weapon/Shotgun.h"
#include "../../MainGame/Weapon/Handgun.h"
#include "../../MainGame/Weapon/Knife.h"
#include "PlayerInfo.h"
#include "PlayerData.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
{
public:
	Player(PData* data, Vector3D pos, float rotY, float horizontal, float vertical);
	~Player();
	void Update();
	void Move();
	void Attack();
	void Animation();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void StandUp();
	void TakeWeapon();
	void PutBackWeapon();
	void SetupWeapon();
	void Recoil();
	void Reload();
	void StealthAttack();
	void Die();
	void Hit();

	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);
	void StageChange(Result_Sphere &data);
	void HitRecoveryItem(Result_Sphere &data);

private:
	Vector3D m_KeyDir;		//キー入力軸
	Vector3D m_LookPos;		//カメラの注視点
	Vector3D m_SightPos;	//視線判定の位置
	Matrix m_MatrixS;		//ショットガン用プレイヤーの行列
	Matrix m_MatrixH;		//ハンドガン用プレイヤーの行列
	Matrix m_MatrixK;		//ナイフ用プレイヤーの行列
	EPlayerState m_State;	//プレイヤーの状態
	EPlayerAnim m_Anim;		//プレイヤーのアニメーション
	EWeapons m_SelectedWeapon; //選択している銃
	PlayerSightInfo m_PlayerSightInfo; //プレイヤーの視界情報
	AuditoryObject m_AuditoryObject; //聴覚対象
	Timer m_Timer;					//ダメージを食らった後の無敵時間
	Timer m_NoActionTime;			//ロード時の無操作時間
	TracerouteTarget m_Target;		//経路探索対象

	Collider m_Bullet;		//弾の当たり判定
	Collider m_HitAmmoBox;	//弾薬箱の当たり判定
	Collider m_HitEnemyAttack; //敵からの攻撃の当たり判定
	Collider m_StageChange; //ステージ移動用
	Collider m_HitRecoveryItem; //回復アイテムの当たり判定

	Sound m_HandGunFire;			//ハンドガンの発砲音
	Sound m_ShotGunFire;			//ショットガンの発砲音
	Sound m_GunEmpty;			    //弾が弾切れになった時の音
	Sound m_GunADS;					//銃を構えた時の音
	Sound m_HitEnemyAttackSound;	//敵から攻撃を食らった音

	Image m_DamegeBlood;

	CameraInfo m_CameraInfo;

	int m_Phase;
	int m_PlayAnim;			//武器に渡す用
	int m_Volume;			//プレイヤーが出す音の音量
	int m_RecoveryItemNum;	//回復薬の携帯数

	float m_MoveSpeed;		//移動速度
	float m_AnimSpeed;		//アニメーションスピード
	float m_PlayAnimTime;	//武器に渡す用
	float m_Radius;			//弾薬箱との当たり判定の半径
	float m_OneFlameTime;	//1フレームの時間

	bool m_ChangeTakeWeapon;		//銃を持つ
	bool m_ChangePutBackWeapon;	//銃をしまう
	bool m_ToggleCrouch;			//しゃがみトグル
	bool m_isCrouch;				//しゃがんでいるか
	bool m_isShiftCrouch;			//しゃがんでいるか
	bool m_isTakeWeapon;			//銃を持っているか
	bool m_SetupWeapon;			//銃を構えているか
	bool m_isRun;					//走っているか
	bool m_isMove;					//移動中か
	bool m_isAttack;				//攻撃中か
	bool m_isReload;				//リロード中か
	bool m_isHit;					//敵の攻撃が当たったか
	bool m_isShot;					//発砲したか
	bool m_isSteAttack;			//ステルスアタック
	bool m_isDead;					//死亡判断
	bool m_IsStop;					//プレイヤーに関係するタスク停止
};

#endif
