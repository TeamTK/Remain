#include "DynamicMeshShader.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"

DynamicMeshShader::DynamicMeshShader()
{
	INIT_NULLPOINTR(m_pCompiledShader);
	INIT_NULLPOINTR(m_pErrors);
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pVertexShader_NoTexture);
	INIT_NULLPOINTR(m_pVertexShader_ShadowMap);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pPixelShader_NoTexture);
	INIT_NULLPOINTR(m_pPixelShader_ShadowMap);
	INIT_NULLPOINTR(m_pVertexLayout);
}

DynamicMeshShader::~DynamicMeshShader()
{
	Release();
}

DynamicMeshShader *DynamicMeshShader::GetInstance()
{
	static DynamicMeshShader DynamicMeshShader;
	return &DynamicMeshShader;
}

bool DynamicMeshShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//���_�V�F�[�_�[������
	if (!InitVertexShader(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("���_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�e�N�X�`���[�Ȃ����_�V�F�[�_�[������
	if (!InitVertexShader_NoTexture(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("�e�N�X�`���[�Ȃ����_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//���_�V�F�[�_�[�������i�e�j
	if (!InitVertexShader_ShadowMap(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("�V���h�E�}�b�v���_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�s�N�Z���V�F�[�_�[������
	if (!InitPixelShader(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[������
	if (!InitPixelShader_NoTexture(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�s�N�Z���V�F�[�_�[�������i�e�j
	if (!InitPixelShader_ShadowMap(pDevice))
	{
		ReleaseBlod();
		MessageBox(0, TEXT("�V���h�E�}�b�v�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	return true;
}

void DynamicMeshShader::SetShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//�e�N�X�`������Ȃ��Ő؂�ւ�
	if (isTexture)
	{
		//�e����Ȃ��Ő؂�ւ�
		if (isShadow)
		{
			//�e����
			pDeviceContext->VSSetShader(m_pVertexShader_ShadowMap, NULL, 0);
			pDeviceContext->PSSetShader(m_pPixelShader_ShadowMap, NULL, 0);
			ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		}
		else
		{
			//�e�Ȃ�
			pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
			pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		}
		pDeviceContext->IASetInputLayout(m_pVertexLayout);
	}
	else
	{
		//�e�N�X�`���Ȃ�
		pDeviceContext->VSSetShader(m_pVertexShader_NoTexture, NULL, 0);
		pDeviceContext->PSSetShader(m_pPixelShader_NoTexture, NULL, 0);

		pDeviceContext->IASetInputLayout(m_pVertexLayout_NoTexture);
	}

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DynamicMeshShader::Release()
{
	ReleaseBlod();

	//�V�F�[�_�[
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexShader_NoTexture);
	SAFE_RELEASE(m_pVertexShader_ShadowMap);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pPixelShader_NoTexture);
	SAFE_RELEASE(m_pPixelShader_ShadowMap);

	//���_���C�A�E�g
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexLayout_NoTexture);
}

void DynamicMeshShader::ReleaseBlod()
{
	SAFE_RELEASE(m_pCompiledShader);
	SAFE_RELEASE(m_pErrors);
}