#ifndef _FIQURE_SHARDERMANAGER_H_
#define _FIQURE_SHARDERMANAGER_H_

#include "FiqureInfo.h"

struct FigureBillboardInfo;

class FiqureShaderManager
{
public:
	~FiqureShaderManager();
	static FiqureShaderManager* GetInstance();
	FigureBillboardInfo *GetBillboardInfo();
	FigureBillboardInfo *GetBillboardAnimation();
	bool Init();

private:
	FiqureShaderManager();
	bool InitBillboard(ID3D10Blob *pCompiledShader, ID3D10Blob *pErrors);
	bool InitBillboardAnimation(ID3D10Blob *pCompiledShader, ID3D10Blob *pErrors);

private:
	class FiqureShaderPimpl;
	FiqureShaderPimpl *m_pFiqurePimpl;
};

#endif