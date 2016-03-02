#include "Billboard.h"
#include "FiqureShaderManager.h"
#include "FiqureInfo.h"
#include "..\ImageSystem\Image.h"
#include "..\System\Camera.h"
#include "..\System\Input.h"
#include <assert.h>

Billboard::Billboard() :
	m_pVertexBuffer(nullptr)
{
	ChangeVertex(VertexBillboardInfo());
}

Billboard::~Billboard()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void Billboard::ChangeVertex(const VertexBillboardInfo& info)
{
	SAFE_RELEASE(m_pVertexBuffer);

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

void Billboard::Render(const Vector3D &pos, float size, const std::string &name)
{
	FigureBillboardInfo *pInfo = FiqureShaderManager::GetInstance()->GetBillboardInfo();
	
	ImageData *pImage = ImageAsset::GetImage(name);
	ImageInfo *p = pImage->GetImageInfo();

	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(pInfo->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pInfo->pPixelShader, NULL, 0);

	Matrix world;

	//平行移動
	world._41 = pos.x;
	world._42 = pos.y;
	world._43 = pos.z;

	//拡大縮小
	world._11 = size;
	world._22 = size;
	world._33 = size;

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

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(pInfo->pConstantBuffer, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(0, 1, &pInfo->pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pInfo->pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(VertexBillboard);
	UINT offset = 0;
	Direct3D11::GetInstance()->GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(pInfo->pVertexLayout);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &p->pSampler);
	pDeviceContext->PSSetShaderResources(0, 1, &p->pTexture);

	//プリミティブをレンダリング
	pDeviceContext->Draw(4, 0);
}

//ビルボードアニメーション
BillboardAnimation::BillboardAnimation() :
	m_IsEnd(false),
	m_FrameNum(0),
	m_FrameAllNum(0),
	m_Speed(0.0f),
	m_pBillboard(nullptr)
{

}

BillboardAnimation::BillboardAnimation(const std::string &assetName, int frameNum, int sizeW, int sizeH)
{
	FrameDivision(assetName, frameNum, sizeW, sizeH);
}

BillboardAnimation::~BillboardAnimation()
{
	if (m_pBillboard != nullptr) delete[] m_pBillboard;
}

bool BillboardAnimation::GetIsEnd() const
{
	return m_IsEnd;
}

void BillboardAnimation::FrameDivision(const std::string &assetName, int frameNum, int sizeW, int sizeH)
{
	ImageInfo *pImage = ImageAsset::GetImage(assetName)->GetImageInfo();
	m_IsEnd = false;
	m_FrameNum = 0;
	m_Speed = 0.0f;

	if (m_pBillboard != nullptr)
	{
		delete[] m_pBillboard;
		m_pBillboard = nullptr;
	}

	m_pBillboard = new Billboard[frameNum];

	m_Name = assetName;
	m_FrameAllNum = frameNum;

	//幅縦の分割数カウント変数
	int DivW = pImage->Width / sizeW;
	int DivH = pImage->Height / sizeH;

	//幅縦の分割数カウント変数
	int cntW = 0;
	int cntH = 0;

	//画像分割幅と高さ
	float cntSizeW = 0.0f;
	float cntSizeH = 0.0f;

	VertexBillboardInfo info;

	for (int i = 0; i < frameNum; i++)
	{
		//ピクセル値からUV座標に変換
		float uvLeft = cntSizeW / (float)pImage->Width;
		float uvRight = (cntSizeW + sizeW) / (float)pImage->Width;
		float uvTop = cntSizeH / (float)pImage->Height;
		float uvBotton = (cntSizeH + sizeH) / (float)pImage->Height;

		info.leftTopUV = Vector2D(uvLeft, uvTop);
		info.leftDwonUV = Vector2D(uvLeft, uvBotton);
		info.rightTopUV = Vector2D(uvRight, uvTop);
		info.rightDwonUV = Vector2D(uvRight, uvBotton);
		m_pBillboard[i].ChangeVertex(info);

		//幅は右に沿って更新
		cntW++;
		cntSizeW += sizeW;

		//横方向の分割更新
		if (cntW == DivW)
		{
			cntW = 0;
			cntSizeW = 0.0f;
			cntH++;
			cntSizeH += sizeH;
		}

		//縦方向の分割更新
		if (cntH > DivH)
		{
			cntH = 0;
			cntSizeH = 0.0f;
			break;
		}
	}
}

void BillboardAnimation::PlayFrame(float frame)
{
	//m_Speed += Math::VelocityToFrameM(frame);
	m_Speed += frame;
}

void BillboardAnimation::Render(const Vector3D &pos, float size)
{
	assert(m_pBillboard != nullptr && "ビルボードアニメーションでの画像分割が行われていません");

	m_FrameNum = (int)m_Speed;

	//設定したフレームを超えたら初期化
	if (m_FrameNum >= m_FrameAllNum)
	{
		m_Speed = 0;
		m_FrameNum = 0;
		m_IsEnd = true;
	}
	m_pBillboard[m_FrameNum].Render(pos, size, m_Name);
}

void BillboardAnimation::DebugFrame()
{
	printf("%d\n", m_FrameNum);
}