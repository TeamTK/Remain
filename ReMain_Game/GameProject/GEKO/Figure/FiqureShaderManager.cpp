#include "FiqureShaderManager.h"
#include "FiqureInfo.h"

class FiqureShaderManager::FiqureShaderPimpl
{
public:
	FigureBillboardInfo billboardInfo;
};

FiqureShaderManager::FiqureShaderManager()
{
	m_pFiqurePimpl = new FiqureShaderPimpl;
}

FiqureShaderManager::~FiqureShaderManager()
{
	delete m_pFiqurePimpl;
}

FiqureShaderManager* FiqureShaderManager::GetInstance()
{
	static FiqureShaderManager fiqureShaderManager;
	return &fiqureShaderManager;
}

FigureBillboardInfo *FiqureShaderManager::GetBillboardInfo()
{
	return &m_pFiqurePimpl->billboardInfo;
}

void FiqureShaderManager::Init()
{
	if(!InitBillboard()) MessageBox(0, TEXT("Billboard作成失敗"), NULL, MB_OK);
}

bool FiqureShaderManager::InitBillboard()
{
	FigureBillboardInfo *info = &m_pFiqurePimpl->billboardInfo;

	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("Billboard.hlsl読み込み失敗"), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("Billboardのバーテックスシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//頂点インプットレイアウトを定義
	//D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &info->pVertexLayout)))
	{
		return FALSE;
	}

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("Billboard.hlsl読み込み失敗"), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("Billboardのピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferFiqureBillboard);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &info->pConstantBuffer)))
	{
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}