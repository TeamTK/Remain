#include "FiqureBase.h"

FiqureBase::FiqureBase()
{
}

FiqureBase::~FiqureBase()
{
}

HRESULT FiqureBase::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Fiqure.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�o�[�e�b�N�X�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 1;
	memcpy(&layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(
		&layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_FigureInfo.pVertexLayout)))
	{
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Fiqure.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferFiqure);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_FigureInfo.pConstantBuffer)))
	{
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return S_OK;
}