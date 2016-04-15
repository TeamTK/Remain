#include "Camera.h"
#include "Window.h"
#include <iostream>
#include "Input.h"

Camera::Camera() : 
	m_EyePt(),
	m_LookatPt(),
	m_UpVec(0.0f, 1.0f, 0.0f),
	m_Near(0.1f),
	m_Far(1000.0f),
	m_ViewAngle(Math::ChangeToRadian(45)),
	m_Vertical(1.0f),
	m_Horizontal(1.0f),
	m_FogDensity(0.1f)
{
	m_FogColor.x = 1.0f;
	m_FogColor.y = 1.0f;
	m_FogColor.z = 1.0f;
}

Camera::~Camera()
{
}

Camera* Camera::GetInstance()
{
	static Camera camera;
	return &camera;
}

const Matrix* Camera::GetView()
{
	return &GetInstance()->m_View;
}

const Matrix* Camera::GetProjection()
{
	return &GetInstance()->m_Proj;
}

const Matrix* Camera::GetViewProjection()
{
	return &GetInstance()->m_ViewProj;
}

const Matrix* Camera::GetViewport()
{
	return &GetInstance()->m_Viewport;
}

const Vector3D Camera::GetEyePosition()
{
	return GetInstance()->m_EyePt;
}

const Vector3D Camera::GetLookAtPosition()
{
	return GetInstance()->m_LookatPt;
}

void Camera::SetNearFar(float Near, float Far)
{
	Camera* pInstance = GetInstance();

	pInstance->m_Near = Near;
	pInstance->m_Far = Far;
}

void Camera::SetViewAngle(float viewAngle)
{
	GetInstance()->m_ViewAngle = Math::ChangeToRadian(viewAngle);
}

void Camera::SetEye(float x, float y, float z)
{
	GetInstance()->m_EyePt.x = x;
	GetInstance()->m_EyePt.y = y;
	GetInstance()->m_EyePt.z = z;
}

void Camera::SetEye(const Vector3D &eyePos)
{
	GetInstance()->m_EyePt = eyePos;
}

void Camera::SetLookat(float x, float y, float z)
{
	GetInstance()->m_LookatPt.x = x;
	GetInstance()->m_LookatPt.y = y;
	GetInstance()->m_LookatPt.z = z;
}

void Camera::SetLookat(const Vector3D &look)
{
	GetInstance()->m_LookatPt = look;
}

void Camera::SetUpVec(float x, float y, float z)
{
	GetInstance()->m_UpVec.x = x;
	GetInstance()->m_UpVec.y = y;
	GetInstance()->m_UpVec.z = z;
}

void Camera::SetUpVec(const Vector3D &upVec)
{
	GetInstance()->m_UpVec = upVec;
}

void Camera::SetDrawArea(float width, float height, float x, float y)
{
	Direct3D11::GetInstance()->SetViewport(width, height, x, y);
}

void Camera::Update()
{
	Camera* pInstance = GetInstance();

	//ビュー更新
	D3DXMatrixLookAtLH(&pInstance->m_View, &pInstance->m_EyePt, &pInstance->m_LookatPt, &pInstance->m_UpVec);

	//射影行列更新
	const WindowSize *winSize = Window::Get()->GetWindowSize();
	FLOAT Width = (FLOAT)winSize->width;
	FLOAT Height = (FLOAT)winSize->height;

	// 希望するクライアント領域のサイズを持つウィンドウサイズを計算
	D3DXMatrixPerspectiveFovLH(&pInstance->m_Proj, (FLOAT)pInstance->m_ViewAngle, Width / Height, (FLOAT)pInstance->m_Near, (FLOAT)pInstance->m_Far);

	/*
	static float w2 = 0;
	static float h2 = 0;

	if (Input::KeyZ.Pressed()) w2--;
	if (Input::KeyX.Pressed()) w2++;

	if (Input::KeyA.Pressed()) h2--;
	if (Input::KeyS.Pressed()) h2++;

	D3DXMatrixOrthoLH(&pInstance->m_Proj, w2, h2, (FLOAT)pInstance->m_Near, (FLOAT)pInstance->m_Far);
	*/

	pInstance->m_ViewProj = pInstance->m_View * pInstance->m_Proj;

	//ビューポート更新
	D3D11_VIEWPORT *viewport = Direct3D11::GetInstance()->GetViewportD3D11();

	float w = viewport->Width / 2;
	float h = viewport->Height / 2;

	pInstance->m_Viewport._11 = w;
	pInstance->m_Viewport._22 = -h;
	pInstance->m_Viewport._33 = viewport->MaxDepth - viewport->MinDepth;
	pInstance->m_Viewport._41 = viewport->TopLeftX + w;
	pInstance->m_Viewport._42 = h + viewport->TopLeftY;
	pInstance->m_Viewport._43 = viewport->MinDepth;
	pInstance->m_Viewport._44 = 1.0f;
}

void Camera::SpaceViewTPS(const Vector3D &lookPos)
{
	Camera* pInstance = GetInstance();
	Point mouseValue = Input::Mouse.GetRelativeValue();

	if (Input::Mouse.RPressed())
	{
		//移動分で計算
		pInstance->m_Vertical -= mouseValue.x * 0.002f;
		pInstance->m_Horizontal -= mouseValue.y * 0.002f;
	}

	static float lenge = 30.0f;

	lenge -= Input::Mouse.GetWheelAmount() * 0.2f;

	Vector3D pos = lookPos;
	pos.x += lenge * sinf(pInstance->m_Horizontal) * cosf(pInstance->m_Vertical);
	pos.y += lenge * cosf(pInstance->m_Horizontal);
	pos.z += lenge * sinf(pInstance->m_Horizontal) * sinf(pInstance->m_Vertical);

	SetEye(pos);
	SetLookat(lookPos);
}

void Camera::FogColor(int red, int green, int blue)
{
	Camera* pInstance = GetInstance();

	pInstance->m_FogColor.x = red * RGB;
	pInstance->m_FogColor.y = green * RGB;
	pInstance->m_FogColor.z = blue * RGB;
}

void Camera::FogDensity(float density)
{
	GetInstance()->m_FogDensity = density;
}