#ifndef _POINT_LIGHT_MANAGER_H_
#define _POINT_LIGHT_MANAGER_H_

#include "PointLight.h"

class PointLightManager
{
public:
	~PointLightManager();
	static PointLightManager* GetInstance();
	const int GetForwardLightNum() const;
	const int GetDeferredLightNum() const;
	const PointLight* GetForwardLight(int arrayNum) const;
	const PointLight* GetDeferreLight(int arrayNum) const;
	void ForwardAdd(PointLight *pPointLight);
	void DeferredAdd(PointLight *pPointLight);
	void ForwardClear(PointLight *pPointLight);
	void DeferredClear(PointLight *pPointLight);
	void Release();

private:
	PointLightManager();

private:
	struct LightPimpl;
	LightPimpl *m_pLightPimpl;
};

#endif