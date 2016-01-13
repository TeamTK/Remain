#ifndef _EFFECTPARABOLA_H_
#define _EFFECTPARABOLA_H_

#include "Effect.h"

class EffectParabola : public Effect
{
public:
	EffectParabola(const EffectInfo &info, const char* effectName, const Vector3D &normal);
	~EffectParabola();
	void Update();

private:

};

#endif