#pragma once

#include "../GEKO/GEKO.h"
//#include "../GEKO/Collider/ColliderManager.h"
//#include "../GEKO/Collider/LineSegment.h"

class CBullet  {
public:
	bool m_isActive;
//	LineSegment_vs_CapsuleCollider m_LineSegmentCollider;
	Vector3D m_pos;			//�ʒu
	Vector3D m_Oldpos;		//�ړ��O�ʒu�i��������p�j
	int   m_cnt;			//�����p�J�E���^�[
	Vector3D m_dir;			//����
	float m_speed;		//����
	StaticMesh m_Model;	//���f��
	float m_Atk;		//�U����
	CBullet();
	~CBullet();
	bool UpDate();
	void Render();
//	void LineSegment_vs_CapsuleCallback(Result_Capsule& r);
};


class CBulletManager {
public:
	static CBulletManager *m_Obj;
	std::vector<CBullet*> m_Core;
	/*!
		@brief	�R���X�g���N�^

	**/
	CBulletManager();
	~CBulletManager();
	/*!
		@brief�@�}�l�[�W���[�̎擾

	**/

	static CBulletManager* GetInstance() {
		if (!m_Obj) m_Obj = new CBulletManager();
		return m_Obj;

	}
	static void ClearInstance() {
		delete m_Obj;
		m_Obj = NULL;
	}
	/*!
		@brief	�X�V

	**/
	void UpDate();
	/*!
		@brief	�`��

	**/
	void Render();

//	void CollisionMap(StaticMesh *m);
	/*!
		@brief	�e�̐���
		@param	pos				[in] ���W
		@param	vec				[in] �ړ���
		@retval	���������e�I�u�W�F�N�g

	**/
	CBullet *Add(const Vector3D pos, const Vector3D &dir,const float speed);

};