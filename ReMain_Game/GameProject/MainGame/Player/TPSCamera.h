#ifndef _TPS_CAMERA_H_
#define _TPS_CAMERA_H_

#include "PlayerInfo.h"

class Player;

class TPSCamera : public Task
{
public:
	TPSCamera(CameraInfo *pCameraInfo);
	~TPSCamera();
	void Update();

private:
	void HitCamera(Result_Porygon &hitData);
	void HitCamera_Group(Result_Porygon_Group_LineSegment &hitData);

private:
	float m_Vertical;			//���������̃}�E�X���͗�
	float m_Horizontal;		//���������̃}�E�X���͗�
	float m_CamSpeed;			//�J�����̉�]���x

	Vector3D m_CameraPos;	    //�J�����̈ʒu
	Vector3D m_HitCameraPos;  //�J�����̓����蔻��ʒu
	Collider m_HitCamera;	    //�J�����̓����蔻��
	Collider m_HitCameraGroup;//�J�����̓����蔻��i�����̌��ʂ��Ԃ��Ă���j

	CameraInfo *m_pCameraInfo;
};

#endif