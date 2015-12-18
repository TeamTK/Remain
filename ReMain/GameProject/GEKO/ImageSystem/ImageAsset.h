#ifndef _IMAGE_ASSET_H_
#define _IMAGE_ASSET_H_

#include "..\System\Direct3D11.h"

//シェーダー用のコンスタントバッファーのアプリ側構造体
struct SHADER_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX mW;
	ALIGN16 float ViewPortWidth;
	ALIGN16 float ViewPortHeight;
	ALIGN16 D3DXVECTOR4 Color;
};

struct ImageInfo
{
	ID3D11InputLayout*  pVertexLayout;	//頂点レイアウト
	ID3D11VertexShader* pVertexShader;	//頂点シェーダー
	ID3D11PixelShader*	pPixelShader;		//ピクセルシェーダー
	ID3D11Buffer*		pConstantBuffer;	//コンスタントバッファー
	ID3D11Buffer*		pVertexBuffer;	//頂点バッファー
	ID3D11SamplerState* pSampler;			//テクスチャーのサンプラー
	ID3D11ShaderResourceView* pTexture;	//テクスチャーリソースビュー
	int Width;  //前の画像の幅
	int Height; //前の画像の高さ
};

class ImageData
{
public:
	ImageData(std::string fileName);
	~ImageData();
	ImageInfo *GetImageInfo();
	void Relese();

private:
	ImageInfo m_ImageInfo;
};

class ImageAsset
{
public:
	~ImageAsset();
	static ImageData* GetImage(std::string name);
	static void LoadAsset(std::string filmeName, std::string name);
	static void LoadFile(std::string filmeName);
	static void PartClear(std::string name);
	static void AllClear();
	static void DebugDraw();

private:
	ImageAsset();
	static ImageAsset *GetInstance();

private:
	class ImagePimpl;
	ImagePimpl *m_pImagePimpl;
};

#endif