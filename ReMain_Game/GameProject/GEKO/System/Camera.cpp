#include "Camera.h"
#include "Window.h"
#include <iostream>
#include "Input.h"

Camera::Camera() : 
	m_EyePt(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_LookatPt(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_UpVec(D3DXVECTOR3(0.0f, 1.0f, 0.0f)),
	m_Near(0.1f),
	m_Far(1000.0f),
	m_ViewAngle(Math::ChangeToRadian(45)),
	m_Vertical(1.0f),
	m_Horizontal(1.0f)
{
}

Camera::~Camera()
{
}

Camera* Camera::GetInstance()
{
	static Camera camera;
	return &camera;
}

const D3DXMATRIX* Camera::GetView()
{
	return &GetInstance()->m_View;
}

const D3DXMATRIX* Camera::GetProjection()
{
	return &GetInstance()->m_Proj;
}

const D3DXMATRIX* Camera::GetViewport()
{
	return &GetInstance()->m_Viewport;
}

const D3DXVECTOR3 Camera::GetEyePositionD3D()
{
	return GetInstance()->m_EyePt;
}

const Vector3D Camera::GetEyePosition()
{
	return GetInstance()->m_EyePt;
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
	GetInstance()->m_EyePt = D3DXVECTOR3(x, y, z);
}

void Camera::SetEye(const Vector3D &eyePos)
{
	GetInstance()->m_EyePt = D3DXVECTOR3(eyePos.x, eyePos.y, eyePos.z);
}

void Camera::SetLookat(float x, float y, float z)
{
	GetInstance()->m_LookatPt = D3DXVECTOR3(x, y, z);
}

void Camera::SetLookat(const Vector3D &look)
{
	GetInstance()->m_LookatPt = D3DXVECTOR3(look.x, look.y, look.z);
}

void Camera::SetUpVec(float x, float y, float z)
{
	GetInstance()->m_UpVec = D3DXVECTOR3(x, y, z);
}

void Camera::SetUpVec(const Vector3D &upVec)
{
	GetInstance()->m_UpVec = D3DXVECTOR3(upVec.x, upVec.y, upVec.z);
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
	WindowSize *winSize = Window::Get()->GetWindowSize();
	FLOAT Width = (FLOAT)winSize->sWidth;
	FLOAT Height = (FLOAT)winSize->sHeight;

	// 希望するクライアント領域のサイズを持つウィンドウサイズを計算
	D3DXMatrixPerspectiveFovLH(&pInstance->m_Proj, (FLOAT)pInstance->m_ViewAngle, Width / Height, (FLOAT)pInstance->m_Near, (FLOAT)pInstance->m_Far);

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