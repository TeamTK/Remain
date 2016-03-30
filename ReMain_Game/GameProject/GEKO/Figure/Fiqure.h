#ifndef _FIQURE_H_
#define _FIQURE_H_

#include "..\System\Math.h"

class Math;

class Fiqure
{
public:
	~Fiqure();
	static void RenderLine3D(const Vector3D &Spos, const Vector3D &Epos, const Vector3D &color);
	static void RenderTriangle3D(const Vector3D &pos1, const Vector3D &pos2, const Vector3D &pos3, const Vector3D &color);
	static void RenderSphere3D(const Vector3D &pos, const Vector3D &scale, const Vector3D &color);

private:
	Fiqure();
	static Fiqure* GetInstance();

private:
	class FiqurePimpl;
	FiqurePimpl *m_Fiqure;
};


#endif