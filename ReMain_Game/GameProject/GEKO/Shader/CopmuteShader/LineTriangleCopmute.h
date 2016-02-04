#ifndef _LINE_TRIANGLE_COPMUTE_H_
#define _LINE_TRIANGLE_COPMUTE_H_

#include "BaseCopmute.h" 

class StaticMesh;
struct Result_Porygon;

class LineTriangleCopmute : public BaseCopmute
{
public:
	LineTriangleCopmute();
	~LineTriangleCopmute();
	void Init(const StaticMesh &staticMesh, const Vector3D *pNormalArray);
	void Release();
	bool Calculation(const StaticMesh &staticMesh, const Vector3D &start, const Vector3D &end, Result_Porygon *pPory);

private:
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pBuffer;
	ID3D11ShaderResourceView* m_pBufferSRV;
	TriangleInfo_GPU *m_pTriangleInfo;
};

#endif