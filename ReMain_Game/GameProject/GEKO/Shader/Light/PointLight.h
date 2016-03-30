#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "../../System/Math.h"

class Vector3D;
class PointLightManager;
struct PointLightInfo;

//後方レンダリングと前方レンダリングに対応した点光源切り替え可能
class PointLight
{
	friend PointLightManager;

public:
	PointLight();
	PointLight(bool isForward);
	~PointLight();
	const Vector3D GetPosition() const;
	const Vector3D GetColor() const;
	const float GetRange() const;
	const float GetIntensity() const;
	void SetRenderingChange(bool isForward);
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3D &pos);
	void SetColor(int r, int g, int b);
	void SetRange(float range);
	void SetIntensity(float intensity);
	void DebugDraw() const;

private:
	bool m_IsForward;
	float m_Range;
	float m_Intensity;
	Vector3D m_Pos;
	Vector3D m_Color;
};

#endif