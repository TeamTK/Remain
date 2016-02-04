#include "BaseCopmute.h"

BaseCopmute::BaseCopmute() :
	m_pBufferResult(nullptr),
	m_pBufferResultUAV(nullptr)
{
}

BaseCopmute::~BaseCopmute()
{
	SAFE_RELEASE(m_pBufferResult);
	SAFE_RELEASE(m_pBufferResultUAV);
}

CopmuteManager::CopmuteManager() :
	m_pLineTriangle(nullptr)
{

}

CopmuteManager::~CopmuteManager()
{
}

CopmuteManager* CopmuteManager::GetInstance()
{
	static CopmuteManager copmuteManager;
	return &copmuteManager;
}

void CopmuteManager::Init()
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetID3D11Device();
	ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//コンピュートシェーダー　作成
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pBlob = NULL;

	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO//HLSL//GPU//LineSegmentTriangle.hlsl"), 0, 0, "CS", "cs_5_0",
								     0, 0, 0, &pBlob, &pErrorBlob, 0)))
	{
		MessageBox(0, TEXT("LineSegmentTriangle.hlsl読み込み失敗"), NULL, MB_OK);
	}
	pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pLineTriangle);

	SAFE_RELEASE(pErrorBlob);
	SAFE_RELEASE(pBlob);
}

void CopmuteManager::Release()
{
	SAFE_RELEASE(m_pLineTriangle);
}

void CopmuteManager::CSSetLineTriangle()
{
	ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();
	pDeviceContext->CSSetShader(m_pLineTriangle, 0, 0);
}