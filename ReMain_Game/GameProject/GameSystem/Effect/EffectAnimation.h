#ifndef _EFFECT_ANIMATION_H_
#define _EFFECT_ANIMATION_H_

#include "..\..\GEKO\GEKO.h"

//エフェクトアニメーションクラスの設定情報
struct EffectAnimationInfo
{
	Vector3D pos;   //位置
	int frameNum;  //アニメーション枚数
	int sizeW;		//一コマのサイズ（横）
	int sizeH;		//一コマのサイズ（縦）
	float size;		//全体のサイズ
	float speed;	//アニメーション切り替え速度
};

//ビルボードを使ったアニメーションエフェクト
class EffectAnimation : public Task
{
public:
	EffectAnimation(const std::string &assetName, const EffectAnimationInfo &info);
	~EffectAnimation();
	void Update();

private:
	BillboardAnimation m_Anim;
	Vector3D m_Pos;
	float m_Size;
	float m_Speed;
};

#endif