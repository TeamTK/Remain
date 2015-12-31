#ifndef NUMBER_H
#define NUMBER_H

#define NUM_WIDTH 22
#define NUM_HEIGHT 32

#include "../../GEKO/GEKO.h"

class Number
{
public:
	Number();
	~Number();

	/// <summary>
	/// 数字を描画します
	/// </summary>
	/// <param name="x">
	/// 描画したい左上X座標
	/// </param>
	/// <param name="y">
	/// 描画したい左上Y座標
	/// </param>
	/// <param name="y">
	/// 描画したい数字
	/// </param>
	/*
	/// <param name="scroll">
	/// 桁が増えた時、1桁目を基準に左右どちらに桁を増やすか
	/// true = 右側に桁を増やす
	/// false = 左側に桁を増やす
	/// </param>
	*/
	void NumDraw(int x, int y, int num);

private:
	Image m_Img;
	int m_Num;
	int m_x;
	int m_y;
};

#endif