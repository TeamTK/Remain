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
	void NumDraw(const Vector2D pos, int number, const bool leftShift);

private:
	Image m_Img1;
	Image m_Img2;
	int m_Num;
	int m_x;
	int m_y;
	bool m_Digit2Avtive;
};

#endif