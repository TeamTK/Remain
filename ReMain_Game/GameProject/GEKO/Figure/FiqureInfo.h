#ifndef _FIQURE_INFO_H_
#define _FIQURE_INFO_H_

#include "..\System\Direct3D11.h"

struct ConstantBufferFiqureBillboard
{
	D3DXMATRIX mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
};

struct VertexBillboard
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
};

struct FigureBillboardInfo
{
	ID3D11InputLayout*	 pVertexLayout;	 //���_���C�A�E�g
	ID3D11VertexShader*  pVertexShader;	 //���_�V�F�[�_�[
	ID3D11PixelShader*	 pPixelShader;	 //�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		 pConstantBuffer;

	FigureBillboardInfo()
	{
		pVertexLayout = nullptr;
		pVertexShader = nullptr;
		pPixelShader = nullptr;
		pConstantBuffer = nullptr;
	}

	~FigureBillboardInfo()
	{
		SAFE_RELEASE(pConstantBuffer);
		SAFE_RELEASE(pVertexShader);
		SAFE_RELEASE(pPixelShader);
		SAFE_RELEASE(pVertexLayout);
	}
};


#endif