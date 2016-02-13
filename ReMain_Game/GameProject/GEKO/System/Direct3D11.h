#pragma warning(disable : 4005)

#ifndef _DIRECT3D11_H_
#define _DIRECT3D11_H_

#pragma comment(lib,"GEKO\\Lib\\d3d11.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dx10.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dx11.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dCompiler.lib")

#include "..\Include\\d3d11.h"
#include "..\Include\\d3dx10.h"
#include "..\Include\\d3dx11.h"
#include "..\Include\\d3dCompiler.h"

#include "Math.h"

#include "..\System\Debug.h"

#define INIT_NULLPOINTR(x)   x = nullptr;	 
#define SAFE_DELETE(x)       if(x){ delete   x; x = nullptr;}
#define SAFE_DELETE_ARRAY(x) if(x){ delete[] x; x = nullptr;}
#define SAFE_RELEASE(x)		 if(x){ x->Release(); x = nullptr;}
#define ALIGN16				 _declspec(align(16))
#define RGB 0.003921568627451f

class Direct3D11
{
public:
	~Direct3D11();

	static Direct3D11* GetInstance();

	/// <summary>
	/// �_�C���N�gX�̏�����
	/// </summary>
	/// <param name="Width">
	/// �E�B���h�E�T�C�Y��
	/// </param>
	/// <param name="Height">
	/// �E�B���h�E�T�C�Y�c
	/// </param>
	/// <returns>
	/// �������̐������f��Ԃ�
	/// </returns>
	HRESULT	InitD3D11(INT Width, INT Height);

	HRESULT InitBackBuffer();

	void ChangeBlendState3D();

	void ChangeBlendState2D();

	void ChangeWindowSize();

	void SetResolution_And_RefreshRate(int width, int height, int refreshRateNum);

	void SetRasterizer(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);

	/// <summary>
	/// �_�C���N�gX�̃h���C�o�[��n��
	/// </summary>
	/// <returns>
	/// �_�C���N�gX�̃h���C�o�[
	/// </returns>
	ID3D11Device* GetID3D11Device() const;

	/// <summary>
	/// �_�C���N�gX�̃h���C�o�[�R���e�L�X�g��n��
	/// </summary>
	/// <returns>
	/// �_�C���N�gX�̃h���C�o�[�R���e�L�X�g
	/// </returns>
	ID3D11DeviceContext* GetID3D11DeviceContext() const;

	ID3D11BlendState* GetBlendState() const;

	D3D11_VIEWPORT* GetViewportD3D11();

	int GetResolutionWidth() const;

	int GetResolutionHeight() const;

	/// <summary>
	///�@��ʃN���A��RGB���w��
	/// </summary>
	/// <param name="r">
	/// �ԐF
	/// </param>
	/// <param name="r">
	/// �ΐF
	/// </param>
	/// <param name="r">
	/// �F
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void Clear(float r, float g, float b);

	/// <summary>
	/// ��ʍX�V
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void Present();

	void RederTarget();

	/// <summary>
	/// ��ʂ��t���X�N���[���ŕ\��
	/// </summary>
	/// <param name="isFullScreen">
	/// true : �t���X�N���[��
	/// false : �E�B���h�E
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void FullScreen(bool isFullScreen);

	/// <summary>
	/// �r���[�|�[�g��ݒ�
	/// </summary>
	/// <param name="width">
	/// �r���[�|�[�g�̕�
	/// </param>
	/// <param name="height">
	/// �r���[�|�[�g�̍���
	/// </param>
	/// <param name="x">
	/// �r���[�|�[�g������WX
	/// </param>
	/// <param name="y">
	/// �r���[�|�[�g������WY
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void SetViewport(float width, float height, float x, float y);

	/// <summary>
	/// �_�C���N�gX�̃��\�[�X�̔j��
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void DestroyD3D11();

private:
	Direct3D11();

private:
	ID3D11Device*			m_pDevice;				//�_�C���N�gX11�̃h���C�o�[
	ID3D11DeviceContext*	m_pDeviceContext;		//�_�C���N�gX11�̃h���C�o�[�̐ݒ�
	IDXGISwapChain*			m_pSwapChain;			//�X���b�v�`�F�[��
	ID3D11Texture2D*		m_pDepthStencil;		//�[�x�E�X�e���V�������e�N�X�`���[
	ID3D11RenderTargetView* m_pRenderTargetView;	//�����_�[�^�[�Q�b�g�r���[
	ID3D11DepthStencilView* m_pDepthStencilView;	//�[�x�E�X�e���V���E�r���[
	ID3D11BlendState*		m_pBlendState;			//�u�����h�ݒ�3D
	ID3D11BlendState*		m_pBlendState2D;		//�u�����h�ݒ�2D
	D3D11_VIEWPORT			m_Viewport;				//�r���[�|�[�g
	int m_ResolutionWidth;							//�𑜓x��
	int m_ResolutionHeight;							//�𑜓x����
};

#endif