#include "DirectionalLight.h"
#include "../ShadowMap/ShaderShadowMap.h"
#include "../../System/Math.h"

struct DirectionalLight::LightPimpl
{
	Vector4D direction;
	Vector4D lightColor;
};

DirectionalLight::DirectionalLight()
{
	m_pLightPimpl = new LightPimpl;
	m_pLightPimpl->direction = Vector4D(0.0f, -1.0f, 0.0f, 0.0f);
	m_pLightPimpl->lightColor = Vector4D(1.0f, 1.0f, 1.0f, 1.0f);
}

DirectionalLight::~DirectionalLight()
{
	delete m_pLightPimpl;
}

DirectionalLight* DirectionalLight::GetInstance()
{
	static DirectionalLight directionalLight;
	return &directionalLight;
}

const Vector4D* DirectionalLight::GetDirection()
{
	return &GetInstance()->m_pLightPimpl->direction;
}

Vector4D *DirectionalLight::GetLightColor()
{
	return &GetInstance()->m_pLightPimpl->lightColor;
}

void DirectionalLight::SetDirection(float x, float y)
{
	Matrix RX, RY;
	RX.RotationDegreeX(x);
	RY.RotationDegreeZ(y);

	Vector3D vec = Vector3D(0.0f, -1.0f, 0.0f) * (RX * RY);
	vec.SetNormalize();

	GetInstance()->m_pLightPimpl->direction.x = vec.x;
	GetInstance()->m_pLightPimpl->direction.y = vec.y;
	GetInstance()->m_pLightPimpl->direction.z = vec.z;
}

void DirectionalLight::SetIntensity(float instensity)
{
	GetInstance()->m_pLightPimpl->lightColor.w = instensity;
}

void DirectionalLight::SetColor(int r, int g, int b)
{
	DirectionalLight *temp = GetInstance();
	temp->m_pLightPimpl->lightColor.x = (float)r * RGB;
	temp->m_pLightPimpl->lightColor.y = (float)g * RGB;
	temp->m_pLightPimpl->lightColor.z = (float)b * RGB;
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