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
	if(!InitBillboard()) MessageBox(0, TEXT("Billboard�쐬���s"), NULL, MB_OK);
}

bool FiqureShaderManager::InitBillboard()
{
	FigureBillboardInfo *info = &m_pFiqurePimpl->billboardInfo;

	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("Billboard.hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("Billboard�̃o�[�e�b�N�X�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//���_�C���v�b�g���C�A�E�g���`
	//D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &info->pVertexLayout)))
	{
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("Billboard.hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &info->pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("Billboard�̃s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
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