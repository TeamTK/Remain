#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Direct3D11.h"

class ConstantShader;

class Camera
{
	friend ConstantShader;

public:
	~Camera();

	/**
	* @brief �r���[�s����擾
	* @return Matrix�^�̃r���[�s��̃|�C���^
	*/
	const static Matrix* GetView();

	/**
	* @brief �ˉe�s����擾
	* @return Matrix�^�̎ˉe�s��̃|�C���^
	*/
	const static Matrix* GetProjection();

	/**
	* @brief �r���[�s�� �~ �ˉe�s����擾
	* @return Matrix�^�̃r���[�s�� �~ �ˉe�s��̃|�C���^
	*/
	const static Matrix* GetViewProjection();

	/**
	* @brief �r���[�|�[�g�s����擾
	* @return Matrix�^�̃r���[�|�[�g�s��̃|�C���^
	*/
	const static Matrix* GetViewport();

	/**
	* @brief �J�����̈ʒu���擾
	* @return Vector3D�^�̃J�����ʒu
	*/
	const static Vector3D GetEyePosition();

	/**
	* @brief �J�����̒����_���擾
	* @return Vector3D�^�̃J�����̒����_�ʒu
	*/
	const static Vector3D GetLookAtPosition();

	/**
	* @brief �J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ�
	* @param[in] ��O�N���b�v����
	* @param[in] ���N���b�v����
	*/
	static void SetNearFar(float Near, float Far);

	/**
	* @brief �J�����̎���p��ݒ�
	* @param[in] ����p
	*/
	static void SetViewAngle(float viewAngle);

	/**
	* @brief �J�����̈ʒu��ݒ�
	* @param[in] X���W
	* @param[in] Y���W
	* @param[in] Z���W
	*/
	static void	SetEye(float x, float y, float z);

	/**
	* @brief �J�����̈ʒu��ݒ�(Vector3D��)
	* @param[in] XYX���W
	*/
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
	* @brief �J�����̍X�V�i���C�u���������Ŏg���Ă��܂����[�U�[�͎g��Ȃ��ł��������j
	*/
	static void Update();

	/**
	* @brief �J������TPS���_�ɂ��ă}�E�X�ő���o����
	* @param[in] �����_�ʒu
	*/
	static void SpaceViewTPS(const Vector3D &lookPos);

	/**
	* @brief ���̐F��ύX
	* @param[in] �ԁi0�`255�j
	* @param[in] �΁i0�`255�j
	* @param[in] �i0�`255�j
	*/
	static void FogColor(int red, int green, int blue);

	/**
	* @brief �J��������ʂɕ`�悷��͈͂ƈʒu��ݒ�
	* @param[in] ���̖��x�i0.0���`1.0���j
	*/
	static void FogDensity(float density);

private:
	Camera();
	static Camera* GetInstance();

private:
	Matrix m_View;		  //�J�����s��
	Matrix m_Proj;		  //�ˉe�s��
	Matrix m_ViewProj;	  //�J�����r���[�s�� �~ �ˉe�s��
	Matrix m_Viewport;	  //�r���[�|�[�g�s��
	Vector3D m_EyePt;	  //���_�ʒu
	Vector3D m_LookatPt;  //�����_�ʒu
	Vector3D m_UpVec;	  //����ʒu
	Vector4D m_FogColor;  //���̐F
	float m_Near;		  //�J�������`�悷��ŏ�����
	float m_Far;		  //�J�������`�悷��ő勗��
	float m_ViewAngle;	  //����p�x�i���W�A���j
	float m_Vertical;	  //�J�����̐�������
	float m_Horizontal;	  //�J�����̐�������
	float m_FogDensity;   //���̖��x
};

#endif