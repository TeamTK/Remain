#ifndef _FIQURE_SHARDERMANAGER_H_
#define _FIQURE_SHARDERMANAGER_H_

struct FigureBillboardInfo;

class FiqureShaderManager
{
public:
	~FiqureShaderManager();
	static FiqureShaderManager* GetInstance();
	FigureBillboardInfo *GetBillboardInfo();
	void Init();

private:
	FiqureShaderManager();
	bool InitBillboard();

private:
	class FiqureShaderPimpl;
	FiqureShaderPimpl *m_pFiqurePimpl;
};

#endif