#ifndef _LINE3D_H_
#define _LINE3D_H_

#include "FiqureBase.h"

struct ConstantBufferLine3D
{
	D3DXVECTOR4 pos[2];
};

class Line3D : public FiqureBase
{
public:
	Line3D();
	~Line3D();
	void Render(const Vector3D &Spos, const Vector3D &Epos, const Vector3D &Color);

private:
	HRESULT InitShader();
	HRESULT InitVertex();

private:
	ID3D11Buffer* m_pConstantBufferLine3D;
};

#endif