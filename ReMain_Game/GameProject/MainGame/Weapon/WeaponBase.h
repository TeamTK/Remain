#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "../../GEKO/GEKO.h"
#include "../enums.h"

#define AMMO_LOADED_SHOTGUN 6	//�ő呕�e���V���b�g�K��
#define AMMO_LOADED_HANDGUN 6	//�ő呕�e���n���h�K��

class WeaponBase : public Task
{
public:
	WeaponBase();
	WeaponBase(int* anim, float* frame, Matrix* m, const char *taskName, unsigned int priority);
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	int GetAmmo();	//���v�c�e�����擾
	int GetLoadedAmmo();
	bool CanShot();	//�e�����Ă邩
	void ReduceBullet();	//�e�����炷
	void AddAmmo(int addnum);	//�e�𑝂₷
	void Reload();		//�e�������[�h

protected:
	int m_Ammo;		//�莝���e��
	int m_LoadedAmmo;	//�e�ɓ����Ă���e��
	int* m_PlayerAnim;		//�v���C���[�̍Đ����A�j���[�V����
	float* m_PlayerAnimFrame;	//�v���C���[�̃A�j���[�V�����t���[��
	StaticMesh m_Model;
	Matrix* m_BoneMtx;		//�v���C���[�̃{�[���s��
	RenderTask m_RenderTask;
};

#endif