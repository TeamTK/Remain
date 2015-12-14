#ifndef _DIRECTIONAL_LIGHT_
#define _DIRECTIONAL_LIGHT_

#include "Direct3D11.h"

class DirectionalLight
{
public:
	~DirectionalLight();
	static const D3DXVECTOR4* GetDirection();
	static Vector4D* GetLightColor();
	static void SetDirection(float x, float y);
	static void SetIntensity(float instensity);
	static void SetColor(int r, int g, int b);

private:
	DirectionalLight();
	static DirectionalLight* GetInstance();

private:
	D3DXVECTOR4 m_Direction;
	Vector4D m_LightColor;
};

#endif