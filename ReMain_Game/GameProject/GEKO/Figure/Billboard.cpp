#include "Billboard.h"
#include "FiqureShaderManager.h"
#include "FiqureInfo.h"
#include "../System/Camera.h"
#include "../ImageSystem/Image.h"

VertexBillboardInfo g_VertexBillboardInfo; //ビルボードの初期情報

Billboard::Billboard() :
	m_IsRenderingRegister(false),
	m_Size(1.0f),
	m_pVertexBuffer(nullptr),
	m_pImageInfo(nullptr),
	m_RenderingType(RenderingType::eForward),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitVertex(g_VertexBillboardInfo);

	//レンダリング管理に登録
	m_Rendering.priorityGroup = 0;
	m_Rendering.priority = 0;
	m_Rendering.func = std::bind(&Billboard::Render, this);
}

Billboard::Billboard(const std::string &assetName, unsigned int priorityGroup, unsigned int priority) :
	m_IsRenderingRegister(true),
	m_Size(1.0f),
	m_pVertexBuffer(nullptr),
	m_pImageInfo(ImageAsset::GetImage(assetName)->GetImageInfo()),
	m_RenderingType(RenderingType::eForward),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitVertex(g_VertexBillboardInfo);

	//レンダリング管理に登録
	m_Rendering.priorityGroup = priorityGroup;
	m_Rendering.priority = priority;
	m_Rendering.func = std::bind(&Billboard::Render, this);
	RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
}

Billboard::~Billboard()
{
	SAFE_RELEASE(m_pVertexBuffer);

	if (m_IsRenderingRegister)
	{
		if (m_RenderingType == RenderingType::eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
}

void Billboard::SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister != isRegister&& "レンダリングがすでに登録されています");

	auto *pRendering = RenderingManager::GetInstance();

	if (isRegister)
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			pRendering->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			pRendering->AddForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
	else
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			pRendering->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			pRendering->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
	m_IsRenderingRegister = isRegister;
}

void Billboard::SetRenderingType(RenderingType renderingType)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");
	if (m_RenderingType == renderingType) return;
	m_RenderingType = renderingType;

	if (m_RenderingType == RenderingType::eDeferred)
	{
		RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		RenderingManager::GetInstance()->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
	}
	else
	{
		RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
	}
}

void Billboard::SetPriority(unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");

	//グループでの優先順位が変わらない場合
	if (m_Rendering.priorityGroup == priorityGroup)
	{
		m_Rendering.priority = priority;

		if (m_RenderingType == RenderingType::eDeferred)
		{
			RenderingManager::GetInstance()->SortDeferred(m_Rendering.priorityGroup);
		}
		else
		{
			RenderingManager::GetInstance()->SortForward(m_Rendering.priorityGroup);
		}
	}
	else
	{
		if (m_RenderingType == eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			RenderingManager::GetInstance()->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
}

void Billboard::SetImageAsset(const std::string &assetName)
{
	m_pImageInfo = ImageAsset::GetImage(assetName)->GetImageInfo();
}

void Billboard::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void Billboard::SetPosition(const Vector3D &position)
{
	m_Position = position;
}

void Billboard::SetColor(int r, int g, int b)
{
	m_Color.x = r * RGB;
	m_Color.y = g * RGB;
	m_Color.z = b * RGB;
}

void Billboard::SetAlpha(int alpha)
{
	m_Color.w = alpha * RGB;
}

void Billboard::SetSize(float size)
{
	m_Size = size;
}

void Billboard::InitVertex(const VertexBillboardInfo& info)
{
	VertexBillboard vertex[] =
	{
		D3DXVECTOR3(info.leftDwonPos),   D3DXVECTOR2(info.leftDwonUV),
		D3DXVECTOR3(info.leftTopPos),    D3DXVECTOR2(info.leftTopUV),
		D3DXVECTOR3(info.rightDwonPos),  D3DXVECTOR2(info.rightDwonUV),
		D3DXVECTOR3(info.rightTopPos),   D3DXVECTOR2(info.rightTopUV)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;

	Direct3D11::GetInstance()->GetID3D11Device()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
}

void Billboard::Render()
{
	FigureBillboardInfo *pInfo = FiqureShaderManager::GetInstance()->GetBillboardInfo();
	
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	//pDirect11->ChangeBlendState2D();

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(pInfo->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pInfo->pPixelShader, NULL, 0);

	Matrix world;

	//平行移動
	world._41 = m_Position.x;
	world._42 = m_Position.y;
	world._43 = m_Position.z;

	//拡大縮小
	world._11 = m_Size;
	world._22 = m_Size;
	world._33 = m_Size;

	Matrix CancelRotation = (*Camera::GetView());
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f;
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	world = CancelRotation * world;

	//変換行列とカラーをシェーダーに送る
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqureBillboard cb;
	if (SUCCEEDED(pDeviceContext->Map(pInfo->pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		Matrix m = world * *Camera::GetViewProjection();
		D3DXMatrixTranspose(&cb.mWVP, &m);

		cb.color = m_Color; //RGBA格納
		
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(pInfo->pConstantBuffer, 0);
	}

	//定数バッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(1, 1, &pInfo->pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &pInfo->pConstantBuffer);

	//頂点バッファーをセット
	UINT stride = sizeof(VertexBillboard);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(pInfo->pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャーをシェーダーに渡す
	if (m_pImageInfo != nullptr)
	{
		pDeviceContext->PSSetSamplers(0, 1, &m_pImageInfo->pSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &m_pImageInfo->pTexture);
	}

	//プリミティブをレンダリング
	pDeviceContext->Draw(4, 0);

	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);

	//pDirect11->ChangeBlendState3D();
}

//ビルボードアニメーション
BillboardAnimation::BillboardAnimation() :
	m_IsRenderingRegister(false),
	m_IsEnd(false),
	m_CurrentFrameNum(0),
	m_PastFrameNum(0),
	m_TotalFrameNum(0),
	m_Speed(0.0f),
	m_Size(1.0f),
	m_pVertexBuffer(nullptr),
	m_pImageInfo(nullptr),
	m_RenderingType(RenderingType::eForward),
	m_pLeftTopUV(nullptr),
	m_pLeftDwonUV(nullptr),
	m_pRightTopUV(nullptr),
	m_pRightDwonUV(nullptr),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitVertex(g_VertexBillboardInfo);

	//レンダリング管理に登録
	m_Rendering.priorityGroup = 0;
	m_Rendering.priority = 0;
	m_Rendering.func = std::bind(&BillboardAnimation::Render, this);
	//RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
}

BillboardAnimation::BillboardAnimation(const std::string &assetName, unsigned int priorityGroup, unsigned int priority, int totalFrameNum, int divW, int divH) :
	m_IsRenderingRegister(true),
	m_Size(1.0f),
	m_pVertexBuffer(nullptr),
	m_pImageInfo(ImageAsset::GetImage(assetName)->GetImageInfo()),
	m_RenderingType(RenderingType::eForward),
	m_pLeftTopUV(nullptr),
	m_pLeftDwonUV(nullptr),
	m_pRightTopUV(nullptr),
	m_pRightDwonUV(nullptr),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitVertex(g_VertexBillboardInfo);

	SetRenderingRegister(m_IsRenderingRegister, priorityGroup, priority);
	FrameDivision(assetName, totalFrameNum, divW, divH);

	//レンダリング管理に登録
	m_Rendering.priorityGroup = priorityGroup;
	m_Rendering.priority = priority;
	m_Rendering.func = std::bind(&BillboardAnimation::Render, this);
	RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
}

BillboardAnimation::~BillboardAnimation()
{
	SAFE_RELEASE(m_pVertexBuffer);

	if (m_IsRenderingRegister)
	{
		if (m_RenderingType == RenderingType::eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}

	if (m_pLeftTopUV != nullptr)
	{
		delete[] m_pLeftTopUV;
		delete[] m_pLeftDwonUV;
		delete[] m_pRightTopUV;
		delete[] m_pRightDwonUV;
	}
}

bool BillboardAnimation::GetIsEnd() const
{
	return m_IsEnd;
}

void BillboardAnimation::SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister != isRegister&& "レンダリングがすでに登録されています");

	auto *pRendering = RenderingManager::GetInstance();

	if (isRegister)
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			pRendering->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			pRendering->AddForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
	else
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			pRendering->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			pRendering->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
	m_IsRenderingRegister = isRegister;
}

void BillboardAnimation::SetRenderingType(RenderingType renderingType)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");

	if (m_RenderingType == renderingType) return;
	m_RenderingType = renderingType;

	if (m_RenderingType == RenderingType::eDeferred)
	{
		RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
		RenderingManager::GetInstance()->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
	}
	else
	{
		RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
		RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
	}
}

void BillboardAnimation::SetPriority(unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");

	//グループでの優先順位が変わらない場合
	if (m_Rendering.priorityGroup == priorityGroup)
	{
		m_Rendering.priority = priority;

		if (m_RenderingType == RenderingType::eDeferred)
		{
			RenderingManager::GetInstance()->SortDeferred(m_Rendering.priorityGroup);
		}
		else
		{
			RenderingManager::GetInstance()->SortForward(m_Rendering.priorityGroup);
		}
	}
	else
	{
		if (m_RenderingType == eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_Rendering.priorityGroup, &m_Rendering);
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			RenderingManager::GetInstance()->AddDeferred(m_Rendering.priorityGroup, &m_Rendering);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_Rendering.priorityGroup, &m_Rendering);
			m_Rendering.priorityGroup = priorityGroup;
			m_Rendering.priority = priority;
			RenderingManager::GetInstance()->AddForward(m_Rendering.priorityGroup, &m_Rendering);
		}
	}
}

void BillboardAnimation::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void BillboardAnimation::SetPosition(const Vector3D &position)
{
	m_Position = position;
}

void BillboardAnimation::SetColor(int r, int g, int b)
{
	m_Color.x = r * RGB;
	m_Color.y = g * RGB;
	m_Color.z = b * RGB;
}

void BillboardAnimation::SetAlpha(int alpha)
{
	m_Color.w = alpha * RGB;
}

void BillboardAnimation::SetSize(float size)
{
	m_Size = size;
}

void BillboardAnimation::FrameDivision(const std::string &assetName, int totalFrameNum, int divW, int divH)
{
	m_pImageInfo = ImageAsset::GetImage(assetName)->GetImageInfo();
	m_IsEnd = false;
	m_CurrentFrameNum = 0;
	m_PastFrameNum = 0;
	m_Speed = 0.0f;

	m_TotalFrameNum = totalFrameNum;

	if (m_pLeftTopUV != nullptr)
	{
		delete[] m_pLeftTopUV;
		delete[] m_pLeftDwonUV;
		delete[] m_pRightTopUV;
		delete[] m_pRightDwonUV;
	}
	m_pLeftTopUV = new Vector2D[m_TotalFrameNum];
	m_pLeftDwonUV = new Vector2D[m_TotalFrameNum];
	m_pRightTopUV = new Vector2D[m_TotalFrameNum];
	m_pRightDwonUV = new Vector2D[m_TotalFrameNum];

	//幅縦の分割長さ
	int divSizeW = 0;
	int divSizeH = 0;

	if (divW == 0) divSizeW = m_pImageInfo->Width / 1;
	else		   divSizeW = m_pImageInfo->Width / divW;

	if (divH == 0) divSizeH = m_pImageInfo->Height / 1;
	else		   divSizeH = m_pImageInfo->Height / divH;

	//幅縦の分割数カウント変数
	int cntW = 0;
	int cntH = 0;

	//画像分割幅と高さ
	float cntSizeW = 0.0f;
	float cntSizeH = 0.0f;

	for (int i = 0; i < totalFrameNum; i++)
	{
		//ピクセル値からUV座標に変換
		float uvLeft = cntSizeW / (float)m_pImageInfo->Width;
		float uvRight = (cntSizeW + divSizeW) / (float)m_pImageInfo->Width;
		float uvTop = cntSizeH / (float)m_pImageInfo->Height;
		float uvBotton = (cntSizeH + divSizeH) / (float)m_pImageInfo->Height;

		m_pLeftTopUV[i] = Vector2D(uvLeft, uvTop);
		m_pLeftDwonUV[i] = Vector2D(uvLeft, uvBotton);
		m_pRightTopUV[i] = Vector2D(uvRight, uvTop);
		m_pRightDwonUV[i] = Vector2D(uvRight, uvBotton);

		//幅は右に沿って更新
		cntW++;
		cntSizeW += divSizeW;

		//横方向の分割更新
		if (cntW == divW)
		{
			cntW = 0;
			cntSizeW = 0.0f;
			cntH++;
			cntSizeH += divSizeH;
		}

		//縦方向の分割更新
		if (cntH > divH)
		{
			cntH = 0;
			cntSizeH = 0.0f;
			break;
		}
	}

	VertexBillboardInfo info;

	VertexBillboard vertex[] =
	{
		Vector3D(info.leftDwonPos),   Vector2D(m_pLeftTopUV[0]),
		Vector3D(info.leftTopPos),    Vector2D(m_pLeftDwonUV[0]),
		Vector3D(info.rightDwonPos),  Vector2D(m_pRightTopUV[0]),
		Vector3D(info.rightTopPos),   Vector2D(m_pRightDwonUV[0])
	};

	//頂点更新
	Direct3D11::GetInstance()->GetID3D11DeviceContext()->UpdateSubresource(m_pVertexBuffer, 0, nullptr, vertex, 0, 0);
}

void BillboardAnimation::PlayFrame(float frame)
{
	//m_Speed += Math::VelocityToFrameM(frame);
	m_Speed += frame;
}

void BillboardAnimation::DebugFrame()
{
	printf("%d\n", m_CurrentFrameNum);
}

void BillboardAnimation::InitVertex(const VertexBillboardInfo& info)
{
	SAFE_RELEASE(m_pVertexBuffer);

	VertexBillboard vertex[] =
	{
		Vector3D(info.leftDwonPos),   Vector2D(info.leftDwonUV),
		Vector3D(info.leftTopPos),    Vector2D(info.leftTopUV),
		Vector3D(info.rightDwonPos),  Vector2D(info.rightDwonUV),
		Vector3D(info.rightTopPos),   Vector2D(info.rightTopUV)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;

	Direct3D11::GetInstance()->GetID3D11Device()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
}

void BillboardAnimation::Render()
{
	m_CurrentFrameNum = (int)m_Speed;

	//設定したフレームを超えたら初期化
	if (m_CurrentFrameNum >= m_TotalFrameNum)
	{
		m_Speed = 0;
		m_CurrentFrameNum = 0;
		m_IsEnd = true;
	}

	if (m_CurrentFrameNum != m_PastFrameNum)
	{
		VertexBillboardInfo info;

		VertexBillboard vertex[] =
		{
			Vector3D(info.leftDwonPos),   Vector2D(m_pLeftTopUV[m_CurrentFrameNum]),
			Vector3D(info.leftTopPos),    Vector2D(m_pLeftDwonUV[m_CurrentFrameNum]),
			Vector3D(info.rightDwonPos),  Vector2D(m_pRightTopUV[m_CurrentFrameNum]),
			Vector3D(info.rightTopPos),   Vector2D(m_pRightDwonUV[m_CurrentFrameNum])
		};

		//頂点更新
		Direct3D11::GetInstance()->GetID3D11DeviceContext()->UpdateSubresource(m_pVertexBuffer, 0, nullptr, vertex, 0, 0);

	}
	m_PastFrameNum = m_CurrentFrameNum;

	FigureBillboardInfo *pInfo = FiqureShaderManager::GetInstance()->GetBillboardAnimation();

	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	//pDirect11->ChangeBlendState2D();

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(pInfo->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pInfo->pPixelShader, NULL, 0);

	Matrix world;

	//平行移動
	world._41 = m_Position.x;
	world._42 = m_Position.y;
	world._43 = m_Position.z;

	//拡大縮小
	world._11 = m_Size;
	world._22 = m_Size;
	world._33 = m_Size;

	Matrix CancelRotation = (*Camera::GetView());
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f;
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	world = CancelRotation * world;

	//変換行列とカラーをシェーダーに送る
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqureBillboardAnimation cb;
	if (SUCCEEDED(pDeviceContext->Map(pInfo->pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		Matrix m = world * *Camera::GetViewProjection();
		D3DXMatrixTranspose(&cb.mWVP, &m);

		cb.color = m_Color; //RGBA格納

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(pInfo->pConstantBuffer, 0);
	}

	//定数バッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(1, 1, &pInfo->pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &pInfo->pConstantBuffer);

	//頂点バッファーをセット
	UINT stride = sizeof(VertexBillboard);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(pInfo->pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャーをシェーダーに渡す
	if (m_pImageInfo != nullptr)
	{
		pDeviceContext->PSSetSamplers(0, 1, &m_pImageInfo->pSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &m_pImageInfo->pTexture);
	}

	//プリミティブをレンダリング
	pDeviceContext->Draw(4, 0);

	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);

	//pDirect11->ChangeBlendState3D();
}