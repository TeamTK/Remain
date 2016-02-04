#include "SphereTriangleCopmute.h"
#include "../../Mesh/StaticMesh.h"
#include "../../Collider/SphereCollider.h"
#include "../../System/Collision.h"

SphereTriangleCopmute::SphereTriangleCopmute() :
	m_pConstantBuffer(nullptr),
	m_pBuffer(nullptr),
	m_pBufferSRV(nullptr)
{
}

SphereTriangleCopmute::~SphereTriangleCopmute()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pBufferSRV);
	SAFE_DELETE_ARRAY(m_pTriangleInfo);
}

void SphereTriangleCopmute::Init(const StaticMesh &staticMesh, const Vector3D *pNormalArray)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetID3D11Device();
	ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	const VertexInfo *vertex = staticMesh.GetVertex();
	const IndexInfo *index = staticMesh.GetIndex();
	int polyNum = staticMesh.GetFaceAllNum();

	m_pTriangleInfo = new Sphere_TriangleInfo_GPU[polyNum];

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
	dc.ByteWidth = sizeof(Sphere_TriangleInfo_GPU) * polyNum;
	dc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	dc.StructureByteStride = sizeof(Sphere_TriangleInfo_GPU);
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
	cb.ByteWidth = sizeof(SphereBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
}

bool SphereTriangleCopmute::Calculation(const StaticMesh &staticMesh, const Vector3D &point, float radius, Result_Porygon_Group_Sphere *pory)
{
	return false;
}