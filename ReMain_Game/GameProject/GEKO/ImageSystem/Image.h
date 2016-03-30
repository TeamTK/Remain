#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ImageAsset.h"
#include "../Shader/RenderingManager.h"

class StaticMesh;
class DynamicMesh;

//頂点の構造体
struct Vertex
{
	Vector3D Pos; //位置
	Vector2D UV; //テクスチャー座標
};

struct RGBA
{
	float sRed;
	float sGreen;
	float sBlue;
	float sAlpha;
};

//画像を描画
class Image
{ 
	friend StaticMesh;
	friend DynamicMesh;

public:
	Image();
	Image(const std::string &assetName, unsigned int priorityGroup, unsigned int priority);
	~Image();
	int GetWidth() const;
	int GetHeight() const;
	void SetDrawRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetAsset(const std::string &assetName);
	void SetDrawPriority(unsigned int priorityGroup, unsigned int priority);
	void SetAlpha(int alpha);
	void SetAngle(int angle);
	void SetRGB(int red, int green, int blue);
	void SetSize(int w, int h);
	void SetCenter(int x, int y);
	void SetDrawArea(int leftX, int leftY, int rightX, int rightY);
	void SetPosition(float x, float y);
	void SetPosition(const Vector2D &position);

private:
	void InitModel(int centerX, int centerY);
	void Draw();

private:
	bool m_IsDrawRegister; //描画登録判断
	int m_SizeW;	//画像の幅
	int m_SizeH;	//画像の高さ
	int m_Angle;    //画像の角度（度数）
	int m_CenterX;	//画像の中心X
	int m_CenterY;	//画像の中心Y
	Vector2D m_Pos; //画像位置（左上が原点のスクリーン座標）
	ImageData *m_pImageData; //画像データ受け取り用
	RECT m_UvSize;  //UV座標に変換前のピクセル値
	RGBA m_RGBA;	//RGBAを設定
	RenderingInfo m_RenderingInfo; //レンダリング管理
};

#endif