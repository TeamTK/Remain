#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ImageAsset.h"

//頂点の構造体
struct Vertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR2 UV; //テクスチャー座標
};

struct RGBA
{
	float sRed;
	float sGreen;
	float sBlue;
	float sAlpha;
};

class Image
{
public:
	Image();
	~Image();
	int GetWidth();
	int GetHeight();
	void SetAsset(const char *name);
	void SetAlpha(int alpha);
	void SetAngle(int angle);
	void SetRGB(int red, int green, int blue);
	void SetSize(int w, int h);
	//void SetCenter(int x, int y);
	void SetDrawPos(int x, int y, int width, int height);
	void Draw(int x, int y, int centerX = 0, int centerY = 0);

private:
	void InitModel(int centerX, int centerY);

private:
	ImageData *m_pImageData;

	RECT m_UvSize;  //UV座標に変換前のピクセル値
	RGBA m_RGBA;	//RGBAを設定

	int m_SizeW;	//画像の幅
	int m_SizeH;	//画像の高さ
	int m_Angle;    //画像の角度（度数）
};

#endif