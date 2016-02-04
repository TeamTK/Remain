#ifndef _BASE_COPMUTE_H_
#define _BASE_COPMUTE_H_

#include "CopmuteInfo.h"

class BaseCopmute
{
public:
	BaseCopmute();
	virtual ~BaseCopmute();

protected:
	ID3D11Buffer* m_pBufferResult;
	ID3D11UnorderedAccessView* m_pBufferResultUAV;
};

class CopmuteManager
{
public:
	~CopmuteManager();
	static CopmuteManager* GetInstance();
	void Init();
	void CSSetLineTriangle();
	void Release();

private:
	CopmuteManager();

private:
	ID3D11ComputeShader* m_pLineTriangle;

};

#endif