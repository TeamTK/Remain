#include "PointLight.h"
#include "PointLightManager.h"

PointLight::PointLight() :
	m_IsForward(false),
	m_Range(1.0f),
	m_Intensity(1.0f),
	m_Color(1.0f, 1.0f, 1.0f)
{
	//PointLightManager::GetInstance()->DeferredAdd(this);
	PointLightManager::GetInstance()->ForwardAdd(this);
}

PointLight::PointLight(bool isForward) :
	m_IsForward(isForward),
	m_Range(1.0f),
	m_Intensity(1.0f),
	m_Color(1.0f, 1.0f, 1.0f)
{
	if (m_IsForward)
	{
		PointLightManager::GetInstance()->ForwardAdd(this);
	}
	else
	{
		PointLightManager::GetInstance()->DeferredAdd(this);
	}
}

PointLight::~PointLight()
{
	if (m_IsForward)
	{
		PointLightManager::GetInstance()->ForwardClear(this);
	}
	else
	{
		PointLightManager::GetInstance()->DeferredClear(this);
	}
}

const Vector3D PointLight::GetPosition() const
{
	return m_Pos;
}

const Vector3D PointLight::GetColor() const
{
	return m_Color;
}

const float PointLight::GetRange() const
{
	return m_Range;
}

const float PointLight::GetIntensity() const
{
	return m_Intensity;
}

void PointLight::SetRenderingChange(bool isForward)
{
	m_IsForward = isForward;
	if (m_IsForward)
	{
		PointLightManager::GetInstance()->DeferredClear(this);
		PointLightManager::GetInstance()->ForwardAdd(this);
	}
	else
	{
		PointLightManager::GetInstance()->ForwardClear(this);
		PointLightManager::GetInstance()->DeferredAdd(this);
	}
}

void PointLight::SetPosition(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}

void PointLight::SetPosition(const Vector3D &pos)
{
	m_Pos = pos;
}

void PointLight::SetColor(int r, int g, int b)
{
	m_Color.x = (float)r * RGB;
	m_Color.y = (float)g * RGB;
	m_Color.z = (float)b * RGB;
}

void PointLight::SetRange(float range)
{
	if (range < 0.0f) return;
	m_Range = range;
}

void PointLight::SetIntensity(float intensity)
{
	m_Intensity = intensity;
}

void PointLight::DebugDraw() const
{
	if (m_IsForward) m_Pos.DebugDraw("Forward");
	else			 m_Pos.DebugDraw("Deferred");
	printf("Range %f\n", m_Range);
}