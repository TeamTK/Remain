#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Direct3D11.h"

class Camera
{
public:
	~Camera();

	/**
	* @brief ビュー行列を取得
	* @return D3DXMATRIX型のビュー行列のポインタ
	*/
	const static D3DXMATRIX* GetView();

	/**
	* @brief 射影行列を取得
	* @return D3DXMATRIX型の射影行列のポインタ
	*/
	const static D3DXMATRIX* GetProjection();

	/**
	* @brief ビューポート行列を取得
	* @return D3DXMATRIX型のビューポート行列のポインタ
	*/
	const static D3DXMATRIX* GetViewport();

	/**
	* @brief カメラの位置を取得
	* @return D3DXVECTOR3型のカメラ位置
	*/
	const static D3DXVECTOR3 GetEyePositionD3D();

	/**
	* @brief カメラの位置を取得
	* @return Vector3D型のカメラ位置
	*/
	const static Vector3D GetEyePosition();

	/**
	* @brief カメラの手前クリップ距離と奥クリップ距離を設定
	* @param[in] 手前クリップ距離
	* @param[in] 奥クリップ距離
	*/
	static void SetNearFar(float Near, float Far);

	/**
	* @brief カメラの位置を設定
	* @param[in] X座標
	* @param[in] Y座標
	* @param[in] Z座標
	*/
	static void	SetEye(float x, float y, float z);

	static void	SetEye(const Vector3D &eyePos);

	/**
	* @brief カメラの注視点を設定
	* @param[in] X座標
	* @param[in] Y座標
	* @param[in] Z座標
	*/
	static void	SetLookat(float x, float y, float z);

	static void	SetLookat(const Vector3D &look);

	/**
	* @brief カメラの上方向を設定
	* @param[in] X座標
	* @param[in] Y座標
	* @param[in] Z座標
	*/
	static void	SetUpVec(float x, float y, float z);

	static void	SetUpVec(const Vector3D &upVec);

	/**
	* @brief カメラが画面に描画する範囲と位置を設定
	* @param[in] 幅
	* @param[in] 高さ
	* @param[in] X座標
	* @param[in] Y座標
	*/
	static void SetDrawArea(float width, float height, float x, float y);

	/**
	* @brief カメラの更新（ライブラリ内部で使っています）
	*/
	static void Update();

	static void SpaceViewTPS(const Vector3D &lookPos);

private:
	Camera();
	static Camera* const GetInstance();

private:
	D3DXMATRIX m_View; 
	D3DXMATRIX m_Proj; //射影行列
	D3DXMATRIX m_Viewport;
	D3DXVECTOR3 m_EyePt;		//カメラ（視点）位置
	D3DXVECTOR3 m_LookatPt;		//注視位置
	D3DXVECTOR3 m_UpVec;		//上方位置
	float m_Near;
	float m_Far;
	float m_Vertical;
	float m_Horizontal;
};

#endif