#ifndef _FIQUREBASE_H_
#define _FIQUREBASE_H_

#include "..\System\Direct3D11.h"
#include "..\Shader\RenderingManager.h"

struct ConstantBufferFiqure
{
	Matrix mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	Vector4D Color;
};

struct FigureInfo
{
	ID3D11InputLayout*	 pVertexLayout;	 //���_���C�A�E�g
	ID3D11VertexShader*  pVertexShader;	 //���_�V�F�[�_�[
	ID3D11PixelShader*	 pPixelShader;	 //�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		 pConstantBuffer;
	ID3D11Buffer*		 pVertexBuffer;	 //���_�o�b�t�@�[

	FigureInfo()
	{
		pVertexLayout = nullptr;
		pVertexShader = nullptr;
		pPixelShader = nullptr;
		pConstantBuffer = nullptr;
		pVertexBuffer = nullptr;
	}

	~FigureInfo()
	{
		SAFE_RELEASE(pConstantBuffer);
		SAFE_RELEASE(pVertexShader);
		SAFE_RELEASE(pPixelShader);
		SAFE_RELEASE(pVertexBuffer);
		SAFE_RELEASE(pVertexLayout);
	}
};

class FiqureBase
{
public:
	FiqureBase();
	~FiqureBase();

protected:
	virtual HRESULT InitShader();

protected:
	FigureInfo m_FigureInfo;
};

#endif