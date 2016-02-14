#include "DynamicMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include "..\Shader\DynamicMeshShader\DynamicMeshShader.h"
#include "..\ImageSystem\Image.h"
#include <assert.h>

DynamicMesh::DynamicMesh() :
	m_IsAnimUpdate(true),
	m_pSkinMeshData(nullptr)
{
}

DynamicMesh::~DynamicMesh()
{
	ReleseCopyBornTree(&m_Born);
	m_pSkinMeshData = nullptr;
	m_CopyBornArray.clear();
	m_CopyBornArray.shrink_to_fit();
}

DynamicMesh::DynamicMesh(const std::string &meshName) :
	m_IsAnimUpdate(true),
	m_pSkinMeshData(nullptr)
{
	AllocationSkinMeshData(meshName);
}

void DynamicMesh::SetAsset(const std::string &meshName)
{
	AllocationSkinMeshData(meshName);
}

void DynamicMesh::ChangeAnimation(unsigned int num)
{	
	//�A�j���[�V�����Z�b�g�͈̔͊O�A�N�Z�X�h�~
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	unsigned int AnimSetAllNum = pBornData->AnimationSetFrameNum.size();
	if (num >= AnimSetAllNum) num = AnimSetAllNum - 1;
	else if (num <= 0) num = 0;

	for (auto& i : m_CopyBornArray) i->animNum = num;
}

void DynamicMesh::PartChangeAnimation(int bornIndex, unsigned int num)
{
	//�A�j���[�V�����Z�b�g�͈̔͊O�A�N�Z�X�h�~
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	unsigned int AnimSetAllNum = pBornData->AnimationSetFrameNum.size();
	if (num >= AnimSetAllNum) num = AnimSetAllNum - 1;
	else if (num <= 0) num = 0;

	m_CopyBornArray[bornIndex]->animNum = num;
}

void DynamicMesh::PartRangeChangeAnimation(int bornStart, int bornEnd, unsigned int num)
{
	//�A�j���[�V�����Z�b�g�͈̔͊O�A�N�Z�X�h�~
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	unsigned int AnimSetAllNum = pBornData->AnimationSetFrameNum.size();
	if (num >= AnimSetAllNum) num = AnimSetAllNum - 1;
	else if (num <= 0) num = 0;

	int cnt = bornStart;
	for (auto& i : m_CopyBornArray)
	{
		if (cnt > bornEnd) break;
		m_CopyBornArray[cnt]->animNum = num;
		cnt++;
	}
}

void DynamicMesh::SetPlayTime(float animSpeed)
{
	if (m_IsAnimUpdate)
	{
		//float ainmFrame = Math::VelocityToFrameM(animSpeed);
		//for (auto& i : m_CopyBornArray) i->animFrame += ainmFrame;
		for (auto& i : m_CopyBornArray) i->animFrame += animSpeed;
	}
}

void DynamicMesh::SetPartPlayTime(int bornIndex, float animSpeed)
{
	if (m_IsAnimUpdate)
	{
		//float ainmFrame = Math::VelocityToFrameM(animSpeed);
		//m_CopyBornArray[bornIndex]->animFrame += ainmFrame;
		m_CopyBornArray[bornIndex]->animFrame += animSpeed;
	}
}

void DynamicMesh::SetPartRangePlayTime(int bornStart, int bornEnd, float animSpeed)
{
	//float ainmFrame = Math::VelocityToFrameM(animSpeed);

	int cnt = bornStart;
	for (auto& i : m_CopyBornArray)
	{
		if (cnt > bornEnd) break;
		//m_CopyBornArray[cnt]->animFrame += ainmFrame;
		m_CopyBornArray[cnt]->animFrame += animSpeed;
		cnt++;
	}
}

void DynamicMesh::SetTime(float animTime)
{
	for (auto& i : m_CopyBornArray) i->animFrame = animTime;
}

void DynamicMesh::SetPartTime(int bornIndex, float animTime)
{
	m_CopyBornArray[bornIndex]->animFrame = animTime;
}

void DynamicMesh::SetPartRangeTime(int bornStart, int bornEnd, float animTime)
{
	int cnt = bornStart;
	for (auto& i : m_CopyBornArray)
	{
		if (cnt > bornEnd) break;
		m_CopyBornArray[cnt]->animFrame = animTime;
		cnt++;
	}
}

void DynamicMesh::StartAnimation()
{
	m_IsAnimUpdate = true;
}

void DynamicMesh::StopAnimation()
{
	m_IsAnimUpdate = false;
}

const SkinVertexInfo *DynamicMesh::GetVertex() const
{
	return m_pSkinMeshData->GetSkinMeshInfo()->pvVertex;
}

float DynamicMesh::GetPlayTime(int bornIndex) const
{
	return m_CopyBornArray[bornIndex]->animFrame;
}

int DynamicMesh::GetPlayAnimation(int bornIndex) const
{
	return m_CopyBornArray[bornIndex]->animNum;
}

int DynamicMesh::GetFaceAllNum() const
{
	return m_pSkinMeshData->GetSkinMeshInfo()->faceNumAll;
}

int DynamicMesh::GetBornNum(std::string name) const
{
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	unsigned int bornNum = pBornData->BornList.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return i;
		}
	}
	return -1;
}

int DynamicMesh::GetBornAllNum() const
{
	return m_pSkinMeshData->GetBornInfo()->BornList.size();
}

std::string DynamicMesh::GetBornName(int bornIndex) const
{
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	return pBornData->BornList[bornIndex]->BornName;
}

Matrix DynamicMesh::GetBornMatrix(int bornIndex, bool isWorld) const
{
	if (isWorld) return m_CopyBornArray[bornIndex]->ParentAndChildMat * m_LocalMatrix * m_Matrix;
	return m_CopyBornArray[bornIndex]->ParentAndChildMat;
}

Matrix DynamicMesh::GetBornMatrix(std::string name, bool isWorld) const
{
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();

	int cnt = 0;
	for (auto& i : pBornData->BornList)
	{
		if (i->BornName == name)
		{
			if (isWorld) return m_CopyBornArray[cnt]->ParentAndChildMat * m_LocalMatrix * m_Matrix;
			return m_CopyBornArray[cnt]->ParentAndChildMat;
		}
		cnt++;
	}

	return Matrix();
}

Vector3D DynamicMesh::GetBornPos(int bornIndex) const
{
	Matrix m = m_CopyBornArray[bornIndex]->ParentAndChildMat;
	return Vector3D(m._41, m._42, m._43) * m_LocalMatrix * m_Matrix;
}

Vector3D DynamicMesh::GetBornPos(std::string name) const
{
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();

	int cnt = 0;
	for (auto& i : pBornData->BornList)
	{
		if (i->BornName == name)
		{
			Matrix m = m_CopyBornArray[cnt]->ParentAndChildMat;
			return Vector3D(m._41, m._42, m._43) * m_LocalMatrix * m_Matrix;
		}
		cnt++;
	}

	return Vector3D();
}

void DynamicMesh::Render()
{
	RenderFunc(m_LocalMatrix * m_Matrix);
}

void DynamicMesh::RenderOutline(float size = 1.0f)
{
	RenderFunc(m_Matrix);

	Matrix m = m_Matrix;
	m._11 *= size;
	//m._22 *= size;
	m._33 *= size;

	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	RenderFunc(m);
	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_BACK, D3D11_FILL_SOLID);
}

void DynamicMesh::RenderMatrix(Matrix &matrix)
{
	RenderFunc(m_LocalMatrix * matrix);
}

void DynamicMesh::BornDebug(eBornDebug eBornDebug) const
{
	/*
	auto it = m_CopyBornArray.begin();
	auto itEnd = m_CopyBornArray.end();
	for (; it != itEnd; it++)
	{
		//(*it)->
	}
	*/

	int cnt = 0;
	for (auto& i : m_pSkinMeshData->GetBornInfo()->BornList)
	{
		printf("%s\n", i->BornName.c_str());

		D3DXMATRIX m = m_CopyBornArray[cnt]->bornMat;
		printf("%f %f %f %f\n", m._11, m._12, m._13, m._14);
		printf("%f %f %f %f\n", m._21, m._22, m._23, m._24);
		printf("%f %f %f %f\n", m._31, m._32, m._33, m._34);
		printf("%f %f %f %f\n", m._41, m._42, m._43, m._44);

		cnt++;
	}
}

void DynamicMesh::AnimationDebug(int animNum) const
{
	m_pSkinMeshData->AnimationDebug(animNum);
}

void DynamicMesh::AllocationSkinMeshData(const std::string &meshName)
{
	//������
	m_Diffuse.clear();
	m_Diffuse.shrink_to_fit();
	m_Specular.clear();
	m_Specular.shrink_to_fit();
	m_Ambient.clear();
	m_Ambient.shrink_to_fit();
	m_pSkinMeshData = nullptr;

	m_pSkinMeshData = DynamicMeshAsset::GetMesh(meshName);
	const SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;

	//�}�e���A�����蓖��
	for (int i = 0; i < data->materialNumAll; i++)
	{
		diffuse = data->m_pMaterial[i].Diffuse;
		specular = data->m_pMaterial[i].Specular;
		ambient = data->m_pMaterial[i].Ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}
	const BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();

	//�{�[��������
	m_CopyBornArray.clear();
	m_CopyBornArray.shrink_to_fit();

	//�{�[�����R�s�[
	m_CopyBornArray.emplace_back(&m_Born);
	ReleseCopyBornTree(&m_Born);
	m_pSkinMeshData->CopyBornTree(&m_Born, &m_CopyBornArray, pBornData->sBorn.child);

	m_LocalMatrix = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat;
}

void DynamicMesh::RenderFunc(Matrix &matrix)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	assert(m_pSkinMeshData != nullptr && "���b�V����񂪂���܂���");

	const SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();

	DynamicMeshShader::GetInstance()->SetVertexShader(pDeviceContext, data->m_IsTexture, false);
	DynamicMeshShader::GetInstance()->SetPixelShader(pDeviceContext, data->m_IsTexture, false);
	DynamicMeshShader::GetInstance()->SetInputLayout(pDeviceContext, data->m_IsTexture);

	DynamicMeshShader::GetInstance()->SetBaseConstantBuffer(pDeviceContext, matrix, false);

	//�A�j���[�V�����X�V
	if (m_IsAnimUpdate)
	{
		m_pSkinMeshData->Update(&m_Born);
	}

	DynamicMeshShader::GetInstance()->SetBornConstantBuffer(pDeviceContext, GetBornAllNum(), m_CopyBornArray);

	//�������ƂɃ����_�����O

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT Stride = sizeof(SkinVertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->m_pVertexBuffer, &Stride, &offset);

	//�}���`�e�N�X�`��
	if (m_pImage != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImage->m_pImageData->GetImageInfo()->pTexture);
	}

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

		DynamicMeshShader::GetInstance()->SetMaterialConstantBuffer(pDeviceContext, diffuse, specular, ambient);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (data->m_pMaterial[i].pTexture)
		{
			pDeviceContext->PSSetSamplers(0, 1, &data->m_pSampleLinear);
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_pMaterial[i].pTexture);
		}

		//�v���~�e�B�u�������_�����O
		pDeviceContext->DrawIndexed(data->m_pMaterial[i].dwNumFace * 3, 0, 0);
	}

	//�e�N�X�`�����\�[�X������
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(1, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(2, 1, &pShaderResource);
}

void DynamicMesh::ReleseCopyBornTree(CopyBorn *pBornCopy) const
{
	//�q�{�[��
	if (pBornCopy->child != nullptr)
	{
		ReleseCopyBornTree(pBornCopy->child);
		delete pBornCopy->child;
	}

	//�e�{�[��
	if (pBornCopy->brother != nullptr)
	{
		ReleseCopyBornTree(pBornCopy->brother);
		delete pBornCopy->brother;
	}
}