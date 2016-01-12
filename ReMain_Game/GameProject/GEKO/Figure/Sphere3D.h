#ifndef _SPHERE3D_H_
#define _SPHERE3D_H_

#include "FiqureBase.h"

class Sphere3D : public FiqureBase
{
public:
	Sphere3D();
	~Sphere3D();
	void Render(const Vector3D &pos, const Vector3D &scale, const Vector3D &color);

private:
	HRESULT InitShader();
	HRESULT InitVertex();

private:

};

#endif