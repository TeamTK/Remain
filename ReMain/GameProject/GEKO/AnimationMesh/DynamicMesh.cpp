#include "DynamicMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include <assert.h>

DynamicMesh::DynamicMesh()
{
}

DynamicMesh::~DynamicMesh()
{
	m_pSkinMeshData = nullptr;
}

void DynamicMesh::SetAsset(SkinMeshData *meshData)
{
	m_pSkinMeshData = meshData;
	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;
	for (int i = 0; i < data->materialNumAll; i++)
	{
		diffuse = data->m_pMaterial[i].Diffuse;
		specular = data->m_pMaterial[i].Specular;
		ambient = data->m_pMaterial[i].Ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	m_LocalMatrix = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat;
}

void DynamicMesh::SetAsset(const std::string &MeshName)
{
	m_pSkinMeshData = DynamicMeshAsset::GetMesh(MeshName);
	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;
	for (int i = 0; i < data->materialNumAll; i++)
	{
		diffuse = data->m_pMaterial[i].Diffuse;
		specular = data->m_pMaterial[i].Specular;
		ambient = data->m_pMaterial[i].Ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	m_LocalMatrix = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat;
}

void DynamicMesh::ChangeAnimation(unsigned int num)
{
	m_pSkinMeshData->ChangeAnimation(num);
}

void DynamicMesh::SetPlayTime(float animSpeed)
{
	m_AnimTime += Math::VelocityToFrameM(animSpeed);
	m_pSkinMeshData->SetTime(m_AnimTime);
	//m_pSkinMeshData->SetRenewalTime(animSpeed);
}

void DynamicMesh::SetTime(float animTime)
{
	m_AnimTime = animTime;
	m_pSkinMeshData->SetTime(m_AnimTime);
}

SkinVertexInfo *DynamicMesh::GetVertex()
{
	return m_pSkinMeshData->GetSkinMeshInfo()->pvVertex;
}

float DynamicMesh::GetPlayTime()
{
	return m_pSkinMeshData->GetPlayTime();
}

int DynamicMesh::GetPlayAnimation()
{
	return m_pSkinMeshData->GetPlayAnimation();
}

int DynamicMesh::GetFaceAllNum()
{
	return m_pSkinMeshData->GetSkinMeshInfo()->faceNumAll;
}

int DynamicMesh::GetBornNum(std::string name)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return i;
		}
	}
	return -1;
}

int DynamicMesh::GetBornAllNum()
{
	return m_pSkinMeshData->GetBornAllNum();
}

std::string DynamicMesh::GetBornName(int bornIndex)
{
	return m_pSkinMeshData->GetBornName(bornIndex);
}

Matrix DynamicMesh::GetBornMatrix(int bornIndex, bool isWorld)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //���[���h�s��i�[
	if (isWorld) return m_pSkinMeshData->GetBornWorld(bornIndex) * m * m_Matrix;
	return m_pSkinMeshData->GetBornWorld(bornIndex);
}

Matrix DynamicMesh::GetBornMatrix(std::string name, bool isWorld)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //���[���h�s��i�[
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			if (isWorld) return m_pSkinMeshData->GetBornWorld(i) * m * m_Matrix;
			return m_pSkinMeshData->GetBornWorld(i);
		}
	}
	
	return Matrix();
}

Vector3D DynamicMesh::GetBornPos(int bornIndex)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //���[���h�s��i�[
	return m_pSkinMeshData->GetBornPos(bornIndex) * m * m_Matrix;
}

Vector3D DynamicMesh::GetBornPos(std::string name)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //���[���h�s��i�[
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return m_pSkinMeshData->GetBornPos(i) * m * m_Matrix;
		}
	}
	return Vector3D();
}

void DynamicMesh::Render()
{
	RenderFunc(m_Matrix);
	m_AnimTime = GetPlayTime();
}

void DynamicMesh::RenderMatrix(Matrix &matrix)
{
	RenderFunc(matrix);
}

void DynamicMesh::BornDebug(eBornDebug eBornDebug)
{
	m_pSkinMeshData->BornDebug(eBornDebug);
}

void DynamicMesh::AnimationDebug(int animNum)
{
	m_pSkinMeshData->AnimationDebug(animNum);
}

void DynamicMesh::RenderFunc(Matrix &matrix)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//assert(m_pSkinMeshData != nullptr && "���b�V����񂪂���܂���");

	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();

	D3DXMATRIX World = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat * matrix; //���[���h�s��i�[

	//�g�p����V�F�[�_�[�̓o�^
	pDeviceContext->VSSetShader(data->m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(data->m_pPixelShader, NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		data->m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SkinConstantBuffer0 sg;
		
		//���[���h�s���n��
		sg.mW = World;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//���[���h�A�J�����A�ˉe�s���n��
		sg.mWVP = World * (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		//���C�g�̕�����n��
		D3DXVec4Normalize(&sg.vLightDir, DirectionalLight::GetDirection());
		sg.fIntensity = DirectionalLight::GetLightColor()->Change();

		//���_�ʒu��n��
		D3DXVECTOR3 EyePos(Camera::GetEyePositionD3D());
		sg.vEye = D3DXVECTOR4(EyePos.x, EyePos.y, EyePos.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer0));
		pDeviceContext->Unmap(data->m_pConstantBuffer0, 0);
	}

	//�R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);

	m_pSkinMeshData->Update();
	if (SUCCEEDED(pDeviceContext->Map(data->m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BornConstantBuffer sg;
		for (int i = 0; i < m_pSkinMeshData->GetBornAllNum(); i++)
		{
			D3DXMATRIX m = data->BornBuffer.bornMat[i];
			D3DXMatrixTranspose(&m, &m);
			sg.bornMat[i] = m;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BornConstantBuffer));
		pDeviceContext->Unmap(data->m_pConstantBufferBone, 0);
	}

	pDeviceContext->VSSetConstantBuffers(2, 1, &data->m_pConstantBufferBone);
	pDeviceContext->PSSetConstantBuffers(2, 1, &data->m_pConstantBufferBone);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(data->m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�������ƂɃ����_�����O

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT Stride = sizeof(SkinVertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->m_pVertexBuffer, &Stride, &offset);

	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 ambient;

	//�����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < data->materialNumAll; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (data->m_pMaterial[i].dwNumFace == 0) continue;

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		pDeviceContext->IASetIndexBuffer(data->m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		diffuse.x = m_Diffuse[i].x;
		diffuse.y = m_Diffuse[i].y;
		diffuse.z = m_Diffuse[i].z;
		diffuse.w = m_Diffuse[i].w;

		specular.x = m_Specular[i].x;
		specular.y = m_Specular[i].y;
		specular.z = m_Specular[i].z;
		specular.w = m_Diffuse[i].w;

		ambient.x = m_Ambient[i].x;
		ambient.y = m_Ambient[i].y;
		ambient.z = m_Ambient[i].z;
		ambient.w = m_Diffuse[i].w;

		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(pDeviceContext->Map(data->m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SkinConstantBuffer1 sg;
			sg.vDiffuse = diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			sg.vSpecular = specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			sg.vAmbient = ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer1));
			pDeviceContext->Unmap(data->m_pConstantBuffer1, 0);
		}
		pDeviceContext->VSSetConstantBuffers(1, 1, &data->m_pConstantBuffer1);
		pDeviceContext->PSSetConstantBuffers(1, 1, &data->m_pConstantBuffer1);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (data->m_pMaterial[i].pTexture)
		{
			pDeviceContext->PSSetSamplers(0, 1, &data->m_pSampleLinear);
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_pMaterial[i].pTexture);
		}
		else
		{
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_Nothing);
		}

		//�v���~�e�B�u�������_�����O
		pDeviceContext->DrawIndexed(data->m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}