#include "DirectionalLight.h"
#include "../Shader/ShadowMap/ShaderShadowMap.h"

DirectionalLight::DirectionalLight() :
	m_Direction(0.0f, -1.0f, 0.0f, 0.0f),
	m_LightColor(1.0f, 1.0f, 1.0f, 1.0f)
{
}

DirectionalLight::~DirectionalLight()
{
}

DirectionalLight* DirectionalLight::GetInstance()
{
	static DirectionalLight directionalLight;
	return &directionalLight;
}

const Vector4D* DirectionalLight::GetDirection()
{
	return &GetInstance()->m_Direction;
}

Vector4D *DirectionalLight::GetLightColor()
{
	return &GetInstance()->m_LightColor;
}

void DirectionalLight::SetDirection(float x, float y)
{
	//D3DXMATRIX RotX, RotZ;
	//D3DXMatrixRotationX(&RotX, Math::ChangeToRadian(x));
	//D3DXMatrixRotationZ(&RotZ, Math::ChangeToRadian(z));

	//D3DXMATRIX all = RotX * RotZ;
	//D3DXVec4Transform(&GetInstance()->m_Direction, &D3DXVECTOR4(0, 1, 0, 0), &all);

	Matrix RX, RY;
	RX.RotationDegreeX(x);
	RY.RotationDegreeZ(y);

	Vector3D vec = Vector3D(0.0f, -1.0f, 0.0f) * (RX * RY);
	vec.SetNormalize();

	GetInstance()->m_Direction.x = vec.x;
	GetInstance()->m_Direction.y = vec.y;
	GetInstance()->m_Direction.z = vec.z;
}

void DirectionalLight::SetIntensity(float instensity)
{
	GetInstance()->m_LightColor.w = instensity;
}

void DirectionalLight::SetColor(int r, int g, int b)
{
	DirectionalLight *temp = GetInstance();
	temp->m_LightColor.x = (float)r * RGB;
	temp->m_LightColor.y = (float)g * RGB;
	temp->m_LightColor.z = (float)b * RGB;
}

void DirectionalLight::SetResolution(float width, float height)
{
	ShaderShadowMap::GetInstance()->SetResolution(width, height);
}

void DirectionalLight::SetLookat(float x, float y, float z)
{
	ShaderShadowMap::GetInstance()->SetLookat(x, y, z);
}

void DirectionalLight::SetLookat(const Vector3D &lookatPt)
{
	ShaderShadowMap::GetInstance()->SetLookat(lookatPt);
}

void DirectionalLight::SetNearFar(float nearZ, float farZ)
{
	ShaderShadowMap::GetInstance()->SetNearFar(nearZ, farZ);
}

void DirectionalLight::SetViewAngle(float viewAngle)
{
	ShaderShadowMap::GetInstance()->SetViewAngle(viewAngle);
}

void DirectionalLight::SetDistance(float distance)
{
	ShaderShadowMap::GetInstance()->SetDistance(distance);
}