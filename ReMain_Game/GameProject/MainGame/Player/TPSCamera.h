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
	float m_Vertical;			//垂直方向のマウス入力量
	float m_Horizontal;		//水平方向のマウス入力量
	float m_CamSpeed;			//カメラの回転速度

	Vector3D m_CameraPos;	    //カメラの位置
	Vector3D m_HitCameraPos;  //カメラの当たり判定位置
	Collider m_HitCamera;	    //カメラの当たり判定
	Collider m_HitCameraGroup;//カメラの当たり判定（複数の結果が返ってくる）

	CameraInfo *m_pCameraInfo;
};

#endif