#include "LineTriangleCopmute.h"
#include "../../Mesh/StaticMesh.h"
#include "../../Collider/LineSegment.h"
#include "../../System/Collision.h"

LineTriangleCopmute::LineTriangleCopmute() :
	m_pBuffer(nullptr),
	m_pBufferSRV(nullptr),
	m_pTriangleInfo(nullptr),
	m_pConstantBuffer(nullptr)
{
}

LineTriangleCopmute::~LineTriangleCopmute()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pBufferSRV);
	SAFE_DELETE_ARRAY(m_pTriangleInfo);
}

void LineTriangleCopmute::Init(const StaticMesh &staticMesh, const Vector3D *pNormalArray)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetID3D11Device();
	ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	const VertexInfo *vertex = staticMesh.GetVertex();
	const IndexInfo *index = staticMesh.GetIndex();
	int polyNum = staticMesh.GetFaceAllNum();

	m_pTriangleInfo = new TriangleInfo_GPU[polyNum];

	//GPUに送るデータ構築
	for (int i = 0; i < polyNum; i++)
	{
		m_pTriangleInfo[i].vertexPos1 = vertex[index[i].vertexIndex[0]].pos;
		m_pTriangleInfo[i].vertexPos2 = vertex[index[i].vertexIndex[1]].pos;
		m_pTriangleInfo[i].vertexPos3 = vertex[index[i].vertexIndex[2]].pos;

		m_pTriangleInfo[i].normal = pNormalArray[i];
		m_pTriangleInfo[i].isHit = false;
	}

	D3D11_BUFFER_DESC dc;
	ZeroMemory(&dc, sizeof(dc));
	dc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	dc.ByteWidth = sizeof(TriangleInfo_GPU) * polyNum;
	dc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	dc.StructureByteStride = sizeof(TriangleInfo_GPU);
	D3D11_SUBRESOURCE_DATA InitData;

	InitData.pSysMem = m_pTriangleInfo;
	pDevice->CreateBuffer(&dc, &InitData, &m_pBuffer);
	pDevice->CreateBuffer(&dc, NULL, &m_pBufferResult);

	//コンピュータシェーダーに送る用
	D3D11_SHADER_RESOURCE_VIEW_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	sd.BufferEx.FirstElement = 0;
	sd.Format = DXGI_FORMAT_UNKNOWN;
	sd.BufferEx.NumElements = polyNum;

	pDevice->CreateShaderResourceView(m_pBuffer, &sd, &m_pBufferSRV);

	//コピー用
	D3D11_UNORDERED_ACCESS_VIEW_DESC ud;
	ZeroMemory(&ud, sizeof(ud));
	ud.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	ud.Buffer.FirstElement = 0;
	ud.Format = DXGI_FORMAT_UNKNOWN;
	ud.Buffer.NumElements = polyNum;
	pDevice->CreateUnorderedAccessView(m_pBufferResult, &ud, &m_pBufferResultUAV);

	//コンスタントバッファー作成　vStart,vEnd渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(LineBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
}

void LineTriangleCopmute::Release()
{
	/*
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pBufferSRV);
	SAFE_DELETE_ARRAY(m_pTriangleInfo);
	SAFE_RELEASE(m_pBufferResult);
	SAFE_RELEASE(m_pBufferResultUAV);
	*/
}

bool LineTriangleCopmute::Calculation(const StaticMesh &staticMesh, const Vector3D &start, const Vector3D &end, Result_Porygon *pPory)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetID3D11Device();
	ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//モデルの変換行列
	Matrix world = *staticMesh.GetWorldMatrix();
	Matrix local = *staticMesh.GetLocalMatrix();
	Matrix m = local * world;
	Matrix inverse = m.GetInverse();

	//頂点データとポリゴンのインデックス
	const IndexInfo *index = staticMesh.GetIndex();
	int polyNum = staticMesh.GetFaceAllNum();

	Vector3D tempStart = start * inverse;
	Vector3D tempEnd = end * inverse;

	//コンスタントバッファーでシェーダーにcStart vEndを渡す	
	D3D11_MAPPED_SUBRESOURCE pConstantData;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pConstantData)))
	{
		LineBuffer sg;

		D3DXVECTOR4 s(tempStart.x, tempStart.y, tempStart.z, 1.0f);
		D3DXVECTOR4 e(tempEnd.x, tempEnd.y, tempEnd.z, 1.0f);

		sg.start = s;
		sg.end = e;
		memcpy_s(pConstantData.pData, pConstantData.RowPitch, (void*)&sg, sizeof(LineBuffer));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	pDeviceContext->CSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//コンピュートシェーダー実行
	CopmuteManager::GetInstance()->CSSetLineTriangle();
	pDeviceContext->CSSetShaderResources(0, 1, &m_pBufferSRV);
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pBufferResultUAV, 0);
	pDeviceContext->Dispatch(polyNum, 1, 1);

	//GPUからデータ取得
	ID3D11Buffer* pBufferCopy = nullptr;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	m_pBufferResult->GetDesc(&bd);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.Usage = D3D11_USAGE_STAGING;
	bd.BindFlags = 0;
	bd.MiscFlags = 0;
	pDevice->CreateBuffer(&bd, NULL, &pBufferCopy);

	pDeviceContext->CopyResource(pBufferCopy, m_pBufferResult);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDeviceContext->Map(pBufferCopy, 0, D3D11_MAP_READ, 0, &MappedResource);
	memcpy(m_pTriangleInfo, (TriangleInfo_GPU*)MappedResource.pData, (sizeof(TriangleInfo_GPU) * polyNum));
	pDeviceContext->Unmap(pBufferCopy, 0);
	SAFE_RELEASE(pBufferCopy);

	//メッシュに当たっていたらデータ格納
	for (int i = 0; i < polyNum; i++)
	{
		if (m_pTriangleInfo[i].isHit)
		{
			pPory->contactPos = m_pTriangleInfo[i].contactPos;
			pPory->normal = m_pTriangleInfo[i].normal;
			pPory->vertexPos[0] = m_pTriangleInfo[i].vertexPos1;
			pPory->vertexPos[1] = m_pTriangleInfo[i].vertexPos2;
			pPory->vertexPos[2] = m_pTriangleInfo[i].vertexPos3;
			pPory->materialIndex = index[i].materialIndex;
			pPory->meshMatrix = m;
			pPory->PoryIndex = i;
			pPory->localMatrix = local;
			pPory->worldMatrix = world;
			return true;
		}
	}
	return false;
}