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
	* @brief ビュー行列を取得
	* @return Matrix型のビュー行列のポインタ
	*/
	const static Matrix* GetView();

	/**
	* @brief 射影行列を取得
	* @return Matrix型の射影行列のポインタ
	*/
	const static Matrix* GetProjection();

	/**
	* @brief ビュー行列 × 射影行列を取得
	* @return Matrix型のビュー行列 × 射影行列のポインタ
	*/
	const static Matrix* GetViewProjection();

	/**
	* @brief ビューポート行列を取得
	* @return Matrix型のビューポート行列のポインタ
	*/
	const static Matrix* GetViewport();

	/**
	* @brief カメラの位置を取得
	* @return Vector3D型のカメラ位置
	*/
	const static Vector3D GetEyePosition();

	/**
	* @brief カメラの注視点を取得
	* @return Vector3D型のカメラの注視点位置
	*/
	const static Vector3D GetLookAtPosition();

	/**
	* @brief カメラの手前クリップ距離と奥クリップ距離を設定
	* @param[in] 手前クリップ距離
	* @param[in] 奥クリップ距離
	*/
	static void SetNearFar(float Near, float Far);

	/**
	* @brief カメラの視野角を設定
	* @param[in] 視野角
	*/
	static void SetViewAngle(float viewAngle);

	/**
	* @brief カメラの位置を設定
	* @param[in] X座標
	* @param[in] Y座標
	* @param[in] Z座標
	*/
	static void	SetEye(float x, float y, float z);

	/**
	* @brief カメラの位置を設定(Vector3D版)
	* @param[in] XYX座標
	*/
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
	* @brief カメラの更新（ライブラリ内部で使っていますユーザーは使わないでください）
	*/
	static void Update();

	/**
	* @brief カメラをTPS視点にしてマウスで操作出来る
	* @param[in] 注視点位置
	*/
	static void SpaceViewTPS(const Vector3D &lookPos);

	/**
	* @brief 霧の色を変更
	* @param[in] 赤（0～255）
	* @param[in] 緑（0～255）
	* @param[in] 青（0～255）
	*/
	static void FogColor(int red, int green, int blue);

	/**
	* @brief カメラが画面に描画する範囲と位置を設定
	* @param[in] 霧の密度（0.0ｆ～1.0ｆ）
	*/
	static void FogDensity(float density);

private:
	Camera();
	static Camera* GetInstance();

private:
	Matrix m_View;		  //カメラ行列
	Matrix m_Proj;		  //射影行列
	Matrix m_ViewProj;	  //カメラビュー行列 × 射影行列
	Matrix m_Viewport;	  //ビューポート行列
	Vector3D m_EyePt;	  //視点位置
	Vector3D m_LookatPt;  //注視点位置
	Vector3D m_UpVec;	  //上方位置
	Vector4D m_FogColor;  //霧の色
	float m_Near;		  //カメラが描画する最小距離
	float m_Far;		  //カメラが描画する最大距離
	float m_ViewAngle;	  //視野角度（ラジアン）
	float m_Vertical;	  //カメラの垂直方向
	float m_Horizontal;	  //カメラの水平方向
	float m_FogDensity;   //霧の密度
};

#endif