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
	/// ”š‚ğ•`‰æ‚µ‚Ü‚·
	/// </summary>
	/// <param name="x">
	/// •`‰æ‚µ‚½‚¢¶ãXÀ•W
	/// </param>
	/// <param name="y">
	/// •`‰æ‚µ‚½‚¢¶ãYÀ•W
	/// </param>
	/// <param name="y">
	/// •`‰æ‚µ‚½‚¢”š
	/// </param>
	void NumDraw(int x, int y, int num);

private:
	Image m_Img;
	int m_Num;
	int m_x;
	int m_y;
};

#endif