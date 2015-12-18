#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "FiqureBase.h"

struct BillboradVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex;
};

class Billboard : public FiqureBase
{
public:
	Billboard();
	~Billboard();
	void Render(const Vector3D &pos, const Vector3D &scale, const std::string &name);

private:
	HRESULT InitShader();
	HRESULT InitVertex();
};

#endif