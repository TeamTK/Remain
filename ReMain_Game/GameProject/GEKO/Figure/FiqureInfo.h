#ifndef _FIQURE_INFO_H_
#define _FIQURE_INFO_H_

#include "..\System\Direct3D11.h"

struct ConstantBufferFiqureBillboard
{
	Matrix mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	Vector4D color;
};

struct ConstantBufferFiqureBillboardAnimation
{
	Matrix mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	Vector4D color;
	//ALIGN16 Vector2D uv;
};

struct VertexBillboard
{
	Vector3D pos;
	Vector2D uv;
};

//�V�F�[�_�[�ŕK�v�ȃr���{�[�h�̏��
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