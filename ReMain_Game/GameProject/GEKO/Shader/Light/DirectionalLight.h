#ifndef _DIRECTIONAL_LIGHT_
#define _DIRECTIONAL_LIGHT_

class Vector3D;
class Vector4D;

class DirectionalLight
{
public:
	~DirectionalLight();
	static const Vector4D* GetDirection();
	static Vector4D* GetLightColor();
	static void SetDirection(float x, float y);
	static void SetIntensity(float instensity);
	static void SetColor(int r, int g, int b);
	static void SetResolution(float width, float height);
	static void SetLookat(float x, float y, float z);
	static void SetLookat(const Vector3D &lookatPt);
	static void SetNearFar(float nearZ, float farZ);
	static void SetViewAngle(float viewAngle);
	static void SetDistance(float distance);

private:
	DirectionalLight();
	static DirectionalLight* GetInstance();

private:
	struct LightPimpl;
	LightPimpl *m_pLightPimpl;
};

#endif