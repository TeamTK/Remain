#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "WeaponInfo.h"
#include "../Player/PlayerInfo.h"

#define AMMO_LOADED_SHOTGUN 6	//�ő呕�e���V���b�g�K��
#define AMMO_LOADED_HANDGUN 6	//�ő呕�e���n���h�K��
#define DEFAULT_BULLET_HAVE 6	//�������e��

class WeaponBase : public Task
{
public:
	WeaponBase();
	WeaponBase(int* anim, float* frame, bool* take, Matrix* m, const char *taskName, unsigned int priority);
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	int *GetAmmo();			//�g�ђe�����擾
	int *GetLoadedAmmo();	//����Ƀ��[�h���Ă���e�����擾
	int *GetAllAmmo();		//���ׂĂ̒e�򐔂��擾
	void SetAmmo(int num);			//�g�ђe����ݒ�
	void SetLoadedAmmo(int num);	//����Ƀ��[�h���Ă���e����ݒ�
	bool CanShot();			//�e�����Ă邩
	void ReduceBullet();	//�e�����炷
	void AddAmmo(int addnum);	//�e�𑝂₷
	void Reload();			//�e�������[�h
	EWeapons GetSelect();

protected:
	int m_Ammo;				//�莝���e��
	int m_LoadedAmmo;		//�e�ɓ����Ă���e��
	int* m_PlayerAnim;		//�v���C���[�̍Đ����A�j���[�V����
	float* m_PlayerAnimFrame;	//�v���C���[�̃A�j���[�V�����t���[��
	bool* m_TakeWeapon;		//�v���C���[������������Ă��邩
	StaticMesh m_Model;
	Matrix* m_BoneMtx;		//�v���C���[�̃{�[���s��
	RenderTask m_RenderTask;
	EWeapons* m_Weapon;
};

#endif