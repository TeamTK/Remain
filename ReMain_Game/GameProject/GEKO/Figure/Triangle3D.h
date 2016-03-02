#ifndef _TRIANGLE3D_H_
#define _TRIANGLE3D_H_

#include "FiqureBase.h"

struct ConstantBufferTriangle3D
{
	Vector4D pos[3];
};

class Triangle3D : public FiqureBase
{
public:
	Triangle3D();
	~Triangle3D();
	void Render(const Vector3D &pos1, const Vector3D &pos2, const Vector3D &pos3, const Vector3D &color);

private:
	HRESULT InitShader();
	HRESULT InitVertex();

private:
	ID3D11Buffer* m_pConstantBufferTriangle3D;
};

#endif