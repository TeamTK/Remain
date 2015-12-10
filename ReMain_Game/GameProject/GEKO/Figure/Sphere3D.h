#ifndef _SPHERE3D_H_
#define _SPHERE3D_H_

#include "FiqureBase.h"

class Sphere3D : public FiqureBase
{
public:
	Sphere3D();
	~Sphere3D();
	void Render(const Matrix &World, const Vector3D &Color);

private:
	HRESULT InitVertex();

private:
	int m_CornerNum;
};

#endif