#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "../GEKO/GEKO.h"
#include "enums.h"

class WeaponBase
{
public:
	WeaponBase();
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	void LineSegment_vs_CapsuleCallback();
	void SetPlayerData(int anim, int frame, Vector3D start, Vector3D end);
	void SetPlayerBomeMtx(Matrix* m);
	Matrix GetMatrix();		//�e�̍s��擾

protected:
	int m_Ammo;		//�e��
	StaticMesh m_Model;
	Collider m_Collider;
	Matrix m_Matrix;	//�e�̍s��

	int m_PlayerAnim;		//�v���C���[�̍Đ����A�j���[�V����
	int m_PlayerAnimFrame;	//�v���C���[�̃A�j���[�V�����t���[��
	Matrix* m_BoneMtx;		//�v���C���[�̃{�[���s��
	Vector3D m_Start;	//�����̓����蔻��p �n�_
	Vector3D m_End;		//�����̓����蔻��p �I�_
};

#endif