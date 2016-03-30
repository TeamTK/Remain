#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "FiqureBase.h"
#include "../Shader/RenderingManager.h"
#include "../Mesh/MeshInfo.h"

struct ImageInfo;

//ビルボードの頂点情報
struct VertexBillboardInfo
{
	//頂点座標
	Vector3D leftTopPos;	 //左上の座標
	Vector3D leftDwonPos;	 //左下の座標
	Vector3D rightTopPos;	 //右上の座標
	Vector3D rightDwonPos;	 //右下の座標

	//テクスチャUV座標
	Vector2D leftTopUV;		 //左上UV座標
	Vector2D leftDwonUV;	 //左下UV座標
	Vector2D rightTopUV;	 //右上UV座標
	Vector2D rightDwonUV;	 //右下UV座標

	VertexBillboardInfo() :
		leftTopPos(-1.0, 1.0, 0.0f),
		leftDwonPos(-1.0, -1.0, 0.0f),
		rightTopPos(1.0, 1.0, 0.0f),
		rightDwonPos(1.0, -1.0, 0.0f),
		leftTopUV(0.0f, 0.0f),
		leftDwonUV(0.0f, 1.0f),
		rightTopUV(1.0f, 0.0f),
		rightDwonUV(1.0f, 1.0f)
	{
	}
};

//ビルボード
class Billboard
{
public:
	Billboard();
	Billboard(const std::string &assetName, unsigned int priorityGroup, unsigned int priority);
	~Billboard();
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetImageAsset(const std::string &assetName);
	void SetPosition(float x, float y);
	void SetPosition(const Vector3D &position);
	void SetColor(int r, int g, int b);
	void SetAlpha(int alpha);
	void SetSize(float size);

private:
	void InitVertex(const VertexBillboardInfo& info);
	void Render();

private:
	bool m_IsRenderingRegister;    //レンダリング登録判断
	float m_Size;				   //ビルボードの大きさ
	ID3D11Buffer* m_pVertexBuffer; //頂点バッファー
	ImageInfo *m_pImageInfo;	   //画像情報へのポインタ
	RenderingType m_RenderingType; //レンダリングの種類
	RenderingInfo m_Rendering;	   //レンダリング管理情報
	Vector3D m_Position;		   //ビルボードの座標
	Vector4D m_Color;			   //ビルボードの色
};

//ビルボードアニメーション
class BillboardAnimation
{
public:
	BillboardAnimation();
	BillboardAnimation(const std::string &assetName, unsigned int priorityGroup, unsigned int priority, int totalFrameNum, int divW, int divH);
	~BillboardAnimation();
	bool GetIsEnd() const;
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetPosition(float x, float y);
	void SetPosition(const Vector3D &position);
	void SetColor(int r, int g, int b);
	void SetAlpha(int alpha);
	void SetSize(float size);
	void FrameDivision(const std::string &assetName, int totalFrameNum, int divW, int divH);
	void PlayFrame(float frame);
	void DebugFrame();

private:
	void InitVertex(const VertexBillboardInfo& info);
	void Render();

private:
	bool m_IsRenderingRegister;    //レンダリング登録判断
	bool m_IsEnd;				   //アニメーションの終わり判断
	int m_CurrentFrameNum;		   //現在のアニメーションフレーム番号
	int m_PastFrameNum;			   //過去のアニメーションフレーム番号
	int m_TotalFrameNum;		   //アニメーションフレームの合計
	float m_Speed;				   //アニメーション速度
	float m_Size;				   //ビルボードの大きさ
	ID3D11Buffer *m_pVertexBuffer; //頂点バッファー
	ImageInfo *m_pImageInfo;       //画像情報へのポインタ
	RenderingType m_RenderingType; //レンダリングの種類
	RenderingInfo m_Rendering;	   //レンダリング管理情報
	Vector2D *m_pLeftTopUV;		   //左上UV座標
	Vector2D *m_pLeftDwonUV;	   //左下UV座標
	Vector2D *m_pRightTopUV;	   //右上UV座標
	Vector2D *m_pRightDwonUV;	   //右下UV座標
	Vector3D m_Position;		   //ビルボードの座標
	Vector4D m_Color;			   //ビルボードの色
};

#endif