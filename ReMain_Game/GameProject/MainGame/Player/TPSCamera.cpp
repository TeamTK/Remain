#include "TPSCamera.h"
#include "Player.h"

#define CAMERA_NO_CROUCH_POS_Y 1.8f;  //���Ⴊ�ݎp������Ȃ��Ƃ��̃J������Y���W�̍���
#define CAMERA_HIT_HIGHT 1.5f;		  //�J�����̓����蔻��̍���
#define CAMERA_LENGE	2.0f		  //�v���C���[�ƃJ�����̋���

TPSCamera::TPSCamera(CameraInfo *pCameraInfo, float horizontal, float vertical) :
	Task("TPSCamera", 1),
	m_Horizontal(Math::ChangeToRadian(horizontal)),
	m_Vertical(Math::ChangeToRadian(vertical)),
	m_CamSpeed(0.000002f),
	m_CameraPos(0.0f, 0.0f, 0.0f),
	m_pCameraInfo(pCameraInfo)
{
	//�J�����̓����蔻��
	m_HitCamera.Regist_L_vs_SMesh(&m_CameraPos, &m_HitCameraPos, REGIST_FUNC(TPSCamera::HitCamera));
	m_HitCamera.SetID(eHITID0, eHITID1);

	//�J�����̓����蔻��i�����j
	m_HitCameraGroup.Regist_LGroup_vs_SMesh(&m_CameraPos, &m_HitCameraPos, REGIST_FUNC(TPSCamera::HitCamera_Group));
	m_HitCameraGroup.SetID(eHITID2, eHITID3);

	Camera::SetEye(m_CameraPos);
}

TPSCamera::~TPSCamera()
{
}

void TPSCamera::Update()
{
	auto *info = m_pCameraInfo;

	Vector3D OldCameraPos = m_CameraPos;
	Vector3D OldLookPos = *info->pLookPos;
	Point mouseValue = Input::Mouse.GetRelativeValue();

	if (*info->pIsStop) return;

	//�J�����ړ�
	if (Input::XInputPad1.GetIsConnection())
	{
		//�R���g���[���[����
		Vector3D PadDir = Vector3D((float)Input::XInputPad1.ThumbRightX(), 0.0f, (float)Input::XInputPad1.ThumbRightY());
		PadDir = (PadDir - Vector3D(128, 0.0f, 128)) / 32767;
		if (abs(PadDir.x) < DeadValue && abs(PadDir.z) < DeadValue)
		{
			PadDir = Vector3D(0.0f, 0.0f, 0.0f);
		}

		m_Horizontal += PadDir.x * m_CamSpeed;
		m_Vertical += PadDir.z * m_CamSpeed;
	}
	else
	{
		//�}�E�X����
		m_Horizontal += mouseValue.x * 0.002f;
		m_Vertical += -mouseValue.y * 0.002f;
	}

	Matrix mRX, mRY, mat;
	mRY.RotationRadianY(m_Horizontal);
	mRX.RotationRadianX(m_Vertical);
	mat = mRY * mRX;

	//�e���\���Ă���Ƃ��̃J�����ʒu
	if (*info->pSetupWeapon && (*info->pState != EPlayerState::eState_Reload))
	{
		m_CamSpeed = 0.03f;
		if (m_Vertical >= 0.55f) m_Vertical = 0.55f;	//�J�����p�x���
		if (m_Vertical <= -0.5f) m_Vertical = -0.5f;	//�J�����p�x����

		info->pRot->x = -m_Vertical;
		info->pRot->y = m_Horizontal;

		info->pModel->SetTranselate(*info->pPos);
		info->pModel->SetRotationRadian(info->pRot->x, info->pRot->y, info->pRot->z);

		Matrix mat = info->pModel->GetBoneMatrix(6, true);
		Vector4D eye = Vector4D(-0.25f, 0.15f, -0.2f, 1.0f) * mat;
		Vector4D at = Vector4D(-0.25f, 0.2f, 0.2f, 1.0f) *  mat;
		m_CameraPos = Vector3D(eye.x, eye.y, eye.z);
		*info->pLookPos = Vector3D(at.x, at.y, at.z);

		//�e�\�����ɂ͓����蔻���~
		m_HitCamera.Sleep();
		m_HitCameraGroup.Sleep();
	}
	//�ʏ��Ԃ̃J�����ʒu
	else
	{
		m_CamSpeed = 0.06f;
		if (m_Vertical >= 0.45f) m_Vertical = 0.45f;	//�J�����p�x����
		if (m_Vertical <= -0.9f) m_Vertical = -0.9f;	//�J�����p�x���

		info->pRot->x = 0.0f;
		*info->pLookPos = info->pModel->GetBonePos(6) + mat.GetAxisX() * 0.4f;

		//�J�����̍��W
		m_CameraPos = *info->pLookPos;
		m_CameraPos -= mat.GetAxisZ() * CAMERA_LENGE;

		//�e���\���Ȃ����ɂ͓����蔻��
		m_HitCamera.Awake();
		m_HitCameraGroup.Awake();
	}

	//�J�����⊮�ړ�
	if (info->pSetupWeapon)
	{
		//������\���Ă���
		m_CameraPos = Vector3D::Lerp(OldCameraPos, m_CameraPos, 0.8f);
		*info->pLookPos = Vector3D::Lerp(OldLookPos, *info->pLookPos, 0.8f);
	}
	else
	{
		//������\���Ă��Ȃ�
		m_CameraPos = Vector3D::Lerp(OldCameraPos, m_CameraPos, 0.4f);
		*info->pLookPos = Vector3D::Lerp(OldLookPos, *info->pLookPos, 0.4f);
	}

	//�J�����̓����蔻��ʒu
	m_HitCameraPos = *info->pPos;
	m_HitCameraPos.y += CAMERA_HIT_HIGHT;

	Camera::SetEye(m_CameraPos);
	Camera::SetLookat(*info->pLookPos);
}

void TPSCamera::HitCamera(Result_Porygon &hitData)
{
	Camera::SetEye(hitData.contactPos * hitData.meshMatrix);
}

void TPSCamera::HitCamera_Group(Result_Porygon_Group_LineSegment &hitData)
{
	Vector3D pos = hitData.pArray[0].contactPos * hitData.meshMatrix;
	Vector3D pos2 = m_pCameraInfo->pModel->GetBonePos(6);
	Vector3D temp;
	float dist = 0.0f;
	float distPrev = 0.0f;
	for (int i = 0; i < hitData.hitNum; i++)
	{
		temp = hitData.pArray[i].contactPos * hitData.meshMatrix;
		dist = (temp - pos2).LengthSq();
		if (dist < distPrev)
		{
			pos = temp;
		}
		distPrev = dist;
	}
	Camera::SetEye(pos);
}
