#include "FiqureShaderManager.h"

enum FiqureType
{
	eBillboard,
	eBillboardAnimation,
};

class FiqureShaderManager::FiqureShaderPimpl
{
public:
	FigureBillboardInfo billboardInfo[2];
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
	return &m_pFiqurePimpl->billboardInfo[eBillboard];
}

FigureBillboardInfo *FiqureShaderManager::GetBillboardAnimation()
{
	return &m_pFiqurePimpl->billboardInfo[eBillboardAnimation];
}

bool FiqureShaderManager::Init()
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ビルボード初期化
	if (!InitBillboard(pCompiledShader, pErrors))
	{
		MessageBox(0, TEXT("Billboard作成失敗"), NULL, MB_OK);
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
	};

	//ビルボードアニメーション初期化
	if (!InitBillboardAnimation(pCompiledShader, pErrors))
	{
		MessageBox(0, TEXT("BillboardAnimation作成失敗"), NULL, MB_OK);
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
	};

	return true;
}

bool FiqureShaderManager::InitBillboard(ID3D10Blob *pCompiledShader, ID3D10Blob *pErrors)
{
	FigureBillboardInfo *info = &m_pFiqurePimpl->billboardInfo[eBillboard];

	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pVertexShader)))
	{
		return false;
	}

	//頂点インプットレイアウトを定義
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
		return false;
	}

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pPixelShader)))
	{
		return false;
	}

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferFiqureBillboard);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &info->pConstantBuffer))) return false;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}

bool FiqureShaderManager::InitBillboardAnimation(ID3D10Blob *pCompiledShader, ID3D10Blob *pErrors)
{
	FigureBillboardInfo *info = &m_pFiqurePimpl->billboardInfo[eBillboardAnimation];

	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\BillboardAnimation.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pVertexShader)))
	{
		return false;
	}

	//頂点インプットレイアウトを定義
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
		return false;
	}

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\BillboardAnimation.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pPixelShader)))
	{
		return false;
	}

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferFiqureBillboardAnimation);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &info->pConstantBuffer))) return false;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}