#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Direct3D11.h"

class Camera
{
public:
	~Camera();

	/**
	* @brief �r���[�s����擾
	* @return D3DXMATRIX�^�̃r���[�s��̃|�C���^
	*/
	const static D3DXMATRIX* GetView();

	/**
	* @brief �ˉe�s����擾
	* @return D3DXMATRIX�^�̎ˉe�s��̃|�C���^
	*/
	const static D3DXMATRIX* GetProjection();

	/**
	* @brief �r���[�|�[�g�s����擾
	* @return D3DXMATRIX�^�̃r���[�|�[�g�s��̃|�C���^
	*/
	const static D3DXMATRIX* GetViewport();

	/**
	* @brief �J�����̈ʒu���擾
	* @return D3DXVECTOR3�^�̃J�����ʒu
	*/
	const static D3DXVECTOR3 GetEyePositionD3D();

	/**
	* @brief �J�����̈ʒu���擾
	* @return Vector3D�^�̃J�����ʒu
	*/
	const static Vector3D GetEyePosition();

	/**
	* @brief �J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ�
	* @param[in] ��O�N���b�v����
	* @param[in] ���N���b�v����
	*/
	static void SetNearFar(float Near, float Far);

	/**
	* @brief �J�����̈ʒu��ݒ�
	* @param[in] X���W
	* @param[in] Y���W
	* @param[in] Z���W
	*/
	static void	SetEye(float x, float y, float z);

	static void	SetEye(const Vector3D &eyePos);

	/**
	* @brief �J�����̒����_��ݒ�
	* @param[in] X���W
	* @param[in] Y���W
	* @param[in] Z���W
	*/
	static void	SetLookat(float x, float y, float z);

	static void	SetLookat(const Vector3D &look);

	/**
	* @brief �J�����̏������ݒ�
	* @param[in] X���W
	* @param[in] Y���W
	* @param[in] Z���W
	*/
	static void	SetUpVec(float x, float y, float z);

	static void	SetUpVec(const Vector3D &upVec);

	/**
	* @brief �J��������ʂɕ`�悷��͈͂ƈʒu��ݒ�
	* @param[in] ��
	* @param[in] ����
	* @param[in] X���W
	* @param[in] Y���W
	*/
	static void SetDrawArea(float width, float height, float x, float y);

	/**
	* @brief �J�����̍X�V�i���C�u���������Ŏg���Ă��܂��j
	*/
	static void Update();

	static void SpaceViewTPS(const Vector3D &lookPos);

private:
	Camera();
	static Camera* const GetInstance();

private:
	D3DXMATRIX m_View; 
	D3DXMATRIX m_Proj; //�ˉe�s��
	D3DXMATRIX m_Viewport;
	D3DXVECTOR3 m_EyePt;		//�J�����i���_�j�ʒu
	D3DXVECTOR3 m_LookatPt;		//�����ʒu
	D3DXVECTOR3 m_UpVec;		//����ʒu
	float m_Near;
	float m_Far;
	float m_Vertical;
	float m_Horizontal;
};

#endif