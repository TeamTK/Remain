#ifndef _SPHERE_TRIANGLE_COPMUTE_H_
#define _SPHERE_TRIANGLE_COPMUTE_H_

#include "BaseCopmute.h"

class StaticMesh;
struct Result_Porygon_Group_Sphere;

class SphereTriangleCopmute : public BaseCopmute
{
public:
	SphereTriangleCopmute();
	~SphereTriangleCopmute();
	void Init(const StaticMesh &staticMesh, const Vector3D *pNormalArray);
	bool Calculation(const StaticMesh &staticMesh, const Vector3D &point, float radius, Result_Porygon_Group_Sphere *pory);

private:
	ID3D11Buffer *m_pConstantBuffer;
	ID3D11Buffer *m_pBuffer;
	ID3D11ShaderResourceView *m_pBufferSRV;
	Sphere_TriangleInfo_GPU *m_pTriangleInfo;
};

#endif