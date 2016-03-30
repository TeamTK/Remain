#include "DeferredRendering.h"
#include "../ShaderInfo.h"

#define MULTI_RENDER_TOTAL 3

struct DeferredVertex
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

struct DeferredRendering::DeferredPimpl
{
	//�p�X1�p�ʏ탁�b�V��
	ID3D11VertexShader *pVertexShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11PixelShader *pPixelShaderPass1[SHADER_TYPE_TOTAL];

	//�p�X1�p�X�L�����b�V��
	ID3D11VertexShader *pSkinVertexShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11PixelShader *pSkinPixelShaderPass1[SHADER_TYPE_TOTAL];

	ID3D11Texture2D *pTexture2D[MULTI_RENDER_TOTAL];
	ID3D11RenderTargetView *pRenderTargetView[MULTI_RENDER_TOTAL];
	ID3D11ShaderResourceView *pShaderResourceView[MULTI_RENDER_TOTAL];

	//�p�X�Q�p
	ID3D11VertexShader *pVertexShaderPass2;
	ID3D11PixelShader *pPixelShaderPass2;
	ID3D11Buffer *pVertexBuffer;
};

DeferredRendering::DeferredRendering()
{
	if (!InitSheder())
	{
		MessageBox(0, TEXT("�f�t�@�[�h�����_�����O�V�F�[�_�[�쐬���s"), NULL, MB_OK);
	}
	
	m_pDeferredPimpl = new DeferredPimpl;
}

DeferredRendering::~DeferredRendering()
{
	Release();
	delete m_pDeferredPimpl;
}

DeferredRendering *DeferredRendering::GetInstance()
{
	static DeferredRendering deferredRendering;
	return &deferredRendering;
}

bool DeferredRendering::Init()
{
	for (int i = 0; i < MULTI_RENDER_TOTAL; i++)
	{
		SAFE_RELEASE(m_pDeferredPimpl->pTexture2D[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pRenderTargetView[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pShaderResourceView[i]);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));

	auto *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//�A���׃h
	desc.Width = Direct3D11::GetInstance()->GetResolutionWidth();
	desc.Height = Direct3D11::GetInstance()->GetResolutionHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	RTVDesc.Format = desc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[eAlbedo]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[eAlbedo], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[eAlbedo]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[eAlbedo], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[eAlbedo]);

	//�@��
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RTVDesc.Format = desc.Format;
	SRVDesc.Format = desc.Format;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[eNormal]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[eNormal], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[eNormal]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[eNormal], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[eNormal]);

	//���W
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RTVDesc.Format = desc.Format;
	SRVDesc.Format = desc.Format;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[eNormal]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[eNormal], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[eNormal]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[eNormal], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[eNormal]);

	return true;
}

void DeferredRendering::ChangeRenderTarget()
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();

	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();
	auto *pDepthStencilView = pDirect11->GetDepthStencilView();
	float red = pDirect11->GetClearColorRed();
	float green = pDirect11->GetClearColorGreen();
	float blue = pDirect11->GetClearColorBlue();

	pDeviceContext->OMSetRenderTargets(MULTI_RENDER_TOTAL, m_pDeferredPimpl->pRenderTargetView, pDepthStencilView);

	float ClearColor[4] = { red, green, blue, 1 };
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[eAlbedo], ClearColor);
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[eNormal], ClearColor);
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[ePosition], ClearColor);
}

void DeferredRendering::SetShaderMesh(ShaderType type)
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	int typeNum = static_cast<int>(type);

	//�p�X1�Ŏg���V�F�[�_�[
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pVertexShaderPass1[typeNum], NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pPixelShaderPass1[typeNum], NULL, 0);
}

void DeferredRendering::SetShaderSkinMesh(ShaderType type)
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	int typeNum = static_cast<int>(type);

	//�p�X1�Ŏg���V�F�[�_�[
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pSkinVertexShaderPass1[typeNum], NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pSkinPixelShaderPass1[typeNum], NULL, 0);
}

void DeferredRendering::Rendering()
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	pDirect11->RenderTarget();
	pDirect11->Clear();

	//�p�X2�Ŏg���V�F�[�_�[
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pVertexShaderPass2, NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pPixelShaderPass2, NULL, 0);

	//�p�X1�ō쐬�����e�N�X�`���[���Z�b�g
	pDeviceContext->PSSetShaderResources(1, 1, &m_pDeferredPimpl->pShaderResourceView[eAlbedo]);
	pDeviceContext->PSSetShaderResources(2, 1, &m_pDeferredPimpl->pShaderResourceView[eNormal]);
	pDeviceContext->PSSetShaderResources(3, 1, &m_pDeferredPimpl->pShaderResourceView[ePosition]);

	//�X�N���[���T�C�Y�̃|���S��
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT stride = sizeof(DeferredVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pDeferredPimpl->pVertexBuffer, &stride, &offset);

	pDeviceContext->Draw(4, 0);
}

void DeferredRendering::Release()
{
	//�p�X�P
	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		//�ʏ탁�b�V��
		SAFE_RELEASE(m_pDeferredPimpl->pVertexShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pPixelShaderPass1[i]);

		//�X�L�����b�V��
		SAFE_RELEASE(m_pDeferredPimpl->pSkinVertexShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pSkinPixelShaderPass1[i]);
	}

	for (int i = 0; i < MULTI_RENDER_TOTAL; i++)
	{
		SAFE_RELEASE(m_pDeferredPimpl->pTexture2D[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pRenderTargetView[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pShaderResourceView[i]);
	}

	//�p�X�Q
	SAFE_RELEASE(m_pDeferredPimpl->pVertexShaderPass2);
	SAFE_RELEASE(m_pDeferredPimpl->pPixelShaderPass2);
	SAFE_RELEASE(m_pDeferredPimpl->pVertexBuffer);
}

bool DeferredRendering::InitSheder()
{
	auto *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	std::string nameVS[] =
	{
		"VS",
		"VS_ShadowMap"
	};

	std::string namePS[] =
	{
		"PS",
		"PS_ShadowMap"
	};

	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	HRESULT hr;

	//�p�X�P�Ŏg���V�F�[�_�[��ǂݍ���
	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		/*
		//���_�V�F�[�_�[�쐬�p�X1
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
		hr = pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pVertexShaderPass1[i]);

		//�X�L�����_�V�F�[�_�[�쐬�p�X1
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
		hr = pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinVertexShaderPass1[i]);

		//�s�N�Z���V�F�[�_�[�쐬�p�X�P
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
		hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pPixelShaderPass1[i]);

		//�s�N�Z���V�F�[�_�[�쐬�p�X�P(�X�L��)
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
		hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinPixelShaderPass1[i]);
		*/

		//���_�V�F�[�_�[�쐬�p�X1
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			return false;
		}

		if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pVertexShaderPass1[i])))
		{
			return false;
		}

		//�X�L�����_�V�F�[�_�[�쐬�p�X1
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			return false;
		}

		if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinVertexShaderPass1[i])))
		{
			return false;
		}

		//�s�N�Z���V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			return false;
		}

		if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pPixelShaderPass1[i])))
		{
			return false;
		}

		//�s�N�Z���V�F�[�_�[�쐬(�X�L��)
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			return false;
		}

		if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinPixelShaderPass1[i])))
		{
			return false;
		}
	}

	//���_�V�F�[�_�[�쐬�p�X2
	hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
		NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
	hr = pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		NULL, &m_pDeferredPimpl->pVertexShaderPass2);

	//�s�N�Z���V�F�[�_�[�쐬�p�X�Q
	hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
		NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL);
	hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		NULL, &m_pDeferredPimpl->pPixelShaderPass2);

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//���_Buffer�쐬
	DeferredVertex VertexData[] =
	{
		Vector3D(-1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 1.0f),
		Vector3D(-1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 0.0f),
		Vector3D(1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 1.0f),
		Vector3D(1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 0.0f)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DeferredVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = VertexData;
	hr = pDevice->CreateBuffer(&bd, &InitData, &m_pDeferredPimpl->pVertexBuffer);

	if (FAILED(hr)) return false;
	return true;
}