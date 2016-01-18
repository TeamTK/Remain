#include "StaticMeshShader.h"

StaticMeshShader::StaticMeshShader()
{
	INIT_NULLPOINTR(m_pConstantBuffer0);
	INIT_NULLPOINTR(m_pConstantBuffer1);
	INIT_NULLPOINTR(m_pConstantBuffer2);
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pVertexLayout);
	INIT_NULLPOINTR(m_pPSClassLinkage);
	INIT_NULLPOINTR(m_pClassInstance);
}

StaticMeshShader::~StaticMeshShader()
{
}

StaticMeshShader *StaticMeshShader::GetInstance()
{
	static StaticMeshShader staticMeshShader;
	return &staticMeshShader;
}

HRESULT StaticMeshShader::Init()
{


	return S_OK;
}

void StaticMeshShader::Begin()
{
	m_pPSClassLinkage->GetClassInstance(m_ClassInstanceName.c_str(), 0, &m_pClassInstance);

	ID3D11DeviceContext *context = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	// ���C�A�E�g�ݒ�
	context->IASetInputLayout(m_pVertexLayout);

	// ���_�V�F�[�_�[�ɒ萔�o�b�t�@��ݒ肷��
	context->VSSetConstantBuffers(1, 1, &m_pConstantBuffer2);

	// �s�N�Z���V�F�[�_�[�𓮓I�V�F�[�_�[�����N�Ƃ��Đݒ肷��
	context->PSSetShader(m_pPixelShader, &m_pClassInstance, 1);

	// �s�N�Z���V�F�[�_�[�ɒ萔�o�b�t�@��ݒ肷��
	context->PSSetConstantBuffers(0, 1, &m_pConstantBuffer2);
}

void StaticMeshShader::End()
{
	SAFE_RELEASE(m_pClassInstance);
}

void StaticMeshShader::Release()
{
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBuffer2);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPSClassLinkage);
	SAFE_RELEASE(m_pClassInstance);
}