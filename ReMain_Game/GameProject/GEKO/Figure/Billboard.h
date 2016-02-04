#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "FiqureBase.h"

//ビルボードの頂点情報
struct VertexBillboardInfo
{
	//頂点座標
	Vector3D leftTopPos;	 //左上の座標
	Vector3D leftDwonPos;	 //左下の座標
	Vector3D rightTopPos;	 //右上の座標
	Vector3D rightDwonPos;	 //右下の座標

	//テクスチャUV座標
	Vector2D leftTopUV;	 //左上UV座標
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
	~Billboard();
	void ChangeVertex(const VertexBillboardInfo& info);
	void Render(const Vector3D &pos, float size, const std::string &name);

private:
	ID3D11Buffer* m_pVertexBuffer; //頂点バッファー

};

//ビルボードアニメーション
class BillboardAnimation
{
public:
	BillboardAnimation();
	BillboardAnimation(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	~BillboardAnimation();
	bool GetIsEnd() const;
	void FrameDivision(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	void PlayFrame(float frame);
	void Render(const Vector3D &pos, float size);
	void DebugFrame();

private:
	bool m_IsEnd;
	int m_FrameNum;
	int m_FrameAllNum;
	float m_Speed;
	Billboard *m_pBillboard;
	std::string m_Name;
};

#endif